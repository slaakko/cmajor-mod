// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/pool/pool_alloc.hpp>

export module cmajor.systemx.machine.clock;

import std.core;

export namespace cmajor::systemx::machine {

class Machine;

class ClockObserver
{
public:
    virtual ~ClockObserver();
    virtual void Tick() = 0;
};

class Clock
{
public:
    Clock();
    void SetMachine(Machine* machine_);
    void Start();
    void Stop();
    void Run();
    void AddObserver(ClockObserver* observer);
    void RemoveObserver(ClockObserver* observer);
private:
    Machine* machine;
    int tickMilliseconds;
    std::chrono::steady_clock::duration tickPeriod;
    std::condition_variable_any clockTickOrExiting;
    std::thread thread;
    std::list<ClockObserver*, boost::fast_pool_allocator<ClockObserver*>> observers;
};

} // namespace cmajor::systemx::machine
