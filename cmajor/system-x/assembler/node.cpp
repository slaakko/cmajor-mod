// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.node;

namespace cmajor::systemx::assembler {

Node::Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_) : kind(kind_), sourcePos(sourcePos_)
{
}

Node::~Node()
{
}

void Node::Write(util::CodeFormatter& formatter)
{
}

} // namespace cmajor::systemx::assembler
