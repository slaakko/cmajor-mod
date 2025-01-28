// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.symbol;

namespace cmajor::masm::assembly {

Symbol::Symbol(const std::string& name_) : Value(ValueKind::symbol, name_)
{
}

} // namespace cmajor::masm::assembly
