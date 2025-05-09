// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/pool/pool_alloc.hpp>

module cmajor.systemx.kernel.inode.manager;

import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

void PutINode(INode* inode);

int32_t EncodeMode(FileType fileType, INodeFlags flags, Access ownerAccess, Access groupAccess, Access otherAccess)
{
    int32_t mode = ((int32_t(ownerAccess) & 7) << 6) | ((int32_t(groupAccess) & 7) << 3) | ((int32_t(otherAccess) & 7));
    int32_t setGIDBit = GetFlag(flags, INodeFlags::setGID) ? 1 : 0;
    mode = mode | (setGIDBit << 10);
    int32_t setUIDBit = GetFlag(flags, INodeFlags::setUID) ? 1 : 0;
    mode = mode | (setUIDBit << 11);
    int32_t mntPointBit = GetFlag(flags, INodeFlags::mountPoint) ? 1 : 0;
    mode = mode | (mntPointBit << 12);
    mode = mode | ((int32_t(fileType) & 7) << 13);
    return mode;
}

void DecodeMode(int32_t mode, FileType& fileType, INodeFlags& flags, Access& ownerAccess, Access& groupAccess, Access& otherAccess)
{
    fileType = FileType((mode >> 13) & 7);
    int32_t mntPointBit = (mode >> 12) & 1;
    if (mntPointBit)
    {
        SetFlag(flags, INodeFlags::mountPoint);
    }
    else
    {
        ResetFlag(flags, INodeFlags::mountPoint);
    }
    int32_t setGIDBit = (mode >> 10) & 1;
    if (setGIDBit)
    {
        SetFlag(flags, INodeFlags::setGID);
    }
    else
    {
        ResetFlag(flags, INodeFlags::setGID);
    }
    int32_t setUIDBit = (mode >> 11) & 1;
    if (setUIDBit)
    {
        SetFlag(flags, INodeFlags::setUID);
    }
    else
    {
        ResetFlag(flags, INodeFlags::setUID);
    }
    ownerAccess = Access((mode >> 6) & 7);
    groupAccess = Access((mode >> 3) & 7);
    otherAccess = Access(mode & 7);
}

int32_t AlterMode(int32_t mode, int32_t umask, bool directory)
{
    if (mode == 0)
    {
        Access access = static_cast<Access>(Access::read | Access::write);
        if (directory)
        {
            access = static_cast<Access>(access | Access::execute);
        }
        mode = cmajor::systemx::kernel::EncodeMode(FileType(), INodeFlags(), access, access, access);
    }
    mode = mode & 0777 & ~umask;
    return mode;
}

void CheckAccess(Access access, int32_t euid, int32_t egid, INode* inode, const std::string& message)
{
    if (euid == inode->UID())
    {
        if ((access & inode->OwnerAccess()) != Access::none)
        {
            return;
        }
    }
    if (egid == inode->GID())
    {
        if ((access & inode->GroupAccess()) != Access::none)
        {
            return;
        }
    }
    if ((access & inode->OtherAccess()) != Access::none)
    {
        return;
    }
    throw SystemError(EPERMISSION, message + ": permission denied", __FUNCTION__);
}

INode::INode() :
    key(),
    fileType(FileType::free),
    flags(INodeFlags::none),
    ownerAccess(Access::none),
    groupAccess(Access::none),
    otherAccess(Access::none),
    nlinks(1),
    uid(0),
    gid(0),
    fileSize(0),
    internalFileSize(0),
    ctime(),
    mtime(),
    atime(),
    directBlockNumbers(NumDirectBlockNumbers(), -1),
    singleIndirectBlockNumber(-1),
    doubleIndirectBlockNumber(-1),
    referenceCount(0),
    lockCount(0),
    lockerProcessId(-1)
{
}

INode::INode(INodeKey key_) :
    key(key_),
    fileType(FileType::free),
    flags(INodeFlags::none),
    ownerAccess(Access::none),
    groupAccess(Access::none),
    otherAccess(Access::none),
    nlinks(1),
    uid(0),
    gid(0),
    fileSize(0),
    ctime(),
    mtime(),
    atime(),
    directBlockNumbers(NumDirectBlockNumbers(), -1),
    singleIndirectBlockNumber(-1),
    doubleIndirectBlockNumber(-1),
    referenceCount(0),
    lockCount(0),
    lockerProcessId(-1)
{
}

int32_t INode::EncodeMode() const
{
    return cmajor::systemx::kernel::EncodeMode(fileType, flags, ownerAccess, groupAccess, otherAccess);
}

void INode::DecodeMode(int32_t mode)
{
    cmajor::systemx::kernel::DecodeMode(mode, fileType, flags, ownerAccess, groupAccess, otherAccess);
}

void INode::SetMode(int32_t mode)
{
    FileType ft;
    INodeFlags f = INodeFlags::none;
    cmajor::systemx::kernel::DecodeMode(mode, ft, f, ownerAccess, groupAccess, otherAccess);
    if ((f & INodeFlags::setUID) != INodeFlags::none)
    {
        SetSetUIDBit();
    }
    else
    {
        ResetSetUIDBit();
    }
    if ((f & INodeFlags::setGID) != INodeFlags::none)
    {
        SetSetGIDBit();
    }
    else
    {
        ResetSetGIDBit();
    }
}

int32_t INode::GetDirectBlockNumber(int32_t index) const
{
    if (index >= 0 && index < NumDirectBlockNumbers())
    {
        return directBlockNumbers[index];
    }
    else
    {
        throw SystemError(EFAIL, "invalid direct block number index", __FUNCTION__);
    }
}

void INode::SetDirectBlockNumber(int32_t blockNumber, int32_t index)
{
    if (index >= 0 && index < NumDirectBlockNumbers())
    {
        directBlockNumbers[index] = blockNumber;
    }
    else
    {
        throw SystemError(EFAIL, "invalid direct block number index", __FUNCTION__);
    }
}

void INode::Read(Block* block, int index)
{
    util::MemoryReader reader(block->Data() + Size() * index, Size());
    int32_t mode = reader.ReadInt();
    DecodeMode(mode);
    nlinks = reader.ReadInt();
    uid = reader.ReadInt();
    gid = reader.ReadInt();
    fileSize = reader.ReadLong();
    ctime = reader.ReadDateTime();
    mtime = reader.ReadDateTime();
    atime = reader.ReadDateTime();
    for (int32_t i = 0; i < NumDirectBlockNumbers(); ++i)
    {
        int32_t blockNumber = reader.ReadInt();
        SetDirectBlockNumber(blockNumber, i);
    }
    singleIndirectBlockNumber = reader.ReadInt();
    doubleIndirectBlockNumber = reader.ReadInt();
}

void INode::Write(Block* block, int index)
{
    util::MemoryWriter writer(block->Data() + Size() * index, Size());
    int32_t mode = EncodeMode();
    writer.Write(mode);
    writer.Write(nlinks);
    writer.Write(uid);
    writer.Write(gid);
    writer.Write(fileSize);
    writer.Write(ctime);
    writer.Write(mtime);
    writer.Write(atime);
    for (int32_t i = 0; i < NumDirectBlockNumbers(); ++i)
    {
        writer.Write(GetDirectBlockNumber(i));
    }
    writer.Write(singleIndirectBlockNumber);
    writer.Write(doubleIndirectBlockNumber);
}

int32_t INode::NumberOfBlocks() const
{
    if (fileSize == 0)
    {
        return 0;
    }
    return static_cast<int32_t>((fileSize - 1) / Block::Size() + 1);
}

void INode::WriteStat(util::MemoryWriter& writer)
{
    writer.Write(key.fsNumber);
    writer.Write(key.inodeNumber);
    writer.Write(static_cast<int32_t>(fileType));
    writer.Write(static_cast<int32_t>(ownerAccess));
    writer.Write(static_cast<int32_t>(groupAccess));
    writer.Write(static_cast<int32_t>(otherAccess));
    writer.Write(static_cast<uint8_t>(SetUIDBit()));
    writer.Write(static_cast<uint8_t>(SetGIDBit()));
    writer.Write(nlinks);
    writer.Write(uid);
    writer.Write(gid);
    writer.Write(fileSize);
    writer.Write(ctime);
    writer.Write(mtime);
    writer.Write(atime);
}

INodePtr::~INodePtr()
{
    if (inode)
    {
        PutINode(inode);
        inode = nullptr;
    }
}

INodePtr& INodePtr::operator=(INodePtr&& that) noexcept
{
    std::swap(inode, that.inode);
    return *this;
}

void INodePtr::SetINode(const INode& inode_)
{
    int32_t lockCount = inode->LockCount();
    int32_t lockerProcessId = inode->LockerProcessId();
    *inode = inode_;
    inode->SetLockCount(lockCount);
    inode->SetLockerProcessId(lockerProcessId);
}

uint64_t INodeKeyHash(INodeKey inodeKey)
{
    return static_cast<uint64_t>(1099511628211) * inodeKey.fsNumber + inodeKey.inodeNumber;
}

using INodeFreeList = std::list<INode*, boost::fast_pool_allocator<INode*>>;

struct INodeHashQueueEntry
{
    INodeHashQueueEntry() : inode(nullptr), it() {}
    INodeHashQueueEntry(INode* inode_, INodeFreeList::iterator it_) : inode(inode_), it(it_) {}
    INode* inode;
    INodeFreeList::iterator it;
};

struct DirChangeNotification
{
    DirChangeNotification(int32_t pid_) : pid(pid_), dirINodeKeys()
    {
    }
    int32_t pid;
    std::vector<INodeKey> dirINodeKeys;
};

class DirChangeNotificationMap
{
public:
    DirChangeNotificationMap();
    void AddDirChangeNotification(INodeKey dirINodeKey, int32_t pid);
    void RemoveDirChangeNotification(int32_t pid);
    DirChangeNotification* GetDirChangeNotification(int32_t pid);
    DirChangeNotification* GetOrInsertDirChangeNotification(int32_t pid);
    std::vector<int32_t>* GetDirChangeNotificationPIDs(const INodeKey& key);
private:
    std::map<int32_t, DirChangeNotification*> pidNotificationMap;
    std::map<INodeKey, std::vector<int32_t>> dirINodePIDNotificationMap;
};

DirChangeNotificationMap::DirChangeNotificationMap() : pidNotificationMap(), dirINodePIDNotificationMap()
{
}

DirChangeNotification* DirChangeNotificationMap::GetDirChangeNotification(int32_t pid)
{
    auto it = pidNotificationMap.find(pid);
    if (it != pidNotificationMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

std::vector<int32_t>* DirChangeNotificationMap::GetDirChangeNotificationPIDs(const INodeKey& key)
{
    auto it = dirINodePIDNotificationMap.find(key);
    if (it != dirINodePIDNotificationMap.cend())
    {
        return &(it->second);
    }
    else
    {
        return nullptr;
    }
}

DirChangeNotification* DirChangeNotificationMap::GetOrInsertDirChangeNotification(int32_t pid)
{
    DirChangeNotification* notification = GetDirChangeNotification(pid);
    if (notification)
    {
        return notification;
    }
    else
    {
        DirChangeNotification* notification = new DirChangeNotification(pid);
        pidNotificationMap[pid] = notification;
        return notification;
    }
}

void DirChangeNotificationMap::AddDirChangeNotification(INodeKey dirINodeKey, int32_t pid)
{
    DirChangeNotification* notification = GetOrInsertDirChangeNotification(pid);
    if (std::find(notification->dirINodeKeys.begin(), notification->dirINodeKeys.end(), dirINodeKey) == notification->dirINodeKeys.end())
    {
        notification->dirINodeKeys.push_back(dirINodeKey);
    }
    std::vector<int32_t>& pids = dirINodePIDNotificationMap[dirINodeKey];
    if (std::find(pids.begin(), pids.end(), pid) == pids.end())
    {
        pids.push_back(pid);
    }
}

void DirChangeNotificationMap::RemoveDirChangeNotification(int32_t pid)
{
    DirChangeNotification* notification = GetDirChangeNotification(pid);
    if (notification)
    {
        for (const INodeKey& key : notification->dirINodeKeys)
        {
            auto it = dirINodePIDNotificationMap.find(key);
            if (it != dirINodePIDNotificationMap.cend())
            {
                std::vector<int32_t>& pids = it->second;
                pids.erase(std::find(pids.begin(), pids.end(), pid), pids.end());
                if (pids.empty())
                {
                    dirINodePIDNotificationMap.erase(key);
                }
            }
        }
        delete notification;
        pidNotificationMap.erase(pid);
    }
}

class INodeManager
{
public:
    static void Init();
    static void Done();
    static INodeManager& Instance() { return *instance; }
    ~INodeManager();
    void SetMachine(cmajor::systemx::machine::Machine* machine_);
    cmajor::systemx::machine::Machine* GetMachine() const { return machine; }
    void Start();
    void Stop();
    int NumberOfHashQueues() const { return numberOfHashQueues; }
    int GetHashQueueNumber(const INodeKey& key) const;
    INodeHashQueueEntry* GetINodeFromHashQueue(const INodeKey& inodeKey);
    void RemoveFromHashQueue(INode* inode);
    void InsertIntoHashQueue(INode* inode);
    void RemoveFromFreeList(INodeHashQueueEntry* entry);
    void PutINodeToFreeList(INode* inode);
    cmajor::systemx::machine::Event GetINodeKeyEvent(const INodeKey& inodeKey);
    void RemoveINodeKeyEvent(const INodeKey& inodeKey);
    bool IsFreeListEmpty() const { return freeList.empty(); }
    INode* GetINodeFromFreeList();
    DirChangeNotificationMap& GetDirChangeNotificationMap() { return dirChangeNotificationMap; }
private:
    INodeManager();
    static std::unique_ptr<INodeManager> instance;
    cmajor::systemx::machine::Machine* machine;
    int numCachedINodes;
    int numberOfHashQueues;
    INodeFreeList freeList;
    std::vector<std::list<INodeHashQueueEntry, boost::fast_pool_allocator<INodeHashQueueEntry>>> hashQueues;
    std::map<INodeKey, cmajor::systemx::machine::Event> inodeKeyMapEventMap;
    int nextINodeKeyEventId;
    DirChangeNotificationMap dirChangeNotificationMap;
};

std::unique_ptr<INodeManager> INodeManager::instance;

void INodeManager::Init()
{
    instance.reset(new INodeManager());
}

void INodeManager::Done()
{
    instance.reset();
}

INodeManager::INodeManager() : machine(nullptr), numCachedINodes(cmajor::systemx::machine::NumCachedINodes()), numberOfHashQueues(cmajor::systemx::machine::NumINodeHashQueues()), nextINodeKeyEventId(0)
{
    hashQueues.resize(numberOfHashQueues);
}

INodeManager::~INodeManager()
{
    for (auto& inode : freeList)
    {
        delete inode;
    }
}

void INodeManager::Start()
{
    for (int i = 0; i < numCachedINodes; ++i)
    {
        freeList.push_back(new INode());
    }
}

void INodeManager::Stop()
{
}

void INodeManager::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    machine = machine_;
}

int INodeManager::GetHashQueueNumber(const INodeKey& key) const
{
    uint64_t inodeKeyHash = INodeKeyHash(key);
    uint64_t n = numberOfHashQueues;
    return static_cast<int>(inodeKeyHash % n);
}

INodeHashQueueEntry* INodeManager::GetINodeFromHashQueue(const INodeKey& inodeKey)
{
    int hashQueueIndex = GetHashQueueNumber(inodeKey);
    auto& hashQueue = hashQueues[hashQueueIndex];
    for (auto it = hashQueue.begin(); it != hashQueue.end(); ++it)
    {
        INode* inode = it->inode;
        if (inode->Key() == inodeKey)
        {
            return &(*it);
        }
    }
    return nullptr;
}

void INodeManager::RemoveFromHashQueue(INode* inode)
{
    if (inode->Key() != INodeKey())
    {
        int hashQueueIndex = GetHashQueueNumber(inode->Key());
        auto& hashQueue = hashQueues[hashQueueIndex];
        auto it = hashQueue.begin();
        while (it != hashQueue.end())
        {
            if (inode == it->inode)
            {
                auto next = it;
                ++next;
                hashQueue.erase(it);
                it = next;
            }
            else
            {
                ++it;
            }
        }
    }
}

void INodeManager::InsertIntoHashQueue(INode* inode)
{
    int hashQueueIndex = GetHashQueueNumber(inode->Key());
    auto& hashQueue = hashQueues[hashQueueIndex];
    hashQueue.push_back(INodeHashQueueEntry(inode, freeList.end()));
}

void INodeManager::RemoveFromFreeList(INodeHashQueueEntry* entry)
{
    if (entry->it != freeList.end())
    {
        freeList.erase(entry->it);
        entry->it = freeList.end();
    }
}

void INodeManager::PutINodeToFreeList(INode* inode)
{
    freeList.push_back(inode);
    INodeHashQueueEntry* entry = GetINodeFromHashQueue(inode->Key());
    if (entry)
    {
        entry->it = --freeList.end();
    }
}

cmajor::systemx::machine::Event INodeManager::GetINodeKeyEvent(const INodeKey& inodeKey)
{
    auto it = inodeKeyMapEventMap.find(inodeKey);
    if (it != inodeKeyMapEventMap.cend())
    {
        return it->second;
    }
    cmajor::systemx::machine::Event evnt = cmajor::systemx::machine::Event(cmajor::systemx::machine::EventKind::inodeFreeEvent, nextINodeKeyEventId++);
    inodeKeyMapEventMap[inodeKey] = evnt;
    return evnt;
}

void INodeManager::RemoveINodeKeyEvent(const INodeKey& inodeKey)
{
    inodeKeyMapEventMap.erase(inodeKey);
}

INode* INodeManager::GetINodeFromFreeList()
{
    INode* inode = freeList.front();
    freeList.pop_front();
    return inode;
}

INodePtr GetINode(INodeKey inodeKey, cmajor::systemx::machine::Process* process)
{
    while (true)
    {
        INodeManager& inodeManager = INodeManager::Instance();
#if (LOCK_DEBUG)
        DebugLock startDebugLock(&inodeManager.GetMachine()->Lock(), INODE_MANAGER, process->Id(), NO_LOCK | GET_INODE);
#endif 
        std::unique_lock<std::recursive_mutex> lock(inodeManager.GetMachine()->Lock());
#if (LOCK_DEBUG)
        DebugLock hasDebugLock(&inodeManager.GetMachine()->Lock(), INODE_MANAGER, process->Id(), HAS_LOCK | GET_INODE);
#endif 
        INodeHashQueueEntry* entry = inodeManager.GetINodeFromHashQueue(inodeKey);
        if (entry)
        {
            if (entry->inode->IsLocked())
            {
                if (entry->inode->LockerProcessId() == process->Id())
                {
                    entry->inode->Lock(process->Id());
                    return INodePtr(entry->inode);
                }
                cmajor::systemx::machine::Event inodeBecomesFreeEvent = inodeManager.GetINodeKeyEvent(inodeKey);
                Sleep(inodeBecomesFreeEvent, process, lock);
                lock.lock();
                continue;
            }
            else
            {
                entry->inode->Lock(process->Id());
                inodeManager.RemoveFromFreeList(entry);
                return INodePtr(entry->inode);
            }
        }
        else
        {
            if (inodeManager.IsFreeListEmpty())
            {
                throw SystemError(ELIMITEXCEEDED, "no cached inodes available", __FUNCTION__);
            }
            else
            {
                INode* inode = inodeManager.GetINodeFromFreeList();
                inodeManager.RemoveFromHashQueue(inode);
                *inode = INode();
                inode->SetKey(inodeKey);
                inodeManager.InsertIntoHashQueue(inode);
                inode->ResetValid();
                inode->Lock(process->Id());
                return INodePtr(inode);
            }
        }
    }
}

void PutINode(INode* inode)
{
    INodeManager& inodeManager = INodeManager::Instance();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&inodeManager.GetMachine()->Lock(), INODE_MANAGER, 0, NO_LOCK | PUT_INODE);
#endif 
    std::unique_lock<std::recursive_mutex> lock(inodeManager.GetMachine()->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&inodeManager.GetMachine()->Lock(), INODE_MANAGER, 0, HAS_LOCK | PUT_INODE);
#endif 
    inode->Unlock();
    if (inode->LockCount() == 0)
    {
        cmajor::systemx::machine::Event inodeFreeEvent = inodeManager.GetINodeKeyEvent(inode->Key());
        Wakeup(nullptr, inodeFreeEvent);
        inodeManager.RemoveINodeKeyEvent(inode->Key());
        inodeManager.PutINodeToFreeList(inode);
    }
}

void AddDirChangeNotification(INodeKey dirINodeKey, int32_t pid)
{
    INodeManager& inodeManager = INodeManager::Instance();
    DirChangeNotificationMap& notificationMap = inodeManager.GetDirChangeNotificationMap();
    notificationMap.AddDirChangeNotification(dirINodeKey, pid);
}

std::vector<int32_t>* GetDirChangeNotificationPIDs(INodeKey dirINodeKey)
{
    INodeManager& inodeManager = INodeManager::Instance();
    DirChangeNotificationMap& notificationMap = inodeManager.GetDirChangeNotificationMap();
    return notificationMap.GetDirChangeNotificationPIDs(dirINodeKey);
}

void RemoveDirChangeNotifications(int32_t pid)
{
    INodeManager& inodeManager = INodeManager::Instance();
    DirChangeNotificationMap& notificationMap = inodeManager.GetDirChangeNotificationMap();
    notificationMap.RemoveDirChangeNotification(pid);
}

void InitINodeManager()
{
    INodeManager::Init();
}

void DoneINodeManager()
{
    INodeManager::Done();
}

void SetINodeManagerMachine(cmajor::systemx::machine::Machine* machine)
{
    INodeManager::Instance().SetMachine(machine);
}

void StartINodeManager()
{
    INodeManager::Instance().Start();
}

void StopINodeManager()
{
    INodeManager::Instance().Stop();
}

} // namespace cmajor::systemx::kernel
