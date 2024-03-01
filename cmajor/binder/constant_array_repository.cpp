// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.constant.array.repository;

namespace cmajor::binder {

ConstantArrayRepository::ConstantArrayRepository()
{
}

void ConstantArrayRepository::AddConstantArray(cmajor::symbols::ConstantSymbol* constantArraySymbol)
{
    constantArrays.push_back(constantArraySymbol);
}

} // namespace cmajor::binder
