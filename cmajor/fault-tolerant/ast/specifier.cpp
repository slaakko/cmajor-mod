// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.specifier;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

SpecifierNode::SpecifierNode(KeywordNode* keyword_, Specifiers specifier_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), keyword(keyword_), specifier(specifier_)
{
    AddChildNode(keyword.get());
}

Node* SpecifierNode::Clone() const
{
    return new SpecifierNode(static_cast<KeywordNode*>(keyword->Clone()), specifier);
}

void SpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SpecifiersNode::SpecifiersNode(const soul::ast::Span& span_, Specifiers specifiers_) : SyntaxNode(span_, CompletionContext::none), specifiers(specifiers_)
{
}

void SpecifiersNode::AddSpecifierNode(SpecifierNode* specifierNode)
{
    AddChildNode(specifierNode);
    specifierNodes.push_back(std::unique_ptr<SpecifierNode>(specifierNode));
}

Node* SpecifiersNode::Clone() const
{
    SpecifiersNode* clone = new SpecifiersNode(GetSpan(), specifiers);
    for (const auto& specifierNode : specifierNodes)
    {
        clone->AddSpecifierNode(static_cast<SpecifierNode*>(specifierNode->Clone()));
    }
    clone->Make();
    return clone;
}

void SpecifiersNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
