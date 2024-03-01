// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debugger;

import cmajor.debug.container;
import cmajor.debug.debugger.output.writer;
import cmajor.debug.debug.info;
import cmajor.debug.gdb;
import cmajor.debug.cmdb.session;
import util;
import std.core;

export namespace cmajor::debug {

export namespace debugger {}

class DebugInfo;
class BoundDebugNode;
class Instruction;
class DebuggerCommand;
struct SourceLocation;

std::unique_ptr<DebuggerCommand> ParseDebuggerCommand(std::string& currentSourceFilePath, const std::string& line);

class GdbBreakpoint
{
public:
    GdbBreakpoint(Instruction* instruction_);
    void SetNumber(int number_) { number = number_; }
    int Number() const { return number; }
    Instruction* GetInstruction() const { return instruction; }
    void SetDebuggerBreakpointId(const std::string& debuggerBreakpoinId_);
    const std::string& DebuggerBreakpointId() const { return debuggerBreakpointId; }
private:
    Instruction* instruction;
    int number;
    std::string debuggerBreakpointId;
};

class DebuggerBreakpoint
{
public:
    DebuggerBreakpoint(const std::string& id_);
    const std::string& Id() const { return id; }
    const std::vector<int>& GdbBreakpointNumbers() const { return gdbBreakpointNumbers; }
    void AddGdbBreakpointNumber(int gdbBreakpointNumber);
    void SetFrame(const Frame& frame_);
    const Frame& GetFrame() const { return frame; }
    std::unique_ptr<util::JsonValue> ToJson() const;
private:
    std::string id;
    std::vector<int> gdbBreakpointNumbers;
    Frame frame;
};

class DebuggerVariable
{
public:
    DebuggerVariable();
    DebuggerVariable(int index_, const std::string& gdbVarName_);
    const std::string& GdbVarName() const { return gdbVarName; }
    int Index() const { return index; }
private:
    int index;
    std::string gdbVarName;
};

class Debugger : public GdbDriver, public CmdbSessionClient
{
public:
    Debugger(bool verbose_, bool breakOnThrow_, DebuggerOutputWriter* outputWriter_, const std::string& executable_, const std::vector<std::string>& args_);
    enum class State
    {
        initializing, programStarted, running, stopped, programExitedNormally, programExited, signalReceived, exitingDebugger
    };
    void StartDebugging(bool startCmdbSession);
    void ResetRunningFlag() { wasRunning = false; }
    void ResetTargetOutputFlag() { targetOutput = false; }
    bool TargetWasRunning() const override { return wasRunning; }
    bool TargetOutput() const override { return targetOutput; }
    void ProcessReplyRecord(GdbReplyRecord* record) override;
    std::string& CurrentSourceFilePath() override { return currentSourceFilePath; }
    bool TargetRunning() const override { return state == State::running; }
    bool Exiting() const override { return state == State::exitingDebugger; }
    void Exit() override;
    bool LatestCommandWasRunningCommand() override;
    bool Run();
    void Help();
    void Next();
    void Step();
    bool Continue();
    void Finish();
    void Until(const SourceLocation& location);
    bool Break(std::unique_ptr<DebuggerBreakpoint>&& bp, const std::vector<Instruction*>& instructions);
    void Break(const SourceLocation& location);
    void SetBreakCondition(int breakpointId, const std::string& condition);
    void Delete(const std::string& breakpointId);
    void Delete(const std::string& breakpointId, bool printResult);
    void Depth();
    void Frames(int low, int high);
    void ShowBreakpoint(int breakpointId);
    void ShowBreakpoints();
    void List(const SourceLocation& location);
    void Print(const std::string& expression);
    void RepeatLatestCommand();
    bool IsStopInstruction(Instruction* instruction) const;
    void Evaluate(const std::string& expression);
    DIType* GetType(const std::string& expression);
    bool ExecuteGDBCommand(const GdbCommand& command);
    void ProcessReply(GdbCommand::Kind commandKind, GdbReply* reply);
    void ProcessBreakInsertReply(GdbReply* reply);
    void ProcessBreakDeleteReply(GdbReply* reply);
    void ProcessExecRunReply(GdbReply* reply);
    void ProcessExecNextReply(GdbReply* reply);
    void ProcessExecStepReply(GdbReply* reply);
    void ProcessExecContinueReply(GdbReply* reply);
    void ProcessExecFinishReply(GdbReply* reply);
    void ProcessExecUntilReply(GdbReply* reply);
    void ProcessStackInfoDepthReply(GdbReply* reply);
    void ProcessStackListFramesReply(GdbReply* reply);
    void ProcessVarCreateReply(GdbReply* reply);
    void ProcessBreakConditionReply(GdbReply* reply);
    void ProcessVarEvaluateReply(GdbReply* reply);
    bool ProcessExecStoppedRecord(GdbExecStoppedRecord* execStoppedRecord);
    virtual void StartProgram(bool breakOnThrow_);
    GdbBreakpoint* SetBreakpoint(Instruction* instruction);
    bool DeleteBreakpoint(Instruction* instruction);
    GdbBreakpoint* GetBreakpoint(Instruction* instruction) const;
    GdbBreakpoint* GetBreakpoint(int number) const;
    Instruction* GetInstructionForCppLocation(const std::string& cppFile, int cppLine);
    void SetLatestCommand(DebuggerCommand* latestCommand_);
    void SetState(State state_);
    static std::string StateStr(State state);
    int GetNextBreakpointNumber() { return nextBreakpointNumber++; }
    std::string GetNextTemporaryBreakpointId();
    DebuggerVariable GetNextDebuggerVariable();
    virtual void AddStopResultToResult();
    DIType* GetDynamicType(DIType* diType, BoundDebugNode* node);
    void AddDebuggerVariable(const DebuggerVariable& debuggerVariable);
    const DebuggerVariable* GetDebuggerVariable(int index) const;
    void ResetResult(util::JsonValue* result_) { result.reset(result_); }
    util::JsonValue* ReleaseResult() { return result.release(); }
    util::JsonValue* GetResult() { return result.get(); }
    Instruction* StoppedInstruction() const { return stoppedInstruction; }
    Container* GetContainer(ContainerClassTemplateKind containerKind, const std::string& containerVarExpr);
    void ClearBrowsingData();
    void AddToNextSet(std::set<Instruction*>& nextSet, Instruction* inst) const;
    void SetBreakOnThrow(bool breakOnThrow_, bool printResult);
    void SetThrowBreakpoints(bool printResult);
    void ClearThrowBreakpoints(bool printResult);
    bool SetCatchBreakpoints();
    void ClearCatchBreakpoints();
    bool Verbose() const { return verbose; }
    DebuggerOutputWriter* OutputWriter() { return outputWriter.get(); }
    void SetDebugInfo(DebugInfo* debugInfo_);
    DebugInfo* GetDebugInfo() const { return debugInfo.get(); }
    virtual void WriteResult(util::JsonValue* result, Instruction* stoppedInstruction) = 0;
    virtual void WriteSourceFile(SourceFile& sourceFile, int line, Instruction* stoppedInstruction) = 0;
    virtual void SetConsoleActive() = 0;
    virtual void SetTargetRunning() = 0;
    virtual void ProcessConsoleOutput(GdbConsoleOutputRecord* record) = 0;
    virtual void ProcessTargetOutput(GdbTargetOutputRecord* record) = 0;
    virtual void ProcessLogOutput(GdbLogOutputRecord* record) = 0;
    const std::vector<Frame>& Frames() const { return frames; }
private:
    std::string executable;
    std::vector<std::string> args;
    bool breakOnThrow;
    std::unique_ptr<DebuggerOutputWriter> outputWriter;
    State state;
    bool wasRunning;
    bool targetOutput;
    Instruction* stoppedInstruction;
    std::unique_ptr<util::JsonValue> result;
    std::unique_ptr<util::JsonValue> stopResult;
    std::vector<Frame> frames;
    bool verbose;
    std::unique_ptr<DebugInfo> debugInfo;
    std::unordered_map<Instruction*, GdbBreakpoint*> gdbBreakpointsByInstruction;
    std::unordered_map<int, GdbBreakpoint*> gdbBreakpointsByNumber;
    std::map<std::string, DebuggerBreakpoint*> debuggerBreakpointMap;
    std::unique_ptr<DebuggerCommand> latestCommand;
    int nextBreakpointNumber;
    int nextTempBreakpointNumber;
    int nextGdbVariableIndex;
    std::unordered_map<int, DebuggerVariable> debuggerVariableMap;
    std::unordered_map<uint64_t, Container*> containerMap;
    std::vector<std::unique_ptr<Container>> containers;
    SourceLocation listLocation;
    std::string currentSourceFilePath;
    std::string debuggerBreakpointId;
    std::string throwBreakpointsId;
    std::string catchBreakpointsId;
};

class DebuggerCommand
{
public:
    enum class Kind
    {
        exit, help, next, step, continue_, finish, until, break_, delete_, depth, frames, showBreakpoint, showBreakpoints, list, print, setCondition,
        setBreakOnThrow, repeatLatest
    };
    DebuggerCommand(Kind kind_);
    virtual ~DebuggerCommand();
    virtual bool IsRunningCommand(GdbDriver& driver) const { return false; }
    virtual void Execute(Debugger& debugger) = 0;
    virtual DebuggerCommand* Clone() = 0;
    Kind GetKind() const { return kind; }
private:
    Kind kind;
};

class DebuggerExitCommand : public DebuggerCommand
{
public:
    DebuggerExitCommand();
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerHelpCommand : public DebuggerCommand
{
public:
    DebuggerHelpCommand();
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerNextCommand : public DebuggerCommand
{
public:
    DebuggerNextCommand();
    bool IsRunningCommand(GdbDriver& driver) const override { return true; }
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerStepCommand : public DebuggerCommand
{
public:
    DebuggerStepCommand();
    bool IsRunningCommand(GdbDriver& driver) const override { return true; }
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerContinueCommand : public DebuggerCommand
{
public:
    DebuggerContinueCommand();
    bool IsRunningCommand(GdbDriver& driver) const override { return true; }
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerFinishCommand : public DebuggerCommand
{
public:
    DebuggerFinishCommand();
    bool IsRunningCommand(GdbDriver& driver) const override { return true; }
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerUntilCommand : public DebuggerCommand
{
public:
    DebuggerUntilCommand(const SourceLocation& location_);
    bool IsRunningCommand(GdbDriver& driver) const override { return true; }
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    SourceLocation location;
};

class DebuggerBreakCommand : public DebuggerCommand
{
public:
    DebuggerBreakCommand(const SourceLocation& location_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    SourceLocation location;
};

class DebuggerDeleteCommand : public DebuggerCommand
{
public:
    DebuggerDeleteCommand(int breakpointNumber_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    int breakpointNumber;
};

class DebuggerDepthCommand : public DebuggerCommand
{
public:
    DebuggerDepthCommand();
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerFramesCommand : public DebuggerCommand
{
public:
    DebuggerFramesCommand(int low_, int high_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    int low;
    int high;
};

class DebuggerShowBreakpointCommand : public DebuggerCommand
{
public:
    DebuggerShowBreakpointCommand(int breakpointId_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    int breakpointId;
};

class DebuggerShowBreakpointsCommand : public DebuggerCommand
{
public:
    DebuggerShowBreakpointsCommand();
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

class DebuggerListCommand : public DebuggerCommand
{
public:
    DebuggerListCommand(const SourceLocation& location_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    SourceLocation location;
};

class DebuggerPrintCommand : public DebuggerCommand
{
public:
    DebuggerPrintCommand(const std::string& expression_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
    const std::string& Expression() const { return expression; }
private:
    std::string expression;
};

class DebuggerSetConditionCommand : public DebuggerCommand
{
public:
    DebuggerSetConditionCommand(int breakpointNumber_, const std::string& expression_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
    int BreakpointNumber() const { return breakpointNumber; }
    const std::string& Expression() const { return expression; }
private:
    int breakpointNumber;
    std::string expression;
};

class DebuggerSetBreakOnThrowCommand : public DebuggerCommand
{
public:
    DebuggerSetBreakOnThrowCommand(bool breakOnThrow_);
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
private:
    bool breakOnThrow;
};

class DebuggerRepeatLatestCommand : public DebuggerCommand
{
public:
    DebuggerRepeatLatestCommand();
    bool IsRunningCommand(GdbDriver& driver) const override;
    void Execute(Debugger& debugger) override;
    DebuggerCommand* Clone() override;
};

} // namespace cmajor::debug
