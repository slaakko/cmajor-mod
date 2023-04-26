// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.typedef_;

import cmajor.ast.node;
import cmajor.ast.specifier;

export namespace cmajor::ast {

class IdentifierNode;

class TypedefNode : public Node
{
public:
    TypedefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TypedefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* TypeExpr() const { return typeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
};
} // namespace cmajor::ast
