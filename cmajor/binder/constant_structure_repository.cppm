// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.constant.structure.repository;

import std.core;
import cmajor.symbols;

export namespace cmajor::binder {

class ConstantStructureRepository
{
public:
    ConstantStructureRepository();
    void AddConstantStructure(cmajor::symbols::ConstantSymbol* constantStructureSymbol);
    const std::vector<cmajor::symbols::ConstantSymbol*>& ConstantStructures() const { return constantStructures; }
private:
    std::vector<cmajor::symbols::ConstantSymbol*> constantStructures;
};

} // namespace cmajor::binder
