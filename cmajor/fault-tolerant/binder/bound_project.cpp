// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.bound.project;

import cmajor.fault.tolerant.binder.bound.compile.unit;

namespace cmajor::fault::tolerant::binder {

void BoundProject::AddBoundCompileUnit(BoundCompileUnit* boundCompileUnit)
{
    boundCompileUnits.push_back(std::unique_ptr<BoundCompileUnit>(boundCompileUnit));
}

BoundCompileUnit* BoundProject::GetBoundCompileUnit(int index) const
{
    if (index >= 0 && index < boundCompileUnits.size())
    {
        return boundCompileUnits[index].get();
    }
    else
    {
        throw std::runtime_error("invalid bound compile unit index");
    }
}

} // namespace cmajor::fault::tolerant::binder
