// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.node;

import soul.xml.error;

namespace soul::xml {

Node::Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_) : 
    kind(kind_), sourcePos(sourcePos_), name(name_), parent(nullptr), prev(nullptr), next(nullptr), ownerDocument(nullptr)
{
}

Node::~Node()
{
}

void Node::SetNamespaceUri(const std::string& namespaceUri_)
{
    namespaceUri = namespaceUri_;
}

std::string Node::Prefix() const
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (colonPos != std::string::npos)
        {
            return name.substr(0, colonPos);
        }
    }
    return std::string();
}

void Node::SetPrefix(const std::string& prefix)
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (prefix.empty())
        {
            if (colonPos != std::string::npos)
            {
                name = name.substr(colonPos + 1);
            }
        }
        else
        {
            if (colonPos != std::string::npos)
            {
                name = prefix + ":" + name.substr(colonPos + 1);
            }
            else
            {
                name = prefix + ":" + name;
            }
        }
    }
    else
    {
        throw XmlException("cannot set prefix for this node type", GetSourcePos());
    }
}

std::string Node::LocalName() const
{
    if (IsElementNode() || IsAttributeNode())
    {
        auto colonPos = name.find(':');
        if (colonPos != std::string::npos)
        {
            return name.substr(colonPos + 1);
        }
        else
        {
            return name;
        }
    }
    else
    {
        return std::string();
    }
}

void Node::LinkBefore(Node* node)
{
    if (prev)
    {
        prev->next = node;
    }
    node->prev = prev;
    node->next = this;
    prev = node;
}

void Node::LinkAfter(Node* node)
{
    if (next)
    {
        next->prev = node;
    }
    node->prev = this;
    node->next = next;
    next = node;
}

void Node::Unlink()
{
    if (prev)
    {
        prev->next = next;
    }
    if (next)
    {
        next->prev = prev;
    }
}

} // namespace soul::xml
