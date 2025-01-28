// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.interface.symbol;

namespace cmajor::fault::tolerant::symbols {

InterfaceTypeSymbol::InterfaceTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::interfaceTypeSymbol, name_, span_)
{
}

void InterfaceTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
}

void InterfaceTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
