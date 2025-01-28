// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.debugger.client;

import cmajor.msg.cmdb.message;
import cmajor.msg.message.map;
import cmajor.debug.cmdb.session;
import cmajor.debug.debug.info;
import soul.xml.dom.parser;
import soul.xml.dom;
import util;

namespace cmajor::debug {

class DebuggerClient;

class ClientCommand
{
public:
    virtual ~ClientCommand();
    virtual void Execute(DebuggerClient& client) = 0;
};

class StartCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class StopCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class ContinueCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class NextCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class StepCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class LocalsCommand : public ClientCommand
{
public:
    void Execute(DebuggerClient& client) override;
};

class ChildrenCommand : public ClientCommand
{
public:
    ChildrenCommand(const std::string& expr_, int start_, int count_);
    void Execute(DebuggerClient& client) override;
private:
    std::string expr;
    int start;
    int count;
};

class EvaluateCommand : public ClientCommand
{
public:
    EvaluateCommand(const std::string& expr_);
    void Execute(DebuggerClient& client) override;
private:
    std::string expr;
};

class BreakCommand : public ClientCommand
{
public:
    BreakCommand(const SourceLoc& location_);
    void Execute(DebuggerClient& client) override;
private:
    SourceLoc location;
};

std::unique_ptr<ClientCommand> ParseCommand(const std::string& line)
{
    if (line == "start")
    {
        return std::unique_ptr<ClientCommand>(new StartCommand());
    }
    else if (line == "stop")
    {
        return std::unique_ptr<ClientCommand>(new StopCommand());
    }
    else if (line == "continue")
    {
        return std::unique_ptr<ClientCommand>(new ContinueCommand());
    }
    else if (line == "next")
    {
        return std::unique_ptr<ClientCommand>(new NextCommand());
    }
    else if (line == "step")
    {
        return std::unique_ptr<ClientCommand>(new StepCommand());
    }
    else if (line == "locals")
    {
        return std::unique_ptr<ClientCommand>(new LocalsCommand());
    }
    else if (line.starts_with("children"))
    {
        std::string::size_type spacePos = line.find(' ');
        if (spacePos != std::string::npos)
        {
            std::string params = line.substr(spacePos + 1);
            std::vector<std::string> paramVec = util::Split(params, ',');
            if (paramVec.size() == 3)
            {
                return std::unique_ptr<ClientCommand>(new ChildrenCommand(paramVec[0], std::stoi(paramVec[1]), std::stoi(paramVec[2])));
            }
            else
            {
                throw std::runtime_error("invalid children params");
            }
        }
        else
        {
            throw std::runtime_error("invalid children params");
        }
    }
    else if (line.starts_with("evaluate"))
    {
        std::string::size_type spacePos = line.find(' ');
        if (spacePos != std::string::npos)
        {
            std::string expr = line.substr(spacePos + 1);
            return std::unique_ptr<ClientCommand>(new EvaluateCommand(expr));
        }
        else
        {
            throw std::runtime_error("invalid evaluate params");
        }
    }
    else if (line.starts_with("break"))
    {
        std::string::size_type spacePos = line.find(' ');
        if (spacePos != std::string::npos)
        {
            std::string params = line.substr(spacePos + 1);
            std::vector<std::string> paramVec = util::Split(params, ':');
            if (paramVec.size() == 1)
            {
                SourceLoc sourceLoc;
                sourceLoc.path = "";
                sourceLoc.line = std::stoi(paramVec[0]); // TODO params[0]?
                return std::unique_ptr<ClientCommand>(new BreakCommand(sourceLoc));
            }
            else if (paramVec.size() == 2)
            {
                SourceLoc sourceLoc;
                sourceLoc.path = paramVec[0];
                sourceLoc.line = std::stoi(paramVec[1]);
                return std::unique_ptr<ClientCommand>(new BreakCommand(sourceLoc));
            }
            else
            {
                throw std::runtime_error("invalid break params");
            }
        }
        else
        {
            throw std::runtime_error("invalid break params");
        }
    }
    else
    {
        throw std::runtime_error("unknown command");
    }
}

class DebuggerClient
{
public:
    DebuggerClient(int port);
    cmajor::msg::MessageKind GetMessageKind(soul::xml::Element* element, std::string& messageKindStr);
    void Start();
    void Stop();
    void Continue();
    void Next();
    void Step();
    void Locals();
    void Children(const std::string& expr, int start, int count);
    void Evaluate(const std::string& expr);
    void Break(const SourceLoc& location);
    void WriteRequest(soul::xml::Element* request);
    void WriteReply(soul::xml::Element* reply);
    std::unique_ptr<soul::xml::Document> ReadReply(cmajor::msg::MessageKind replyMessageKind);
    void ProcessMessage(soul::xml::Element* message, cmajor::msg::MessageKind messageKind, const std::string& messageKindStr);
    void ProcessTargetRunningRequest(const TargetRunningRequest& targetRunningRequest);
    void ProcessTargetInputRequest(const TargetInputRequest& targetInputRequest);
    void ProcessTargetOutputRequest(const TargetOutputRequest& targetOutputRequest);
    void ProcessLogMessageRequest(const LogDebugMessageRequest& logDebugMessageRequest);
    void ProcessErrorReply(const GenericDebugErrorReply& errorReply);
    void ProcessStartReply(soul::xml::Element* reply);
    void ProcessStopReply(soul::xml::Element* reply);
    void ProcessContinueReply(soul::xml::Element* reply);
    void ProcessNextReply(soul::xml::Element* reply);
    void ProcessStepReply(soul::xml::Element* reply);
    int ProcessCountReply(soul::xml::Element* reply);
    void ProcessEvaluateChildReply(soul::xml::Element* reply);
    void ProcessEvaluateReply(soul::xml::Element* reply);
    void ProcessBreakReply(soul::xml::Element* reply);
    bool Stopped() const { return stopped; }
private:
    cmajor::msg::MessageMap messageMap;
    int port;
    util::TcpSocket socket;
    bool stopped;
};

DebuggerClient::DebuggerClient(int port_) : port(port_), stopped(false)
{
}

cmajor::msg::MessageKind DebuggerClient::GetMessageKind(soul::xml::Element* element, std::string& messageKindStr)
{
    messageKindStr = util::ToUtf8(element->Name());
    return messageMap.GetMessageKind(messageKindStr);
}

void DebuggerClient::ProcessMessage(soul::xml::Element* message, cmajor::msg::MessageKind messageKind, const std::string& messageKindStr)
{
    switch (messageKind)
    {
    case cmajor::msg::MessageKind::targetRunningRequest:
    {
        TargetRunningRequest targetRunningRequest;
        targetRunningRequest.FromXml(message);
        ProcessTargetRunningRequest(targetRunningRequest);
        break;
    }
    case cmajor::msg::MessageKind::targetInputRequest:
    {
        TargetInputRequest targetInputRequest;
        targetInputRequest.FromXml(message);
        ProcessTargetInputRequest(targetInputRequest);
        break;
    }
    case cmajor::msg::MessageKind::targetOutputRequest:
    {
        TargetOutputRequest targetOutputRequest;
        targetOutputRequest.FromXml(message);
        ProcessTargetOutputRequest(targetOutputRequest);
        break;
    }
    case cmajor::msg::MessageKind::logMessageRequest:
    {
        LogDebugMessageRequest logDebugMessageRequest;
        logDebugMessageRequest.FromXml(message);
        ProcessLogMessageRequest(logDebugMessageRequest);
        break;
    }
    case cmajor::msg::MessageKind::genericErrorReply:
    {
        GenericDebugErrorReply errorReply;
        errorReply.FromXml(message);
        ProcessErrorReply(errorReply);
        break;
    }
    }
}

void DebuggerClient::ProcessTargetRunningRequest(const TargetRunningRequest& targetRunningRequest)
{
    TargetRunningReply targetRunningReply;
    std::unique_ptr<soul::xml::Element> reply(targetRunningReply.ToXml("targetRunningReply"));
    WriteReply(reply.release());
}

void DebuggerClient::ProcessTargetInputRequest(const TargetInputRequest& targetInputRequest)
{
    TargetInputReply targetInputReply;
    std::string line;
    if (std::getline(std::cin, line))
    {
        targetInputReply.line = line;
    }
    else
    {
        targetInputReply.eof = true;
    }
    std::unique_ptr<soul::xml::Element> reply(targetInputReply.ToXml("targetInputReply"));
    WriteReply(reply.release());
}

void DebuggerClient::ProcessTargetOutputRequest(const TargetOutputRequest& targetOutputRequest)
{
    int handle = targetOutputRequest.handle;
    if (handle == 1)
    {
        std::cout << targetOutputRequest.output;
    }
    else if (handle == 2)
    {
        std::cerr << targetOutputRequest.output;
    }
    TargetOutputReply targetOutputReply;
    std::unique_ptr<soul::xml::Element> reply(targetOutputReply.ToXml("targetOutputReply"));
    WriteReply(reply.release());
}

void DebuggerClient::ProcessLogMessageRequest(const LogDebugMessageRequest& logDebugMessageRequest)
{
    std::cout << logDebugMessageRequest.logMessage << std::endl;
    LogDebugMessageReply logDebugMessageReply;
    std::unique_ptr<soul::xml::Element> reply(logDebugMessageReply.ToXml("logDebugMessageReply"));
    WriteReply(reply.release());
}

void DebuggerClient::ProcessErrorReply(const GenericDebugErrorReply& errorReply)
{
    std::cerr << errorReply.errorMessage << std::endl;
}

void DebuggerClient::WriteRequest(soul::xml::Element* request)
{
    soul::xml::Document requestDoc;
    requestDoc.AppendChild(request);
    std::stringstream strStream;
    util::CodeFormatter formatter(strStream);
    requestDoc.Write(formatter);
    std::string requestStr = strStream.str();
    Write(socket, requestStr);
}

void DebuggerClient::WriteReply(soul::xml::Element* reply)
{
    soul::xml::Document replyDoc;
    replyDoc.AppendChild(reply);
    std::stringstream strStream;
    util::CodeFormatter formatter(strStream);
    replyDoc.Write(formatter);
    std::string replyStr = strStream.str();
    Write(socket, replyStr);
}

std::unique_ptr<soul::xml::Document> DebuggerClient::ReadReply(cmajor::msg::MessageKind replyMessageKind)
{
    std::string replyStr = ReadStr(socket);
    std::unique_ptr<soul::xml::Document> replyDoc = soul::xml::ParseXmlContent(replyStr, "socket");
    std::string messageKindStr;
    cmajor::msg::MessageKind messageKind = GetMessageKind(replyDoc->DocumentElement(), messageKindStr);
    while (messageKind != replyMessageKind)
    {
        ProcessMessage(replyDoc->DocumentElement(), messageKind, messageKindStr);
        replyStr = ReadStr(socket);
        replyDoc = soul::xml::ParseXmlContent(replyStr, "socket");
        messageKind = GetMessageKind(replyDoc->DocumentElement(), messageKindStr);
    }
    return replyDoc;
}

void DebuggerClient::Start()
{
    socket.Connect("localhost", std::to_string(port));
    StartDebugRequest startDebugRequest;
    std::unique_ptr<soul::xml::Element> request(startDebugRequest.ToXml("startDebugRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::startReply);
    ProcessStartReply(replyDoc->DocumentElement());
}

void DebuggerClient::ProcessStartReply(soul::xml::Element* reply)
{
    StartDebugReply startDebugReply;
    startDebugReply.FromXml(reply);
}

void DebuggerClient::Stop()
{
    StopDebugRequest stopDebugRequest;
    std::unique_ptr<soul::xml::Element> request(stopDebugRequest.ToXml("stopDebugRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::stopReply);
    ProcessStopReply(replyDoc->DocumentElement());
}

void DebuggerClient::ProcessStopReply(soul::xml::Element* reply)
{
    StopDebugReply stopDebugReply;
    stopDebugReply.FromXml(reply);
    stopped = true;
}

void DebuggerClient::Continue()
{
    ContinueRequest continueRequest;
    std::unique_ptr<soul::xml::Element> request(continueRequest.ToXml("continueRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::continueReply);
    ProcessContinueReply(replyDoc->DocumentElement());
}

void DebuggerClient::ProcessContinueReply(soul::xml::Element* reply)
{
    ContinueReply continueReply;
    continueReply.FromXml(reply);
}

void DebuggerClient::Next()
{
    NextRequest nextRequest;
    std::unique_ptr<soul::xml::Element> request(nextRequest.ToXml("nextRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::nextReply);
    ProcessNextReply(replyDoc->DocumentElement());
}

void DebuggerClient::ProcessNextReply(soul::xml::Element* reply)
{
    NextReply nextReply;
    nextReply.FromXml(reply);
}

void DebuggerClient::Step()
{
    StepRequest stepRequest;
    std::unique_ptr<soul::xml::Element> request(stepRequest.ToXml("stepRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::stepReply);
    ProcessStepReply(replyDoc->DocumentElement());
}

void DebuggerClient::ProcessStepReply(soul::xml::Element* reply)
{
    StepReply stepReply;
    stepReply.FromXml(reply);
}

void DebuggerClient::Locals()
{
    CountRequest countRequest;
    countRequest.expression = "@locals";
    std::unique_ptr<soul::xml::Element> request(countRequest.ToXml("countRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> countReplyDoc = ReadReply(cmajor::msg::MessageKind::countReply);
    int numLocals = ProcessCountReply(countReplyDoc->DocumentElement());
    EvaluateChildRequest evaluateChildRequest;
    evaluateChildRequest.expression = "@locals";
    evaluateChildRequest.start = 0;
    evaluateChildRequest.count = numLocals;
    std::unique_ptr<soul::xml::Element> req(evaluateChildRequest.ToXml("evaluateChildRequest"));
    WriteRequest(req.release());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::evaluateChildReply);
    ProcessEvaluateChildReply(replyDoc->DocumentElement());
}

int DebuggerClient::ProcessCountReply(soul::xml::Element* reply)
{
    CountReply countReply;
    countReply.FromXml(reply);
    int numLocals = countReply.count;
    return numLocals;
}

void DebuggerClient::ProcessEvaluateChildReply(soul::xml::Element* reply)
{
    EvaluateChildReply evaluateChildReply;
    evaluateChildReply.FromXml(reply);
    int n = evaluateChildReply.results.size();
    for (int i = 0; i < n; ++i)
    {
        const ChildResult& childResult = evaluateChildReply.results[i];
        std::string s = childResult.name;
        if (!childResult.value.empty())
        {
            s.append(" = ").append(childResult.value);
        }
        std::cout << s << " : [" << childResult.expr << ", " << childResult.type << ", " << childResult.count << "]" << std::endl;
    }
}

void DebuggerClient::ProcessEvaluateReply(soul::xml::Element* reply)
{
    EvaluateReply evaluateReply;
    evaluateReply.FromXml(reply);
    if (evaluateReply.success)
    {
        std::cout << evaluateReply.result.value << std::endl;
    }
    else
    {
        std::cerr << evaluateReply.error << std::endl;
    }
}

void DebuggerClient::ProcessBreakReply(soul::xml::Element* reply)
{
    BreakReply breakReply;
    breakReply.FromXml(reply);
}

void DebuggerClient::Children(const std::string& expr, int start, int count)
{
    EvaluateChildRequest evaluateChildRequest;
    evaluateChildRequest.expression = expr;
    evaluateChildRequest.start = start;
    evaluateChildRequest.count = count;
    std::unique_ptr<soul::xml::Element> req(evaluateChildRequest.ToXml("evaluateChildRequest"));
    WriteRequest(req.get());
    std::unique_ptr<soul::xml::Document> replyDoc = ReadReply(cmajor::msg::MessageKind::evaluateChildReply);
    ProcessEvaluateChildReply(replyDoc->DocumentElement());
}

void DebuggerClient::Evaluate(const std::string& expr)
{
    EvaluateRequest evaluateRequest;
    evaluateRequest.expression = expr;
    std::unique_ptr<soul::xml::Element> request(evaluateRequest.ToXml("evaluateRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> reply = ReadReply(cmajor::msg::MessageKind::evaluateReply);
    ProcessEvaluateReply(reply->DocumentElement());
}

void DebuggerClient::Break(const SourceLoc& location)
{
    BreakRequest breakRequest;
    breakRequest.breakpointLocation = location;
    std::unique_ptr<soul::xml::Element> request(breakRequest.ToXml("breakRequest"));
    WriteRequest(request.release());
    std::unique_ptr<soul::xml::Document> reply = ReadReply(cmajor::msg::MessageKind::breakReply);
    ProcessBreakReply(reply->DocumentElement());
}

ClientCommand::~ClientCommand()
{
}

void StartCommand::Execute(DebuggerClient& client)
{
    client.Start();
}

void StopCommand::Execute(DebuggerClient& client)
{
    client.Stop();
}

void ContinueCommand::Execute(DebuggerClient& client)
{
    client.Continue();
}

void NextCommand::Execute(DebuggerClient& client)
{
    client.Next();
}

void StepCommand::Execute(DebuggerClient& client)
{
    client.Step();
}

void LocalsCommand::Execute(DebuggerClient& client)
{
    client.Locals();
}

ChildrenCommand::ChildrenCommand(const std::string& expr_, int start_, int count_) : expr(expr_), start(start_), count(count_)
{
}

void ChildrenCommand::Execute(DebuggerClient& client)
{
    client.Children(expr, start, count);
}

EvaluateCommand::EvaluateCommand(const std::string& expr_) : expr(expr_)
{
}

void EvaluateCommand::Execute(DebuggerClient& client)
{
    client.Evaluate(expr);
}

BreakCommand::BreakCommand(const SourceLoc& location_) : location(location_)
{
}

void BreakCommand::Execute(DebuggerClient& client)
{
    client.Break(location);
}

void RunClient(int port)
{
    DebuggerClient client(port);
    while (!client.Stopped())
    {
        std::string line;
        std::cout << "> ";
        std::getline(std::cin, line);
        std::unique_ptr<ClientCommand> command = ParseCommand(line);
        command->Execute(client);
    }
}

} // namespace cmajor::debug
