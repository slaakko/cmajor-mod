// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.namespaces;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

enum class NsFlags : int8_t
{
    none = 0, isUnnamedNs = 1 << 0, hasUnnamedNs = 1 << 1
};

inline constexpr NsFlags operator|(NsFlags left, NsFlags right)
{
    return static_cast<NsFlags>(static_cast<int8_t>(left) | static_cast<int8_t>(right));
}

inline constexpr NsFlags operator&(NsFlags left, NsFlags right)
{
    return static_cast<NsFlags>(static_cast<int8_t>(left) & static_cast<int8_t>(right));
}

class NamespaceNode : public Node
{
public:
    NamespaceNode(const soul::ast::Span& span_);
    NamespaceNode(const soul::ast::Span& span_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddMember(Node* member);
    IdentifierNode* Id() const;
    NodeList<Node>& Members() { return members; }
    const NodeList<Node>& Members() const { return members; }
    bool IsUnnamedNs() const { return (flags & NsFlags::isUnnamedNs) != NsFlags::none; }
    void SetUnnamedNs() { flags = flags | NsFlags::isUnnamedNs; }
    bool HasUnnamedNs() const { return (flags & NsFlags::hasUnnamedNs) != NsFlags::none; }
    void SetHasUnnamedNs() { flags = flags | NsFlags::hasUnnamedNs; }
    void SetFileIndex(int32_t fileIndex_) { fileIndex = fileIndex_; }
    int32_t FileIndex() const override;
    void SetModuleId(const util::uuid& moduleId_);
    const util::uuid& ModuleId() const override;
private:
    std::unique_ptr<IdentifierNode> id;
    NodeList<Node> members;
    NsFlags flags;
    int32_t fileIndex;
    util::uuid moduleId;
};

class NamespaceImportNode : public Node
{
public:
    NamespaceImportNode(const soul::ast::Span& span_);
    NamespaceImportNode(const soul::ast::Span& span_, IdentifierNode* ns_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    IdentifierNode* Ns() const;
private:
    std::unique_ptr<IdentifierNode> ns;
};

} // namespace cmajor::ast
