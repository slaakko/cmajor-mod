// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.delegate;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

DelegateNode::DelegateNode(const soul::ast::Span& span_, CompletionContext completionContext_) :
    SyntaxNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void DelegateNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void DelegateNode::SetDelegateKeyword(DelegateKeywordNode* delegateKeyword_)
{
    delegateKeyword.reset(delegateKeyword_);
    AddChildNode(delegateKeyword.get());
}

void DelegateNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void DelegateNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void DelegateNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DelegateNode::Clone() const
{
    DelegateNode* clone = new DelegateNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetDelegateKeyword(static_cast<DelegateKeywordNode*>(delegateKeyword->Clone()));
    clone->SetReturnType(returnType->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void DelegateNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    SyntaxNode::Make();
}

void DelegateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ClassDelegateNode::ClassDelegateNode(const soul::ast::Span& span_, CompletionContext completionContext_) :
    SyntaxNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void ClassDelegateNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void ClassDelegateNode::SetClassKeyword(ClassKeywordNode* classKeyword_)
{
    classKeyword.reset(classKeyword_);
    AddChildNode(classKeyword.get());
}

void ClassDelegateNode::SetDelegateKeyword(DelegateKeywordNode* delegateKeyword_)
{
    delegateKeyword.reset(delegateKeyword_);
    AddChildNode(delegateKeyword.get());
}

void ClassDelegateNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void ClassDelegateNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void ClassDelegateNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ClassDelegateNode::Clone() const
{
    ClassDelegateNode* clone = new ClassDelegateNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetClassKeyword(static_cast<ClassKeywordNode*>(classKeyword->Clone()));
    clone->SetDelegateKeyword(static_cast<DelegateKeywordNode*>(delegateKeyword->Clone()));
    clone->SetReturnType(returnType->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ClassDelegateNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    SyntaxNode::Make();
}

void ClassDelegateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
