// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.node.list;

import cmajor.fault.tolerant.ast.node;
import std.core;

export namespace cmajor::fault::tolerant::ast {

template<typename T>
class NodeList : public Node
{
public:
    NodeList() : Node(NodeKind::nodeList, soul::ast::Span(), CompletionContext::none) {}
    void AddNode(T* node)
    {
        nodes.push_back(std::unique_ptr<T>(node));
        AddChildNode(node);
    }
    T* GetNode(int index) const
    {
        return nodes[index].get();
    }
    std::vector<std::unique_ptr<T>>& Nodes() 
    { 
        return nodes; 
    }
    const std::vector<std::unique_ptr<T>>& Nodes() const
    {
        return nodes;
    }
    void Accept(Visitor& visitor) override
    {
        for (const auto& node : nodes)
        {
            node->Accept(visitor);
        }
    }
private:
    std::vector<std::unique_ptr<T>> nodes;
};

} // namespace cmajor::fault::tolerant::ast
