// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.delegate.symbol;

import cmajor.fault.tolerant.symbols.variable.symbol;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

DelegateSymbol::DelegateSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::delegateSymbol, name_, span_), returnType(nullptr)
{
}

void DelegateSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    if (returnType)
    {
        writer.BinaryStreamWriter().Write(returnType->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
}

void DelegateSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(returnTypeId);
    if (!returnTypeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void DelegateSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!returnTypeId.is_nil())
    {
        returnType = symbolTable->GetType(returnTypeId);
    }
}

void DelegateSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    TypeSymbol::AddSymbol(symbol, symbolTable);
    if (symbol->IsParameterSymbol())
    {
        parameters.push_back(static_cast<ParameterSymbol*>(symbol));
    }
}

ClassDelegateSymbol::ClassDelegateSymbol(const std::u16string& name_, const soul::ast::Span& span_) : TypeSymbol(SymbolKind::classDelegateSymbol, name_, span_), returnType(nullptr)
{
}

void ClassDelegateSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    if (returnType)
    {
        writer.BinaryStreamWriter().Write(returnType->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
}

void ClassDelegateSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(returnTypeId);
    if (!returnTypeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void ClassDelegateSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!returnTypeId.is_nil())
    {
        returnType = symbolTable->GetType(returnTypeId);
    }
}

void ClassDelegateSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    TypeSymbol::AddSymbol(symbol, symbolTable);
    if (symbol->IsParameterSymbol())
    {
        parameters.push_back(static_cast<ParameterSymbol*>(symbol));
    }
}

} // namespace cmajor::fault::tolerant::symbols
