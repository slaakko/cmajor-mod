// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.bound.project;

import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

class BoundProject
{
public:
    void AddBoundCompileUnit(BoundCompileUnit* boundCompileUnit);
    BoundCompileUnit* GetBoundCompileUnit(int index) const;
private:
    std::vector<std::unique_ptr<BoundCompileUnit>> boundCompileUnits;
};

} // namespace cmajor::fault::tolerant::binder
