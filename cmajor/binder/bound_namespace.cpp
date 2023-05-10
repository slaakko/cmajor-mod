// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound_namespace;

import cmajor.binder.bound.node.visitor;
import cmajor.symbols;

namespace cmajor::binder {

BoundNamespace::BoundNamespace(cmajor::ast::NamespaceNode& namespaceNode_) : BoundNode(namespaceNode_.GetSourcePos(), namespaceNode_.ModuleId(), BoundNodeType::boundNamespace), 
    namespaceNode(namespaceNode_)
{
}

void BoundNamespace::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundNamespace::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load a namespace", GetSourcePos(), ModuleId());
}

void BoundNamespace::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store a namespace", GetSourcePos(), ModuleId());
}

void BoundNamespace::AddMember(std::unique_ptr<BoundNode>&& member)
{
    members.push_back(std::move(member));
}

} // namespace cmajor::binder
