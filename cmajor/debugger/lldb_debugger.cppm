// =================================
// Copyright (c) 2023 Seppo Laakko
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
    std::unique_ptr<Reply> Until(const Location& loc) override;
private:
    std::unique_ptr<util::Process> lldb;
    std::unique_ptr<DebugLogger> logger;
    std::unique_ptr<MessageWriter> messageWriter;
};

} // namespace cmajor::debugger
