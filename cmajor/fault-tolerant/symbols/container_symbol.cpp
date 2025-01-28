// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.container.symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;

namespace cmajor::fault::tolerant::symbols {

ContainerSymbol::ContainerSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_) : Symbol(kind_, name_, span_), count(0)
{
    scope.SetContainer(this);
}

void ContainerSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    int32_t n = symbols.size();
    writer.BinaryStreamWriter().Write(n);
    for (const auto& symbol : symbols)
    {
        writer.Write(symbol.get());
    }
}

void ContainerSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    int n = reader.BinaryStreamReader().ReadInt();
    for (int i = 0; i < n; ++i)
    {
        Symbol* symbol = reader.ReadSymbol();
        AddSymbol(symbol, reader.GetSymbolTable());
    }
}

void ContainerSymbol::AddSymbol(Symbol* symbol, SymbolTable* symbolTable)
{
    symbol->SetParent(this);
    symbol->SetIndex(symbols.size());
    symbols.push_back(std::unique_ptr<Symbol>(symbol));
    ++count;
    scope.Install(symbol);
    if (symbol->IsTypeSymbol())
    {
        symbolTable->MapType(static_cast<TypeSymbol*>(symbol));
    }
}

std::unique_ptr<Symbol> ContainerSymbol::RemoveSymbol(int index, ErrorContainer* errors)
{
    if (index == -1)
    {
        errors->AddError(std::runtime_error("error: ContainerSymboil::RemoveSymbol(): symbol index is -1"));
        return std::unique_ptr<Symbol>();
    }
    else
    {
        std::unique_ptr<Symbol> symbol = std::move(symbols[index]);
        if (symbol)
        {
            symbol->SetIndex(-1);
            --count;
            if (symbol->IsInstalled())
            {
                scope.Uninstall(symbol.get());
            }
            symbol->SetParent(nullptr);
            symbols[index].reset();
        }
        else
        {
            errors->AddError(std::runtime_error("error: ContainerSymboil::RemoveSymbol(): symbol '" + std::to_string(index) + "' already removed"));
        }
        return symbol;
    }
}

void ContainerSymbol::Compact()
{
    count = 0;
    int n = symbols.size();
    int index = 0;
    for (int i = 0; i < n; ++i)
    {
        Symbol* symbol = symbols[i].get();
        if (symbol)
        {
            if (symbol->IsContainerSymbol())
            {
                ContainerSymbol* container = static_cast<ContainerSymbol*>(symbols[i].get());
                container->Compact();
            }
            if (i != index)
            {
                std::unique_ptr<Symbol> symbol(symbols[i].release());
                symbol->SetIndex(index);
                symbols[index].reset(symbol.release());
                ++index;
            }
            else
            {
                ++index;
            }
            ++count;
        }
    }
    if (index != n)
    {
        symbols.erase(symbols.begin() + index, symbols.end());
    }
}

} // namespace cmajor::fault::tolerant::symbols
