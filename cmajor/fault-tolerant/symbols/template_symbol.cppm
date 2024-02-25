// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.template_symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class TemplateParameterSymbol : public TypeSymbol
{
public:
    TemplateParameterSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

} // namespace cmajor::fault::tolerant::symbols
