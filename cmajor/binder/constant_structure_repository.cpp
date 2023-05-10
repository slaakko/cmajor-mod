// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.constant.structure.repository;

namespace cmajor::binder {

ConstantStructureRepository::ConstantStructureRepository()
{
}

void ConstantStructureRepository::AddConstantStructure(cmajor::symbols::ConstantSymbol* constantStructureSymbol)
{
    constantStructures.push_back(constantStructureSymbol);
}

} // namespace cmajor::binder
