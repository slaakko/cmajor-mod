// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.block.manager;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

struct BlockKey
{
    BlockKey() : fsNumber(-1), blockNumber(-1) {}
    BlockKey(int32_t fsNumber_, int32_t blockNumber_) : fsNumber(fsNumber_), blockNumber(blockNumber_) {}
    int32_t fsNumber;
    int32_t blockNumber;
};

inline bool operator==(BlockKey left, BlockKey right)
{
    return left.fsNumber == right.fsNumber && left.blockNumber == right.blockNumber;
}

inline bool operator!=(BlockKey left, BlockKey right)
{
    return !(left == right);
}

inline bool operator<(BlockKey left, BlockKey right)
{
    if (left.fsNumber < right.fsNumber) return true;
    if (left.fsNumber > right.fsNumber) return false;
    return left.blockNumber < right.blockNumber;
}

inline BlockKey SuperBlockKey(int32_t fsNumber)
{
    return BlockKey(fsNumber, 0);
}

inline BlockKey MakeBlockKey(int32_t fsNumber, int32_t blockNumber)
{
    return BlockKey(fsNumber, blockNumber);
}

const int blockSize = 4096;

enum class BlockFlags : int32_t
{
    none = 0, locked = 1 << 0, valid = 1 << 1, dirty = 1 << 2
};

constexpr BlockFlags operator|(BlockFlags left, BlockFlags right)
{
    return BlockFlags(int32_t(left) | int32_t(right));
}

constexpr BlockFlags operator&(BlockFlags left, BlockFlags right)
{
    return BlockFlags(int32_t(left) & int32_t(right));
}

constexpr BlockFlags operator~(BlockFlags flags)
{
    return BlockFlags(~int(flags));
}

class Block
{
public:
    Block();
    Block(BlockKey key_);
    bool GetFlag(BlockFlags flag) const { return (flags & flag) != BlockFlags::none; }
    void SetFlag(BlockFlags flag) { flags = flags | flag; }
    void ResetFlag(BlockFlags flag) { flags = flags & ~flag; }
    bool IsLocked() const { return GetFlag(BlockFlags::locked); }
    void SetLocked() { SetFlag(BlockFlags::locked); }
    void ResetLocked() { ResetFlag(BlockFlags::locked); }
    bool IsValid() const;
    void SetValid() { SetFlag(BlockFlags::valid); }
    void ResetValid() { ResetFlag(BlockFlags::valid); }
    bool IsDirty() const { return GetFlag(BlockFlags::dirty); }
    void SetDirty() { SetFlag(BlockFlags::dirty); }
    void ResetDirty() { ResetFlag(BlockFlags::dirty); }
    const BlockKey& Key() const { return key; }
    void SetKey(const BlockKey& key_) { key = key_; }
    void Clear();
    static constexpr int Size() { return blockSize; }
    uint8_t* Data() { return &data[0]; }
private:
    BlockFlags flags;
    BlockKey key;
    uint8_t data[blockSize];
};

class BlockPtr
{
public:
    BlockPtr(Block* block_) noexcept : block(block_) {}
    ~BlockPtr();
    BlockPtr(const BlockPtr&) = delete;
    BlockPtr& operator=(const BlockPtr&) = delete;
    BlockPtr(BlockPtr&& that) noexcept : block(that.block) { that.block = nullptr; }
    BlockPtr& operator=(BlockPtr&& that) noexcept
    {
        std::swap(block, that.block);
        return *this;
    }
    Block* Get() { return block; }
    void Release();
private:
    Block* block;
};

BlockPtr GetBlock(BlockKey blockKey, cmajor::systemx::machine::Process* process);

void InitBlockManager();
void DoneBlockManager();
void SetBlockManagerMachine(cmajor::systemx::machine::Machine* machine);
void StartBlockManager();
void StopBlockManager();

} // namespace cmajor::systemx::kernel
