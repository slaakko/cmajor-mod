// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.io;

import std.core;

export namespace cmajor::debugger {

struct OutputRequest
{
    OutputRequest();
    int handle;
    std::string output;
};

} // namespace cmajor::debugger
