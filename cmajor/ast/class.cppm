// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.classes;

import cmajor.ast.function;
import cmajor.ast.identifier;
import cmajor.ast.statement;
import cmajor.ast.attribute;
import cmajor.ast.node;
import cmajor.ast.node.list;
import cmajor.ast.concepts;
import cmajor.ast.templates;
import cmajor.ast.parameter;

export namespace cmajor::ast {

class ClassNode : public Node
{
public:
    ClassNode(const soul::ast::Span& span_);
    ClassNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddTemplateParameter(TemplateParameterNode* templateParameter) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    void SetSpecifiers(Specifiers specifiers_) { specifiers = specifiers_; }
    IdentifierNode* Id() const { return id.get(); }
    void AddBaseClassOrInterface(Node* baseClassOrInterface);
    const WhereConstraintNode* WhereConstraint() const { return constraint.get(); }
    WhereConstraintNode* WhereConstraint() { return constraint.get(); }
    void SetConstraint(WhereConstraintNode* whereConstraint);
    void AddMember(Node* member);
    const NodeList<TemplateParameterNode>& TemplateParameters() const { return templateParameters; }
    const NodeList<Node>& BaseClassOrInterfaces() const { return baseClassOrInterfaces; }
    const NodeList<Node>& Members() const { return members; }
    AttributesNode* GetAttributes() const { return attributes.get(); }
    void ArrangeMembers();
    int Level() const;
private:
    Specifiers specifiers;
    std::unique_ptr<IdentifierNode> id;
    NodeList<TemplateParameterNode> templateParameters;
    NodeList<Node> baseClassOrInterfaces;
    std::unique_ptr<WhereConstraintNode> constraint;
    NodeList<Node> members;
    std::unique_ptr<AttributesNode> attributes;
};

class InitializerNode : public Node
{
public:
    InitializerNode(NodeType nodeType_, const soul::ast::Span& span_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddArgument(Node* argument) override;
    const NodeList<Node>& Arguments() const { return arguments; }
private:
    NodeList<Node> arguments;
};

class ThisInitializerNode : public InitializerNode
{
public:
    ThisInitializerNode(const soul::ast::Span& span_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class BaseInitializerNode : public InitializerNode
{
public:
    BaseInitializerNode(const soul::ast::Span& span_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class MemberInitializerNode : public InitializerNode
{
public:
    MemberInitializerNode(const soul::ast::Span& span_);
    MemberInitializerNode(const soul::ast::Span& span_, IdentifierNode* memberId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const IdentifierNode* MemberId() const { return memberId.get(); }
    IdentifierNode* MemberId() { return memberId.get(); }
private:
    std::unique_ptr<IdentifierNode> memberId;
};

class StaticConstructorNode : public FunctionNode
{
public:
    StaticConstructorNode(const soul::ast::Span& span_);
    StaticConstructorNode(const soul::ast::Span& span_, Specifiers specifiers_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddInitializer(InitializerNode* initializer);
    const NodeList<InitializerNode>& Initializers() const { return initializers; }
    void SetClassId(IdentifierNode* classId_) { classId.reset(classId_); }
    IdentifierNode* ClassId() { return classId.get(); }
private:
    NodeList<InitializerNode> initializers;
    std::unique_ptr<IdentifierNode> classId;
};

class ConstructorNode : public FunctionNode
{
public:
    ConstructorNode(const soul::ast::Span& span_);
    ConstructorNode(const soul::ast::Span& span_, Specifiers specifiers_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddInitializer(InitializerNode* initializer);
    const NodeList<InitializerNode>& Initializers() const { return initializers; }
    void SetClassId(IdentifierNode* classId_) { classId.reset(classId_); }
    IdentifierNode* ClassId() { return classId.get(); }
private:
    NodeList<InitializerNode> initializers;
    std::unique_ptr<IdentifierNode> classId;
};

class DestructorNode : public FunctionNode
{
public:
    DestructorNode(const soul::ast::Span& span_);
    DestructorNode(const soul::ast::Span& span_, Specifiers specifiers_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void SetClassId(IdentifierNode* classId_) { classId.reset(classId_); }
    IdentifierNode* ClassId() { return classId.get(); }
private:
    std::unique_ptr<IdentifierNode> classId;
};

class MemberFunctionNode : public FunctionNode
{
public:
    MemberFunctionNode(const soul::ast::Span& span_);
    MemberFunctionNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsConst() const { return (GetSpecifiers() & Specifiers::const_) != Specifiers::none; }
    void SetConst() { SetSpecifiers(GetSpecifiers() | Specifiers::const_); }
};

class ConversionFunctionNode : public FunctionNode
{
public:
    ConversionFunctionNode(const soul::ast::Span& span_);
    ConversionFunctionNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsConst() const { return (GetSpecifiers() & Specifiers::const_) != Specifiers::none; }
    void SetConst() { SetSpecifiers(GetSpecifiers() | Specifiers::const_); }
};

class MemberVariableNode : public Node
{
public:
    MemberVariableNode(const soul::ast::Span& span_);
    MemberVariableNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* TypeExpr() const { return typeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
    AttributesNode* GetAttributes() const { return attributes.get(); }
/*
    void SetSpecifierSourcePos(const soul::ast::SourcePos& specifierSourcePos_) { specifierSourcePos = specifierSourcePos_; }
    const soul::ast::SourcePos& SpecifierSourcePos() const { return specifierSourcePos; }
*/
private:
    Specifiers specifiers;
    //soul::ast::SourcePos specifierSourcePos;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<AttributesNode> attributes;
};
} // namespace cmajor::ast
