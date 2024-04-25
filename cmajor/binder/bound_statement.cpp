// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound.statement;

import cmajor.binder.bound.node.visitor;
import cmajor.binder.bound.expression;

namespace cmajor::binder {

BoundStatement::BoundStatement(const soul::ast::Span& span_, BoundNodeType boundNodeType_) :
    BoundNode(span_, boundNodeType_), flags(BoundStatementFlags::none)
{
}

BoundCompoundStatement* BoundStatement::Block()
{
    if (GetBoundNodeType() == BoundNodeType::boundCompoundStatement)
    {
        return static_cast<BoundCompoundStatement*>(this);
    }
    return StatementParent()->Block();
}

BoundStatement* BoundStatement::StatementParent() const
{
    BoundNode* parent = Parent();
    if (parent->IsBoundStatement())
    {
        return static_cast<BoundStatement*>(parent);
    }
    else
    {
        throw cmajor::symbols::Exception("statement parent expected", GetFullSpan());
    }
}

bool BoundStatement::IsConditionalStatementInBlock(BoundCompoundStatement* block) const
{
    if (this == block)
    {
        return false;
    }
    if (GetBoundNodeType() == BoundNodeType::boundIfStatement || GetBoundNodeType() == BoundNodeType::boundSwitchStatement) return true;
    BoundStatement* parent = StatementParent();
    if (parent)
    {
        return parent->IsConditionalStatementInBlock(block);
    }
    return false;
}

void BoundStatement::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from statement", GetFullSpan());
}

void BoundStatement::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to statement", GetFullSpan());
}

void BoundStatement::SetLabel(const std::u32string& label_)
{
    label = label_;
}

BoundSequenceStatement::BoundSequenceStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& first_, std::unique_ptr<BoundStatement>&& second_) :
    BoundStatement(span_, BoundNodeType::boundSequenceStatement), first(std::move(first_)), second(std::move(second_))
{
    first->SetParent(this);
    second->SetParent(this);
}

void BoundSequenceStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundSequenceStatement::SetForLoopStatementNode()
{
    first->SetForLoopStatementNode();
}

BoundCompoundStatement::BoundCompoundStatement(const soul::ast::Span& span_) : 
    BoundStatement(span_, BoundNodeType::boundCompoundStatement), endSpan(), destroyed(false)
{
}

BoundCompoundStatement::BoundCompoundStatement(const soul::ast::Span& span_, const soul::ast::Span& endSpan_) :
    BoundStatement(span_, BoundNodeType::boundCompoundStatement), endSpan(endSpan_), destroyed(false)
{
}

void BoundCompoundStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundCompoundStatement::InsertStatementToFront(std::unique_ptr<BoundStatement>&& statement)
{
    statement->SetParent(this);
    statements.insert(statements.begin(), std::move(statement));
}

void BoundCompoundStatement::AddStatement(std::unique_ptr<BoundStatement>&& statement)
{
    statement->SetParent(this);
    statements.push_back(std::move(statement));
}

BoundReturnStatement::BoundReturnStatement(std::unique_ptr<BoundFunctionCall>&& returnFunctionCall_, const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundReturnStatement), returnFunctionCall(std::move(returnFunctionCall_))
{
    if (returnFunctionCall)
    {
        returnFunctionCall->SetParent(this);
    }
}

void BoundReturnStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundIfStatement::BoundIfStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_, std::unique_ptr<BoundStatement>&& thenS_, 
    std::unique_ptr<BoundStatement>&& elseS_) :
    BoundStatement(span_, BoundNodeType::boundIfStatement), condition(std::move(condition_)), thenS(std::move(thenS_)), elseS(std::move(elseS_))
{
    condition->SetParent(this);
    thenS->SetParent(this);
    if (elseS)
    {
        elseS->SetParent(this);
    }
}

void BoundIfStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundWhileStatement::BoundWhileStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_, std::unique_ptr<BoundStatement>&& statement_) :
    BoundStatement(span_, BoundNodeType::boundWhileStatement), condition(std::move(condition_)), statement(std::move(statement_))
{
    condition->SetParent(this);
    statement->SetParent(this);
}

void BoundWhileStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundDoStatement::BoundDoStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& statement_, std::unique_ptr<BoundExpression>&& condition_) :
    BoundStatement(span_, BoundNodeType::boundDoStatement), statement(std::move(statement_)), condition(std::move(condition_))
{
    statement->SetParent(this);
    condition->SetParent(this);
}

void BoundDoStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundForStatement::BoundForStatement(const soul::ast::Span& span_, std::unique_ptr<BoundStatement>&& initS_, std::unique_ptr<BoundExpression>&& condition_, 
    std::unique_ptr<BoundStatement>&& loopS_,
    std::unique_ptr<BoundStatement>&& actionS_) : 
    BoundStatement(span_, BoundNodeType::boundForStatement), initS(std::move(initS_)), condition(std::move(condition_)), loopS(std::move(loopS_)),
    actionS(std::move(actionS_))
{
    initS->SetParent(this);
    condition->SetParent(this);
    loopS->SetParent(this);
    actionS->SetParent(this);
}

void BoundForStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundSwitchStatement::BoundSwitchStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& condition_) :
    BoundStatement(span_, BoundNodeType::boundSwitchStatement), condition(std::move(condition_))
{
    condition->SetParent(this);
}

void BoundSwitchStatement::AddCaseStatement(std::unique_ptr<BoundCaseStatement>&& caseStatement)
{
    caseStatement->SetParent(this);
    caseStatements.push_back(std::move(caseStatement));
}

void BoundSwitchStatement::SetDefaultStatement(std::unique_ptr<BoundDefaultStatement>&& defaultStatement_)
{
    defaultStatement = std::move(defaultStatement_);
    defaultStatement->SetParent(this);
}

void BoundSwitchStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundCaseStatement::BoundCaseStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundCaseStatement)
{
}

void BoundCaseStatement::AddCaseValue(std::unique_ptr<cmajor::symbols::Value>&& caseValue)
{
    caseValues.push_back(std::move(caseValue));
}

void BoundCaseStatement::AddStatement(std::unique_ptr<BoundStatement>&& statement)
{
    if (compoundStatement)
    {
        compoundStatement->AddStatement(std::move(statement));
    }
    else if (statement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement)
    {
        compoundStatement.reset(static_cast<BoundCompoundStatement*>(statement.release()));
        compoundStatement->SetParent(this);
    }
    else
    {
        compoundStatement.reset(new BoundCompoundStatement(GetSpan()));
        compoundStatement->SetParent(this);
        compoundStatement->AddStatement(std::move(statement));
    }
}

void BoundCaseStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundDefaultStatement::BoundDefaultStatement(const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundDefaultStatement)
{
}

void BoundDefaultStatement::AddStatement(std::unique_ptr<BoundStatement>&& statement)
{
    if (compoundStatement)
    {
        compoundStatement->AddStatement(std::move(statement));
    }
    else if (statement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement)
    {
        compoundStatement.reset(static_cast<BoundCompoundStatement*>(statement.release()));
        compoundStatement->SetParent(this);
    }
    else
    {
        compoundStatement.reset(new BoundCompoundStatement(GetSpan()));
        compoundStatement->SetParent(this);
        compoundStatement->AddStatement(std::move(statement));
    }
}

void BoundDefaultStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundGotoCaseStatement::BoundGotoCaseStatement(const soul::ast::Span& span_, std::unique_ptr<cmajor::symbols::Value>&& caseValue_) :
    BoundStatement(span_, BoundNodeType::boundGotoCaseStatement), caseValue(std::move(caseValue_))
{
}

void BoundGotoCaseStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundGotoDefaultStatement::BoundGotoDefaultStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundGotoDefaultStatement)
{
}

void BoundGotoDefaultStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundBreakStatement::BoundBreakStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundBreakStatement)
{
}

void BoundBreakStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundContinueStatement::BoundContinueStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundContinueStatement)
{
}

void BoundContinueStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundGotoStatement::BoundGotoStatement(const soul::ast::Span& span_, const std::u32string& target_) :
    BoundStatement(span_, BoundNodeType::boundGotoStatement), target(target_), targetStatement(nullptr), targetBlock(nullptr)
{
}

void BoundGotoStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundConstructionStatement::BoundConstructionStatement(std::unique_ptr<BoundFunctionCall>&& constructorCall_, const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundConstructionStatement), constructorCall(std::move(constructorCall_)), localVariable(nullptr)
{
    constructorCall->SetParent(this);
}

void BoundConstructionStatement::SetLocalVariable(cmajor::symbols::LocalVariableSymbol* localVariable_)
{
    localVariable = localVariable_;
}

void BoundConstructionStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundAssignmentStatement::BoundAssignmentStatement(std::unique_ptr<BoundFunctionCall>&& assignmentCall_, const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundAssignmentStatement), assignmentCall(std::move(assignmentCall_))
{
    assignmentCall->SetParent(this);
}

void BoundAssignmentStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionStatement::BoundExpressionStatement(std::unique_ptr<BoundExpression>&& expression_, const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundExpressionStatement), expression(std::move(expression_))
{
    if (expression)
    {
        expression->SetParent(this);
    }
}

void BoundExpressionStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundInitializationStatement::BoundInitializationStatement(std::unique_ptr<BoundExpression>&& initializationExpression_) :
    BoundStatement(initializationExpression_->GetSpan(), BoundNodeType::boundInitializationStatement),
    initializationExpression(std::move(initializationExpression_))
{
    initializationExpression->SetParent(this);
}

void BoundInitializationStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundEmptyStatement::BoundEmptyStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundEmptyStatement)
{
}

void BoundEmptyStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundSetVmtPtrStatement::BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::ClassTypeSymbol* classType_) :
    BoundStatement(classPtr_->GetSpan(), BoundNodeType::boundSetVmtPtrStatement), classPtr(std::move(classPtr_)), classType(classType_)
{
    classPtr->SetParent(this);
}

void BoundSetVmtPtrStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundThrowStatement::BoundThrowStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& throwCallExpr_) :
    BoundStatement(span_, BoundNodeType::boundThrowStatement), throwCallExpr(std::move(throwCallExpr_))
{
    throwCallExpr->SetParent(this);
}

void BoundThrowStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundRethrowStatement::BoundRethrowStatement(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& releaseCall_) :
    BoundStatement(span_, BoundNodeType::boundRethrowStatement), releaseCall(std::move(releaseCall_))
{
    if (releaseCall)
    {
        releaseCall->SetParent(this);
    }
}

void BoundRethrowStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundTryStatement::BoundTryStatement(const soul::ast::Span& span_) : BoundStatement(span_, BoundNodeType::boundTryStatement)
{
}

void BoundTryStatement::SetTryBlock(std::unique_ptr<BoundStatement>&& tryBlock_)
{
    tryBlock = std::move(tryBlock_);
    tryBlock->SetParent(this);
}

void BoundTryStatement::AddCatch(std::unique_ptr<BoundCatchStatement>&& catchStatement)
{
    catchStatement->SetParent(this);
    catches.push_back(std::move(catchStatement));
}

void BoundTryStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundCatchStatement::BoundCatchStatement(const soul::ast::Span& span_) :
    BoundStatement(span_, BoundNodeType::boundCatchStatement), caughtType(nullptr), catchTypeUuidId(-1), catchVar(nullptr)
{
}

void BoundCatchStatement::SetCatchBlock(std::unique_ptr<BoundStatement>&& catchBlock_)
{
    catchBlock = std::move(catchBlock_);
    catchBlock->SetParent(this);
}

void BoundCatchStatement::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::binder
