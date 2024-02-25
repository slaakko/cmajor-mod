// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.host.fs;

import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.host.file.manager;
import cmajor.systemx.kernel.io.manager;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.block.file;
import cmajor.systemx.kernel.dir.file;
import cmajor.systemx.kernel.kernel;
import cmajor.systemx.kernel.os.api;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.machine;
import std.filesystem;
import util;

namespace cmajor::systemx::kernel {

class HostFilesystemFile : public BlockFile
{
public:
    HostFilesystemFile(HostFilesystem* fs_, int32_t fileId_, const std::string& filePath, OpenFlags flags_, INodeKey inodeKey_);
    void Close(cmajor::systemx::kernel::Process* process) override;
    bool IsReadable() const override { return (flags & OpenFlags::read) != OpenFlags::none; }
    bool IsWritable() const override { return (flags & OpenFlags::write) != OpenFlags::none; }
    bool IsConsole() const override { return false; }
    bool IsHostTextFile() const override { return (flags & OpenFlags::text) != OpenFlags::none; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return false; }
    bool HasColors() const override { return false; }
    int Columns() const override { return 0; }
    int Rows() const override { return 0; }
    int CursorX() const override { return 0; }
    int CursorY() const override { return 0; }
    int32_t GetBlockNumber(INode* inode, cmajor::systemx::machine::Process* process, bool allocate) const override;
    INodePtr GetINode(cmajor::systemx::machine::Process* process) override;
    int64_t Read(Block* block, cmajor::systemx::machine::Process* process) override;
    int64_t Write(Block* block, cmajor::systemx::machine::Process* process) override;
private:
    HostFilesystem* fs;
    int32_t fileId;
    OpenFlags flags;
    int32_t hostFileId;
};

HostFilesystemFile::HostFilesystemFile(HostFilesystem* fs_, int32_t fileId_, const std::string& filePath, OpenFlags flags_, INodeKey inodeKey_) :
    BlockFile(filePath, inodeKey_), fs(fs_), fileId(fileId_), flags(flags_), hostFileId(OpenHostFile(filePath, flags))
{
}

void HostFilesystemFile::Close(cmajor::systemx::kernel::Process* process)
{
    INodePtr inode = GetINode(process);
    int64_t fileSize = inode.Get()->FileSize();
    int64_t internalFileSize = inode.Get()->InternalFileSize();
    HostFile* hostFile = GetHostFile(hostFileId);
    std::string filePath = hostFile->FilePath();
    CloseHostFile(hostFileId);
    fs->Close(fileId, inode.Get(), process);
    std::error_code ec;
    if (fileSize != 0 && internalFileSize != 0 && internalFileSize != fileSize)
    {
        std::filesystem::resize_file(filePath, inode.Get()->InternalFileSize(), ec);
        if (ec)
        {
            throw SystemError(EHOST, "could not resize host file system file '" + Name() + "': " + util::PlatformStringToUtf8(ec.message()), __FUNCTION__);
        }
    }
    if (ec)
    {
        throw SystemError(EHOST, "could not get size of host file system file '" + Name() + "': " + util::PlatformStringToUtf8(ec.message()), __FUNCTION__);
    }
}

int32_t HostFilesystemFile::GetBlockNumber(INode* inode, cmajor::systemx::machine::Process* process, bool allocate) const
{
    int64_t filePos = FilePos();
    return static_cast<int32_t>(filePos / Block::Size());
}

INodePtr HostFilesystemFile::GetINode(cmajor::systemx::machine::Process* process)
{
    INodePtr inode = cmajor::systemx::kernel::GetINode(GetINodeKey(), process);
    if (std::filesystem::exists(Name()))
    {
        inode.Get()->SetFileSize(std::filesystem::file_size(Name()));
    }
    if ((flags & OpenFlags::append) != OpenFlags::none)
    {
        SetFilePos(inode.Get()->FileSize());
    }
    return inode;
}

int64_t HostFilesystemFile::Read(Block* block, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&fs->GetMachine()->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | READ);
#endif 
    std::unique_lock<std::recursive_mutex> lock(fs->GetMachine()->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&fs->GetMachine()->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | READ);
#endif 
    int32_t requestId = cmajor::systemx::kernel::Read(hostFileId, block);
    cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::ioEvent, requestId);
    Sleep(evnt, process, lock);
    lock.lock();
    IORequest* request = GetRequest(requestId);
    if (request->Failed())
    {
        SystemError error = request->Error();
        DeleteRequest(requestId);
        throw error;
    }
    else
    {
        block->SetValid();
        int64_t bytesTransferred = request->BytesTransferred();
        DeleteRequest(requestId);
        return bytesTransferred;
    }
}

int64_t HostFilesystemFile::Write(Block* block, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&fs->GetMachine()->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | WRITE);
#endif 
    std::unique_lock<std::recursive_mutex> lock(fs->GetMachine()->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&fs->GetMachine()->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | WRITE);
#endif 
    int32_t requestId = cmajor::systemx::kernel::Write(hostFileId, block);
    cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::ioEvent, requestId);
    Sleep(evnt, process, lock);
    lock.lock();
    IORequest* request = GetRequest(requestId);
    if (request->Failed())
    {
        SystemError error = request->Error();
        DeleteRequest(requestId);
        throw error;
    }
    else
    {
        block->ResetDirty();
        int64_t bytesTransferred = request->BytesTransferred();
        DeleteRequest(requestId);
        return bytesTransferred;
    }
}

class HostFilesystemDirFile : public DirFile
{
public:
    HostFilesystemDirFile(HostFilesystem* fs_, const std::string& name_, int32_t id_);
    void Close(cmajor::systemx::kernel::Process* process) override;
    int32_t Read(DirectoryEntry& dirEntry, cmajor::systemx::machine::Process* process) override;
    int32_t Id() const { return id; }
private:
    HostFilesystem* fs;
    int32_t id;
    int32_t dirEntryIndex;
    void* searchHandle;
};

HostFilesystemDirFile::HostFilesystemDirFile(HostFilesystem* fs_, const std::string& name_, int32_t id_) : DirFile(name_), fs(fs_), id(id_), dirEntryIndex(0), searchHandle(nullptr)
{
}

void HostFilesystemDirFile::Close(cmajor::systemx::kernel::Process* process)
{
    fs->CloseDir(id);
}

int32_t HostFilesystemDirFile::Read(DirectoryEntry& dirEntry, cmajor::systemx::machine::Process* process)
{
    std::string entryStr;
    if (searchHandle == nullptr)
    {
        if (OsFindFirstFile(Name(), entryStr, searchHandle))
        {
            dirEntry.SetINodeNumber(-1);
            dirEntry.SetNameTruncate(entryStr);
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (OsFindNextFile(searchHandle, entryStr))
        {
            dirEntry.SetINodeNumber(-1);
            dirEntry.SetNameTruncate(entryStr);
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

HostFilesystem::HostFilesystem(int32_t id_, const std::string& prefix_) : Filesystem(id_), nextINodeId(0), nextDirId(0), machine(nullptr), prefix(prefix_)
{
}

void HostFilesystem::Initialize()
{
}

File* HostFilesystem::Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process)
{
    OpenFlags openFlags = OpenFlags::create | OpenFlags::truncate | OpenFlags::write | OpenFlags::random_access;
    return Open(path, dirINode, static_cast<int32_t>(openFlags), mode, process);
}

File* HostFilesystem::Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | OPEN);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | OPEN);
#endif 
    std::string fullPath;
    if (dirINode)
    {
        auto it = data.inodePathMap.find(dirINode->Key().inodeNumber);
        if (it != data.inodePathMap.cend())
        {
            fullPath = util::Path::Combine(it->second, path);
        }
        else
        {
            throw SystemError(ENOTFOUND, "path '" + path + "' not found from host file system '" + prefix + "'", __FUNCTION__);
        }
    }
    else
    {
        fullPath = util::Path::Combine(prefix, path);
    }
    OpenFlags openFlags = static_cast<OpenFlags>(flags);
    if ((openFlags & OpenFlags::truncate) != OpenFlags::none)
    {
        if (std::filesystem::exists(fullPath))
        {
            std::error_code ec;
            std::filesystem::remove(fullPath, ec);
            if (ec)
            {
                throw SystemError(EFAIL, "could not remove file '" + fullPath + "' from host file system: " + util::PlatformStringToUtf8(ec.message()), __FUNCTION__);
            }
        }
    }
    int32_t fileId = nextINodeId++;
    INodeKey inodeKey(Id(), fileId);
    INodePtr inodePtr = GetINode(inodeKey, process);
    INode* inode = inodePtr.Get();
    inode->IncrementReferenceCount();
    inode->SetValid();
    data.inodePathMap[inode->Key().inodeNumber] = fullPath;
    HostFilesystemFile* file = new HostFilesystemFile(this, fileId, fullPath, openFlags, inodeKey);
    fileMap[fileId] = file;
    return file;
}

INodePtr HostFilesystem::SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | SEARCH_DIRECTORY);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | SEARCH_DIRECTORY);
#endif 
    std::string fullPath;
    std::string partialPath;
    if (dirINode->Key() == mountPoint)
    {
        fullPath = util::GetFullPath(util::Path::Combine(prefix, name));
        partialPath = ".";
    }
    else if (dirINode->Key().fsNumber == Id())
    {
        auto it = data.inodePathMap.find(dirINode->Key().inodeNumber);
        if (it != data.inodePathMap.cend())
        {
            fullPath = util::Path::Combine(it->second, name);
        }
        auto it2 = data.inodePartialPathMap.find(dirINode->Key().inodeNumber);
        if (it2 != data.inodePartialPathMap.cend())
        {
            partialPath = util::Path::Combine(it2->second, name);
        }
    }
    if (std::filesystem::exists(fullPath))
    {
        INodeKey inodeKey(Id(), nextINodeId++);
        data.inodePathMap[inodeKey.inodeNumber] = fullPath;
        INodePtr inode = GetINode(inodeKey, process);
        Stat(inode.Get(), process);
        if (inode.Get()->GetFileType() == FileType::directory)
        {
            data.inodePartialPathMap[inodeKey.inodeNumber] = partialPath;
        }
        return inode;
    }
    else
    {
        return INodePtr(nullptr);
    }
}

void HostFilesystem::Stat(INode* inode, cmajor::systemx::machine::Process* process)
{
    auto it = data.inodePathMap.find(inode->Key().inodeNumber);
    if (it != data.inodePathMap.cend())
    {
        std::string fullPath = it->second;
        std::error_code ec;
        std::filesystem::file_status status = std::filesystem::status(fullPath, ec);
        cmajor::systemx::kernel::Access access = static_cast<cmajor::systemx::kernel::Access>(cmajor::systemx::kernel::Access::read | kernel::Access::write);
        std::string ext = util::Path::GetExtension(fullPath);
        if (ext == ".exe")
        {
            access = static_cast<cmajor::systemx::kernel::Access>(access | cmajor::systemx::kernel::Access::execute);
        }
        if (status.type() == std::filesystem::file_type::regular || status.type() == std::filesystem::file_type::not_found)
        {
            inode->SetFileType(FileType::regular);
            if (status.type() != std::filesystem::file_type::not_found)
            {
                uintmax_t size = std::filesystem::file_size(fullPath, ec);
                if (!ec)
                {
                    inode->SetFileSize(size);
                }
                else
                {
                    inode->SetFileSize(0);
                }
            }
            else
            {
                inode->SetFileSize(0);
            }
        }
        else if (status.type() == std::filesystem::file_type::directory)
        {
            inode->SetFileType(FileType::directory);
            access = static_cast<cmajor::systemx::kernel::Access>(access | cmajor::systemx::kernel::Access::execute);
        }
        inode->SetOwnerAccess(access);
        inode->SetGroupAccess(access);
        inode->SetOtherAccess(access);
        inode->SetNLinks(1);
        if (inode->GetFileType() != FileType::directory)
        {
            std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(fullPath, ec);
            if (!ec)
            {
                util::DateTime mtime = util::ToFileDatetime(lastWriteTime);
                inode->SetMTime(mtime);
            }
        }
    }
    else
    {
        throw SystemError(ENOTFOUND, "path not found", __FUNCTION__);
    }
}

void HostFilesystem::Close(int32_t fileId, INode* inode, cmajor::systemx::kernel::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, NO_LOCK | CLOSE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, HAS_LOCK | CLOSE);
#endif 
    auto it = fileMap.find(fileId);
    if (it != fileMap.cend())
    {
        File* file = it->second;
        inode->DecrementReferenceCount();
        if (inode->ReferenceCount() == 0)
        {
            fileMap.erase(fileId);
            file->Release(process);
        }
    }
}

BlockFile* HostFilesystem::HostFile() const
{
    throw SystemError(EFAIL, "host filesystem does not provide a host file", __FUNCTION__);
}

DirFile* HostFilesystem::OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, NO_LOCK | OPEN_DIR);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, HAS_LOCK | OPEN_DIR);
#endif 
    auto it = data.inodePathMap.find(dirINode->Key().inodeNumber);
    if (it != data.inodePathMap.cend())
    {
        std::string fullPath = it->second;
        HostFilesystemDirFile* dirFile = new HostFilesystemDirFile(this, fullPath, nextDirId++);
        dirFileMap[dirFile->Id()] = dirFile;
        return dirFile;
    }
    else
    {
        throw SystemError(ENOTFOUND, "path not found from inode path map with inode number " + std::to_string(dirINode->Key().inodeNumber), __FUNCTION__);
    }
}

void HostFilesystem::MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, NO_LOCK | MK_DIR);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, HAS_LOCK | MK_DIR);
#endif 
    auto it = data.inodePathMap.find(parentDirINode->Key().inodeNumber);
    if (it != data.inodePathMap.cend())
    {
        std::string fullPath = util::Path::Combine(it->second, dirName);
        std::error_code ec;
        std::filesystem::create_directory(fullPath, ec);
        if (ec)
        {
            throw SystemError(EHOST, "could not create host directory '" + fullPath + "': " + util::PlatformStringToUtf8(ec.message()), __FUNCTION__);
        }
    }
    else
    {
        throw SystemError(ENOTFOUND, "path not found from inode path map with inode number " + std::to_string(parentDirINode->Key().inodeNumber), __FUNCTION__);
    }
}

void HostFilesystem::CloseDir(int32_t dirId)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, NO_LOCK | CLOSE_DIR);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, 0, HAS_LOCK | CLOSE_DIR);
#endif 
    auto it = dirFileMap.find(dirId);
    if (it != dirFileMap.cend())
    {
        DirFile* dir = it->second;
        dirFileMap.erase(dirId);
        delete dir;
    }
}

std::string HostFilesystem::GetHostFilePath(int32_t inodeNumber, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | GET_HOST_FILE_PATH);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | GET_HOST_FILE_PATH);
#endif 
    auto iti = data.inodePathMap.find(inodeNumber);
    if (iti != data.inodePathMap.cend())
    {
        std::string fullPath = iti->second;
        return fullPath;
    }
    else
    {
        return std::string();
    }
}

INodePtr HostFilesystem::ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), NO_LOCK | READ_INODE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_SYSTEM, process->Id(), HAS_LOCK | READ_INODE);
#endif 
    auto it = data.inodePathMap.find(inodeKey.inodeNumber);
    if (it != data.inodePathMap.cend())
    {
        INodePtr inode = GetINode(inodeKey, process);
        Stat(inode.Get(), process);
        return inode;
    }
    else
    {
        throw SystemError(EFAIL, "host file system could not retrieve inode", __FUNCTION__);
    }
}

void HostFilesystem::WriteINode(INode* inode, cmajor::systemx::machine::Process* process)
{
    // TODO
/*
    auto it = data.inodePathMap.find(inode->Key().inodeNumber);
    if (it != data.inodePathMap.cend())
    {
        std::string fullPath = it->second;
        util::DateTime dt = inode->MTime();
        if (dt != util::DateTime())
        {
            time_t time = MkTime(dt);
            if (time != -1)
            {
                std::error_code ec;
                std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
                auto t = std::chrono::clock_cast<std::chrono::file_clock>(tp);
                std::filesystem::last_write_time(fullPath, t, ec);
                if (ec)
                {
                    throw SystemError(EHOST, "host file system could not set last write time", __FUNCTION__);
                }
            }
        }
    }
*/
}

std::string HostFilesystem::INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process)
{
    auto it = data.inodePartialPathMap.find(inodeKey.inodeNumber);
    if (it != data.inodePartialPathMap.cend())
    {
        std::string inodePath = it->second;
        Filesystem* rootFs = GetFs(mountPoint.fsNumber);
        std::string mountPointPath = rootFs->INodeToPath(mountPoint, process);
        return util::GetFullPath(util::Path::Combine(mountPointPath, inodePath));
    }
    else
    {
        throw SystemError(EFAIL, "host file system could not retrieve path from inode", __FUNCTION__);
    }
}

} // namespace cmajor::systemx::kernel
