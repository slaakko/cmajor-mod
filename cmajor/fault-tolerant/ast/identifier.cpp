// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.identifier;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, CompletionContext completionContext_, const std::u16string& str_) : 
    SyntaxNode(span_, completionContext_), str(str_)
{
}

Node* IdentifierNode::Clone() const
{
    IdentifierNode* clone = new IdentifierNode(GetSpan(), GetCompletionContext(), str);
    clone->Make();
    return clone;
}

void IdentifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

QualifiedIdNode::QualifiedIdNode(IdentifierNode* idNode) : SyntaxNode(soul::ast::Span(), CompletionContext::none)
{
    AddIdentifierNode(idNode);
}

void QualifiedIdNode::AddIdentifierNode(IdentifierNode* idNode)
{
    AddChildNode(idNode);
    nodes.push_back(std::unique_ptr<Node>(idNode));
    identifierNodes.push_back(idNode);
}

void QualifiedIdNode::AddDotNode(DotNode* dotNode)
{
    AddChildNode(dotNode);
    nodes.push_back(std::unique_ptr<Node>(dotNode));
}

Node* QualifiedIdNode::Clone() const
{
    QualifiedIdNode* clone = new QualifiedIdNode(static_cast<IdentifierNode*>(nodes.front()->Clone()));
    int n = nodes.size();
    for (int i = 1; i < n; ++i)
    {
        Node* node = nodes[i].get();
        if (node->IsDot())
        {
            clone->AddDotNode(static_cast<DotNode*>(node->Clone()));
        }
        else
        {
            clone->AddIdentifierNode(static_cast<IdentifierNode*>(node->Clone()));
        }
    }
    clone->Make();
    return clone;
}

void QualifiedIdNode::Make()
{
    SyntaxNode::Make();
    bool first = true;
    for (const auto& identifierNode : identifierNodes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, '.');
        }
        str.append(identifierNode->Str());
    }
}

void QualifiedIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
