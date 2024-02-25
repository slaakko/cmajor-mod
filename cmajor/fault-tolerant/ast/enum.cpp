// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.enumeration;

import cmajor.fault.tolerant.ast.literal;
import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

EnumConstantNode::EnumConstantNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void EnumConstantNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void EnumConstantNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void EnumConstantNode::SetValue(Node* value_)
{
    value.reset(value_);
    AddChildNode(value.get());
}

Node* EnumConstantNode::Clone() const
{
    EnumConstantNode* clone = new EnumConstantNode(GetSpan(), GetCompletionContext());
    IdentifierNode* idClone = static_cast<IdentifierNode*>(id->Clone());
    clone->SetId(idClone);
    if (assign)
    {
        AssignNode* assignClone = static_cast<AssignNode*>(assign->Clone());
        clone->SetAssign(assignClone);
        Node* valueClone = value->Clone();
        clone->SetValue(valueClone);
    }
    clone->Make();
    return clone;
}

void EnumConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EnumConstantListNode::EnumConstantListNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ListNode(span_, completionContext_)
{
}

void EnumConstantListNode::AddEnumConstant(EnumConstantNode* enumConstant)
{
    enumConstants.push_back(enumConstant);
    AddNode(enumConstant);
}

void EnumConstantListNode::CloneFrom(const EnumConstantListNode& that)
{
    for (const auto& node : that.Nodes())
    {
        if (node->IsComma())
        {
            AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            AddEnumConstant(static_cast<EnumConstantNode*>(node->Clone()));
        }
    }
}

EnumTypeNode::EnumTypeNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), enumConstantList(soul::ast::Span(), CompletionContext::none)
{
}

void EnumTypeNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void EnumTypeNode::SetEnumKeyword(EnumKeywordNode* enumKeyword_)
{
    enumKeyword.reset(enumKeyword_);
    AddChildNode(enumKeyword.get());
}

void EnumTypeNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void EnumTypeNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void EnumTypeNode::SetUnderlyingType(Node* underlyingType_)
{
    underlyingType.reset(underlyingType_);
    AddChildNode(underlyingType.get());
}

Node* EnumTypeNode::UnderlyingType() const
{
    return underlyingType.get();
}

void EnumTypeNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void EnumTypeNode::AddEnumConstant(EnumConstantNode* enumConstant)
{
    enumConstantList.AddEnumConstant(enumConstant);
}

void EnumTypeNode::AddComma(CommaNode* comma)
{
    enumConstantList.AddComma(comma);
}

void EnumTypeNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* EnumTypeNode::Clone() const
{
    EnumTypeNode* clone = new EnumTypeNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetEnumKeyword(static_cast<EnumKeywordNode*>(enumKeyword->Clone()));
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    if (colon)
    {
        clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    }
    if (underlyingType)
    {
        clone->SetUnderlyingType(underlyingType->Clone());
    }
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    clone->enumConstantList.CloneFrom(enumConstantList);
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void EnumTypeNode::Make()
{
    enumConstantList.Make();
    AddChildNode(&enumConstantList);
    SyntaxNode::Make();
}

void EnumTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* NextEnumConstantValue(EnumTypeNode* enumType)
{
    EnumConstantListNode* enumConstantList = enumType->EnumConstantList();
    if (enumConstantList->EnumConstants().empty())
    {
        if (enumType->UnderlyingType()->IsUnsignedTypeNode())
        {
            return new ByteLiteralNode(new LiteralTokenNode(soul::ast::Span(), CompletionContext::none, u"0"), 0u);
        }
        else
        {
            return new SByteLiteralNode(new LiteralTokenNode(soul::ast::Span(), CompletionContext::none, u"0"), 0);
        }
    }
    else
    {
        if (enumType->UnderlyingType()->IsUnsignedTypeNode())
        {
            return new BinaryExprNode(enumConstantList->EnumConstants()[enumConstantList->Count() - 1]->Clone(), 
                new ByteLiteralNode(new LiteralTokenNode(soul::ast::Span(), CompletionContext::none, u"1"), 1u), 
                new PlusNode(soul::ast::Span(), CompletionContext::none));
        }
        else
        {
            return new BinaryExprNode(enumConstantList->EnumConstants()[enumConstantList->Count() - 1]->Clone(),
                new SByteLiteralNode(new LiteralTokenNode(soul::ast::Span(), CompletionContext::none, u"1"), 1u),
                new PlusNode(soul::ast::Span(), CompletionContext::none));
        }
    }
}

} // namespace cmajor::fault::tolerant::ast
