// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.class_;

import cmajor.ast.function;
import cmajor.ast.identifier;
import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class TemplateParameterNode;
class WhereConstraintNode;
class AttributesNode;

class ClassNode : public Node
{
public:
    ClassNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ClassNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddTemplateParameter(Node* templateParameter) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    void SetSpecifiers(Specifiers specifiers_) { specifiers = specifiers_; }
    IdentifierNode* Id() const { return id.get(); }
    void AddBaseClassOrInterface(Node* baseClassOrInterface);
    const WhereConstraintNode* WhereConstraint() const { return constraint.get(); }
    WhereConstraintNode* WhereConstraint() { return constraint.get(); }
    void SetConstraint(WhereConstraintNode* whereConstraint);
    void AddMember(Node* member);
    //const NodeList<TemplateParameterNode>& TemplateParameters() const { return templateParameters; }
    const NodeList<Node>& TemplateParameters() const { return templateParameters; }
    const NodeList<Node>& BaseClassOrInterfaces() const { return baseClassOrInterfaces; }
    const NodeList<Node>& Members() const { return members; }
    AttributesNode* GetAttributes() const { return attributes.get(); }
    void SetSpecifierSourcePos(const soul::ast::SourcePos& specifierSourcePos_) { specifierSourcePos = specifierSourcePos_; }
    const soul::ast::SourcePos& SpecifierSourcePos() const { return specifierSourcePos; }
    void SetClassSourcePos(const soul::ast::SourcePos& classSourcePos_) { classSourcePos = classSourcePos_; }
    const soul::ast::SourcePos& ClassSourcePos() const { return classSourcePos; }
    void SetBeginBraceSourcePos(const soul::ast::SourcePos& beginBraceSourcePos_) { beginBraceSourcePos = beginBraceSourcePos_; }
    const soul::ast::SourcePos& BeginBraceSourcePos() const { return beginBraceSourcePos; }
    void SetEndBraceSourcePos(const soul::ast::SourcePos& endBraceSourcePos_) { endBraceSourcePos = endBraceSourcePos_; }
    const soul::ast::SourcePos& EndBraceSourcePos() const { return endBraceSourcePos; }
    void ArrangeMembers();
    int Level() const;
private:
    Specifiers specifiers;
    soul::ast::SourcePos specifierSourcePos;
    soul::ast::SourcePos classSourcePos;
    soul::ast::SourcePos beginBraceSourcePos;
    soul::ast::SourcePos endBraceSourcePos;
    std::unique_ptr<IdentifierNode> id;
    //NodeList<TemplateParameterNode> templateParameters;
    NodeList<Node> templateParameters;
    NodeList<Node> baseClassOrInterfaces;
    std::unique_ptr<WhereConstraintNode> constraint;
    NodeList<Node> members;
    std::unique_ptr<AttributesNode> attributes;
};

class InitializerNode : public Node
{
public:
    InitializerNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
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
    ThisInitializerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class BaseInitializerNode : public InitializerNode
{
public:
    BaseInitializerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class MemberInitializerNode : public InitializerNode
{
public:
    MemberInitializerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MemberInitializerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* memberId_);
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
    StaticConstructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    StaticConstructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, AttributesNode* attributes_);
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
    ConstructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConstructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, AttributesNode* attributes_);
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
    DestructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DestructorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, AttributesNode* attributes_);
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
    MemberFunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MemberFunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsConst() const { return (GetSpecifiers() & Specifiers::const_) != Specifiers::none; }
    void SetConst() { SetSpecifiers(GetSpecifiers() | Specifiers::const_); }
};

class ConversionFunctionNode : public FunctionNode
{
public:
    ConversionFunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConversionFunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsConst() const { return (GetSpecifiers() & Specifiers::const_) != Specifiers::none; }
    void SetConst() { SetSpecifiers(GetSpecifiers() | Specifiers::const_); }
};

class MemberVariableNode : public Node
{
public:
    MemberVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MemberVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, AttributesNode* attributes_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* TypeExpr() const { return typeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
    AttributesNode* GetAttributes() const { return attributes.get(); }
    void SetSpecifierSourcePos(const soul::ast::SourcePos& specifierSourcePos_) { specifierSourcePos = specifierSourcePos_; }
    const soul::ast::SourcePos& SpecifierSourcePos() const { return specifierSourcePos; }
private:
    Specifiers specifiers;
    soul::ast::SourcePos specifierSourcePos;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<AttributesNode> attributes;
};
} // namespace cmajor::ast
