// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.interface;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

InterfaceNode::InterfaceNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void InterfaceNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void InterfaceNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void InterfaceNode::SetInterfaceKeyword(InterfaceKeywordNode* interfaceKeyword_)
{
    interfaceKeyword.reset(interfaceKeyword_);
    AddChildNode(interfaceKeyword.get());
}

void InterfaceNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void InterfaceNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void InterfaceNode::AddMemberFunction(MemberFunctionNode* memberFunction)
{
    memberFunctions.AddNode(memberFunction);
}

void InterfaceNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* InterfaceNode::Clone() const
{
    InterfaceNode* clone = new InterfaceNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetInterfaceKeyword(static_cast<InterfaceKeywordNode*>(interfaceKeyword->Clone()));
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& memberFunction : memberFunctions.Nodes())
    {
        clone->AddMemberFunction(static_cast<MemberFunctionNode*>(memberFunction->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void InterfaceNode::Make()
{
    memberFunctions.Make();
    AddChildNode(&memberFunctions);
    SyntaxNode::Make();
}

void InterfaceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
