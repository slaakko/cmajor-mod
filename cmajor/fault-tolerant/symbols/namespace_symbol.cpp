// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.namespace_symbol;

import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.basic.type.symbol;
import cmajor.fault.tolerant.ast;

namespace cmajor::fault::tolerant::symbols {

NamespaceSymbol::NamespaceSymbol(const std::u16string& name_) : ContainerSymbol(SymbolKind::namespaceSymbol, name_, soul::ast::Span())
{
}

void NamespaceSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
}

void NamespaceSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
}

void NamespaceSymbol::Import(const NamespaceSymbol* that, SymbolTable* symbolTable)
{
    cmajor::fault::tolerant::ast::IdentifierNode id(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none, that->Name());
    bool added = symbolTable->BeginNamespace(&id, nullptr, nullptr, false);
    if (added)
    {
        for (const auto& symbol : that->Symbols())
        {
            if (symbol->IsNamespaceSymbol())
            {
                const NamespaceSymbol* thatNs = static_cast<NamespaceSymbol*>(symbol.get());
                Import(thatNs, symbolTable);
            }
            else
            {
                symbolTable->Container()->Scope()->Install(symbol.get());
            }
        }
        symbolTable->EndNamespace();
    }
}

GlobalNamespaceSymbol::GlobalNamespaceSymbol() : NamespaceSymbol(std::u16string()), symbolTable(nullptr)
{
}

void GlobalNamespaceSymbol::Write(SymbolWriter& writer)
{
    NamespaceSymbol::Write(writer);
}

void GlobalNamespaceSymbol::Read(SymbolReader& reader)
{
    NamespaceSymbol::Read(reader);
}

void GlobalNamespaceSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    NamespaceSymbol::AddSymbol(symbol, symbolTable);
    if (symbol->IsBasicTypeSymbol())
    {
        symbolTable->AddBasicType(static_cast<BasicTypeSymbol*>(symbol));
    }
}

} // namespace cmajor::fault::tolerant::symbols
