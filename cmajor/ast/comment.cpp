// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.comment;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

CommentNode::CommentNode(const soul::ast::Span& span_) : Node(NodeType::commentNode, span_)
{
} 

CommentNode::CommentNode(const soul::ast::Span& span_, const std::u32string& comment_) : Node(NodeType::commentNode, span_), comment(comment_)
{
}

Node* CommentNode::Clone(CloneContext& cloneContext) const
{
    CommentNode* clone = new CommentNode(GetSpan(), comment);
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
