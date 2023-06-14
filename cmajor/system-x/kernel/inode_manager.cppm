// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/pool/pool_alloc.hpp>

export module cmajor.systemx.kernel.inode.manager;

import cmajor.systemx.machine;
import util;
import std.core;

export namespace cmajor::systemx::kernel {

class Block;
class Process;
class BlockFile;

enum class FileType : int32_t
{
    free = 0, regular = 1, directory = 2, fifo = 3
};

enum class Access : int32_t
{
    none = 0, read = 1 << 2, write = 1 << 1, execute = 1 << 0
};

constexpr Access operator|(Access left, Access right)
{
    return Access(int32_t(left) | int32_t(right));
}

constexpr Access operator&(Access left, Access right)
{
    return Access(int32_t(left) & int32_t(right));
}

constexpr Access operator~(Access access)
{
    return Access(~int32_t(access));
}

enum class INodeFlags : int32_t
{
    none = 0, mountPoint = 1 << 0, setUID = 1 << 1, setGID = 1 << 2, locked = 1 << 3, valid = 1 << 4
};

constexpr INodeFlags operator|(INodeFlags left, INodeFlags right)
{
    return INodeFlags(int32_t(left) | int32_t(right));
}

constexpr INodeFlags operator&(INodeFlags left, INodeFlags right)
{
    return INodeFlags(int32_t(left) & int32_t(right));
}

constexpr INodeFlags operator~(INodeFlags flags)
{
    return INodeFlags(~int32_t(flags));
}

inline bool GetFlag(INodeFlags flags, INodeFlags flag) { return (flags & flag) != INodeFlags::none; }
inline void SetFlag(INodeFlags& flags, INodeFlags flag) { flags = flags | flag; }
inline void ResetFlag(INodeFlags& flags, INodeFlags flag) { flags = flags & ~flag; }

class INode;

int32_t EncodeMode(FileType fileType, INodeFlags flags, Access ownerAccess, Access groupAccess, Access otherAccess);
void DecodeMode(int32_t mode, FileType& fileType, INodeFlags& flags, Access& ownerAccess, Access& groupAccess, Access& otherAccess);
int32_t AlterMode(int32_t mode, int32_t umask, bool directory);
void CheckAccess(Access access, int32_t euid, int32_t egid, INode* inode, const std::string& message);

struct INodeKey
{
    INodeKey() : fsNumber(-1), inodeNumber(-1) {}
    INodeKey(int32_t fsNumber_, int32_t inodeNumber_) : fsNumber(fsNumber_), inodeNumber(inodeNumber_) {}
    bool IsDefault() const { return fsNumber == -1 && inodeNumber == -1; }
    std::string ToString() const { return "inode key=" + std::to_string(fsNumber) + ":" + std::to_string(inodeNumber); }
    int32_t fsNumber;
    int32_t inodeNumber;
};

inline bool operator==(const INodeKey& left, const INodeKey& right)
{
    return left.fsNumber == right.fsNumber && left.inodeNumber == right.inodeNumber;
}

inline bool operator!=(const INodeKey& left, const INodeKey& right)
{
    return !(left == right);
}

inline bool operator<(INodeKey left, INodeKey right)
{
    if (left.fsNumber < right.fsNumber) return true;
    if (left.fsNumber > right.fsNumber) return false;
    return left.inodeNumber < right.inodeNumber;
}

inline INodeKey RootDirectoryINodeKey(int32_t fsNumber)
{
    return INodeKey(fsNumber, 0);
}

inline INodeKey MakeINodeKey(int32_t fsNumber, int32_t inodeNumber)
{
    return INodeKey(fsNumber, inodeNumber);
}

class INode
{
public:
    static constexpr int Size() { return 128; }
    INode();
    INode(INodeKey key_);
    const INodeKey& Key() const { return key; }
    void SetKey(const INodeKey& key_) { key = key_; }
    int32_t EncodeMode() const;
    void DecodeMode(int32_t mode);
    void SetMode(int32_t mode);
    void SetSetUIDBit() { SetFlag(flags, INodeFlags::setUID); }
    void ResetSetUIDBit() { ResetFlag(flags, INodeFlags::setUID); }
    bool SetUIDBit() const { return GetFlag(flags, INodeFlags::setUID); }
    void SetSetGIDBit() { SetFlag(flags, INodeFlags::setGID); }
    void ResetSetGIDBit() { ResetFlag(flags, INodeFlags::setGID); }
    bool SetGIDBit() const { return GetFlag(flags, INodeFlags::setGID); }
    void Read(Block* block, int index);
    void Write(Block* block, int index);
    bool IsLocked() const { return GetFlag(flags, INodeFlags::locked); }
    void Lock(int32_t processId) { SetFlag(flags, INodeFlags::locked); lockerProcessId = processId; ++lockCount; }
    void Unlock() { --lockCount; if (lockCount == 0) { ResetFlag(flags, INodeFlags::locked); lockerProcessId = -1; } }
    bool IsValid() const { return GetFlag(flags, INodeFlags::valid); }
    void SetValid() { SetFlag(flags, INodeFlags::valid); }
    void ResetValid() { ResetFlag(flags, INodeFlags::valid); }
    bool IsMountPoint() const { return GetFlag(flags, INodeFlags::mountPoint); }
    void SetMountPoint() { SetFlag(flags, INodeFlags::mountPoint); }
    void ResetMountPoint() { ResetFlag(flags, INodeFlags::mountPoint); }
    bool IsFree() const { return fileType == FileType::free; }
    void SetFree() { fileType = FileType::free; }
    FileType GetFileType() const { return fileType; }
    void SetFileType(FileType fileType_) { fileType = fileType_; }
    INodeFlags Flags() const { return flags; }
    Access OwnerAccess() const { return ownerAccess; }
    void SetOwnerAccess(Access ownerAccess_) { ownerAccess = ownerAccess_; }
    Access GroupAccess() const { return groupAccess; }
    void SetGroupAccess(Access groupAccess_) { groupAccess = groupAccess_; }
    Access OtherAccess() const { return otherAccess; }
    void SetOtherAccess(Access otherAccess_) { otherAccess = otherAccess_; }
    int32_t NLinks() const { return nlinks; }
    void SetNLinks(int32_t nlinks_) { nlinks = nlinks_; }
    int32_t UID() const { return uid; }
    void SetUID(int32_t uid_) { uid = uid_; }
    int32_t GID() const { return gid; }
    void SetGID(int32_t gid_) { gid = gid_; }
    int64_t FileSize() const { return fileSize; }
    void SetFileSize(int64_t fileSize_) { fileSize = fileSize_; }
    int64_t InternalFileSize() const { return internalFileSize; }
    void SetInternalFileSize(int64_t internalFileSize_) { internalFileSize = internalFileSize_; }
    const util::DateTime& CTime() const { return ctime; }
    void SetCTime(const util::DateTime& ctime_) { ctime = ctime_; }
    const util::DateTime& MTime() const { return mtime; }
    void SetMTime(const util::DateTime& mtime_) { mtime = mtime_; }
    const util::DateTime& ATime() const { return atime; }
    void SetATime(const util::DateTime& atime_) { atime = atime_; }
    static constexpr int32_t NumDirectBlockNumbers() { return 16; }
    int32_t GetDirectBlockNumber(int32_t index) const;
    void SetDirectBlockNumber(int32_t blockNumber, int32_t index);
    int32_t SingleIndirectBlockNumber() const { return singleIndirectBlockNumber; }
    void SetSingleIndirectBlockNumber(int32_t singleIndirectBlockNumber_) { singleIndirectBlockNumber = singleIndirectBlockNumber_; }
    int32_t DoubleIndirectBlockNumber() const { return doubleIndirectBlockNumber; }
    void SetDoubleIndirectBlockNumber(int32_t doubleIndirectBlockNumber_) { doubleIndirectBlockNumber = doubleIndirectBlockNumber_; }
    int32_t NumberOfBlocks() const;
    int32_t LockCount() const { return lockCount; }
    void SetLockCount(int32_t lockCount_) { lockCount = lockCount_; }
    int32_t LockerProcessId() const { return lockerProcessId; }
    void SetLockerProcessId(int32_t lockerProcessId_) { lockerProcessId = lockerProcessId_; }
    int32_t ReferenceCount() const { return referenceCount; }
    void IncrementReferenceCount() { ++referenceCount; }
    void DecrementReferenceCount() { --referenceCount; }
    static constexpr int32_t StatBufSize() { return 70; }
    void WriteStat(util::MemoryWriter& writer);
private:
    INodeKey key;
    FileType fileType;
    INodeFlags flags;
    Access ownerAccess;
    Access groupAccess;
    Access otherAccess;
    int32_t nlinks;
    int32_t uid;
    int32_t gid;
    int64_t fileSize;
    int64_t internalFileSize;
    util::DateTime ctime;
    util::DateTime mtime;
    util::DateTime atime;
    std::vector<int32_t> directBlockNumbers;
    int32_t singleIndirectBlockNumber;
    int32_t doubleIndirectBlockNumber;
    int32_t referenceCount;
    int32_t lockCount;
    int32_t lockerProcessId;
};

class INodePtr
{
public:
    explicit INodePtr(INode* inode_) noexcept : inode(inode_) {}
    ~INodePtr();
    INodePtr(const INodePtr&) = delete;
    INodePtr& operator=(const INodePtr&) = delete;
    INodePtr(INodePtr&& that) noexcept : inode(that.inode) { that.inode = nullptr; }
    INodePtr& operator=(INodePtr&& that) noexcept;
    INode* Get() { return inode; }
    void SetINode(const INode& inode_);
private:
    INode* inode;
};

INodePtr GetINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process);

void AddDirChangeNotification(INodeKey dirINodeKey, int32_t pid);
std::vector<int32_t>* GetDirChangeNotificationPIDs(INodeKey dirINodeKey);
void RemoveDirChangeNotifications(int32_t pid);

void InitINodeManager();
void DoneINodeManager();
void SetINodeManagerMachine(cmajor::systemx::machine::Machine* machine);
void StartINodeManager();
void StopINodeManager();

} // namespace cmajor::systemx::kernel
