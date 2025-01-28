// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.clock;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

struct AlarmMsg
{
    AlarmMsg(int32_t md_, const std::vector<uint8_t>& msgData_);
    int32_t md;
    std::vector<uint8_t> msgData;
};

struct Alarm
{
    Alarm(Process* process_, std::chrono::steady_clock::time_point dueTime_, bool sleep_, AlarmMsg* msg_) : id(-1), process(process_), dueTime(dueTime_), sleep(sleep_), msg(msg_) {}
    int id;
    Process* process;
    std::chrono::steady_clock::time_point dueTime;
    bool sleep;
    AlarmMsg* msg;
};

class Clock : public cmajor::systemx::machine::ClockObserver
{
public:
    static void Init();
    static void Done();
    static Clock& Instance() { return *instance; }
    void SetMachine(cmajor::systemx::machine::Machine* machine_);
    void Start();
    void Stop();
    void Tick() override;
    void Schedule(Alarm& alarm);
private:
    Clock();
    static std::unique_ptr<Clock> instance;
    cmajor::systemx::machine::Machine* machine;
    std::vector<Alarm> alarms;
    int nextAlarmId;
};

void InitClock();
void DoneClock();

} // namespace cmajor::systemx::kernel

