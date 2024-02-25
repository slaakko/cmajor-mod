// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.statement;

import cmajor.fault.tolerant.parser.expression;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseCompoundStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> compoundStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    compoundStatement.reset(new cmajor::fault::tolerant::ast::CompoundStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    compoundStatement->SetLBrace(lbrace.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            compoundStatement->AddStatement(statement.release());
        }
        else if (*lexer != RBRACE)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
            compoundStatement->AddStatement(statement.release());
            lexer.SetPos(save);
            break;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    compoundStatement->SetRBrace(rbrace.release());
    compoundStatement->Make();
    return soul::parser::Match(true, compoundStatement.release());
}

soul::parser::Match ParseReturnStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ReturnStatementNode> returnStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::ReturnKeywordNode> returnKeyword;
    if (*lexer == RETURN)
    {
        returnKeyword.reset(new cmajor::fault::tolerant::ast::ReturnKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> returnValue;
    soul::parser::Match returnValueMatch = ParseExpression(context, lexer);
    if (returnValueMatch.hit)
    {
        returnValue.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(returnValueMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    returnStatement.reset(new cmajor::fault::tolerant::ast::ReturnStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    returnStatement->SetReturnKeyword(returnKeyword.release());
    if (returnValue)
    {
        returnStatement->SetReturnValue(returnValue.release());
    }
    returnStatement->SetSemicolon(semicolon.release());
    returnStatement->Make();
    return soul::parser::Match(true, returnStatement.release());
}

soul::parser::Match ParseIfStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::IfStatementNode> ifStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::IfKeywordNode> ifKeyword;
    if (*lexer == IF)
    {
        ifKeyword.reset(new cmajor::fault::tolerant::ast::IfKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> condition;
    soul::parser::Match conditionMatch = ParseExpression(context, lexer);
    if (conditionMatch.hit)
    {
        condition.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conditionMatch.value));
    }
    else
    {
        condition.reset(new cmajor::fault::tolerant::ast::ConditionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::booleanExpr));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> thenS;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        thenS.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        thenS.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ElseKeywordNode> elseKeyword;
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> elseS;
    if (*lexer == ELSE)
    {
        elseKeyword.reset(new cmajor::fault::tolerant::ast::ElseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            elseS.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
        }
        else
        {
            elseS.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
        }
    }
    ifStatement.reset(new cmajor::fault::tolerant::ast::IfStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    ifStatement->SetIfKeyword(ifKeyword.release());
    ifStatement->SetLParen(lparen.release());
    ifStatement->SetCondition(condition.release());
    ifStatement->SetRParen(rparen.release());
    ifStatement->SetThenStatement(thenS.release());
    if (elseKeyword)
    {
        ifStatement->SetElseKeyword(elseKeyword.release());
        ifStatement->SetElseStatement(elseS.release());
    }
    ifStatement->Make();
    return soul::parser::Match(true, ifStatement.release());
}

soul::parser::Match ParseWhileStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::WhileStatementNode> whileStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::WhileKeywordNode> whileKeyword;
    if (*lexer == WHILE)
    {
        whileKeyword.reset(new cmajor::fault::tolerant::ast::WhileKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> condition;
    soul::parser::Match conditionMatch = ParseExpression(context, lexer);
    if (conditionMatch.hit)
    {
        condition.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conditionMatch.value));
    }
    else
    {
        condition.reset(new cmajor::fault::tolerant::ast::ConditionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::booleanExpr));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        statement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    whileStatement.reset(new cmajor::fault::tolerant::ast::WhileStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    whileStatement->SetWhileKeyword(whileKeyword.release());
    whileStatement->SetLParen(lparen.release());
    whileStatement->SetCondition(condition.release());
    whileStatement->SetRParen(rparen.release());
    whileStatement->SetStatement(statement.release());
    whileStatement->Make();
    return soul::parser::Match(true, whileStatement.release());
}

soul::parser::Match ParseDoStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::DoStatementNode> doStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::DoKeywordNode> doKeyword;
    if (*lexer == DO)
    {
        doKeyword.reset(new cmajor::fault::tolerant::ast::DoKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        statement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::WhileKeywordNode> whileKeyword;
    if (*lexer == WHILE)
    {
        whileKeyword.reset(new cmajor::fault::tolerant::ast::WhileKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        whileKeyword.reset(new cmajor::fault::tolerant::ast::WhileKeywordNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> condition;
    soul::parser::Match conditionMatch = ParseExpression(context, lexer);
    if (conditionMatch.hit)
    {
        condition.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conditionMatch.value));
    }
    else
    {
        condition.reset(new cmajor::fault::tolerant::ast::ConditionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::booleanExpr));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    doStatement.reset(new cmajor::fault::tolerant::ast::DoStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    doStatement->SetDoKeyword(doKeyword.release());
    doStatement->SetStatement(statement.release());
    doStatement->SetWhileKeyword(whileKeyword.release());
    doStatement->SetLParen(lparen.release());
    doStatement->SetCondition(condition.release());
    doStatement->SetRParen(rparen.release());
    doStatement->SetSemicolon(semicolon.release());
    doStatement->Make();
    return soul::parser::Match(true, doStatement.release());
}

soul::parser::Match ParseRangeForStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::RangeForStatementNode> rangeForStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::ForKeywordNode> forKeyword;
    if (*lexer == FOR)
    {
        forKeyword.reset(new cmajor::fault::tolerant::ast::ForKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> container;
    soul::parser::Match expressionMatch = ParseExpression(context, lexer);
    if (expressionMatch.hit)
    {
        container.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
    }
    else
    {
        container.reset(new cmajor::fault::tolerant::ast::ContainerCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::container));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        statement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    rangeForStatement.reset(new cmajor::fault::tolerant::ast::RangeForStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    rangeForStatement->SetForKeyword(forKeyword.release());
    rangeForStatement->SetLParen(lparen.release());
    rangeForStatement->SetTypeExpr(typeExpr.release());
    rangeForStatement->SetId(id.release());
    rangeForStatement->SetColon(colon.release());
    rangeForStatement->SetContainer(container.release());
    rangeForStatement->SetRParen(rparen.release());
    rangeForStatement->SetStatement(statement.release());
    rangeForStatement->Make();
    return soul::parser::Match(true, rangeForStatement.release());
}

soul::parser::Match ParseForInitStatement(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match assignmentStatementMatch = ParseAssignmentStatement(context, lexer);
    if (assignmentStatementMatch.hit)
    {
        return assignmentStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match constructionStatementMatch = ParseConstructionStatement(context, lexer);
    if (constructionStatementMatch.hit)
    {
        return constructionStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match emptyStatementMatch = ParseEmptyStatement(context, lexer);
    if (emptyStatementMatch.hit)
    {
        return emptyStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseForLoopStatement(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match assignmentStatementMatch = ParseAssignmentStatementNoSemicolon(context, lexer);
    if (assignmentStatementMatch.hit)
    {
        return assignmentStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match expressionMatch = ParseExpression(context, lexer);
    if (expressionMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> expression;
        expression.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
        std::unique_ptr<cmajor::fault::tolerant::ast::ExpressionStatementNode> expressionStatement;
        expressionStatement.reset(new cmajor::fault::tolerant::ast::ExpressionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        expressionStatement->SetExpression(expression.release());
        expressionStatement->Make();
        return soul::parser::Match(true, expressionStatement.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::EmptyStatementNode> emptyStatement;
    emptyStatement.reset(new cmajor::fault::tolerant::ast::EmptyStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    emptyStatement->Make();
    return soul::parser::Match(true, emptyStatement.release());
}

soul::parser::Match ParseForStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ForStatementNode> forStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::ForKeywordNode> forKeyword;
    if (*lexer == FOR)
    {
        forKeyword.reset(new cmajor::fault::tolerant::ast::ForKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> forInitStatement;
    soul::parser::Match forInitStatementMatch = ParseForInitStatement(context, lexer);
    if (forInitStatementMatch.hit)
    {
        forInitStatement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(forInitStatementMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> condition;
    soul::parser::Match conditionMatch = ParseExpression(context, lexer);
    if (conditionMatch.hit)
    {
        condition.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conditionMatch.value));
    }
    else
    {
        condition.reset(new cmajor::fault::tolerant::ast::TrueKeywordNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> loopStatement;
    soul::parser::Match loopStatementMatch = ParseForLoopStatement(context, lexer);
    if (loopStatementMatch.hit)
    {
        loopStatement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(loopStatementMatch.value));
    }
    else
    {
        loopStatement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        statement.reset(new cmajor::fault::tolerant::ast::CompletionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::statement));
    }
    forStatement.reset(new cmajor::fault::tolerant::ast::ForStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    forStatement->SetForKeyword(forKeyword.release());
    forStatement->SetLParen(lparen.release());
    forStatement->SetInitStatement(forInitStatement.release());
    forStatement->SetCondition(condition.release());
    forStatement->SetSemicolon(semicolon.release());
    forStatement->SetLoopStatement(loopStatement.release());
    forStatement->SetRParen(rparen.release());
    forStatement->SetStatement(statement.release());
    forStatement->Make();
    return soul::parser::Match(true, forStatement.release());
}

soul::parser::Match ParseBreakStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::BreakStatementNode> breakStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::BreakKeywordNode> breakKeyword;
    if (*lexer == BREAK)
    {
        breakKeyword.reset(new cmajor::fault::tolerant::ast::BreakKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    breakStatement.reset(new cmajor::fault::tolerant::ast::BreakStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    breakStatement->SetBreakKeyword(breakKeyword.release());
    breakStatement->SetSemicolon(semicolon.release());
    breakStatement->Make();
    return soul::parser::Match(true, breakStatement.release());
}

soul::parser::Match ParseContinueStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ContinueStatementNode> continueStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::ContinueKeywordNode> continueKeyword;
    if (*lexer == CONTINUE)
    {
        continueKeyword.reset(new cmajor::fault::tolerant::ast::ContinueKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    continueStatement.reset(new cmajor::fault::tolerant::ast::ContinueStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    continueStatement->SetContinueKeyword(continueKeyword.release());
    continueStatement->SetSemicolon(semicolon.release());
    continueStatement->Make();
    return soul::parser::Match(true, continueStatement.release());
}

soul::parser::Match ParseGotoStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoStatementNode> gotoStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoKeywordNode> gotoKeyword;
    if (*lexer == GOTO)
    {
        gotoKeyword.reset(new cmajor::fault::tolerant::ast::GotoKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> target;
    soul::parser::Match identifieMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifieMatch.hit)
    {
        target.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifieMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    gotoStatement.reset(new cmajor::fault::tolerant::ast::GotoStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    gotoStatement->SetGotoKeyword(gotoKeyword.release());
    gotoStatement->SetTarget(target.release());
    gotoStatement->SetSemicolon(semicolon.release());
    gotoStatement->Make();
    return soul::parser::Match(true, gotoStatement.release());
}

soul::parser::Match ParseGotoCaseStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoCaseStatementNode> gotoCaseStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoKeywordNode> gotoKeyword;
    if (*lexer == GOTO)
    {
        gotoKeyword.reset(new cmajor::fault::tolerant::ast::GotoKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CaseKeywordNode> caseKeyword;
    if (*lexer == CASE)
    {
        caseKeyword.reset(new cmajor::fault::tolerant::ast::CaseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> caseExpr;
    soul::parser::Match caseExprMatch = ParseExpression(context, lexer);
    if (caseExprMatch.hit)
    {
        caseExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(caseExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    gotoCaseStatement.reset(new cmajor::fault::tolerant::ast::GotoCaseStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    gotoCaseStatement->SetGotoKeyword(gotoKeyword.release());
    gotoCaseStatement->SetCaseKeyword(caseKeyword.release());
    gotoCaseStatement->SetCaseExpr(caseExpr.release());
    gotoCaseStatement->SetSemicolon(semicolon.release());
    gotoCaseStatement->Make();
    return soul::parser::Match(true, gotoCaseStatement.release());
}

soul::parser::Match ParseGotoDefaultStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoDefaultStatementNode> gotoDefaultStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::GotoKeywordNode> gotoKeyword;
    if (*lexer == GOTO)
    {
        gotoKeyword.reset(new cmajor::fault::tolerant::ast::GotoKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::DefaultKeywordNode> defaultKeyword;
    if (*lexer == DEFAULT)
    {
        defaultKeyword.reset(new cmajor::fault::tolerant::ast::DefaultKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    gotoDefaultStatement.reset(new cmajor::fault::tolerant::ast::GotoDefaultStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    gotoDefaultStatement->SetGotoKeyword(gotoKeyword.release());
    gotoDefaultStatement->SetDefaultKeyword(defaultKeyword.release());
    gotoDefaultStatement->SetSemicolon(semicolon.release());
    gotoDefaultStatement->Make();
    return soul::parser::Match(true, gotoDefaultStatement.release());
}

soul::parser::Match ParseCaseExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CaseExprNode> caseExpr;
    std::unique_ptr<cmajor::fault::tolerant::ast::CaseKeywordNode> caseKeyword;
    if (*lexer == CASE)
    {
        caseKeyword.reset(new cmajor::fault::tolerant::ast::CaseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    caseExpr.reset(new cmajor::fault::tolerant::ast::CaseExprNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    caseExpr->SetCaseKeyword(caseKeyword.release());
    caseExpr->SetCaseExpr(expr.release());
    caseExpr->SetColon(colon.release());
    caseExpr->Make();
    return soul::parser::Match(true, caseExpr.release());
}

soul::parser::Match ParseCaseStatement(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::CaseStatementNode> caseStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::CaseExprNode> caseExpr;
    soul::parser::Match caseMatch = ParseCaseExpr(context, lexer);
    if (caseMatch.hit)
    {
        caseExpr.reset(static_cast<cmajor::fault::tolerant::ast::CaseExprNode*>(caseMatch.value));
    }
    else
    {
        lexer.SetPos(save);
        return soul::parser::Match(false);
    }
    caseStatement.reset(new cmajor::fault::tolerant::ast::CaseStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    caseStatement->AddCaseExpr(caseExpr.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CaseExprNode> caseExpr;
        soul::parser::Match caseMatch = ParseCaseExpr(context, lexer);
        if (caseMatch.hit)
        {
            caseExpr.reset(static_cast<cmajor::fault::tolerant::ast::CaseExprNode*>(caseMatch.value));
            caseStatement->AddCaseExpr(caseExpr.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    caseStatement->SetLBrace(lbrace.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            caseStatement->AddStatement(statement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    caseStatement->SetRBrace(rbrace.release());
    caseStatement->Make();
    return soul::parser::Match(true, caseStatement.release());
}

soul::parser::Match ParseDefaultStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::DefaultStatementNode> defaultStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::DefaultKeywordNode> defaultKeyword;
    if (*lexer == DEFAULT)
    {
        defaultKeyword.reset(new cmajor::fault::tolerant::ast::DefaultKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    defaultStatement.reset(new cmajor::fault::tolerant::ast::DefaultStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    defaultStatement->SetDefaultKeyword(defaultKeyword.release());
    defaultStatement->SetColon(colon.release());
    defaultStatement->SetLBrace(lbrace.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            defaultStatement->AddStatement(statement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    defaultStatement->SetRBrace(rbrace.release());
    defaultStatement->Make();
    return soul::parser::Match(true, defaultStatement.release());
}

soul::parser::Match ParseSwitchStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::SwitchStatementNode> switchStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::SwitchKeywordNode> switchKeyword;
    if (*lexer == SWITCH)
    {
        switchKeyword.reset(new cmajor::fault::tolerant::ast::SwitchKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> condition;
    soul::parser::Match conditionMatch = ParseExpression(context, lexer);
    if (conditionMatch.hit)
    {
        condition.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conditionMatch.value));
    }
    else
    {
        condition.reset(new cmajor::fault::tolerant::ast::ConditionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::booleanExpr));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    switchStatement.reset(new cmajor::fault::tolerant::ast::SwitchStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    switchStatement->SetSwitchKeyword(switchKeyword.release());
    switchStatement->SetLParen(lparen.release());
    switchStatement->SetCondition(condition.release());
    switchStatement->SetRParen(rparen.release());
    switchStatement->SetLBrace(lbrace.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match caseStatementMatch = ParseCaseStatement(context, lexer);
        if (caseStatementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::CaseStatementNode> caseStatement;
            caseStatement.reset(static_cast<cmajor::fault::tolerant::ast::CaseStatementNode*>(caseStatementMatch.value));
            switchStatement->AddCaseStatement(caseStatement.release());
        }
        else
        {
            lexer.SetPos(save);
            soul::parser::Match defaultStatementMatch = ParseDefaultStatement(context, lexer);
            if (defaultStatementMatch.hit)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::DefaultStatementNode> defaultStatement;
                defaultStatement.reset(static_cast<cmajor::fault::tolerant::ast::DefaultStatementNode*>(defaultStatementMatch.value));
                switchStatement->SetDefaultStatement(defaultStatement.release());
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    switchStatement->SetRBrace(rbrace.release());
    switchStatement->Make();
    return soul::parser::Match(true, switchStatement.release());
}

soul::parser::Match ParseControlStatement(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case LBRACE:
        {
            soul::parser::Match compoundStatementMatch = ParseCompoundStatement(context, lexer);
            return compoundStatementMatch;
        }
        case RETURN:
        {
            soul::parser::Match returnStatementMatch = ParseReturnStatement(context, lexer);
            return returnStatementMatch;
        }
        case IF:
        {
            soul::parser::Match ifStatementMatch = ParseIfStatement(context, lexer);
            return ifStatementMatch;
        }
        case WHILE:
        {
            soul::parser::Match whileStatementMatch = ParseWhileStatement(context, lexer);
            return whileStatementMatch;
        }
        case DO:
        {
            soul::parser::Match doStatementMatch = ParseDoStatement(context, lexer);
            return doStatementMatch;
        }
        case FOR:
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match rangeForStatementMatch = ParseRangeForStatement(context, lexer);
            if (rangeForStatementMatch.hit)
            {
                return rangeForStatementMatch;
            }
            else
            {
                lexer.SetPos(save);
            }
            soul::parser::Match forStatementMatch = ParseForStatement(context, lexer);
            return forStatementMatch;
        }
        case BREAK:
        {
            soul::parser::Match breakStatementMatch = ParseBreakStatement(context, lexer);
            return breakStatementMatch;
        }
        case CONTINUE:
        {
            soul::parser::Match continueStatementMatch = ParseContinueStatement(context, lexer);
            return continueStatementMatch;
        }
        case GOTO:
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match gotoStatementMatch = ParseGotoStatement(context, lexer);
            if (gotoStatementMatch.hit)
            {
                return gotoStatementMatch;
            }
            else
            {
                lexer.SetPos(save);
            }
            soul::parser::Match gotoCaseStatementMatch = ParseGotoCaseStatement(context, lexer);
            if (gotoCaseStatementMatch.hit)
            { 
                return gotoCaseStatementMatch;
            }
            else
            {
                lexer.SetPos(save);
            }
            soul::parser::Match gotoDefaultStatementMatch = ParseGotoDefaultStatement(context, lexer);
            return gotoDefaultStatementMatch;
        }
        case SWITCH:
        {
            soul::parser::Match switchStatementMatch = ParseSwitchStatement(context, lexer);
            return switchStatementMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseDeleteStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::DeleteStatementNode> deleteStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::DeleteKeywordNode> deleteKeyword;
    if (*lexer == DELETE)
    {
        deleteKeyword.reset(new cmajor::fault::tolerant::ast::DeleteKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> ptr;
    soul::parser::Match ptrMatch = ParseExpression(context, lexer);
    if (ptrMatch.hit)
    {
        ptr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(ptrMatch.value));
    }
    else
    {
        ptr.reset(new cmajor::fault::tolerant::ast::ExpressionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::expression));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    deleteStatement.reset(new cmajor::fault::tolerant::ast::DeleteStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    deleteStatement->SetDeleteKeyword(deleteKeyword.release());
    deleteStatement->SetPtr(ptr.release());
    deleteStatement->SetSemicolon(semicolon.release());
    deleteStatement->Make();
    return soul::parser::Match(true, deleteStatement.release());
}

soul::parser::Match ParseDestroyStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::DestroyStatementNode> destroyStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::DestroyKeywordNode> destroyKeyword;
    if (*lexer == DESTROY)
    {
        destroyKeyword.reset(new cmajor::fault::tolerant::ast::DestroyKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> ptr;
    soul::parser::Match ptrMatch = ParseExpression(context, lexer);
    if (ptrMatch.hit)
    {
        ptr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(ptrMatch.value));
    }
    else
    {
        ptr.reset(new cmajor::fault::tolerant::ast::ExpressionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::expression));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    destroyStatement.reset(new cmajor::fault::tolerant::ast::DestroyStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    destroyStatement->SetDestroyKeyword(destroyKeyword.release());
    destroyStatement->SetPtr(ptr.release());
    destroyStatement->SetSemicolon(semicolon.release());
    destroyStatement->Make();
    return soul::parser::Match(true, destroyStatement.release());
}

soul::parser::Match ParseEmptyStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::EmptyStatementNode> emptyStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    emptyStatement.reset(new cmajor::fault::tolerant::ast::EmptyStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    emptyStatement->SetSemicolon(semicolon.release());
    emptyStatement->Make();
    return soul::parser::Match(true, emptyStatement.release());
}

soul::parser::Match ParseThrowStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ThrowStatementNode> throwStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::ThrowKeywordNode> throwKeyword;
    if (*lexer == THROW)
    {
        throwKeyword.reset(new cmajor::fault::tolerant::ast::ThrowKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> exception;
    int64_t save = lexer.GetPos();
    soul::parser::Match exceptionMatch = ParseExpression(context, lexer);
    if (exceptionMatch.hit)
    {
        exception.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exceptionMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    throwStatement.reset(new cmajor::fault::tolerant::ast::ThrowStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    throwStatement->SetThrowKeyword(throwKeyword.release());
    if (exception)
    {
        throwStatement->SetException(exception.release());
    }
    throwStatement->SetSemicolon(semicolon.release());
    throwStatement->Make();
    return soul::parser::Match(true, throwStatement.release());
}

soul::parser::Match ParseCatch(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CatchNode> catchNode;
    std::unique_ptr<cmajor::fault::tolerant::ast::CatchKeywordNode> catchKeyword;
    if (*lexer == CATCH)
    {
        catchKeyword.reset(new cmajor::fault::tolerant::ast::CatchKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifier;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        identifier.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> catchBlock;
    soul::parser::Match catchBlockMatch = ParseCompoundStatement(context, lexer);
    if (catchBlockMatch.hit)
    {
        catchBlock.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(catchBlockMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    catchNode.reset(new cmajor::fault::tolerant::ast::CatchNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    catchNode->SetCatchKeyword(catchKeyword.release());
    catchNode->SetLParen(lparen.release());
    catchNode->SetTypeExpr(typeExpr.release());
    if (identifier)
    {
        catchNode->SetId(identifier.release());
    }
    catchNode->SetRParen(rparen.release());
    catchNode->SetCatchBlock(catchBlock.release());
    catchNode->Make();
    return soul::parser::Match(true, catchNode.release());
}

soul::parser::Match ParseTryStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TryStatementNode> tryStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::TryKeywordNode> tryKeyword;
    if (*lexer == TRY)
    {
        tryKeyword.reset(new cmajor::fault::tolerant::ast::TryKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> tryBlock;
    soul::parser::Match tryBlockMatch = ParseCompoundStatement(context, lexer);
    if (tryBlockMatch.hit)
    {
        tryBlock.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(tryBlockMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CatchNode> catchNode;
    soul::parser::Match catchMatch = ParseCatch(context, lexer);
    if (catchMatch.hit)
    {
        catchNode.reset(static_cast<cmajor::fault::tolerant::ast::CatchNode*>(catchMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    tryStatement.reset(new cmajor::fault::tolerant::ast::TryStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    tryStatement->SetTryKeyword(tryKeyword.release());
    tryStatement->SetTryBlock(tryBlock.release());
    tryStatement->AddCatch(catchNode.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CatchNode> catchNode;
        soul::parser::Match catchMatch = ParseCatch(context, lexer);
        if (catchMatch.hit)
        {
            catchNode.reset(static_cast<cmajor::fault::tolerant::ast::CatchNode*>(catchMatch.value));
            tryStatement->AddCatch(catchNode.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    tryStatement->Make();
    return soul::parser::Match(true, tryStatement.release());
}

soul::parser::Match ParseLabeledStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
    soul::parser::Match statementMatch = ParseStatement(context, lexer);
    if (statementMatch.hit)
    {
        statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LabeledStatementNode> labeledStatement;
    labeledStatement.reset(new cmajor::fault::tolerant::ast::LabeledStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    labeledStatement->SetId(id.release());
    labeledStatement->SetColon(colon.release());
    labeledStatement->SetStatement(statement.release());
    labeledStatement->Make();
    return soul::parser::Match(true, labeledStatement.release());
}

struct ExpressionStatementGuard
{
    ExpressionStatementGuard(Context& context_) : context(context_) { context.PushParsingExpressionStatement(true); }
    ~ExpressionStatementGuard() { context.PopParsingExpressionStatement(); }
    Context& context;
};

soul::parser::Match ParseExpressionStatement(Context& context, Lexer& lexer)
{
    ExpressionStatementGuard guard(context);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expression;
    soul::parser::Match expressionMatch = ParseExpression(context, lexer);
    if (expressionMatch.hit)
    {
        expression.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ExpressionStatementNode> expressionStatement;
    expressionStatement.reset(new cmajor::fault::tolerant::ast::ExpressionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    expressionStatement->SetExpression(expression.release());
    expressionStatement->SetSemicolon(semicolon.release());
    expressionStatement->Make();
    return soul::parser::Match(true, expressionStatement.release());
}

struct ParsingLvalueGuard
{
    ParsingLvalueGuard(Context& context_) : context(context_) { context.PushParsingLvalue(true); }
    ~ParsingLvalueGuard() { context.PopParsingLvalue(); }
    Context& context;
};

soul::parser::Match ParseAssignmentStatementNoSemicolon(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> left;
    soul::parser::Match leftMatch(false);
    {
        ParsingLvalueGuard guard(context);
        leftMatch = ParseExpression(context, lexer);
    }
    if (leftMatch.hit)
    {
        left.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
    if (*lexer == ASSIGN)
    {
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> right;
    soul::parser::Match rightMatch = ParseExpression(context, lexer);
    if (rightMatch.hit)
    {
        right.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value));
    }
    else
    {
        right.reset(new cmajor::fault::tolerant::ast::ExpressionCompletionNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::expression));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignmentStatementNode> assignmentStatement;
    assignmentStatement.reset(new cmajor::fault::tolerant::ast::AssignmentStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    assignmentStatement->SetLeft(left.release());
    assignmentStatement->SetAssign(assign.release());
    assignmentStatement->SetRight(right.release());
    assignmentStatement->Make();
    return soul::parser::Match(true, assignmentStatement.release());
}

soul::parser::Match ParseAssignmentStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignmentStatementNode> assignmentStatement;
    soul::parser::Match assignmentStatementMatch = ParseAssignmentStatementNoSemicolon(context, lexer);
    if (assignmentStatementMatch.hit)
    {
        assignmentStatement.reset(static_cast<cmajor::fault::tolerant::ast::AssignmentStatementNode*>(assignmentStatementMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    assignmentStatement->SetSemicolon(semicolon.release()); 
    assignmentStatement->Make();
    return soul::parser::Match(true, assignmentStatement.release());
}

soul::parser::Match ParseConstructionStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstructionStatementNode> constructionStatement;
    constructionStatement.reset(new cmajor::fault::tolerant::ast::ConstructionStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    constructionStatement->SetTypeExpr(typeExpr.release());
    constructionStatement->SetId(id.release());
    int64_t save = lexer.GetPos();
    if (*lexer == ASSIGN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        constructionStatement->SetAssign(assign.release());
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> expression;
        soul::parser::Match expressionMatch = ParseExpression(context, lexer);
        if (expressionMatch.hit)
        {
            expression.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
            constructionStatement->AddNode(expression.release());
        }
        else
        {
            expression.reset(new cmajor::fault::tolerant::ast::ExpressionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            constructionStatement->AddNode(expression.release());
        }
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == LPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
            lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            constructionStatement->SetLParen(lparen.release());
            ParseArgumentList(context, lexer, constructionStatement.get());
            if (*lexer == RPAREN)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
                rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                constructionStatement->SetRParen(rparen.release());
            }
            else
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
                rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
                constructionStatement->SetRParen(rparen.release());
            }
        }
        else
        {
            lexer.SetPos(save);
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    constructionStatement->SetSemicolon(semicolon.release());
    constructionStatement->Make();
    return soul::parser::Match(true, constructionStatement.release());
}

soul::parser::Match ParseAssertStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::HashNode> hash;
    if (*lexer == HASH)
    {
        hash.reset(new cmajor::fault::tolerant::ast::HashNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AssertKeywordNode> assertKeyword;
    if (*lexer == ASSERT)
    {
        assertKeyword.reset(new cmajor::fault::tolerant::ast::AssertKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expression;
    soul::parser::Match expressionMatch = ParseExpression(context, lexer);
    if (expressionMatch.hit)
    {
        expression.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
    }
    else
    {
        expression.reset(new cmajor::fault::tolerant::ast::ExpressionCompletionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AssertStatementNode> assertStatement;
    assertStatement.reset(new cmajor::fault::tolerant::ast::AssertStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    assertStatement->SetHash(hash.release());
    assertStatement->SetAssertKeyword(assertKeyword.release());
    assertStatement->SetExpression(expression.release());
    assertStatement->SetSemicolon(semicolon.release());
    assertStatement->Make();
    return soul::parser::Match(true, assertStatement.release());
}

soul::parser::Match ParseCondCompPrimary(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
        std::unique_ptr<cmajor::fault::tolerant::ast::CondCompSymbolNode> symbol;
        symbol.reset(new cmajor::fault::tolerant::ast::CondCompSymbolNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none, id.release()));
        symbol->Make();
        return soul::parser::Match(true, symbol.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    soul::parser::Match exprMatch = ParseCondCompExpr(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompParenExprNode> parenExpr;
    parenExpr.reset(new cmajor::fault::tolerant::ast::CondCompParenExprNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    parenExpr->SetLParen(lparen.release());
    parenExpr->SetExpr(expr.release());
    parenExpr->SetRParen(rparen.release());
    parenExpr->Make();
    return soul::parser::Match(true, parenExpr.release());
}

soul::parser::Match ParseCondCompPrefix(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    if (*lexer == EXCLAMATION)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::NotNode> notOp;
        notOp.reset(new cmajor::fault::tolerant::ast::NotNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        soul::parser::Match prefixMatch = ParseCondCompPrefix(context, lexer);
        if (prefixMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> prefix;
            prefix.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(prefixMatch.value));
            expr.reset(new cmajor::fault::tolerant::ast::CondCompNotNode(notOp.release(), prefix.release()));
            expr->Make();
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        soul::parser::Match primaryMatch = ParseCondCompPrimary(context, lexer);
        if (primaryMatch.hit)
        {
            expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(primaryMatch.value));
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseCondCompConjunction(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    soul::parser::Match leftMatch = ParseCondCompPrefix(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(leftMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::AmpAmpNode> op;
        if (*lexer == AMPAMP)
        {
            op.reset(new cmajor::fault::tolerant::ast::AmpAmpNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match rightMatch = ParseCondCompPrefix(context, lexer);
        if (rightMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> right;
            right.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(rightMatch.value));
            expr.reset(new cmajor::fault::tolerant::ast::CondCompConjunctionNode(expr.release(), op.release(), right.release()));
            expr->Make();
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseCondCompExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    soul::parser::Match leftMatch = ParseCondCompConjunction(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(leftMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::DisjunctionNode> op;
        if (*lexer == DISJUNCTION)
        {
            op.reset(new cmajor::fault::tolerant::ast::DisjunctionNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match rightMatch = ParseCondCompConjunction(context, lexer);
        if (rightMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> right;
            right.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(rightMatch.value));
            expr.reset(new cmajor::fault::tolerant::ast::CondCompDisjunctionNode(expr.release(), op.release(), right.release()));
            expr->Make();
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseIfGroup(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::HashNode> hash;
    if (*lexer == HASH)
    {
        hash.reset(new cmajor::fault::tolerant::ast::HashNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IfKeywordNode> ifKeyword;
    if (*lexer == IF)
    {
        ifKeyword.reset(new cmajor::fault::tolerant::ast::IfKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    soul::parser::Match exprMatch = ParseCondCompExpr(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(exprMatch.value));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IfGroupNode> ifGroup;
    ifGroup.reset(new cmajor::fault::tolerant::ast::IfGroupNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    ifGroup->SetHash(hash.release());
    ifGroup->SetIfKeyword(ifKeyword.release());
    ifGroup->SetLParen(lparen.release());
    ifGroup->SetExpr(expr.release());
    ifGroup->SetRParen(rparen.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            ifGroup->AddStatement(statement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    ifGroup->Make();
    return soul::parser::Match(true, ifGroup.release());
}

soul::parser::Match ParseElifGroup(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::HashNode> hash;
    if (*lexer == HASH)
    {
        hash.reset(new cmajor::fault::tolerant::ast::HashNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ElifKeywordNode> elifKeyword;
    if (*lexer == ELIF)
    {
        elifKeyword.reset(new cmajor::fault::tolerant::ast::ElifKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompExprNode> expr;
    soul::parser::Match exprMatch = ParseCondCompExpr(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::CondCompExprNode*>(exprMatch.value));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ElifGroupNode> elifGroup;
    elifGroup.reset(new cmajor::fault::tolerant::ast::ElifGroupNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    elifGroup->SetHash(hash.release());
    elifGroup->SetElifKeyword(elifKeyword.release());
    elifGroup->SetLParen(lparen.release());
    elifGroup->SetExpr(expr.release());
    elifGroup->SetRParen(rparen.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            elifGroup->AddStatement(statement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    elifGroup->Make();
    return soul::parser::Match(true, elifGroup.release());
}

soul::parser::Match ParseElseGroup(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::HashNode> hash;
    if (*lexer == HASH)
    {
        hash.reset(new cmajor::fault::tolerant::ast::HashNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ElseKeywordNode> elseKeyword;
    if (*lexer == ELSE)
    {
        elseKeyword.reset(new cmajor::fault::tolerant::ast::ElseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ElseGroupNode> elseGroup;
    elseGroup.reset(new cmajor::fault::tolerant::ast::ElseGroupNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    elseGroup->SetHash(hash.release());
    elseGroup->SetElseKeyword(elseKeyword.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match statementMatch = ParseStatement(context, lexer);
        if (statementMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StatementNode> statement;
            statement.reset(static_cast<cmajor::fault::tolerant::ast::StatementNode*>(statementMatch.value));
            elseGroup->AddStatement(statement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    elseGroup->Make();
    return soul::parser::Match(true, elseGroup.release());
}

soul::parser::Match ParseConditionalCompilationStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CondCompStatementNode> condCompStatement;
    std::unique_ptr<cmajor::fault::tolerant::ast::IfGroupNode> ifGroup;
    soul::parser::Match ifGroupMatch = ParseIfGroup(context, lexer);
    if (ifGroupMatch.hit)
    {
        ifGroup.reset(static_cast<cmajor::fault::tolerant::ast::IfGroupNode*>(ifGroupMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    condCompStatement.reset(new cmajor::fault::tolerant::ast::CondCompStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    condCompStatement->SetIfGroup(ifGroup.release());
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match elifGroupMatch = ParseElifGroup(context, lexer);
        if (elifGroupMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ElifGroupNode> elifGroup;
            elifGroup.reset(static_cast<cmajor::fault::tolerant::ast::ElifGroupNode*>(elifGroupMatch.value));
            condCompStatement->AddElifGroup(elifGroup.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match elseGroupMatch = ParseElseGroup(context, lexer);
    if (elseGroupMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ElseGroupNode> elsefGroup;
        elsefGroup.reset(static_cast<cmajor::fault::tolerant::ast::ElseGroupNode*>(elseGroupMatch.value));
        condCompStatement->SetElseGroup(elsefGroup.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::HashNode> hash;
    if (*lexer == HASH)
    {
        hash.reset(new cmajor::fault::tolerant::ast::HashNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::EndifKeywordNode> endifKeyword;
    if (*lexer == ENDIF)
    {
        endifKeyword.reset(new cmajor::fault::tolerant::ast::EndifKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    condCompStatement->SetHash(hash.release());
    condCompStatement->SetEndifKeyword(endifKeyword.release());
    condCompStatement->Make();
    return soul::parser::Match(true, condCompStatement.release());
}

soul::parser::Match ParseStatement(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    switch (*lexer)
    {
        case LBRACE: case RETURN: case IF: case WHILE: case DO: case FOR: case BREAK: case CONTINUE: case GOTO: case SWITCH: 
        {
            soul::parser::Match controlStatementMatch = ParseControlStatement(context, lexer);
            return controlStatementMatch;
        }
        case DELETE:
        {
            soul::parser::Match deleteStatementMatch = ParseDeleteStatement(context, lexer);
            return deleteStatementMatch;
        }
        case DESTROY:
        {
            soul::parser::Match destroyStatementMatch = ParseDestroyStatement(context, lexer);
            return destroyStatementMatch;
        }
        case SEMICOLON:
        {
            soul::parser::Match emptyStatementMatch = ParseEmptyStatement(context, lexer);
            return emptyStatementMatch;
        }
        case THROW:
        {
            soul::parser::Match throwStatementMatch = ParseThrowStatement(context, lexer);
            return throwStatementMatch;
        }
        case TRY:
        {
            soul::parser::Match tryStatementMatch = ParseTryStatement(context, lexer);
            return tryStatementMatch;
        }
    }
    soul::parser::Match labeledStatementMatch = ParseLabeledStatement(context, lexer);
    if (labeledStatementMatch.hit)
    {
        return labeledStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match expressionStatementMatch = ParseExpressionStatement(context, lexer);
    if (expressionStatementMatch.hit)
    {
        return expressionStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match assignmentStatementMatch = ParseAssignmentStatement(context, lexer);
    if (assignmentStatementMatch.hit)
    {
        return assignmentStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match constructionStatementMatch = ParseConstructionStatement(context, lexer);
    if (constructionStatementMatch.hit)
    {
        return constructionStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match assertStatementMatch = ParseAssertStatement(context, lexer);
    if (assertStatementMatch.hit)
    {
        return assertStatementMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match conditionalCompilationStatementMatch = ParseConditionalCompilationStatement(context, lexer);
    if (conditionalCompilationStatementMatch.hit)
    {
        return conditionalCompilationStatementMatch;
    }
    return soul::parser::Match(false);
}

} // namespace cmajor::fault::tolerant::parser
