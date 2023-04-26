// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.parameter;

import cmajor.ast.node;

export namespace cmajor::ast {

class IdentifierNode;

class ParameterNode : public Node
{
public:
    ParameterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ParameterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Node* TypeExpr() const { return typeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
    void SetId(IdentifierNode* id_);
    bool ArtificialId() const { return artificialId; }
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    bool artificialId;
};
} // namespace cmajor::ast

