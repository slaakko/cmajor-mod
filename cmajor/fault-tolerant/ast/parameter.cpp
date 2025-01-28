// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.parameter;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

ParameterNode::ParameterNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ParameterNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void ParameterNode::SetName(IdentifierNode* name_)
{
    name.reset(name_);
    AddChildNode(name.get());
}

Node* ParameterNode::Clone() const
{
    ParameterNode* clone = new ParameterNode(GetSpan(), GetCompletionContext());
    clone->SetTypeExpr(typeExpr->Clone());
    if (name)
    {
        clone->SetName(static_cast<IdentifierNode*>(name->Clone()));
    }
    clone->Make();
    return clone;
}

void ParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParameterListNode::ParameterListNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ListNode(span_, completionContext_)
{
}

void ParameterListNode::AddParameter(ParameterNode* parameter)
{
    AddNode(parameter);
    parameters.push_back(parameter);
}

ParameterNode* ParameterListNode::GetParameter(int index) const
{
    if (index >= 0 && index < parameters.size())
    {
        return parameters[index];
    }
    else
    {
        throw std::runtime_error("invalid parameter index");
    }
}

void ParameterListNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ParameterListNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void ParameterListNode::CloneFrom(const ParameterListNode& that)
{
    SetLParen(static_cast<LParenNode*>(that.lparen->Clone()));
    for (const auto& node : that.Nodes())
    {
        if (node->IsComma())
        {
            AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            AddParameter(static_cast<ParameterNode*>(node->Clone()));
        }
    }
    SetRParen(static_cast<RParenNode*>(that.rparen->Clone()));
}

void ParameterListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
