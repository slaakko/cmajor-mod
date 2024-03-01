// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.function.symbol;

import cmajor.fault.tolerant.symbols.variable.symbol;
import cmajor.fault.tolerant.symbols.template_symbol;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.type.symbol;


namespace cmajor::fault::tolerant::symbols {

FunctionSymbol::FunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_) : ContainerSymbol(SymbolKind::functionSymbol, name_, span_), returnType(nullptr)
{
}

FunctionSymbol::FunctionSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_) : ContainerSymbol(kind_, name_, span_), returnType(nullptr)
{
}

void FunctionSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    if (returnType)
    {
        writer.BinaryStreamWriter().Write(returnType->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
}

void FunctionSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(returnTypeId);
    if (!returnTypeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void FunctionSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!returnTypeId.is_nil())
    {
        returnType = symbolTable->GetType(returnTypeId);
    }
}

void FunctionSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    ContainerSymbol::AddSymbol(symbol, symbolTable);
    if (symbol->IsTemplateParameterSymbol())
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(symbol));
    }
    else if (symbol->IsParameterSymbol())
    {
        parameters.push_back(static_cast<ParameterSymbol*>(symbol));
    }
    else if (symbol->IsLocalVariableSymbol())
    {
        localVariables.push_back(static_cast<LocalVariableSymbol*>(symbol));
    }
}

StaticConstructorSymbol::StaticConstructorSymbol(const std::u16string& name_, const soul::ast::Span& span_) : FunctionSymbol(SymbolKind::staticConstructorSymbol, name_, span_)
{
}

void StaticConstructorSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
}

void StaticConstructorSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
}

ConstructorSymbol::ConstructorSymbol(const std::u16string& name_, const soul::ast::Span& span_) : FunctionSymbol(SymbolKind::constructorSymbol, name_, span_)
{
}

void ConstructorSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
}

void ConstructorSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
}

DestructorSymbol::DestructorSymbol(const std::u16string& name_, const soul::ast::Span& span_) : FunctionSymbol(SymbolKind::destructorSymbol, name_, span_)
{
}

void DestructorSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
}

void DestructorSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
}

MemberFunctionSymbol::MemberFunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_) : FunctionSymbol(SymbolKind::memberFunctionSymbol, name_, span_)
{
}

void MemberFunctionSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
}

void MemberFunctionSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
}

ConversionFunctionSymbol::ConversionFunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_) : FunctionSymbol(SymbolKind::conversionFunctionSymbol, name_, span_)
{
}

void ConversionFunctionSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
}

void ConversionFunctionSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
