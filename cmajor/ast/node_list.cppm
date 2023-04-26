
// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.node.list;

import std.core;
import cmajor.ast.node;
import cmajor.ast.writer;
import cmajor.ast.reader;

export namespace cmajor::ast {

template<typename T>
class NodeList
{
public:
    int Count() const
    {
        return static_cast<int>(nodes.size());
    }
    T* operator[](int index) const
    {
        return nodes[index].get();
    }
    void Add(T* node)
    {
        nodes.push_back(std::unique_ptr<T>(node));
    }
    T* Release(int index)
    {
        return nodes[index].release();
    }
    void Insert(int index, T* node)
    {
        if (index >= static_cast<int>(nodes.size()))
        {
            Add(node);
        }
        else
        {
            nodes.insert(nodes.begin() + index, std::unique_ptr<T>(node));
        }
    }
    void Clear()
    {
        nodes.clear();
    }
    void RemoveEmpty()
    {
        int p = 0;
        int n = static_cast<int>(nodes.size());
        for (int i = 0; i < n; ++i)
        {
            if (nodes[i])
            {
                if (p != i)
                {
                    nodes[p].reset(nodes[i].release());
                }
                ++p;
            }
        }
        if (p != n)
        {
            nodes.erase(nodes.begin() + p, nodes.end());
        }
    }
    void SetParent(Node* parent)
    {
        for (const std::unique_ptr<T>& node : nodes)
        {
            node->SetParent(parent);
        }
    }
    void Write(AstWriter& writer)
    {
        uint32_t n = static_cast<uint32_t>(nodes.size());
        writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
        for (uint32_t i = 0u; i < n; ++i)
        {
            writer.Write(nodes[i].get());
        }
    }
    void Read(AstReader& reader)
    {
        uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
        for (uint32_t i = 0u; i < n; ++i)
        {
            Node* node = reader.ReadNode();
            T* asTPtrNode = dynamic_cast<T*>(node);
            // Assert(asTPtrNode, "wrong node type"); TODO
            nodes.push_back(std::unique_ptr<T>(asTPtrNode));
        }
    }
    std::vector<std::unique_ptr<T>>& Nodes() { return nodes; }
    std::vector<std::unique_ptr<T>> Content() { return std::move(nodes); }
    void SetContent(std::vector<std::unique_ptr<T>>&& content) { nodes = std::move(content); }
private:
    std::vector<std::unique_ptr<T>> nodes;
};

} // namespace sngcm::ast

