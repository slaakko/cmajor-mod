// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.concept_symbol;

namespace cmajor::fault::tolerant::symbols {

ConceptSymbol::ConceptSymbol(const std::u16string& name_, const soul::ast::Span& span_) : ContainerSymbol(SymbolKind::conceptSymbol, name_, span_)
{
}

void ConceptSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
}

void ConceptSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
}

TypeParameterSymbol::TypeParameterSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::typeParameterSymbol, name_, span_)
{
}

void TypeParameterSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
}

void TypeParameterSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
