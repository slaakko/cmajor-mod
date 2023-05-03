// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.interface;

import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.attribute;

namespace cmajor::ast {

InterfaceNode::InterfaceNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    Node(NodeType::interfaceNode, sourcePos_, moduleId_), specifiers(), id(), members()
{
}

InterfaceNode::InterfaceNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_, AttributesNode* attributes_) :
    Node(NodeType::interfaceNode, sourcePos_, moduleId_), specifiers(specifiers_), id(id_), members(), attributes(attributes_)
{
    id->SetParent(this);
}

Node* InterfaceNode::Clone(CloneContext& cloneContext) const
{
    AttributesNode* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = static_cast<AttributesNode*>(attributes->Clone(cloneContext));
    }
    InterfaceNode* clone = new InterfaceNode(GetSourcePos(), ModuleId(), specifiers, static_cast<IdentifierNode*>(id->Clone(cloneContext)), clonedAttributes);
    int n = members.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddMember(members[i]->Clone(cloneContext));
    }
    return clone;
}

void InterfaceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InterfaceNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    bool hasAttributes = attributes != nullptr;
    writer.GetBinaryStreamWriter().Write(hasAttributes);
    if (hasAttributes)
    {
        writer.Write(attributes.get());
    }
    writer.Write(id.get());
    members.Write(writer);
}

void InterfaceNode::Read(AstReader& reader)
{
    Node::Read(reader);
    bool hasAttributes = reader.GetBinaryStreamReader().ReadBool();
    if (hasAttributes)
    {
        attributes.reset(reader.ReadAttributesNode());
    }
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    members.Read(reader);
    members.SetParent(this);
}

void InterfaceNode::AddMember(Node* member)
{
    member->SetParent(this);
    members.Add(member);
}

} // namespace cmajor::ast
