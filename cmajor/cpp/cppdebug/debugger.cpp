// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.debugger;

import cmajor.debug.console;
import cmajor.debug.container;
import cmajor.debug.command.parser;
import cmajor.debug.command.lexer;
import cmajor.debug.expr.lexer;
import cmajor.debug.expr.parser;
import cmajor.debug.bound.debug.expr;
import cmajor.debug.debug.expr.binder;
import cmajor.debug.debug.expression.evaluator;
import cmajor.debug.cmdb.session;
import cmajor.debug.container.subscript.translator;
import cmajor.debug.gdb;
import cmajor.debug.token.value.parser;
import util;
import std.filesystem;

namespace cmajor::debug {

std::unique_ptr<DebuggerCommand> ParseDebuggerCommand(std::string& currentSourceFilePath, const std::string& line)
{
    std::u32string commandLine = util::ToUtf32(line);
    commandLine.append(U"\r\n");
    auto lexer = cmajor::debug::command::lexer::MakeLexer(commandLine.c_str(), commandLine.c_str() + commandLine.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<DebuggerCommand> command = cmajor::debug::command::parser::DebugCommandParser<LexerType>::Parse(lexer, &currentSourceFilePath);
    return command;
}

GdbBreakpoint::GdbBreakpoint(Instruction* instruction_) : instruction(instruction_), number(-1)
{
}

void GdbBreakpoint::SetDebuggerBreakpointId(const std::string& debuggerBreakpoinId_)
{
    debuggerBreakpointId = debuggerBreakpoinId_;
}

DebuggerBreakpoint::DebuggerBreakpoint(const std::string& id_) : id(id_), frame()
{
}

void DebuggerBreakpoint::AddGdbBreakpointNumber(int gdbBreakpointNumber)
{
    gdbBreakpointNumbers.push_back(gdbBreakpointNumber);
}

void DebuggerBreakpoint::SetFrame(const Frame& frame_)
{
    frame = frame_;
}

std::unique_ptr<util::JsonValue> DebuggerBreakpoint::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    jsonObject->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(id))));
    jsonObject->AddField(U"frame", frame.ToJson(false));
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

Frame GetFrame(GdbTupleValue* frameValue)
{
    Frame frame;
    GdbValue* levelValue = frameValue->GetField("level");
    if (levelValue && levelValue->GetKind() == GdbValue::Kind::string)
    {
        try
        {
            frame.level = std::stoi(static_cast<GdbStringValue*>(levelValue)->Value());
        }
        catch (const std::exception&)
        {
            frame.level = -1;
        }
    }
    GdbValue* funcValue = frameValue->GetField("func");
    if (funcValue && funcValue->GetKind() == GdbValue::Kind::string)
    {
        frame.func = static_cast<GdbStringValue*>(funcValue)->Value();
    }
    GdbValue* fileValue = frameValue->GetField("file");
    if (fileValue && fileValue->GetKind() == GdbValue::Kind::string)
    {
        frame.file = static_cast<GdbStringValue*>(fileValue)->Value();
    }
    GdbValue* lineValue = frameValue->GetField("line");
    if (lineValue && lineValue->GetKind() == GdbValue::Kind::string)
    {
        frame.line = std::stoi(static_cast<GdbStringValue*>(lineValue)->Value());
    }
    return frame;
}

bool GetBreakpointResults(GdbResults* results, std::string& file, int& line, int& number, std::unique_ptr<util::JsonValue>& result)
{
    file.clear();
    line = 0;
    number = -1;
    try
    {
        GdbValue* bkptValue = results->GetField("bkpt");
        if (bkptValue)
        {
            if (bkptValue->GetKind() == GdbValue::Kind::tuple)
            {
                GdbTupleValue* tuple = static_cast<GdbTupleValue*>(bkptValue);
                GdbValue* fileValue = tuple->GetField("file");
                if (fileValue && fileValue->GetKind() == GdbValue::Kind::string)
                {
                    file = static_cast<GdbStringValue*>(fileValue)->Value();
                }
                GdbValue* lineValue = tuple->GetField("line");
                if (lineValue && lineValue->GetKind() == GdbValue::Kind::string)
                {
                    line = std::stoi(static_cast<GdbStringValue*>(lineValue)->Value());
                }
                GdbValue* numberValue = tuple->GetField("number");
                if (numberValue && numberValue->GetKind() == GdbValue::Kind::string)
                {
                    number = std::stoi(static_cast<GdbStringValue*>(numberValue)->Value());
                }
                if (result && result->Type() == util::JsonValueType::object)
                {
                    util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
                    resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        if (!result)
        {
            result.reset(new util::JsonObject());
        }
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
            resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
            resultObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        }
    }
    return !file.empty() && line != 0 && number != -1;
}

bool GetStoppedResults(GdbResults* results, std::unique_ptr<util::JsonValue>& result, std::unique_ptr<util::JsonObject>& stopObject, Frame& frame, Debugger& debugger)
{
    util::JsonObject* resultObject = nullptr;
    if (result && result->Type() == util::JsonValueType::object)
    {
        resultObject = static_cast<util::JsonObject*>(result.get());
    }
    stopObject.reset(new util::JsonObject());
    try
    {
        GdbValue* reasonValue = results->GetField("reason");
        if (resultObject)
        {
            resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        }
        if (reasonValue && reasonValue->GetKind() == GdbValue::Kind::string)
        {
            std::string reason = static_cast<GdbStringValue*>(reasonValue)->Value();
            stopObject->AddField(U"reason", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(reason))));
            if (reason == "exited")
            {
                GdbValue* exitCodeValue = results->GetField("exit-code");
                if (exitCodeValue && exitCodeValue->GetKind() == GdbValue::Kind::string)
                {
                    std::string exitCode = static_cast<GdbStringValue*>(exitCodeValue)->Value();
                    stopObject->AddField(U"exitCode", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(exitCode))));
                }
            }
            else if (reason == "breakpoint-hit")
            {
                GdbValue* breakpointNumberValue = results->GetField("bkptno");
                if (breakpointNumberValue && breakpointNumberValue->GetKind() == GdbValue::Kind::string)
                {
                    std::string breakpointNumberStr = static_cast<GdbStringValue*>(breakpointNumberValue)->Value();
                    stopObject->AddField(U"gdbBreakpointNumber", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(breakpointNumberStr))));
                    int gdbBreakpointNumber = std::stoi(breakpointNumberStr);
                    GdbBreakpoint* gdbBreakpoint = debugger.GetBreakpoint(gdbBreakpointNumber);
                    if (gdbBreakpoint && !gdbBreakpoint->DebuggerBreakpointId().empty())
                    {
                        stopObject->AddField(U"breakpointId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(gdbBreakpoint->DebuggerBreakpointId()))));
                    }
                }
            }
            else if (reason == "signal-received")
            {
                GdbValue* signalNameValue = results->GetField("signal-name");
                if (signalNameValue && signalNameValue->GetKind() == GdbValue::Kind::string)
                {
                    std::string signalName = static_cast<GdbStringValue*>(signalNameValue)->Value();
                    stopObject->AddField(U"signalName", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(signalName))));
                }
                GdbValue* signalMeaningValue = results->GetField("signal-meaning");
                if (signalMeaningValue && signalMeaningValue->GetKind() == GdbValue::Kind::string)
                {
                    std::string signalMeaning = static_cast<GdbStringValue*>(signalMeaningValue)->Value();
                    stopObject->AddField(U"signalMeaning", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(signalMeaning))));
                }
            }
        }
        GdbValue* threadIdValue = results->GetField("thread-id");
        if (threadIdValue && threadIdValue->GetKind() == GdbValue::Kind::string)
        {
            stopObject->AddField(U"threadId",
                std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(static_cast<GdbStringValue*>(threadIdValue)->Value()))));
        }
        GdbValue* frameValue = results->GetField("frame");
        if (frameValue && frameValue->GetKind() == GdbValue::Kind::tuple)
        {
            frame = GetFrame(static_cast<GdbTupleValue*>(frameValue));
            stopObject->AddField(U"cppFrame", frame.ToJson(false));
        }
    }
    catch (const std::exception& ex)
    {
        if (resultObject)
        {
            resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
            resultObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        }
        return false;
    }
    return true;
}

bool GetStackDepthResult(GdbResults* results, std::unique_ptr<util::JsonValue>& depthResult)
{
    util::JsonObject* jsonObject = new util::JsonObject();
    try
    {
        GdbValue* depthValue = results->GetField("depth");
        if (depthValue && depthValue->GetKind() == GdbValue::Kind::string)
        {
            jsonObject->AddField(U"depth", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(static_cast<GdbStringValue*>(depthValue)->Value()))));
            jsonObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        }
        depthResult.reset(jsonObject);
        return true;
    }
    catch (const std::exception& ex)
    {
        jsonObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
        jsonObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
    }
    depthResult.reset(jsonObject);
    return false;
}

bool GetStackListFramesResult(GdbResults* results, std::unique_ptr<util::JsonValue>& cppStackResult, std::vector<Frame>& cppFrames)
{
    util::JsonObject* jsonObject = new util::JsonObject();
    try
    {
        util::JsonArray* framesArray = new util::JsonArray();
        GdbValue* stackValue = results->GetField("stack");
        if (stackValue && stackValue->GetKind() == GdbValue::Kind::list)
        {
            GdbListValue* list = static_cast<GdbListValue*>(stackValue);
            int n = list->Count();
            for (int i = 0; i < n; ++i)
            {
                GdbValue* frameValue = list->GetValue(i);
                if (frameValue && frameValue->GetKind() == GdbValue::Kind::result)
                {
                    GdbResult* result = static_cast<GdbResult*>(frameValue);
                    GdbValue* value = result->Value();
                    if (value && value->GetKind() == GdbValue::Kind::tuple)
                    {
                        Frame frame = GetFrame(static_cast<GdbTupleValue*>(value));
                        framesArray->AddItem(frame.ToJson(true));
                        cppFrames.push_back(frame);
                    }
                }
            }
        }
        jsonObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        jsonObject->AddField(U"cppFrames", std::unique_ptr<util::JsonValue>(framesArray));
        cppStackResult.reset(jsonObject);
        return true;
    }
    catch (const std::exception& ex)
    {
        jsonObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
        jsonObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
    }
    cppStackResult.reset(jsonObject);
    return false;
}

bool GetVarCreateResult(GdbResults* results, std::unique_ptr<util::JsonValue>& result)
{
    try
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        GdbValue* value = results->GetField("value");
        if (value && value->GetKind() == GdbValue::Kind::string)
        {
            resultObject->AddField(U"value", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(static_cast<GdbStringValue*>(value)->Value()))));
        }
        result.reset(resultObject);
        return true;
    }
    catch (const std::exception& ex)
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
        resultObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        result.reset(resultObject);
    }
    return false;
}

bool GetBreakConditionResult(GdbResults* results, std::unique_ptr<util::JsonValue>& result)
{
    try
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        result.reset(resultObject);
        return true;
    }
    catch (const std::exception& ex)
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
        resultObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        result.reset(resultObject);
    }
    return false;
}

bool GetVarEvaluateResult(GdbResults* results, std::unique_ptr<util::JsonValue>& result)
{
    try
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        GdbValue* value = results->GetField("value");
        if (value && value->GetKind() == GdbValue::Kind::string)
        {
            resultObject->AddField(U"value", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(static_cast<GdbStringValue*>(value)->Value()))));
        }
        result.reset(resultObject);
        return true;
    }
    catch (const std::exception& ex)
    {
        util::JsonObject* resultObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(false)));
        resultObject->AddField(U"error", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        result.reset(resultObject);
    }
    return false;
}

DebuggerVariable::DebuggerVariable() : index(-1)
{
}

DebuggerVariable::DebuggerVariable(int index_, const std::string& gdbVarName_) : index(index_), gdbVarName(gdbVarName_)
{
}

Debugger::Debugger(bool verbose_, bool breakOnThrow_, DebuggerOutputWriter* outputWriter_, const std::string& executable_, const std::vector<std::string>& args_) :
    executable(executable_), args(args_), outputWriter(outputWriter_), verbose(verbose_), state(State::initializing), wasRunning(false), targetOutput(false),
    nextBreakpointNumber(1), nextTempBreakpointNumber(1), nextGdbVariableIndex(1), stoppedInstruction(nullptr), breakOnThrow(breakOnThrow_)
{
}

void Debugger::StartDebugging(bool startCmdbSession)
{
    std::string cmdbFilePath;
    if (executable.ends_with(".exe"))
    {
        cmdbFilePath = util::Path::ChangeExtension(executable, ".cmdb");
    }
    else
    {
        cmdbFilePath = executable + ".cmdb";
    }
    if (std::filesystem::exists(cmdbFilePath))
    {
        if (Verbose())
        {
            OutputWriter()->WriteLogMessage("> " + cmdbFilePath);
        }
        std::unique_ptr<DebugInfo> debugInfo = ReadDebugInfo(cmdbFilePath);
        SetDebugInfo(debugInfo.release());
    }
    else
    {
        throw std::runtime_error("error: debug information '" + cmdbFilePath + "' file for executable '" + executable + "' not found");
    }
    std::string cmdbSessionFilePath;
    if (startCmdbSession)
    {
        if (executable.ends_with(".exe"))
        {
            cmdbSessionFilePath = util::Path::ChangeExtension(executable, ".cmdbs");
        }
        else
        {
            cmdbSessionFilePath = executable + ".cmdbs";
        }
        StartCmdbSession(cmdbSessionFilePath, OutputWriter(), this, Verbose());
    }
    RemoveCmdbSessionFileGuard removeSessionFileGuard(cmdbSessionFilePath);
    if (Verbose())
    {
        OutputWriter()->WriteLogMessage("starting GDB...");
    }
    if (Verbose())
    {
        std::string argsStr;
        bool first = true;
        for (const std::string& arg : args)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                argsStr.append(" ");
            }
            argsStr.append(arg);
        }
        OutputWriter()->WriteLogMessage("executable=" + executable + ", args=[" + argsStr + "]");
    }
    StartGDB(executable, args, *this);
    if (Verbose())
    {
        GdbReply* startReply = GetGDBStartReply();
        std::unique_ptr<util::JsonValue> value = startReply->ToJson();
        OutputWriter()->WriteJsonValue(value.get());
    }
    StartProgram(breakOnThrow);
}

void Debugger::Exit()
{
    SetState(State::exitingDebugger);
}

void Debugger::StartProgram(bool breakOnThrow_)
{
    result.reset(new util::JsonObject());
    Instruction* mainFunctionEntryInstruction = debugInfo->GetMainFunctionEntryInstruction();
    debuggerBreakpointId.clear();
    GdbBreakpoint* bp = SetBreakpoint(mainFunctionEntryInstruction);
    if (bp != nullptr)
    {
        bool succeeded = Run();
        if (succeeded)
        {
            succeeded = DeleteBreakpoint(bp->GetInstruction());
            if (succeeded)
            {
                if (breakOnThrow_)
                {
                    SetBreakOnThrow(breakOnThrow_, false);
                }
                if (verbose)
                {
                    outputWriter->WriteLogMessage("program started.");
                }
                SetState(State::programStarted);
                AddStopResultToResult();
                Instruction* instruction = nullptr;
                if (state != State::programExitedNormally && state != State::programExited)
                {
                    instruction = stoppedInstruction;
                }
                WriteResult(result.get(), instruction);
            }
        }
    }
    Prompt();
}

GdbBreakpoint* Debugger::SetBreakpoint(Instruction* instruction)
{
    GdbBreakpoint* breakpoint = new GdbBreakpoint(instruction);
    gdbBreakpointsByInstruction[instruction] = breakpoint;
    GdbBreakInsertCommand breakInsertCommand(instruction->GetExplicitCppLocationArgs());
    bool succeeded = ExecuteGDBCommand(breakInsertCommand);
    if (succeeded)
    {
        return breakpoint;
    }
    else
    {
        DeleteBreakpoint(instruction);
        return nullptr;
    }
}

bool Debugger::DeleteBreakpoint(Instruction* instruction)
{
    bool succeeded = false;
    GdbBreakpoint* breakpoint = GetBreakpoint(instruction);
    if (breakpoint)
    {
        if (breakpoint->Number() != -1)
        {
            GdbBreakDeleteCommand breakDeleteComand(std::to_string(breakpoint->Number()));
            succeeded = ExecuteGDBCommand(breakDeleteComand);
            gdbBreakpointsByNumber.erase(breakpoint->Number());
        }
        auto it = gdbBreakpointsByInstruction.find(instruction);
        if (it != gdbBreakpointsByInstruction.cend())
        {
            GdbBreakpoint* bp = it->second;
            delete bp;
            gdbBreakpointsByInstruction.erase(instruction);
        }
    }
    return succeeded;
}

GdbBreakpoint* Debugger::GetBreakpoint(Instruction* instruction) const
{
    auto it = gdbBreakpointsByInstruction.find(instruction);
    if (it != gdbBreakpointsByInstruction.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

GdbBreakpoint* Debugger::GetBreakpoint(int number) const
{
    auto it = gdbBreakpointsByNumber.find(number);
    if (it != gdbBreakpointsByNumber.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Instruction* Debugger::GetInstructionForCppLocation(const std::string& cppFile, int cppLine)
{
    try
    {
        std::string projectDir = util::Path::GetDirectoryName(cppFile);
        Project* project = debugInfo->GetProjectByPath(projectDir);
        if (project)
        {
            std::string compileUnitBaseName = util::Path::GetFileNameWithoutExtension(cppFile);
            CompileUnit* compileUnit = project->GetCompileUnit(compileUnitBaseName);
            if (compileUnit)
            {
                Instruction* instruction = compileUnit->GetInstruction(cppLine);
                return instruction;
            }
        }
    }
    catch (const std::exception& ex)
    {
        if (!result)
        {
            result.reset(new util::JsonObject());
        }
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
            resultObject->AddField(U"instGetError", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ex.what()))));
        }
    }
    return nullptr;
}

void Debugger::SetLatestCommand(DebuggerCommand* latestCommand_)
{
    latestCommand.reset(latestCommand_);
}

bool Debugger::LatestCommandWasRunningCommand()
{
    if (!latestCommand) return false;
    return latestCommand->IsRunningCommand(*this);
}

void Debugger::SetState(State state_)
{
    state = state_;
}

std::string Debugger::StateStr(State state)
{
    switch (state)
    {
    case State::initializing: return "initializing";
    case State::programStarted: return "program started";
    case State::running: return "running";
    case State::stopped: return "stopped";
    case State::programExitedNormally: return "program exited normally";
    case State::programExited: return "program exited ";
    case State::signalReceived: return "signal received";
    case State::exitingDebugger: return "exiting debugger";
    }
    return std::string();
}

std::string Debugger::GetNextTemporaryBreakpointId()
{
    int tempBpNumber = nextTempBreakpointNumber++;
    return "t" + std::to_string(tempBpNumber);
}

DebuggerVariable Debugger::GetNextDebuggerVariable()
{
    int index = nextGdbVariableIndex++;
    std::string variableName = "v" + std::to_string(index);
    return DebuggerVariable(index, variableName);
}

void Debugger::AddStopResultToResult()
{
    if (stopResult)
    {
        if (!result)
        {
            result.reset(new util::JsonObject());
        }
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
            resultObject->AddField(U"stopped", std::move(stopResult));
        }
    }
}

bool Debugger::Run()
{
    ClearBrowsingData();
    GdbExecRunCommand execRunCommand;
    bool succeeded = ExecuteGDBCommand(execRunCommand);
    if (state == State::stopped)
    {
        if (stoppedInstruction != nullptr)
        {
            if (stoppedInstruction->CppLineIndex() == 0 && IsStopInstruction(stoppedInstruction))
            {
                AddStopResultToResult();
                WriteResult(result.get(), stoppedInstruction);
            }
        }
    }
    else
    {
        AddStopResultToResult();
        Instruction* instruction = nullptr;
        if (state != State::programExitedNormally && state != State::programExited)
        {
            instruction = stoppedInstruction;
        }
        WriteResult(result.get(), instruction);
    }
    return succeeded;
}

void Debugger::Help()
{
    outputWriter->WriteLogMessage("(e)xit | (q)uit |");
    outputWriter->WriteLogMessage("(n)ext |");
    outputWriter->WriteLogMessage("(s)tep |");
    outputWriter->WriteLogMessage("(c)ontinue |");
    outputWriter->WriteLogMessage("(f)inish |");
    outputWriter->WriteLogMessage("(u)ntil <location> |");
    outputWriter->WriteLogMessage("(b)reak <location> |");
    outputWriter->WriteLogMessage("(d)elete <breakpoint_number> |");
    outputWriter->WriteLogMessage("depth |");
    outputWriter->WriteLogMessage("frames [low high] |");
    outputWriter->WriteLogMessage("show breakpoint <breakpoint_number> |");
    outputWriter->WriteLogMessage("show breakpoints");
    outputWriter->WriteLogMessage("list (<location> | *) |");
    outputWriter->WriteLogMessage("print <expression>");
}

void Debugger::Next()
{
    bool succeeded = false;
    ClearBrowsingData();
    result.reset(new util::JsonObject());
    if (state != State::stopped && state != State::programStarted)
    {
        throw std::runtime_error("error: state is '" + StateStr(state) + "'");
    }
    std::set<Instruction*> next;
    if (stoppedInstruction != nullptr)
    {
        AddToNextSet(next, stoppedInstruction);
    }
    if (stoppedInstruction != nullptr)
    {
        if ((stoppedInstruction->GetFlags() & InstructionFlags::throwInst) != InstructionFlags::none)
        {
            bool succeeded = SetCatchBreakpoints();
            if (succeeded)
            {
                Continue();
                ClearCatchBreakpoints();
            }
            return;
        }
    }
    Instruction* prevStoppedInstruction = stoppedInstruction;
    bool stop = false;
    while (!stop)
    {
        stoppedInstruction = nullptr;
        GdbExecNextCommand execNextCommand;
        succeeded = ExecuteGDBCommand(execNextCommand);
        if (succeeded)
        {
            if (state == State::stopped)
            {
                if (stoppedInstruction != nullptr)
                {
                    AddToNextSet(next, stoppedInstruction);
                    if (stoppedInstruction->CppLineIndex() == 0)
                    {
                        bool foundInNext = next.find(stoppedInstruction) != next.cend();
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction() ||
                            stoppedInstruction->GetSourceSpan().line > prevStoppedInstruction->GetSourceSpan().line || foundInNext)
                        {
                            if (IsStopInstruction(stoppedInstruction) || foundInNext)
                            {
                                AddStopResultToResult();
                                WriteResult(result.get(), stoppedInstruction);
                                stop = true;
                            }
                        }
                    }
                    else
                    {
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction())
                        {
                            if (IsStopInstruction(stoppedInstruction))
                            {
                                AddStopResultToResult();
                                WriteResult(result.get(), stoppedInstruction);
                                stop = true;
                            }
                        }
                    }
                }
            }
            else
            {
                AddStopResultToResult();
                Instruction* instruction = nullptr;
                if (state != State::programExitedNormally && state != State::programExited)
                {
                    instruction = stoppedInstruction;
                }
                WriteResult(result.get(), instruction);
                stop = true;
            }
        }
        else
        {
            AddStopResultToResult();
            if (result)
            {
                WriteResult(result.get(), nullptr);
            }
            stop = true;
        }
    }
}

void Debugger::Step()
{
    bool step = true;
    if (state == State::stopped && stoppedInstruction != nullptr && (stoppedInstruction->GetFlags() & InstructionFlags::endBrace) != InstructionFlags::none)
    {
        step = false;
        GdbStackListFramesCommand frames(1, 1);
        bool succeeded = ExecuteGDBCommand(frames);
        if (succeeded)
        {
            if (result->Type() == util::JsonValueType::object)
            {
                util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
                util::JsonValue* framesValue = resultObject->GetField(U"frames");
                if (framesValue && framesValue->Type() == util::JsonValueType::array)
                {
                    util::JsonArray* framesArray = static_cast<util::JsonArray*>(framesValue);
                    if (framesArray->Count() == 1)
                    {
                        util::JsonValue* frameValue = (*framesArray)[0];
                        if (frameValue && frameValue->Type() == util::JsonValueType::object)
                        {
                            util::JsonObject* frameObject = static_cast<util::JsonObject*>(frameValue);
                            util::JsonValue* line = frameObject->GetField(U"line");
                            if (line)
                            {
                                step = true;
                            }
                        }
                    }
                }
            }
        }
    }
    if (!step)
    {
        Next();
        return;
    }
    ClearBrowsingData();
    result.reset(new util::JsonObject());
    if (state != State::stopped && state != State::programStarted)
    {
        throw std::runtime_error("error: state is '" + StateStr(state) + "'");
    }
    std::set<Instruction*> next;
    AddToNextSet(next, stoppedInstruction);
    Instruction* prevStoppedInstruction = stoppedInstruction;
    Instruction* prevSingleStepInstruction = nullptr;
    bool stop = false;
    bool skipNext = false;
    while (!stop)
    {
        GdbExecStepCommand execStepCommand;
        bool succeeded = true;
        if (skipNext)
        {
            skipNext = false;
        }
        else
        {
            stoppedInstruction = nullptr;
            succeeded = ExecuteGDBCommand(execStepCommand);
        }
        if (succeeded)
        {
            if (state == State::stopped)
            {
                if (stoppedInstruction != nullptr)
                {
                    AddToNextSet(next, stoppedInstruction);
                    prevSingleStepInstruction = stoppedInstruction;
                    if (stoppedInstruction->CppLineIndex() == 0)
                    {
                        bool foundInNext = next.find(stoppedInstruction) != next.cend();
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction() ||
                            stoppedInstruction->GetSourceSpan().line > prevStoppedInstruction->GetSourceSpan().line || foundInNext)
                        {
                            if (IsStopInstruction(stoppedInstruction) || foundInNext)
                            {
                                AddStopResultToResult();
                                WriteResult(result.get(), stoppedInstruction);
                                stop = true;
                            }
                        }
                    }
                    else
                    {
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction())
                        {
                            if (IsStopInstruction(stoppedInstruction))
                            {
                                AddStopResultToResult();
                                WriteResult(result.get(), stoppedInstruction);
                                stop = true;
                            }
                        }
                    }
                }
                else
                {
                    if (prevSingleStepInstruction != nullptr && prevSingleStepInstruction->Next() != nullptr)
                    {
                        GdbExecUntilCommand execUntilCommand(prevSingleStepInstruction->Next()->GetFileLineCppLocationArgs());
                        succeeded = ExecuteGDBCommand(execUntilCommand);
                        skipNext = true;
                    }
                }
            }
            else
            {
                AddStopResultToResult();
                Instruction* instruction = nullptr;
                if (state != State::programExitedNormally && state != State::programExited)
                {
                    instruction = stoppedInstruction;
                }
                WriteResult(result.get(), instruction);
                stop = true;
            }
        }
        else if (result)
        {
            ResetConsole();
            Proceed();
            WriteResult(result.get(), nullptr);
        }
    }
}

bool Debugger::Continue()
{
    ClearBrowsingData();
    result.reset(new util::JsonObject());
    if (state != State::stopped && state != State::programStarted)
    {
        throw std::runtime_error("error: state is '" + StateStr(state) + "'");
    }
    GdbExecContinueCommand exeContinueCommand;
    bool succeeded = ExecuteGDBCommand(exeContinueCommand);
    if (succeeded)
    {
        if (state == State::stopped)
        {
            if (stoppedInstruction != nullptr)
            {
                if (stoppedInstruction->CppLineIndex() == 0 && IsStopInstruction(stoppedInstruction))
                {
                    AddStopResultToResult();
                    WriteResult(result.get(), stoppedInstruction);
                }
            }
        }
        else
        {
            AddStopResultToResult();
            Instruction* instruction = nullptr;
            if (state != State::programExitedNormally && state != State::programExited)
            {
                instruction = stoppedInstruction;
            }
            WriteResult(result.get(), instruction);
        }
    }
    else if (result)
    {
        WriteResult(result.get(), nullptr);
        return false;
    }
    else
    {
        return false;
    }
    return true;
}

void Debugger::Finish()
{
    ClearBrowsingData();
    result.reset(new util::JsonObject());
    if (state != State::stopped && state != State::programStarted)
    {
        throw std::runtime_error("error: state is '" + StateStr(state) + "'");
    }
    GdbStackListFramesCommand frames(1, 1);
    bool succeeded = ExecuteGDBCommand(frames);
    if (succeeded)
    {
        succeeded = false;
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
            util::JsonValue* framesValue = resultObject->GetField(U"frames");
            if (framesValue && framesValue->Type() == util::JsonValueType::array)
            {
                util::JsonArray* framesArray = static_cast<util::JsonArray*>(framesValue);
                if (framesArray->Count() == 1)
                {
                    util::JsonValue* frameValue = (*framesArray)[0];
                    if (frameValue && frameValue->Type() == util::JsonValueType::object)
                    {
                        util::JsonObject* frameObject = static_cast<util::JsonObject*>(frameValue);
                        util::JsonValue* line = frameObject->GetField(U"line");
                        if (line)
                        {
                            util::JsonValue* cppFramesValue = resultObject->GetField(U"cppFrames");
                            if (cppFramesValue && cppFramesValue->Type() == util::JsonValueType::array)
                            {
                                util::JsonArray* cppFramesArray = static_cast<util::JsonArray*>(cppFramesValue);
                                if (cppFramesArray->Count() == 1)
                                {
                                    util::JsonValue* cppFrameValue = (*cppFramesArray)[0];
                                    if (cppFrameValue && cppFrameValue->Type() == util::JsonValueType::object)
                                    {
                                        util::JsonObject* cppFrameObject = static_cast<util::JsonObject*>(cppFrameValue);
                                        std::string func = util::ToUtf8(cppFrameObject->GetStringField(U"func")->Value());
                                        std::string file = util::ToUtf8(cppFrameObject->GetStringField(U"file")->Value());
                                        std::string line = util::ToUtf8(cppFrameObject->GetStringField(U"line")->Value());
                                        Frame frame;
                                        frame.func = func;
                                        frame.file = file;
                                        frame.line = std::stoi(line);
                                        Instruction* inst = debugInfo->GetInstruction(frame, *outputWriter);
                                        if (inst)
                                        {
                                            std::set<Instruction*> nextSet;
                                            AddToNextSet(nextSet, inst);
                                            while (nextSet.empty() && inst)
                                            {
                                                inst = inst->Next();
                                                if (inst && !inst->GetCmajorFrame().IsEmpty() && inst->CppLineIndex() == 0)
                                                {
                                                    nextSet.insert(inst);
                                                }
                                            }
                                            std::vector<Instruction*> instructions;
                                            for (Instruction* next : nextSet)
                                            {
                                                instructions.push_back(next);
                                            }
                                            if (!instructions.empty())
                                            {
                                                std::unique_ptr<DebuggerBreakpoint> bp(new DebuggerBreakpoint(GetNextTemporaryBreakpointId()));
                                                std::string bpId = bp->Id();
                                                if (Break(std::move(bp), instructions))
                                                {
                                                    Continue();
                                                    Delete(bpId, false);
                                                    succeeded = true;
                                                }
                                                else
                                                {
                                                    throw std::runtime_error("error: finish: could not set temporary breakpoint");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!succeeded)
        {
            throw std::runtime_error("error: finish: could not set temporary breakpoint");
        }
    }
    if (!succeeded && result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::Until(const SourceLocation& location)
{
    ClearBrowsingData();
    result.reset(new util::JsonObject());
    if (state != State::stopped && state != State::programStarted)
    {
        throw std::runtime_error("error: state is '" + StateStr(state) + "'");
    }
    std::vector<Instruction*> instructions = debugInfo->GetSourceFileMap().GetInstructions(location);
    bool succeeded = false;
    if (!instructions.empty())
    {
        std::unique_ptr<DebuggerBreakpoint> bp(new DebuggerBreakpoint(GetNextTemporaryBreakpointId()));
        std::string bpId = bp->Id();
        if (Break(std::move(bp), instructions))
        {
            Continue();
            Delete(bpId, false);
            succeeded = true;
        }
        else
        {
            throw std::runtime_error("error: could not set temporary breakpoint to location '" + location.ToString() + "'");
        }
    }
    else
    {
        throw std::runtime_error("error: could not set temporary breakpoint to location '" + location.ToString() + "': no matching instructions");
    }
    if (!succeeded && result)
    {
        WriteResult(result.get(), nullptr);
    }
}

bool Debugger::Break(std::unique_ptr<DebuggerBreakpoint>&& bp, const std::vector<Instruction*>& instructions)
{
    std::unique_ptr<DebuggerBreakpoint> breakpoint(std::move(bp));
    for (Instruction* inst : instructions)
    {
        GdbBreakpoint* gdbbp = SetBreakpoint(inst);
        if (gdbbp != nullptr)
        {
            breakpoint->AddGdbBreakpointNumber(gdbbp->Number());
        }
        else
        {
            for (int gdbBreakpointNumber : breakpoint->GdbBreakpointNumbers())
            {
                GdbBreakpoint* gdbbp = GetBreakpoint(gdbBreakpointNumber);
                DeleteBreakpoint(gdbbp->GetInstruction());
            }
            return false;
        }
    }
    DebuggerBreakpoint* b = breakpoint.release();
    if (!instructions.empty())
    {
        b->SetFrame(instructions[0]->GetCmajorFrame());
    }
    debuggerBreakpointMap[b->Id()] = b;
    return true;
}

void Debugger::Break(const SourceLocation& location)
{
    util::JsonObject* resultObject = new util::JsonObject();
    result.reset(resultObject);
    int breakpointNumber = GetNextBreakpointNumber();
    debuggerBreakpointId = std::to_string(breakpointNumber);
    std::unique_ptr<DebuggerBreakpoint> bp(new DebuggerBreakpoint(std::to_string(breakpointNumber)));
    std::vector<Instruction*> instructions = debugInfo->GetSourceFileMap().GetInstructions(location);
    if (Break(std::move(bp), instructions))
    {
        util::JsonObject* breakpointObject = new util::JsonObject();
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        resultObject->AddField(U"breakpoint", std::unique_ptr<util::JsonValue>(breakpointObject));
        breakpointObject->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(breakpointNumber)))));
        breakpointObject->AddField(U"numInsts", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(instructions.size())))));
        breakpointObject->AddField(U"location", std::unique_ptr<util::JsonValue>(instructions[0]->GetCmajorFrame().ToJson(false)));
    }
    currentSourceFilePath = instructions[0]->GetCmajorFrame().file;
    WriteResult(result.get(), nullptr);
}

void Debugger::SetBreakCondition(int breakpointId, const std::string& expression)
{
    util::JsonObject* resultObject = new util::JsonObject();
    result.reset(resultObject);
    if (!stoppedInstruction)
    {
        throw std::runtime_error("error: not stopped");
    }
    std::u32string expr = util::ToUtf32(expression);
    auto lexer = cmajor::debug::expr::lexer::MakeLexer(expr.c_str(), expr.c_str() + expr.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<DebugExprNode> node = cmajor::debug::expr::parser::DebugExprParser<LexerType>::Parse(lexer);
    DebugExprBinder binder(stoppedInstruction, debugInfo.get(), stoppedInstruction->GetScope(), true);
    node->Accept(binder);
    BoundDebugExpression* boundExpression = binder.BoundExpression(node.get());
    std::string condition;
    if (boundExpression->HasContainerSubscript())
    {
        ContainerSubscriptTranslator translator(*this);
        boundExpression->Accept(translator);
        SetBreakCondition(breakpointId, translator.Expression());
    }
    else
    {
        condition = boundExpression->GdbExprString();
    }
    auto it = debuggerBreakpointMap.find(std::to_string(breakpointId));
    if (it != debuggerBreakpointMap.cend())
    {
        DebuggerBreakpoint* bp = it->second;
        bool succeeded = true;
        for (int gdbBreakpointNumber : bp->GdbBreakpointNumbers())
        {
            GdbBreakConditionCommand breakConditionCommand(gdbBreakpointNumber, condition);
            succeeded = ExecuteGDBCommand(breakConditionCommand);
            if (!succeeded)
            {
                break;
            }
        }
    }
    else
    {
        throw std::runtime_error("breakpoint id " + std::to_string(breakpointId) + " not found");
    }
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::Delete(const std::string& breakpointId)
{
    Delete(breakpointId, true);
}

void Debugger::Delete(const std::string& breakpointId, bool printResult)
{
    util::JsonObject* resultObject = new util::JsonObject();
    result.reset(resultObject);
    auto it = debuggerBreakpointMap.find(breakpointId);
    if (it != debuggerBreakpointMap.cend())
    {
        DebuggerBreakpoint* bp = it->second;
        std::string gdbBreakpoints;
        bool first = true;
        for (int gdbBreakpointNumber : bp->GdbBreakpointNumbers())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                gdbBreakpoints.append(1, ' ');
            }
            gdbBreakpoints.append(std::to_string(gdbBreakpointNumber));
        }
        GdbBreakDeleteCommand breakDeleteCommand(gdbBreakpoints);
        bool succeeded = ExecuteGDBCommand(breakDeleteCommand);
        if (succeeded)
        {
            delete bp;
            debuggerBreakpointMap.erase(breakpointId);
            resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        }
        else
        {
            throw std::runtime_error("error: could not delete breakpoint " + breakpointId);
        }
    }
    else
    {
        throw std::runtime_error("error: breakpoint " + breakpointId + " not found");
    }
    if (printResult)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::Depth()
{
    result.reset(new util::JsonObject());
    GdbStackInfoDepthCommand stackInfoDepthCommand;
    bool succeeded = ExecuteGDBCommand(stackInfoDepthCommand);
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::Frames(int low, int high)
{
    frames.clear();
    result.reset(new util::JsonObject());
    GdbStackListFramesCommand stackListFrames(low, high);
    bool succeeded = ExecuteGDBCommand(stackListFrames);
    if (succeeded)
    {
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
            resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        }
    }
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::ShowBreakpoint(int breakpointId)
{
    util::JsonObject* resultObject = new util::JsonObject();
    result.reset(resultObject);
    auto it = debuggerBreakpointMap.find(std::to_string(breakpointId));
    if (it != debuggerBreakpointMap.cend())
    {
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        DebuggerBreakpoint* bp = it->second;
        resultObject->AddField(U"breakpoint", bp->ToJson());
    }
    else
    {
        throw std::runtime_error("error: breakpoint id '" + std::to_string(breakpointId) + "' not found");
    }
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::ShowBreakpoints()
{
    util::JsonObject* resultObject = new util::JsonObject();
    result.reset(resultObject);
    util::JsonArray* breakpointsArray = new util::JsonArray();
    for (const auto& p : debuggerBreakpointMap)
    {
        DebuggerBreakpoint* bp = p.second;
        breakpointsArray->AddItem(bp->ToJson());
    }
    resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
    resultObject->AddField(U"breakpoints", std::unique_ptr<util::JsonValue>(breakpointsArray));
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::List(const SourceLocation& location)
{
    SourceLocation loc = location;
    if (loc.IsEmpty())
    {
        loc = listLocation;
    }
    else if (loc.path == "*")
    {
        loc = SourceLocation(stoppedInstruction->GetCmajorFrame().file, std::max(1, stoppedInstruction->GetSourceSpan().line - debugInfo->GetSourceFileWindowSize()));
    }
    std::string sourceFilePath = debugInfo->GetSourceFileMap().GetSourceFilePath(loc);
    SourceFile& sourceFile = debugInfo->GetSourceFileCache().GetSourceFile(sourceFilePath);
    WriteSourceFile(sourceFile, loc.line, stoppedInstruction);
    listLocation = loc;
    listLocation.line = std::min(listLocation.line + 2 * debugInfo->GetSourceFileWindowSize(), int(sourceFile.Lines().size() + 1));
    currentSourceFilePath = listLocation.path;
}

void Debugger::Print(const std::string& expression)
{
    if (!stoppedInstruction)
    {
        throw std::runtime_error("error: not stopped");
    }
    Evaluate(expression);
    if (result)
    {
        WriteResult(result.get(), nullptr);
    }
}

DIType* Debugger::GetDynamicType(DIType* diType, BoundDebugNode* node)
{
    if (diType->GetKind() == DIType::Kind::pointerType)
    {
        DIPointerType* pointerType = static_cast<DIPointerType*>(diType);
        DIType* pointedType = pointerType->PointedToType();
        BoundDerefNode derefNode(pointedType, node->Clone(), nullptr);
        DIType* dynamicType = GetDynamicType(pointedType, &derefNode);
        if (dynamicType)
        {
            DIType* ptrType = MakePointerType(dynamicType);
            return ptrType;
        }
    }
    else if (diType->GetKind() == DIType::Kind::referenceType)
    {
        DIReferenceType* referenceType = static_cast<DIReferenceType*>(diType);
        DIType* referredType = referenceType->BaseType();
        BoundDerefNode derefNode(referredType, node->Clone(), nullptr);
        DIType* dynamicType = GetDynamicType(referredType, &derefNode);
        if (dynamicType)
        {
            DIType* refType = MakeReferenceType(dynamicType);
            return refType;
        }
    }
    else if (diType->GetKind() == DIType::Kind::constType)
    {
        DIConstType* constType = static_cast<DIConstType*>(diType);
        DIType* baseType = constType->BaseType();
        DIType* dynamicType = GetDynamicType(baseType, node);
        if (dynamicType)
        {
            DIType* cnstType = MakeConstType(dynamicType);
            return cnstType;
        }
    }
    else
    {
        if (diType->GetKind() == DIType::Kind::classType || diType->GetKind() == DIType::Kind::specializationType)
        {
            DIClassType* classType = static_cast<DIClassType*>(diType);
            if (classType->IsPolymorphic())
            {
                if (classType->VmtPtrIndex() != -1)
                {
                    std::string member = "m" + std::to_string(classType->VmtPtrIndex());
                    std::string gdbExprString = node->GdbExprString() + "." + member;
                    BoundDotNode vmtPtrMember(classType, node->Clone(), member, gdbExprString, nullptr);
                    BoundAddrOfNode vmtPtrAddr(classType, vmtPtrMember.Clone(), nullptr);
                    DebuggerVariable variable = GetNextDebuggerVariable();
                    GdbVarCreateCommand varCreateCommand(variable.GdbVarName(), "*", vmtPtrAddr.GdbExprString());
                    std::unique_ptr<util::JsonValue> mainResult(result.release());
                    result.reset(new util::JsonObject());
                    bool succeeded = ExecuteGDBCommand(varCreateCommand);
                    if (succeeded)
                    {
                        AddDebuggerVariable(variable);
                        if (result->Type() == util::JsonValueType::object)
                        {
                            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
                            util::JsonValue* value = jsonObject->GetField(U"value");
                            if (value && value->Type() == util::JsonValueType::string)
                            {
                                std::string vmtVarFieldStr = util::ToUtf8(static_cast<util::JsonString*>(value)->Value());
                                std::string vmtVarName = ParseVmtVariableName(vmtVarFieldStr);
                                if (!vmtVarName.empty())
                                {
                                    DIType* dynamicType = debugInfo->GetPolymorphicType(vmtVarName);
                                    result.reset(mainResult.release());
                                    return dynamicType;
                                }
                                else
                                {
                                    DebuggerVariable variable = GetNextDebuggerVariable();
                                    GdbVarCreateCommand varCreateCommand(variable.GdbVarName(), "*", vmtPtrMember.GdbExprString());
                                    result.reset(new util::JsonObject());
                                    bool succeeded = ExecuteGDBCommand(varCreateCommand);
                                    AddDebuggerVariable(variable);
                                    if (result->Type() == util::JsonValueType::object)
                                    {
                                        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
                                        util::JsonValue* value = jsonObject->GetField(U"value");
                                        std::string vmtVarFieldStr = util::ToUtf8(static_cast<util::JsonString*>(value)->Value());
                                        std::string vmtVarName = ParseVmtVariableName(vmtVarFieldStr);
                                        if (!vmtVarName.empty())
                                        {
                                            DIType* dynamicType = debugInfo->GetPolymorphicType(vmtVarName);
                                            result.reset(mainResult.release());
                                            return dynamicType;
                                        }
                                        /*  todo!!!!!!!!!!!!!!!!!!!!
                                                                                else
                                                                                {
                                                                                    uint64_t addr = ParseHex(vmtVarFieldStr);
                                                                                    if (addr != 0)
                                                                                    {
                                                                                        uint64_t typeIdAddr = addr + 16;
                                                                                        GdbExamineCommand examineCommand(16, 'x', 'b', typeIdAddr);
                                                                                        result.reset(new util::JsonObject());
                                                                                        bool success = ExecuteGDBCommand(examineCommand);
                                                                                        if (success)
                                                                                        {

                                                                                        }
                                                                                    }
                                                                                }
                                        */
                                    }
                                }
                            }
                        }
                    }
                    result.reset(mainResult.release());
                }
                else if (!classType->BaseClassId().is_nil())
                {
                    return GetDynamicType(classType->BaseClassType(), node);
                }
            }
        }
    }
    return nullptr;
}

void Debugger::AddDebuggerVariable(const DebuggerVariable& debuggerVariable)
{
    debuggerVariableMap[debuggerVariable.Index()] = debuggerVariable;
}

const DebuggerVariable* Debugger::GetDebuggerVariable(int index) const
{
    auto it = debuggerVariableMap.find(index);
    if (it != debuggerVariableMap.cend())
    {
        return &it->second;
    }
    else
    {
        return nullptr;
    }
}

Container* Debugger::GetContainer(ContainerClassTemplateKind containerKind, const std::string& containerVarExpr)
{
    uint64_t address = GetContainerAddress(*this, containerKind, containerVarExpr);
    if (address != 0)
    {
        auto it = containerMap.find(address);
        if (it != containerMap.cend())
        {
            Container* container = it->second;
            if (container->GetKind() == containerKind)
            {
                return container;
            }
        }
    }
    Container* container = CreateContainer(*this, containerKind, address);
    if (address != 0)
    {
        containerMap[address] = container;
    }
    containers.push_back(std::unique_ptr<Container>(container));
    return container;
}

void Debugger::ClearBrowsingData()
{
    for (auto& container : containers)
    {
        container->ClearBrowsingData();
    }
}

void Debugger::AddToNextSet(std::set<Instruction*>& nextSet, Instruction* inst) const
{
    CompileUnitFunction* function = inst->GetCompileUnitFunction();
    if (function)
    {
        ControlFlowGraphNode* node = function->GetControlFlowGraph().GetNodeByCppLineNumber(inst->CppLineNumber());
        if (node)
        {
            for (int32_t nextId : node->Next())
            {
                ControlFlowGraphNode* next = function->GetControlFlowGraph().GetNodeById(nextId);
                if (next)
                {
                    Instruction* inst = next->Inst();
                    if (!inst)
                    {
                        CompileUnit* compileUnit = function->GetCompileUnit();
                        if (compileUnit)
                        {
                            inst = compileUnit->GetInstruction(next->CppLineNumber());
                            if (inst)
                            {
                                next->SetInst(inst);
                            }
                        }
                    }
                    if (inst)
                    {
                        nextSet.insert(inst);
                    }
                }
            }
        }
    }
}

void Debugger::SetBreakOnThrow(bool breakOnThrow_, bool printResult)
{
    result.reset(new util::JsonObject());
    util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
    resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
    bool succeeded = true;
    bool resultPrinted = false;
    if (breakOnThrow_)
    {
        if (!breakOnThrow)
        {
            breakOnThrow = breakOnThrow_;
            if (verbose)
            {
                outputWriter->WriteLogMessage("setting throw breakpoints...");
            }
            SetThrowBreakpoints(printResult);
            if (verbose)
            {
                outputWriter->WriteLogMessage("done.");
            }
            resultPrinted = printResult;
        }
    }
    else
    {
        if (breakOnThrow)
        {
            breakOnThrow = breakOnThrow_;
            if (verbose)
            {
                outputWriter->WriteLogMessage("clearing throw breakpoints...");
            }
            ClearThrowBreakpoints(printResult);
            if (verbose)
            {
                outputWriter->WriteLogMessage("done.");
            }
            resultPrinted = printResult;
        }
    }
    if (!resultPrinted && printResult)
    {
        WriteResult(result.get(), nullptr);
    }
}

void Debugger::SetThrowBreakpoints(bool printResult)
{
    result.reset(new util::JsonObject());
    std::unique_ptr<DebuggerBreakpoint> bp(new DebuggerBreakpoint(GetNextTemporaryBreakpointId()));
    throwBreakpointsId = bp->Id();
    bool succeeded = Break(std::move(bp), debugInfo->GetThrowInstructions());
    if (succeeded)
    {
        util::JsonObject* resultObject = static_cast<util::JsonObject*>(result.get());
        resultObject->AddField(U"success", std::unique_ptr<util::JsonValue>(new util::JsonBool(true)));
        if (printResult)
        {
            WriteResult(result.get(), nullptr);
        }
    }
    else
    {
        throw std::runtime_error("error: could not set throw breakpoints");
    }
}

void Debugger::ClearThrowBreakpoints(bool printResult)
{
    Delete(throwBreakpointsId, printResult);
}

bool Debugger::SetCatchBreakpoints()
{
    result.reset(new util::JsonObject());
    std::unique_ptr<DebuggerBreakpoint> bp(new DebuggerBreakpoint(GetNextTemporaryBreakpointId()));
    catchBreakpointsId = bp->Id();
    bool succeeded = Break(std::move(bp), debugInfo->GetCatchInstructions());
    return succeeded;
}

void Debugger::ClearCatchBreakpoints()
{
    Delete(catchBreakpointsId, false);
}

void Debugger::SetDebugInfo(DebugInfo* debugInfo_)
{
    debugInfo.reset(debugInfo_);
}

void Debugger::RepeatLatestCommand()
{
    if (latestCommand)
    {
        if (latestCommand->GetKind() == DebuggerCommand::Kind::list)
        {
            List(SourceLocation());
        }
        else
        {
            latestCommand->Execute(*this);
        }
    }
    else
    {
        throw std::runtime_error("error: no latest command: try 'help'");
    }
}

bool Debugger::IsStopInstruction(Instruction* instruction) const
{
    InstructionFlags flags = instruction->GetFlags();
    if ((flags & (InstructionFlags::beginBrace | InstructionFlags::endBrace)) != InstructionFlags::none)
    {
        return true;
    }
    if ((flags & (InstructionFlags::entryCode | InstructionFlags::exitCode)) != InstructionFlags::none)
    {
        return false;
    }
    if ((flags & (InstructionFlags::startFunction)) != InstructionFlags::none)
    {
        return false;
    }
    return true;
}

void Debugger::Evaluate(const std::string& expression)
{
    result.reset(new util::JsonObject());
    std::u32string expr = util::ToUtf32(expression);
    auto lexer = cmajor::debug::expr::lexer::MakeLexer(expr.c_str(), expr.c_str() + expr.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<DebugExprNode> node = cmajor::debug::expr::parser::DebugExprParser<LexerType>::Parse(lexer);
    DebugExprBinder binder(stoppedInstruction, debugInfo.get(), stoppedInstruction->GetScope(), false);
    node->Accept(binder);
    BoundDebugExpression* boundExpression = binder.BoundExpression(node.get());
    if (boundExpression->HasContainerSubscript())
    {
        ContainerSubscriptTranslator translator(*this);
        boundExpression->Accept(translator);
        Evaluate(translator.Expression());
    }
    else
    {
        DebugExpressionEvaluator evaluator(*this, debugInfo.get());
        boundExpression->Accept(evaluator);
        result.reset(evaluator.ReleaseResult());
    }
}

DIType* Debugger::GetType(const std::string& expression)
{
    std::u32string expr = util::ToUtf32(expression);
    auto lexer = cmajor::debug::expr::lexer::MakeLexer(expr.c_str(), expr.c_str() + expr.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<DebugExprNode> node = cmajor::debug::expr::parser::DebugExprParser<LexerType>::Parse(lexer);
    DebugExprBinder binder(stoppedInstruction, debugInfo.get(), stoppedInstruction->GetScope(), false);
    node->Accept(binder);
    BoundDebugExpression* boundExpression = binder.BoundExpression(node.get());
    return boundExpression->Type();
}

bool Debugger::ExecuteGDBCommand(const GdbCommand& command)
{
    std::unique_ptr<GdbReply> reply = cmajor::debug::ExecuteGDBCommand(command, *this);
    if (!reply) return false;
    GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
    while (stoppedRecord == nullptr)
    {
        ProcessReply(command.GetKind(), reply.get());
        ResetTargetOutputFlag();
        reply = ReadGDBReply(*this);
        if (!reply) return false;
        stoppedRecord = reply->GetStoppedRecord();
    }
    ProcessReply(command.GetKind(), reply.get());
    bool succeeded = true;
    GdbReplyRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord != nullptr)
    {
        succeeded = resultRecord->CommandSucceeded();
    }
    return succeeded;
}

void Debugger::ProcessReply(GdbCommand::Kind commandKind, GdbReply* reply)
{
    switch (commandKind)
    {
    case GdbCommand::Kind::breakInsert:
    {
        ProcessBreakInsertReply(reply);
        break;
    }
    case GdbCommand::Kind::breakDelete:
    {
        ProcessBreakDeleteReply(reply);
        break;
    }
    case GdbCommand::Kind::execRun:
    {
        ProcessExecRunReply(reply);
        break;
    }
    case GdbCommand::Kind::execNext:
    {
        ProcessExecNextReply(reply);
        break;
    }
    case GdbCommand::Kind::execStep:
    {
        ProcessExecStepReply(reply);
        break;
    }
    case GdbCommand::Kind::execContinue:
    {
        ProcessExecContinueReply(reply);
        break;
    }
    case GdbCommand::Kind::execFinish:
    {
        ProcessExecFinishReply(reply);
        break;
    }
    case GdbCommand::Kind::execUntil:
    {
        ProcessExecUntilReply(reply);
        break;
    }
    case GdbCommand::Kind::stackInfoDepth:
    {
        ProcessStackInfoDepthReply(reply);
        break;
    }
    case GdbCommand::Kind::stackListFrames:
    {
        ProcessStackListFramesReply(reply);
        break;
    }
    case GdbCommand::Kind::varCreate:
    {
        ProcessVarCreateReply(reply);
        break;
    }
    case GdbCommand::Kind::breakCondition:
    {
        ProcessBreakConditionReply(reply);
        break;
    }
    case GdbCommand::Kind::varEvaluateExpression:
    {
        ProcessVarEvaluateReply(reply);
        break;
    }
    }
}

void Debugger::ProcessReplyRecord(GdbReplyRecord* record)
{
    switch (record->GetKind())
    {
    case GdbReplyRecord::Kind::execRunning:
    {
        SetState(State::running);
        wasRunning = true;
        SetConsoleActive();
        SetTargetRunning();
        break;
    }
    case GdbReplyRecord::Kind::execStopped:
    {
        SetState(State::stopped);
        break;
    }
    case GdbReplyRecord::Kind::notification:
    {
        // todo
        break;
    }
    case GdbReplyRecord::Kind::consoleOutput:
    {
        ProcessConsoleOutput(static_cast<GdbConsoleOutputRecord*>(record));
        break;
    }
    case GdbReplyRecord::Kind::targetOutput:
    {
        ProcessTargetOutput(static_cast<GdbTargetOutputRecord*>(record));
        targetOutput = true;
        break;
    }
    case GdbReplyRecord::Kind::logOutput:
    {
        ProcessLogOutput(static_cast<GdbLogOutputRecord*>(record));
    }
    }
}

void Debugger::ProcessBreakInsertReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            GdbResults* results = resultRecord->Results();
            if (results)
            {
                std::string breakpointFile;
                int breakpointLine = 0;
                int breakpointNumber = -1;
                if (GetBreakpointResults(results, breakpointFile, breakpointLine, breakpointNumber, result))
                {
                    Instruction* instruction = GetInstructionForCppLocation(breakpointFile, breakpointLine);
                    if (instruction)
                    {
                        GdbBreakpoint* breakpoint = GetBreakpoint(instruction);
                        if (breakpoint)
                        {
                            breakpoint->SetNumber(breakpointNumber);
                            breakpoint->SetDebuggerBreakpointId(debuggerBreakpointId);
                            gdbBreakpointsByNumber[breakpointNumber] = breakpoint;
                            success = true;
                        }
                    }
                }
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-break-insert")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessBreakDeleteReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            success = true;
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-break-delete")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecRunReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-run")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecContinueReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-continue")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecNextReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-next")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecStepReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-step")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecFinishReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-finish")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessExecUntilReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::running)
        {
            success = true;
        }
        else if (resultRecord->GetClass() == GdbResultRecord::Class::error)
        {
            SetState(State::stopped);
        }
    }
    else
    {
        cmajor::debug::GdbReplyRecord* stoppedRecord = reply->GetStoppedRecord();
        if (stoppedRecord)
        {
            if (stoppedRecord->GetKind() == cmajor::debug::GdbReplyRecord::Kind::execStopped)
            {
                GdbExecStoppedRecord* execStoppedRecord = static_cast<GdbExecStoppedRecord*>(stoppedRecord);
                success = ProcessExecStoppedRecord(execStoppedRecord);
            }
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-exec-until")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessStackInfoDepthReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            success = GetStackDepthResult(resultRecord->Results(), result);
        }
    }
    if (!success)
    {
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-stack-info-depth")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessStackListFramesReply(GdbReply* reply)
{
    std::unique_ptr<util::JsonValue> cppStackResult(new util::JsonObject());
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            std::unique_ptr<util::JsonObject> stackResult(new util::JsonObject());
            std::unique_ptr<util::JsonArray> framesArray(new util::JsonArray());
            std::vector<Frame> cppFrames;
            success = GetStackListFramesResult(resultRecord->Results(), cppStackResult, cppFrames);
            if (success)
            {
                for (const Frame& cppFrame : cppFrames)
                {
                    try
                    {
                        Instruction* instruction = debugInfo->GetInstruction(cppFrame, *outputWriter);
                        if (instruction)
                        {
                            Frame cmajorFrame = instruction->GetCmajorFrame();
                            cmajorFrame.level = cppFrame.level;
                            frames.push_back(cmajorFrame);
                            framesArray->AddItem(cmajorFrame.ToJson(true));
                        }
                        else
                        {
                            Frame frame;
                            frame.level = cppFrame.level;
                            frames.push_back(frame);
                            framesArray->AddItem(frame.ToJson(true));
                        }
                    }
                    catch (const std::exception&)
                    {
                        Frame frame;
                        frame.level = cppFrame.level;
                        frames.push_back(frame);
                        framesArray->AddItem(frame.ToJson(true));
                    }
                }
                if (cppStackResult->Type() == util::JsonValueType::object)
                {
                    util::JsonObject* cppStackObject = static_cast<util::JsonObject*>(cppStackResult.get());
                    util::JsonValue* cppFramesValue = cppStackObject->GetField(U"cppFrames");
                    if (cppFramesValue)
                    {
                        stackResult->AddField(U"cppFrames", std::unique_ptr<util::JsonValue>(cppFramesValue->Clone()));
                    }
                }
                stackResult->AddField(U"frames", std::unique_ptr<util::JsonValue>(framesArray.release()));
                result.reset(stackResult.release());
            }
        }
    }
    if (!success)
    {
        result.reset(cppStackResult.release());
    }
}

void Debugger::ProcessVarCreateReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            success = GetVarCreateResult(resultRecord->Results(), result);
        }
    }
    if (!success)
    {
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-var-create")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessBreakConditionReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            success = GetBreakConditionResult(resultRecord->Results(), result);
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-break-condition")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

void Debugger::ProcessVarEvaluateReply(GdbReply* reply)
{
    bool success = false;
    cmajor::debug::GdbResultRecord* resultRecord = reply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->GetClass() == GdbResultRecord::Class::done)
        {
            success = GetVarEvaluateResult(resultRecord->Results(), result);
        }
    }
    if (!success)
    {
        if (result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* jsonObject = static_cast<util::JsonObject*>(result.get());
            jsonObject->AddField(U"command", std::unique_ptr<util::JsonValue>(new util::JsonString(U"-var-evaluate")));
            jsonObject->AddField(U"reply", reply->ToJson());
        }
    }
}

bool Debugger::ProcessExecStoppedRecord(GdbExecStoppedRecord* execStoppedRecord)
{
    std::unique_ptr<util::JsonObject> stopObject;
    bool success = false;
    GdbResults* results = execStoppedRecord->Results();
    if (results)
    {
        Frame cppFrame;
        success = GetStoppedResults(results, result, stopObject, cppFrame, *this);
        if (success)
        {
            util::JsonValue* reasonField = stopObject->GetField(U"reason");
            std::u32string reason;
            if (reasonField->Type() == util::JsonValueType::string)
            {
                reason = static_cast<util::JsonString*>(reasonField)->Value();
            }
            if (reason == U"exited-normally")
            {
                SetState(State::programExitedNormally);
            }
            else if (reason == U"exited")
            {
                SetState(State::programExited);
            }
            else
            {
                if (reason == U"signal-received")
                {
                    SetState(State::signalReceived);
                }
                Instruction* instruction = debugInfo->GetInstruction(cppFrame, *outputWriter);
                if (instruction)
                {
                    stopObject->AddField(U"frame", instruction->GetCmajorFrame().ToJson(false));
                    stoppedInstruction = instruction;
                    SourceFile& sourceFile = stoppedInstruction->GetCompileUnitFunction()->GetSourceFile();
                    int sourceLine = std::min(int(sourceFile.Lines().size()), instruction->GetSourceSpan().line + debugInfo->GetSourceFileWindowSize() + 1);
                    listLocation = SourceLocation(sourceFile.FilePath(), sourceLine);
                    currentSourceFilePath = sourceFile.FilePath();
                }
                else
                {
                    stoppedInstruction = nullptr;
                }
            }
        }
    }
    stopResult.reset(stopObject.release());
    return success;
}

DebuggerCommand::DebuggerCommand(Kind kind_) : kind(kind_)
{
}

DebuggerCommand::~DebuggerCommand()
{
}

DebuggerExitCommand::DebuggerExitCommand() : DebuggerCommand(Kind::exit)
{
}

void DebuggerExitCommand::Execute(Debugger& debugger)
{
    debugger.Exit();
}

DebuggerCommand* DebuggerExitCommand::Clone()
{
    return new DebuggerExitCommand();
}

DebuggerHelpCommand::DebuggerHelpCommand() : DebuggerCommand(Kind::help)
{
}

void DebuggerHelpCommand::Execute(Debugger& debugger)
{
    debugger.Help();
}

DebuggerCommand* DebuggerHelpCommand::Clone()
{
    return new DebuggerHelpCommand();
}

DebuggerNextCommand::DebuggerNextCommand() : DebuggerCommand(Kind::next)
{
}

void DebuggerNextCommand::Execute(Debugger& debugger)
{
    debugger.Next();
}

DebuggerCommand* DebuggerNextCommand::Clone()
{
    return new DebuggerNextCommand();
}

DebuggerStepCommand::DebuggerStepCommand() : DebuggerCommand(Kind::step)
{
}

void DebuggerStepCommand::Execute(Debugger& debugger)
{
    debugger.Step();
}

DebuggerCommand* DebuggerStepCommand::Clone()
{
    return new DebuggerStepCommand();
}

DebuggerContinueCommand::DebuggerContinueCommand() : DebuggerCommand(Kind::continue_)
{
}

void DebuggerContinueCommand::Execute(Debugger& debugger)
{
    debugger.Continue();
}

DebuggerCommand* DebuggerContinueCommand::Clone()
{
    return new DebuggerContinueCommand();
}

DebuggerFinishCommand::DebuggerFinishCommand() : DebuggerCommand(Kind::finish)
{
}

void DebuggerFinishCommand::Execute(Debugger& debugger)
{
    debugger.Finish();
}

DebuggerCommand* DebuggerFinishCommand::Clone()
{
    return new DebuggerFinishCommand();
}

DebuggerUntilCommand::DebuggerUntilCommand(const SourceLocation& location_) : DebuggerCommand(Kind::until), location(location_)
{
}

void DebuggerUntilCommand::Execute(Debugger& debugger)
{
    debugger.Until(location);
}

DebuggerCommand* DebuggerUntilCommand::Clone()
{
    return new DebuggerUntilCommand(location);
}

DebuggerBreakCommand::DebuggerBreakCommand(const SourceLocation& location_) : DebuggerCommand(Kind::break_), location(location_)
{
}

void DebuggerBreakCommand::Execute(Debugger& debugger)
{
    debugger.Break(location);
}

DebuggerCommand* DebuggerBreakCommand::Clone()
{
    return new DebuggerBreakCommand(location);
}

DebuggerDeleteCommand::DebuggerDeleteCommand(int breakpointNumber_) : DebuggerCommand(Kind::delete_), breakpointNumber(breakpointNumber_)
{
}

void DebuggerDeleteCommand::Execute(Debugger& debugger)
{
    debugger.Delete(std::to_string(breakpointNumber));
}

DebuggerCommand* DebuggerDeleteCommand::Clone()
{
    return new DebuggerDeleteCommand(breakpointNumber);
}

DebuggerDepthCommand::DebuggerDepthCommand::DebuggerDepthCommand() : DebuggerCommand(Kind::depth)
{
}

void DebuggerDepthCommand::Execute(Debugger& debugger)
{
    debugger.Depth();
}

DebuggerFramesCommand::DebuggerFramesCommand(int low_, int high_) : DebuggerCommand(Kind::frames), low(low_), high(high_)
{
}

void DebuggerFramesCommand::Execute(Debugger& debugger)
{
    debugger.Frames(low, high);
}

DebuggerCommand* DebuggerFramesCommand::Clone()
{
    return new DebuggerFramesCommand(low, high);
}

DebuggerCommand* DebuggerDepthCommand::Clone()
{
    return new DebuggerDepthCommand();
}

DebuggerShowBreakpointCommand::DebuggerShowBreakpointCommand(int breakpointId_) : DebuggerCommand(Kind::showBreakpoint), breakpointId(breakpointId_)
{
}

void DebuggerShowBreakpointCommand::Execute(Debugger& debugger)
{
    debugger.ShowBreakpoint(breakpointId);
}

DebuggerCommand* DebuggerShowBreakpointCommand::Clone()
{
    return new DebuggerShowBreakpointCommand(breakpointId);
}

DebuggerShowBreakpointsCommand::DebuggerShowBreakpointsCommand() : DebuggerCommand(Kind::showBreakpoints)
{
}

void DebuggerShowBreakpointsCommand::Execute(Debugger& debugger)
{
    debugger.ShowBreakpoints();
}

DebuggerCommand* DebuggerShowBreakpointsCommand::Clone()
{
    return new DebuggerShowBreakpointsCommand();
}

DebuggerListCommand::DebuggerListCommand(const SourceLocation& location_) : DebuggerCommand(Kind::list), location(location_)
{
}

void DebuggerListCommand::Execute(Debugger& debugger)
{
    debugger.List(location);
}

DebuggerCommand* DebuggerListCommand::Clone()
{
    return new DebuggerListCommand(location);
}

DebuggerPrintCommand::DebuggerPrintCommand(const std::string& expression_) : DebuggerCommand(Kind::print), expression(expression_)
{
}

void DebuggerPrintCommand::Execute(Debugger& debugger)
{
    debugger.Print(expression);
}

DebuggerCommand* DebuggerPrintCommand::Clone()
{
    return new DebuggerPrintCommand(expression);
}

std::string RemoveWhiteSpace(const std::string& expression)
{
    std::string expr;
    for (char c : expression)
    {
        if (c != ' ' && c != '\t')
        {
            expr.append(1, c);
        }
    }
    return expr;
}

DebuggerSetConditionCommand::DebuggerSetConditionCommand(int breakpointNumber_, const std::string& expression_) :
    DebuggerCommand(Kind::setCondition), breakpointNumber(breakpointNumber_), expression(RemoveWhiteSpace(expression_))
{
}

void DebuggerSetConditionCommand::Execute(Debugger& debugger)
{
    debugger.SetBreakCondition(breakpointNumber, expression);
}

DebuggerCommand* DebuggerSetConditionCommand::Clone()
{
    return new DebuggerSetConditionCommand(breakpointNumber, expression);
}

DebuggerSetBreakOnThrowCommand::DebuggerSetBreakOnThrowCommand(bool breakOnThrow_) : DebuggerCommand(Kind::setBreakOnThrow), breakOnThrow(breakOnThrow_)
{
}

void DebuggerSetBreakOnThrowCommand::Execute(Debugger& debugger)
{
    debugger.SetBreakOnThrow(breakOnThrow, true);
}

DebuggerCommand* DebuggerSetBreakOnThrowCommand::Clone()
{
    return new DebuggerSetBreakOnThrowCommand(breakOnThrow);
}

DebuggerRepeatLatestCommand::DebuggerRepeatLatestCommand() : DebuggerCommand(Kind::repeatLatest)
{
}

bool DebuggerRepeatLatestCommand::IsRunningCommand(GdbDriver& driver) const
{
    return driver.LatestCommandWasRunningCommand();
}

void DebuggerRepeatLatestCommand::Execute(Debugger& debugger)
{
    debugger.RepeatLatestCommand();
}

DebuggerCommand* DebuggerRepeatLatestCommand::Clone()
{
    return new DebuggerRepeatLatestCommand();
}

} // namespace cmajor::debug
