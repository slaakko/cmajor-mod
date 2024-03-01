// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.cpp;

import std.core;
import util.code.formatter;
import soul.ast.source.pos;

export namespace soul::ast::cpp {

using util::CodeFormatter;

class Visitor;

enum class Operator
{
    deref, addrOf, plus, minus, and_, or_, not_, cpl, dotStar, arrowStar, bitand_, bitxor, bitor_, mul, div, rem, shiftLeft, shiftRight,
    less, greater, lessOrEq, greaterOrEq, eq, notEq, comma,
    assign, mulAssign, divAssign, remAssign, plusAssign, minusAssign, shiftLeftAssign, shiftRightAssign, andAssign, xorAssign, orAssign
};

std::string OperatorStr(Operator op);

enum class NodeKind
{
    exprListNode, thisNode, idExprNode, indexExprNode, invokeNode, memberAccessNode, ptrMemberAccessNode, postIncrementNode, postDecrementNode,
    staticCastNode, dynamicCastNode, reinterpretCastNode, constCastNode, postCastNode, typeIdExprNode, preIncrementNode, preDecrementNode,
    unaryOpExprNode, sizeOfNode, castNode, binaryOpExprNode, conditionalNode, throwExprNode, newNode, deleteNode, parenExprNode, literalNode,
    labeledStatementNode, caseStatementNode, defaultStatementNode, emptyStatementNode, expressionStatementNode, compoundStatementNode,
    ifStatementNode, switchStatementNode, whileStatementNode, doStatementNode, forStatementNode, breakStatementNode, continueStatementNode,
    gotoStatementNode, returnStatementNode, conditionWithDeclaratorNode, forRangeDeclarationNode, rangeForStatementNode, declarationStatementNode,
    exceptionDeclarationNode, handlerNode, tryStatementNode, ifdefStatementNode, endIfStatementNode, assignInitNode, initializerNode,
    initDeclaratorNode, initDeclaratorListNode, typedefNode, typeSpecifierNode, constNode, volatileNode, typeNameNode, typeNode,
    storageClassSpecifierNode, typeIdNode, simpleDeclarationNode, namespaceAliasNode, usingDeclarationNode, usingDirectiveNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    NodeKind Kind() const { return kind; }
    Node* Parent() const { return parent; }
    void SetParent(Node* parent_) { parent = parent_; }
    virtual ~Node();
    virtual Node* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Write(CodeFormatter& formatter) = 0;
    virtual void Add(Node* node) {}
    virtual std::string ToString() const { return std::string(); }
    virtual void Replace(Node* node, Node* replacement) {}
private:
    NodeKind kind;
    soul::ast::SourcePos sourcePos;
    Node* parent;
};

class UnaryNode : public Node
{
public:
    UnaryNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Child() const { return child.get(); }
    void Replace(Node* node, Node* replacement) override;
private:
    std::unique_ptr<Node> child;
};

class BinaryNode : public Node
{
public:
    BinaryNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, Node* left_, Node* right_);
    Node* Left() const { return left.get(); }
    Node* Right() const { return right.get(); }
    void Replace(Node* node, Node* replacement) override;
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class ExprListNode : public Node
{
public:
    ExprListNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
    void Add(Node* node) override;
    void Replace(Node* node, Node* replacement) override;
    const std::vector<std::unique_ptr<Node>>& Exprs() const { return exprs; }
    int Count() const { return exprs.size(); }
private:
    std::vector<std::unique_ptr<Node>> exprs;
};

class ThisNode : public Node
{
public:
    ThisNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return "this"; }
};

class IdExprNode : public Node
{
public:
    IdExprNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void SetId(const std::string& id_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return id; }
private:
    std::string id;
};

class IndexExprNode : public UnaryNode
{
public:
    IndexExprNode(const soul::ast::SourcePos& sourcePos_, Node* child_, Node* index_);
    Node* Index() const { return index.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> index;
};

class InvokeNode : public UnaryNode
{
public:
    InvokeNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    void Add(Node* node) override;
    void Replace(Node* node, Node* replacement) override;
    const std::vector<std::unique_ptr<Node>>& Args() const { return args; }
    std::string ToString() const override;
private:
    std::vector<std::unique_ptr<Node>> args;
};

class MemberAccessNode : public UnaryNode
{
public:
    MemberAccessNode(const soul::ast::SourcePos& sourcePos_, Node* child_, Node* member_);
    Node* Member() const { return member.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> member;
};

class PtrMemberAccessNode : public UnaryNode
{
public:
    PtrMemberAccessNode(const soul::ast::SourcePos& sourcePos_, Node* child_, Node* member_);
    Node* Member() const { return member.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> member;
};

class PostIncrementNode : public UnaryNode
{
public:
    PostIncrementNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class PostDecrementNode : public UnaryNode
{
public:
    PostDecrementNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class CppCastNode : public Node
{
public:
    CppCastNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
};

class StaticCastNode : public CppCastNode
{
public:
    StaticCastNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return "static_cast"; }
};

class DynamicCastNode : public CppCastNode
{
public:
    DynamicCastNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return "dynamic_cast"; }
};

class ReinterpretCastNode : public CppCastNode
{
public:
    ReinterpretCastNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return "reinterpret_cast"; }
};

class ConstCastNode : public CppCastNode
{
public:
    ConstCastNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return "const_cast"; }
};

class PostCastNode : public UnaryNode
{
public:
    PostCastNode(const soul::ast::SourcePos& sourcePos_, CppCastNode* cppCastNode_, Node* type_, Node* child_);
    CppCastNode* GetCppCastNode() const { return cppCastNode.get(); }
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<CppCastNode> cppCastNode;
    std::unique_ptr<Node> type;
};

class TypeIdExprNode : public UnaryNode
{
public:
    TypeIdExprNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class PreIncrementNode : public UnaryNode
{
public:
    PreIncrementNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class PreDecrementNode : public UnaryNode
{
public:
    PreDecrementNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class UnaryOpExprNode : public UnaryNode
{
public:
    UnaryOpExprNode(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* child_);
    Operator Op() const { return op; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    Operator op;
};

class SizeOfNode : public UnaryNode
{
public:
    SizeOfNode(const soul::ast::SourcePos& sourcePos_, Node* child_, bool parens_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    bool parens;
};

class CastNode : public UnaryNode
{
public:
    CastNode(const soul::ast::SourcePos& sourcePos_, Node* type_, Node* child_);
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> type;
};

class BinaryOpExprNode : public BinaryNode
{
public:
    BinaryOpExprNode(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* left_, Node* right_);
    Operator Op() const { return op; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    Operator op;
};

class ConditionalNode : public Node
{
public:
    ConditionalNode(const soul::ast::SourcePos& sourcePos_, Node* condition_, Node* thenExpr_, Node* elseExpr_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    Node* Condition() const { return condition.get(); }
    Node* ThenExpr() const { return thenExpr.get(); }
    Node* ElseExpr() const { return elseExpr.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> thenExpr;
    std::unique_ptr<Node> elseExpr;
};

class ThrowExprNode : public Node
{
public:
    ThrowExprNode(const soul::ast::SourcePos& sourcePos_, Node* exception_);
    Node* Exception() const { return exception.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> exception;
};

class NewNode : public Node
{
public:
    NewNode(const soul::ast::SourcePos& sourcePos_, bool global_);
    bool Global() const { return global; }
    bool Parens() const { return parens; }
    void SetParens(bool parens_) { parens = parens_; }
    void BeginAddToPlacement() { addToPlacement = true; }
    void EndAddToPlacement() { addToPlacement = false; }
    void BeginAddToInitializer() { addToInitializer = true; }
    void EndAddToInitializer() { addToPlacement = false; }
    void Add(Node* node) override;
    void SetTypeId(Node* typeId_);
    Node* TypeId() const { return typeId.get(); }
    const std::vector<std::unique_ptr<Node>>& Placement() const { return placement; }
    const std::vector<std::unique_ptr<Node>>& Initializer() const { return initializer; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    bool global;
    std::vector<std::unique_ptr<Node>> placement;
    std::unique_ptr<Node> typeId;
    bool parens;
    bool addToPlacement;
    bool addToInitializer;
    std::vector<std::unique_ptr<Node>> initializer;
};

class DeleteNode : public UnaryNode
{
public:
    DeleteNode(const soul::ast::SourcePos& sourcePos_, bool global_, bool isArray_, Node* child_);
    bool Global() const { return global; }
    bool IsArray() const { return isArray; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    bool global;
    bool isArray;
};

class ParenExprNode : public UnaryNode
{
public:
    ParenExprNode(const soul::ast::SourcePos& sourcePos_, Node* child_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class LiteralNode : public Node
{
public:
    LiteralNode(const soul::ast::SourcePos& sourcePos_, const std::string& rep_);
    const std::string& Rep() const { return rep; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return rep; }
private:
    std::string rep;
};

class StatementNode : public Node
{
public:
    StatementNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
    virtual bool IsCompoundStatementNode() const { return false; }
};

class LabeledStatementNode : public StatementNode
{
public:
    LabeledStatementNode(const soul::ast::SourcePos& sourcePos_, const std::string& label_, StatementNode* stmt_);
    const std::string& Label() const { return label; }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::string label;
    std::unique_ptr<StatementNode> stmt;
};

class CaseStatementNode : public StatementNode
{
public:
    CaseStatementNode(const soul::ast::SourcePos& sourcePos_, Node* caseExpr_, StatementNode* stmt_);
    Node* CaseExpr() const { return caseExpr.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> caseExpr;
    std::unique_ptr<StatementNode> stmt;
};

class DefaultStatementNode : public StatementNode
{
public:
    DefaultStatementNode(const soul::ast::SourcePos& sourcePos_, StatementNode* stmt_);
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<StatementNode> stmt;
};

class EmptyStatementNode : public StatementNode
{
public:
    EmptyStatementNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
};

class ExpressionStatementNode : public StatementNode
{
public:
    ExpressionStatementNode(const soul::ast::SourcePos& sourcePos_, Node* expr_);
    Node* Expr() const { return expr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> expr;
};

class CompoundStatementNode : public StatementNode
{
public:
    CompoundStatementNode(const soul::ast::SourcePos& sourcePos_);
    void InsertFront(StatementNode* stmt);
    void Add(StatementNode* stmt);
    void Replace(Node* node, Node* replacement) override;
    const std::vector<std::unique_ptr<StatementNode>>& Statements() const { return statements; };
    bool IsCompoundStatementNode() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(const soul::ast::SourcePos& sourcePos_, Node* cond_, StatementNode* thenStmt_, StatementNode* elseStmt_);
    Node* Cond() const { return cond.get(); }
    StatementNode* ThenStmt() const { return thenStmt.get(); }
    StatementNode* ElseStmt() const { return elseStmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<StatementNode> thenStmt;
    std::unique_ptr<StatementNode> elseStmt;
};

class SwitchStatementNode : public StatementNode
{
public:
    SwitchStatementNode(const soul::ast::SourcePos& sourcePos_, Node* cond_, StatementNode* stmt_);
    Node* Cond() const { return cond.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<StatementNode> stmt;
};

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(const soul::ast::SourcePos& sourcePos_, Node* cond_, StatementNode* stmt_);
    Node* Cond() const { return cond.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<StatementNode> stmt;
};

class DoStatementNode : public StatementNode
{
public:
    DoStatementNode(const soul::ast::SourcePos& sourcePos_, Node* cond_, StatementNode* stmt_);
    Node* Cond() const { return cond.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<StatementNode> stmt;
};

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(const soul::ast::SourcePos& sourcePos_, Node* init_, Node* cond_, Node* iter_, StatementNode* stmt_);
    Node* Init() const { return init.get(); }
    Node* Cond() const { return cond.get(); }
    Node* Iter() const { return iter.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> init;
    std::unique_ptr<Node> cond;
    std::unique_ptr<Node> iter;
    std::unique_ptr<StatementNode> stmt;
};

class BreakStatementNode : public StatementNode
{
public:
    BreakStatementNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
};

class ContinueStatementNode : public StatementNode
{
public:
    ContinueStatementNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
};

class GotoStatementNode : public StatementNode
{
public:
    GotoStatementNode(const soul::ast::SourcePos& sourcePos_, const std::string& target_);
    const std::string& Target() const { return target; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::string target;
};

class ReturnStatementNode : public StatementNode
{
public:
    ReturnStatementNode(const soul::ast::SourcePos& sourcePos_, Node* expr_);
    Node* Expr() const { return expr.get(); }
    void SetExpr(Node* expr_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> expr;
};

class TypeIdNode;

class ConditionWithDeclaratorNode : public Node
{
public:
    ConditionWithDeclaratorNode(const soul::ast::SourcePos& sourcePos_, TypeIdNode* type_, const std::string& declarator_, Node* expression_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    TypeIdNode* Type() const { return type.get(); }
    const std::string& Declarator() const { return declarator; }
    Node* Expression() const { return expression.get(); }
private:
    std::unique_ptr<TypeIdNode> type;
    std::string declarator;
    std::unique_ptr<Node> expression;
};

class SimpleDeclarationNode;

class ForRangeDeclarationNode : public Node
{
public:
    ForRangeDeclarationNode(const soul::ast::SourcePos& sourcePos_);
    SimpleDeclarationNode* Declaration() const { return declaration.get(); }
    const std::string& Declarator() const { return declarator; }
    void SetDeclarator(const std::string& declarator_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<SimpleDeclarationNode> declaration;
    std::string declarator;
};

class RangeForStatementNode : public StatementNode
{
public:
    RangeForStatementNode(const soul::ast::SourcePos& sourcePos_, ForRangeDeclarationNode* declaration_, Node* container_, StatementNode* stmt_);
    ForRangeDeclarationNode* Declaration() const { return declaration.get(); }
    Node* Container() const { return container.get(); }
    StatementNode* Stmt() const { return stmt.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<ForRangeDeclarationNode> declaration;
    std::unique_ptr<Node> container;
    std::unique_ptr<StatementNode> stmt;
};

class DeclarationStatementNode : public StatementNode
{
public:
    DeclarationStatementNode(const soul::ast::SourcePos& sourcePos_, Node* declaration_);
    Node* Declaration() const { return declaration.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> declaration;
};

class ExceptionDeclarationNode : public Node
{
public:
    ExceptionDeclarationNode(const soul::ast::SourcePos& sourcePos_);
    TypeIdNode* TypeId() const { return typeId.get(); }
    const std::string& Declarator() const { return declarator; }
    void SetDeclarator(const std::string& declarator_);
    bool CatchAll() const { return catchAll; }
    void SetCatchAll() { catchAll = true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<TypeIdNode> typeId;
    std::string declarator;
    bool catchAll;
};

class HandlerNode : public Node
{
public:
    HandlerNode(const soul::ast::SourcePos& sourcePos_, ExceptionDeclarationNode* exceptionDeclaration_, CompoundStatementNode* handlerBlock_);
    ExceptionDeclarationNode* ExceptionDeclaration() const { return exceptionDeclaration.get(); }
    CompoundStatementNode* HandlerBlock() const { return handlerBlock.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<ExceptionDeclarationNode> exceptionDeclaration;
    std::unique_ptr<CompoundStatementNode> handlerBlock;
};

class TryStatementNode : public StatementNode
{
public:
    TryStatementNode(const soul::ast::SourcePos& sourcePos_, CompoundStatementNode* tryBlock_);
    CompoundStatementNode* TryBlock() const { return tryBlock.get(); }
    void AddHandler(HandlerNode* handler);
    const std::vector<std::unique_ptr<HandlerNode>>& Handlers() const { return handlers; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
private:
    std::unique_ptr<CompoundStatementNode> tryBlock;
    std::vector<std::unique_ptr<HandlerNode>> handlers;
};

class IfdefStatementNode : public StatementNode
{
public:
    IfdefStatementNode(const soul::ast::SourcePos& sourcePos_, Node* symbol_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    Node* Symbol() const { return symbol.get(); }
private:
    std::unique_ptr<Node> symbol;
};

class EndIfStatementNode : public StatementNode
{
public:
    EndIfStatementNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
};

class AssignInitNode : public Node
{
public:
    AssignInitNode(const soul::ast::SourcePos& sourcePos_, Node* assignmentExpr_);
    Node* AssignmentExpr() const { return assignmentExpr.get(); }
    const std::vector<std::unique_ptr<AssignInitNode>>& SubInits() const { return subInits; }
    void Add(AssignInitNode* subInit);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
private:
    std::unique_ptr<Node> assignmentExpr;
    std::vector<std::unique_ptr<AssignInitNode>> subInits;
};

class InitializerNode : public Node
{
public:
    InitializerNode(const soul::ast::SourcePos& sourcePos_, AssignInitNode* assignInit_);
    AssignInitNode* GetAssignInit() const { return assignInit.get(); }
    const std::vector<std::unique_ptr<Node>>& GetExpressionList() const { return expressionList; }
    void Add(Node* expr) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AssignInitNode> assignInit;
    std::vector<std::unique_ptr<Node>> expressionList;
};

class InitDeclaratorNode : public Node
{
public:
    InitDeclaratorNode(const soul::ast::SourcePos& sourcePos_, const std::string& declarator_, InitializerNode* initializer_);
    const std::string& Declarator() const { return declarator; }
    InitializerNode* GetInitializer() const { return initializer.get(); }
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string declarator;
    std::unique_ptr<InitializerNode> initializer;
};

class InitDeclaratorListNode : public Node
{
public:
    InitDeclaratorListNode(const soul::ast::SourcePos& sourcePos_);
    const std::vector<std::unique_ptr<InitDeclaratorNode>>& InitDeclarators() const { return initDeclarators; }
    void Add(InitDeclaratorNode* initDeclarator);
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<InitDeclaratorNode>> initDeclarators;
};

class DeclSpecifierNode : public Node
{
public:
    DeclSpecifierNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    const std::string& Name() const { return name; }
private:
    std::string name;
};

class TypedefNode : public DeclSpecifierNode
{
public:
    TypedefNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class TypeSpecifierNode : public DeclSpecifierNode
{
public:
    TypeSpecifierNode(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    TypeSpecifierNode(const soul::ast::SourcePos& sourcePos_, NodeKind kind_, const std::string& name_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override { return Name(); }
};

class ConstNode : public TypeSpecifierNode
{
public:
    ConstNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class VolatileNode : public TypeSpecifierNode
{
public:
    VolatileNode(const soul::ast::SourcePos& sourcePos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TypeNameNode : public TypeSpecifierNode
{
public:
    TypeNameNode(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    void AddTemplateArgument(Node* templateArgument);
    const std::vector<std::unique_ptr<Node>>& TemplateArgs() const { return templateArguments; }
    bool IsTemplate() const { return isTemplate; }
    void SetTemplate() { isTemplate = true; }
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
    void Accept(Visitor& visitor) override;
    Node* Clone() const override;
private:
    bool isTemplate;
    std::vector<std::unique_ptr<Node>> templateArguments;
};

class TypeNode : public Node
{
public:
    TypeNode(const soul::ast::SourcePos& sourcePos_);
    void Add(TypeSpecifierNode* typeSpecifier);
    const std::vector<std::unique_ptr<TypeSpecifierNode>>& TypeSpecifiers() const { return typeSpecifiers; }
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<TypeSpecifierNode>> typeSpecifiers;
};

class StorageClassSpecifierNode : public DeclSpecifierNode
{
public:
    StorageClassSpecifierNode(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    std::string ToString() const override;
};

class TypeIdNode : public Node
{
public:
    TypeIdNode(const soul::ast::SourcePos& sourcePos_);
    void Add(TypeSpecifierNode* typeSpecifier);
    const std::vector<std::unique_ptr<TypeSpecifierNode>>& TypeSpecifiers() const { return typeSpecifiers; }
    const std::string& Declarator() const { return declarator; }
    void SetDeclarator(const std::string& declarator_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(CodeFormatter& formatter) override;
    void WriteNonPtrType(CodeFormatter& formatter);
    std::string ToString() const override;
    bool IsPtrType() const;
private:
    std::vector<std::unique_ptr<TypeSpecifierNode>> typeSpecifiers;
    std::string declarator;
};

class SimpleDeclarationNode : public Node
{
public:
    SimpleDeclarationNode(const soul::ast::SourcePos& sourcePos_);
    void Add(DeclSpecifierNode* declSpecifier);
    const std::vector<std::unique_ptr<DeclSpecifierNode>>& DeclSpecifiers() const { return declSpecifiers; }
    void SetInitDeclaratorList(InitDeclaratorListNode* initDeclaratorList_);
    InitDeclaratorListNode* GetInitDeclaratorList() const { return initDeclaratorList.get(); }
    void Write(CodeFormatter& formatter) override;
    void Accept(Visitor& visitor) override;
    Node* Clone() const override;
private:
    std::vector<std::unique_ptr<DeclSpecifierNode>> declSpecifiers;
    std::unique_ptr<InitDeclaratorListNode> initDeclaratorList;
};

class UsingObjectNode : public Node
{
public:
    UsingObjectNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    const std::string& Name() const { return name; }
private:
    std::string name;
};

class NamespaceAliasNode : public UsingObjectNode
{
public:
    NamespaceAliasNode(const soul::ast::SourcePos& sourcePos_, const std::string& aliasName_, const std::string& namespaceName_);
    void Write(CodeFormatter& formatter) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string aliasName;
    std::string namespaceName;
};

class UsingDeclarationNode : public UsingObjectNode
{
public:
    UsingDeclarationNode(const soul::ast::SourcePos& sourcePos_, const std::string& usingId_);
    const std::string& UsingId() const { return usingId; }
    void Write(CodeFormatter& formatter) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string usingId;
};

class UsingDirectiveNode : public UsingObjectNode
{
public:
    UsingDirectiveNode(const soul::ast::SourcePos& sourcePos_, const std::string& usingNs_);
    const std::string& UsingNs() const { return usingNs; }
    void Write(CodeFormatter& formatter) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string usingNs;
};

class Visitor
{
public:
    virtual void Visit(ExprListNode& node) {}
    virtual void Visit(ThisNode& node) {}
    virtual void Visit(IdExprNode& node) {}
    virtual void Visit(IndexExprNode& node) {}
    virtual void Visit(InvokeNode& node) {}
    virtual void Visit(MemberAccessNode& node) {}
    virtual void Visit(PtrMemberAccessNode& node) {}
    virtual void Visit(PostIncrementNode& node) {}
    virtual void Visit(PostDecrementNode& node) {}
    virtual void Visit(StaticCastNode& node) {}
    virtual void Visit(DynamicCastNode& node) {}
    virtual void Visit(ReinterpretCastNode& node) {}
    virtual void Visit(ConstCastNode& node) {}
    virtual void Visit(PostCastNode& node) {}
    virtual void Visit(TypeIdExprNode& node) {}
    virtual void Visit(PreIncrementNode& node) {}
    virtual void Visit(PreDecrementNode& node) {}
    virtual void Visit(UnaryOpExprNode& node) {}
    virtual void Visit(SizeOfNode& node) {}
    virtual void Visit(CastNode& node) {}
    virtual void Visit(BinaryOpExprNode& node) {}
    virtual void Visit(ConditionalNode& node) {}
    virtual void Visit(ThrowExprNode& node) {}
    virtual void Visit(NewNode& node) {}
    virtual void Visit(DeleteNode& node) {}
    virtual void Visit(ParenExprNode& node) {}
    virtual void Visit(LiteralNode& node) {}
    virtual void Visit(LabeledStatementNode& node) {}
    virtual void Visit(CaseStatementNode& node) {}
    virtual void Visit(DefaultStatementNode& node) {}
    virtual void Visit(EmptyStatementNode& node) {}
    virtual void Visit(ExpressionStatementNode& node) {}
    virtual void Visit(CompoundStatementNode& node) {}
    virtual void Visit(IfStatementNode& node) {}
    virtual void Visit(SwitchStatementNode& node) {}
    virtual void Visit(WhileStatementNode& node) {}
    virtual void Visit(DoStatementNode& node) {}
    virtual void Visit(ForStatementNode& node) {}
    virtual void Visit(BreakStatementNode& node) {}
    virtual void Visit(ContinueStatementNode& node) {}
    virtual void Visit(GotoStatementNode& node) {}
    virtual void Visit(ReturnStatementNode& node) {}
    virtual void Visit(ConditionWithDeclaratorNode& node) {}
    virtual void Visit(ForRangeDeclarationNode& node) {}
    virtual void Visit(RangeForStatementNode& node) {}
    virtual void Visit(DeclarationStatementNode& node) {}
    virtual void Visit(ExceptionDeclarationNode& node) {}
    virtual void Visit(HandlerNode& node) {}
    virtual void Visit(TryStatementNode& node) {}
    virtual void Visit(IfdefStatementNode& node) {}
    virtual void Visit(EndIfStatementNode& node) {}
    virtual void Visit(AssignInitNode& node) {}
    virtual void Visit(InitializerNode& node) {}
    virtual void Visit(InitDeclaratorNode& node) {}
    virtual void Visit(InitDeclaratorListNode& node) {}
    virtual void Visit(TypedefNode& node) {}
    virtual void Visit(TypeSpecifierNode& node) {}
    virtual void Visit(ConstNode& node) {}
    virtual void Visit(VolatileNode& node) {}
    virtual void Visit(TypeNameNode& node) {}
    virtual void Visit(TypeNode& node) {}
    virtual void Visit(StorageClassSpecifierNode& node) {}
    virtual void Visit(TypeIdNode& node) {}
    virtual void Visit(SimpleDeclarationNode& node) {}
    virtual void Visit(NamespaceAliasNode& node) {}
    virtual void Visit(UsingDeclarationNode& node) {}
    virtual void Visit(UsingDirectiveNode& node) {}
};

class DefaultVisitor : public Visitor
{
public:
    void Visit(ExprListNode& node) override;
    void Visit(IndexExprNode& node) override;
    void Visit(InvokeNode& node) override;
    void Visit(MemberAccessNode& node) override;
    void Visit(PtrMemberAccessNode& node) override;
    void Visit(PostIncrementNode& node) override;
    void Visit(PostDecrementNode& node) override;
    void Visit(PostCastNode& node) override;
    void Visit(TypeIdExprNode& node) override;
    void Visit(PreIncrementNode& node) override;
    void Visit(PreDecrementNode& node) override;
    void Visit(UnaryOpExprNode& node) override;
    void Visit(SizeOfNode& node) override;
    void Visit(CastNode& node) override;
    void Visit(BinaryOpExprNode& node) override;
    void Visit(ConditionalNode& node) override;
    void Visit(ThrowExprNode& node) override;
    void Visit(NewNode& node) override;
    void Visit(DeleteNode& node) override;
    void Visit(ParenExprNode& node) override;
    void Visit(LabeledStatementNode& node) override;
    void Visit(CaseStatementNode& node) override;
    void Visit(DefaultStatementNode& node) override;
    void Visit(ExpressionStatementNode& node) override;
    void Visit(CompoundStatementNode& node) override;
    void Visit(IfStatementNode& node) override;
    void Visit(SwitchStatementNode& node) override;
    void Visit(WhileStatementNode& node) override;
    void Visit(DoStatementNode& node) override;
    void Visit(ForStatementNode& node) override;
    void Visit(ReturnStatementNode& node) override;
    void Visit(ConditionWithDeclaratorNode& node) override;
    void Visit(ForRangeDeclarationNode& node) override;
    void Visit(RangeForStatementNode& node) override;
    void Visit(DeclarationStatementNode& node) override;
    void Visit(ExceptionDeclarationNode& node) override;
    void Visit(HandlerNode& node) override;
    void Visit(TryStatementNode& node) override;
    void Visit(IfdefStatementNode& node) override;
    void Visit(AssignInitNode& node) override;
    void Visit(InitializerNode& node) override;
    void Visit(InitDeclaratorNode& node) override;
    void Visit(InitDeclaratorListNode& node) override;
    void Visit(TypeNameNode& node) override;
    void Visit(TypeNode& node) override;
    void Visit(TypeIdNode& node) override;
    void Visit(SimpleDeclarationNode& node) override;
};

class CodeEvaluationVisitor : public DefaultVisitor
{
public:
    CodeEvaluationVisitor();
    bool HasReturn() const { return hasReturn; }
    bool HasPass() const { return hasPass; }
    bool HasVars() const { return hasVars; }
    void Visit(ReturnStatementNode& node) override;
    void Visit(IdExprNode& node) override;
private:
    bool hasReturn;
    bool hasPass;
    bool hasVars;
};

} // namespace soul::ast::cpp
