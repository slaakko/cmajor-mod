// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.namespaces;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;
import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class NsFlags : int
{
    none = 0, isUnnamedNs = 1 << 0
};

constexpr NsFlags operator|(NsFlags left, NsFlags right)
{
    return static_cast<NsFlags>(static_cast<int>(left) | static_cast<int>(right));
}

constexpr NsFlags operator&(NsFlags left, NsFlags right)
{
    return static_cast<NsFlags>(static_cast<int>(left) & static_cast<int>(right));
}

constexpr NsFlags operator~(NsFlags flags)
{
    return static_cast<NsFlags>(~int(flags));
}

class NamespaceDefinitionNode : public SyntaxNode
{
public:
    NamespaceDefinitionNode(const soul::ast::Span& span, CompletionContext completionContext);
    NamespaceDefinitionNode(NamespaceKeywordNode* namespaceNode_);
    NamespaceKeywordNode* NamespaceKeyword() const { return namespaceNode.get(); }
    NodeList<SyntaxNode>& Content() { return content; }
    void SetNamespaceName(QualifiedIdNode* namespaceName_);
    QualifiedIdNode* NamespaceName() const { return namespaceName.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddNode(SyntaxNode* node);
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    bool IsUnnamedNs() const { return (flags & NsFlags::isUnnamedNs) != NsFlags::none; }
    void SetUnnamedNs() { flags = flags | NsFlags::isUnnamedNs; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<NamespaceKeywordNode> namespaceNode;
    std::unique_ptr<QualifiedIdNode> namespaceName;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<SyntaxNode> content;
    std::unique_ptr<RBraceNode> rbrace;
    NsFlags flags;
};

class NamespaceImportNode : public SyntaxNode
{
public:
    NamespaceImportNode();
    void SetUsingKeyword(UsingKeywordNode* usingKeyword_);
    void SetQualifiedId(QualifiedIdNode* qualifiedId_);
    void SetSemicolon(SemicolonNode* semicolon_);
    UsingKeywordNode* UsingKeyword() const { return usingKeyword.get(); }
    QualifiedIdNode* QualifiedId() const { return qualifiedId.get(); }
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<UsingKeywordNode> usingKeyword;
    std::unique_ptr<QualifiedIdNode> qualifiedId;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
