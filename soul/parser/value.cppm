// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.parser.value;

import std.core;

export namespace soul::parser {

template<class T>
struct Value
{
    Value(const T& value_) : value(value_) {}
    T value;
};

} // namespace soul::parser
