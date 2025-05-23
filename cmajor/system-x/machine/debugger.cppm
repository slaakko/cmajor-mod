// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.debugger;

import std.core;

export namespace cmajor::systemx::machine {

class Processor;
class Machine;

class Debugger
{
public:
    virtual ~Debugger();
    virtual void Intercept(Processor* processor) = 0;
    virtual void ProcessExit() = 0;
};

} // namespace cmajor::systemx::machine
