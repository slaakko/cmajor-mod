// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.constant.array.repository;

import std.core;
import cmajor.symbols;

export namespace cmajor::binder {

class ConstantArrayRepository
{
public:
    ConstantArrayRepository();
    void AddConstantArray(cmajor::symbols::ConstantSymbol* constantArraySymbol);
    const std::vector<cmajor::symbols::ConstantSymbol*>& ConstantArrays() const { return constantArrays; }
private:
    std::vector<cmajor::symbols::ConstantSymbol*> constantArrays;
};

} // namespace cmajor::binder
