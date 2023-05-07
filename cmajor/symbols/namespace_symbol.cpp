// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.namespaces;

import cmajor.symbols.symbol.table;

namespace cmajor::symbols {

NamespaceSymbol::NamespaceSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    ContainerSymbol(SymbolType::namespaceSymbol, sourcePos_, sourceModuleId_, name_)
{
}

void NamespaceSymbol::Import(NamespaceSymbol* that, SymbolTable& symbolTable)
{
    NamespaceSymbol* ns = symbolTable.BeginNamespace(that->Name(), that->GetSourcePos(), that->SourceModuleId());
    symbolTable.MapNs(that, ns);
    for (const std::unique_ptr<Symbol>& symbol : that->Members())
    {
        if (symbol->GetSymbolType() == SymbolType::namespaceSymbol)
        {
            NamespaceSymbol* thatNs = static_cast<NamespaceSymbol*>(symbol.get());
            Import(thatNs, symbolTable);
        }
        else
        {
            if (symbol->GetSymbolType() == SymbolType::functionGroupSymbol)
            {
                continue;
            }
            if (symbol->GetSymbolType() == SymbolType::conceptGroupSymbol)
            {
                continue;
            }
            if (symbol->GetSymbolType() == SymbolType::classGroupTypeSymbol)
            {
                continue;
            }
            if (symbol->GetSymbolType() == SymbolType::globalVariableGroupSymbol)
            {
                continue;
            }
            symbolTable.Container()->AddOwnedMember(symbol.get());
        }
    }
    symbolTable.EndNamespace();
}

std::unique_ptr<soul::xml::Element> NamespaceSymbol::CreateDomElement(TypeMap& typeMap)
{
    if (HasProjectMembers() && Access() != SymbolAccess::private_)
    {
        return std::unique_ptr<soul::xml::Element>(soul::xml::MakeElement("NamespaceSymbol"));
    }
    else
    {
        return std::unique_ptr<soul::xml::Element>();
    }
}

} // namespace cmajor::symbols
