// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.comment;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

CommentNode::CommentNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::commentNode, sourcePos_, moduleId_)
{
}

CommentNode::CommentNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& comment_) : Node(NodeType::commentNode, sourcePos_, moduleId_), comment(comment_)
{
}

Node* CommentNode::Clone(CloneContext& cloneContext) const
{
    CommentNode* clone = new CommentNode(GetSourcePos(), ModuleId(), comment);
    return clone;
}

void CommentNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CommentNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(comment);
}

void CommentNode::Read(AstReader& reader)
{
    Node::Read(reader);
    comment = reader.GetBinaryStreamReader().ReadUtf32String();
}
} // namespace cmajor::ast
