// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.array.type.compare;

import cmajor.fault.tolerant.symbols.array.type.symbol;

namespace cmajor::fault::tolerant::symbols {

bool ArrayTypeLess::operator()(ArrayTypeSymbol* left, ArrayTypeSymbol* right) const
{
    if (left->ElementType() < right->ElementType()) return true;
    if (left->ElementType() > right->ElementType()) return false;
    return left->Size() < right->Size();
}

} // namespace cmajor::fault::tolerant::symbols
