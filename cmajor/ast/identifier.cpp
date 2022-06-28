// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.identifier;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

IdentifierNode::IdentifierNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::identifierNode, sourcePos_, moduleId_), identifier()
{
}
IdentifierNode::IdentifierNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, NodeType nodeType_) : Node(nodeType_, sourcePos_, moduleId_), identifier()
{
}
IdentifierNode::IdentifierNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& identifier_) : Node(NodeType::identifierNode, sourcePos_, moduleId_), identifier(identifier_)
{
}
IdentifierNode::IdentifierNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, NodeType nodeType_, const std::u32string& identifier_) : Node(nodeType_, sourcePos_, moduleId_), identifier(identifier_)
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

IdentifierNode::IdentifierNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const Token& token) : Node(NodeType::identifierNode, sourcePos_, moduleId_)
{
    identifier = std::u32string(token.match.begin, token.match.end);
}

Node* IdentifierNode::Clone(CloneContext& cloneContext) const
{
    IdentifierNode* clone = new IdentifierNode(GetSourcePos(), ModuleId(), identifier);
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

CursorIdNode::CursorIdNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : IdentifierNode(sourcePos_, moduleId_, NodeType::cursorIdNode)
{
}

CursorIdNode::CursorIdNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& identifier_) : IdentifierNode(sourcePos_, moduleId_, NodeType::cursorIdNode, identifier_)
{
}

Node* CursorIdNode::Clone(CloneContext& cloneContext) const
{
    CursorIdNode* clone = new CursorIdNode(GetSourcePos(), ModuleId(), Str());
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
