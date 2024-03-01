// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.namespaces;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

NamespaceDefinitionNode::NamespaceDefinitionNode(const soul::ast::Span& span, CompletionContext completionContext) : 
    SyntaxNode(span, completionContext)
{
}

NamespaceDefinitionNode::NamespaceDefinitionNode(NamespaceKeywordNode* namespaceNode_) : 
    SyntaxNode(soul::ast::Span(), CompletionContext::none), namespaceNode(namespaceNode_), flags(NsFlags::none)
{
    AddChildNode(namespaceNode.get());
}

void NamespaceDefinitionNode::SetNamespaceName(QualifiedIdNode* namespaceName_)
{
    namespaceName.reset(namespaceName_);
    AddChildNode(namespaceName.get());
}

void NamespaceDefinitionNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void NamespaceDefinitionNode::AddNode(SyntaxNode* node)
{
    content.AddNode(node);
    AddChildNode(node);
}

void NamespaceDefinitionNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* NamespaceDefinitionNode::Clone() const
{
    NamespaceKeywordNode* clonedNamespaceKeyword = nullptr;
    if (namespaceNode)
    {
        clonedNamespaceKeyword = static_cast<NamespaceKeywordNode*>(namespaceNode->Clone());
    }
    NamespaceDefinitionNode* clone = new NamespaceDefinitionNode(static_cast<NamespaceKeywordNode*>(clonedNamespaceKeyword));
    if (namespaceName)
    {
        clone->SetNamespaceName(static_cast<QualifiedIdNode*>(namespaceName->Clone()));
    }
    if (IsUnnamedNs())
    {
        clone->SetUnnamedNs();
    }
    if (lbrace)
    {
        clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    }
    for (const auto& node : content.Nodes())
    {
        clone->AddNode(static_cast<SyntaxNode*>(node->Clone()));
    }
    if (rbrace)
    {
        clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    }
    clone->Make();
    return clone;
}

void NamespaceDefinitionNode::Make()
{
    content.Make();
    AddChildNode(&content);
    SyntaxNode::Make();
}

void NamespaceDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NamespaceImportNode::NamespaceImportNode() : SyntaxNode(soul::ast::Span(), CompletionContext::none)
{
}

void NamespaceImportNode::SetUsingKeyword(UsingKeywordNode* usingKeyword_)
{
    usingKeyword.reset(usingKeyword_);
    AddChildNode(usingKeyword.get());
}

void NamespaceImportNode::SetQualifiedId(QualifiedIdNode* qualifiedId_)
{
    qualifiedId.reset(qualifiedId_);
    AddChildNode(qualifiedId.get());
}

void NamespaceImportNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* NamespaceImportNode::Clone() const
{
    NamespaceImportNode* clone = new NamespaceImportNode();
    if (usingKeyword)
    {
        clone->SetUsingKeyword(static_cast<UsingKeywordNode*>(usingKeyword->Clone()));
    }
    clone->SetQualifiedId(static_cast<QualifiedIdNode*>(qualifiedId->Clone()));
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void NamespaceImportNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
