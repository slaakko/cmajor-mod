// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.statement;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import cmajor.binder.bound.node;

export namespace cmajor::binder {

class BoundFunctionCall;
class BoundExpression;
class BoundCompoundStatement;

enum class BoundStatementFlags : uint8_t
{
    none = 0,
    postfix = 1 << 0,
    generated = 1 << 1,
    forLoopStatementNode = 1 << 2,
    ignoreNode = 1 << 3,
    assertNode = 1 << 4
};

inline BoundStatementFlags operator|(BoundStatementFlags left, BoundStatementFlags right)
{
    return BoundStatementFlags(uint8_t(left) | uint8_t(right));
}

inline BoundStatementFlags operator&(BoundStatementFlags left, BoundStatementFlags right)
{
    return BoundStatementFlags(uint8_t(left) & uint8_t(right));
}

class BoundStatement : public BoundNode
{
public:
    BoundStatement(const soul::ast::Span& span_, BoundNodeType boundNodeType_);
    BoundStatement(const BoundStatement&) = delete;
    BoundStatement& operator=(const BoundStatement&) = delete;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    bool IsBoundStatement() const override { return true; }
    BoundStatement* StatementParent() const;
    BoundCompoundStatement* Block();
    void SetLabel(const std::u32string& label_);
    const std::u32string& Label() const { return label; }
    void SetPostfix() { SetFlag(BoundStatementFlags::postfix); }
    bool Postfix() const { return GetFlag(BoundStatementFlags::postfix); }
    void SetGenerated() { SetFlag(BoundStatementFlags::generated); }
    bool Generated() const { return GetFlag(BoundStatementFlags::generated); }
    bool IsForLoopStatementNode() const { return GetFlag(BoundStatementFlags::forLoopStatementNode); }
    virtual void SetForLoopStatementNode() { SetFlag(BoundStatementFlags::forLoopStatementNode); }
    virtual bool IsOrContainsBoundReturnStatement() const { return false; }
    bool IgnoreNode() const { return GetFlag(BoundStatementFlags::ignoreNode); }
    void SetIgnoreNode() { SetFlag(BoundStatementFlags::ignoreNode); }
    bool IsAssertNode() const { return GetFlag(BoundStatementFlags::assertNode); }
    void SetAssertNode() { SetFlag(BoundStatementFlags::assertNode); }
private:
    std::u32string label;
    BoundStatementFlags flags;
    bool GetFlag(BoundStatementFlags flag) const { return (flags & flag) != BoundStatementFlags::none; }
    void SetFlag(BoundStatementFlags flag) { flags = flags | flag; }
};

class BoundSequenceStatement : public BoundStatement
{
public:
    BoundSequenceStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& first_, std::unique_ptr<BoundStatement>&& second_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundStatement* First() { return first.get(); }
    BoundStatement* Second() { return second.get(); }
    bool IsOrContainsBoundReturnStatement() const override { return second->IsOrContainsBoundReturnStatement(); }
    void SetForLoopStatementNode() override;
private:
    std::unique_ptr<BoundStatement> first;
    std::unique_ptr<BoundStatement> second;
};

class BoundCompoundStatement : public BoundStatement
{
public:
    BoundCompoundStatement(const soul::ast::Span& span_);
    BoundCompoundStatement(const soul::ast::Span& span_, const soul::ast::Span& endSpan_);
    BoundCompoundStatement(const BoundCompoundStatement&) = delete;
    BoundCompoundStatement& operator=(const BoundCompoundStatement&) = delete;
    void Accept(BoundNodeVisitor& visitor) override;
    void InsertStatementToFront(std::unique_ptr<BoundStatement>&& statement);
    void AddStatement(std::unique_ptr<BoundStatement>&& statement);
    const std::vector<std::unique_ptr<BoundStatement>>& Statements() const { return statements; }
    const soul::ast::Span& EndSpan() const { return endSpan; }
private:
    std::vector<std::unique_ptr<BoundStatement>> statements;
    soul::ast::Span endSpan;
};

class BoundReturnStatement : public BoundStatement
{
public:
    BoundReturnStatement(std::unique_ptr<BoundFunctionCall>&& returnFunctionCall_, const soul::ast::Span& span_);
    BoundReturnStatement(const BoundReturnStatement&) = delete;
    BoundReturnStatement& operator=(const BoundReturnStatement&) = delete;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundFunctionCall* ReturnFunctionCall() { return returnFunctionCall.get(); }
    bool IsOrContainsBoundReturnStatement() const override { return true; }
private:
    std::unique_ptr<BoundFunctionCall> returnFunctionCall;
};

class BoundIfStatement : public BoundStatement
{
public:
    BoundIfStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_, 
        std::unique_ptr<BoundStatement>&& thenS_, std::unique_ptr<BoundStatement>&& elseS_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* Condition() { return condition.get(); }
    BoundStatement* ThenS() { return thenS.get(); }
    BoundStatement* ElseS() { return elseS.get(); }
private:
    std::unique_ptr<BoundExpression> condition;
    std::unique_ptr<BoundStatement> thenS;
    std::unique_ptr<BoundStatement> elseS;
};

class BoundWhileStatement : public BoundStatement
{
public:
    BoundWhileStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_, 
        std::unique_ptr<BoundStatement>&& statement_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* Condition() { return condition.get(); }
    BoundStatement* Statement() { return statement.get(); }
private:
    std::unique_ptr<BoundExpression> condition;
    std::unique_ptr<BoundStatement> statement;
};

class BoundDoStatement : public BoundStatement
{
public:
    BoundDoStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& statement_, 
        std::unique_ptr<BoundExpression>&& condition_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundStatement* Statement() { return statement.get(); }
    BoundExpression* Condition() { return condition.get(); }
private:
    std::unique_ptr<BoundStatement> statement;
    std::unique_ptr<BoundExpression> condition;
};

class BoundForStatement : public BoundStatement
{
public:
    BoundForStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& initS_, 
        std::unique_ptr<BoundExpression>&& condition_, std::unique_ptr<BoundStatement>&& loopS_, std::unique_ptr<BoundStatement>&& actionS_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundStatement* InitS() { return initS.get(); }
    BoundExpression* Condition() { return condition.get(); }
    BoundStatement* LoopS() { return loopS.get(); }
    BoundStatement* ActionS() { return actionS.get(); }
private:
    std::unique_ptr<BoundStatement> initS;
    std::unique_ptr<BoundExpression> condition;
    std::unique_ptr<BoundStatement> loopS;
    std::unique_ptr<BoundStatement> actionS;
};

class BoundCaseStatement;
class BoundDefaultStatement;

class BoundSwitchStatement : public BoundStatement
{
public:
    BoundSwitchStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_);
    BoundExpression* Condition() { return condition.get(); }
    const std::vector<std::unique_ptr<BoundCaseStatement>>& CaseStatements() { return caseStatements; }
    void AddCaseStatement(std::unique_ptr<BoundCaseStatement>&& caseStatement);
    BoundDefaultStatement* DefaultStatement() { return defaultStatement.get(); }
    void SetDefaultStatement(std::unique_ptr<BoundDefaultStatement>&& defaultStatement_);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpression> condition;
    std::vector<std::unique_ptr<BoundCaseStatement>> caseStatements;
    std::unique_ptr<BoundDefaultStatement> defaultStatement;
};

class BoundCaseStatement : public BoundStatement
{
public:
    BoundCaseStatement(const soul::ast::Span& span_);
    void AddCaseValue(std::unique_ptr<cmajor::symbols::Value>&& caseValue_);
    const std::vector<std::unique_ptr<cmajor::symbols::Value>>& CaseValues() const { return caseValues; }
    void AddStatement(std::unique_ptr<BoundStatement>&& statement);
    BoundCompoundStatement* CompoundStatement() { return compoundStatement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<cmajor::symbols::Value>> caseValues;
    std::unique_ptr<BoundCompoundStatement> compoundStatement;
};

class BoundDefaultStatement : public BoundStatement
{
public:
    BoundDefaultStatement(const soul::ast::Span& span_);
    void AddStatement(std::unique_ptr<BoundStatement>&& statement);
    BoundCompoundStatement* CompoundStatement() { return compoundStatement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundCompoundStatement> compoundStatement;
};

class BoundGotoCaseStatement : public BoundStatement
{
public:
    BoundGotoCaseStatement(const soul::ast::Span& span_, std::unique_ptr<cmajor::symbols::Value>&& caseValue_);
    void Accept(BoundNodeVisitor& visitor) override;
    cmajor::symbols::Value* CaseValue() { return caseValue.get(); }
private:
    std::unique_ptr<cmajor::symbols::Value> caseValue;
};

class BoundGotoDefaultStatement : public BoundStatement
{
public:
    BoundGotoDefaultStatement(const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundBreakStatement : public BoundStatement
{
public:
    BoundBreakStatement(const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundContinueStatement : public BoundStatement
{
public:
    BoundContinueStatement(const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundGotoStatement : public BoundStatement
{
public:
    BoundGotoStatement(const soul::ast::Span& span_, const std::u32string& target_);
    void Accept(BoundNodeVisitor& visitor) override;
    const std::u32string& Target() const { return target; }
    void SetTargetStatement(BoundStatement* targetStatement_) { targetStatement = targetStatement_; }
    BoundStatement* TargetStatement() { return targetStatement; }
    void SetTargetBlock(BoundCompoundStatement* targetBlock_) { targetBlock = targetBlock_; }
    BoundCompoundStatement* TargetBlock() { return targetBlock; }
private:
    std::u32string target;
    BoundStatement* targetStatement;
    BoundCompoundStatement* targetBlock;
};

class BoundConstructionStatement : public BoundStatement
{
public:
    BoundConstructionStatement(std::unique_ptr<BoundFunctionCall>&& constructorCall_, const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundFunctionCall* ConstructorCall() { return constructorCall.get(); }
    void SetLocalVariable(cmajor::symbols::LocalVariableSymbol* localVariable_);
    cmajor::symbols::LocalVariableSymbol* GetLocalVariable() const { return localVariable; }
private:
    std::unique_ptr<BoundFunctionCall> constructorCall;
    cmajor::symbols::LocalVariableSymbol* localVariable;
};

class BoundAssignmentStatement : public BoundStatement
{
public:
    BoundAssignmentStatement(std::unique_ptr<BoundFunctionCall>&& assignmentCall_, const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundFunctionCall* AssignmentCall() { return assignmentCall.get(); }
private:
    std::unique_ptr<BoundFunctionCall> assignmentCall;
};

class BoundExpressionStatement : public BoundStatement
{
public:
    BoundExpressionStatement(std::unique_ptr<BoundExpression>&& expression_, const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* Expression() { return expression.get(); }
private:
    std::unique_ptr<BoundExpression> expression;
};

class BoundInitializationStatement : public BoundStatement
{
public:
    BoundInitializationStatement(std::unique_ptr<BoundExpression>&& initializationExpression_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* InitializationExpression() { return initializationExpression.get(); }
private:
    std::unique_ptr<BoundExpression> initializationExpression;
};

class BoundEmptyStatement : public BoundStatement
{
public:
    BoundEmptyStatement(const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundSetVmtPtrStatement : public BoundStatement
{
public:
    BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::ClassTypeSymbol* classType_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* ClassPtr() { return classPtr.get(); }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
private:
    std::unique_ptr<BoundExpression> classPtr;
    cmajor::symbols::ClassTypeSymbol* classType;
};

class BoundThrowStatement : public BoundStatement
{
public:
    BoundThrowStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& throwCallExpr_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* ThrowCallExpr() { return throwCallExpr.get(); }
private:
    std::unique_ptr<BoundExpression> throwCallExpr;
};

class BoundRethrowStatement : public BoundStatement
{
public:
    BoundRethrowStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& releaseCall_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpression* ReleaseCall() { return releaseCall.get(); }
private:
    std::unique_ptr<BoundExpression> releaseCall;
};

class BoundCatchStatement;

class BoundTryStatement : public BoundStatement
{
public:
    BoundTryStatement(const soul::ast::Span& span_);
    void SetTryBlock(std::unique_ptr<BoundStatement>&& tryBlock_);
    BoundStatement* TryBlock() { return tryBlock.get(); }
    void AddCatch(std::unique_ptr<BoundCatchStatement>&& catchStatement);
    const std::vector<std::unique_ptr<BoundCatchStatement>>& Catches() const { return catches; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundStatement> tryBlock;
    std::vector<std::unique_ptr<BoundCatchStatement>> catches;
};

class BoundCatchStatement : public BoundStatement
{
public:
    BoundCatchStatement(const soul::ast::Span& span_);
    void SetCaughtType(cmajor::symbols::TypeSymbol* caughtType_) { caughtType = caughtType_; }
    cmajor::symbols::TypeSymbol* CaughtType() { return caughtType; }
    void SetCatchVar(cmajor::symbols::LocalVariableSymbol* catchVar_) { catchVar = catchVar_; }
    cmajor::symbols::LocalVariableSymbol* CatchVar() { return catchVar; }
    void SetCatchBlock(std::unique_ptr<BoundStatement>&& catchBlock_);
    BoundStatement* CatchBlock() { return catchBlock.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
    int CatchTypeUuidId() const { return catchTypeUuidId; }
    void SetCatchTypeUuidId(int catchTypeUuidId_) { catchTypeUuidId = catchTypeUuidId_; }
private:
    cmajor::symbols::TypeSymbol* caughtType;
    cmajor::symbols::LocalVariableSymbol* catchVar;
    std::unique_ptr<BoundStatement> catchBlock;
    int catchTypeUuidId;
};

} // namespace cmajor::binder
