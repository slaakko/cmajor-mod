// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.host.fs;

import cmajor.systemx.kernel.fs;
import std.core;

export namespace cmajor::systemx::kernel {

struct HostFilesystemData
{
    std::map<int32_t, std::string> inodePathMap;
    std::map<int32_t, std::string> inodePartialPathMap;
};

class HostFilesystem : public Filesystem
{
public:
    HostFilesystem(int32_t id_, const std::string& prefix_);
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override { machine = machine_; }
    cmajor::systemx::machine::Machine* GetMachine() const { return machine; }
    void Initialize() override;
    File* Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process) override;
    File* Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process) override;
    INodePtr SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process) override;
    void Stat(INode* inode, cmajor::systemx::machine::Process* process) override;
    void Close(int32_t fileId, INode* inode, cmajor::systemx::kernel::Process* process);
    BlockFile* HostFile() const override;
    INodeKey MountPoint() const override { return mountPoint; }
    void SetMountPoint(INodeKey mountPoint_) override { mountPoint = mountPoint_; }
    DirFile* OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process) override;
    void MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode) override;
    void CloseDir(int32_t dirId);
    std::string GetHostFilePath(int32_t inodeNumber, cmajor::systemx::machine::Process* process) override;
    INodePtr ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
    void WriteINode(INode* inode, cmajor::systemx::machine::Process* process) override;
    std::string INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
private:
    cmajor::systemx::machine::Machine* machine;
    int32_t nextINodeId;
    int32_t nextDirId;
    HostFilesystemData data;
    std::map<int32_t, BlockFile*> fileMap;
    std::map<int32_t, DirFile*> dirFileMap;
    std::string prefix;
    INodeKey mountPoint;
};

} // namespace cmajor::systemx::kernel
