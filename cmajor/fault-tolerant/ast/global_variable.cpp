// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.global.variable;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

GlobalVariableNode::GlobalVariableNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void GlobalVariableNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void GlobalVariableNode::SetType(Node* type_)
{
    type.reset(type_);
    AddChildNode(type.get());
}

void GlobalVariableNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void GlobalVariableNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void GlobalVariableNode::SetInitializer(Node* initializer_)
{
    initializer.reset(initializer_);
    AddChildNode(initializer.get());
}

void GlobalVariableNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* GlobalVariableNode::Clone() const
{
    GlobalVariableNode* clone = new GlobalVariableNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetType(type->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    if (assign)
    {
        clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    }
    if (initializer)
    {
        clone->SetInitializer(initializer->Clone());
    }
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void GlobalVariableNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
