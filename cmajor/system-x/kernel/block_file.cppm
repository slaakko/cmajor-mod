// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.block.file;

import cmajor.systemx.kernel.file;
import cmajor.systemx.kernel.inode.manager;

export namespace cmajor::systemx::kernel {

class Block;

class BlockFile : public File
{
public:
    BlockFile(const std::string& name_, INodeKey inodeKey_);
    File* Share() override;
    void Release(cmajor::systemx::kernel::Process* process) override;
    std::vector<uint8_t> Read(int64_t count, cmajor::systemx::machine::Process* process) override;
    int64_t Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process) override;
    int64_t Seek(int64_t offset, Origin whence, cmajor::systemx::machine::Process* process) override;
    int64_t Tell(cmajor::systemx::machine::Process* process) override;
    virtual INodePtr GetINode(cmajor::systemx::machine::Process* process) = 0;
    virtual int32_t GetBlockNumber(INode* inode, cmajor::systemx::machine::Process* process, bool allocate) const = 0;
    virtual int64_t Read(Block* block, cmajor::systemx::machine::Process* process) = 0;
    virtual int64_t Write(Block* block, cmajor::systemx::machine::Process* process) = 0;
    const INodeKey& GetINodeKey() const { return inodeKey; }
    int64_t FilePos() const { return filePos; }
    void SetFilePos(int64_t filePos_) { filePos = filePos_; }
private:
    INodeKey inodeKey;
    int64_t filePos;
    int referenceCount;
};

} // namespace cmajor::systemx::kernel
