// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.location;

namespace cmajor::debugger {

Location::Location() : func(), file(), line(0), scol(0), ecol(0), level(0)
{
}

Location::Location(const std::string& file_, int line_) : file(file_), line(line_), scol(0), ecol(0), level(0)
{
}

Location ToLocation(const cmajor::debug::Frame& frame)
{
    return ToLocation(frame, false);
}

Location ToLocation(const cmajor::debug::Frame& frame, bool includeLevel)
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

} // namespace cmajor::debugger
