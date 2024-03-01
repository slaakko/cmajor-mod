// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.statement;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

StatementNode::StatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

CompoundStatementNode::CompoundStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void CompoundStatementNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void CompoundStatementNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
    AddChildNode(statement);
}

void CompoundStatementNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* CompoundStatementNode::Clone() const
{
    CompoundStatementNode* clone = new CompoundStatementNode(GetSpan(), GetCompletionContext());
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void CompoundStatementNode::Make()
{
    statements.Make();
    AddChildNode(&statements);
    StatementNode::Make();
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ReturnStatementNode::ReturnStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void ReturnStatementNode::SetReturnKeyword(ReturnKeywordNode* returnKeyword_)
{
    returnKeyword.reset(returnKeyword_);
    AddChildNode(returnKeyword.get());
}

void ReturnStatementNode::SetReturnValue(Node* returnValue_)
{
    returnValue.reset(returnValue_);
    AddChildNode(returnValue.get());
}

void ReturnStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ReturnStatementNode::Clone() const
{
    ReturnStatementNode* clone = new ReturnStatementNode(GetSpan(), GetCompletionContext());
    clone->SetReturnKeyword(static_cast<ReturnKeywordNode*>(returnKeyword->Clone()));
    clone->SetReturnValue(returnValue->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ReturnStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IfStatementNode::IfStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void IfStatementNode::SetIfKeyword(IfKeywordNode* ifKeyword_)
{
    ifKeyword.reset(ifKeyword_);
    AddChildNode(ifKeyword.get());
}

void IfStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void IfStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    AddChildNode(condition.get());
}

void IfStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void IfStatementNode::SetThenStatement(StatementNode* thenS_)
{
    thenS.reset(thenS_);
    AddChildNode(thenS.get());
}

void IfStatementNode::SetElseKeyword(ElseKeywordNode* elseKeyword_)
{
    elseKeyword.reset(elseKeyword_);
    AddChildNode(elseKeyword.get());
}

void IfStatementNode::SetElseStatement(StatementNode* elseS_)
{
    elseS.reset(elseS_);
    AddChildNode(elseS.get());
}

Node* IfStatementNode::Clone() const
{
    IfStatementNode* clone = new IfStatementNode(GetSpan(), GetCompletionContext());
    clone->SetIfKeyword(static_cast<IfKeywordNode*>(ifKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetCondition(condition->Clone());
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetThenStatement(static_cast<StatementNode*>(thenS->Clone()));
    if (elseKeyword)
    {
        clone->SetElseKeyword(static_cast<ElseKeywordNode*>(elseKeyword->Clone()));
    }
    if (elseS)
    {
        clone->SetElseStatement(static_cast<StatementNode*>(elseS->Clone()));
    }
    clone->Make();
    return clone;
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WhileStatementNode::WhileStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void WhileStatementNode::SetWhileKeyword(WhileKeywordNode* whileKeyword_)
{
    whileKeyword.reset(whileKeyword_);
    AddChildNode(whileKeyword.get());
}

void WhileStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void WhileStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    AddChildNode(condition.get());
}

void WhileStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void WhileStatementNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    AddChildNode(statement.get());
}

Node* WhileStatementNode::Clone() const
{
    WhileStatementNode* clone = new WhileStatementNode(GetSpan(), GetCompletionContext());
    clone->SetWhileKeyword(static_cast<WhileKeywordNode*>(whileKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetCondition(condition->Clone());
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    clone->Make();
    return clone;
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoStatementNode::DoStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void DoStatementNode::SetDoKeyword(DoKeywordNode* doKeyword_)
{
    doKeyword.reset(doKeyword_);
    AddChildNode(doKeyword.get());
}

void DoStatementNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    AddChildNode(statement.get());
}

void DoStatementNode::SetWhileKeyword(WhileKeywordNode* whileKeyword_)
{
    whileKeyword.reset(whileKeyword_);
    AddChildNode(whileKeyword.get());
}

void DoStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void DoStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    AddChildNode(condition.get());
}

void DoStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void DoStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DoStatementNode::Clone() const
{
    DoStatementNode* clone = new DoStatementNode(GetSpan(), GetCompletionContext());
    clone->SetDoKeyword(static_cast<DoKeywordNode*>(doKeyword->Clone()));
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    clone->SetWhileKeyword(static_cast<WhileKeywordNode*>(whileKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetCondition(condition->Clone());
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void DoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RangeForStatementNode::RangeForStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void RangeForStatementNode::SetForKeyword(ForKeywordNode* forKeyword_)
{
    forKeyword.reset(forKeyword_);
    AddChildNode(forKeyword.get());
}

void RangeForStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void RangeForStatementNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void RangeForStatementNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void RangeForStatementNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void RangeForStatementNode::SetContainer(Node* container_)
{
    container.reset(container_);
    AddChildNode(container.get());
}

void RangeForStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void RangeForStatementNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    AddChildNode(statement.get());
}

Node* RangeForStatementNode::Clone() const
{
    RangeForStatementNode* clone = new RangeForStatementNode(GetSpan(), GetCompletionContext());
    clone->SetForKeyword(static_cast<ForKeywordNode*>(forKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetTypeExpr(typeExpr->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    clone->SetContainer(container->Clone());
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    clone->Make();
    return clone;
}

void RangeForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ForStatementNode::ForStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void ForStatementNode::SetForKeyword(ForKeywordNode* forKeyword_)
{
    forKeyword.reset(forKeyword_);
    AddChildNode(forKeyword.get());
}

void ForStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ForStatementNode::SetInitStatement(StatementNode* initStatement_)
{
    initStatement.reset(initStatement_);
    AddChildNode(initStatement.get());
}

void ForStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    AddChildNode(condition.get());
}

void ForStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

void ForStatementNode::SetLoopStatement(StatementNode* loopStatement_)
{
    loopStatement.reset(loopStatement_);
    AddChildNode(loopStatement.get());
}

void ForStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void ForStatementNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    AddChildNode(statement.get());
}

Node* ForStatementNode::Clone() const
{
    ForStatementNode* clone = new ForStatementNode(GetSpan(), GetCompletionContext());
    clone->SetForKeyword(static_cast<ForKeywordNode*>(forKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetInitStatement(static_cast<StatementNode*>(initStatement->Clone()));
    clone->SetCondition(condition->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->SetLoopStatement(static_cast<StatementNode*>(loopStatement->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    clone->Make();
    return clone;
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BreakStatementNode::BreakStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void BreakStatementNode::SetBreakKeyword(BreakKeywordNode* breakKeyword_)
{
    breakKeyword.reset(breakKeyword_);
    AddChildNode(breakKeyword.get());
}

void BreakStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* BreakStatementNode::Clone() const
{
    BreakStatementNode* clone = new BreakStatementNode(GetSpan(), GetCompletionContext());
    clone->SetBreakKeyword(static_cast<BreakKeywordNode*>(breakKeyword->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void BreakStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ContinueStatementNode::ContinueStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void ContinueStatementNode::SetContinueKeyword(ContinueKeywordNode* continueKeyword_)
{
    continueKeyword.reset(continueKeyword_);
    AddChildNode(continueKeyword.get());
}

void ContinueStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ContinueStatementNode::Clone() const
{
    ContinueStatementNode* clone = new ContinueStatementNode(GetSpan(), GetCompletionContext());
    clone->SetContinueKeyword(static_cast<ContinueKeywordNode*>(continueKeyword->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ContinueStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GotoStatementNode::GotoStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void GotoStatementNode::SetGotoKeyword(GotoKeywordNode* gotoKeyword_)
{
    gotoKeyword.reset(gotoKeyword_);
    AddChildNode(gotoKeyword.get());
}

void GotoStatementNode::SetTarget(IdentifierNode* target_)
{
    target.reset(target_);
    AddChildNode(target.get());
}

void GotoStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* GotoStatementNode::Clone() const
{
    GotoStatementNode* clone = new GotoStatementNode(GetSpan(), GetCompletionContext());
    clone->SetGotoKeyword(static_cast<GotoKeywordNode*>(gotoKeyword->Clone()));
    clone->SetTarget(static_cast<IdentifierNode*>(target->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void GotoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GotoCaseStatementNode::GotoCaseStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void GotoCaseStatementNode::SetGotoKeyword(GotoKeywordNode* gotoKeyword_)
{
    gotoKeyword.reset(gotoKeyword_);
    AddChildNode(gotoKeyword.get());
}

void GotoCaseStatementNode::SetCaseKeyword(CaseKeywordNode* caseKeyword_)
{
    caseKeyword.reset(caseKeyword_);
    AddChildNode(caseKeyword.get());
}

void GotoCaseStatementNode::SetCaseExpr(Node* caseExpr_)
{
    caseExpr.reset(caseExpr_);
    AddChildNode(caseExpr.get());
}

void GotoCaseStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* GotoCaseStatementNode::Clone() const
{
    GotoCaseStatementNode* clone = new GotoCaseStatementNode(GetSpan(), GetCompletionContext());
    clone->SetGotoKeyword(static_cast<GotoKeywordNode*>(gotoKeyword->Clone()));
    clone->SetCaseKeyword(static_cast<CaseKeywordNode*>(caseKeyword->Clone()));
    clone->SetCaseExpr(caseExpr->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void GotoCaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GotoDefaultStatementNode::GotoDefaultStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void GotoDefaultStatementNode::SetGotoKeyword(GotoKeywordNode* gotoKeyword_)
{
    gotoKeyword.reset(gotoKeyword_);
    AddChildNode(gotoKeyword.get());
}

void GotoDefaultStatementNode::SetDefaultKeyword(DefaultKeywordNode* defaultKeyword_)
{
    defaultKeyword.reset(defaultKeyword_);
    AddChildNode(defaultKeyword.get());
}

void GotoDefaultStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* GotoDefaultStatementNode::Clone() const
{
    GotoDefaultStatementNode* clone = new GotoDefaultStatementNode(GetSpan(), GetCompletionContext());
    clone->SetGotoKeyword(static_cast<GotoKeywordNode*>(gotoKeyword->Clone()));
    clone->SetDefaultKeyword(static_cast<DefaultKeywordNode*>(defaultKeyword->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void GotoDefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CaseExprNode::CaseExprNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void CaseExprNode::SetCaseKeyword(CaseKeywordNode* caseKeyword_)
{
    caseKeyword.reset(caseKeyword_);
    AddChildNode(caseKeyword.get());
}

void CaseExprNode::SetCaseExpr(Node* caseExpr_)
{
    caseExpr.reset(caseExpr_);
    AddChildNode(caseExpr.get());
}

void CaseExprNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

Node* CaseExprNode::Clone() const
{
    CaseExprNode* clone = new CaseExprNode(GetSpan(), GetCompletionContext());
    clone->SetCaseKeyword(static_cast<CaseKeywordNode*>(caseKeyword->Clone()));
    clone->SetCaseExpr(caseExpr->Clone());
    clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    clone->Make();
    return clone;
}

void CaseExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CaseStatementNode::CaseStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void CaseStatementNode::AddCaseExpr(CaseExprNode* caseExpr)
{
    cases.AddNode(caseExpr);
}

void CaseStatementNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void CaseStatementNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
}

void CaseStatementNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* CaseStatementNode::Clone() const
{
    CaseStatementNode* clone = new CaseStatementNode(GetSpan(), GetCompletionContext());
    for (const auto& caseExpr : cases.Nodes())
    {
        clone->AddCaseExpr(static_cast<CaseExprNode*>(caseExpr->Clone()));
    }
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void CaseStatementNode::Make()
{
    cases.Make();
    AddChildNode(&cases);
    statements.Make();
    AddChildNode(&statements);
    StatementNode::Make();
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultStatementNode::DefaultStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void DefaultStatementNode::SetDefaultKeyword(DefaultKeywordNode* defaultKeyword_)
{
    defaultKeyword.reset(defaultKeyword_);
    AddChildNode(defaultKeyword.get());
}

void DefaultStatementNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void DefaultStatementNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void DefaultStatementNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
}

void DefaultStatementNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* DefaultStatementNode::Clone() const
{
    DefaultStatementNode* clone = new DefaultStatementNode(GetSpan(), GetCompletionContext());
    clone->SetDefaultKeyword(static_cast<DefaultKeywordNode*>(defaultKeyword->Clone()));
    clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void DefaultStatementNode::Make()
{
    statements.Make();
    AddChildNode(&statements);
    StatementNode::Make();
}

void DefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SwitchStatementNode::SwitchStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void SwitchStatementNode::SetSwitchKeyword(SwitchKeywordNode* switchKeyword_)
{
    switchKeyword.reset(switchKeyword_);
    AddChildNode(switchKeyword.get());
}

void SwitchStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void SwitchStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    AddChildNode(condition.get());
}

void SwitchStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void SwitchStatementNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void SwitchStatementNode::AddCaseStatement(CaseStatementNode* caseStatement)
{
    caseStatements.AddNode(caseStatement);
    AddChildNode(caseStatement);
}

void SwitchStatementNode::SetDefaultStatement(DefaultStatementNode* defaultStatement_)
{
    defaultStatement.reset(defaultStatement_);
    AddChildNode(defaultStatement.get());
}

void SwitchStatementNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* SwitchStatementNode::Clone() const
{
    SwitchStatementNode* clone = new SwitchStatementNode(GetSpan(), GetCompletionContext());
    clone->SetSwitchKeyword(static_cast<SwitchKeywordNode*>(switchKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetCondition(condition->Clone());
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& caseStatement : caseStatements.Nodes())
    {
        clone->AddCaseStatement(static_cast<CaseStatementNode*>(caseStatement->Clone()));
    }
    if (defaultStatement)
    {
        clone->SetDefaultStatement(static_cast<DefaultStatementNode*>(defaultStatement->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void SwitchStatementNode::Make()
{
    caseStatements.Make();
    AddChildNode(&caseStatements);
    StatementNode::Make();
}

void SwitchStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AssignmentStatementNode::AssignmentStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void AssignmentStatementNode::SetLeft(Node* left_)
{
    left.reset(left_);
    AddChildNode(left.get());
}

void AssignmentStatementNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void AssignmentStatementNode::SetRight(Node* right_)
{
    right.reset(right_);
    AddChildNode(right.get());
}

void AssignmentStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* AssignmentStatementNode::Clone() const
{
    AssignmentStatementNode* clone = new AssignmentStatementNode(GetSpan(), GetCompletionContext());
    clone->SetLeft(left->Clone());
    clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    clone->SetRight(right->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void AssignmentStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void ExpressionStatementNode::SetExpression(Node* expression_)
{
    expression.reset(expression_);
    AddChildNode(expression.get());
}

void ExpressionStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ExpressionStatementNode::Clone() const
{
    ExpressionStatementNode* clone = new ExpressionStatementNode(GetSpan(), GetCompletionContext());
    clone->SetExpression(expression->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EmptyStatementNode::EmptyStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void EmptyStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* EmptyStatementNode::Clone() const
{
    EmptyStatementNode* clone = new EmptyStatementNode(GetSpan(), GetCompletionContext());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void EmptyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstructionStatementNode::ConstructionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) :
    StatementNode(span_, completionContext_), argumentList(soul::ast::Span(), CompletionContext::none)
{
}

void ConstructionStatementNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void ConstructionStatementNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void ConstructionStatementNode::SetAssign(AssignNode* assign_)
{
    assign.reset(assign_);
    AddChildNode(assign.get());
}

void ConstructionStatementNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ConstructionStatementNode::AddNode(Node* node)
{
    argumentList.AddNode(node);
}

void ConstructionStatementNode::AddComma(CommaNode* comma)
{
    argumentList.AddComma(comma);
}

void ConstructionStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

void ConstructionStatementNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* ConstructionStatementNode::Clone() const
{
    ConstructionStatementNode* clone = new ConstructionStatementNode(GetSpan(), GetCompletionContext());
    clone->SetTypeExpr(typeExpr->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    if (assign)
    {
        clone->SetAssign(static_cast<AssignNode*>(assign->Clone()));
    }
    if (lparen)
    {
        clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    }
    if (rparen)
    {
        clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    }
    clone->argumentList.CloneFrom(argumentList);
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ConstructionStatementNode::Make()
{
    argumentList.Make();
    AddChildNode(&argumentList);
    StatementNode::Make();
}

void ConstructionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeleteStatementNode::DeleteStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void DeleteStatementNode::SetDeleteKeyword(DeleteKeywordNode* deleteKeyword_)
{
    deleteKeyword.reset(deleteKeyword_);
    AddChildNode(deleteKeyword.get());
}

void DeleteStatementNode::SetPtr(Node* ptr_)
{
    ptr.reset(ptr_);
    AddChildNode(ptr.get());
}

void DeleteStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DeleteStatementNode::Clone() const
{
    DeleteStatementNode* clone = new DeleteStatementNode(GetSpan(), GetCompletionContext());
    clone->SetDeleteKeyword(static_cast<DeleteKeywordNode*>(deleteKeyword->Clone()));
    clone->SetPtr(ptr->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void DeleteStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DestroyStatementNode::DestroyStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void DestroyStatementNode::SetDestroyKeyword(DestroyKeywordNode* destroyKeyword_)
{
    destroyKeyword.reset(destroyKeyword_);
    AddChildNode(destroyKeyword.get());
}

void DestroyStatementNode::SetPtr(Node* ptr_)
{
    ptr.reset(ptr_);
    AddChildNode(ptr.get());
}

void DestroyStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DestroyStatementNode::Clone() const
{
    DestroyStatementNode* clone = new DestroyStatementNode(GetSpan(), GetCompletionContext());
    clone->SetDestroyKeyword(static_cast<DestroyKeywordNode*>(destroyKeyword->Clone()));
    clone->SetPtr(ptr->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void DestroyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThrowStatementNode::ThrowStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void ThrowStatementNode::SetThrowKeyword(ThrowKeywordNode* throwKeyword_)
{
    throwKeyword.reset(throwKeyword_);
    AddChildNode(throwKeyword.get());
}

void ThrowStatementNode::SetException(Node* exception_)
{
    exception.reset(exception_);
    AddChildNode(exception.get());
}

void ThrowStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ThrowStatementNode::Clone() const
{
    ThrowStatementNode* clone = new ThrowStatementNode(GetSpan(), GetCompletionContext());
    clone->SetThrowKeyword(static_cast<ThrowKeywordNode*>(throwKeyword->Clone()));
    if (exception)
    {
        clone->SetException(exception->Clone());
    }
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ThrowStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CatchNode::CatchNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void CatchNode::SetCatchKeyword(CatchKeywordNode* catchKeyword_)
{
    catchKeyword.reset(catchKeyword_);
    AddChildNode(catchKeyword.get());
}

void CatchNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void CatchNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void CatchNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void CatchNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void CatchNode::SetCatchBlock(CompoundStatementNode* catchBlock_)
{
    catchBlock.reset(catchBlock_);
    AddChildNode(catchBlock.get());
}

Node* CatchNode::Clone() const
{
    CatchNode* clone = new CatchNode(GetSpan(), GetCompletionContext());
    clone->SetCatchKeyword(static_cast<CatchKeywordNode*>(catchKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetTypeExpr(typeExpr->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetCatchBlock(static_cast<CompoundStatementNode*>(catchBlock->Clone()));
    clone->Make();
    return clone;
}

void CatchNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TryStatementNode::TryStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void TryStatementNode::SetTryKeyword(TryKeywordNode* tryKeyword_)
{
    tryKeyword.reset(tryKeyword_);
    AddChildNode(tryKeyword.get());
}

void TryStatementNode::SetTryBlock(CompoundStatementNode* tryBlock_)
{
    tryBlock.reset(tryBlock_);
    AddChildNode(tryBlock.get());
}

void TryStatementNode::AddCatch(CatchNode* catchNode)
{
    catches.AddNode(catchNode);
}

Node* TryStatementNode::Clone() const
{
    TryStatementNode* clone = new TryStatementNode(GetSpan(), GetCompletionContext());
    clone->SetTryKeyword(static_cast<TryKeywordNode*>(tryKeyword->Clone()));
    clone->SetTryBlock(static_cast<CompoundStatementNode*>(tryBlock->Clone()));
    for (const auto& catchNode : catches.Nodes())
    {
        clone->AddCatch(static_cast<CatchNode*>(catchNode->Clone()));
    }
    clone->Make();
    return clone;
}

void TryStatementNode::Make()
{
    catches.Make();
    AddChildNode(&catches);
    StatementNode::Make();
}

void TryStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LabeledStatementNode::LabeledStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void LabeledStatementNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void LabeledStatementNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void LabeledStatementNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    AddChildNode(statement.get());
}

Node* LabeledStatementNode::Clone() const
{
    LabeledStatementNode* clone = new LabeledStatementNode(GetSpan(), GetCompletionContext());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    clone->Make();
    return clone;
}

void LabeledStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AssertStatementNode::AssertStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void AssertStatementNode::SetHash(HashNode* hash_)
{
    hash.reset(hash_);
    AddChildNode(hash.get());
}

void AssertStatementNode::SetAssertKeyword(AssertKeywordNode* assertKeyword_)
{
    assertKeyword.reset(assertKeyword_);
    AddChildNode(assertKeyword.get());
}

void AssertStatementNode::SetExpression(Node* expression_)
{
    expression.reset(expression_);
    AddChildNode(expression.get());
}

void AssertStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* AssertStatementNode::Clone() const
{
    AssertStatementNode* clone = new AssertStatementNode(GetSpan(), GetCompletionContext());
    clone->SetHash(static_cast<HashNode*>(hash->Clone()));
    clone->SetAssertKeyword(static_cast<AssertKeywordNode*>(assertKeyword->Clone()));
    clone->SetExpression(expression->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void AssertStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompExprNode::CondCompExprNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

CondCompBinaryExprNode::CondCompBinaryExprNode(CondCompExprNode* left_, Node* op_, CondCompExprNode* right_) : 
    CondCompExprNode(soul::ast::Span(), CompletionContext::none), left(left_), op(op_), right(right_)
{
    AddChildNode(left.get());
    AddChildNode(op.get());
    AddChildNode(right.get());
}

CondCompDisjunctionNode::CondCompDisjunctionNode(CondCompExprNode* left_, DisjunctionNode* op_, CondCompExprNode* right_) : CondCompBinaryExprNode(left_, op_, right_)
{
}

Node* CondCompDisjunctionNode::Clone() const
{
    CondCompDisjunctionNode* clone = new CondCompDisjunctionNode(static_cast<CondCompExprNode*>(Left()->Clone()), static_cast<DisjunctionNode*>(Op()->Clone()),
        static_cast<CondCompExprNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void CondCompDisjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompConjunctionNode::CondCompConjunctionNode(CondCompExprNode* left_, AmpAmpNode* op_, CondCompExprNode* right_) : CondCompBinaryExprNode(left_, op_, right_)
{
}

Node* CondCompConjunctionNode::Clone() const
{
    CondCompConjunctionNode* clone = new CondCompConjunctionNode(static_cast<CondCompExprNode*>(Left()->Clone()), static_cast<AmpAmpNode*>(Op()->Clone()),
        static_cast<CondCompExprNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void CondCompConjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompNotNode::CondCompNotNode(NotNode* not_, CondCompExprNode* prefix_) : CondCompExprNode(soul::ast::Span(), CompletionContext::none), notOp(not_), prefix(prefix_)
{
    AddChildNode(notOp.get());
    AddChildNode(prefix.get());
}

Node* CondCompNotNode::Clone() const
{
    CondCompNotNode* clone = new CondCompNotNode(static_cast<NotNode*>(notOp->Clone()), static_cast<CondCompExprNode*>(prefix->Clone()));
    clone->Make();
    return clone;
}

void CondCompNotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompParenExprNode::CondCompParenExprNode(const soul::ast::Span& span_, CompletionContext completionContext_) : CondCompExprNode(span_, completionContext_)
{
}

void CondCompParenExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void CondCompParenExprNode::SetExpr(CondCompExprNode* expr_)
{
    expr.reset(expr_);
    AddChildNode(expr.get());
}

void CondCompParenExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* CondCompParenExprNode::Clone() const
{
    CondCompParenExprNode* clone = new CondCompParenExprNode(GetSpan(), GetCompletionContext());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetExpr(static_cast<CondCompExprNode*>(expr->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void CondCompParenExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompSymbolNode::CondCompSymbolNode(const soul::ast::Span& span_, CompletionContext completionContext_, IdentifierNode* symbol_) :
    CondCompExprNode(span_, completionContext_), symbol(symbol_)
{
    AddChildNode(symbol.get());
}

Node* CondCompSymbolNode::Clone() const
{
    CondCompSymbolNode* clone = new CondCompSymbolNode(GetSpan(), GetCompletionContext(), static_cast<IdentifierNode*>(symbol->Clone()));
    clone->Make();
    return clone;
}

void CondCompSymbolNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IfGroupNode::IfGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void IfGroupNode::SetHash(HashNode* hash_)
{
    hash.reset(hash_);
    AddChildNode(hash.get());
}

void IfGroupNode::SetIfKeyword(IfKeywordNode* ifKeyword_)
{
    ifKeyword.reset(ifKeyword_);
    AddChildNode(ifKeyword.get());
}

void IfGroupNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void IfGroupNode::SetExpr(CondCompExprNode* expr_)
{
    expr.reset(expr_);
    AddChildNode(expr.get());
}

void IfGroupNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void IfGroupNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
}

Node* IfGroupNode::Clone() const
{
    IfGroupNode* clone = new IfGroupNode(GetSpan(), GetCompletionContext());
    clone->SetHash(static_cast<HashNode*>(hash->Clone()));
    clone->SetIfKeyword(static_cast<IfKeywordNode*>(ifKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetExpr(static_cast<CondCompExprNode*>(expr->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->Make();
    return clone;
}

void IfGroupNode::Make()
{
    statements.Make();
    AddChildNode(&statements);
    SyntaxNode::Make();
}

void IfGroupNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ElifGroupNode::ElifGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ElifGroupNode::SetHash(HashNode* hash_)
{
    hash.reset(hash_);
    AddChildNode(hash.get());
}

void ElifGroupNode::SetElifKeyword(ElifKeywordNode* elifKeyword_)
{
    elifKeyword.reset(elifKeyword_);
    AddChildNode(elifKeyword.get());
}

void ElifGroupNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ElifGroupNode::SetExpr(CondCompExprNode* expr_)
{
    expr.reset(expr_);
    AddChildNode(expr.get());
}

void ElifGroupNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void ElifGroupNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
}

Node* ElifGroupNode::Clone() const
{
    ElifGroupNode* clone = new ElifGroupNode(GetSpan(), GetCompletionContext());
    clone->SetHash(static_cast<HashNode*>(hash->Clone()));
    clone->SetElifKeyword(static_cast<ElifKeywordNode*>(elifKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetExpr(static_cast<CondCompExprNode*>(expr->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->Make();
    return clone;
}

void ElifGroupNode::Make()
{
    statements.Make();
    AddChildNode(&statements);
    SyntaxNode::Make();
}

void ElifGroupNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ElseGroupNode::ElseGroupNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ElseGroupNode::SetHash(HashNode* hash_)
{
    hash.reset(hash_);
    AddChildNode(hash.get());
}

void ElseGroupNode::SetElseKeyword(ElseKeywordNode* elseKeyword_)
{
    elseKeyword.reset(elseKeyword_);
    AddChildNode(elseKeyword.get());
}

void ElseGroupNode::AddStatement(StatementNode* statement)
{
    statements.AddNode(statement);
}

Node* ElseGroupNode::Clone() const
{
    ElseGroupNode* clone = new ElseGroupNode(GetSpan(), GetCompletionContext());
    clone->SetHash(static_cast<HashNode*>(hash->Clone()));
    clone->SetElseKeyword(static_cast<ElseKeywordNode*>(elseKeyword->Clone()));
    for (const auto& statement : statements.Nodes())
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->Make();
    return clone;
}

void ElseGroupNode::Make()
{
    statements.Make();
    AddChildNode(&statements);
    SyntaxNode::Make();
}

void ElseGroupNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CondCompStatementNode::CondCompStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

void CondCompStatementNode::SetIfGroup(IfGroupNode* ifGroup_)
{
    ifGroup.reset(ifGroup_);
    AddChildNode(ifGroup.get());
}

void CondCompStatementNode::AddElifGroup(ElifGroupNode* elifGroup)
{
    elifGroups.AddNode(elifGroup);
    AddChildNode(elifGroup);
}

void CondCompStatementNode::SetElseGroup(ElseGroupNode* elseGroup_)
{
    elseGroup.reset(elseGroup_);
    AddChildNode(elseGroup.get());
}

void CondCompStatementNode::SetHash(HashNode* hash_)
{
    hash.reset(hash_);
    AddChildNode(hash.get());
}

void CondCompStatementNode::SetEndifKeyword(EndifKeywordNode* endifKeyword_)
{
    endifKeyword.reset(endifKeyword_);
    AddChildNode(endifKeyword.get());
}

Node* CondCompStatementNode::Clone() const
{
    CondCompStatementNode* clone = new CondCompStatementNode(GetSpan(), GetCompletionContext());
    clone->SetIfGroup(static_cast<IfGroupNode*>(ifGroup->Clone()));
    for (const auto& elifGroup : elifGroups.Nodes())
    {
        clone->AddElifGroup(static_cast<ElifGroupNode*>(elifGroup->Clone()));
    }
    if (elseGroup)
    {
        clone->SetElseGroup(static_cast<ElseGroupNode*>(elseGroup->Clone()));
    }
    clone->SetHash(static_cast<HashNode*>(hash->Clone()));
    clone->SetEndifKeyword(static_cast<EndifKeywordNode*>(endifKeyword->Clone()));
    clone->Make();
    return clone;
}

void CondCompStatementNode::Make()
{
    elifGroups.Make();
    AddChildNode(&elifGroups);
    StatementNode::Make();
}

void CondCompStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CompletionStatementNode::CompletionStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : StatementNode(span_, completionContext_)
{
}

Node* CompletionStatementNode::Clone() const
{
    CompletionStatementNode* clone = new CompletionStatementNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CompletionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConditionCompletionNode::ConditionCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

Node* ConditionCompletionNode::Clone() const
{
    ConditionCompletionNode* clone = new ConditionCompletionNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ConditionCompletionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ContainerCompletionNode::ContainerCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

Node* ContainerCompletionNode::Clone() const
{
    ContainerCompletionNode* clone = new ContainerCompletionNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ContainerCompletionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExpressionCompletionNode::ExpressionCompletionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

Node* ExpressionCompletionNode::Clone() const
{
    ExpressionCompletionNode* clone = new ExpressionCompletionNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ExpressionCompletionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
