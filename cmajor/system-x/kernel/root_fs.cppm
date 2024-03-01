// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.root.fs;

import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.file;
import std.core;

export namespace cmajor::systemx::kernel {

class RootFilesystem : public Filesystem
{
public:
    RootFilesystem();
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override { machine = machine_; }
    void Initialize() override;
    File* Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process) override;
    File* Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process) override;
    INodePtr SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process) override;
    void Stat(INode* inode, cmajor::systemx::machine::Process* process) override;
    BlockFile* HostFile() const override { return hostFile; }
    DirFile* OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process) override;
    void MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode) override;
    void CloseFile(int32_t id, cmajor::systemx::kernel::Process* process);
    std::string GetHostFilePath(int32_t inodeNumber, cmajor::systemx::machine::Process* process) override { return std::string(); }
    INodePtr ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
    void WriteINode(INode* inode, cmajor::systemx::machine::Process* process) override;
    std::string INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process) override;
private:
    cmajor::systemx::machine::Machine* machine;
    Filesystem* hostFs;
    BlockFile* hostFile;
    int32_t nextFileId;
    std::map<int32_t, File*> fileMap;
};

std::string RootFsHostFilePath();
bool RootFsExists();
void MakeRootFs(RootFilesystem& rootFs);

} // namespace cmajor::systemx::kernel
