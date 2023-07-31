// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.debug.logger;

import cmajor.debugger.request;
import cmajor.debugger.reply;
import std.core;

export namespace cmajor::debugger {

class DebugLogger
{
public:
    virtual ~DebugLogger();
    virtual void LogRequest(Request* request) = 0;
    virtual void LogReply(Reply* reply) = 0;
};

} // namespace cmajor::debugger
