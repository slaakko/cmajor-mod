// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.sync.node;

import cmajor.ast.visitor;

namespace cmajor::ast {

SyncNode::SyncNode(const soul::ast::Span& span_) : Node(NodeType::syncNode, span_)
{
}

Node* SyncNode::Clone(CloneContext& cloneContext) const
{
    return new SyncNode(GetSpan());
}

void SyncNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::ast
