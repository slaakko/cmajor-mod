// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.msg.message.map;

import std.core;

export namespace cmajor::msg {

enum class MessageKind
{
    none, startRequest, startReply, stopRequest, stopReply, genericErrorReply, continueRequest, continueReply, nextRequest, nextReply, stepRequest, stepReply,
    finishRequest, finishReply, untilRequest, untilReply,
    breakRequest, breakReply, deleteRequest, deleteReply, depthRequest, depthReply, framesRequest, framesReply,
    countRequest, countReply, evaluateChildRequest, evaluateChildReply, evaluateRequest, evaluateReply,
    targetRunningRequest, targetRunningReply, targetInputRequest, targetInputReply, targetOutputRequest, targetOutputReply, logMessageRequest, logMessageReply,
    killRequest, killReply
};

class MessageMap
{
public:
    MessageMap();
    MessageKind GetMessageKind(const std::string& messageKindStr);
private:
    std::map<std::string, MessageKind> map;
};

} // namespace cmajor::debug
