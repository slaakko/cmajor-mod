// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.instantiation_guard;

namespace cmajor::binder {

InstantiationGuard::InstantiationGuard(cmajor::symbols::SymbolTable& symbolTable_, int32_t fileIndex, const util::uuid& moduleId) : symbolTable(symbolTable_)
{
    symbolTable.PushInstantiatingTemplate();
    symbolTable.PushCurrentFileIndex(fileIndex);
    symbolTable.PushCurrentModuleId(moduleId);
}

InstantiationGuard::~InstantiationGuard()
{
    symbolTable.PopCurrentModuleId();
    symbolTable.PopCurrentFileIndex();
    symbolTable.PopInstantiatingTemplate();
}

} // namespace cmajor::binder
