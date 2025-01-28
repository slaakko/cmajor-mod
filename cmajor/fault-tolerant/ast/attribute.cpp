// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.attribute;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

AttributeNode::AttributeNode(IdentifierNode* name_, Node* value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), name(name_), value(value_)
{
}

void AttributeNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

Node* AttributeNode::Clone() const
{
    AttributeNode* clone = new AttributeNode(static_cast<IdentifierNode*>(name->Clone()), value->Clone());
    if (assign)
    {
        clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    }
    clone->Make();
    return clone;
}

void AttributeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AttributeListNode::AttributeListNode() : ListNode(soul::ast::Span(), CompletionContext::none)
{
}

void AttributeListNode::AddAttribute(AttributeNode* attribute)
{
    attributes.push_back(attribute);
    AddNode(attribute);
}

AttributesNode::AttributesNode() : SyntaxNode(soul::ast::Span(), CompletionContext::none)
{
}

void AttributesNode::SetLBracket(LBracketNode* lbracket_)
{
    lbracket.reset(lbracket_);
    AddChildNode(lbracket.get());
}

void AttributesNode::AddAttribute(AttributeNode* attribute)
{
    attributeList.AddAttribute(attribute);
    attributeMap[attribute->Name()->Str()] = attribute;
}

void AttributesNode::AddComma(CommaNode* comma)
{
    attributeList.AddComma(comma);
}

AttributeNode* AttributesNode::GetAttribute(const std::u16string& name) const
{
    auto it = attributeMap.find(name);
    if (it != attributeMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void AttributesNode::SetRBracket(RBracketNode* rbracket_)
{
    rbracket.reset(rbracket_);
    AddChildNode(rbracket.get());
}

Node* AttributesNode::Clone() const
{
    AttributesNode* clone = new AttributesNode();
    clone->SetLBracket(static_cast<LBracketNode*>(lbracket->Clone()));
    clone->SetRBracket(static_cast<RBracketNode*>(rbracket->Clone()));
    for (const auto& node : attributeList.Nodes())
    {
        if (node->IsComma())
        {
            clone->AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            clone->AddAttribute(static_cast<AttributeNode*>(node->Clone()));
        }
    }
    clone->Make();
    return clone;
}

void AttributesNode::Make()
{
    attributeList.Make();
    AddChildNode(&attributeList);
    SyntaxNode::Make();
}

void AttributesNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
