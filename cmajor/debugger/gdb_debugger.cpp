// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.gdb.debugger;

import cmajor.debugger.reply.lexer;
import cmajor.debugger.reply.parser;
import cmajor.debugger.message.writer;
import cmajor.debugger.record;
import cmajor.debugger.util;
import cmajor.debugger.evaluator;
import cmajor.debugger.request;

namespace cmajor::debugger {

class OutputWriter : public cmajor::debug::DebuggerOutputWriter
{
public:
    OutputWriter(MessageWriter* messageWriter_);
    void WriteLogMessage(const std::string& message) override;
    void WriteJsonValue(util::JsonValue* jsonValue) override;
    void WriteWarning(const std::string& warning) override;
private:
    MessageWriter* messageWriter;
};

OutputWriter::OutputWriter(MessageWriter* messageWriter_) : messageWriter(messageWriter_)
{
}

void OutputWriter::WriteLogMessage(const std::string& message)
{
    if (messageWriter)
    {
        messageWriter->WriteMessage(message);
    }
}

void OutputWriter::WriteJsonValue(util::JsonValue* jsonValue)
{
}

void OutputWriter::WriteWarning(const std::string& warning)
{
    if (messageWriter)
    {
        messageWriter->WriteMessage("warning: " + warning);
    }
}

GDBDebugger::GDBDebugger() : exited(false), gdbExitCode(0), outputWriter(new OutputWriter(nullptr)), stoppedInstruction(nullptr), nextGdbVariableIndex(0)
{
}

GDBDebugger::~GDBDebugger()
{
    try
    {
        Stop();
    }
    catch (...)
    {
    }
}

void GDBDebugger::SetDebugLogger(DebugLogger* debugLogger)
{
    logger.reset(debugLogger);
}

void GDBDebugger::SetMessageWriter(MessageWriter* messageWriter_)
{
    messageWriter.reset(messageWriter_);
    outputWriter.reset(new OutputWriter(messageWriter.get()));
}

std::unique_ptr<Reply> GDBDebugger::Start(DebuggerStartParams& startParams)
{
    ClearBrowsingData();
    std::string startCommand;
    startCommand.append("gdb");
    startCommand.append(" --interpreter=mi");
    if (!startParams.programArguments.empty())
    {
        startCommand.append(" --args");
        startCommand.append(" \"").append(util::Path::MakeCanonical(startParams.executableFilePath)).append("\"");
        startCommand.append(" ").append(startParams.programArguments);
    }
    else
    {
        startCommand.append(" \"").append(startParams.executableFilePath).append("\"");
    }
    gdb.reset(new util::Process(startCommand,
        util::Process::Redirections::processStdIn |
            util::Process::Redirections::processStdOut |
            util::Process::Redirections::processStdErr));
    std::string cmdbFilePath;
    if (startParams.executableFilePath.ends_with(".exe"))
    {
        cmdbFilePath = util::Path::ChangeExtension(startParams.executableFilePath, ".cmdb");
    }
    else
    {
        cmdbFilePath = startParams.executableFilePath + ".cmdb";
    }
    if (std::filesystem::exists(cmdbFilePath))
    {
        messageWriter->WriteMessage("> " + cmdbFilePath);
        debugInfo = cmajor::debug::ReadDebugInfo(cmdbFilePath);
    }
    else
    {
        throw std::runtime_error("error: debug information '" + cmdbFilePath + "' file for executable '" + startParams.executableFilePath + "' not found");
    }
    std::unique_ptr<Reply> reply = ReadReply(nullptr);
    messageWriter->WriteMessage("setting breakpoints...");
    SetBreakpoints(startParams.breakpoints);
    messageWriter->WriteMessage("breakpoints set");
    return reply;
}

void GDBDebugger::Stop()
{
    if (exited) return;
    exited = true;
    if (gdb)
    {
        ExitRequest exitRequest;
        Execute(&exitRequest);
        gdb->WaitForExit();
        gdbExitCode = gdb->ExitCode();
        gdb.reset();
    }
}

std::unique_ptr<Reply> GDBDebugger::Execute(Request* request)
{
    if (logger)
    {
        logger->LogRequest(request);
    }
    gdb->WriteLine(request->ToString());
    if (!request->IsExitRequest())
    {
        return ReadReply(request);
    }
    else
    {
        return std::unique_ptr<Reply>();
    }
}

std::unique_ptr<Reply> GDBDebugger::ReadReply(Request* request)
{
    std::string line = gdb->ReadLine(util::Process::StdHandle::stdOut);
    std::unique_ptr<Reply> reply = ParseReplyLine(line);
    if (logger)
    {
        logger->LogReply(reply.get());
    }
    while (!reply->IsFinal(request))
    {
        line = gdb->ReadLine(util::Process::StdHandle::stdOut);
        std::unique_ptr<Reply> next = ParseReplyLine(line);
        if (logger)
        {
            logger->LogReply(next.get());
        }
        reply->Merge(*next);
    }
    reply->SetStopReason(StopReason::unknown);
    try
    {
        if (request && request->IsExecRequest())
        {
            AsyncRecord* execRecord = reply->GetExecRecord();
            if (execRecord)
            {
                if (execRecord->IsStoppedRecord())
                {
                    Results* results = execRecord->GetResults();
                    if (results && !results->IsEmpty())
                    {
                        std::string reasonText = results->GetString("reason");
                        if (!reasonText.empty())
                        {
                            StopReason reason = ParseStopReason(reasonText);
                            reply->SetStopReason(reason);
                            if (reason == StopReason::exited)
                            {
                                std::string exitCodeStr = results->GetString("exit-code");
                                if (!exitCodeStr.empty())
                                {
                                    int exitCode = std::stoi(exitCodeStr);
                                    reply->SetExitCode(exitCode);
                                }
                            }
                        }
                        cmajor::debug::Frame cppFrame = GetCppFrame(results, debugInfo.get());
                        if (!cppFrame.IsEmpty())
                        {
                            stoppedInstruction = debugInfo->GetInstruction(cppFrame, *outputWriter);
                            if (stoppedInstruction)
                            {
                                reply->SetStoppedInstruction(stoppedInstruction);
                                cmajor::debug::Frame cmajorFrame = stoppedInstruction->GetCmajorFrame();
                                if (!cmajorFrame.IsEmpty())
                                {
                                    results->Add(new Item("cm.frame", MakeFrameTuple(cmajorFrame)));
                                    reply->SetLocation(ToLocation(cmajorFrame));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        messageWriter->WriteMessage(ex.what());
    }
    return reply;
}

std::unique_ptr<Reply> GDBDebugger::ParseReplyLine(const std::string& line)
{
    std::u32string content = util::ToUtf32(line);
    auto lexer = cmajor::debugger::reply::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::debugger::Reply> reply = cmajor::debugger::reply::parser::ReplyParser<LexerType>::Parse(lexer);
    return reply;
}

std::unique_ptr<Reply> GDBDebugger::Run()
{
    ClearBrowsingData();
    RunRequest runRequest;
    return Execute(&runRequest);
}

std::unique_ptr<Reply> GDBDebugger::Continue()
{
    ClearBrowsingData();
    ContinueRequest continueRequest;
    return Execute(&continueRequest);
}

std::unique_ptr<Reply> GDBDebugger::Next()
{
    ClearBrowsingData();
    cmajor::debug::Instruction* prevStoppedInstruction = stoppedInstruction;
    std::set<cmajor::debug::Instruction*> nextSet;
    if (stoppedInstruction)
    {
        cmajor::debug::AddToNextSet(nextSet, stoppedInstruction);
    }
    while (true)
    {
        NextRequest nextRequest;
        std::unique_ptr<Reply> nextReply = Execute(&nextRequest);
        ResultRecord* resultRecord = nextReply->GetResultRecord();
        if (resultRecord && resultRecord->IsError())
        {
            throw std::runtime_error(resultRecord->ErrorMessage());
        }
        StopReason stopReason = nextReply->GetStopReason();
        switch (stopReason)
        {
            case StopReason::exitedNormally:
            case StopReason::exited:
            case StopReason::exitedSignaled:
            case StopReason::signalReceived:
            {
                return nextReply;
            }
            case StopReason::endSteppingRange:
            case StopReason::breakpointHit:
            {
                if (stoppedInstruction)
                {
                    cmajor::debug::AddToNextSet(nextSet, stoppedInstruction);
                    bool foundInNextSet = nextSet.find(stoppedInstruction) != nextSet.end();
                    if (stoppedInstruction->CppLineIndex() == 0)
                    {
                        if (stoppedInstruction->IsStopInstruction() || foundInNextSet)
                        {
                            if (prevStoppedInstruction == nullptr ||
                                prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction() ||
                                stoppedInstruction->GetSourceSpan().line > prevStoppedInstruction->GetSourceSpan().line || 
                                foundInNextSet)
                            {
                                return nextReply;
                            }
                        }
                    }
                    else
                    {
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction())
                        {
                            if (stoppedInstruction->IsStopInstruction())
                            {
                                return nextReply;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

std::unique_ptr<Reply> GDBDebugger::Step()
{
    ClearBrowsingData();
    bool step = true;
    if (stoppedInstruction && stoppedInstruction->AtEndBrace())
    {
        step = false;
        FramesRequest framesRequest(1, 1); // parent frame
        std::unique_ptr<Reply> framesReply = Execute(&framesRequest);
        if (StackFrameHasLine(framesReply.get()))
        {
            step = true;
        }
    }
    if (!step)
    {
        return Next();
    }
    std::set<cmajor::debug::Instruction*> nextSet;
    if (stoppedInstruction)
    {
        cmajor::debug::AddToNextSet(nextSet, stoppedInstruction);
    }
    cmajor::debug::Instruction* prevStoppedInstruction = stoppedInstruction;
    while (true)
    {
        StepRequest stepRequest;
        std::unique_ptr<Reply> stepReply = Execute(&stepRequest);
        ResultRecord* resultRecord = stepReply->GetResultRecord();
        if (resultRecord && resultRecord->IsError())
        {
            throw std::runtime_error(resultRecord->ErrorMessage());
        }
        StopReason stopReason = stepReply->GetStopReason();
        switch (stopReason)
        {
        case StopReason::exitedNormally:
        case StopReason::exited:
        case StopReason::exitedSignaled:
        case StopReason::signalReceived:
        {
            return stepReply;
        }
        case StopReason::endSteppingRange:
        case StopReason::breakpointHit:
        {
            if (stoppedInstruction)
            {
                cmajor::debug::AddToNextSet(nextSet, stoppedInstruction);
                bool foundInNextSet = nextSet.find(stoppedInstruction) != nextSet.end();
                if (stoppedInstruction->CppLineIndex() == 0)
                {
                    if (stoppedInstruction->IsStopInstruction() || foundInNextSet)
                    {
                        if (prevStoppedInstruction == nullptr ||
                            prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction() ||
                            stoppedInstruction->GetSourceSpan().line > prevStoppedInstruction->GetSourceSpan().line ||
                            foundInNextSet)
                        {
                            return stepReply;
                        }
                    }
                }
                else
                {
                    if (prevStoppedInstruction == nullptr ||
                        prevStoppedInstruction->GetCompileUnitFunction() != stoppedInstruction->GetCompileUnitFunction())
                    {
                        if (stoppedInstruction->IsStopInstruction())
                        {
                            return stepReply;
                        }
                    }
                }
            }
            break;
        }
        }
    }
}

std::unique_ptr<Reply> GDBDebugger::Finish()
{
    ClearBrowsingData();
    return std::unique_ptr<Reply>();
}

std::unique_ptr<Reply> GDBDebugger::Until(const cmajor::info::db::Location& loc)
{
    ClearBrowsingData();
    return std::unique_ptr<Reply>();
}

int GDBDebugger::Depth()
{
    DepthRequest depthRequest;
    std::unique_ptr<Reply> depthReply = Execute(&depthRequest);
    ResultRecord* resultRecord = depthReply->GetResultRecord();
    if (resultRecord)
    {
        if (resultRecord->IsError())
        {
            throw std::runtime_error(resultRecord->ErrorMessage());
        }
        else if (resultRecord->IsDone())
        {
            cmajor::debugger::Results* results = resultRecord->GetResults();
            if (results)
            {
                int depth = results->GetInt("depth");
                return depth;
            }
            else
            {
                throw std::runtime_error("depth request failed: empty results");
            }
        }
        else
        {
            throw std::runtime_error("depth request failed: result record kind='" + ResultRecordKindStr(resultRecord->GetResultRecordKind()) + "'");
        }
    }
    throw std::runtime_error("depth request failed");
}

std::vector<cmajor::info::db::Location> GDBDebugger::Frames(int lowFrame, int highFrame)
{
    FramesRequest framesRequest(lowFrame, highFrame);
    std::unique_ptr<Reply> framesReply = Execute(&framesRequest);
    ResultRecord* resultRecord = framesReply->GetResultRecord();
    if (resultRecord && resultRecord->IsError())
    {
        throw std::runtime_error(resultRecord->ErrorMessage());
    }
    else if (resultRecord->IsDone())
    {
        cmajor::debugger::Results* results = resultRecord->GetResults();
        if (results)
        {
            std::vector<cmajor::info::db::Location> frames = GetFrames(results, debugInfo.get(), outputWriter.get());
            return frames;
        }
        else
        {
            throw std::runtime_error("frames request failed: empty results");
        }
    }
    else
    {
        throw std::runtime_error("frames request failed: result record kind='" + ResultRecordKindStr(resultRecord->GetResultRecordKind()) + "'");
    }
}

cmajor::info::db::CountReply GDBDebugger::Count(const cmajor::info::db::CountRequest& countRequest)
{
    if (stoppedInstruction)
    {
        cmajor::debug::CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
        if (countRequest.expression == "@locals")
        {
            int localVariableCount = function->LocalVariables().size();
            cmajor::info::db::CountReply countReply;
            countReply.count = localVariableCount;
            return countReply;
        }
        else
        {
            throw std::runtime_error("cannot evaluate count: unknown count expression");
        }
    }
    else
    {
        throw std::runtime_error("cannot evaluate count: not stopped");
    }
}

cmajor::info::db::EvaluateReply GDBDebugger::Evaluate(const std::string& expression)
{
    cmajor::info::db::EvaluateReply evaluateReply = cmajor::debugger::Evaluate(expression, this);
    util::uuid staticTypeId = util::ParseUuid(evaluateReply.result.staticType.id);
    if (stoppedInstruction)
    {
        cmajor::debug::CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
        cmajor::debug::CompileUnit* compileUnit = function->GetCompileUnit();
        cmajor::debug::Project* project = compileUnit->GetProject();
        cmajor::debug::DIType* type = project->GetType(staticTypeId);
        int64_t count = 0;
        cmajor::debug::DIType* dynType = nullptr;
        std::set<uint64_t> printedPointers;
        int level = 0;
        int maxLevel = maxPointerLevel;
        evaluateReply.result.value = cmajor::debugger::GetValue(expression, type, count, dynType, printedPointers, level, maxLevel, this);
    }
    else
    {
        throw std::runtime_error("cannot evaluate: not stopped");
    }
    return evaluateReply;
}

cmajor::info::db::EvaluateChildReply GDBDebugger::EvaluateChild(const cmajor::info::db::EvaluateChildRequest& request)
{
    cmajor::info::db::EvaluateChildReply reply;
    int start = request.start;
    int count = request.count;
    if (stoppedInstruction)
    {
        cmajor::debug::CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
        if (request.expression == "@locals")
        {
            int localVariableCount = function->LocalVariables().size();
            if (start >= 0 && start < localVariableCount)
            {
                if (start + count >= 0 && start + count <= localVariableCount)
                {
                    for (int i = start; i < start + count; ++i)
                    {
                        cmajor::debug::DIVariable* localVariable = function->LocalVariables()[i];
                        cmajor::info::db::ChildResult result;
                        result.expr = localVariable->Name();
                        result.name = localVariable->Name();
                        result.type = localVariable->GetType()->Name();
                        int64_t count = 0;
                        cmajor::debug::DIType* dynType = nullptr;
                        std::set<uint64_t> printedPointers;
                        int level = 0;
                        int maxLevel = maxPointerLevel;
                        result.value = GetValue(std::string(), localVariable, count, dynType, printedPointers, level, maxLevel, this);
                        if (dynType != nullptr)
                        {
                            result.dynType = dynType->Name();
                        }
                        result.count = count;
                        reply.results.push_back(result);
                    }
                }
                else
                {
                    throw std::runtime_error("start/count not valid");
                }
            }
            else
            {
                throw std::runtime_error("start not valid");
            }
        }
        else
        {
            cmajor::debug::CompileUnit* compileUnit = function->GetCompileUnit();
            cmajor::debug::Project* project = compileUnit->GetProject();
            std::set<uint64_t> printedPointers;
            int level = 0;
            int maxLevel = maxPointerLevel;
            cmajor::debugger::EvaluateChildRequest(project, request.expression, start, count, reply, printedPointers, level, maxLevel, this);
        }
    }
    else
    {
        throw std::runtime_error("cannot evaluate: not stopped");
    }
    return reply;
}

void GDBDebugger::SetBreakpoints(const std::vector<Breakpoint*>& breakpoints)
{
    int n = breakpoints.size();
    for (int i = 0; i < n; ++i)
    {
        Breakpoint* breakpoint = breakpoints[i];
        SetBreakpoint(breakpoint);
    }
}

void GDBDebugger::SetBreakpoint(Breakpoint* breakpoint)
{
    cmajor::debug::SourceLocation breakpointLocation(breakpoint->list->FilePath(), breakpoint->line);
    std::vector<cmajor::debug::Instruction*> instructions = debugInfo->GetSourceFileMap().GetInstructions(breakpointLocation);
    breakpoint->ids.clear();
    breakpoint->disabled = true;
    for (const auto& instruction : instructions)
    {
        BreakInsertRequest request(instruction->GetExplicitCppLocationArgs());
        std::unique_ptr<Reply> reply = Execute(&request);
        ResultRecord* resultRecord = reply->GetResultRecord();
        if (resultRecord)
        {
            if (resultRecord->IsDone())
            {
                std::string breakpointId;
                Results* results = resultRecord->GetResults();
                if (results)
                {
                    Value* value = results->Get("bkpt");
                    if (value)
                    {
                        if (value->IsTuple())
                        {
                            Tuple* bkpt = static_cast<Tuple*>(value);
                            breakpointId = bkpt->GetString("number");
                        }
                    }
                }
                if (breakpointId.empty())
                {
                    messageWriter->WriteMessage("> error setting breakpoint: 'number' field not found in reply");
                }
                else
                {
                    breakpoint->ids.push_back(breakpointId);
                    breakpoint->disabled = false;
                    messageWriter->WriteMessage("> breakpoint " + breakpointId + " set");
                }
            }
            else if (resultRecord->IsError())
            {
                messageWriter->WriteMessage("> error setting breakpoint: " + resultRecord->ErrorMessage());
            }
        }
    }
}

cmajor::debug::DebuggerVariable GDBDebugger::GetNextDebuggerVariable()
{
    int index = nextGdbVariableIndex++;
    std::string variableName = "v" + std::to_string(index);
    return cmajor::debug::DebuggerVariable(index, variableName);
}

} // namespace cmajor::debugger

