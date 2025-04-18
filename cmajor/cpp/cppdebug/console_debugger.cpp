// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.console.debugger;

import cmajor.debug.console;
import util;
import std.filesystem;

namespace cmajor::debug {

ConsoleDebuggerOutputWriter::ConsoleDebuggerOutputWriter(util::CodeFormatter& formatter_) : formatter(formatter_)
{
}

void ConsoleDebuggerOutputWriter::WriteLogMessage(const std::string& message)
{
    formatter.WriteLine(message);
}

void ConsoleDebuggerOutputWriter::WriteJsonValue(util::JsonValue* jsonValue)
{
    jsonValue->Write(formatter);
}

void ConsoleDebuggerOutputWriter::WriteWarning(const std::string& warning)
{
    formatter.WriteLine(warning);
}

ConsoleDebugger::ConsoleDebugger(const std::string& executable, const std::vector<std::string>& args, bool verbose_, util::CodeFormatter& formatter_, Console& console_,
    bool breakOnThrow_) : Debugger(verbose_, breakOnThrow_, new ConsoleDebuggerOutputWriter(formatter_), executable, args), formatter(formatter_), console(console_),
    outFormatter(std::cout), errorFormatter(std::cerr)
{
    StartDebugging(true);
}

ConsoleDebugger::~ConsoleDebugger()
{
    try
    {
        if (!console.Terminated())
        {
            if (Verbose())
            {
                std::lock_guard<std::recursive_mutex> lock(outputMutex);
                formatter.WriteLine("stopping GDB...");
            }
            StopGDB(*this);
        }
    }
    catch (...)
    {
    }
    try
    {
        if (Verbose())
        {
            std::lock_guard<std::recursive_mutex> lock(outputMutex);
            formatter.WriteLine("stopping CMDB session...");
        }
        StopCmdbSession();
    }
    catch (...)
    {
    }
}

void ConsoleDebugger::Prompt()
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    formatter.Write("cmdb> ");
}

void ConsoleDebugger::TargetInputPrompt()
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    formatter.Write("target input> ");
}

void ConsoleDebugger::Error(const std::string& msg)
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    formatter.WriteLine(msg);
}

void ConsoleDebugger::StartProgram(bool breakOnThrow_)
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    Debugger::StartProgram(breakOnThrow_);
}

void ConsoleDebugger::WriteResult(util::JsonValue* result, Instruction* stoppedInstruction)
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    result->Write(formatter);
    if (stoppedInstruction != nullptr)
    {
        stoppedInstruction->PrintSource(formatter);
    }
}

void ConsoleDebugger::WriteSourceFile(SourceFile& sourceFile, int line, Instruction* stoppedInstruction)
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    sourceFile.Print(formatter, line, stoppedInstruction, false);
}

void ConsoleDebugger::Proceed()
{
    console.Proceed();
}

void ConsoleDebugger::ResetConsole()
{
    console.Reset();
}

void ConsoleDebugger::SetConsoleActive()
{
    console.SetActive();
}

void ConsoleDebugger::SetTargetRunning()
{
    console.SetTargetRunning();
}

std::string ConsoleDebugger::GetTargetInputBytes()
{
    std::string targetInputLine = console.GetTargetInputLine();
    if (console.TargetInputEof())
    {
        return std::string();
    }
    else
    {
        std::string targetInputBytes;
        for (unsigned char c : targetInputLine)
        {
            targetInputBytes.append(util::ToHexString(static_cast<uint8_t>(c)));
        }
        targetInputBytes.append(util::ToHexString(static_cast<uint8_t>('\n')));
        return targetInputBytes;
    }
}

void ConsoleDebugger::WriteTargetOuput(int handle, const std::string& s)
{
    if (handle == 1)
    {
        outFormatter.Write(s);
    }
    else if (handle == 2)
    {
        errorFormatter.Write(s);
    }
}

void ConsoleDebugger::ProcessConsoleOutput(GdbConsoleOutputRecord* record)
{
    SetConsoleActive();
}

void ConsoleDebugger::ProcessTargetOutput(GdbTargetOutputRecord* record)
{
    std::lock_guard<std::recursive_mutex> lock(outputMutex);
    formatter.WriteLine(record->Text());
    SetConsoleActive();
}

void ConsoleDebugger::ProcessLogOutput(GdbLogOutputRecord* record)
{
    console.SetActive();
}

void RunConsole(Console* console)
{
    console->Run();
}

void RunDebuggerInteractive(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version)
{
    util::CodeFormatter formatter(std::cout);
    formatter.WriteLine("Cmajor debugger version " + version);
    Console console;
    ConsoleDebugger debugger(executable, args, verbose, formatter, console, breakOnThrow);
    console.SetDriver(&debugger);
    std::thread consoleThread{ RunConsole, & console };
    try
    {
        while (!debugger.Exiting())
        {
            try
            {
                debugger.ResetRunningFlag();
                debugger.ResetTargetOutputFlag();
                std::unique_ptr<DebuggerCommand> command = console.GetCommand();
                if (!command || console.Terminated())
                {
                    break;
                }
                command->Execute(debugger);
                debugger.Proceed();
                if (command->GetKind() != DebuggerCommand::Kind::repeatLatest)
                {
                    debugger.SetLatestCommand(command->Clone());
                }
            }
            catch (const std::exception& ex)
            {
                util::JsonObject* result = new util::JsonObject();
                result->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
                result->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
                result->Write(formatter);
                debugger.ResetConsole();
                debugger.Proceed();
            }
        }
        consoleThread.join();
    }
    catch (const std::exception&)
    {
        consoleThread.join();
        throw;
    }
}

} // namespace cmajor::debug
