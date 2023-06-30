// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.debugger;

import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import std.core;

export namespace sxgmdb {

const uint8_t exit_code_stop = 255;

enum class DebuggerState
{
    idle = 0, waiting_for_command = 1, started = 2, singleStep = 3, running = 4
};

class DebuggerObserver
{
public:
    virtual ~DebuggerObserver();
    virtual void DebuggerReady() = 0;
    virtual void DebuggerError(const std::string& message) = 0;
    virtual void DebuggerProcessExit() = 0;
};

class Debugger : public cmajor::systemx::machine::Debugger
{
public:
    Debugger(cmajor::systemx::machine::Machine* machine_, cmajor::systemx::kernel::Process* process_);
    void SetObserver(DebuggerObserver* observer_);
    void Run();
    void Stop();
    void SingleStep();
    void StepOver();
    void Continue();
    void Release();
    void WaitForCommand();
    void AddBreakpoint(uint64_t address);
    void RemoveBreakpoint(uint64_t address);
    bool HasBreakpoint(uint64_t address) const;
    void ToggleBreakpoint(uint64_t address);
    void Intercept() override;
    void ProcessExit() override;
private:
    void NotifyReady();
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
    DebuggerObserver* observer;
    DebuggerState state;
    std::mutex mtx;
    std::condition_variable commandAvailableOrExitingVar;
    bool exiting;
    bool commandAvailable;
    std::set<uint64_t> breakpoints;
    std::set<uint64_t> breakpointsToRemove;
};

} // namespace sxgmdb
