// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound_class;

import cmajor.binder.bound.node;
import cmajor.symbols;
import cmajor.ir;

namespace cmajor::binder {

class BoundNodeVisitor;

class BoundClass : public BoundNode
{
public:
    BoundClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol_);
    BoundClass(const BoundClass&) = delete;
    BoundClass& operator=(const BoundClass&) = delete;
    void Accept(BoundNodeVisitor& visitor) override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void AddMember(std::unique_ptr<BoundNode>&& member);
    const std::vector<std::unique_ptr<BoundNode>>& Members() const { return members; }
    cmajor::symbols::ClassTypeSymbol* GetClassTypeSymbol() { return classTypeSymbol; }
    bool ContainsSourceFunctions() const;
    bool IsInlineFunctionContainer() const { return inlineFunctionContainer; }
    void SetInlineFunctionContainer() { inlineFunctionContainer = true; }
private:
    cmajor::symbols::ClassTypeSymbol* classTypeSymbol;
    std::vector<std::unique_ptr<BoundNode>> members;
    bool inlineFunctionContainer;
};

} // namespace cmajor::binder
