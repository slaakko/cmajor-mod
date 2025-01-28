// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.interface;

import cmajor.ast.node;
import cmajor.ast.node.list;
import cmajor.ast.function;
import cmajor.ast.specifier;

export namespace cmajor::ast {

class AttributesNode;
class IdentifierNode;

class InterfaceNode : public Node
{
public:
    InterfaceNode(const soul::ast::Span& span_);
    InterfaceNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    IdentifierNode* Id() const { return id.get(); }
    const NodeList<Node>& Members() const { return members; }
    void AddMember(Node* member);
    AttributesNode* GetAttributes() const { return attributes.get(); }
    const soul::ast::Span& SpecifierSpan() const { return specifierSpan; }
    void SetSpecifierSpan(const soul::ast::Span& specifierSpan_) { specifierSpan = specifierSpan_; }
    const soul::ast::Span& BeginBraceSpan() const { return beginBraceSpan; }
    void SetBeginBraceSpan(const soul::ast::Span& beginBraceSpan_) { beginBraceSpan = beginBraceSpan_; }
    const soul::ast::Span& EndBraceSpan() const { return endBraceSpan; }
    void SetEndBraceSpan(const soul::ast::Span& endBraceSpan_) { endBraceSpan = endBraceSpan_; }
private:
    Specifiers specifiers;
    std::unique_ptr<IdentifierNode> id;
    NodeList<Node> members;
    std::unique_ptr<AttributesNode> attributes;
    soul::ast::Span specifierSpan;
    soul::ast::Span beginBraceSpan;
    soul::ast::Span endBraceSpan;
};
} // namespace cmajor::ast
