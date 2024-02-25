// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.node;

import cmajor.fault.tolerant.ast.punctuation;

namespace cmajor::fault::tolerant::ast {

Node::Node(NodeKind nodeKind_, const soul::ast::Span& span_, CompletionContext completionContext_) : 
    nodeKind(nodeKind_), index(-1), span(span_), completionContext(completionContext_), parent(nullptr)
{
}

Node::~Node()
{
}

void Node::AddChildNode(Node* childNode)
{
    childNode->SetParent(this);
    childNode->SetIndex(childNodes.size());
    childNodes.push_back(childNode);
}

Node* Node::GetChildNode(int index) const
{
    if (index >= 0 && index < childNodes.size())
    {
        return childNodes[index];
    }
    return nullptr;
}

struct NodeLess
{
    bool operator()(const Node* left, const Node* right) const
    {
        if (left->GetSpan().IsValid() && right->GetSpan().IsValid())
        {
            return left->GetSpan().pos < right->GetSpan().pos;
        }
        else if (left->GetSpan().IsValid())
        {
            return true;
        }
        else if (right->GetSpan().IsValid())
        {
            return false;
        }
        else
        {
            return left->Index() < right->Index();
        }
    }
};

void Node::Make()
{
    std::sort(childNodes.begin(), childNodes.end(), NodeLess());
    if (!GetSpan().IsValid() && !childNodes.empty())
    {
        Node* firstChild = nullptr;
        int n = childNodes.size();
        for (int i = 0; i < n; ++i)
        {
            firstChild = childNodes[i];
            if (firstChild->GetSpan().IsValid())
            {
                span = firstChild->GetSpan();
                break;
            }
        }
        for (int i = n - 1; i >= 0; --i)
        {
            Node* lastChild = childNodes[i];
            if (lastChild != firstChild && lastChild->GetSpan().IsValid())
            {
                span.len = (lastChild->GetSpan().pos + lastChild->GetSpan().len) - span.pos;
                break;
            }
        }
    }
    int index = 0;
    for (auto node : childNodes)
    {
        node->SetIndex(index++);
    }
}

Node* Node::FindNode(int pos) const
{
    if (span.Contains(pos))
    {
        if (!childNodes.empty())
        {
            Node n(NodeKind::syntaxNode, soul::ast::Span(pos, 0), CompletionContext::none);
            auto it = std::lower_bound(childNodes.begin(), childNodes.end(), &n, NodeLess());
            if (it != childNodes.end())
            {
                Node* node = *it;
                while (it != childNodes.end() && !node->GetSpan().Contains(pos))
                {
                    ++it;
                    node = *it;
                }
                if (node->GetSpan().Contains(pos))
                {
                    return node->FindNode(pos);
                }
            }
        }
        return const_cast<Node*>(this);
    }
    else
    {
        return nullptr;
    }
}

void Node::AddNode(Node* node)
{
    throw std::runtime_error("Node::AddNode not overridden");
}

void Node::AddComma(CommaNode* comma)
{
    throw std::runtime_error("Node::AddComma not overridden");
}

void Node::Accept(Visitor& visitor)
{
    throw std::runtime_error("Node::Accept not overridden");
}

const std::u16string& Node::Str() const
{
    static std::u16string empty;
    return empty;
}

KeywordNode::KeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : Node(NodeKind::keywordNode, span_, completionContext_)
{
}

TokenNode::TokenNode(const soul::ast::Span& span_, CompletionContext completionContext_) : Node(NodeKind::tokenNode, span_, completionContext_)
{
}

SyntaxNode::SyntaxNode(const soul::ast::Span& span_, CompletionContext completionContext_) : Node(NodeKind::syntaxNode, span_, completionContext_)
{
}

void SyntaxNode::AddChildNode(Node* childNode)
{
    Node::AddChildNode(childNode);
    if (childNode->IsSyntaxNode())
    {
        childSyntaxNodes.push_back(static_cast<SyntaxNode*>(childNode));
    }
}

UnaryNode::UnaryNode(Node* operand_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), operand(operand_)
{
    AddChildNode(operand.get());
}

BinaryNode::BinaryNode(Node* left_, Node* right_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), left(left_), right(right_)
{
    AddChildNode(left.get());
    AddChildNode(right.get());
}

ListNode::ListNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ListNode::AddNode(Node* node)
{
    nodes.push_back(std::unique_ptr<Node>(node));
    AddChildNode(node);
    items.push_back(node);
}

void ListNode::AddComma(CommaNode* comma)
{
    nodes.push_back(std::unique_ptr<Node>(comma));
    AddChildNode(comma);
}

Node* ListNode::GetItem(int index) const
{
    if (index >= 0 && index < items.size())
    {
        return items[index];
    }
    else
    {
        throw std::runtime_error("invalid list node index");
    }
}

void ListNode::CloneFrom(const ListNode& that)
{
    for (const auto& node : that.nodes)
    {
        if (node->IsComma())
        {
            AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            AddNode(node->Clone());
        }
    }
    Make();
}

void ListNode::Accept(Visitor& visitor)
{
    for (const auto& node : nodes)
    {
        node->Accept(visitor);
    }
}

} // namespace cmajor::fault::tolerant::ast
