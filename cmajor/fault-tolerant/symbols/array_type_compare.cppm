// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.array.type.compare;

export namespace cmajor::fault::tolerant::symbols {

class ArrayTypeSymbol;

struct ArrayTypeLess
{
    bool operator()(ArrayTypeSymbol* left, ArrayTypeSymbol* right) const;
};

} // namespace cmajor::fault::tolerant::symbols
