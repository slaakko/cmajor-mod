// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.declaration.block.symbol;

namespace cmajor::fault::tolerant::symbols {

DeclarationBlockSymbol::DeclarationBlockSymbol(const std::u16string& name_, const soul::ast::Span& span_) : ContainerSymbol(SymbolKind::declarationBlockSymbol, name_, span_)
{
}

void DeclarationBlockSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
}

void DeclarationBlockSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
