// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.root.fs;

import cmajor.systemx.kernel.host.file.manager;
import cmajor.systemx.kernel.kernel;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.block.file;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.dir.file;
import cmajor.systemx.kernel.os.api;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.config;
import cmajor.systemx.machine;
import util;
import std.filesystem;

namespace cmajor::systemx::kernel {

class RootFilesystemFile : public BlockFile
{
public:
    RootFilesystemFile(RootFilesystem* fs_, const std::string& name_, OpenFlags flags_, INodeKey inodeKey_, int32_t id_);
    void Close(cmajor::systemx::kernel::Process* process) override;
    bool IsReadable() const override { return (flags & OpenFlags::read) != OpenFlags::none; }
    bool IsWritable() const override { return (flags & OpenFlags::write) != OpenFlags::none; }
    bool IsConsole() const override { return false; }
    bool IsHostTextFile() const override { return false; }
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
    int32_t Id() const { return id; }
private:
    RootFilesystem* fs;
    OpenFlags flags;
    int32_t id;
};

RootFilesystemFile::RootFilesystemFile(RootFilesystem* fs_, const std::string& name_, OpenFlags flags_, INodeKey inodeKey_, int32_t id_) :
    BlockFile(name_, inodeKey_), fs(fs_), flags(flags_), id(id_)
{
}

void RootFilesystemFile::Close(cmajor::systemx::kernel::Process* process)
{
    INodePtr inodePtr = GetINode(process);
    INode* inode = inodePtr.Get();
    inode->DecrementReferenceCount();
    if (inode->ReferenceCount() == 0)
    {
        if (inode->NLinks() == 0)
        {
            FreeBlocks(inode, fs, process);
            FreeINode(inode, process);
        }
    }
    fs->CloseFile(id, process);
}

int32_t RootFilesystemFile::GetBlockNumber(INode* inode, cmajor::systemx::machine::Process* process, bool allocate) const
{
    int64_t filePos = FilePos();
    int32_t logicalBlockNumber = filePos / Block::Size();
    int32_t blockNumber = cmajor::systemx::kernel::MapBlockNumber(logicalBlockNumber, inode, fs, process);
    if (blockNumber == -1 && allocate)
    {
        cmajor::systemx::kernel::BlockPtr blockPtr = AllocateBlock(fs->Id(), process);
        cmajor::systemx::kernel::Block* block = blockPtr.Get();
        cmajor::systemx::kernel::SetBlockNumber(logicalBlockNumber, block->Key().blockNumber, inode, fs, process);
        blockNumber = block->Key().blockNumber;
    }
    return blockNumber;
}

INodePtr RootFilesystemFile::GetINode(cmajor::systemx::machine::Process* process)
{
    INodePtr inodePtr = cmajor::systemx::kernel::ReadINode(GetINodeKey(), process);
    INode* inode = inodePtr.Get();
    if (inode)
    {
        if ((flags & OpenFlags::append) != OpenFlags::none)
        {
            SetFilePos(inode->FileSize());
        }
    }
    return inodePtr;
}

int64_t RootFilesystemFile::Read(Block* block, cmajor::systemx::machine::Process* process)
{
    return fs->HostFile()->Read(block, process);
}

int64_t RootFilesystemFile::Write(Block* block, cmajor::systemx::machine::Process* process)
{
    return fs->HostFile()->Write(block, process);
}

class RootFilesystemDirFile : public DirFile
{
public:
    RootFilesystemDirFile(RootFilesystem* fs_, const std::string& name_, int32_t id_, const INodeKey& dirINodeKey_);
    void Close(cmajor::systemx::kernel::Process* process) override;
    int32_t Read(DirectoryEntry& dirEntry, cmajor::systemx::machine::Process* process) override;
    int32_t Id() const { return id; }
private:
    RootFilesystem* fs;
    int32_t id;
    INodeKey dirINodeKey;
    int32_t logicalBlockNumber;
    int32_t dirEntryIndex;
};

RootFilesystemDirFile::RootFilesystemDirFile(RootFilesystem* fs_, const std::string& name_, int32_t id_, const INodeKey& dirINodeKey_) :
    DirFile(name_), fs(fs_), id(id_), dirINodeKey(dirINodeKey_), logicalBlockNumber(0), dirEntryIndex(0)
{
}

void RootFilesystemDirFile::Close(cmajor::systemx::kernel::Process* process)
{
    fs->CloseFile(id, process);
}

int32_t RootFilesystemDirFile::Read(DirectoryEntry& dirEntry, cmajor::systemx::machine::Process* process)
{
    INodePtr dirINodePtr = cmajor::systemx::kernel::ReadINode(dirINodeKey, process);
    INode* dirINode = dirINodePtr.Get();
    if (dirINode->GetFileType() != FileType::directory)
    {
        throw SystemError(EFAIL, "not a directory inode", __FUNCTION__);
    }
    int32_t blockNumber = MapBlockNumber(logicalBlockNumber, dirINode, fs, process);
    while (blockNumber != -1)
    {
        DirectoryBlock directoryBlock;
        BlockPtr blockPtr = ReadDirectoryBlock(directoryBlock, blockNumber, fs, process);
        int32_t n = directoryBlock.Entries().size();
        if (dirEntryIndex < n)
        {
            dirEntry.Read(blockPtr.Get(), dirEntryIndex++);
            return 1;
        }
        dirEntryIndex = 0;
        ++logicalBlockNumber;
        blockNumber = MapBlockNumber(logicalBlockNumber, dirINode, fs, process);
    }
    return 0;
}

void MountHostDirectories(Filesystem* fs, cmajor::systemx::machine::Process* kernelProcess, int32_t mode);
void MountDeviceFilesystem(Filesystem* fs, cmajor::systemx::machine::Process* kernelProcess, int32_t mode);

RootFilesystem::RootFilesystem() : Filesystem(rootFSNumber), machine(nullptr), hostFs(nullptr), hostFile(nullptr), nextFileId(0)
{
}

void RootFilesystem::Initialize()
{
    hostFs = Kernel::Instance().GetMountTable().GetFilesystem(hostFSNumber);
    if (!RootFsExists())
    {
        int32_t mode = 0;
        File* file = hostFs->Create(RootFsHostFilePath(), nullptr, mode, Kernel::Instance().GetKernelProcess());
        hostFile = static_cast<BlockFile*>(file);
        MakeRootFs(*this);
    }
    else
    {
        OpenFlags flags = OpenFlags::read | OpenFlags::write | OpenFlags::random_access;
        int32_t mode = 0;
        File* file = hostFs->Open(RootFsHostFilePath(), nullptr, static_cast<int32_t>(flags), mode, Kernel::Instance().GetKernelProcess());
        hostFile = static_cast<BlockFile*>(file);
        MountHostDirectories(this, Kernel::Instance().GetKernelProcess(), mode);
        MountDeviceFilesystem(this, Kernel::Instance().GetKernelProcess(), mode);
    }
}

File* RootFilesystem::Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process)
{
    OpenFlags flags = OpenFlags::create | OpenFlags::truncate | OpenFlags::write;
    return Open(path, dirINode, static_cast<int32_t>(flags), mode, process);
}

File* RootFilesystem::Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process)
{
    if (path.empty() || path == "/")
    {
        throw SystemError(EPARAM, "could not open: path '" + path + "' not valid", __FUNCTION__);
    }
    std::string dirPath = util::Path::GetDirectoryName(path);
    if (dirPath.empty())
    {
        if (path.starts_with("/"))
        {
            dirPath = "/";
        }
        else
        {
            dirPath = ".";
        }
    }
    std::string fileName = util::Path::GetFileName(path);
    INodePtr dirINodePtr = PathToINode(dirPath, this, process);
    if (!dirINodePtr.Get())
    {
        throw SystemError(EFAIL, "could not open: directory '" + dirPath + "' not found", __FUNCTION__);
    }
    Filesystem* fs = GetFs(dirINodePtr.Get()->Key().fsNumber);
    if (fs != this)
    {
        File* file = fs->Open(fileName, dirINodePtr.Get(), flags, mode, process);
        return file;
    }
    INodePtr fileINode = cmajor::systemx::kernel::SearchDirectory(fileName, dirINodePtr.Get(), dirPath, this, process);
    OpenFlags openFlags = static_cast<OpenFlags>(flags);
    if ((openFlags & OpenFlags::create) != OpenFlags::none)
    {
        CheckAccess(Access::write, process->EUID(), process->EGID(), dirINodePtr.Get(), "could not open directory '" + dirPath + "' for writing");
    }
    bool truncated = false;
    if ((openFlags & OpenFlags::truncate) != OpenFlags::none)
    {
        if (fileINode.Get())
        {
            FreeBlocks(fileINode.Get(), this, process);
        }
        truncated = true;
    }
    if ((openFlags & OpenFlags::create) != OpenFlags::none)
    {
        if (fileINode.Get())
        {
            if (!truncated)
            {
                throw SystemError(EALREADYEXISTS, "could not open: path '" + path + "' already exists", __FUNCTION__);
            }
        }
        else
        {
            fileINode = AllocateINode(Id(), process);
            INode* inode = fileINode.Get();
            inode->SetFileType(FileType::regular);
            inode->SetUID(process->UID());
            inode->SetGID(process->GID());
            inode->SetMode(AlterMode(mode, process->UMask(), false));
            if ((openFlags & OpenFlags::read) != OpenFlags::none)
            {
                CheckAccess(Access::read, process->EUID(), process->EGID(), inode, "could not open file '" + path + "' for reading");
            }
            if ((openFlags & OpenFlags::write) != OpenFlags::none)
            {
                CheckAccess(Access::write, process->EUID(), process->EGID(), inode, "could not open file '" + path + "' for writing");
            }
            WriteINode(fileINode.Get(), process);
            DirectoryEntry entry;
            entry.SetName(fileName);
            entry.SetINodeNumber(fileINode.Get()->Key().inodeNumber);
            AddDirectoryEntry(entry, dirINodePtr.Get(), this, process);
        }
    }
    if (!fileINode.Get())
    {
        throw SystemError(ENOTFOUND, "could not open: path '" + path + "' not found", __FUNCTION__);
    }
    if ((openFlags & OpenFlags::read) != OpenFlags::none)
    {
        CheckAccess(Access::read, process->EUID(), process->EGID(), fileINode.Get(), "could not open file '" + path + "' for reading");
    }
    if ((openFlags & OpenFlags::write) != OpenFlags::none)
    {
        CheckAccess(Access::write, process->EUID(), process->EGID(), fileINode.Get(), "could not open file '" + path + "' for writing");
    }
    fileINode.Get()->IncrementReferenceCount();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, process->Id(), NO_LOCK | OPEN);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, process->Id(), HAS_LOCK | OPEN);
#endif 
    RootFilesystemFile* file = new RootFilesystemFile(this, path, openFlags, fileINode.Get()->Key(), nextFileId++);
    fileMap[file->Id()] = file;
    return file;
}

INodePtr RootFilesystem::SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process)
{
    CheckAccess(Access::execute, process->EUID(), process->EGID(), dirINode, "could not search directory '" + dirPath + "'");
    return cmajor::systemx::kernel::SearchDirectory(name, dirINode, dirPath, this, process);
}

void RootFilesystem::Stat(INode* inode, cmajor::systemx::machine::Process* process)
{
    // status already obtained
}

DirFile* RootFilesystem::OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, process->Id(), NO_LOCK | OPEN_DIR);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, process->Id(), HAS_LOCK | OPEN_DIR);
#endif 
    RootFilesystemDirFile* dirFile = new RootFilesystemDirFile(this, path, nextFileId++, dirINode->Key());
    fileMap[dirFile->Id()] = dirFile;
    return dirFile;
}

void RootFilesystem::MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode)
{
    INodePtr dirINodePtr = AllocateINode(Id(), process);
    INode* dirINode = dirINodePtr.Get();
    dirINode->SetFileType(FileType::directory);
    dirINode->SetUID(process->UID());
    dirINode->SetGID(process->GID());
    dirINode->SetMode(AlterMode(mode, process->UMask(), true));
    BlockPtr dirBlockPtr = AllocateBlock(Id(), process);
    dirINode->SetDirectBlockNumber(dirBlockPtr.Get()->Key().blockNumber, 0);
    DirectoryBlock dirBlock;
    DirectoryEntry thisEntry;
    thisEntry.SetINodeNumber(dirINode->Key().inodeNumber);
    thisEntry.SetName(".");
    dirBlock.AddEntry(thisEntry);
    DirectoryEntry parentEntry;
    parentEntry.SetINodeNumber(parentDirINode->Key().inodeNumber);
    parentEntry.SetName("..");
    dirBlock.AddEntry(parentEntry);
    dirINode->SetFileSize(2 * DirectoryEntry::Size());
    dirINode->SetCTime(util::GetCurrentDateTime());
    dirINode->SetMTime(util::GetCurrentDateTime());
    WriteDirectoryBlock(dirBlock, dirBlockPtr, this, process);
    WriteINode(dirINode, process);
    DirectoryEntry entry;
    entry.SetName(dirName);
    entry.SetINodeNumber(dirINode->Key().inodeNumber);
    AddDirectoryEntry(entry, parentDirINode, this, process);
}

void RootFilesystem::CloseFile(int32_t id, cmajor::systemx::kernel::Process* process)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, 0, NO_LOCK | CLOSE_FILE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), ROOT_FILE_SYSTEM, 0, HAS_LOCK | CLOSE_FILE);
#endif 
    auto it = fileMap.find(id);
    if (it != fileMap.cend())
    {
        File* file = it->second;
        fileMap.erase(id);
        file->Release(process);
    }
}

INodePtr RootFilesystem::ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process)
{
    return cmajor::systemx::kernel::ReadINode(inodeKey, process);
}

void RootFilesystem::WriteINode(INode* inode, cmajor::systemx::machine::Process* process)
{
    SuperBlock superBlock;
    ReadSuperBlock(superBlock, this, process);
    int32_t inodeBlockNumber = superBlock.GetINodeBlockNumber(inode->Key().inodeNumber);
    INodeBlock inodeBlock;
    BlockPtr inb = ReadINodeBlock(inodeBlock, inodeBlockNumber, this, process);
    int32_t inodeIndex = superBlock.GetINodeIndexInBlock(inode->Key().inodeNumber);
    inode->SetValid();
    inodeBlock.SetINode(*inode, inodeIndex);
    WriteINodeBlock(inodeBlock, inb, this, process);
}

std::string RootFilesystem::INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process)
{
    INodePtr dirINodePtr = ReadINode(inodeKey, process);
    if (dirINodePtr.Get())
    {
        INodePtr parentDirINodePtr = cmajor::systemx::kernel::SearchDirectory("..", dirINodePtr.Get(), "..", this, process);
        if (parentDirINodePtr.Get())
        {
            std::string path;
            while (dirINodePtr.Get()->Key() != parentDirINodePtr.Get()->Key())
            {
                DirectoryEntry entry = GetDirectoryEntry(parentDirINodePtr.Get(), dirINodePtr.Get()->Key().inodeNumber, this, process);
                if (!entry.IsFree())
                {
                    path = util::Path::Combine(entry.Name(), path);
                }
                else
                {
                    throw SystemError(EFAIL, "parent directory entry not found", __FUNCTION__);
                }
                INodePtr grandParentDirINodePtr = cmajor::systemx::kernel::SearchDirectory("..", parentDirINodePtr.Get(), "..", this, process);
                if (!grandParentDirINodePtr.Get())
                {
                    throw SystemError(EFAIL, "grand parent directory not found", __FUNCTION__);
                }
                dirINodePtr = std::move(parentDirINodePtr);
                parentDirINodePtr = std::move(grandParentDirINodePtr);
            }
            path = util::Path::Combine("/", path);
            return path;
        }
        else
        {
            throw SystemError(EFAIL, "parent directory not found from current directory", __FUNCTION__);
        }
    }
    else
    {
        throw SystemError(EFAIL, "current directory not found from process", __FUNCTION__);
    }
}

std::string RootFsHostFilePath()
{
    std::string cmajorRootDir = util::GetFullPath(util::CmajorRoot());
    std::string rootFsHostFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajorRootDir, "system-x"), "fs"), "root.fs"));
    return rootFsHostFilePath;
}

bool RootFsExists()
{
    std::string rootFsHostFilePath = RootFsHostFilePath();
    return std::filesystem::exists(rootFsHostFilePath);
}

void MakeSuperBlock(SuperBlock& superBlock, BlockFile* rootHostFile, cmajor::systemx::machine::Process* kernelProcess)
{
    BlockPtr superBlockPtr = GetBlock(SuperBlockKey(rootFSNumber), kernelProcess);
    superBlock.SetNumBlocks(cmajor::systemx::machine::RootFSNumBlocks());
    superBlock.SetNumINodes(cmajor::systemx::machine::RootFSMaxFiles());
    int32_t firstBlockNumberBlockNumber = 1;
    superBlock.SetFirstBlockNumberBlockNumber(firstBlockNumberBlockNumber);
    superBlock.SetCurrentBlockNumberBlockNumber(firstBlockNumberBlockNumber);
    int32_t numBlockNumberBlocks = superBlock.NumberOfBlockNumberBlocks();
    int32_t firstINodeBlockNumber = firstBlockNumberBlockNumber + numBlockNumberBlocks;
    superBlock.SetFirstINodeBlockNumber(firstINodeBlockNumber);
    superBlock.SetCurrentINodeBlockNumber(firstINodeBlockNumber);
    int32_t numINodeBlocks = superBlock.NumberOfINodeBlocks();
    int32_t firstDataBlockNumber = firstINodeBlockNumber + numINodeBlocks;
    superBlock.SetFirstDataBlockNumber(firstDataBlockNumber);
    superBlock.Write(superBlockPtr.Get());
    rootHostFile->Write(superBlockPtr.Get(), kernelProcess);
}

void MakeBlockNumberBlocks(const SuperBlock& superBlock, BlockFile* rootHostFile, cmajor::systemx::machine::Process* kernelProcess)
{
    int32_t dataBlockNumber = superBlock.FirstDataBlockNumber();
    int32_t numberOfDataBlocksLeft = superBlock.NumberOfDataBlocks();
    int32_t n = superBlock.NumberOfBlockNumberBlocks();
    for (int32_t i = 0; i < n; ++i)
    {
        int32_t blockNumberBlockNumber = superBlock.FirstBlockNumberBlockNumber() + i;
        BlockKey blockNumberBlockKey(rootFSNumber, blockNumberBlockNumber);
        BlockPtr blockNumberBlockPtr = GetBlock(blockNumberBlockKey, kernelProcess);
        int32_t count = std::min(numberOfDataBlocksLeft, BlockNumberBlock::BlockNumberCount());
        BlockNumberBlock blockNumberBlock;
        for (int32_t i = 0; i < count; ++i)
        {
            blockNumberBlock.SetBlockNumber(dataBlockNumber + i, i);
        }
        blockNumberBlock.Write(blockNumberBlockPtr.Get());
        rootHostFile->Write(blockNumberBlockPtr.Get(), kernelProcess);
        dataBlockNumber += count;
        numberOfDataBlocksLeft -= count;
    }
}

void MakeINodeBlocks(const SuperBlock& superBlock, BlockFile* rootHostFile, cmajor::systemx::machine::Process* kernelProcess)
{
    int32_t n = superBlock.NumberOfINodeBlocks();
    int32_t numINodesLeft = superBlock.NumINodes();
    for (int32_t i = 0; i < n; ++i)
    {
        int32_t inodeBlockNumber = superBlock.FirstINodeBlockNumber() + i;
        BlockKey inodeBlockKey(rootFSNumber, inodeBlockNumber);
        BlockPtr inodeBlockPtr = GetBlock(inodeBlockKey, kernelProcess);
        int32_t count = std::min(numINodesLeft, numINodesInBlock);
        INodeBlock inodeBlock;
        for (int i = 0; i < count; ++i)
        {
            INode inode;
            inodeBlock.SetINode(inode, i);
        }
        inodeBlock.Write(inodeBlockPtr.Get());
        rootHostFile->Write(inodeBlockPtr.Get(), kernelProcess);
        numINodesLeft -= count;
    }
}

void MakeRootDirectory(cmajor::systemx::machine::Process* process, int32_t mode)
{
    Filesystem* rootFs = GetFs(rootFSNumber);
    SuperBlock superBlock;
    ReadSuperBlock(superBlock, rootFs, process);
    if (superBlock.INodeNumberOfRootDirectory() != -1)
    {
        throw SystemError(EFAIL, "root directory of root filesystem already created", __FUNCTION__);
    }
    INodePtr rootDirINodePtr = AllocateINode(rootFSNumber, process);
    INode* rootDirINode = rootDirINodePtr.Get();
    rootDirINode->SetFileType(FileType::directory);
    rootDirINode->SetMode(AlterMode(mode, process->UMask(), true));
    superBlock.SetINodeNumberOfRootDirectory(rootDirINode->Key().inodeNumber);
    process->SetINodeKeyOfWorkingDir(ToULong(rootDirINode->Key()));
    BlockPtr rootDirBlockPtr = AllocateBlock(rootFSNumber, process);
    rootDirINode->SetDirectBlockNumber(rootDirBlockPtr.Get()->Key().blockNumber, 0);
    DirectoryBlock rootDirBlock;
    DirectoryEntry thisEntry;
    thisEntry.SetINodeNumber(rootDirINode->Key().inodeNumber);
    thisEntry.SetName(".");
    rootDirBlock.AddEntry(thisEntry);
    DirectoryEntry parentEntry;
    parentEntry.SetINodeNumber(rootDirINode->Key().inodeNumber);
    parentEntry.SetName("..");
    rootDirBlock.AddEntry(parentEntry);
    rootDirINode->SetFileSize(2 * DirectoryEntry::Size());
    WriteDirectoryBlock(rootDirBlock, rootDirBlockPtr, rootFs, process);
    WriteSuperBlock(superBlock, rootFs, process);
    WriteINode(rootDirINode, process);
}

void MountHostDirectories(Filesystem* fs, cmajor::systemx::machine::Process* kernelProcess, int32_t mode)
{
    std::string mountDirPath = "/mnt";
    if (!DirectoryExists(mountDirPath, fs, kernelProcess))
    {
        MakeDirectory(mountDirPath, fs, kernelProcess, mode);
    }
    std::string driveStr = OsGetLogicalDrives();
    std::vector<std::string> drives = util::Split(driveStr, ';');
    for (const std::string& drive : drives)
    {
        uint32_t driveType = OsGetDriveType(drive.c_str());
        if (driveType == fixedDriveType)
        {
            std::string hostPath = util::GetFullPath(drive);
            if (Config::Instance().IncludedInMountRootDirs(hostPath))
            {
                std::string driveMountDirPath = "/mnt/" + std::string(1, std::tolower(hostPath[0]));
                cmajor::systemx::kernel::MountHostDir(hostPath, driveMountDirPath, kernelProcess, mode);
                std::string driveStr(1, hostPath[0]);
                driveStr.append(1, ':');
                cmajor::systemx::kernel::MapDrive(driveStr, driveMountDirPath);
            }
        }
    }
    std::string cmajorRootPath = util::GetFullPath(util::CmajorRoot());
    std::string cmajorMountDirPath = "/mnt/cmajor";
    cmajor::systemx::kernel::MountHostDir(cmajorRootPath, cmajorMountDirPath, kernelProcess, mode);
    std::string sxRootPath = util::GetFullPath(util::Path::Combine(util::CmajorRoot(), "system-x"));
    std::string sxMountDirPath = "/mnt/sx";
    cmajor::systemx::kernel::MountHostDir(sxRootPath, sxMountDirPath, kernelProcess, mode);
}

void MountDeviceFilesystem(Filesystem* fs, cmajor::systemx::machine::Process* kernelProcess, int32_t mode)
{
    cmajor::systemx::kernel::MountDevDir("/dev", kernelProcess, mode);
}

void MakeRootFs(RootFilesystem& rootFs)
{
    BlockFile* rootHostFile = rootFs.HostFile();
    std::string directoryName = util::Path::GetDirectoryName(rootHostFile->Name());
    std::error_code ec;
    std::filesystem::create_directories(directoryName, ec);
    if (ec)
    {
        std::string errorMessage = util::PlatformStringToUtf8(ec.message());
        throw SystemError(EFAIL, errorMessage, __FUNCTION__);
    }
    cmajor::systemx::machine::Process* kernelProcess = Kernel::Instance().GetKernelProcess();
    SuperBlock superBlock;
    MakeSuperBlock(superBlock, rootHostFile, kernelProcess);
    MakeBlockNumberBlocks(superBlock, rootHostFile, kernelProcess);
    MakeINodeBlocks(superBlock, rootHostFile, kernelProcess);
    int32_t mode = 0;
    MakeRootDirectory(kernelProcess, mode);
    MountHostDirectories(&rootFs, kernelProcess, mode);
    MountDeviceFilesystem(&rootFs, kernelProcess, mode);
}

} // namespace cmajor::systemx::kernel
