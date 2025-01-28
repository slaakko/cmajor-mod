// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.dominator.tree.pass;

import cmajor.systemx.intermediate.pass;
import std.core;

export namespace cmajor::systemx::intermediate {

class DominatorTreeCooper;

class DominatorTreePass : public FunctionPass
{
public:
    DominatorTreePass();
    void Run(Function& function) override;
    void Clear(Function& function) override;
    DominatorTreeCooper* GetDominatorTree() const { return dominatorTree.get(); }
private:
    std::unique_ptr<DominatorTreeCooper> dominatorTree;
};

} // cmajor::systemx::intermediate
