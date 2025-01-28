// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.dom.parsing.flags;

export namespace soul::xml {

enum class ParsingFlags : int
{
    none = 0, generateWarnings = 1 << 0
};

constexpr ParsingFlags operator|(ParsingFlags left, ParsingFlags right)
{
    return ParsingFlags(int(left) | int(right));
}

constexpr ParsingFlags operator&(ParsingFlags left, ParsingFlags right)
{
    return ParsingFlags(int(left) & int(right));
}

constexpr ParsingFlags operator~(ParsingFlags flags)
{
    return ParsingFlags(~int(flags));
}

} // namespace soul::xml
