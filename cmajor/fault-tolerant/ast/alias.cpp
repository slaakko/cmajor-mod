// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.alias;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

UsingAliasNode::UsingAliasNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void UsingAliasNode::SetSpecifiers(SpecifiersNode* specifiersNode_)
{
    specifiersNode.reset(specifiersNode_);
    AddChildNode(specifiersNode.get());
}

void UsingAliasNode::SetUsingKeyword(UsingKeywordNode* usingKeyword_)
{
    usingKeyword.reset(usingKeyword_);
    AddChildNode(usingKeyword.get());
}

void UsingAliasNode::SetIdentifier(IdentifierNode* identifierNode_)
{
    identifierNode.reset(identifierNode_);
    AddChildNode(identifierNode.get());
}

void UsingAliasNode::SetAssign(AssignNode* assignNode_)
{
    assignNode.reset(assignNode_);
    AddChildNode(assignNode.get());
}

void UsingAliasNode::SetTypeExpr(TypeExprNode* typeExprNode_)
{
    typeExprNode.reset(typeExprNode_);
    AddChildNode(typeExprNode.get());
}

void UsingAliasNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* UsingAliasNode::Clone() const
{
    UsingAliasNode* clone = new UsingAliasNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiersNode->Clone()));
    clone->SetUsingKeyword(static_cast<UsingKeywordNode*>(usingKeyword->Clone()));
    clone->SetIdentifier(static_cast<IdentifierNode*>(identifierNode->Clone()));
    clone->SetAssign(static_cast<AssignNode*>(assignNode->Clone()));
    clone->SetTypeExpr(static_cast<TypeExprNode*>(typeExprNode->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void UsingAliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypedefAliasNode::TypedefAliasNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void TypedefAliasNode::SetSpecifiersNode(SpecifiersNode* specifiersNode_)
{
    specifiersNode.reset(specifiersNode_);
    AddChildNode(specifiersNode.get());
}

void TypedefAliasNode::SetTypedefKeyword(TypedefKeywordNode* typedefNode_)
{
    typedefNode.reset(typedefNode_);
    AddChildNode(typedefNode.get());
}

void TypedefAliasNode::SetTypeExprNode(TypeExprNode* typeExprNode_)
{
    typeExprNode.reset(typeExprNode_);
    AddChildNode(typeExprNode.get());
}

void TypedefAliasNode::SetIdentifierNode(IdentifierNode* identifierNode_)
{
    identifierNode.reset(identifierNode_);
    AddChildNode(identifierNode.get());
}

void TypedefAliasNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* TypedefAliasNode::Clone() const
{
    TypedefAliasNode* clone = new TypedefAliasNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiersNode(static_cast<SpecifiersNode*>(specifiersNode->Clone()));
    clone->SetTypedefKeyword(static_cast<TypedefKeywordNode*>(typedefNode->Clone()));
    clone->SetTypeExprNode(static_cast<TypeExprNode*>(typeExprNode->Clone()));
    clone->SetIdentifierNode(static_cast<IdentifierNode*>(identifierNode->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void TypedefAliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
