// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.classes;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.attribute;
import cmajor.fault.tolerant.ast.specifier;
import cmajor.fault.tolerant.ast.templates;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.concepts;
import cmajor.fault.tolerant.ast.statement;
import cmajor.fault.tolerant.ast.parameter;
import cmajor.fault.tolerant.ast.expression;

export namespace cmajor::fault::tolerant::ast {

class ClassNode : public SyntaxNode
{
public:
    ClassNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetClassKeyword(ClassKeywordNode* classKeyword_);
    ClassKeywordNode* ClassKeyword() const { return classKeyword.get(); }
    void SetIdentifier(IdentifierNode* identifier_);
    IdentifierNode* Identifier() const { return identifier.get(); }
    TemplateParameterListNode* TemplateParameterList() { return &templateParameterList; }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void AddComma(CommaNode* comma) override;
    void AddBaseClassOrInterface(Node* baseClassOrInterface);
    ListNode* BaseClassAndInterfaceList() { return &baseClassAndInterfaceList; }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddMember(Node* member);
    NodeList<Node>& Members() { return members; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<ClassKeywordNode> classKeyword;
    std::unique_ptr<IdentifierNode> identifier;
    TemplateParameterListNode templateParameterList;
    std::unique_ptr<ColonNode> colon;
    ListNode baseClassAndInterfaceList;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<Node> members;
    std::unique_ptr<RBraceNode> rbrace;
};

class InitializerNode : public SyntaxNode
{
public:
    InitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void AddNode(Node* argument) override;
    void AddComma(CommaNode* comma) override;
    ListNode& ArgumentList() { return argumentList; }
    const ListNode& ArgumentList() const { return argumentList; }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
private:
    std::unique_ptr<LParenNode> lparen;
    ListNode argumentList;
    std::unique_ptr<RParenNode> rparen;
};

class ThisInitializerNode : public InitializerNode
{
public:
    ThisInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetThisKeyword(ThisKeywordNode* thisKeyword_);
    ThisKeywordNode* ThisKeyword() const { return thisKeyword.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ThisKeywordNode> thisKeyword;
};

class BaseInitializerNode : public InitializerNode
{
public:
    BaseInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetBaseKeyword(BaseKeywordNode* baseKeyword_);
    BaseKeywordNode* BaseKeyword() const { return baseKeyword.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<BaseKeywordNode> baseKeyword;
};

class MemberInitializerNode : public InitializerNode
{
public:
    MemberInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> id;
};

class InitializerListNode : public ListNode
{
public:
    InitializerListNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddInitializer(InitializerNode* initializer);
    const std::vector<InitializerNode*>& Initializers() const { return initializers; }
    void CloneFrom(const InitializerListNode& that);
private:
    std::vector<InitializerNode*> initializers;
};

class StaticConstructorNode : public SyntaxNode
{
public:
    StaticConstructorNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetClassName(IdentifierNode* className_);
    IdentifierNode* ClassName() const { return className.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void AddInitializer(InitializerNode* initializer);
    void AddComma(CommaNode* comma) override;
    InitializerListNode* InitializerList() { return &initializerList; }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* body_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<IdentifierNode> className;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<ColonNode> colon;
    InitializerListNode initializerList;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ConstructorNode : public SyntaxNode
{
public:
    ConstructorNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetClassName(IdentifierNode* className_);
    IdentifierNode* ClassName() const { return className.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void AddInitializer(InitializerNode* initializer);
    void AddComma(CommaNode* comma) override;
    InitializerListNode* InitializerList() { return &initializerList; }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* body_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<IdentifierNode> className;
    ParameterListNode parameterList;
    std::unique_ptr<ColonNode> colon;
    InitializerListNode initializerList;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
};

class DestructorNode : public SyntaxNode
{
public:
    DestructorNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetComplement(ComplementNode* complement_);
    ComplementNode* Complement() const { return complement.get(); }
    void SetClassName(IdentifierNode* className_);
    IdentifierNode* ClassName() const { return className.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* body_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<ComplementNode> complement;
    std::unique_ptr<IdentifierNode> className;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
};

class MemberFunctionNode : public SyntaxNode
{
public:
    MemberFunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetFunctionGroupId(Node* functionGroupId_);
    Node* FunctionGroupId() const { return functionGroupId.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetConstKeyword(ConstKeywordNode* constKeyword_);
    ConstKeywordNode* ConstKeyword() const { return constKeyword.get(); }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* body_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<Node> returnType;
    std::unique_ptr<Node> functionGroupId;
    ParameterListNode parameterList;
    std::unique_ptr<ConstKeywordNode> constKeyword;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ConversionFunctionNode : public SyntaxNode
{
public:
    ConversionFunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Name() const { return name; }
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetOperatorKeyword(OperatorKeywordNode* operatorKeyword_);
    OperatorKeywordNode* OperatorKeyword() const { return operatorKeyword.get(); }
    void SetType(Node* type_);
    Node* Type() const { return type.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetConstKeyword(ConstKeywordNode* constKeyword_);
    ConstKeywordNode* ConstKeyword() const { return constKeyword.get(); }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* body_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<OperatorKeywordNode> operatorKeyword;
    std::unique_ptr<Node> type;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<ConstKeywordNode> constKeyword;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
    std::u16string name;
};

class MemberVariableNode : public SyntaxNode
{
public:
    MemberVariableNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetType(Node* type_);
    Node* Type() const { return type.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<Node> type;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
