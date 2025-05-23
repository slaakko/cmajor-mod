// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.identifier;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

IdentifierNode::IdentifierNode(const soul::ast::Span& span_) : Node(NodeType::identifierNode, span_), identifier()
{
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, NodeType nodeType_) : Node(nodeType_, span_), identifier()
{
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, const std::u32string& identifier_) : 
    Node(NodeType::identifierNode, span_), identifier(identifier_)
{
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, NodeType nodeType_, const std::u32string& identifier_) : 
    Node(nodeType_, span_), identifier(identifier_)
{
    std::u32string result;
    for (char32_t c : identifier)
    {
        if (c != '`')
        {
            result.append(1, c);
        }
    }
    std::swap(result, identifier);
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, const Token& token) : Node(NodeType::identifierNode, span_)
{
    identifier = std::u32string(token.match.begin, token.match.end);
}

Node* IdentifierNode::Clone(CloneContext& cloneContext) const
{
    IdentifierNode* clone = new IdentifierNode(GetSpan(), identifier);
    return clone;
}

void IdentifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdentifierNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(identifier);
}

void IdentifierNode::Read(AstReader& reader)
{
    Node::Read(reader);
    identifier = reader.GetBinaryStreamReader().ReadUtf32String();
}

std::string IdentifierNode::ToString() const
{
    return util::ToUtf8(identifier);
}

bool IdentifierNode::IsInternal() const
{
    return !identifier.empty() && identifier.front() == '@';
}

CursorIdNode::CursorIdNode(const soul::ast::Span& span_) : IdentifierNode(span_, NodeType::cursorIdNode)
{
}

CursorIdNode::CursorIdNode(const soul::ast::Span& span_, const std::u32string& identifier_) : 
    IdentifierNode(span_, NodeType::cursorIdNode, identifier_)
{
}

Node* CursorIdNode::Clone(CloneContext& cloneContext) const
{
    CursorIdNode* clone = new CursorIdNode(GetSpan(), Str());
    return clone;
}

void CursorIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CursorIdNode::Write(AstWriter& writer)
{
    IdentifierNode::Write(writer);
}

void CursorIdNode::Read(AstReader& reader)
{
    IdentifierNode::Read(reader);
}

} // namespace cmajor::ast
