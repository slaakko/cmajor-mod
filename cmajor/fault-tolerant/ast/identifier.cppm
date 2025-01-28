// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.identifier;

import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.node;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class IdentifierNode : public SyntaxNode
{
public:
    IdentifierNode(const soul::ast::Span& span_, CompletionContext completionContext_, const std::u16string& str_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class QualifiedIdNode : public SyntaxNode
{
public:
    QualifiedIdNode(IdentifierNode* idNode);
    const std::u16string& Str() const override { return str; }
    void AddIdentifierNode(IdentifierNode* idNode);
    void AddDotNode(DotNode* dotNode);
    const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes; }
    std::vector<std::unique_ptr<Node>>& Nodes() { return nodes; }
    const std::vector<IdentifierNode*>& IdentifierNodes() const { return identifierNodes; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<IdentifierNode*> identifierNodes;
    std::u16string str;
};

} // namespace cmajor::fault::tolerant::ast
