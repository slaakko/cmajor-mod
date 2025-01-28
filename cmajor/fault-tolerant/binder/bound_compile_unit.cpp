// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.bound.compile.unit;

namespace cmajor::fault::tolerant::binder {

BoundCompileUnit::BoundCompileUnit()
{
}

void BoundCompileUnit::AddFileScope(cmajor::fault::tolerant::symbols::FileScope* fileScope)
{
    fileScopes.push_back(std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>(fileScope));
}

} // namespace cmajor::fault::tolerant::binder
