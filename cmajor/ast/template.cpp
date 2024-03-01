// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.templates;

import std.core;
import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;

namespace cmajor::ast {

TemplateIdNode::TemplateIdNode(const soul::ast::Span& span_) : Node(NodeType::templateIdNode, span_)
{
}

TemplateIdNode::TemplateIdNode(const soul::ast::Span& span_, Node* primary_) :
    Node(NodeType::templateIdNode, span_), primary(primary_)
{
    primary->SetParent(this);
}

Node* TemplateIdNode::Clone(CloneContext& cloneContext) const
{
    TemplateIdNode* clone = new TemplateIdNode(GetSpan(), primary->Clone(cloneContext));
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

TemplateParameterNode::TemplateParameterNode(const soul::ast::Span& span_) : 
    Node(NodeType::templateParameterNode, span_), id()
{
}

TemplateParameterNode::TemplateParameterNode(const soul::ast::Span& span_, IdentifierNode* id_, Node* defaultTemplateArgument_) :
    Node(NodeType::templateParameterNode, span_), id(id_), defaultTemplateArgument(defaultTemplateArgument_)
{
    id->SetParent(this);
    if (defaultTemplateArgument)
    {
        defaultTemplateArgument->SetParent(this);
    }
}

Node* TemplateParameterNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedDefaultTemplateArgument = nullptr;
    if (defaultTemplateArgument)
    {
        clonedDefaultTemplateArgument = defaultTemplateArgument->Clone(cloneContext);
    }
    TemplateParameterNode* clone = new TemplateParameterNode(GetSpan(), static_cast<IdentifierNode*>(id->Clone(cloneContext)), clonedDefaultTemplateArgument);
    return clone;
}

void TemplateParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateParameterNode::Write(AstWriter& writer)
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

void TemplateParameterNode::Read(AstReader& reader)
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

FullInstantiationRequestNode::FullInstantiationRequestNode(const soul::ast::Span& span_) : 
    Node(NodeType::fullInstantiationRequestNode, span_), templateId()
{
}

FullInstantiationRequestNode::FullInstantiationRequestNode(const soul::ast::Span& span_, TemplateIdNode* templateId_) :
    Node(NodeType::fullInstantiationRequestNode, span_), templateId(templateId_)
{
}

Node* FullInstantiationRequestNode::Clone(CloneContext& cloneContext) const
{
    return new FullInstantiationRequestNode(GetSpan(), static_cast<TemplateIdNode*>(templateId->Clone(cloneContext)));
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
