// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.function;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

FunctionNode::FunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), templateParameterList(), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void FunctionNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void FunctionNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void FunctionNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void FunctionNode::SetFunctionGroupId(Node* functionGroupId_)
{
    functionGroupId.reset(functionGroupId_);
    AddChildNode(functionGroupId.get());
}

void FunctionNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void FunctionNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void FunctionNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* FunctionNode::Clone() const
{
    FunctionNode* clone = new FunctionNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetReturnType(returnType->Clone());
    clone->SetFunctionGroupId(functionGroupId->Clone());
    clone->templateParameterList.CloneFrom(templateParameterList);
    clone->parameterList.CloneFrom(parameterList);
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void FunctionNode::Make()
{
    templateParameterList.Make();
    AddChildNode(&templateParameterList);
    parameterList.Make();
    AddChildNode(&parameterList);
    SyntaxNode::Make();
}

void FunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OperatorFunctionGroupIdNode::OperatorFunctionGroupIdNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void OperatorFunctionGroupIdNode::SetOperatorKeyword(OperatorKeywordNode* operatorKeyword_)
{
    operatorKeyword.reset(operatorKeyword_);
    AddChildNode(operatorKeyword.get());
}

void OperatorFunctionGroupIdNode::AddOperatorNode(Node* operatorNode)
{
    operatorNodes.AddNode(operatorNode);
    AddChildNode(operatorNode);
}

Node* OperatorFunctionGroupIdNode::Clone() const
{
    OperatorFunctionGroupIdNode* clone = new OperatorFunctionGroupIdNode(GetSpan(), GetCompletionContext());
    clone->SetOperatorKeyword(static_cast<OperatorKeywordNode*>(operatorKeyword->Clone()));
    for (const auto& node : operatorNodes.Nodes())
    {
        clone->AddOperatorNode(node->Clone());
    }
    clone->Make();
    return clone;
}

void OperatorFunctionGroupIdNode::Make()
{
    operatorNodes.Make();
    AddChildNode(&operatorNodes);
    str = operatorKeyword->Str();
    for (const auto& op : operatorNodes.Nodes())
    {
        str.append(op->Str());
    }
    SyntaxNode::Make();
}

void OperatorFunctionGroupIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
