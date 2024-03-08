// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.literal;

import cmajor.masm.assembly.value;
import std.core;

export namespace cmajor::masm::assembly {

class NumericLiteral : public Value
{
public:
    NumericLiteral(int64_t value_, int size_);
private:
    int64_t value;
    int size;
};

class StringLiteral : public Value
{
public:
    StringLiteral(const std::string& value_);
private:
    std::string value;
};

} // namespace cmajor::masm::assembly
