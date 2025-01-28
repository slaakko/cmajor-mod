// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound_namespace;

import cmajor.binder.bound.node.visitor;
import cmajor.symbols;

namespace cmajor::binder {

BoundNamespace::BoundNamespace(cmajor::ast::NamespaceNode& namespaceNode_) : BoundNode(namespaceNode_.GetSpan(), BoundNodeType::boundNamespace), namespaceNode(namespaceNode_)
{
}

void BoundNamespace::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundNamespace::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load a namespace", GetFullSpan());
}

void BoundNamespace::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store a namespace", GetFullSpan());
}

void BoundNamespace::AddMember(std::unique_ptr<BoundNode>&& member)
{
    member->SetParent(this);
    members.push_back(std::move(member));
}

} // namespace cmajor::binder
