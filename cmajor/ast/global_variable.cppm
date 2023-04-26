// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.global.variable;

import cmajor.ast.node;
import cmajor.ast.specifier;

export namespace cmajor::ast {

class IdentifierNode;
class CompileUnitNode;

class GlobalVariableNode : public Node
{
public:
    GlobalVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    GlobalVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, CompileUnitNode* cu_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    Node* Initializer() { return initializer.get(); }
    void SetInitializer(Node* initializer_);
    CompileUnitNode* CompileUnit() const { return cu; }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> initializer;
    CompileUnitNode* cu;
};
} // namespace cmajor::ast
