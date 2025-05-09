// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.msg.queue;

import cmajor.systemx.kernel.debug.msg;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

void ProcessMessageQueues::Add(int32_t md)
{
    mds.push_back(md);
}
void ProcessMessageQueues::Remove(int32_t md)
{
    auto it = std::find(mds.begin(), mds.end(), md);
    if (it != mds.end())
    {
        mds.erase(it);
    }
}

void ProcessMessageQueues::CloseAll(cmajor::systemx::machine::Process* process)
{
    for (auto md : mds)
    {
        if (IsMsgQOpen(md))
        {
            CloseMsgQ(process, md, false);
        }
    }
}

Msg::Msg(const std::vector<uint8_t>& data_) : data(data_)
{
}

class MsgQueue
{
public:
    MsgQueue(const std::string& name_);
    void Put(const Msg& msg);
    Msg Get();
    bool IsEmpty() const { return queue.empty(); }
    int32_t Length() const { return queue.size(); }
    const Msg& Front() const { return queue.front(); }
    void IncrementReferenceCount() { ++referenceCount; }
    void DecrementReferenceCount() { --referenceCount; }
    int32_t ReferenceCount() const { return referenceCount; }
    const std::string& Name() const { return name; }
private:
    int32_t referenceCount;
    std::string name;
    std::list<Msg> queue;
};

MsgQueue::MsgQueue(const std::string& name_) : name(name_), queue(), referenceCount(1)
{
}

void MsgQueue::Put(const Msg& msg)
{
    queue.push_back(msg);
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: put " + GetMsgStr(msg) + " to '" + Name() + "'");
    }
}

Msg MsgQueue::Get()
{
    Msg msg = queue.front();
    queue.pop_front();
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: get " + GetMsgStr(msg) + " from '" + Name() + "'");
    }
    return msg;
}

class MsgQueues
{
public:
    static void Init();
    static void Done();
    static MsgQueues& Instance() { return *instance; }
    int32_t Open(const std::string& name);
    bool IsOpen(int32_t md) const;
    MsgQueue* Get(int32_t md);
    void Delete(cmajor::systemx::machine::Process* process, int32_t md);
private:
    static std::unique_ptr<MsgQueues> instance;
    std::map<std::string, int32_t> queueMap;
    std::vector<std::unique_ptr<MsgQueue>> queues;
};

std::unique_ptr<MsgQueues> MsgQueues::instance;

void MsgQueues::Init()
{
    instance.reset(new MsgQueues());
}

void MsgQueues::Done()
{
    instance.reset();
}

int32_t MsgQueues::Open(const std::string& name)
{
    auto it = queueMap.find(name);
    if (it != queueMap.cend())
    {
        int32_t md = it->second;
        MsgQueue* queue = Get(md);
        queue->IncrementReferenceCount();
        if ((GetDebugMode() & debugMsgQueueMode) != 0)
        {
            DebugWrite("kernel.msgq: message queue '" + name + "' opened");
        }
        return md;
    }
    int32_t md = queues.size();
    queueMap[name] = md;
    MsgQueue* queue = new MsgQueue(name);
    queues.push_back(std::unique_ptr<MsgQueue>(queue));
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: message queue '" + name + "' created");
    }
    return md;
}

bool MsgQueues::IsOpen(int32_t md) const
{
    if (md >= 0 && md < queues.size())
    {
        MsgQueue* q = queues[md].get();
        if (q)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

MsgQueue* MsgQueues::Get(int32_t md)
{
    if (md >= 0 && md < queues.size())
    {
        MsgQueue* q = queues[md].get();
        if (q)
        {
            return q;
        }
        else
        {
            throw SystemError(EBADF, "message queue " + std::to_string(md) + " is closed", __FUNCTION__);
        }
    }
    else
    {
        throw SystemError(EBADF, "invalid message queue descriptor " + std::to_string(md), __FUNCTION__);
    }
}

void MsgQueues::Delete(cmajor::systemx::machine::Process* process, int32_t md)
{
    if (md >= 0 && md < queues.size())
    {
        if ((GetDebugMode() & debugMsgQueueMode) != 0)
        {
            DebugWrite("kernel.msgq: message queue '" + queues[md]->Name() + "' deleted");
        }
        queueMap.erase(queues[md]->Name());
        queues[md].reset();
        cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::msgQEvent, md);
        Wakeup(process, evnt);
    }
    else
    {
        throw SystemError(EBADF, "invalid message queue descriptor " + std::to_string(md), __FUNCTION__);
    }
}

int32_t MsgQ(cmajor::systemx::kernel::Process* process, int64_t nameAddr)
{
    if (nameAddr == 0)
    {
        throw SystemError(EPARAM, "name is null", __FUNCTION__);
    }
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    std::string name = ReadString(process, nameAddr, mem);
    int32_t md = MsgQueues::Instance().Open(name);
    process->MessageQueues().Add(md);
    return md;
}

void CloseMsgQ(cmajor::systemx::machine::Process* process, int32_t md)
{
    CloseMsgQ(process, md, true);
}

void CloseMsgQ(cmajor::systemx::machine::Process* process, int32_t md, bool remove)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    if (remove)
    {
        process->RemoveMessageQueue(md);
    }
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: closing message queue '" + queue->Name() + "'");
    }
    queue->DecrementReferenceCount();
    if (queue->ReferenceCount() == 0)
    {
        MsgQueues::Instance().Delete(process, md);
    }
}

void WaitMsgLocked(std::unique_lock<std::recursive_mutex>& lock, cmajor::systemx::machine::Process* process, int32_t md)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    int processId = process->Id();
    std::string queueName = queue->Name();
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: > wait msg '" + queueName + "': pid=" + std::to_string(processId));
    }
    if (queue->Length() > 0)
    {
        if ((GetDebugMode() & debugMsgQueueMode) != 0)
        {
            DebugWrite("kernel.msgq: < wait msg '" + queueName + "': queue not empty: pid=" + std::to_string(processId));
        }
        return;
    }
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: >> wait msg '" + queueName + "': sleep: pid=" + std::to_string(processId));
    }
    cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::msgQEvent, md);
    Sleep(evnt, process, lock);
    if (IsMsgQOpen(md))
    {
        if ((GetDebugMode() & debugMsgQueueMode) != 0)
        {
            DebugWrite("kernel.msgq: < wait msg '" + queueName + "': wake up: pid=" + std::to_string(processId));
        }
    }
    else
    {
        if ((GetDebugMode() & debugMsgQueueMode) != 0)
        {
            DebugWrite("kernel.msgq: < wait msg '" + queueName + "': wake up: note message queue deleted: pid=" + std::to_string(processId));
        }
        throw SystemError(EBADF, "message queue '" + queueName + "': " + std::to_string(md) + " deleted", __FUNCTION__);
    }
}

void WaitMsg(cmajor::systemx::machine::Process* process, int32_t md)
{
    cmajor::systemx::machine::Machine* machine = process->GetMachine();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), MSG_QUEUE, Id(), NO_LOCK | EXIT);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), MSG_QUEUE, Id(), HAS_LOCK | EXIT);
#endif
    WaitMsgLocked(lock, process, md);
}

bool IsMsgQOpen(int32_t md)
{
    return MsgQueues::Instance().IsOpen(md);
}

void PutMsg(cmajor::systemx::kernel::Process* process, int32_t md, const std::vector<std::uint8_t>& msgData)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    queue->Put(Msg(msgData));
    if ((GetDebugMode() & debugMsgQueueMode) != 0)
    {
        DebugWrite("kernel.msgq: > put msg '" + queue->Name() + "'");
    }
    cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::msgQEvent, md);
    Wakeup(process, evnt);
}

void PutMsg(cmajor::systemx::kernel::Process* process, int32_t md, int64_t msgDataAddr, int32_t msgSize)
{
    cmajor::systemx::machine::Machine* machine = process->GetProcessor()->GetMachine();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), MSG_QUEUE, Id(), NO_LOCK | EXIT);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), MSG_QUEUE, Id(), HAS_LOCK | EXIT);
#endif 
    if (msgSize == 0)
    {
        throw SystemError(EPARAM, "message is empty", __FUNCTION__);
    }
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    std::vector<uint8_t> data = ReadProcessMemory(process, msgDataAddr, msgSize);
    PutMsg(process, md, data);
}

int32_t GetMsgQueueLength(cmajor::systemx::machine::Process* process, int32_t md)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    return queue->Length();
}

int32_t GetMsgSize(cmajor::systemx::machine::Process* process, int32_t md)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    if (queue->IsEmpty())
    {
        return 0;
    }
    else
    {
        return queue->Front().Data().size();
    }
}

std::vector<uint8_t> GetMsg(int32_t md)
{
    MsgQueue* queue = MsgQueues::Instance().Get(md);
    if (queue->IsEmpty())
    {
        throw SystemError(EFAIL, "message queue " + std::to_string(md) + " is empty", __FUNCTION__);
    }
    Msg msg = queue->Get();
    return msg.Data();
}

void GetMsg(cmajor::systemx::kernel::Process* process, int32_t md, int64_t bufferAddr)
{
    if (bufferAddr == 0)
    {
        throw SystemError(EPARAM, "buffer is null", __FUNCTION__);
    }
    std::vector<uint8_t> msg = GetMsg(md);
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    WriteProcessMemory(process, bufferAddr, msg);
}

void InitMsgQueue()
{
    MsgQueues::Init();
}

void DoneMsgQueue()
{
    MsgQueues::Done();
}

} // namespace cmajor::systemx::kernel
