// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.mount;

import cmajor.systemx.kernel.inode.manager;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Filesystem;

const int32_t rootFSNumber = 0;
const int32_t hostFSNumber = 1;
const int32_t devFSNumber = 2;

class MountTable
{
public:
    MountTable(cmajor::systemx::machine::Machine* machine_);
    MountTable(const MountTable&) = delete;
    MountTable& operator=(const MountTable&) = delete;
    void AddFilesystem(Filesystem* filesystem);
    Filesystem* GetFilesystem(int32_t fsNumber) const;
    Filesystem* GetMountedFilesystem(INodeKey mountPoint) const;
    int32_t NextFileSystemId() { return nextFileSystemId++; }
    void MapDrive(const std::string& drive, const std::string& mountedPath);
    std::string GetMappedDrivePrefix(std::string& driveStr) const;
    std::string MapPath(const std::string& windowsFilePath) const;
private:
    int32_t nextFileSystemId;
    cmajor::systemx::machine::Machine* machine;
    std::vector<std::unique_ptr<Filesystem>> filesystems;
    std::map<int32_t, Filesystem*> filesystemMap;
    std::map<std::string, std::string> driveMap;
};

void MountHostDir(const std::string& hostPath, const std::string& dirPath, cmajor::systemx::machine::Process* process, int32_t mode);
void MountDevDir(const std::string& dirPath, cmajor::systemx::machine::Process* process, int32_t mode);
void MapDrive(const std::string& drive, const std::string& mountedPath);
std::string MapPath(const std::string& windowsFilePath);

} // namespace cmajor::systemx::kernel
