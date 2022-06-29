// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.template_;

import std.core;
import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
//import util;

namespace cmajor::ast {

TemplateIdNode::TemplateIdNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::templateIdNode, sourcePos_, moduleId_)
{
}

TemplateIdNode::TemplateIdNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* primary_) :
    Node(NodeType::templateIdNode, sourcePos_, moduleId_), primary(primary_)
{
    primary->SetParent(this);
}

Node* TemplateIdNode::Clone(CloneContext& cloneContext) const
{
    TemplateIdNode* clone = new TemplateIdNode(GetSourcePos(), ModuleId(), primary->Clone(cloneContext));
    int n = templateArguments.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* templateArgument = templateArguments[i];
        clone->AddTemplateArgument(templateArgument->Clone(cloneContext));
    }
    return clone;
}

void TemplateIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateIdNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(primary.get());
    templateArguments.Write(writer);
}

void TemplateIdNode::Read(AstReader& reader)
{
    Node::Read(reader);
    primary.reset(reader.ReadNode());
    primary->SetParent(this);
    templateArguments.Read(reader);
    templateArguments.SetParent(this);
}

std::string TemplateIdNode::ToString() const
{
    std::string s = primary->ToString();
    s.append("<");
    int n = templateArguments.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(templateArguments[i]->ToString());
    }
    s.append(">");
    return s;
}

void TemplateIdNode::AddTemplateArgument(Node* templateArgument)
{
    templateArgument->SetParent(this);
    templateArguments.Add(templateArgument);
}

TemplateParameterNodeX::TemplateParameterNodeX(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::templateParameterNode, sourcePos_, moduleId_), id()
{
}

TemplateParameterNodeX::TemplateParameterNodeX(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, IdentifierNode* id_, Node* defaultTemplateArgument_) :
    Node(NodeType::templateParameterNode, sourcePos_, moduleId_), id(id_), defaultTemplateArgument(defaultTemplateArgument_)
{
    id->SetParent(this);
    if (defaultTemplateArgument)
    {
        defaultTemplateArgument->SetParent(this);
    }
}

Node* TemplateParameterNodeX::Clone(CloneContext& cloneContext) const
{
    Node* clonedDefaultTemplateArgument = nullptr;
    if (defaultTemplateArgument)
    {
        clonedDefaultTemplateArgument = defaultTemplateArgument->Clone(cloneContext);
    }
    TemplateParameterNodeX* clone = new TemplateParameterNodeX(GetSourcePos(), ModuleId(), static_cast<IdentifierNode*>(id->Clone(cloneContext)), clonedDefaultTemplateArgument);
    return clone;
}

void TemplateParameterNodeX::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateParameterNodeX::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    bool hasDefaultTemplateArgument = defaultTemplateArgument != nullptr;
    writer.GetBinaryStreamWriter().Write(hasDefaultTemplateArgument);
    if (hasDefaultTemplateArgument)
    {
        writer.Write(defaultTemplateArgument.get());
    }
}

void TemplateParameterNodeX::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    bool hasDefaultTemplateArgument = reader.GetBinaryStreamReader().ReadBool();
    if (hasDefaultTemplateArgument)
    {
        defaultTemplateArgument.reset(reader.ReadNode());
        defaultTemplateArgument->SetParent(this);
    }
}

FullInstantiationRequestNode::FullInstantiationRequestNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::fullInstantiationRequestNode, sourcePos_, moduleId_), templateId()
{
}

FullInstantiationRequestNode::FullInstantiationRequestNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, TemplateIdNode* templateId_) :
    Node(NodeType::fullInstantiationRequestNode, sourcePos_, moduleId_), templateId(templateId_)
{
}

Node* FullInstantiationRequestNode::Clone(CloneContext& cloneContext) const
{
    return new FullInstantiationRequestNode(GetSourcePos(), ModuleId(), static_cast<TemplateIdNode*>(templateId->Clone(cloneContext)));
}

void FullInstantiationRequestNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FullInstantiationRequestNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(templateId.get());
}

void FullInstantiationRequestNode::Read(AstReader& reader)
{
    Node::Read(reader);
    templateId.reset(reader.ReadTemplateIdNode());
}
} // namespace cmajor::ast
