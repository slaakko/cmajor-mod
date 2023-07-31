// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.message.writer;

import std.core;

export namespace cmajor::debugger {

class MessageWriter
{
public:
    virtual ~MessageWriter();
    virtual void WriteMessage(const std::string& message) = 0;
};

} // namespace cmajor::debugger
