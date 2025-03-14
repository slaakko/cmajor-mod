// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.constant;

import cmajor.ast.node;
import cmajor.ast.specifier;

export namespace cmajor::ast {

class IdentifierNode;

class ConstantNode : public Node
{
public:
    ConstantNode(const soul::ast::Span& span_);
    ConstantNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, Node* value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const Node* Value() const { return value.get(); }
    Node* Value() { return value.get(); }
    void SetStrValue(const std::string& strValue_) { strValue = strValue_; }
    const std::string& StrValue() const { return strValue; }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> value;
    std::string strValue;
};

} // namespace cmajor::ast

