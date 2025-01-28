// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef PARSER_VALUE_INCLUDED
#define PARSER_VALUE_INCLUDED

namespace soul::parser {

template<class T>
struct Value
{
    Value(const T& value_) : value(value_) {}
    T value;
};

} // namespace soul::parser

#endif // PARSER_VALUE_INCLUDED
