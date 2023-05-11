// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.statement;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class IdentifierNode;

class LabelNode : public Node
{
public:
    LabelNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LabelNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& label_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::u32string& Label() const { return label; }
private:
    std::u32string label;
};

class StatementNode : public Node
{
public:
    StatementNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsStatementNode() const override { return true; }
    virtual bool IsFunctionTerminatingNode() const { return false; }
    virtual bool IsCaseTerminatingNode() const { return false; }
    virtual bool IsDefaultTerminatingNode() const { return false; }
    virtual bool IsBreakEnclosingStatementNode() const { return false; }
    virtual bool IsContinueEnclosingStatementNode() const { return false; }
};

class LabeledStatementNode : public StatementNode
{
public:
    LabeledStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LabeledStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, StatementNode* stmt_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const LabelNode* Label() const { return labelNode.get(); }
    LabelNode* Label() { return labelNode.get(); }
    void SetLabelNode(LabelNode* labelNode_);
    StatementNode* Stmt() { return stmt.get(); }
private:
    std::unique_ptr<LabelNode> labelNode;
    std::unique_ptr<StatementNode> stmt;
};

class SyncStatementNode : public StatementNode
{
public:
    SyncStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class CompoundStatementNode : public StatementNode
{
public:
    CompoundStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddStatement(StatementNode* statement);
    NodeList<StatementNode>& Statements() { return statements; }
    bool TracerInserted() const { return tracerInserted; }
    void SetTracerInserted() { tracerInserted = true; }
    int Level() const;
private:
    NodeList<StatementNode> statements;
    bool tracerInserted;
};

class ReturnStatementNode : public StatementNode
{
public:
    ReturnStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ReturnStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsFunctionTerminatingNode() const override { return true; }
    bool IsCaseTerminatingNode() const override { return true; }
    bool IsDefaultTerminatingNode() const override { return true; }
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
private:
    std::unique_ptr<Node> expression;
};

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    IfStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* condition_, StatementNode* thenS_, StatementNode* elseS_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Condition() const { return condition.get(); }
    Node* Condition() { return condition.get(); }
    const StatementNode* ThenS() const { return thenS.get(); }
    StatementNode* ThenS() { return thenS.get(); }
    const StatementNode* ElseS() const { return elseS.get(); }
    StatementNode* ElseS() { return elseS.get(); }
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> thenS;
    std::unique_ptr<StatementNode> elseS;
};

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    WhileStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* condition_, StatementNode* statement_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsBreakEnclosingStatementNode() const override { return true; }
    bool IsContinueEnclosingStatementNode() const override { return true; }
    const Node* Condition() const { return condition.get(); }
    Node* Condition() { return condition.get(); }
    const StatementNode* Statement() const { return statement.get(); }
    StatementNode* Statement() { return statement.get(); }
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> statement;
};

class DoStatementNode : public StatementNode
{
public:
    DoStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DoStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, StatementNode* statement_, Node* condition_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsBreakEnclosingStatementNode() const override { return true; }
    bool IsContinueEnclosingStatementNode() const override { return true; }
    const StatementNode* Statement() const { return statement.get(); }
    StatementNode* Statement() { return statement.get(); }
    const Node* Condition() const { return condition.get(); }
    Node* Condition() { return condition.get(); }
private:
    std::unique_ptr<StatementNode> statement;
    std::unique_ptr<Node> condition;
};

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ForStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, StatementNode* initS_, Node* condition_, StatementNode* loopS_, StatementNode* actionS_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsBreakEnclosingStatementNode() const override { return true; }
    bool IsContinueEnclosingStatementNode() const override { return true; }
    const StatementNode* InitS() const { return initS.get(); }
    StatementNode* InitS() { return initS.get(); }
    const Node* Condition() const { return condition.get(); }
    Node* Condition() { return condition.get(); }
    const StatementNode* LoopS() const { return loopS.get(); }
    StatementNode* LoopS() { return loopS.get(); }
    const StatementNode* ActionS() const { return actionS.get(); }
    StatementNode* ActionS() { return actionS.get(); }
private:
    std::unique_ptr<StatementNode> initS;
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> loopS;
    std::unique_ptr<StatementNode> actionS;
};

class BreakStatementNode : public StatementNode
{
public:
    BreakStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsCaseTerminatingNode() const override { return true; }
    bool IsDefaultTerminatingNode() const override { return true; }
};

class ContinueStatementNode : public StatementNode
{
public:
    ContinueStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsCaseTerminatingNode() const override { return true; }
    bool IsDefaultTerminatingNode() const override { return true; }
};

class GotoStatementNode : public StatementNode
{
public:
    GotoStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    GotoStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& target_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::u32string& Target() const { return target; }
private:
    std::u32string target;
};

class ConstructionStatementNode : public StatementNode
{
public:
    ConstructionStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConstructionStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddArgument(Node* argument) override;
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const NodeList<Node>& Arguments() const { return arguments; }
    void SetAssignment() { assignment = true; }
    bool Assignment() const { return assignment; }
    void SetEmpty() { empty = true; }
    bool Empty() const { return empty; }
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    NodeList<Node> arguments;
    bool assignment;
    bool empty;
};

class DeleteStatementNode : public StatementNode
{
public:
    DeleteStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DeleteStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
private:
    std::unique_ptr<Node> expression;
};

class DestroyStatementNode : public StatementNode
{
public:
    DestroyStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DestroyStatementNode(const soul::ast::SourcePos&, const util::uuid& moduleId_sourcePos_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
private:
    std::unique_ptr<Node> expression;
};

class AssignmentStatementNode : public StatementNode
{
public:
    AssignmentStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AssignmentStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* targetExpr_, Node* sourceExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* TargetExpr() const { return targetExpr.get(); }
    Node* TargetExpr() { return targetExpr.get(); }
    const Node* SourceExpr() const { return sourceExpr.get(); }
    Node* SourceExpr() { return sourceExpr.get(); }
private:
    std::unique_ptr<Node> targetExpr;
    std::unique_ptr<Node> sourceExpr;
};

class ExpressionStatementNode : public StatementNode
{
public:
    ExpressionStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ExpressionStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
private:
    std::unique_ptr<Node> expression;
};

class EmptyStatementNode : public StatementNode
{
public:
    EmptyStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class RangeForStatementNode : public StatementNode
{
public:
    RangeForStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    RangeForStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_, Node* container_, StatementNode* action_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsBreakEnclosingStatementNode() const override { return true; }
    bool IsContinueEnclosingStatementNode() const override { return true; }
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const Node* Container() const { return container.get(); }
    Node* Container() { return container.get(); }
    const StatementNode* Action() const { return action.get(); }
    StatementNode* Action() { return action.get(); }
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> container;
    std::unique_ptr<StatementNode> action;
};

class CaseStatementNode;
class DefaultStatementNode;

class SwitchStatementNode : public StatementNode
{
public:
    SwitchStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    SwitchStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* condition_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsBreakEnclosingStatementNode() const override { return true; }
    const Node* Condition() const { return condition.get(); }
    Node* Condition() { return condition.get(); }
    void AddCase(CaseStatementNode* caseS);
    const NodeList<CaseStatementNode>& Cases() const { return cases; }
    void SetDefault(DefaultStatementNode* defaultS_);
    const DefaultStatementNode* Default() const { return defaultS.get(); }
    DefaultStatementNode* Default() { return defaultS.get(); }
private:
    std::unique_ptr<Node> condition;
    NodeList<CaseStatementNode> cases;
    std::unique_ptr<DefaultStatementNode> defaultS;
};

class CaseStatementNode : public StatementNode
{
public:
    CaseStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddCaseExpr(Node* caseExpr);
    const NodeList<Node>& CaseExprs() const { return caseExprs; }
    void AddStatement(StatementNode* statement);
    const NodeList<StatementNode>& Statements() const { return statements; }
private:
    NodeList<Node> caseExprs;
    NodeList<StatementNode> statements;
};

class DefaultStatementNode : public StatementNode
{
public:
    DefaultStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const NodeList<StatementNode>& Statements() const { return statements; }
    void AddStatement(StatementNode* statement);
private:
    NodeList<StatementNode> statements;
};

class GotoCaseStatementNode : public StatementNode
{
public:
    GotoCaseStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    GotoCaseStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* caseExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsCaseTerminatingNode() const override { return true; }
    bool IsDefaultTerminatingNode() const override { return true; }
    const Node* CaseExpr() const { return caseExpr.get(); }
    Node* CaseExpr() { return caseExpr.get(); }
private:
    std::unique_ptr<Node> caseExpr;
};

class GotoDefaultStatementNode : public StatementNode
{
public:
    GotoDefaultStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsCaseTerminatingNode() const override { return true; }
};

class ThrowStatementNode : public StatementNode
{
public:
    ThrowStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ThrowStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsFunctionTerminatingNode() const override { return true; }
    bool IsCaseTerminatingNode() const override { return true; }
    bool IsDefaultTerminatingNode() const override { return true; }
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
private:
    std::unique_ptr<Node> expression;
};

class CatchNode;

class TryStatementNode : public StatementNode
{
public:
    TryStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TryStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, CompoundStatementNode* tryBlock_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const CompoundStatementNode* TryBlock() const { return tryBlock.get(); }
    CompoundStatementNode* TryBlock() { return tryBlock.get(); }
    const NodeList<CatchNode>& Catches() const { return catches; }
    void AddCatch(CatchNode* catch_);
private:
    std::unique_ptr<CompoundStatementNode> tryBlock;
    NodeList<CatchNode> catches;
};

class CatchNode : public Node
{
public:
    CatchNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    CatchNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_, CompoundStatementNode* catchBlock_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const CompoundStatementNode* CatchBlock() const { return catchBlock.get(); }
    CompoundStatementNode* CatchBlock() { return catchBlock.get(); }
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<CompoundStatementNode> catchBlock;
};

class AssertStatementNode : public StatementNode
{
public:
    AssertStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AssertStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* assertExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* AssertExpr() const { return assertExpr.get(); }
    Node* AssertExpr() { return assertExpr.get(); }
private:
    std::unique_ptr<Node> assertExpr;
};

class ConditionalCompilationExpressionNode : public Node
{
public:
    ConditionalCompilationExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsConditionalCompilationExpressionNode() const override { return true; }
};

class ConditionalCompilationBinaryExpressionNode : public ConditionalCompilationExpressionNode
{
public:
    ConditionalCompilationBinaryExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationBinaryExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    ConditionalCompilationExpressionNode* Left() const { return left.get(); }
    ConditionalCompilationExpressionNode* Right() const { return right.get(); }
private:
    std::unique_ptr<ConditionalCompilationExpressionNode> left;
    std::unique_ptr<ConditionalCompilationExpressionNode> right;
};

class ConditionalCompilationDisjunctionNode : public ConditionalCompilationBinaryExpressionNode
{
public:
    ConditionalCompilationDisjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationDisjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class ConditionalCompilationConjunctionNode : public ConditionalCompilationBinaryExpressionNode
{
public:
    ConditionalCompilationConjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationConjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

class ConditionalCompilationNotNode : public ConditionalCompilationExpressionNode
{
public:
    ConditionalCompilationNotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationNotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    ConditionalCompilationExpressionNode* Expr() const { return expr.get(); }
private:
    std::unique_ptr<ConditionalCompilationExpressionNode> expr;
};

class ConditionalCompilationPrimaryNode : public ConditionalCompilationExpressionNode
{
public:
    ConditionalCompilationPrimaryNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationPrimaryNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& symbol_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::u32string& Symbol() const { return symbol; }
private:
    std::u32string symbol;
};

class ParenthesizedConditionalCompilationExpressionNode : public ConditionalCompilationExpressionNode
{
public:
    ParenthesizedConditionalCompilationExpressionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ParenthesizedConditionalCompilationExpressionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    ConditionalCompilationExpressionNode* Expr() const { return expr.get(); }
private:
    std::unique_ptr<ConditionalCompilationExpressionNode> expr;
};

class cmajor::ast::ConditionalCompilationPartNode : public Node
{
public:
    cmajor::ast::ConditionalCompilationPartNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    cmajor::ast::ConditionalCompilationPartNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_);
    void AddStatement(StatementNode* statement);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    ConditionalCompilationExpressionNode* Expr() const { return expr.get(); }
    const NodeList<StatementNode>& Statements() const { return statements; }
private:
    std::unique_ptr<ConditionalCompilationExpressionNode> expr;
    NodeList<StatementNode> statements;
};

class ConditionalCompilationStatementNode : public StatementNode
{
public:
    ConditionalCompilationStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConditionalCompilationStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* ifExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddIfStatement(StatementNode* statement);
    void AddElifExpr(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, ConditionalCompilationExpressionNode* expr);
    void AddElifStatement(StatementNode* statement);
    void AddElseStatement(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, StatementNode* statement);
    cmajor::ast::ConditionalCompilationPartNode* IfPart() { return ifPart.get(); }
    const NodeList<cmajor::ast::ConditionalCompilationPartNode>& ElifParts() const { return elifParts; }
    cmajor::ast::ConditionalCompilationPartNode* ElsePart() { return elsePart.get(); }
    void SetIfPart(cmajor::ast::ConditionalCompilationPartNode* ifPart_);
    void AddElifPart(cmajor::ast::ConditionalCompilationPartNode* elifPart);
    void SetElsePart(cmajor::ast::ConditionalCompilationPartNode* elsePart_);
private:
    std::unique_ptr<cmajor::ast::ConditionalCompilationPartNode> ifPart;
    NodeList<cmajor::ast::ConditionalCompilationPartNode> elifParts;
    std::unique_ptr<cmajor::ast::ConditionalCompilationPartNode> elsePart;
};

} // namespace cmajor::ast
