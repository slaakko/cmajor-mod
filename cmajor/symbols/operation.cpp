module cmajor.symbols.operation;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.function.symbol;
import std.core;

namespace cmajor::symbols {

Operation::~Operation()
{
}
FunctionSymbol* Operation::Get(std::vector<Operation*>& operations)
{
    if (!fun)
    {
        fun.reset(Create());
        operations.push_back(this);
    }
    return fun.get();
}
void Operation::Release()
{
    fun.reset();
}

} // namespace cmajor::symbols
