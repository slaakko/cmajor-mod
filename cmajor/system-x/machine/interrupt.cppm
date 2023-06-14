// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.interrupt;

import std.core;

export namespace cmajor::systemx::machine {

class Processor;

const uint8_t irq_software = 0;
const uint8_t irq_clock = 1;
const uint64_t SOFTWARE_INTERRUPT_BIT = static_cast<uint64_t>(1) << irq_software;
const uint64_t CLOCK_INTERRUPT_BIT = static_cast<uint64_t>(1) << irq_clock;
const uint64_t ALL_INTERRUPT_BITS = 0xFFFFFFFFFFFFFFFF;

class InterruptHandler
{
public:
    virtual ~InterruptHandler();
    virtual void HandleInterrupt(Processor& processor) = 0;
};

void SetInterruptHandler(uint8_t irq, InterruptHandler* handler);
InterruptHandler* GetInterruptHandler(uint8_t irq);
void InitInterrupt();
void DoneInterrupt();

} // namespace cmajor::systemx::machine
