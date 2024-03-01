// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.fs;

import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.inode.manager;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class File;
class BlockFile;
class DirFile;

class Filesystem
{
public:
    Filesystem(int32_t id_);
    virtual ~Filesystem();
    int32_t Id() const { return id; }
    virtual void SetMachine(cmajor::systemx::machine::Machine* machine) = 0;
    virtual void Initialize() = 0;
    virtual File* Create(const std::string& path, INode* dirINode, int32_t mode, cmajor::systemx::machine::Process* process) = 0;
    virtual File* Open(const std::string& path, INode* dirINode, int32_t flags, int32_t mode, cmajor::systemx::machine::Process* process) = 0;
    virtual BlockFile* HostFile() const = 0;
    virtual INodeKey MountPoint() const { return INodeKey(); }
    virtual void SetMountPoint(INodeKey mountPoint_) {}
    virtual INodePtr SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, cmajor::systemx::machine::Process* process) = 0;
    virtual void Stat(INode* inode, cmajor::systemx::machine::Process* process) = 0;
    virtual DirFile* OpenDir(const std::string& path, INode* dirINode, cmajor::systemx::machine::Process* process) = 0;
    virtual void MkDir(INode* parentDirINode, const std::string& dirName, cmajor::systemx::machine::Process* process, int32_t mode) = 0;
    virtual std::string GetHostFilePath(int32_t inodeNumber, cmajor::systemx::machine::Process* process) = 0;
    virtual INodePtr ReadINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process) = 0;
    virtual void WriteINode(INode* inode, cmajor::systemx::machine::Process* process) = 0;
    virtual std::string INodeToPath(INodeKey inodeKey, cmajor::systemx::machine::Process* process) = 0;
private:
    int32_t id;
};

class SuperBlock
{
public:
    SuperBlock();
    void Read(Block* block);
    void Write(Block* block);
    void SetNumBlocks(int32_t numBlocks_) { numBlocks = numBlocks_; }
    int32_t NumBlocks() const { return numBlocks; }
    void SetNumINodes(int32_t numINodes_) { numINodes = numINodes_; }
    int32_t NumINodes() const { return numINodes; }
    void SetFirstBlockNumberBlockNumber(int32_t firstBlockNumberBlockNumber_) { firstBlockNumberBlockNumber = firstBlockNumberBlockNumber_; }
    int32_t FirstBlockNumberBlockNumber() const { return firstBlockNumberBlockNumber; }
    void SetCurrentBlockNumberBlockNumber(int32_t currentBlockNumberBlockNumber_) { currentBlockNumberBlockNumber = currentBlockNumberBlockNumber_; }
    int32_t CurrentBlockNumberBlockNumber() const { return currentBlockNumberBlockNumber; }
    int32_t LastBlockNumberBlockNumber() const { return FirstBlockNumberBlockNumber() + NumberOfBlockNumberBlocks() - 1; }
    void SetFirstINodeBlockNumber(int32_t firstINodeBlockNumber_) { firstINodeBlockNumber = firstINodeBlockNumber_; }
    int32_t FirstINodeBlockNumber() const { return firstINodeBlockNumber; }
    void SetCurrentINodeBlockNumber(int32_t currentINodeBlockNumber_) { currentINodeBlockNumber = currentINodeBlockNumber_; }
    int32_t CurrentINodeBlockNumber() const { return currentINodeBlockNumber; }
    int32_t LastINodeBlockNumber() const { return FirstINodeBlockNumber() + NumberOfINodeBlocks() - 1; }
    void SetFirstDataBlockNumber(int32_t firstDataBlockNumber_) { firstDataBlockNumber = firstDataBlockNumber_; }
    int32_t FirstDataBlockNumber() const { return firstDataBlockNumber; }
    int32_t NumberOfBlockNumberBlocks() const;
    int32_t NumberOfINodeBlocks() const;
    int32_t NumberOfDataBlocks() const;
    int32_t GetINodeBlockNumber(int32_t inodeNumber) const;
    int32_t GetINodeIndexInBlock(int32_t inodeNumber) const;
    int32_t MakeINodeNumber(int32_t inodeBlockNumber, int32_t inodeIndexInBlock) const;
    int32_t CurrentNumberOfFiles() const { return currentNumberOfFiles; }
    void IncrementNumberOfFiles() { ++currentNumberOfFiles; }
    void DecrementNumberOfFiles() { --currentNumberOfFiles; }
    int32_t INodeNumberOfRootDirectory() const { return inodeNumberOfRootDirectory; }
    void SetINodeNumberOfRootDirectory(int32_t inodeNumberOfRootDirectory_) { inodeNumberOfRootDirectory = inodeNumberOfRootDirectory_; }
private:
    int32_t numBlocks;
    int32_t numINodes;
    int32_t firstBlockNumberBlockNumber;
    int32_t currentBlockNumberBlockNumber;
    int32_t firstINodeBlockNumber;
    int32_t currentINodeBlockNumber;
    int32_t firstDataBlockNumber;
    int32_t currentNumberOfFiles;
    int32_t inodeNumberOfRootDirectory;
};

class BlockNumberBlock
{
public:
    BlockNumberBlock();
    static constexpr int32_t BlockNumberCount() { return Block::Size() / 4; }
    void Read(Block* block);
    void Write(Block* block);
    int32_t GetBlockNumber(int index) const;
    void SetBlockNumber(int32_t blockNumber, int index);
    int32_t GetFreeBlockNumber();
    bool PutFreeBlockNumber(int32_t blockNumber);
private:
    std::vector<int32_t> blockNumbers;
};

const int numINodesInBlock = Block::Size() / INode::Size();

class INodeBlock
{
public:
    INodeBlock();
    void Read(Block* block);
    void Write(Block* block);
    int FirstFreeINodeIndex() const;
    static constexpr int Count() { return numINodesInBlock; }
    const INode& GetINode(int index) const { return inodes[index]; }
    void SetINode(const INode& inode, int index) { inodes[index] = inode; }
private:
    INode inodes[numINodesInBlock];
};

class DirectoryEntry
{
public:
    DirectoryEntry();
    void Read(Block* block, int index);
    void Write(Block* block, int index);
    static constexpr int Size() { return 256; }
    static constexpr int MaxNameLength() { return Size() - 4 - 1; }
    bool IsFree() const { return inodeNumber == -1; }
    void SetFree() { inodeNumber = -1; }
    int32_t INodeNumber() const { return inodeNumber; }
    void SetINodeNumber(int32_t inodeNumber_) { inodeNumber = inodeNumber_; }
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    void SetNameTruncate(const std::string& name_);
private:
    int32_t inodeNumber;
    std::string name;
};

class DirectoryBlock
{
public:
    DirectoryBlock();
    static constexpr int NumDirectoryEntriesInBlock() { return Block::Size() / DirectoryEntry::Size(); }
    void Read(Block* block);
    void Write(Block* block);
    const std::vector<DirectoryEntry>& Entries() const { return entries; }
    void AddEntry(const DirectoryEntry& entry);
    void RemoveEntry(int index);
    bool IsEmpty() const { return entries.empty(); }
    bool IsFull() const { return entries.size() == NumDirectoryEntriesInBlock(); }
private:
    std::vector<DirectoryEntry> entries;
};

uint64_t ToULong(const INodeKey& key);
INodeKey ToINodeKey(uint64_t key);
Filesystem* GetFs(int32_t fsNumber);
void ReadSuperBlock(SuperBlock& superBlock, Filesystem* fs, cmajor::systemx::machine::Process* process);
void WriteSuperBlock(SuperBlock& superBlock, Filesystem* fs, cmajor::systemx::machine::Process* process);
BlockPtr ReadBlockNumberBlock(BlockNumberBlock& blockNumberBlock, int32_t blockNumberBlockNumber, Filesystem* fs, cmajor::systemx::machine::Process* process);
void WriteBlockNumberBlock(BlockNumberBlock& blockNumberBlock, BlockPtr& bnb, Filesystem* fs, cmajor::systemx::machine::Process* process);
BlockPtr ReadINodeBlock(INodeBlock& inodeBlock, int32_t inodeBlockNumber, Filesystem* fs, cmajor::systemx::machine::Process* process);
void WriteINodeBlock(INodeBlock& inodeBlock, BlockPtr& inb, Filesystem* fs, cmajor::systemx::machine::Process* process);
BlockPtr ReadDirectoryBlock(DirectoryBlock& directoryBlock, int32_t directoryBlockNumber, Filesystem* fs, cmajor::systemx::machine::Process* process);
void WriteDirectoryBlock(DirectoryBlock& directoryBlock, BlockPtr& db, Filesystem* fs, cmajor::systemx::machine::Process* process);
int32_t GetFreeBlockNumber(int32_t fsNumber, cmajor::systemx::machine::Process* process);
void PutFreeBlockNumber(int32_t fsNumber, int32_t freeBlockNumber, cmajor::systemx::machine::Process* process);
INodePtr AllocateINode(int32_t fsNumber, cmajor::systemx::machine::Process* process);
void FreeINode(INode* inode, cmajor::systemx::machine::Process* process);
INodePtr ReadINode(const INodeKey& inodeKey, cmajor::systemx::machine::Process* process);
void WriteINode(INode* inode, cmajor::systemx::machine::Process* process);
BlockPtr AllocateBlock(int32_t fsNumber, cmajor::systemx::machine::Process* process);
void FreeBlock(Block* block, cmajor::systemx::machine::Process* process);
void FreeBlocks(INode* inode, Filesystem* fs, cmajor::systemx::machine::Process* process);
int32_t MapBlockNumber(int32_t logicalBlockNumber, INode* inode, Filesystem* fs, cmajor::systemx::machine::Process* process);
void SetBlockNumber(int32_t logicalBlockNumber, int32_t blockNumber, INode* inode, Filesystem* fs, cmajor::systemx::machine::Process* process);
INodePtr SearchDirectory(const std::string& name, INode* dirINode, const std::string& dirPath, Filesystem* fs, cmajor::systemx::machine::Process* process);
DirectoryEntry GetDirectoryEntry(INode* dirINode, int32_t inodeNumber, Filesystem* fs, cmajor::systemx::machine::Process* process);
void AddDirectoryEntry(const DirectoryEntry& entry, INode* dirINode, Filesystem* fs, cmajor::systemx::machine::Process* process);
void RemoveDirectoryEntry(const std::string& name, const std::string& filePath, INode* dirINode, Filesystem* fs, cmajor::systemx::machine::Process* process);
void RemoveDirectoryEntry(const std::string& name, const std::string& filePath, INode* dirINode, Filesystem* fs, cmajor::systemx::machine::Process* process, bool unlink);
void RenameDirectoryEntry(INode* dirINode, const std::string& oldName, const std::string& newName, Filesystem* fs, cmajor::systemx::machine::Process* process);
INodePtr MakeDirectory(const std::string& path, Filesystem* fs, cmajor::systemx::machine::Process* process, int32_t mode);
bool DirectoryExists(const std::string& path, Filesystem* fs, cmajor::systemx::machine::Process* process);
std::string GetHostFilePath(const INodeKey& key, cmajor::systemx::machine::Process* process);

enum class PathToINodeFlags : int32_t
{
    none = 0, ignoreMountPoint = 1 << 0, stat = 1 << 1
};

constexpr PathToINodeFlags operator|(PathToINodeFlags left, PathToINodeFlags right)
{
    return PathToINodeFlags(int32_t(left) | int32_t(right));
}

constexpr PathToINodeFlags operator&(PathToINodeFlags left, PathToINodeFlags right)
{
    return PathToINodeFlags(int32_t(left) & int32_t(right));
}

constexpr PathToINodeFlags operator~(PathToINodeFlags flags)
{
    return PathToINodeFlags(~int32_t(flags));
}

INodePtr PathToINode(const std::string& path, Filesystem* fs, cmajor::systemx::machine::Process* process);
INodePtr PathToINode(const std::string& path, Filesystem* fs, cmajor::systemx::machine::Process* process, PathToINodeFlags flags);

} // namespace cmajor::systemx::kernel
