// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.mode;

namespace cmajor::systemx::sxcdb {

std::string ModeName(Mode mode)
{
    switch (mode)
    {
        case Mode::breakOnThrow: return std::to_string(int(Mode::breakOnThrow)) + " break on throw"; 
        case Mode::breakOnFork: return std::to_string(int(Mode::breakOnFork)) + " break on fork";
        case Mode::breakOnExit: return std::to_string(int(Mode::breakOnExit)) + " break on exit";
    }
    return std::string();
}

} // namespace cmajor::systemx::sxcdb
