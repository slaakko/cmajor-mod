// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.declaration.block.symbol;

import cmajor.fault.tolerant.symbols.container.symbol;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class DeclarationBlockSymbol : public ContainerSymbol
{
public:
    DeclarationBlockSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

} // namespace cmajor::fault::tolerant::symbols
