// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.mode;

import std.core;

export namespace cmajor::systemx::sxcdb {

enum class Mode : int
{
    none = 0, breakOnThrow = 1 << 0, breakOnFork = 1 << 1, breakOnExit = 1 << 2, last = breakOnExit, all = breakOnThrow | breakOnFork | breakOnExit
};

std::string ModeName(Mode mode);

constexpr Mode operator|(Mode left, Mode right)
{
    return Mode(int(left) | int(right));
}

constexpr Mode operator&(Mode left, Mode right)
{
    return Mode(int(left) & int(right));
}

constexpr Mode operator~(Mode operand)
{
    return Mode(~int(operand));
}

} // namespace cmajor::systemx::sxcdb
