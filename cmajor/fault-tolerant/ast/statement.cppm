// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.statement;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.expression;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class StatementNode : public SyntaxNode
{
public:
    StatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
};

class CompoundStatementNode : public StatementNode
{
public:
    CompoundStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<StatementNode> statements;
    std::unique_ptr<RBraceNode> rbrace;
};

class ReturnStatementNode : public StatementNode
{
public:
    ReturnStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetReturnKeyword(ReturnKeywordNode* returnKeyword_);
    ReturnKeywordNode* ReturnKeyword() const { return returnKeyword.get(); }
    void SetReturnValue(Node* returnValue_);
    Node* ReturnValue() const { return returnValue.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ReturnKeywordNode> returnKeyword;
    std::unique_ptr<Node> returnValue;
    std::unique_ptr<SemicolonNode> semicolon;
};

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetIfKeyword(IfKeywordNode* ifKeyword_);
    IfKeywordNode* IfKeyword() const { return ifKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetThenStatement(StatementNode* thenS_);
    StatementNode* ThenStatement() const { return thenS.get(); }
    void SetElseKeyword(ElseKeywordNode* elseKeyword_);
    ElseKeywordNode* ElseKeyword() const { return elseKeyword.get(); }
    void SetElseStatement(StatementNode* elseS_);
    StatementNode* ElseStatement() const { return elseS.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IfKeywordNode> ifKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> condition;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<StatementNode> thenS;
    std::unique_ptr<ElseKeywordNode> elseKeyword;
    std::unique_ptr<StatementNode> elseS;
};

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetWhileKeyword(WhileKeywordNode* whileKeyword_);
    WhileKeywordNode* WhileKeyword() const { return whileKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<WhileKeywordNode> whileKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> condition;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<StatementNode> statement;
};

class DoStatementNode : public StatementNode
{
public:
    DoStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetDoKeyword(DoKeywordNode* doKeyword_);
    DoKeywordNode* DoKeyword() const { return doKeyword.get(); }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    void SetWhileKeyword(WhileKeywordNode* whileKeyword_);
    WhileKeywordNode* WhileKeyword() const { return whileKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<DoKeywordNode> doKeyword;
    std::unique_ptr<StatementNode> statement;
    std::unique_ptr<WhileKeywordNode> whileKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> condition;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<SemicolonNode> semicolon;
};

class RangeForStatementNode : public StatementNode
{
public:
    RangeForStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetForKeyword(ForKeywordNode* forKeyword_);
    ForKeywordNode* ForKeyword() const { return forKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void SetContainer(Node* container_);
    Node* Container() const { return container.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ForKeywordNode> forKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<ColonNode> colon;
    std::unique_ptr<Node> container;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<StatementNode> statement;
};

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetForKeyword(ForKeywordNode* forKeyword_);
    ForKeywordNode* ForKeyword() const { return forKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetInitStatement(StatementNode* initStatement_);
    StatementNode* InitStatement() const { return initStatement.get(); }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    void SetLoopStatement(StatementNode* loopStatement_);
    StatementNode* LoopStatement() const { return loopStatement.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ForKeywordNode> forKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<StatementNode> initStatement;
    std::unique_ptr<Node> condition;
    std::unique_ptr<SemicolonNode> semicolon;
    std::unique_ptr<StatementNode> loopStatement;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<StatementNode> statement;
};

class BreakStatementNode : public StatementNode
{
public:
    BreakStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetBreakKeyword(BreakKeywordNode* breakKeyword_);
    BreakKeywordNode* BreakKeyword() const { return breakKeyword.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<BreakKeywordNode> breakKeyword;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ContinueStatementNode : public StatementNode
{
public:
    ContinueStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetContinueKeyword(ContinueKeywordNode* continueKeyword_);
    ContinueKeywordNode* ContinueKeyword() const { return continueKeyword.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ContinueKeywordNode> continueKeyword;
    std::unique_ptr<SemicolonNode> semicolon;
};

class GotoStatementNode : public StatementNode
{
public:
    GotoStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetGotoKeyword(GotoKeywordNode* gotoKeyword_);
    GotoKeywordNode* GotoKeyword() const { return gotoKeyword.get(); }
    void SetTarget(IdentifierNode* target_);
    IdentifierNode* Target() const { return target.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<GotoKeywordNode> gotoKeyword;
    std::unique_ptr<IdentifierNode> target;
    std::unique_ptr<SemicolonNode> semicolon;
};

class GotoCaseStatementNode : public StatementNode
{
public:
    GotoCaseStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetGotoKeyword(GotoKeywordNode* gotoKeyword_);
    GotoKeywordNode* GotoKeyword() const { return gotoKeyword.get(); }
    void SetCaseKeyword(CaseKeywordNode* caseKeyword_);
    CaseKeywordNode* CaseKeyword() const { return caseKeyword.get(); }
    void SetCaseExpr(Node* caseExpr_);
    Node* CaseExpr() const { return caseExpr.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<GotoKeywordNode> gotoKeyword;
    std::unique_ptr<CaseKeywordNode> caseKeyword;
    std::unique_ptr<Node> caseExpr;
    std::unique_ptr<SemicolonNode> semicolon;
};

class GotoDefaultStatementNode : public StatementNode
{
public:
    GotoDefaultStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetGotoKeyword(GotoKeywordNode* gotoKeyword_);
    GotoKeywordNode* GotoKeyword() const { return gotoKeyword.get(); }
    void SetDefaultKeyword(DefaultKeywordNode* defaultKeyword_);
    DefaultKeywordNode* DefaultKeyword() const { return defaultKeyword.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<GotoKeywordNode> gotoKeyword;
    std::unique_ptr<DefaultKeywordNode> defaultKeyword;
    std::unique_ptr<SemicolonNode> semicolon;
};

class CaseExprNode : public SyntaxNode
{
public:
    CaseExprNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetCaseKeyword(CaseKeywordNode* caseKeyword_);
    CaseKeywordNode* CaseKeyword() const { return caseKeyword.get(); }
    void SetCaseExpr(Node* caseExpr_);
    Node* CaseExpr() const { return caseExpr.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<CaseKeywordNode> caseKeyword;
    std::unique_ptr<Node> caseExpr;
    std::unique_ptr<ColonNode> colon;
};

class CaseStatementNode : public StatementNode
{
public:
    CaseStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddCaseExpr(CaseExprNode* caseExpr);
    NodeList<CaseExprNode>* Cases() { return &cases; }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    NodeList<CaseExprNode> cases;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<StatementNode> statements;
    std::unique_ptr<RBraceNode> rbrace;
};

class DefaultStatementNode : public StatementNode
{
public:
    DefaultStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetDefaultKeyword(DefaultKeywordNode* defaultKeyword_);
    DefaultKeywordNode* DefaultKeyword() const { return defaultKeyword.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<DefaultKeywordNode> defaultKeyword;
    std::unique_ptr<ColonNode> colon;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<StatementNode> statements;
    std::unique_ptr<RBraceNode> rbrace;
};

class SwitchStatementNode : public StatementNode
{
public:
    SwitchStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSwitchKeyword(SwitchKeywordNode* switchKeyword_);
    SwitchKeywordNode* SwitchKeyword() const { return switchKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddCaseStatement(CaseStatementNode* caseStatement);
    void SetDefaultStatement(DefaultStatementNode* defaultStatement_);
    NodeList<CaseStatementNode>* CaseStatements() { return &caseStatements; }
    DefaultStatementNode* DefaultStatement() const { return defaultStatement.get(); }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SwitchKeywordNode> switchKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> condition;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<CaseStatementNode> caseStatements;
    std::unique_ptr<DefaultStatementNode> defaultStatement;
    std::unique_ptr<RBraceNode> rbrace;
};

class AssignmentStatementNode : public StatementNode
{
public:
    AssignmentStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLeft(Node* left_);
    Node* Left() const { return left.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    void SetRight(Node* right_);
    Node* Right() const { return right.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<Node> right;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ExpressionStatementNode : public StatementNode
{
public:
    ExpressionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetExpression(Node* expression_);
    Node* Expression() const { return expression.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> expression;
    std::unique_ptr<SemicolonNode> semicolon;
};

class EmptyStatementNode : public StatementNode
{
public:
    EmptyStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SemicolonNode> semicolon;
};

class ConstructionStatementNode : public StatementNode
{
public:
    ConstructionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void AddNode(Node* node) override;
    void AddComma(CommaNode* comma) override;
    ListNode* ArgumentList() { return &argumentList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    ListNode argumentList;
    std::unique_ptr<SemicolonNode> semicolon;
};

class DeleteStatementNode : public StatementNode
{
public:
    DeleteStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetDeleteKeyword(DeleteKeywordNode* deleteKeyword_);
    DeleteKeywordNode* DeleteKeyword() const { return deleteKeyword.get(); }
    void SetPtr(Node* ptr_);
    Node* Ptr() const { return ptr.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<DeleteKeywordNode> deleteKeyword;
    std::unique_ptr<Node> ptr;
    std::unique_ptr<SemicolonNode> semicolon;
};

class DestroyStatementNode : public StatementNode
{
public:
    DestroyStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetDestroyKeyword(DestroyKeywordNode* destroyKeyword_);
    DestroyKeywordNode* DestroyKeyword() const { return destroyKeyword.get(); }
    void SetPtr(Node* ptr_);
    Node* Ptr() const { return ptr.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<DestroyKeywordNode> destroyKeyword;
    std::unique_ptr<Node> ptr;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ThrowStatementNode : public StatementNode
{
public:
    ThrowStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetThrowKeyword(ThrowKeywordNode* throwKeyword_);
    ThrowKeywordNode* ThrowKeyword() const { return throwKeyword.get(); }
    void SetException(Node* exception_);
    Node* Exception() const { return exception.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ThrowKeywordNode> throwKeyword;
    std::unique_ptr<Node> exception;
    std::unique_ptr<SemicolonNode> semicolon;
};

class CatchNode : public SyntaxNode
{
public:
    CatchNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetCatchKeyword(CatchKeywordNode* catchKeyword_);
    CatchKeywordNode* CatchKeyword() const { return catchKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetCatchBlock(CompoundStatementNode* catchBlock_);
    CompoundStatementNode* CatchBlock() const { return catchBlock.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<CatchKeywordNode> catchKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<CompoundStatementNode> catchBlock;
};

class TryStatementNode : public StatementNode
{
public:
    TryStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTryKeyword(TryKeywordNode* tryKewyord_);
    TryKeywordNode* TryKeyword() const { return tryKeyword.get(); }
    void SetTryBlock(CompoundStatementNode* tryBlock_);
    CompoundStatementNode* TryBlock() const { return tryBlock.get(); }
    void AddCatch(CatchNode* catchNode);
    NodeList<CatchNode>* Catches() { return &catches; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TryKeywordNode> tryKeyword;
    std::unique_ptr<CompoundStatementNode> tryBlock;
    NodeList<CatchNode> catches;
};

class LabeledStatementNode : public StatementNode
{
public:
    LabeledStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<ColonNode> colon;
    std::unique_ptr<StatementNode> statement;
};

class AssertStatementNode : public StatementNode
{
public:
    AssertStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetHash(HashNode* hash_);
    HashNode* Hash() const { return hash.get(); }
    void SetAssertKeyword(AssertKeywordNode* assertKeyword_);
    AssertKeywordNode* AssertKeyword() const { return assertKeyword.get(); }
    void SetExpression(Node* expression_);
    Node* Expression() const { return expression.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<HashNode> hash;
    std::unique_ptr<AssertKeywordNode> assertKeyword;
    std::unique_ptr<Node> expression;
    std::unique_ptr<SemicolonNode> semicolon;
};

class CondCompExprNode : public SyntaxNode
{
public:
    CondCompExprNode(const soul::ast::Span& span_, CompletionContext completionContext_);
};

class CondCompBinaryExprNode : public CondCompExprNode
{
public:
    CondCompBinaryExprNode(CondCompExprNode* left_, Node* op_, CondCompExprNode* right_);
    CondCompExprNode* Left() const { return left.get(); }
    Node* Op() const { return op.get(); }
    CondCompExprNode* Right() const { return right.get(); }
private:
    std::unique_ptr<CondCompExprNode> left;
    std::unique_ptr<Node> op;
    std::unique_ptr<CondCompExprNode> right;
};

class CondCompDisjunctionNode : public CondCompBinaryExprNode
{
public:
    CondCompDisjunctionNode(CondCompExprNode* left_, DisjunctionNode* op_, CondCompExprNode* right_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class CondCompConjunctionNode : public CondCompBinaryExprNode
{
public:
    CondCompConjunctionNode(CondCompExprNode* left_, AmpAmpNode* op_, CondCompExprNode* right_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class CondCompNotNode : public CondCompExprNode
{
public:
    CondCompNotNode(NotNode* not_, CondCompExprNode* prefix_);
    NotNode* Not() const { return notOp.get(); }
    CondCompExprNode* Prefix() const { return prefix.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<NotNode> notOp;
    std::unique_ptr<CondCompExprNode> prefix;
};

class CondCompParenExprNode : public CondCompExprNode
{
public:
    CondCompParenExprNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetExpr(CondCompExprNode* expr_);
    CondCompExprNode* Expr() const { return expr.get(); }
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<CondCompExprNode> expr;
    std::unique_ptr<RParenNode> rparen;
};

class CondCompSymbolNode : public CondCompExprNode
{
public:
    CondCompSymbolNode(const soul::ast::Span& span_, CompletionContext completionContext_, IdentifierNode* symbol_);
    IdentifierNode* Symbol() const { return symbol.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> symbol;
};

class IfGroupNode : public SyntaxNode
{
public:
    IfGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetHash(HashNode* hash_);
    HashNode* Hash() const { return hash.get(); }
    void SetIfKeyword(IfKeywordNode* ifKeyword_);
    IfKeywordNode* IfKeyword() const { return ifKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetExpr(CondCompExprNode* expr_);
    CondCompExprNode* Expr() const { return expr.get(); }
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<HashNode> hash;
    std::unique_ptr<IfKeywordNode> ifKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<CondCompExprNode> expr;
    std::unique_ptr<RParenNode> rparen;
    NodeList<StatementNode> statements;
};

class ElifGroupNode : public SyntaxNode
{
public:
    ElifGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetHash(HashNode* hash_);
    HashNode* Hash() const { return hash.get(); }
    void SetElifKeyword(ElifKeywordNode* elifKeyword_);
    ElifKeywordNode* ElifKeyword() const { return elifKeyword.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetExpr(CondCompExprNode* expr_);
    CondCompExprNode* Expr() const { return expr.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<HashNode> hash;
    std::unique_ptr<ElifKeywordNode> elifKeyword;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<CondCompExprNode> expr;
    std::unique_ptr<RParenNode> rparen;
    NodeList<StatementNode> statements;
};

class ElseGroupNode : public SyntaxNode
{
public:
    ElseGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetHash(HashNode* hash_);
    HashNode* Hash() const { return hash.get(); }
    void SetElseKeyword(ElseKeywordNode* elseKeyword_);
    ElseKeywordNode* ElseKeyword() const { return elseKeyword.get(); }
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>* Statements() { return &statements; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<HashNode> hash;
    std::unique_ptr<ElseKeywordNode> elseKeyword;
    NodeList<StatementNode> statements;
};

class CondCompStatementNode : public StatementNode
{
public:
    CondCompStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetIfGroup(IfGroupNode* ifGroup_);
    IfGroupNode* IfGroup() const { return ifGroup.get(); }
    void AddElifGroup(ElifGroupNode* elifGroup);
    NodeList<ElifGroupNode>* ElifGroups() { return &elifGroups; }
    void SetElseGroup(ElseGroupNode* elseGroup_);
    ElseGroupNode* ElseGroup() const { return elseGroup.get(); }
    void SetHash(HashNode* hash_);
    HashNode* Hash() const { return hash.get(); }
    void SetEndifKeyword(EndifKeywordNode* endifKeyword_);
    EndifKeywordNode* EndifKeyword() const { return endifKeyword.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IfGroupNode> ifGroup;
    NodeList<ElifGroupNode> elifGroups;
    std::unique_ptr<ElseGroupNode> elseGroup;
    std::unique_ptr<HashNode> hash;
    std::unique_ptr<EndifKeywordNode> endifKeyword;
};

class CompletionStatementNode : public StatementNode
{
public:
    CompletionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConditionCompletionNode : public SyntaxNode
{
public:
    ConditionCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ContainerCompletionNode : public SyntaxNode
{
public:
    ContainerCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ExpressionCompletionNode : public SyntaxNode
{
public:
    ExpressionCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

} // namespace cmajor::fault::tolerant::ast
