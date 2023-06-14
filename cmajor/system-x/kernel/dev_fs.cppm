// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.dev.fs;

import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.dir.file;
import std.core;

export namespace cmajor::systemx::kernel {

class DevFilesystem : public Filesystem
{
public:
    DevFilesystem();
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override { machine = machine_; }
    void Initialize() override;
    File* Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process) override;
    File* Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process) override;
    INodePtr SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process) override;
    void Stat(INode* inode, cmajor::systemx::machine::Process* process) override;
    BlockFile* HostFile() const override;
    INodeKey MountPoint() const override { return mountPoint; }
    void SetMountPoint(INodeKey mountPoint_) override { mountPoint = mountPoint_; }
    DirFile* OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process) override;
    void MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode) override;
    std::string GetHostFilePath(int32_t inodeNumber, cmajor::systemx::machine::Process* process) override;
    INodePtr ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
    void WriteINode(INode* inode, cmajor::systemx::machine::Process* process) override;
    std::string INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
private:
    cmajor::systemx::machine::Machine* machine;
    INodeKey mountPoint;
    INodeKey devINodeKey;
    std::unique_ptr<DirFile> devDirFile;
};

} // namespace cmajor::systemx::kernel
