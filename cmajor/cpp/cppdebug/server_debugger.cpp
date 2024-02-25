// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.server.debugger;

import cmajor.debug.client.channel;
import cmajor.debug.debug.info;
import cmajor.debug.debugger;
import cmajor.debug.debugger.output.writer;
import cmajor.debug.di.type;
import cmajor.debug.di.variable;
import cmajor.msg.cmdb.message;
import cmajor.msg.message.map;
import soul.xml.dom.parser;
import soul.xml.dom;
import cmajor.ast;
import util;
import std.filesystem;

namespace cmajor::debug {

std::string CmdbLogFilePath()
{
    return util::Path::Combine(cmajor::ast::CmajorLogFileDir(), "cmdb.log");
}

Location ToLocation(const Frame& frame, bool includeLevel)
{
    Location loc;
    loc.func = frame.func;
    loc.file = frame.file;
    loc.line = frame.line;
    loc.scol = frame.scol;
    loc.ecol = frame.ecol;
    if (includeLevel)
    {
        loc.level = frame.level;
    }
    return loc;
}

Location ToLocation(const Frame& frame)
{
    return ToLocation(frame, false);
}

SourceLocation ToSourceLocation(const SourceLoc& breakpointLocation)
{
    SourceLocation sourceLocation(breakpointLocation.path, breakpointLocation.line);
    return sourceLocation;
}

class ServerDebugger;

class ServerDebuggerOutputWriter : public DebuggerOutputWriter
{
public:
    ServerDebuggerOutputWriter(ServerDebugger* debugger_, bool log_);
    void WriteLogMessage(const std::string& message) override;
    void WriteJsonValue(util::JsonValue* jsonValue) override;
    void WriteWarning(const std::string& warning) override;
private:
    ServerDebugger* serverDebugger;
    bool log;
    std::string logFilePath;
};

ServerDebuggerOutputWriter::ServerDebuggerOutputWriter(ServerDebugger* serverDebugger_, bool log_) :
    serverDebugger(serverDebugger_), log(log_), logFilePath(CmdbLogFilePath())
{
}

void ServerDebuggerOutputWriter::WriteJsonValue(util::JsonValue* jsonValue)
{
    if (log)
    {
        util::LogFileWriter writer(logFilePath);
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        util::CodeFormatter formatter(writer.LogFile());
        jsonValue->Write(formatter);
    }
}

void ServerDebuggerOutputWriter::WriteWarning(const std::string& warning)
{
    if (log)
    {
        util::LogFileWriter writer(logFilePath);
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer.WriteLine(warning);
    }
}

class ServerDebugger : public Debugger, public ClientChannelUser
{
public:
    const int maxPointerLevel = 3;
    ServerDebugger(const std::string& executable, const std::vector<std::string>& args, bool verbose_, bool breakOnThrow_, const std::string& version_, int port_, bool log_, bool idle_);
    ~ServerDebugger();
    void StartServer();
    void StopServer();
    void RunServer();
    void LogRequest(const std::unique_ptr<soul::xml::Document>& requestDoc);
    void LogReply(soul::xml::Document& replyDoc);
    bool SendIdleChannelMessages() const override { return idle; }
    std::unique_ptr<soul::xml::Element> ProcessRequest(soul::xml::Element* requestElement);
    std::unique_ptr<soul::xml::Element> GetIdleClientChannelMessage() override;
    bool IsIdleChannelMessage(soul::xml::Element* message) const override;
    void ClientChannelError(const std::string& error) override;
    void ProcessReceivedClientChannelMessage(soul::xml::Element* message) override;
    void ProcessTargetRunningReply(soul::xml::Element* message);
    void ProcessTargetInputReply(soul::xml::Element* message);
    void ProcessTargetOutputReply(soul::xml::Element* message);
    std::unique_ptr<soul::xml::Element> ProcessStartRequest(const StartDebugRequest& startDebugRequest);
    std::vector<BreakpointInfo> SetBreakpoints(const std::vector<SourceLoc>& breakpointLocations);
    BreakpointInfo SetBreakpoint(const SourceLocation& sourceLocation);
    std::unique_ptr<soul::xml::Element> ProcessStopRequest(const StopDebugRequest& stopDebugRequest);
    std::unique_ptr<soul::xml::Element> ProcessContinueRequest(const ContinueRequest& continueRequest);
    std::unique_ptr<soul::xml::Element> ProcessNextRequest(const NextRequest& nextRequest);
    std::unique_ptr<soul::xml::Element> ProcessStepRequest(const StepRequest& stepRequest);
    std::unique_ptr<soul::xml::Element> ProcessFinishRequest(const FinishRequest& finishRequst);
    std::unique_ptr<soul::xml::Element> ProcessUntilRequest(const UntilRequest& untilRequest);
    std::unique_ptr<soul::xml::Element> ProcessBreakRequest(const BreakRequest& breakRequest);
    std::unique_ptr<soul::xml::Element> ProcessDeleteRequest(const DeleteRequest& deleteRequest);
    std::unique_ptr<soul::xml::Element> ProcessDepthRequest(const DepthRequest& depthRequest);
    std::unique_ptr<soul::xml::Element> ProcessFramesRequest(const FramesRequest& framesRequest);
    std::unique_ptr<soul::xml::Element> ProcessCountRequest(const CountRequest& countRequest);
    std::unique_ptr<soul::xml::Element> ProcessEvaluateChildRequest(const EvaluateChildRequest& evaluateChildRequest);
    std::unique_ptr<soul::xml::Element> ProcessEvaluateRequest(const EvaluateRequest& evaluateRequest);
    void DoEvaluateChildRequest(Project* project, const std::string& expression, int start, int count, EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    void EvaluateSpecializationTypeChildRequest(DIClassTemplateSpecializationType* specializationType, const std::string& expression, int start, int count,
        EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    void EvaluateContainerTypeChildRequest(DIClassTemplateSpecializationType* containerType, const std::string& expression, int start, int count,
        EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    void EvaluateClassTypeChildRequest(DIClassType* classType, const std::string& expression, int start, int count, EvaluateChildReply& reply,
        std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetValue(const std::string& parentExpr, DIVariable* variable, int64_t& count, DIType*& dynType, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetValue(const std::string& expression, DIType* type, int64_t& count, DIType*& dynType, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetEnumeratedTypeValue(uint64_t value, DIEnumType* enumType);
    std::string GetSpecializationValue(bool initialized, const std::string& expression, DIClassTemplateSpecializationType* specializationType, int64_t& count,
        std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetClassValue(const std::string& expression, DIClassType* classType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetPointedValue(const std::string& expression, DIType* derefType, DIType* dynamicType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetClassDelegateValue(const std::string& expression, DIClassDelegateType* classDelegateType, std::set<uint64_t>& printedPointers, int& level, int maxLevel);
    std::string GetStringValue(const std::string& expression);
    std::string GetWStringValue(const std::string& expression);
    std::string GetUStringValue(const std::string& expression);
    EvaluateReply DoEvaluate(const std::string& expression);
    void GetLocationResult(bool& success, std::string& error, Location& loc, TargetState& targetState);
    cmajor::msg::MessageKind GetMessageKind(soul::xml::Element* message, std::string& messageKindStr);
    void AddStopResultToResult() override;
    void Proceed() override;
    void ResetConsole() override;
    void Prompt() override;
    void TargetInputPrompt() override;
    void Error(const std::string& msg) override;
    void WriteResult(util::JsonValue* result, Instruction* stoppedInstruction) override;
    void WriteSourceFile(SourceFile& sourceFile, int line, Instruction* stoppedInstruction) override;
    void SetConsoleActive() override;
    void SetTargetRunning() override;
    void ProcessConsoleOutput(GdbConsoleOutputRecord* record) override;
    void ProcessTargetOutput(GdbTargetOutputRecord* record) override;
    void ProcessLogOutput(GdbLogOutputRecord* record) override;
    std::string GetTargetInputBytes() override;
    void WriteTargetOuput(int handle, const std::string& s) override;
    void SetClientChannel(ClientChannel* channel);
    void WriteLogMessage(const std::string& logMessage);
private:
    cmajor::msg::MessageMap messageMap;
    std::thread mainThread;
    std::string version;
    int port;
    bool log;
    bool exiting;
    bool listening;
    bool idle;
    util::TcpSocket listenSocket;
    util::TcpSocket socket;
    std::string logFilePath;
    Location location;
    TargetState state;
    ClientChannel* clientChannel;
    std::mutex targetIOMutex;
    TargetRunningReply targetRunningReply;
    bool targetInputReplyReceived;
    std::condition_variable targetInputReplyReceivedVar;
    TargetInputReply targetInputReply;
    bool targetOutputReplyReceived;
    std::condition_variable targetOutputReplyReceivedVar;
    TargetOutputReply targetOutputReply;
    int targetRunningIntervalMs;
};

void ServerDebuggerOutputWriter::WriteLogMessage(const std::string& message)
{
    if (log)
    {
        util::LogFileWriter writer(logFilePath);
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer << message << std::endl;
    }
    serverDebugger->WriteLogMessage(message);
}

struct ClientChannelGuard
{
    ClientChannelGuard(ServerDebugger* debugger_, ClientChannel* channel) : debugger(debugger_)
    {
        debugger->SetClientChannel(channel);
    }
    ~ClientChannelGuard()
    {
        debugger->SetClientChannel(nullptr);
    }
    ServerDebugger* debugger;
};

ServerDebugger::ServerDebugger(const std::string& executable, const std::vector<std::string>& args, bool verbose_, bool breakOnThrow_, const std::string& version_,
    int port_, bool log_, bool idle_) :
    Debugger(verbose_, breakOnThrow_, new ServerDebuggerOutputWriter(this, log_), executable, args), version(version_), port(port_), log(log_), exiting(false),
    listening(false), logFilePath(CmdbLogFilePath()), clientChannel(nullptr), targetInputReplyReceived(false), targetOutputReplyReceived(false), targetRunningIntervalMs(250),
    idle(idle_)
{
}

void ServerDebugger::SetClientChannel(ClientChannel* channel)
{
    clientChannel = channel;
}

void ServerDebugger::WriteLogMessage(const std::string& logMessage)
{
    if (clientChannel)
    {
        LogDebugMessageRequest logDebugMessageRequest;
        logDebugMessageRequest.logMessage = logMessage;
        std::unique_ptr<soul::xml::Element> request(logDebugMessageRequest.ToXml("logDebugMessageRequest"));
        clientChannel->SendMessage(request.release());
    }
}

ServerDebugger::~ServerDebugger()
{
    try
    {
        StopGDB(*this);
    }
    catch (...)
    {
    }
    try
    {
        StopCmdbSession();
    }
    catch (...)
    {
    }
}

void ServerDebugger::RunServer()
{
    try
    {
        listening = true;
        listenSocket.Bind(port);
        listenSocket.Listen(10);
        std::cout << "debug-server-ready" << std::endl;
        std::cout << "Cmajor debugger version " + version << std::endl;
        util::LogFileWriter writer(logFilePath, log);
        if (log)
        {
            writer.WriteLine("================================================================================");
            writer.WriteCurrentDateTime();
            writer << "debug server run: port=" << port << std::endl;
        }
        socket = listenSocket.Accept();
        listening = false;
        while (!exiting)
        {
            std::string request = ReadStr(socket);
            std::u32string content = util::ToUtf32(request);
            if (content.empty()) return;
            std::unique_ptr<soul::xml::Document> requestDoc = soul::xml::ParseXmlContent(content, "socket");
            LogRequest(requestDoc);
            std::unique_ptr<soul::xml::Element> replyElement = ProcessRequest(requestDoc->DocumentElement());
            soul::xml::Document replyDoc;
            replyDoc.AppendChild(replyElement.release());
            LogReply(replyDoc);
            std::stringstream strStream;
            util::CodeFormatter formatter(strStream);
            replyDoc.Write(formatter);
            std::string reply = strStream.str();
            Write(socket, reply);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "debug-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        std::cerr << "exception from ServerDebugger::RunServer: " << ex.what() << std::endl;
        throw;
    }
}

void ServerDebugger::LogRequest(const std::unique_ptr<soul::xml::Document>& requestDoc)
{
    if (log)
    {
        util::LogFileWriter writer(logFilePath);
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer << "request:" << std::endl;
        util::CodeFormatter formatter(writer.LogFile());
        requestDoc->Write(formatter);
    }
}

void ServerDebugger::LogReply(soul::xml::Document& replyDoc)
{
    if (log)
    {
        util::LogFileWriter writer(logFilePath);
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer << "reply:" << std::endl;
        util::CodeFormatter formatter(writer.LogFile());
        replyDoc.Write(formatter);
    }
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessRequest(soul::xml::Element* requestMessage)
{
    ClientChannel channel(this, socket, targetRunningIntervalMs);
    ClientChannelGuard channelGuard(this, &channel);
    std::string messageKindStr;
    cmajor::msg::MessageKind messageKind = GetMessageKind(requestMessage, messageKindStr);
    switch (messageKind)
    {
    case cmajor::msg::MessageKind::startRequest:
    {
        StartDebugRequest startDebugRequest;
        startDebugRequest.FromXml(requestMessage);
        return ProcessStartRequest(startDebugRequest);
    }
    case cmajor::msg::MessageKind::stopRequest:
    {
        StopDebugRequest stopDebugRequest;
        stopDebugRequest.FromXml(requestMessage);
        return ProcessStopRequest(stopDebugRequest);
    }
    case cmajor::msg::MessageKind::continueRequest:
    {
        ContinueRequest continueRequest;
        continueRequest.FromXml(requestMessage);
        return ProcessContinueRequest(continueRequest);
    }
    case cmajor::msg::MessageKind::nextRequest:
    {
        NextRequest nextRequest;
        nextRequest.FromXml(requestMessage);
        return ProcessNextRequest(nextRequest);
    }
    case cmajor::msg::MessageKind::stepRequest:
    {
        StepRequest stepRequest;
        stepRequest.FromXml(requestMessage);
        return ProcessStepRequest(stepRequest);
    }
    case cmajor::msg::MessageKind::finishRequest:
    {
        FinishRequest finishRequest;
        finishRequest.FromXml(requestMessage);
        return ProcessFinishRequest(finishRequest);
    }
    case cmajor::msg::MessageKind::untilRequest:
    {
        UntilRequest untilRequest;
        untilRequest.FromXml(requestMessage);
        return ProcessUntilRequest(untilRequest);
    }
    case cmajor::msg::MessageKind::breakRequest:
    {
        BreakRequest breakRequest;
        breakRequest.FromXml(requestMessage);
        return ProcessBreakRequest(breakRequest);
    }
    case cmajor::msg::MessageKind::deleteRequest:
    {
        DeleteRequest deleteRequest;
        deleteRequest.FromXml(requestMessage);
        return ProcessDeleteRequest(deleteRequest);
    }
    case cmajor::msg::MessageKind::depthRequest:
    {
        DepthRequest depthRequest;
        depthRequest.FromXml(requestMessage);
        return ProcessDepthRequest(depthRequest);
    }
    case cmajor::msg::MessageKind::framesRequest:
    {
        FramesRequest framesRequest;
        framesRequest.FromXml(requestMessage);
        return ProcessFramesRequest(framesRequest);
    }
    case cmajor::msg::MessageKind::countRequest:
    {
        CountRequest countRequest;
        countRequest.FromXml(requestMessage);
        return ProcessCountRequest(countRequest);
    }
    case cmajor::msg::MessageKind::evaluateChildRequest:
    {
        EvaluateChildRequest evaluateChildRequest;
        evaluateChildRequest.FromXml(requestMessage);
        return ProcessEvaluateChildRequest(evaluateChildRequest);
    }
    case cmajor::msg::MessageKind::evaluateRequest:
    {
        EvaluateRequest evaluateRequest;
        evaluateRequest.FromXml(requestMessage);
        return ProcessEvaluateRequest(evaluateRequest);
    }
    default:
    {
        GenericDebugErrorReply genericDebugErrorReply;
        genericDebugErrorReply.errorMessage = "unknown request: messageKind='" + messageKindStr + "'";
        return std::unique_ptr<soul::xml::Element>(genericDebugErrorReply.ToXml("genericDebugErrorReply"));
    }
    }
}

std::unique_ptr<soul::xml::Element> ServerDebugger::GetIdleClientChannelMessage()
{
    TargetRunningRequest targetRunningRequest;
    return std::unique_ptr<soul::xml::Element>(targetRunningRequest.ToXml("targetRunningRequest"));
}

bool ServerDebugger::IsIdleChannelMessage(soul::xml::Element* message) const
{
    if (message->Name() == "targetRunningRequest") return true;
    return false;
}

void ServerDebugger::ClientChannelError(const std::string& error)
{
    std::cerr << error << std::endl;
}

void ServerDebugger::ProcessReceivedClientChannelMessage(soul::xml::Element* message)
{
    std::string messageKindStr;
    cmajor::msg::MessageKind messageKind = GetMessageKind(message, messageKindStr);
    switch (messageKind)
    {
    case cmajor::msg::MessageKind::targetRunningReply:
    {
        ProcessTargetRunningReply(message);
        break;
    }
    case cmajor::msg::MessageKind::targetInputReply:
    {
        ProcessTargetInputReply(message);
        break;
    }
    case cmajor::msg::MessageKind::targetOutputReply:
    {
        ProcessTargetOutputReply(message);
        break;
    }
    }
}

void ServerDebugger::ProcessTargetRunningReply(soul::xml::Element* message)
{
    std::lock_guard<std::mutex> lock(targetIOMutex);
    TargetRunningReply reply;
    reply.FromXml(message);
    targetRunningReply = reply;
}

void ServerDebugger::ProcessTargetInputReply(soul::xml::Element* message)
{
    std::lock_guard<std::mutex> lock(targetIOMutex);
    TargetInputReply reply;
    reply.FromXml(message);
    targetInputReply = reply;
    targetInputReplyReceived = true;
    targetInputReplyReceivedVar.notify_one();
}

void ServerDebugger::ProcessTargetOutputReply(soul::xml::Element* message)
{
    std::lock_guard<std::mutex> lock(targetIOMutex);
    TargetOutputReply reply;
    reply.FromXml(message);
    targetOutputReply = reply;
    targetOutputReplyReceived = true;
    targetOutputReplyReceivedVar.notify_one();
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessStartRequest(const StartDebugRequest& startDebugRequest)
{
    StartDebugReply startDebugReply;
    try
    {
        StartDebugging(true);
        startDebugReply.location = location;
        startDebugReply.state = state;
        OutputWriter()->WriteLogMessage("setting breakpoints...");
        std::vector<BreakpointInfo> breakpointInfos = SetBreakpoints(startDebugRequest.breakpointLocations);
        startDebugReply.breakpointInfos = breakpointInfos;
        startDebugReply.success = true;
        OutputWriter()->WriteLogMessage("breakpoints set");
    }
    catch (const std::exception& ex)
    {
        startDebugReply.success = false;
        startDebugReply.error = ex.what();
    }
    return std::unique_ptr<soul::xml::Element>(startDebugReply.ToXml("startDebugReply"));
}

std::vector<BreakpointInfo> ServerDebugger::SetBreakpoints(const std::vector<SourceLoc>& breakpointLocations)
{
    std::vector<BreakpointInfo> breakpointInfos;
    int n = breakpointLocations.size();
    for (int i = 0; i < n; ++i)
    {
        SourceLocation sourceLocation = ToSourceLocation(breakpointLocations[i]);
        breakpointInfos.push_back(SetBreakpoint(sourceLocation));
    }
    return breakpointInfos;
}

BreakpointInfo ServerDebugger::SetBreakpoint(const SourceLocation& location)
{
    BreakpointInfo info;
    try
    {
        Break(location);
        util::JsonValue* resultValue = GetResult();
        if (resultValue && resultValue->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(resultValue);
            util::JsonValue* successField = resultObject->GetField(U"success");
            if (successField && successField->Type() == util::JsonValueType::boolean)
            {
                info.success = static_cast<util::JsonBool*>(successField)->Value();
            }
            util::JsonValue* breakpointField = resultObject->GetField(U"breakpoint");
            if (breakpointField && breakpointField->Type() == util::JsonValueType::object)
            {
                util::JsonObject* breakpointObject = static_cast<util::JsonObject*>(breakpointField);
                util::JsonValue* idField = breakpointObject->GetField(U"id");
                if (idField && idField->Type() == util::JsonValueType::string)
                {
                    info.breakpointId = util::ToUtf8(static_cast<util::JsonString*>(idField)->Value());
                }
                util::JsonValue* numInstsField = breakpointObject->GetField(U"numInsts");
                if (numInstsField && numInstsField->Type() == util::JsonValueType::string)
                {
                    info.numInsts = std::stoi(util::ToUtf8(static_cast<util::JsonString*>(numInstsField)->Value()));
                }
                util::JsonValue* locationField = breakpointObject->GetField(U"location");
                if (locationField && locationField->Type() == util::JsonValueType::object)
                {
                    util::JsonObject* locationObject = static_cast<util::JsonObject*>(locationField);
                    util::JsonValue* funcField = locationObject->GetField(U"func");
                    if (funcField && funcField->Type() == util::JsonValueType::string)
                    {
                        info.location.func = util::ToUtf8(static_cast<util::JsonString*>(funcField)->Value());
                    }
                    util::JsonValue* fileField = locationObject->GetField(U"file");
                    if (fileField && fileField->Type() == util::JsonValueType::string)
                    {
                        info.location.file = util::ToUtf8(static_cast<util::JsonString*>(fileField)->Value());
                    }
                    util::JsonValue* lineField = locationObject->GetField(U"line");
                    if (lineField && lineField->Type() == util::JsonValueType::string)
                    {
                        info.location.line = std::stoi(util::ToUtf8(static_cast<util::JsonString*>(lineField)->Value()));
                    }
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        info.success = false;
        info.error = ex.what();
    }
    return info;
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessStopRequest(const StopDebugRequest& stopDebugRequest)
{
    StopDebugReply stopDebugReply;
    exiting = true;
    return std::unique_ptr<soul::xml::Element>(stopDebugReply.ToXml("stopDebugReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessContinueRequest(const ContinueRequest& continueRequest)
{
    if (clientChannel)
    {
        clientChannel->StartSendingIdleMessages();
    }
    ContinueReply continueReply;
    try
    {
        Continue();
        GetLocationResult(continueReply.success, continueReply.error, continueReply.location, continueReply.state);
    }
    catch (const std::exception& ex)
    {
        continueReply.success = false;
        continueReply.error = ex.what();
    }
    if (clientChannel)
    {
        clientChannel->StopSendingIdleMessages();
    }
    return std::unique_ptr<soul::xml::Element>(continueReply.ToXml("continueReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessNextRequest(const NextRequest& nextRequest)
{
    if (clientChannel)
    {
        clientChannel->StartSendingIdleMessages();
    }
    NextReply nextReply;
    try
    {
        Next();
        GetLocationResult(nextReply.success, nextReply.error, nextReply.location, nextReply.state);
    }
    catch (const std::exception& ex)
    {
        nextReply.success = false;
        nextReply.error = ex.what();
    }
    if (clientChannel)
    {
        clientChannel->StopSendingIdleMessages();
    }
    return std::unique_ptr<soul::xml::Element>(nextReply.ToXml("nextReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessStepRequest(const StepRequest& stepRequest)
{
    if (clientChannel)
    {
        clientChannel->StartSendingIdleMessages();
    }
    StepReply stepReply;
    try
    {
        Step();
        GetLocationResult(stepReply.success, stepReply.error, stepReply.location, stepReply.state);
    }
    catch (const std::exception& ex)
    {
        stepReply.success = false;
        stepReply.error = ex.what();
    }
    if (clientChannel)
    {
        clientChannel->StopSendingIdleMessages();
    }
    return std::unique_ptr<soul::xml::Element>(stepReply.ToXml("stepReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessFinishRequest(const FinishRequest& finishRequest)
{
    if (clientChannel)
    {
        clientChannel->StartSendingIdleMessages();
    }
    FinishReply finishReply;
    try
    {
        Finish();
        GetLocationResult(finishReply.success, finishReply.error, finishReply.location, finishReply.state);
    }
    catch (const std::exception& ex)
    {
        finishReply.success = false;
        finishReply.error = ex.what();
    }
    if (clientChannel)
    {
        clientChannel->StopSendingIdleMessages();
    }
    return std::unique_ptr<soul::xml::Element>(finishReply.ToXml("finishReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessUntilRequest(const UntilRequest& untilRequest)
{
    if (clientChannel)
    {
        clientChannel->StartSendingIdleMessages();
    }
    UntilReply untilReply;
    try
    {
        SourceLocation sourceLocation = ToSourceLocation(untilRequest.sourceLoc);
        Until(sourceLocation);
        GetLocationResult(untilReply.success, untilReply.error, untilReply.location, untilReply.state);
    }
    catch (const std::exception& ex)
    {
        untilReply.success = false;
        untilReply.error = ex.what();
    }
    if (clientChannel)
    {
        clientChannel->StopSendingIdleMessages();
    }
    return std::unique_ptr<soul::xml::Element>(untilReply.ToXml("untilReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessBreakRequest(const BreakRequest& breakRequest)
{
    BreakReply breakReply;
    try
    {
        SourceLocation location = ToSourceLocation(breakRequest.breakpointLocation);
        BreakpointInfo breakpointInfo = SetBreakpoint(location);
        breakReply.breakpointInfo = breakpointInfo;
    }
    catch (const std::exception& ex)
    {
        BreakpointInfo breakpointInfo;
        breakpointInfo.success = false;
        breakpointInfo.error = ex.what();
        breakReply.breakpointInfo = breakpointInfo;
    }
    return std::unique_ptr<soul::xml::Element>(breakReply.ToXml("breakReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessDeleteRequest(const DeleteRequest& deleteRequest)
{
    DeleteReply deleteReply;
    try
    {
        Delete(deleteRequest.breakpointId);
        util::JsonValue* result = GetResult();
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
            util::JsonValue* successField = resultObject->GetField(U"success");
            if (successField && successField->Type() == util::JsonValueType::boolean)
            {
                deleteReply.success = static_cast<util::JsonBool*>(successField)->Value();
            }
            util::JsonValue* errorField = resultObject->GetField(U"error");
            if (errorField && errorField->Type() == util::JsonValueType::string)
            {
                deleteReply.error = util::ToUtf8(static_cast<util::JsonString*>(errorField)->Value());
            }
        }
    }
    catch (const std::exception& ex)
    {
        deleteReply.success = false;
        deleteReply.error = ex.what();
    }
    return std::unique_ptr<soul::xml::Element>(deleteReply.ToXml("deleteReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessDepthRequest(const DepthRequest& depthRequest)
{
    DepthReply depthReply;
    try
    {
        Depth();
        util::JsonValue* result = GetResult();
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
            util::JsonValue* successField = resultObject->GetField(U"success");
            if (successField && successField->Type() == util::JsonValueType::boolean)
            {
                depthReply.success = static_cast<util::JsonBool*>(successField)->Value();
            }
            util::JsonValue* depthField = resultObject->GetField(U"depth");
            if (depthField && depthField->Type() == util::JsonValueType::string)
            {
                depthReply.depth = std::stoi(util::ToUtf8(static_cast<util::JsonString*>(depthField)->Value()));
            }
            util::JsonValue* errorField = resultObject->GetField(U"error");
            if (errorField && errorField->Type() == util::JsonValueType::string)
            {
                depthReply.error = util::ToUtf8(static_cast<util::JsonString*>(errorField)->Value());
            }
        }
    }
    catch (const std::exception& ex)
    {
        depthReply.success = false;
        depthReply.error = ex.what();
    }
    return std::unique_ptr<soul::xml::Element>(depthReply.ToXml("depthReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessFramesRequest(const FramesRequest& framesRequest)
{
    FramesReply framesReply;
    try
    {
        Frames(framesRequest.lowFrame, framesRequest.highFrame);
        util::JsonValue* result = GetResult();
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
            util::JsonValue* successField = resultObject->GetField(U"success");
            if (successField && successField->Type() == util::JsonValueType::boolean)
            {
                framesReply.success = static_cast<util::JsonBool*>(successField)->Value();
            }
            util::JsonValue* errorField = resultObject->GetField(U"error");
            if (errorField && errorField->Type() == util::JsonValueType::string)
            {
                framesReply.error = util::ToUtf8(static_cast<util::JsonString*>(errorField)->Value());
            }
        }
        if (framesReply.success)
        {
            const std::vector<Frame>& frames = Frames();
            int n = frames.size();
            for (int i = 0; i < n; ++i)
            {
                const Frame& frame = frames[i];
                Location loc = ToLocation(frame);
                framesReply.frames.push_back(std::move(loc));
            }
        }
    }
    catch (const std::exception& ex)
    {
        framesReply.success = false;
        framesReply.error = ex.what();
    }
    return std::unique_ptr<soul::xml::Element>(framesReply.ToXml("framesReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessCountRequest(const CountRequest& countRequest)
{
    CountReply countReply;
    Instruction* stoppedInstruction = StoppedInstruction();
    if (stoppedInstruction)
    {
        CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
        if (countRequest.expression == "@locals")
        {
            int localVariableCount = function->LocalVariables().size();
            countReply.success = true;
            countReply.count = localVariableCount;
        }
        else
        {
            countReply.success = false;
            countReply.error = "unknonwn count expression";
        }
    }
    else
    {
        countReply.success = false;
        countReply.error = "not stopped";
    }
    return std::unique_ptr<soul::xml::Element>(countReply.ToXml("countReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessEvaluateChildRequest(const EvaluateChildRequest& evaluateChildRequest)
{
    EvaluateChildReply evaluateChildReply;
    try
    {
        int start = evaluateChildRequest.start;
        int count = evaluateChildRequest.count;
        Instruction* stoppedInstruction = StoppedInstruction();
        if (stoppedInstruction)
        {
            CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
            if (evaluateChildRequest.expression == "@locals")
            {
                int localVariableCount = function->LocalVariables().size();
                if (start >= 0 && start < localVariableCount)
                {
                    if (start + count >= 0 && start + count <= localVariableCount)
                    {
                        evaluateChildReply.success = true;
                        for (int i = start; i < start + count; ++i)
                        {
                            DIVariable* localVariable = function->LocalVariables()[i];
                            ChildResult result;
                            result.expr = localVariable->Name();
                            result.name = localVariable->Name();
                            result.type = localVariable->GetType()->Name();
                            int64_t count = 0;
                            DIType* dynType = nullptr;
                            std::set<uint64_t> printedPointers;
                            int level = 0;
                            int maxLevel = maxPointerLevel;
                            result.value = GetValue(std::string(), localVariable, count, dynType, printedPointers, level, maxLevel);
                            if (dynType != nullptr)
                            {
                                result.dynType = dynType->Name();
                            }
                            result.count = count;
                            evaluateChildReply.results.push_back(result);
                        }
                    }
                    else
                    {
                        evaluateChildReply.success = false;
                        evaluateChildReply.error = "start/count not valid";
                    }
                }
                else
                {
                    evaluateChildReply.success = false;
                    evaluateChildReply.error = "start not valid";
                }
            }
            else
            {
                CompileUnit* compileUnit = function->GetCompileUnit();
                Project* project = compileUnit->GetProject();
                std::set<uint64_t> printedPointers;
                int level = 0;
                int maxLevel = maxPointerLevel;
                DoEvaluateChildRequest(project, evaluateChildRequest.expression, start, count, evaluateChildReply, printedPointers, level, maxLevel);
            }
        }
        else
        {
            evaluateChildReply.success = false;
            evaluateChildReply.error = "not stopped";
        }
    }
    catch (const std::exception& ex)
    {
        evaluateChildReply.success = false;
        evaluateChildReply.error = ex.what();
    }
    return std::unique_ptr<soul::xml::Element>(evaluateChildReply.ToXml("evaluateChildReply"));
}

std::unique_ptr<soul::xml::Element> ServerDebugger::ProcessEvaluateRequest(const EvaluateRequest& evaluateRequest)
{
    EvaluateReply evaluateReply = DoEvaluate(evaluateRequest.expression);
    if (evaluateReply.success)
    {
        try
        {
            util::uuid staticTypeId = util::ParseUuid(evaluateReply.result.staticType.id);
            Instruction* stoppedInstruction = StoppedInstruction();
            if (stoppedInstruction)
            {
                CompileUnitFunction* function = stoppedInstruction->GetCompileUnitFunction();
                CompileUnit* compileUnit = function->GetCompileUnit();
                Project* project = compileUnit->GetProject();
                DIType* type = project->GetType(staticTypeId);
                int64_t count = 0;
                DIType* dynType = nullptr;
                std::set<uint64_t> printedPointers;
                int level = 0;
                int maxLevel = maxPointerLevel;
                evaluateReply.result.value = GetValue(evaluateRequest.expression, type, count, dynType, printedPointers, level, maxLevel);
            }
            else
            {
                evaluateReply.success = false;
                evaluateReply.error = "not stopped";
            }
        }
        catch (const std::exception& ex)
        {
            evaluateReply.success = false;
            evaluateReply.error = ex.what();
        }
    }
    return std::unique_ptr<soul::xml::Element>(evaluateReply.ToXml("evaluateReply"));
}

void ServerDebugger::DoEvaluateChildRequest(Project* project, const std::string& expression, int start, int count, EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    EvaluateReply evaluateReply = DoEvaluate(expression);
    if (evaluateReply.success)
    {
        const Result& result = evaluateReply.result;
        const Type& staticType = result.staticType;
        const Type& dynamicType = result.dynamicType;
        util::uuid staticTypeId = util::ParseUuid(staticType.id);
        DIType* type = project->GetType(staticTypeId);
        while (type)
        {
            switch (type->GetKind())
            {
            case DIType::Kind::specializationType:
            {
                DIClassTemplateSpecializationType* specializationType = static_cast<DIClassTemplateSpecializationType*>(type);
                EvaluateSpecializationTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel);
                return;
            }
            case DIType::Kind::classType:
            {
                DIClassType* classType = static_cast<DIClassType*>(type);
                EvaluateClassTypeChildRequest(classType, expression, start, count, reply, printedPointers, level, maxLevel);
                return;
            }
            case DIType::Kind::pointerType:
            {
                if (!dynamicType.id.empty())
                {
                    DoEvaluateChildRequest(project, "(*cast<typeid(\"" + dynamicType.id + "\")>(" + expression + "))", start, count, reply, printedPointers, level, maxLevel);
                }
                else
                {
                    DoEvaluateChildRequest(project, "(*" + expression + ")", start, count, reply, printedPointers, level, maxLevel);
                }
                return;
            }
            case DIType::Kind::referenceType:
            {
                if (!dynamicType.id.empty())
                {
                    DoEvaluateChildRequest(project, "(*cast<typeid(\"" + dynamicType.id + "\")>(" + expression + "))", start, count, reply, printedPointers, level, maxLevel);
                }
                else
                {
                    DoEvaluateChildRequest(project, "(*" + expression + ")", start, count, reply, printedPointers, level, maxLevel);
                }
                return;
            }
            case DIType::Kind::constType:
            {
                DIConstType* constType = static_cast<DIConstType*>(type);
                type = constType->BaseType();
                break;
            }
            }
        }
    }
    else
    {
        reply.success = false;
        reply.error = evaluateReply.error;
    }
}

void ServerDebugger::EvaluateSpecializationTypeChildRequest(DIClassTemplateSpecializationType* specializationType, const std::string& expression, int start, int count,
    EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    if (specializationType->GetContainerClassTemplateKind() != ContainerClassTemplateKind::notContainerClassTemplate)
    {
        EvaluateContainerTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel);
    }
    else
    {
        EvaluateClassTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel);
    }
}

void ServerDebugger::EvaluateContainerTypeChildRequest(DIClassTemplateSpecializationType* containerType, const std::string& expression, int start, int count,
    EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    Container* container = GetContainer(containerType->GetContainerClassTemplateKind(), expression);
    int64_t childCount = container->Count(expression);
    if (start >= 0 && start < childCount)
    {
        if (start + count >= 0 && start + count <= childCount)
        {
            DIType* valueType = containerType->ValueType();
            for (int i = start; i < start + count; ++i)
            {
                ChildResult result;
                result.expr = expression + "[" + std::to_string(i) + "]";
                result.name = "[" + std::to_string(i) + "]";
                result.type = valueType->Name();
                int64_t count = 0;
                DIType* dynType = nullptr;
                result.value = GetValue(result.expr, valueType, count, dynType, printedPointers, level, maxLevel);
                if (dynType != nullptr)
                {
                    result.dynType = dynType->Name();
                }
                result.count = count;
                reply.results.push_back(std::move(result));
            }
            reply.success = true;
        }
        else
        {
            reply.success = false;
            reply.error = "start/count not valid";
        }
    }
    else
    {
        reply.success = false;
        reply.error = "start not valid";
    }
}

void ServerDebugger::EvaluateClassTypeChildRequest(DIClassType* classType, const std::string& expression, int start, int count, EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    int index = 0;
    int nb = NumBaseClasses(classType);
    for (int i = 0; i < nb; ++i)
    {
        if (index >= start && index < start + count)
        {
            int nc = i + 1;
            std::string expr = expression;
            DIClassType* type = classType;
            for (int j = 0; j < nc; ++j)
            {
                expr.append(".base");
                type = static_cast<DIClassType*>(type->BaseClassType());
            }
            ChildResult childResult;
            childResult.expr = expr;
            childResult.name = "[" + type->Name() + "]";
            childResult.type = type->Name();
            int64_t count = 0;
            DIType* dynType = nullptr;
            childResult.value = GetValue(expr, type, count, dynType, printedPointers, level, maxLevel);
            if (dynType != nullptr)
            {
                childResult.dynType = dynType->Name();
            }
            childResult.count = count;
            reply.results.push_back(childResult);
        }
        ++index;
    }
    int nm = classType->MemberVariables().size();
    for (int i = 0; i < nm; ++i)
    {
        if (index >= start && index < start + count)
        {
            DIVariable* memberVar = classType->MemberVariables()[i].get();
            std::string expr = expression;
            expr.append(".").append(memberVar->Name());
            ChildResult childResult;
            childResult.expr = expr;
            childResult.name = memberVar->Name();
            DIType* type = memberVar->GetType();
            childResult.type = type->Name();
            int64_t count = 0;
            DIType* dynType = nullptr;
            childResult.value = GetValue(expr, type, count, dynType, printedPointers, level, maxLevel);
            if (dynType != nullptr)
            {
                childResult.dynType = dynType->Name();
            }
            childResult.count = count;
            reply.results.push_back(childResult);
        }
        ++index;
    }
    reply.success = true;
}

std::string ServerDebugger::GetValue(const std::string& expression, DIType* type, int64_t& count, DIType*& dynType, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    dynType = nullptr;
    EvaluateReply evaluateReply = DoEvaluate(expression);
    if (evaluateReply.success)
    {
        switch (type->GetKind())
        {
        case DIType::Kind::primitiveType:
        {
            return evaluateReply.result.value;
        }
        case DIType::Kind::enumType:
        {
            if (evaluateReply.result.initialized)
            {
                try
                {
                    uint64_t value = std::stoull(evaluateReply.result.value);
                    return GetEnumeratedTypeValue(value, static_cast<DIEnumType*>(type));
                }
                catch (...)
                {
                    return evaluateReply.result.value;
                }
            }
            else
            {
                return evaluateReply.result.value;
            }
        }
        case DIType::Kind::specializationType:
        {
            return GetSpecializationValue(evaluateReply.result.initialized, expression, static_cast<DIClassTemplateSpecializationType*>(type), count,
                printedPointers, level, maxLevel);
        }
        case DIType::Kind::classType:
        {
            return GetClassValue(expression, static_cast<DIClassType*>(type), count, printedPointers, level, maxLevel);
        }
        case DIType::Kind::pointerType:
        {
            std::string value = evaluateReply.result.value;
            uint64_t addr = ParseHex(value);
            if (addr != 0)
            {
                if (printedPointers.find(addr) == printedPointers.cend())
                {
                    printedPointers.insert(addr);
                    DIType* dynamicType = nullptr;
                    if (!evaluateReply.result.dynamicType.id.empty())
                    {
                        try
                        {
                            util::uuid dynamicTypeId = util::ParseUuid(evaluateReply.result.dynamicType.id);
                            dynamicType = type->GetProject()->GetType(dynamicTypeId);
                            dynType = dynamicType;
                        }
                        catch (...)
                        {
                        }
                    }
                    std::string pointedValue = GetPointedValue(expression, static_cast<DIPointerType*>(type)->PointedToType(), dynamicType, count, printedPointers, level, maxLevel);
                    if (!pointedValue.empty())
                    {
                        value.append(" *=").append(pointedValue);
                    }
                }
            }
            return value;
        }
        case DIType::Kind::referenceType:
        {
            std::string value = evaluateReply.result.value;
            uint64_t addr = ParseHex(value);
            if (printedPointers.find(addr) == printedPointers.cend())
            {
                printedPointers.insert(addr);
                DIType* dynamicType = nullptr;
                if (!evaluateReply.result.dynamicType.id.empty())
                {
                    try
                    {
                        util::uuid dynamicTypeId = util::ParseUuid(evaluateReply.result.dynamicType.id);
                        dynamicType = type->GetProject()->GetType(dynamicTypeId);
                        dynType = dynamicType;
                    }
                    catch (...)
                    {
                    }
                }
                std::string pointedValue = GetPointedValue(expression, static_cast<DIReferenceType*>(type)->BaseType(), dynamicType, count, printedPointers, level, maxLevel);
                if (!pointedValue.empty())
                {
                    value.append(" *=").append(pointedValue);
                }
            }
            return value;
        }
        case DIType::Kind::constType:
        {
            DIConstType* constType = static_cast<DIConstType*>(type);
            return GetValue(expression, constType->BaseType(), count, dynType, printedPointers, level, maxLevel);
        }
        case DIType::Kind::delegateType:
        {
            return evaluateReply.result.value;
        }
        case DIType::Kind::classDelegateType:
        {
            return GetClassDelegateValue(expression, static_cast<DIClassDelegateType*>(type), printedPointers, level, maxLevel);
        }
        }
    }
    return std::string();
}

std::string ServerDebugger::GetValue(const std::string& parentExpr, DIVariable* variable, int64_t& count, DIType*& dynType, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    std::string expression;
    if (!parentExpr.empty())
    {
        expression.append(parentExpr).append(".");
    }
    expression.append(variable->Name());
    return GetValue(expression, variable->GetType(), count, dynType, printedPointers, level, maxLevel);
}

std::string ServerDebugger::GetEnumeratedTypeValue(uint64_t value, DIEnumType* enumType)
{
    uint64_t wholeValue = value;
    std::string strValue;
    bool first = true;
    while (true)
    {
        DIEnumConstant* enumConstant = enumType->GetEnumConstant(value);
        if (enumConstant)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                strValue.append(" | ");
            }
            strValue.append(enumType->Name()).append(".").append(enumConstant->Name());
            if (value == 0)
            {
                break;
            }
        }
        else
        {
            return std::to_string(wholeValue);
        }
    }
    return strValue;
}

std::string ServerDebugger::GetSpecializationValue(bool initialized, const std::string& expression, DIClassTemplateSpecializationType* specializationType, int64_t& count,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    if (specializationType->GetContainerClassTemplateKind() != ContainerClassTemplateKind::notContainerClassTemplate)
    {
        std::string value;
        value.append("{ ").append("container=<").append(ContainerName(specializationType->GetContainerClassTemplateKind())).append(">");
        if (initialized)
        {
            Container* container = GetContainer(specializationType->GetContainerClassTemplateKind(), expression);
            count = container->Count(expression);
            value.append(", count=").append(std::to_string(count));
        }
        else
        {
            value.append(", value=<uninitialized>");
        }
        value.append(" }");
        return value;
    }
    else
    {
        if (specializationType->Name() == "String<char>")
        {
            return GetStringValue(expression);
        }
        else if (specializationType->Name() == "String<wchar>")
        {
            return GetWStringValue(expression);
        }
        else if (specializationType->Name() == "String<uchar>")
        {
            return GetUStringValue(expression);
        }
        else
        {
            return GetClassValue(expression, specializationType, count, printedPointers, level, maxLevel);
        }
    }
}

std::string ServerDebugger::GetClassValue(const std::string& expression, DIClassType* classType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    bool first = true;
    std::string value = "{ ";
    if (!classType->BaseClassId().is_nil())
    {
        DIType* baseClassType = classType->BaseClassType();
        if (baseClassType && (baseClassType->GetKind() == DIType::Kind::classType || baseClassType->GetKind() == DIType::Kind::specializationType))
        {
            int64_t cnt = 0;
            std::string baseClassValue = GetClassValue(expression + ".base", static_cast<DIClassType*>(baseClassType), cnt, printedPointers, level, maxLevel);
            value.append(baseClassValue);
        }
        if (first)
        {
            first = false;
        }
    }
    for (const std::unique_ptr<DIVariable>& memberVar : classType->MemberVariables())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            value.append(", ");
        }
        int64_t count = 0;
        DIType* dynType = nullptr;
        value.append(memberVar->Name()).append("=").append(GetValue(expression, memberVar.get(), count, dynType, printedPointers, level, maxLevel));
    }
    value.append(" }");
    count = NumBaseClasses(classType) + classType->MemberVariables().size();
    return value;
}

std::string ServerDebugger::GetPointedValue(const std::string& expression, DIType* derefType, DIType* dynamicType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    if (level >= maxLevel)
    {
        return "...";
    }
    ++level;
    DIType* dynType = nullptr;
    if (dynamicType)
    {
        return GetValue("(*cast<typeid(\"" + util::ToString(dynamicType->Id()) + "\")>(" + expression + "))", dynamicType->DerefType(), count, dynType,
            printedPointers, level, maxLevel);
    }
    else
    {
        if (derefType->GetKind() == DIType::Kind::primitiveType)
        {
            DIPrimitiveType* primitiveType = static_cast<DIPrimitiveType*>(derefType);
            if (primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::charType ||
                primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::wcharType ||
                primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::ucharType ||
                primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::voidType)
            {
                return std::string();
            }
        }
        return GetValue("(*" + expression + ")", derefType, count, dynType, printedPointers, level, maxLevel);
    }
}

std::string ServerDebugger::GetClassDelegateValue(const std::string& expression, DIClassDelegateType* classDelegateType, std::set<uint64_t>& printedPointers, int& level, int maxLevel)
{
    int64_t count = 0;
    std::string value = GetClassValue(expression, static_cast<DIClassType*>(classDelegateType->GetClassType()), count, printedPointers, level, maxLevel);
    return value;
}

std::string ServerDebugger::GetStringValue(const std::string& expression)
{
    EvaluateReply evaluateReply = DoEvaluate(expression + ".chars");
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type firstDoubleQuotePos = value.find('"');
        if (firstDoubleQuotePos != std::string::npos)
        {
            std::string::size_type start = firstDoubleQuotePos + 1;
            while (start != std::string::npos)
            {
                std::string::size_type secondDoubleQuotePos = value.find('"', start);
                if (secondDoubleQuotePos > firstDoubleQuotePos + 1 && value[secondDoubleQuotePos - 1] == '\\')
                {
                    start = secondDoubleQuotePos + 1;
                }
                else if (secondDoubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(firstDoubleQuotePos, secondDoubleQuotePos - firstDoubleQuotePos + 1);
                    if (str.length() >= 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

std::string ServerDebugger::GetWStringValue(const std::string& expression)
{
    EvaluateReply evaluateReply = DoEvaluate(expression + ".chars");
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type uPos = value.find('u');
        if (uPos != std::string::npos)
        {
            std::string::size_type start = uPos + 2;
            while (start != std::string::npos)
            {
                std::string::size_type doubleQuotePos = value.find('"', start);
                if (doubleQuotePos > uPos + 2 && value[doubleQuotePos - 1] == '\\')
                {
                    start = doubleQuotePos + 1;
                }
                else if (doubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(uPos, doubleQuotePos - uPos + 1);
                    if (str.length() > 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

std::string ServerDebugger::GetUStringValue(const std::string& expression)
{
    EvaluateReply evaluateReply = DoEvaluate(expression + ".chars");
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type uPos = value.find('U');
        if (uPos != std::string::npos)
        {
            std::string::size_type start = uPos + 2;
            while (start != std::string::npos)
            {
                std::string::size_type doubleQuotePos = value.find('"', start);
                if (doubleQuotePos > uPos + 2 && value[doubleQuotePos - 1] == '\\')
                {
                    start = doubleQuotePos + 1;
                }
                else if (doubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(uPos, doubleQuotePos - uPos + 1);
                    if (str.length() > 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

EvaluateReply ServerDebugger::DoEvaluate(const std::string& expression)
{
    EvaluateReply evaluateReply;
    try
    {
        Print(expression);
        util::JsonValue* result = GetResult();
        if (result && result->Type() == util::JsonValueType::object)
        {
            util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
            util::JsonValue* success = resultObject->GetField(U"success");
            if (success && success->Type() == util::JsonValueType::boolean)
            {
                evaluateReply.success = static_cast<util::JsonBool*>(success)->Value();
            }
            util::JsonValue* error = resultObject->GetField(U"error");
            if (error && error->Type() == util::JsonValueType::string)
            {
                evaluateReply.error = util::ToUtf8(static_cast<util::JsonString*>(error)->Value());
            }
            util::JsonValue* staticType = resultObject->GetField(U"static_type");
            if (staticType && staticType->Type() == util::JsonValueType::object)
            {
                util::JsonObject* staticTypeObject = static_cast<util::JsonObject*>(staticType);
                util::JsonValue* id = staticTypeObject->GetField(U"id");
                if (id && id->Type() == util::JsonValueType::string)
                {
                    evaluateReply.result.staticType.id = util::ToUtf8(static_cast<util::JsonString*>(id)->Value());
                }
                util::JsonValue* name = staticTypeObject->GetField(U"name");
                if (name && name->Type() == util::JsonValueType::string)
                {
                    evaluateReply.result.staticType.name = util::ToUtf8(static_cast<util::JsonString*>(name)->Value());
                }
            }
            util::JsonValue* dynamicType = resultObject->GetField(U"dynamic_type");
            if (dynamicType && dynamicType->Type() == util::JsonValueType::object)
            {
                util::JsonObject* dynamicTypeObject = static_cast<util::JsonObject*>(dynamicType);
                util::JsonValue* id = dynamicTypeObject->GetField(U"id");
                if (id && id->Type() == util::JsonValueType::string)
                {
                    evaluateReply.result.dynamicType.id = util::ToUtf8(static_cast<util::JsonString*>(id)->Value());
                }
                util::JsonValue* name = dynamicTypeObject->GetField(U"name");
                if (name && name->Type() == util::JsonValueType::string)
                {
                    evaluateReply.result.dynamicType.name = util::ToUtf8(static_cast<util::JsonString*>(name)->Value());
                }
            }
            util::JsonValue* status = resultObject->GetField(U"status");
            if (status && status->Type() == util::JsonValueType::string)
            {
                std::string s = util::ToUtf8(static_cast<util::JsonString*>(status)->Value());
                if (s == "initialized")
                {
                    evaluateReply.result.initialized = true;
                    util::JsonValue* value = resultObject->GetField(U"value");
                    if (value && value->Type() == util::JsonValueType::string)
                    {
                        evaluateReply.result.value = util::ToUtf8(static_cast<util::JsonString*>(value)->Value());
                    }
                }
                else
                {
                    evaluateReply.result.initialized = false;
                    evaluateReply.result.value = "<uninitialized>";
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        evaluateReply.success = false;
        evaluateReply.error = ex.what();
    }
    return evaluateReply;
}

void ServerDebugger::GetLocationResult(bool& success, std::string& error, Location& loc, TargetState& targetState)
{
    util::JsonValue* result = GetResult();
    if (result && result->Type() == util::JsonValueType::object)
    {
        util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
        util::JsonValue* successField = resultObject->GetField(U"success");
        if (successField && successField->Type() == util::JsonValueType::boolean)
        {
            success = static_cast<util::JsonBool*>(successField)->Value();
        }
        util::JsonValue* errorField = resultObject->GetField(U"error");
        if (errorField && errorField->Type() == util::JsonValueType::string)
        {
            error = util::ToUtf8(static_cast<util::JsonString*>(errorField)->Value());
        }
    }
    loc = location;
    targetState = state;
}

cmajor::msg::MessageKind ServerDebugger::GetMessageKind(soul::xml::Element* message, std::string& messageKindStr)
{
    messageKindStr = util::ToUtf8(message->Name());
    return messageMap.GetMessageKind(messageKindStr);
}

void ServerDebugger::AddStopResultToResult()
{
    Debugger::AddStopResultToResult();
    Instruction* stoppedInstruction = StoppedInstruction();
    if (stoppedInstruction)
    {
        Frame frame = stoppedInstruction->GetCmajorFrame();
        location = ToLocation(frame);
    }
    else
    {
        location = Location();
    }
    util::JsonValue* result = GetResult();
    if (result && result->Type() == util::JsonValueType::object)
    {
        util::JsonObject* resultObject = static_cast<util::JsonObject*>(result);
        util::JsonValue* stoppedResult = resultObject->GetField(U"stopped");
        if (stoppedResult && stoppedResult->Type() == util::JsonValueType::object)
        {
            util::JsonObject* stoppedObject = static_cast<util::JsonObject*>(stoppedResult);
            util::JsonValue* reasonValue = stoppedObject->GetField(U"reason");
            if (reasonValue && reasonValue->Type() == util::JsonValueType::string)
            {
                state.stopReason = util::ToUtf8(static_cast<util::JsonString*>(reasonValue)->Value());
            }
            if (state.stopReason == "exited")
            {
                util::JsonValue* exitCodeValue = stoppedObject->GetField(U"exitCode");
                if (exitCodeValue && exitCodeValue->Type() == util::JsonValueType::string)
                {
                    state.exitCode = util::ToUtf8(static_cast<util::JsonString*>(exitCodeValue)->Value());
                }
            }
            if (state.stopReason == "breakpoint-hit")
            {
                util::JsonValue* breakpointIdValue = stoppedObject->GetField(U"breakpointId");
                if (breakpointIdValue && breakpointIdValue->Type() == util::JsonValueType::string)
                {
                    state.breakpointId = util::ToUtf8(static_cast<util::JsonString*>(breakpointIdValue)->Value());
                }
            }
            if (state.stopReason == "signal-received")
            {
                util::JsonValue* signalNameValue = stoppedObject->GetField(U"signalName");
                if (signalNameValue && signalNameValue->Type() == util::JsonValueType::string)
                {
                    state.signalName = util::ToUtf8(static_cast<util::JsonString*>(signalNameValue)->Value());
                }
                util::JsonValue* signalMeaningValue = stoppedObject->GetField(U"signalMeaning");
                if (signalMeaningValue && signalMeaningValue->Type() == util::JsonValueType::string)
                {
                    state.signalMeaning = util::ToUtf8(static_cast<util::JsonString*>(signalMeaningValue)->Value());
                }
            }
        }
    }
}

void DoRunServer(ServerDebugger* server)
{
    try
    {
        server->RunServer();
    }
    catch (const std::exception& ex)
    {
        std::cout << "debug-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        std::cerr << "exception from DoRunServer: " << ex.what() << std::endl;
    }
}

void ServerDebugger::StartServer()
{
    try
    {
        mainThread = std::thread{ DoRunServer, this };
    }
    catch (const std::exception& ex)
    {
        std::cout << "debug-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        std::cerr << "exception from ServerDebugger::StartServer: " << ex.what() << std::endl;
        throw;
    }
}

void ServerDebugger::StopServer()
{
    try
    {
        exiting = true;
        if (listening)
        {
            util::TcpSocket socket("localhost", std::to_string(port));
        }
        mainThread.join();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "exception from ServerDebugger::StopServer: " << ex.what() << std::endl;
        throw;
    }
}

void ServerDebugger::Proceed()
{
}

void ServerDebugger::ResetConsole()
{
}

void ServerDebugger::Prompt()
{
}

void ServerDebugger::TargetInputPrompt()
{
}

void ServerDebugger::Error(const std::string& msg)
{
}

void ServerDebugger::WriteResult(util::JsonValue* result, Instruction* stoppedInstruction)
{
}

void ServerDebugger::WriteSourceFile(SourceFile& sourceFile, int line, Instruction* stoppedInstruction)
{
}

void ServerDebugger::SetConsoleActive()
{
}

void ServerDebugger::SetTargetRunning()
{
}

void ServerDebugger::ProcessConsoleOutput(GdbConsoleOutputRecord* record)
{
}

void ServerDebugger::ProcessTargetOutput(GdbTargetOutputRecord* record)
{
}

void ServerDebugger::ProcessLogOutput(GdbLogOutputRecord* record)
{
}

std::string ServerDebugger::GetTargetInputBytes()
{
    targetInputReplyReceived = false;
    TargetInputRequest targetInputRequest;
    std::unique_ptr<soul::xml::Element> request(targetInputRequest.ToXml("targetInputRequest"));
    clientChannel->SendMessage(request.release());
    std::unique_lock<std::mutex> lock(targetIOMutex);
    targetInputReplyReceivedVar.wait(lock, [this] { return targetInputReplyReceived; });
    if (targetInputReply.eof)
    {
        return std::string();
    }
    else
    {
        std::string targetInputBytes;
        for (unsigned char c : targetInputReply.line)
        {
            targetInputBytes.append(util::ToHexString(static_cast<uint8_t>(c)));
        }
        targetInputBytes.append(util::ToHexString(static_cast<uint8_t>('\n')));
        return targetInputBytes;
    }
}

void ServerDebugger::WriteTargetOuput(int handle, const std::string& s)
{
    TargetOutputRequest targetOutputRequest;
    targetOutputRequest.handle = handle;
    targetOutputRequest.output = s;
    std::unique_ptr<soul::xml::Element> request(targetOutputRequest.ToXml("targetOutputRequest"));
    clientChannel->SendMessage(request.release());
    std::unique_lock<std::mutex> lock(targetIOMutex);
    targetOutputReplyReceivedVar.wait(lock, [this] { return targetOutputReplyReceived; });
}

ServerDebugger* serverDebugger = nullptr;

void StartDebuggerServer(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version,
    int port, bool log, bool idle)
{
    if (!serverDebugger)
    {
        serverDebugger = new ServerDebugger(executable, args, verbose, breakOnThrow, version, port, log, idle);
    }
    serverDebugger->StartServer();
}

void StopDebuggerServer()
{
    serverDebugger->StopServer();
    delete serverDebugger;
}

void RunDebuggerServer(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version, int port,
    bool log, bool idle)
{
    if (!serverDebugger)
    {
        serverDebugger = new ServerDebugger(executable, args, verbose, breakOnThrow, version, port, log, idle);
    }
    serverDebugger->RunServer();
    delete serverDebugger;
}

} // namespace cmajor::debug
