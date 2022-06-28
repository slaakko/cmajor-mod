// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.delegate;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class IdentifierNode;
class ParameterNode;

class DelegateNode : public Node
{
public:
    DelegateNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    DelegateNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* ReturnTypeExpr() const { return returnTypeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> returnTypeExpr;
    std::unique_ptr<IdentifierNode> id;
    NodeList<ParameterNode> parameters;
};

class ClassDelegateNode : public Node
{
public:
    ClassDelegateNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    ClassDelegateNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* ReturnTypeExpr() const { return returnTypeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> returnTypeExpr;
    std::unique_ptr<IdentifierNode> id;
    NodeList<ParameterNode> parameters;
};
} // namespace cmajor::ast
