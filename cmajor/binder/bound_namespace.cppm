// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound_namespace;

import std.core;
import cmajor.binder.bound.node;
import cmajor.ir;
import cmajor.ast;

export namespace cmajor::binder {

class BoundNodeVisitor;

class BoundNamespace : public BoundNode
{
public:
    BoundNamespace(cmajor::ast::NamespaceNode& namespaceNode_);
    BoundNamespace(const BoundNamespace&) = delete;
    BoundNamespace& operator=(const BoundNamespace&) = delete;
    void Accept(BoundNodeVisitor& visitor) override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    cmajor::ast::NamespaceNode& GetNamespaceNode() { return namespaceNode; }
    void AddMember(std::unique_ptr<BoundNode>&& member);
    const std::vector<std::unique_ptr<BoundNode>>& Members() const { return members; }
private:
    cmajor::ast::NamespaceNode& namespaceNode;
    std::vector<std::unique_ptr<BoundNode>> members;
};

} // namespace cmajor::binder
