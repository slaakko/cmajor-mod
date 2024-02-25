// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.templates;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

TemplateIdNode::TemplateIdNode(const soul::ast::Span& span_, CompletionContext completionContext_, QualifiedIdNode* primary_) : 
    SyntaxNode(span_, completionContext_), primary(primary_), templateArgumentList(soul::ast::Span(), CompletionContext::none)
{
    AddChildNode(primary.get());
}

void TemplateIdNode::SetLAngle(LAngleNode* langle_)
{
    langle.reset(langle_);
    AddChildNode(langle.get());
}

void TemplateIdNode::AddTemplateArgument(TypeExprNode* templateArgument)
{
    templateArgumentList.AddNode(templateArgument);
    AddChildNode(templateArgument);
}

void TemplateIdNode::AddComma(CommaNode* comma)
{
    templateArgumentList.AddComma(comma);
}

void TemplateIdNode::SetRAngle(RAngleNode* rangle_)
{
    rangle.reset(rangle_);
    AddChildNode(rangle.get());
}

Node* TemplateIdNode::Clone() const
{
    TemplateIdNode* clone = new TemplateIdNode(GetSpan(), GetCompletionContext(), static_cast<QualifiedIdNode*>(primary->Clone()));
    clone->SetLAngle(static_cast<LAngleNode*>(langle->Clone()));
    for (const auto& node : templateArgumentList.Nodes())
    {
        if (node->IsComma())
        {
            clone->AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            clone->AddTemplateArgument(static_cast<TypeExprNode*>(node->Clone()));
        }
    }
    clone->SetRAngle(static_cast<RAngleNode*>(rangle->Clone()));
    clone->Make();
    return clone;
}

void TemplateIdNode::Make()
{
    templateArgumentList.Make();
    AddChildNode(&templateArgumentList);
    SyntaxNode::Make();
}

void TemplateIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TemplateParameterNode::TemplateParameterNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void TemplateParameterNode::SetIdentifier(IdentifierNode* identifier_)
{
    identifier.reset(identifier_);
    AddChildNode(identifier.get());
}

void TemplateParameterNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void TemplateParameterNode::SetDefaultValue(Node* defaultValue_)
{
    defaultValue.reset(defaultValue_);
    AddChildNode(defaultValue.get());
}

Node* TemplateParameterNode::Clone() const
{
    TemplateParameterNode* clone = new TemplateParameterNode(GetSpan(), GetCompletionContext());
    clone->SetIdentifier(static_cast<IdentifierNode*>(identifier->Clone()));
    if (assign)
    {
        clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    }
    if (defaultValue)
    {
        clone->SetDefaultValue(defaultValue->Clone());
    }
    clone->Make();
    return clone;
}

void TemplateParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TemplateParameterListNode::TemplateParameterListNode() : ListNode(soul::ast::Span(), CompletionContext::none)
{
}

void TemplateParameterListNode::SetLAngle(LAngleNode* langle_)
{
    langle.reset(langle_);
    AddChildNode(langle.get());
}

void TemplateParameterListNode::AddTemplateParameter(TemplateParameterNode* templateParameter)
{
    AddNode(templateParameter);
    templateParameters.push_back(templateParameter);
}

void TemplateParameterListNode::SetRAngle(RAngleNode* rangle_)
{
    rangle.reset(rangle_);
    AddChildNode(rangle.get());
}

void TemplateParameterListNode::CloneFrom(const TemplateParameterListNode& that)
{
    SetLAngle(static_cast<LAngleNode*>(that.langle->Clone()));
    SetRAngle(static_cast<RAngleNode*>(that.rangle->Clone()));
    for (const auto& node : that.Nodes())
    {
        if (node->IsComma())
        {
            AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            AddTemplateParameter(static_cast<TemplateParameterNode*>(node->Clone()));
        }
    }
}

void TemplateParameterListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FullInstantiationRequest::FullInstantiationRequest(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void FullInstantiationRequest::SetNewKeyword(NewKeywordNode* newKeyword_)
{
    newKeyword.reset(newKeyword_);
    AddChildNode(newKeyword.get());
}

void FullInstantiationRequest::SetClassKeyword(ClassKeywordNode* classKeyword_)
{
    classKeyword.reset(classKeyword_);
    AddChildNode(classKeyword.get());
}

void FullInstantiationRequest::SetTemplateId(TemplateIdNode* templateId_)
{
    templateId.reset(templateId_);
    AddChildNode(templateId.get());
}

void FullInstantiationRequest::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* FullInstantiationRequest::Clone() const
{
    FullInstantiationRequest* clone = new FullInstantiationRequest(GetSpan(), GetCompletionContext());
    clone->SetNewKeyword(static_cast<NewKeywordNode*>(newKeyword->Clone()));
    clone->SetClassKeyword(static_cast<ClassKeywordNode*>(classKeyword->Clone()));
    clone->SetTemplateId(static_cast<TemplateIdNode*>(templateId->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void FullInstantiationRequest::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
