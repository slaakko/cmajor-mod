// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.bound.compile.unit;

import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit
{
public:
    BoundCompileUnit();
    void AddFileScope(cmajor::fault::tolerant::symbols::FileScope* fileScope);
    cmajor::fault::tolerant::symbols::FileScope* FirstFileScope() { return fileScopes.front().get(); }
    const std::vector<std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>>& FileScopes() const { return fileScopes; }
private:
    std::vector<std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>> fileScopes;
};

} // namespace cmajor::fault::tolerant::binder
