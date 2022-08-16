// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.type_expr;

import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;

namespace cmajor::ast {

ConstNode::ConstNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::constNode, sourcePos_, moduleId_)
{
}

ConstNode::ConstNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* subject_) : Node(NodeType::constNode, sourcePos_, moduleId_), subject(subject_)
{
    subject->SetParent(this);
}

Node* ConstNode::Clone(CloneContext& cloneContext) const
{
    ConstNode* clone = new ConstNode(GetSourcePos(), ModuleId(), subject->Clone(cloneContext));
    return clone;
}

void ConstNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
}

void ConstNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
}

std::string ConstNode::ToString() const
{
    return "const " + subject->ToString();
}

LValueRefNode::LValueRefNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::lvalueRefNode, sourcePos_, moduleId_)
{
}

LValueRefNode::LValueRefNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* subject_) :
    Node(NodeType::lvalueRefNode, sourcePos_, moduleId_), subject(subject_)
{
    subject->SetParent(this);
}

Node* LValueRefNode::Clone(CloneContext& cloneContext) const
{
    LValueRefNode* clone = new LValueRefNode(GetSourcePos(), ModuleId(), subject->Clone(cloneContext));
    return clone;
}

void LValueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LValueRefNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
}

void LValueRefNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
}

std::string LValueRefNode::ToString() const
{
    return subject->ToString() + "&";
}

RValueRefNode::RValueRefNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::rvalueRefNode, sourcePos_, moduleId_)
{
}

RValueRefNode::RValueRefNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* subject_) :
    Node(NodeType::rvalueRefNode, sourcePos_, moduleId_), subject(subject_)
{
    subject->SetParent(this);
}

Node* RValueRefNode::Clone(CloneContext& cloneContext) const
{
    RValueRefNode* clone = new RValueRefNode(GetSourcePos(), ModuleId(), subject->Clone(cloneContext));
    return clone;
}

void RValueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RValueRefNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
}

void RValueRefNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
}

std::string RValueRefNode::ToString() const
{
    return subject->ToString() + "&&";
}

PointerNode::PointerNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::pointerNode, sourcePos_, moduleId_)
{
}

PointerNode::PointerNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* subject_) :
    Node(NodeType::pointerNode, sourcePos_, moduleId_), subject(subject_)
{
    subject->SetParent(this);
}

Node* PointerNode::Clone(CloneContext& cloneContext) const
{
    PointerNode* clone = new PointerNode(GetSourcePos(), ModuleId(), subject->Clone(cloneContext));
    return clone;
}

void PointerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PointerNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
}

void PointerNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
}

std::string PointerNode::ToString() const
{
    return subject->ToString() + "*";
}

ArrayNode::ArrayNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::arrayNode, sourcePos_, moduleId_), subject(), size()
{
}

ArrayNode::ArrayNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* subject_, Node* size_) :
    Node(NodeType::arrayNode, sourcePos_, moduleId_), subject(subject_), size(size_)
{
    subject->SetParent(this);
    if (size)
    {
        size->SetParent(this);
    }
}

Node* ArrayNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedSize = nullptr;
    if (size)
    {
        clonedSize = size->Clone(cloneContext);
    }
    ArrayNode* clone = new ArrayNode(GetSourcePos(), ModuleId(), subject->Clone(cloneContext), clonedSize);
    return clone;
}

void ArrayNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
    bool hasSize = size != nullptr;
    writer.GetBinaryStreamWriter().Write(hasSize);
    if (hasSize)
    {
        writer.Write(size.get());
    }
}

void ArrayNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    bool hasSize = reader.GetBinaryStreamReader().ReadBool();
    if (hasSize)
    {
        size.reset(reader.ReadNode());
        size->SetParent(this);
    }
}

std::string ArrayNode::ToString() const
{
    std::string s = subject->ToString();
    s.append("[");
    if (size)
    {
        s.append(size->ToString());
    }
    s.append("]");
    return s;
}
} // namespace cmajor::ast
