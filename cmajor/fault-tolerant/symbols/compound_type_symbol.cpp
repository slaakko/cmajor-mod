// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.compound.type.symbol;

import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;

namespace cmajor::fault::tolerant::symbols {

std::u16string MakeCompoundTypeName(TypeSymbol* baseType, const Derivations& derivations)
{
    std::u16string name;
    if (HasDerivation(derivations, Derivation::constDerivation))
    {
        name.append(u"const ");
    }
    name.append(baseType->Name());
    int pointerCount = PointerCount(derivations);
    if (pointerCount > 0)
    {
        name.append(pointerCount, '*');
    }
    if (HasDerivation(derivations, Derivation::lvalueRefDerivation))
    {
        name.append(u"&");
    }
    else if (HasDerivation(derivations, Derivation::rvalueRefDerivation))
    {
        name.append(u"&&");
    }
    return name;
}

CompoundTypeSymbol::CompoundTypeSymbol(const std::u16string& name_) : TypeSymbol(SymbolKind::compoundTypeSymbol, name_, soul::ast::Span()), baseType(nullptr)
{
}

CompoundTypeSymbol::CompoundTypeSymbol(TypeSymbol* baseType_, const Derivations& derivations_) : 
    TypeSymbol(SymbolKind::compoundTypeSymbol, MakeCompoundTypeName(baseType_, derivations_), soul::ast::Span()), baseType(baseType_), derivations(derivations_)
{
}

TypeSymbol* CompoundTypeSymbol::PlainType(SymbolTable* symbolTable)
{
    Derivations plainDerivations = Plain(derivations);
    if (baseType)
    {
        return symbolTable->MakeCompoundType(baseType, plainDerivations);
    }
    else
    {
        return nullptr;
    }
}

void CompoundTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    if (baseType)
    {
        writer.BinaryStreamWriter().Write(baseType->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
    cmajor::fault::tolerant::symbols::Write(writer, derivations);
}

void CompoundTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(baseTypeId);
    cmajor::fault::tolerant::symbols::Read(reader, derivations);
    if (!baseTypeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void CompoundTypeSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!baseTypeId.is_nil())
    {
        baseType = symbolTable->GetType(baseTypeId);
    }
}

} // namespace cmajor::fault::tolerant::symbols
