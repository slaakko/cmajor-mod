// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.sync.node;

import cmajor.ast.visitor;

namespace cmajor::ast {

SyncNode::SyncNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::syncNode, sourcePos_, moduleId_)
{
}

Node* SyncNode::Clone(CloneContext& cloneContext) const
{
    return new SyncNode(GetSourcePos(), ModuleId());
}

void SyncNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::ast
