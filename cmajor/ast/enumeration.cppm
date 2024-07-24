// =================================
// Copyright (c) 2024 Seppo Laakko
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
    EnumTypeNode(const soul::ast::Span& span_);
    EnumTypeNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_);
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
    const soul::ast::Span& BeginBraceSpan() const { return beginBraceSpan; }
    void SetBeginBraceSpan(const soul::ast::Span& beginBraceSpan_) { beginBraceSpan = beginBraceSpan_; }
    const soul::ast::Span& EndBraceSpan() const { return endBraceSpan; }
    void SetEndBraceSpan(const soul::ast::Span& endBraceSpan_) { endBraceSpan = endBraceSpan_; }
private:
    Specifiers specifiers;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> underlyingType;
    NodeList<EnumConstantNode> constants;
    soul::ast::Span beginBraceSpan;
    soul::ast::Span endBraceSpan;
};

class EnumConstantNode : public Node
{
public:
    EnumConstantNode(const soul::ast::Span& span_);
    EnumConstantNode(const soul::ast::Span& span_, IdentifierNode* id_, Node* value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Node* GetValue() const { return value.get(); }
    IdentifierNode* Id() const { return id.get(); }
    void SetHasValue() { hasValue = true; }
    bool HasValue() const { return hasValue; }
    void SetStrValue(const std::string& strValue_) { strValue = strValue_; }
    const std::string& StrValue() const { return strValue; }
private:
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> value;
    bool hasValue;
    std::string strValue;
};

Node* MakeNextEnumConstantValue(const soul::ast::Span& span, EnumTypeNode* enumType);

} // namespace cmajor::ast
