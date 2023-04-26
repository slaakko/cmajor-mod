// =================================
// Copyright (c) 2023 Seppo Laakko
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
    InterfaceNode(const soul::ast::SourcePos& span_, const util::uuid& moduleId_);
    InterfaceNode(const soul::ast::SourcePos& span_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    IdentifierNode* Id() const { return id.get(); }
    const NodeList<Node>& Members() const { return members; }
    void AddMember(Node* member);
    AttributesNode* GetAttributes() const { return attributes.get(); }
    void SetSpecifierSourcePos(const soul::ast::SourcePos& specifierSourcePos_) { specifierSourcePos = specifierSourcePos_; }
    const soul::ast::SourcePos& SpecifierSourcePos() const { return specifierSourcePos; }
    void SetBeginBraceSourcePos(const soul::ast::SourcePos& beginBraceSourcePos_) { beginBraceSourcePos = beginBraceSourcePos_; }
    const soul::ast::SourcePos& BeginBraceSourcePos() const { return beginBraceSourcePos; }
    void SetEndBraceSourcePos(const soul::ast::SourcePos& endBraceSourcePos_) { endBraceSourcePos = endBraceSourcePos_; }
    const soul::ast::SourcePos& EndBraceSourcePos() const { return endBraceSourcePos; }
private:
    Specifiers specifiers;
    soul::ast::SourcePos specifierSourcePos;
    soul::ast::SourcePos beginBraceSourcePos;
    soul::ast::SourcePos endBraceSourcePos;
    std::unique_ptr<IdentifierNode> id;
    NodeList<Node> members;
    std::unique_ptr<AttributesNode> attributes;
};
} // namespace cmajor::ast
