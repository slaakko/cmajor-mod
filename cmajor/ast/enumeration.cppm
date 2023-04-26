// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.enumeration;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class IdentifierNode;
class EnumConstantNode;

class EnumTypeNode : public Node
{
public:
    EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddConstant(EnumConstantNode* constant);
    EnumConstantNode* GetLastConstant() const;
    Specifiers GetSpecifiers() const override { return specifiers; }
    IdentifierNode* Id() const { return id.get(); }
    void SetUnderlyingType(Node* underlyingType_);
    Node* GetUnderlyingType() const { return underlyingType.get(); }
    const NodeList<EnumConstantNode>& Constants() const { return constants; }
    void SetBeginBraceSourcePos(const soul::ast::SourcePos& beginBraceSourcePos_) { beginBraceSourcePos = beginBraceSourcePos_; }
    const soul::ast::SourcePos& BeginBraceSourcePos() const { return beginBraceSourcePos; }
    void SetEndBraceSourcePos(const soul::ast::SourcePos& endBraceSourcePos_) { endBraceSourcePos = endBraceSourcePos_; }
    const soul::ast::SourcePos& EndBraceSourcePos() const { return endBraceSourcePos; }
private:
    Specifiers specifiers;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> underlyingType;
    NodeList<EnumConstantNode> constants;
    soul::ast::SourcePos beginBraceSourcePos;
    soul::ast::SourcePos endBraceSourcePos;
};

class EnumConstantNode : public Node
{
public:
    EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* id_, Node* value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Node* GetValue() const { return value.get(); }
    IdentifierNode* Id() const { return id.get(); }
    void SetHasValue() { hasValue = true; }
    bool HasValue() const { return hasValue; }
    void SetStrValue(const std::u32string& strValue_) { strValue = strValue_; }
    const std::u32string& StrValue() const { return strValue; }
private:
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> value;
    bool hasValue;
    std::u32string strValue;
};

Node* MakeNextEnumConstantValue(const soul::ast::SourcePos& span, const util::uuid& moduleId_, EnumTypeNode* enumType);

} // namespace cmajor::ast
