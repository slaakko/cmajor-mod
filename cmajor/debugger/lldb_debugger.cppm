// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.lldb.debugger;

import cmajor.debugger;
import cmajor.debugger.breakpoint;
import util;
import std.core;

export namespace cmajor::debugger {

class LLDBDebugger : public Debugger
{
public:
    LLDBDebugger();
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
    cmajor::debug::DebugInfo* GetDebugInfo() override { return nullptr; }
    cmajor::debug::Instruction* StoppedInstruction() override { return nullptr; }
    cmajor::debug::DebuggerVariable GetNextDebuggerVariable() override;
    std::unique_ptr<Reply> Execute(Request* request) override;
private:
    std::unique_ptr<util::Process> lldb;
    std::unique_ptr<DebugLogger> logger;
    std::unique_ptr<MessageWriter> messageWriter;
};

} // namespace cmajor::debugger
