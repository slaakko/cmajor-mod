// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.symbol;

import cmajor.masm.assembly.value;
import std.core;

export namespace cmajor::masm::assembly {

class Symbol : public Value
{
public:
    Symbol(const std::string& name_);
};

} // cmajor::masm::assembly
