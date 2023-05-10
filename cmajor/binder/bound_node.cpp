// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound.node;

namespace cmajor::binder {

BoundNode::BoundNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundNodeType boundNodeType_) : 
    sourcePos(sourcePos_), moduleId(moduleId_), boundNodeType(boundNodeType_)
{
}

} // namespace cmajor::binder
