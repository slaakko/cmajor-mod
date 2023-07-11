// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debugger.output.writer;

import util;
import std.core;

export namespace cmajor::debug {

class Instruction;

class DebuggerOutputWriter
{
public:
    virtual ~DebuggerOutputWriter();
    virtual void WriteLogMessage(const std::string& message) = 0;
    virtual void WriteJsonValue(util::JsonValue* jsonValue) = 0;
    virtual void WriteWarning(const std::string& warning) = 0;
};

} // namespace cmajor::debug
