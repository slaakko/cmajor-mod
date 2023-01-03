module cmajor.ast.sync.node;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <sngcm/ast/Visitor.hpp>
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
