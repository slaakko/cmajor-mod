// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.gdb.debugger;

import cmajor.debug;
import cmajor.debugger;
import cmajor.debugger.breakpoint;
import util;
import std.core;

export namespace cmajor::debugger {

class Mediator;

class GDBDebugger : public Debugger
{
public:
    GDBDebugger();
    ~GDBDebugger() override;
    void SetDebugLogger(DebugLogger* debugLogger) override;
    void SetMessageWriter(MessageWriter* messageWriter_) override;
    std::unique_ptr<Reply> Start(DebuggerStartParams& startParams) override;
    void Stop() override;
    std::unique_ptr<Reply> Run() override;
    std::unique_ptr<Reply> Continue() override;
    std::unique_ptr<Reply> Next() override;
    std::unique_ptr<Reply> Step() override;
    std::unique_ptr<Reply> Finish() override;
    std::unique_ptr<Reply> Until(const Location& loc) override;
    void SetBreakpoints(const std::vector<Breakpoint*>& breakpoints);
    void SetBreakpoint(Breakpoint* breakpoint);
    std::unique_ptr<Reply> Execute(Request* request);
    std::unique_ptr<Reply> ReadReply(Request* request);
    std::unique_ptr<Reply> ParseReplyLine(const std::string& line);
private:
    std::unique_ptr<util::Process> gdb;
    std::unique_ptr<DebugLogger> logger;
    std::unique_ptr<MessageWriter> messageWriter;
    std::unique_ptr<cmajor::debug::DebuggerOutputWriter> outputWriter;
    std::unique_ptr<cmajor::debug::DebugInfo> debugInfo;
    cmajor::debug::Instruction* stoppedInstruction;
    bool exited;
    int gdbExitCode;
};

} // namespace cmajor::debugger
