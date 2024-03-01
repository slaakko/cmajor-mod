// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.flags;

import std.core;

export namespace cmajor::build {

enum class Flags
{
    none = 0,
    verbose = 1 << 0,
    singleThreadedParse= 1 << 1,
    ast = 1 << 2,
    print = 1 << 3
};

constexpr Flags operator|(Flags left, Flags right)
{
    return Flags(int(left) | int(right));
}

constexpr Flags operator&(Flags left, Flags right)
{
    return Flags(int(left) & int(right));
}

constexpr Flags operator~(Flags flags)
{
    return Flags(~int(flags));
}

} // cmajor::build
