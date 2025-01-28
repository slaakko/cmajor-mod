// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.array.type.symbol;

import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;
import util;

namespace cmajor::fault::tolerant::symbols {

std::u16string MakeArrayTypeName(TypeSymbol* elementType, int64_t size)
{
    std::u16string arrayTypeName;
    arrayTypeName.append(elementType->Name());
    if (size == -1)
    {
        arrayTypeName.append(u"[]");
    }
    else
    {
        arrayTypeName.append(1, '[');
        arrayTypeName.append(util::ToUtf16(std::to_string(size)));
        arrayTypeName.append(1, ']');
    }
    return arrayTypeName;
}

ArrayTypeSymbol::ArrayTypeSymbol(TypeSymbol* elementType_, int64_t size_) : 
    TypeSymbol(SymbolKind::arrayTypeSymbol, MakeArrayTypeName(elementType_, size_), soul::ast::Span()), elementType(elementType_), size(size_), elementTypeId()
{
}

ArrayTypeSymbol::ArrayTypeSymbol(const std::u16string& name_) : 
    TypeSymbol(SymbolKind::arrayTypeSymbol, name_, soul::ast::Span()), elementType(nullptr), size(-1), elementTypeId()
{
}

void ArrayTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    if (elementType)
    {
        writer.BinaryStreamWriter().Write(elementType->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
}

void ArrayTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(elementTypeId);
    if (!elementTypeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void ArrayTypeSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!elementTypeId.is_nil())
    {
        elementType = symbolTable->GetType(elementTypeId);
    }
}

} // namespace cmajor::fault::tolerant::symbols
