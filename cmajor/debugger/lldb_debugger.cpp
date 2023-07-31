// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.lldb.debugger;

import std.filesystem;
import util;

namespace cmajor::debugger {

LLDBDebugger::LLDBDebugger()
{
}

void LLDBDebugger::SetDebugLogger(DebugLogger* logger_)
{
    logger.reset(logger_);
}

void LLDBDebugger::SetMessageWriter(MessageWriter* messageWriter_)
{
    messageWriter.reset(messageWriter_);
}

std::unique_ptr<Reply> LLDBDebugger::Start(DebuggerStartParams& startParams)
{
    std::string startCommand;
    std::string executableFilePath = startParams.executableFilePath;
    if (!executableFilePath.ends_with(".exe"))
    {
        executableFilePath.append(".exe");
    }
    if (!std::filesystem::exists(executableFilePath))
    {
        throw std::runtime_error("error: cannot start debugging: executable '" + executableFilePath + "' not found");
    }
    startCommand.append("lldb").append(" --no-use-colors").append(" ").append(executableFilePath);
    if (!startParams.programArguments.empty())
    {
        startCommand.append(" -- ").append(startParams.programArguments);
    }
    lldb.reset(new util::Process(startCommand,
        util::Process::Redirections::processStdIn |
        util::Process::Redirections::processStdOut |
        util::Process::Redirections::processStdErr));
    std::unique_ptr<Reply> reply;
    return reply;
}

void LLDBDebugger::Stop()
{
    if (lldb)
    {
        lldb->Terminate();
        lldb.reset();
    }
}

std::unique_ptr<Reply> LLDBDebugger::Run()
{
    lldb->WriteLine("run");
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> LLDBDebugger::Continue()
{
    lldb->WriteLine("continue");
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> LLDBDebugger::Next()
{
    lldb->WriteLine("next");
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> LLDBDebugger::Step()
{
    lldb->WriteLine("step");
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> LLDBDebugger::Finish()
{
    lldb->WriteLine("finish");
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> LLDBDebugger::Until(const Location& loc)
{
    lldb->WriteLine("until");
    return std::unique_ptr<Reply>();
}

} // namespace cmajor::debugger
