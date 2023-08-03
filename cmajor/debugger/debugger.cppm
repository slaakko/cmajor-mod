// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger;

import cmajor.debugger.breakpoint;
import cmajor.debugger.debug.logger;
import cmajor.debugger.message.writer;
import cmajor.debug;
import cmajor.info;
import util;
import std.core;

export namespace cmajor::debugger {

struct DebuggerStartParams
{
    std::string executableFilePath;
    std::string programArguments;
    std::vector<Breakpoint*> breakpoints;
};

struct TargetState
{
    std::string stopReason;
    std::string exitCode;
    std::string breakpointId;
    std::string signalName;
    std::string signalMeaning;
};

enum class DebugResultKind
{
    start = 0, error = 1, control = 2, setBreakpoint = 3
};

class Debugger
{
public:
    Debugger();
    virtual ~Debugger();
    virtual void SetDebugLogger(DebugLogger* debugLogger) = 0;
    virtual void SetMessageWriter(MessageWriter* messageWriter) = 0;
    virtual std::unique_ptr<Reply> Start(DebuggerStartParams& startParams) = 0;
    virtual void Stop() = 0;
    virtual std::unique_ptr<Reply> Run() = 0;
    virtual std::unique_ptr<Reply> Continue() = 0;
    virtual std::unique_ptr<Reply> Step() = 0;
    virtual std::unique_ptr<Reply> Next() = 0;
    virtual std::unique_ptr<Reply> Finish() = 0;
    virtual std::unique_ptr<Reply> Until(const cmajor::info::db::Location& loc) = 0;
    virtual int Depth() = 0;
    virtual std::vector<cmajor::info::db::Location> Frames(int lowFrame, int highFrame) = 0;
};

} // namespace cmajor::debugger
