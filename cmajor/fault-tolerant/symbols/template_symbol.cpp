// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.template_symbol;

namespace cmajor::fault::tolerant::symbols {

TemplateParameterSymbol::TemplateParameterSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::templateParameterSymbol, name_, span_)
{
}

void TemplateParameterSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
}

void TemplateParameterSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
