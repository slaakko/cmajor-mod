// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.class_symbol;

import cmajor.fault.tolerant.symbols.template_symbol;
import cmajor.fault.tolerant.symbols.variable.symbol;
import cmajor.fault.tolerant.symbols.interface.symbol;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

ClassTypeSymbol::ClassTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::classTypeSymbol, name_, span_), baseClass(nullptr)
{
}

void ClassTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    if (baseClass)
    {
        writer.BinaryStreamWriter().Write(baseClass->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
    int32_t n = interfaces.size();
    writer.BinaryStreamWriter().Write(n);
    for (auto interface : interfaces)
    {
        writer.BinaryStreamWriter().Write(interface->Id());
    }
}

void ClassTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(baseClassId);
    int32_t n = reader.BinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        util::uuid interfaceId;
        reader.BinaryStreamReader().ReadUuid(interfaceId);
        interfaceIds.push_back(interfaceId);
    }
    if (!baseClassId.is_nil() || n != 0)
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void ClassTypeSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!baseClassId.is_nil())
    {
        TypeSymbol* baseClassType = symbolTable->GetType(baseClassId);
        if (baseClassType && baseClassType->IsClassTypeSymbol())
        {
            baseClass = static_cast<ClassTypeSymbol*>(baseClassType);
        }
    }
    for (const auto& interfaceId : interfaceIds)
    {
        TypeSymbol* typeSymbol = symbolTable->GetType(interfaceId);
        if (typeSymbol && typeSymbol->IsInterfaceTypeSymbol())
        {
            InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<InterfaceTypeSymbol*>(typeSymbol);
            interfaces.push_back(interfaceTypeSymbol);
        }
    }
}

void ClassTypeSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    TypeSymbol::AddSymbol(symbol, symbolTable);
    if (symbol->IsTemplateParameterSymbol())
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(symbol));
    }
    else if (symbol->IsMemberVariableSymbol())
    {
        memberVariables.push_back(static_cast<MemberVariableSymbol*>(symbol));
    }
}

void ClassTypeSymbol::AddInterface(InterfaceTypeSymbol* interface)
{
    interfaces.push_back(interface);
}

} // namespace cmajor::fault::tolerant::symbols
