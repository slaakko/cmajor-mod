// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol.reader;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.symbol.factory;

namespace cmajor::fault::tolerant::symbols {

SymbolReader::SymbolReader(const std::string& filePath_) : 
    filePath(filePath_), fileStream(filePath, util::OpenMode::read | util::OpenMode::binary), bufferedStream(fileStream), reader(bufferedStream), symbolTable(nullptr)
{
}

soul::ast::Span SymbolReader::ReadSpan()
{
    bool valid = reader.ReadBool();
    if (valid)
    {
        int pos = reader.ReadInt();
        int len = reader.ReadInt();
        return soul::ast::Span(pos, len);
    }
    else
    {
        return soul::ast::Span();
    }
}

Symbol* SymbolReader::ReadSymbol()
{
    SymbolKind kind = static_cast<SymbolKind>(reader.ReadByte());
    std::u16string name = reader.ReadUtf16String();
    soul::ast::Span span = ReadSpan();
    Symbol* symbol = CreateSymbol(kind, name, span);
    symbol->Read(*this);
    return symbol;
}

void SymbolReader::AddSymbolToBeResolved(Symbol* symbol)
{
    toBeResolved.push_back(symbol);
}

void SymbolReader::ResolveSymbols()
{
    for (auto symbol : toBeResolved)
    {
        symbol->Resolve(symbolTable);
    }
    toBeResolved.clear();
}

} // namespace cmajor::fault::tolerant::symbols
