// =================================
// Copyright (c) 2024 Seppo Laakko
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
    std::unique_ptr<Reply> Until(const cmajor::info::db::Location& loc) override;
    int Depth() override;
    std::vector<cmajor::info::db::Location> Frames(int lowFrame, int highFrame) override;
    cmajor::info::db::CountReply Count(const cmajor::info::db::CountRequest& countRequest) override;
    cmajor::info::db::EvaluateReply Evaluate(const std::string& expression) override;
    cmajor::info::db::EvaluateChildReply EvaluateChild(const cmajor::info::db::EvaluateChildRequest& request) override;
    void SetBreakpoints(const std::vector<Breakpoint*>& breakpoints);
    void SetBreakpoint(Breakpoint* breakpoint);
    std::unique_ptr<Reply> Execute(Request* request) override;
    std::unique_ptr<Reply> ReadReply(Request* request);
    std::unique_ptr<Reply> ParseReplyLine(const std::string& line);
    cmajor::debug::DebugInfo* GetDebugInfo() override { return debugInfo.get(); }
    cmajor::debug::Instruction* StoppedInstruction() override { return stoppedInstruction; }
    cmajor::debug::DebuggerVariable GetNextDebuggerVariable() override;
private:
    std::unique_ptr<util::Process> gdb;
    std::unique_ptr<DebugLogger> logger;
    std::unique_ptr<MessageWriter> messageWriter;
    std::unique_ptr<cmajor::debug::DebuggerOutputWriter> outputWriter;
    std::unique_ptr<cmajor::debug::DebugInfo> debugInfo;
    cmajor::debug::Instruction* stoppedInstruction;
    bool exited;
    int gdbExitCode;
    int nextGdbVariableIndex;
};

} // namespace cmajor::debugger
