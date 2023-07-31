// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.location;

import cmajor.debug;
import std.core;

export namespace cmajor::debugger {

struct Location
{
    Location();
    Location(const std::string& file_, int line_);
    bool IsValid() const { return !file.empty() && line != 0; }
    std::string func;
    std::string file;
    int line;
    int scol;
    int ecol;
    int level;
};

Location ToLocation(const cmajor::debug::Frame& frame, bool includeLevel);
Location ToLocation(const cmajor::debug::Frame& frame);

} // namespace cmajor::debugger
