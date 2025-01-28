// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.msg.queue;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

const int firstScreenMessageId = 0;
const int systemScreenQuitMessageId = 0;
const int systemScreenKeyPressedMessageId = 1;
const int systemScreenWriteScreenMessageId = 2;
const int systemScreenTimerMessageId = 3;
const int lastScreenMessageId = 3;

const int firstCompletionMessageId = 10;
const int tabCompletionInitMessageId = 10;
const int tabCompletionExitMessageId = 11;
const int tabCompletionRequestMessageId = 12;
const int tabCompletionReplyMessageId = 13;
const int tabCompletionErrorReplyMessageId = 14;
const int lastCompletionMessageId = 14;

const int firstUserMessageId = 100;

class Process;

class ProcessMessageQueues
{
public:
    void Add(int32_t md);
    void Remove(int32_t md);
    void CloseAll(cmajor::systemx::machine::Process* process);
private:
    std::vector<int32_t> mds;
};

class Msg
{
public:
    Msg(const std::vector<uint8_t>& data_);
    const std::vector<uint8_t>& Data() const { return data; }
private:
    std::vector<uint8_t> data;
};

int32_t MsgQ(cmajor::systemx::kernel::Process* process, int64_t nameAddr);
void CloseMsgQ(cmajor::systemx::machine::Process* process, int32_t md);
void CloseMsgQ(cmajor::systemx::machine::Process* process, int32_t md, bool remove);
void WaitMsgLocked(std::unique_lock<std::recursive_mutex>& lock, cmajor::systemx::machine::Process* process, int32_t md);
void WaitMsg(cmajor::systemx::machine::Process* process, int32_t md);
bool IsMsgQOpen(int32_t md);
void PutMsg(int32_t md, const std::vector<std::uint8_t>& msgData);
void PutMsg(cmajor::systemx::kernel::Process* process, int32_t md, int64_t msgDataAddr, int32_t msgSize);
int32_t GetMsgQueueLength(cmajor::systemx::machine::Process* process, int32_t md);
int32_t GetMsgSize(cmajor::systemx::machine::Process* process, int32_t md);
std::vector<uint8_t> GetMsg(int32_t md);
void GetMsg(cmajor::systemx::kernel::Process* process, int32_t md, int64_t bufferAttr);
void InitMsgQueue();
void DoneMsgQueue();

} // namespace cmajor::systemx::kernel
