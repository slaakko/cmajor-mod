// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.console.debugger;

import cmajor.debug.debugger;
import cmajor.debug.debugger.output.writer;
import util;
import std.core;

export namespace cmajor::debug {

class Console;

class ConsoleDebuggerOutputWriter : public DebuggerOutputWriter
{
public:
    ConsoleDebuggerOutputWriter(util::CodeFormatter& formatter_);
    void WriteLogMessage(const std::string& message) override;
    void WriteJsonValue(util::JsonValue* jsonValue) override;
    void WriteWarning(const std::string& warning) override;
private:
    util::CodeFormatter& formatter;
};

class ConsoleDebugger : public Debugger
{
public:
    ConsoleDebugger(const std::string& executable, const std::vector<std::string>& args, bool verbose_, util::CodeFormatter& formatter_, Console& console_, bool breakOnThrow_);
    ~ConsoleDebugger();
    void Prompt() override;
    void WriteTargetOuput(int handle, const std::string& s) override;
    void TargetInputPrompt() override;
    void Error(const std::string& msg) override;
    void StartProgram(bool breakOnThrow_) override;
    void WriteResult(util::JsonValue* result, Instruction* stoppedInstruction) override;
    void WriteSourceFile(SourceFile& sourceFile, int line, Instruction* stoppedInstruction) override;
    std::string GetTargetInputBytes() override;
    void ProcessConsoleOutput(GdbConsoleOutputRecord* record) override;
    void ProcessTargetOutput(GdbTargetOutputRecord* record) override;
    void ProcessLogOutput(GdbLogOutputRecord* record) override;
    void Proceed() override;
    void ResetConsole() override;
    void SetConsoleActive() override;
    void SetTargetRunning() override;
private:
    util::CodeFormatter& formatter;
    util::CodeFormatter outFormatter;
    util::CodeFormatter errorFormatter;
    Console& console;
    std::recursive_mutex outputMutex;
};

void RunDebuggerInteractive(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version);

} // namespace cmajor::debug
