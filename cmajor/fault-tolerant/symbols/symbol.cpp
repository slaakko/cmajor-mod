// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol;

import cmajor.fault.tolerant.symbols.container.symbol;
import cmajor.fault.tolerant.symbols.source;
import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;

namespace cmajor::fault::tolerant::symbols {

Symbol::Symbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_) : 
    kind(kind_), name(name_), span(span_), specifiers(cmajor::fault::tolerant::ast::Specifiers::none), 
    parent(nullptr), index(-1), source(nullptr)
{
}

Symbol::~Symbol()
{
}

bool Symbol::IsBasicTypeSymbol() const
{
    switch (kind)
    {
        case SymbolKind::autoTypeSymbol:
        case SymbolKind::boolTypeSymbol:
        case SymbolKind::sbyteTypeSymbol:
        case SymbolKind::byteTypeSymbol:
        case SymbolKind::shortTypeSymbol:
        case SymbolKind::ushortTypeSymbol:
        case SymbolKind::intTypeSymbol:
        case SymbolKind::uintTypeSymbol:
        case SymbolKind::longTypeSymbol:
        case SymbolKind::ulongTypeSymbol:
        case SymbolKind::floatTypeSymbol:
        case SymbolKind::doubleTypeSymbol:
        case SymbolKind::charTypeSymbol:
        case SymbolKind::wcharTypeSymbol:
        case SymbolKind::ucharTypeSymbol:
        case SymbolKind::voidTypeSymbol:
        {
            return true;
        }
    }
    return false;
}

bool Symbol::IsContainerSymbol() const
{
    switch (kind)
    {
        case SymbolKind::namespaceSymbol:
        case SymbolKind::delegateSymbol:
        case SymbolKind::classDelegateSymbol:
        case SymbolKind::enumTypeSymbol:
        case SymbolKind::templateParameterSymbol:
        case SymbolKind::functionSymbol:
        case SymbolKind::staticConstructorSymbol:
        case SymbolKind::constructorSymbol:
        case SymbolKind::destructorSymbol:
        case SymbolKind::memberFunctionSymbol:
        case SymbolKind::conversionFunctionSymbol:
        case SymbolKind::classTypeSymbol:
        case SymbolKind::interfaceTypeSymbol:
        case SymbolKind::conceptSymbol:
        case SymbolKind::declarationBlockSymbol:
        case SymbolKind::typeParameterSymbol:
        case SymbolKind::compoundTypeSymbol:
        case SymbolKind::arrayTypeSymbol:
        case SymbolKind::autoTypeSymbol:
        case SymbolKind::boolTypeSymbol:
        case SymbolKind::sbyteTypeSymbol:
        case SymbolKind::byteTypeSymbol:
        case SymbolKind::shortTypeSymbol:
        case SymbolKind::ushortTypeSymbol:
        case SymbolKind::intTypeSymbol:
        case SymbolKind::uintTypeSymbol:
        case SymbolKind::longTypeSymbol:
        case SymbolKind::ulongTypeSymbol:
        case SymbolKind::floatTypeSymbol:
        case SymbolKind::doubleTypeSymbol:
        case SymbolKind::charTypeSymbol:
        case SymbolKind::wcharTypeSymbol:
        case SymbolKind::ucharTypeSymbol:
        case SymbolKind::voidTypeSymbol:
        {
            return true;
        }
    }
    return false;
}

bool Symbol::IsTypeSymbol() const
{
    switch (kind)
    {
        case SymbolKind::delegateSymbol:
        case SymbolKind::classDelegateSymbol:
        case SymbolKind::enumTypeSymbol:
        case SymbolKind::templateParameterSymbol:
        case SymbolKind::classTypeSymbol:
        case SymbolKind::interfaceTypeSymbol:
        case SymbolKind::compoundTypeSymbol:
        case SymbolKind::arrayTypeSymbol:
        case SymbolKind::autoTypeSymbol:
        case SymbolKind::boolTypeSymbol:
        case SymbolKind::sbyteTypeSymbol:
        case SymbolKind::byteTypeSymbol:
        case SymbolKind::shortTypeSymbol:
        case SymbolKind::ushortTypeSymbol:
        case SymbolKind::intTypeSymbol:
        case SymbolKind::uintTypeSymbol:
        case SymbolKind::longTypeSymbol:
        case SymbolKind::ulongTypeSymbol:
        case SymbolKind::floatTypeSymbol:
        case SymbolKind::doubleTypeSymbol:
        case SymbolKind::charTypeSymbol:
        case SymbolKind::wcharTypeSymbol:
        case SymbolKind::ucharTypeSymbol:
        case SymbolKind::voidTypeSymbol:
        {
            return true;
        }
    }
    return false;
}

void Symbol::Write(SymbolWriter& writer)
{
    writer.BinaryStreamWriter().Write(static_cast<uint8_t>(flags));
    writer.BinaryStreamWriter().Write(static_cast<int32_t>(specifiers));
    if (source)
    {
        writer.BinaryStreamWriter().Write(source->Index());
    }
    else
    {
        writer.BinaryStreamWriter().Write(static_cast<int>(-1));
    }
}

void Symbol::Read(SymbolReader& reader)
{
    flags = static_cast<SymbolFlags>(reader.BinaryStreamReader().ReadByte());
    specifiers = static_cast<cmajor::fault::tolerant::ast::Specifiers>(reader.BinaryStreamReader().ReadInt());
    int sourceIndex = reader.BinaryStreamReader().ReadInt();
    if (sourceIndex != -1)
    {
        Source* source = reader.GetSymbolTable()->GetSource(sourceIndex);
        if (source)
        {
            source->AddSymbol(this);
        }
    }
}

void Symbol::RemoveFromParent(ErrorContainer* errors, std::vector<std::unique_ptr<Symbol>>& removedSymbols)
{
    if (parent)
    {
        std::unique_ptr<Symbol> symbol = parent->RemoveSymbol(index, errors);
        if (symbol)
        {
            removedSymbols.push_back(std::move(symbol));
        }
    }
}

SymbolTable* Symbol::GetSymbolTable()
{
    if (parent)
    {
        return parent->GetSymbolTable();
    }
    else
    {
        return nullptr;
    }
}

ContainerScope* Symbol::Scope()
{
    if (parent)
    {
        return parent->Scope();
    }
    else
    {
        return nullptr;
    }
}

void Symbol::Resolve(SymbolTable* symbolTable)
{
}

void Symbol::Compact()
{
}

} // namespace cmajor::fault::tolerant::symbols
