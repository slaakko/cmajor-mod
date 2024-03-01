// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.constant;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

ConstantNode::ConstantNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ConstantNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void ConstantNode::SetConstKeyword(ConstKeywordNode* constKeyword_)
{
    constKeyword.reset(constKeyword_);
    AddChildNode(constKeyword.get());
}

void ConstantNode::SetType(Node* type_)
{
    type.reset(type_);
    AddChildNode(type.get());
}

void ConstantNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void ConstantNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void ConstantNode::SetExpr(Node* expr_)
{
    expr.reset(expr_);
    AddChildNode(expr.get());
}

void ConstantNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ConstantNode::Clone() const
{
    ConstantNode* clone = new ConstantNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetConstKeyword(static_cast<ConstKeywordNode*>(constKeyword->Clone()));
    clone->SetType(type->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    clone->SetExpr(expr->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
