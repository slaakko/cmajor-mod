
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/statement.parser' using soul parser generator spg version 4.1.0

module cmajor.statement.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.expression.parser;
import cmajor.type.expr.parser;
import cmajor.identifier.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::expression::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::identifier::parser;

namespace cmajor::statement::parser {

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Statement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Statement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776705);
    std::unique_ptr<cmajor::ast::StatementNode> labeledStatement;
    std::unique_ptr<cmajor::ast::StatementNode> controlStatement;
    std::unique_ptr<cmajor::ast::StatementNode> expressionStatement;
    std::unique_ptr<cmajor::ast::StatementNode> assignmentStatement;
    std::unique_ptr<cmajor::ast::StatementNode> constructionStatement;
    std::unique_ptr<cmajor::ast::StatementNode> deleteStatement;
    std::unique_ptr<cmajor::ast::StatementNode> destroyStatement;
    std::unique_ptr<cmajor::ast::StatementNode> emptyStatement;
    std::unique_ptr<cmajor::ast::StatementNode> throwStatement;
    std::unique_ptr<cmajor::ast::StatementNode> tryStatement;
    std::unique_ptr<cmajor::ast::StatementNode> assertStatement;
    std::unique_ptr<cmajor::ast::StatementNode> condCompStatement;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t save = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                switch (*lexer)
                                                {
                                                    case ID:
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = StatementParser<LexerT>::LabeledStatement(lexer, context);
                                                            labeledStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                                    #endif
                                                                    return soul::parser::Match(true, labeledStatement.release());
                                                                }
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            *parentMatch11 = match;
                                                        }
                                                        break;
                                                    }
                                                    case BREAK:
                                                    case CONTINUE:
                                                    case DO:
                                                    case FOR:
                                                    case GOTO:
                                                    case IF:
                                                    case LBRACE:
                                                    case RETURN:
                                                    case SWITCH:
                                                    case WHILE:
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = StatementParser<LexerT>::ControlStatement(lexer, context);
                                                            controlStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                                    #endif
                                                                    return soul::parser::Match(true, controlStatement.release());
                                                                }
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            *parentMatch11 = match;
                                                        }
                                                        break;
                                                    }
                                                }
                                                *parentMatch10 = match;
                                                if (!match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch15 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = StatementParser<LexerT>::ExpressionStatement(lexer, context);
                                                            expressionStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                                    #endif
                                                                    return soul::parser::Match(true, expressionStatement.release());
                                                                }
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                            }
                                            *parentMatch9 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch16 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch17 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = StatementParser<LexerT>::AssignmentStatement(lexer, context);
                                                        assignmentStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                                #endif
                                                                return soul::parser::Match(true, assignmentStatement.release());
                                                            }
                                                        }
                                                        *parentMatch17 = match;
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
                                        }
                                        *parentMatch8 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch19 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = StatementParser<LexerT>::ConstructionStatement(lexer, context);
                                                    constructionStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                            #endif
                                                            return soul::parser::Match(true, constructionStatement.release());
                                                        }
                                                    }
                                                    *parentMatch19 = match;
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                    }
                                    *parentMatch7 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch20 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch21 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = StatementParser<LexerT>::DeleteStatement(lexer, context);
                                                deleteStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    {
                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                        #endif
                                                        return soul::parser::Match(true, deleteStatement.release());
                                                    }
                                                }
                                                *parentMatch21 = match;
                                            }
                                            *parentMatch20 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                }
                                *parentMatch6 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch22 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch23 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = StatementParser<LexerT>::DestroyStatement(lexer, context);
                                            destroyStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                    #endif
                                                    return soul::parser::Match(true, destroyStatement.release());
                                                }
                                            }
                                            *parentMatch23 = match;
                                        }
                                        *parentMatch22 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch24 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch25 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::EmptyStatement(lexer, context);
                                        emptyStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                #endif
                                                return soul::parser::Match(true, emptyStatement.release());
                                            }
                                        }
                                        *parentMatch25 = match;
                                    }
                                    *parentMatch24 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch26 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch27 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::ThrowStatement(lexer, context);
                                    throwStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                            #endif
                                            return soul::parser::Match(true, throwStatement.release());
                                        }
                                    }
                                    *parentMatch27 = match;
                                }
                                *parentMatch26 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch28 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch29 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = StatementParser<LexerT>::TryStatement(lexer, context);
                                tryStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                        #endif
                                        return soul::parser::Match(true, tryStatement.release());
                                    }
                                }
                                *parentMatch29 = match;
                            }
                            *parentMatch28 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch30 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch31 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::AssertStatement(lexer, context);
                            assertStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                    #endif
                                    return soul::parser::Match(true, assertStatement.release());
                                }
                            }
                            *parentMatch31 = match;
                        }
                        *parentMatch30 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch32 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch33 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationStatement(lexer, context);
                        condCompStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                #endif
                                return soul::parser::Match(true, condCompStatement.release());
                            }
                        }
                        *parentMatch33 = match;
                    }
                    *parentMatch32 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
        else soul::lexer::WriteFailureToLog(lexer, "Statement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Label(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Label");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776706);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::u32string label = std::u32string();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == ID)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                        label = lexer.GetToken(pos).ToString();
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == COLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
                #endif
                return soul::parser::Match(true, new cmajor::ast::LabelNode(s, context->ModuleId(), label));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
        else soul::lexer::WriteFailureToLog(lexer, "Label");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::LabeledStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LabeledStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776707);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::LabeledStatementNode> statementNode = std::unique_ptr<cmajor::ast::LabeledStatementNode>();
    std::unique_ptr<cmajor::ast::LabelNode> lbl;
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = StatementParser<LexerT>::Label(lexer, context);
                    lbl.reset(static_cast<cmajor::ast::LabelNode*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                    stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            statementNode.reset(new cmajor::ast::LabeledStatementNode(s, context->ModuleId(), stmt.release()));
            statementNode->SetLabelNode(lbl.release());
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
                #endif
                return soul::parser::Match(true, statementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
        else soul::lexer::WriteFailureToLog(lexer, "LabeledStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ControlStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ControlStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776708);
    std::unique_ptr<cmajor::ast::CompoundStatementNode> compoundStatement;
    std::unique_ptr<cmajor::ast::StatementNode> returnStatement;
    std::unique_ptr<cmajor::ast::StatementNode> ifStatement;
    std::unique_ptr<cmajor::ast::StatementNode> whileStatement;
    std::unique_ptr<cmajor::ast::StatementNode> doStatement;
    std::unique_ptr<cmajor::ast::StatementNode> rangeForStatement;
    std::unique_ptr<cmajor::ast::StatementNode> forStatement;
    std::unique_ptr<cmajor::ast::StatementNode> breakStatement;
    std::unique_ptr<cmajor::ast::StatementNode> continueStatement;
    std::unique_ptr<cmajor::ast::StatementNode> gotoStatement;
    std::unique_ptr<cmajor::ast::StatementNode> switchStatement;
    std::unique_ptr<cmajor::ast::StatementNode> gotoCaseStatement;
    std::unique_ptr<cmajor::ast::StatementNode> gotoDefaultStatement;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case DO:
                    case FOR:
                    case IF:
                    case LBRACE:
                    case RETURN:
                    case WHILE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            switch (*lexer)
                            {
                                case LBRACE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch6 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                        compoundStatement.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, compoundStatement.release());
                                            }
                                        }
                                        *parentMatch6 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case RETURN:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::ReturnStatement(lexer, context);
                                        returnStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, returnStatement.release());
                                            }
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case IF:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::IfStatement(lexer, context);
                                        ifStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, ifStatement.release());
                                            }
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case WHILE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::WhileStatement(lexer, context);
                                        whileStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, whileStatement.release());
                                            }
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case DO:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::DoStatement(lexer, context);
                                        doStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, doStatement.release());
                                            }
                                        }
                                        *parentMatch10 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case FOR:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::RangeForStatement(lexer, context);
                                        rangeForStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, rangeForStatement.release());
                                            }
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                            }
                            *parentMatch4 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::ForStatement(lexer, context);
                                        forStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                                #endif
                                                return soul::parser::Match(true, forStatement.release());
                                            }
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch12 = match;
                                }
                                *parentMatch4 = match;
                            }
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case BREAK:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::BreakStatement(lexer, context);
                            breakStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                    #endif
                                    return soul::parser::Match(true, breakStatement.release());
                                }
                            }
                            *parentMatch14 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case CONTINUE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::ContinueStatement(lexer, context);
                            continueStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                    #endif
                                    return soul::parser::Match(true, continueStatement.release());
                                }
                            }
                            *parentMatch15 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case GOTO:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch16 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::GotoStatement(lexer, context);
                            gotoStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                    #endif
                                    return soul::parser::Match(true, gotoStatement.release());
                                }
                            }
                            *parentMatch16 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case SWITCH:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::SwitchStatement(lexer, context);
                            switchStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                    #endif
                                    return soul::parser::Match(true, switchStatement.release());
                                }
                            }
                            *parentMatch17 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch18 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch19 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::GotoCaseStatement(lexer, context);
                            gotoCaseStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                    #endif
                                    return soul::parser::Match(true, gotoCaseStatement.release());
                                }
                            }
                            *parentMatch19 = match;
                        }
                        *parentMatch18 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch20 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch21 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = StatementParser<LexerT>::GotoDefaultStatement(lexer, context);
                        gotoDefaultStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
                                #endif
                                return soul::parser::Match(true, gotoDefaultStatement.release());
                            }
                        }
                        *parentMatch21 = match;
                    }
                    *parentMatch20 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ControlStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ControlStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CompoundStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompoundStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776709);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::CompoundStatementNode> compoundStatementNode = std::unique_ptr<cmajor::ast::CompoundStatementNode>();
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == LBRACE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                            compoundStatementNode.reset(new cmajor::ast::CompoundStatementNode(s, context->ModuleId()));
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                            stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                            if (match.hit)
                                            {
                                                compoundStatementNode->AddStatement(stmt.release());
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch6 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                        break;
                                    }
                                }
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
                #endif
                return soul::parser::Match(true, compoundStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
        else soul::lexer::WriteFailureToLog(lexer, "CompoundStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ReturnStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ReturnStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776710);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == RETURN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            expr.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ReturnStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ReturnStatementNode(s, context->ModuleId(), expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ReturnStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ReturnStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::IfStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IfStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776711);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> cond;
    std::unique_ptr<cmajor::ast::StatementNode> thenS;
    std::unique_ptr<cmajor::ast::StatementNode> elseS;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == IF)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        s = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == LPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                cond.reset(static_cast<cmajor::ast::Node*>(match.value));
                                *parentMatch9 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                        thenS.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                        *parentMatch11 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch13 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == ELSE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch15 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch16 = &match;
                                {
                                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                    elseS.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch13 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
                    }
                    *parentMatch12 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IfStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::IfStatementNode(s, context->ModuleId(), cond.release(), thenS.release(), elseS.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IfStatement");
        else soul::lexer::WriteFailureToLog(lexer, "IfStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::WhileStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "WhileStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776712);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> cond;
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == WHILE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    s = lexer.GetSourcePos(pos);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            cond.reset(static_cast<cmajor::ast::Node*>(match.value));
                            *parentMatch8 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch10 = &match;
                {
                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                    stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    *parentMatch10 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhileStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::WhileStatementNode(s, context->ModuleId(), cond.release(), stmt.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhileStatement");
        else soul::lexer::WriteFailureToLog(lexer, "WhileStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DoStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DoStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776713);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    std::unique_ptr<cmajor::ast::Node> cond;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == DO)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            s = lexer.GetSourcePos(pos);
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                        stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == WHILE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            cond.reset(static_cast<cmajor::ast::Node*>(match.value));
                            *parentMatch12 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch13 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch13 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch14 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch14 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::DoStatementNode(s, context->ModuleId(), stmt.release(), cond.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DoStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::RangeForStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RangeForStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776714);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::Node> container;
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == FOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                s = lexer.GetSourcePos(pos);
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                        type.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                    id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                    *parentMatch12 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            container.reset(static_cast<cmajor::ast::Node*>(match.value));
                            *parentMatch14 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch15 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch15 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch16 = &match;
                {
                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                    stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    *parentMatch16 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RangeForStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::RangeForStatementNode(s, context->ModuleId(), type.release(), id.release(), container.release(), stmt.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RangeForStatement");
        else soul::lexer::WriteFailureToLog(lexer, "RangeForStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776715);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::StatementNode> init;
    std::unique_ptr<cmajor::ast::Node> cond;
    std::unique_ptr<cmajor::ast::StatementNode> loop;
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == FOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                s = lexer.GetSourcePos(pos);
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match = StatementParser<LexerT>::ForInitStatement(lexer, context);
                                        init.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                        cond.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            *parentMatch13 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch12 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch14 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == SEMICOLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch14 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            soul::parser::Match match = StatementParser<LexerT>::ForLoopStatementExpr(lexer, context);
                            loop.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            *parentMatch15 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch17 = &match;
                {
                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                    stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    *parentMatch17 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ForStatementNode(s, context->ModuleId(), init.release(), cond.release(), loop.release(), stmt.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ForStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForInitStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForInitStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776716);
    std::unique_ptr<cmajor::ast::StatementNode> assignmentStatement;
    std::unique_ptr<cmajor::ast::StatementNode> constructionStatement;
    std::unique_ptr<cmajor::ast::StatementNode> emptyStatement;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = StatementParser<LexerT>::AssignmentStatement(lexer, context);
                    assignmentStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForInitStatement");
                            #endif
                            return soul::parser::Match(true, assignmentStatement.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::ConstructionStatement(lexer, context);
                            constructionStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForInitStatement");
                                    #endif
                                    return soul::parser::Match(true, constructionStatement.release());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = StatementParser<LexerT>::EmptyStatement(lexer, context);
                        emptyStatement.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForInitStatement");
                                #endif
                                return soul::parser::Match(true, emptyStatement.release());
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForInitStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ForInitStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForLoopStatementExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForLoopStatementExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776717);
    std::unique_ptr<cmajor::ast::StatementNode> assignmentStatementExpr;
    std::unique_ptr<cmajor::ast::Node> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = StatementParser<LexerT>::AssignmentStatementExpr(lexer, context);
                    assignmentStatementExpr.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForLoopStatementExpr");
                            #endif
                            return soul::parser::Match(true, assignmentStatementExpr.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            expr.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForLoopStatementExpr");
                                    #endif
                                    return soul::parser::Match(true, new cmajor::ast::ExpressionStatementNode(lexer.GetSourcePos(pos), context->ModuleId(), expr.release()));
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForLoopStatementExpr");
                                #endif
                                return soul::parser::Match(true, new cmajor::ast::EmptyStatementNode(lexer.GetSourcePos(pos), context->ModuleId()));
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForLoopStatementExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ForLoopStatementExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::BreakStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BreakStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776718);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == BREAK)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BreakStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::BreakStatementNode(s, context->ModuleId()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BreakStatement");
        else soul::lexer::WriteFailureToLog(lexer, "BreakStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ContinueStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ContinueStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776719);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == CONTINUE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContinueStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ContinueStatementNode(s, context->ModuleId()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContinueStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ContinueStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::GotoStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GotoStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776720);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::u32string target = std::u32string();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == GOTO)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == ID)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                target = lexer.GetToken(pos).ToString();
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::GotoStatementNode(s, context->ModuleId(), target));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoStatement");
        else soul::lexer::WriteFailureToLog(lexer, "GotoStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::SwitchStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SwitchStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776721);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::SwitchStatementNode> switchStatementNode = std::unique_ptr<cmajor::ast::SwitchStatementNode>();
    std::unique_ptr<cmajor::ast::Node> cond;
    std::unique_ptr<cmajor::ast::CaseStatementNode> caseStatement;
    std::unique_ptr<cmajor::ast::DefaultStatementNode> defaultStatement;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == SWITCH)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            s = lexer.GetSourcePos(pos);
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == LPAREN)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                    cond.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    *parentMatch10 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        switchStatementNode.reset(new cmajor::ast::SwitchStatementNode(s, context->ModuleId(), cond.release()));
                                    }
                                    *parentMatch12 = match;
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch13 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == LBRACE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch13 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch14 = &match;
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch17 = &match;
                                        {
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = StatementParser<LexerT>::CaseStatement(lexer, context);
                                                caseStatement.reset(static_cast<cmajor::ast::CaseStatementNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    switchStatementNode->AddCase(caseStatement.release());
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch17 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch19 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch20 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = StatementParser<LexerT>::DefaultStatement(lexer, context);
                                                        defaultStatement.reset(static_cast<cmajor::ast::DefaultStatementNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            switchStatementNode->SetDefault(defaultStatement.release());
                                                        }
                                                        *parentMatch20 = match;
                                                    }
                                                    *parentMatch19 = match;
                                                }
                                                *parentMatch17 = match;
                                            }
                                        }
                                        *parentMatch16 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch15 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                        break;
                                    }
                                }
                            }
                        }
                        *parentMatch14 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch21 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch21 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchStatement");
                #endif
                return soul::parser::Match(true, switchStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchStatement");
        else soul::lexer::WriteFailureToLog(lexer, "SwitchStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CaseStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CaseStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776722);
    std::unique_ptr<cmajor::ast::CaseStatementNode> caseStatementNode = std::unique_ptr<cmajor::ast::CaseStatementNode>();
    std::unique_ptr<cmajor::ast::Node> caseExpr;
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            caseStatementNode.reset(new cmajor::ast::CaseStatementNode(lexer.GetSourcePos(pos), context->ModuleId()));
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == CASE)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                caseExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    caseStatementNode->AddCaseExpr(caseExpr.release());
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(true);
                            soul::parser::Match* parentMatch13 = &match;
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch15 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == CASE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch18 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                        caseExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            caseStatementNode->AddCaseExpr(caseExpr.release());
                                                        }
                                                        *parentMatch18 = match;
                                                    }
                                                    *parentMatch17 = match;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch19 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == COLON)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch19 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch14 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch13 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                        break;
                                    }
                                }
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch20 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch21 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch22 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch23 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                        stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            caseStatementNode->AddStatement(stmt.release());
                                        }
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch22 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch21 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch20 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CaseStatement");
                #endif
                return soul::parser::Match(true, caseStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CaseStatement");
        else soul::lexer::WriteFailureToLog(lexer, "CaseStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DefaultStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DefaultStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776723);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::DefaultStatementNode> defaultStatementNode = std::unique_ptr<cmajor::ast::DefaultStatementNode>();
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == DEFAULT)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == COLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                defaultStatementNode.reset(new cmajor::ast::DefaultStatementNode(s, context->ModuleId()));
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                        stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                        if (match.hit)
                                        {
                                            defaultStatementNode->AddStatement(stmt.release());
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch8 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefaultStatement");
                #endif
                return soul::parser::Match(true, defaultStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefaultStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DefaultStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::GotoCaseStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GotoCaseStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776724);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> caseExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == GOTO)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSourcePos(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == CASE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                        caseExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoCaseStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::GotoCaseStatementNode(s, context->ModuleId(), caseExpr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoCaseStatement");
        else soul::lexer::WriteFailureToLog(lexer, "GotoCaseStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::GotoDefaultStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GotoDefaultStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776725);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == GOTO)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == DEFAULT)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoDefaultStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::GotoDefaultStatementNode(s, context->ModuleId()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoDefaultStatement");
        else soul::lexer::WriteFailureToLog(lexer, "GotoDefaultStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ExpressionStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExpressionStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776726);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            context->PushParsingExpressionStatement(true);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            expr.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                s = lexer.GetSourcePos(pos);
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->PopParsingExpressionStatement();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExpressionStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ExpressionStatementNode(s, context->ModuleId(), expr.release()));
            }
        }
        else
        {
            context->PopParsingExpressionStatement();
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExpressionStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ExpressionStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::AssignmentStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AssignmentStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776727);
    std::unique_ptr<cmajor::ast::StatementNode> stmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match = StatementParser<LexerT>::AssignmentStatementExpr(lexer, context);
                stmt.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssignmentStatement");
                #endif
                return soul::parser::Match(true, stmt.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssignmentStatement");
        else soul::lexer::WriteFailureToLog(lexer, "AssignmentStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::AssignmentStatementExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AssignmentStatementExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776728);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> target;
    std::unique_ptr<cmajor::ast::Node> source;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                context->PushParsingLvalue(true);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                target.reset(static_cast<cmajor::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    s = lexer.GetSourcePos(pos);
                                    context->PopParsingLvalue();
                                }
                                else
                                {
                                    context->PopParsingLvalue();
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == ASSIGN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                    source.reset(static_cast<cmajor::ast::Node*>(match.value));
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssignmentStatementExpr");
                #endif
                return soul::parser::Match(true, new cmajor::ast::AssignmentStatementNode(s, context->ModuleId(), target.release(), source.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssignmentStatementExpr");
        else soul::lexer::WriteFailureToLog(lexer, "AssignmentStatementExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConstructionStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructionStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776729);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConstructionStatementNode> constructionStatementNode = std::unique_ptr<cmajor::ast::ConstructionStatementNode>();
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::Node> value;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                            type.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                s = lexer.GetSourcePos(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                if (match.hit)
                                {
                                    constructionStatementNode.reset(new cmajor::ast::ConstructionStatementNode(s, context->ModuleId(), type.release(), id.release()));
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                switch (*lexer)
                                {
                                    case ASSIGN:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == ASSIGN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch14 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                    value.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        constructionStatementNode->AddArgument(value.release());
                                                        constructionStatementNode->SetAssignment();
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch11 = match;
                                        }
                                        break;
                                    }
                                    case LPAREN:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch15 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == LPAREN)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, constructionStatementNode.get());
                                                    *parentMatch17 = match;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == RPAREN)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch11 = match;
                                        }
                                        break;
                                    }
                                }
                                *parentMatch10 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch19 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch20 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(true);
                                            if (match.hit)
                                            {
                                                constructionStatementNode->SetEmpty();
                                            }
                                            *parentMatch20 = match;
                                        }
                                        *parentMatch19 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch21 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch21 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructionStatement");
                #endif
                return soul::parser::Match(true, constructionStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructionStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructionStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DeleteStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeleteStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776730);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> ptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == DELETE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                        ptr.reset(static_cast<cmajor::ast::Node*>(match.value));
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeleteStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::DeleteStatementNode(s, context->ModuleId(), ptr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeleteStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DeleteStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DestroyStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DestroyStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776731);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> ptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == DESTROY)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                        ptr.reset(static_cast<cmajor::ast::Node*>(match.value));
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestroyStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::DestroyStatementNode(s, context->ModuleId(), ptr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestroyStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DestroyStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::EmptyStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EmptyStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776732);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SEMICOLON)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    s = lexer.GetSourcePos(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EmptyStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::EmptyStatementNode(s, context->ModuleId()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EmptyStatement");
        else soul::lexer::WriteFailureToLog(lexer, "EmptyStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ThrowStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ThrowStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776733);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> exception;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == THROW)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            exception.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ThrowStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ThrowStatementNode(s, context->ModuleId(), exception.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ThrowStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ThrowStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::TryStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TryStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776734);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::TryStatementNode> tryStatementNode = std::unique_ptr<cmajor::ast::TryStatementNode>();
    std::unique_ptr<cmajor::ast::CompoundStatementNode> tryBlock;
    std::unique_ptr<cmajor::ast::CatchNode> handler;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == TRY)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSourcePos(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                            tryBlock.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                            if (match.hit)
                            {
                                tryStatementNode.reset(new cmajor::ast::TryStatementNode(s, context->ModuleId(), tryBlock.release()));
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = StatementParser<LexerT>::Catch(lexer, context);
                                handler.reset(static_cast<cmajor::ast::CatchNode*>(match.value));
                                if (match.hit)
                                {
                                    tryStatementNode->AddCatch(handler.release());
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch8 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch11 = &match;
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = StatementParser<LexerT>::Catch(lexer, context);
                                        handler.reset(static_cast<cmajor::ast::CatchNode*>(match.value));
                                        if (match.hit)
                                        {
                                            tryStatementNode->AddCatch(handler.release());
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch12 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch11 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TryStatement");
                #endif
                return soul::parser::Match(true, tryStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TryStatement");
        else soul::lexer::WriteFailureToLog(lexer, "TryStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Catch(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Catch");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776735);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> catchType;
    std::unique_ptr<cmajor::ast::IdentifierNode> catchId;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> catchBlock;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == CATCH)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        s = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == LPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                catchType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                *parentMatch9 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                catchId.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                if (match.hit)
                                {
                                    *parentMatch11 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                {
                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                    catchBlock.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                    *parentMatch13 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Catch");
                #endif
                return soul::parser::Match(true, new cmajor::ast::CatchNode(s, context->ModuleId(), catchType.release(), catchId.release(), catchBlock.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Catch");
        else soul::lexer::WriteFailureToLog(lexer, "Catch");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::AssertStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AssertStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776736);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::Node> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == HASH)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSourcePos(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == ASSERT)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                        expr.reset(static_cast<cmajor::ast::Node*>(match.value));
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssertStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::AssertStatementNode(s, context->ModuleId(), expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AssertStatement");
        else soul::lexer::WriteFailureToLog(lexer, "AssertStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776737);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    soul::ast::SourcePos elifPos = soul::ast::SourcePos();
    soul::ast::SourcePos elsePos = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConditionalCompilationStatementNode> condCompStmt = std::unique_ptr<cmajor::ast::ConditionalCompilationStatementNode>();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> ifExpr;
    std::unique_ptr<cmajor::ast::StatementNode> ifs;
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> elifExpr;
    std::unique_ptr<cmajor::ast::StatementNode> elifs;
    std::unique_ptr<cmajor::ast::StatementNode> elses;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == HASH)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    s = lexer.GetSourcePos(pos);
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == IF)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch14 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationExpression(lexer, context);
                                            ifExpr.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                                            if (match.hit)
                                            {
                                                condCompStmt.reset(new cmajor::ast::ConditionalCompilationStatementNode(s, context->ModuleId(), ifExpr.release()));
                                            }
                                            *parentMatch14 = match;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch16 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch17 = &match;
                                {
                                    while (true)
                                    {
                                        int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch19 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                                    ifs.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        condCompStmt->AddIfStatement(ifs.release());
                                                    }
                                                    *parentMatch19 = match;
                                                }
                                                *parentMatch18 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch17 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            soul::parser::Match match(true);
                            soul::parser::Match* parentMatch21 = &match;
                            {
                                while (true)
                                {
                                    int64_t save = lexer.GetPos();
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch22 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch23 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch24 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch25 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch26 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch27 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch28 = &match;
                                                                {
                                                                    int64_t pos = lexer.GetPos();
                                                                    soul::parser::Match match(false);
                                                                    if (*lexer == HASH)
                                                                    {
                                                                        ++lexer;
                                                                        match.hit = true;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        elifPos = lexer.GetSourcePos(pos);
                                                                    }
                                                                    *parentMatch28 = match;
                                                                }
                                                                *parentMatch27 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch29 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    if (*lexer == ELIF)
                                                                    {
                                                                        ++lexer;
                                                                        match.hit = true;
                                                                    }
                                                                    *parentMatch29 = match;
                                                                }
                                                                *parentMatch27 = match;
                                                            }
                                                            *parentMatch26 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch30 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                if (*lexer == LPAREN)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                *parentMatch30 = match;
                                                            }
                                                            *parentMatch26 = match;
                                                        }
                                                        *parentMatch25 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch31 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch32 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationExpression(lexer, context);
                                                                elifExpr.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    condCompStmt->AddElifExpr(elifPos, context->ModuleId(), elifExpr.release());
                                                                }
                                                                *parentMatch32 = match;
                                                            }
                                                            *parentMatch31 = match;
                                                        }
                                                        *parentMatch25 = match;
                                                    }
                                                    *parentMatch24 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch33 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        if (*lexer == RPAREN)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        *parentMatch33 = match;
                                                    }
                                                    *parentMatch24 = match;
                                                }
                                                *parentMatch23 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch34 = &match;
                                                {
                                                    soul::parser::Match match(true);
                                                    soul::parser::Match* parentMatch35 = &match;
                                                    {
                                                        while (true)
                                                        {
                                                            int64_t save = lexer.GetPos();
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch36 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch37 = &match;
                                                                    {
                                                                        int64_t pos = lexer.GetPos();
                                                                        soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                                                        elifs.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            condCompStmt->AddElifStatement(elifs.release());
                                                                        }
                                                                        *parentMatch37 = match;
                                                                    }
                                                                    *parentMatch36 = match;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    *parentMatch35 = match;
                                                                }
                                                                else
                                                                {
                                                                    lexer.SetPos(save);
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    *parentMatch34 = match;
                                                }
                                                *parentMatch23 = match;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch21 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                            break;
                                        }
                                    }
                                }
                            }
                            *parentMatch20 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch38 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch39 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch40 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch41 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch42 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch43 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == HASH)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                elsePos = lexer.GetSourcePos(pos);
                                            }
                                            *parentMatch43 = match;
                                        }
                                        *parentMatch42 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch44 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == ELSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch44 = match;
                                        }
                                        *parentMatch42 = match;
                                    }
                                    *parentMatch41 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch45 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        soul::parser::Match* parentMatch46 = &match;
                                        {
                                            while (true)
                                            {
                                                int64_t save = lexer.GetPos();
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch47 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch48 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
                                                            elses.reset(static_cast<cmajor::ast::StatementNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                condCompStmt->AddElseStatement(elsePos, context->ModuleId(), elses.release());
                                                            }
                                                            *parentMatch48 = match;
                                                        }
                                                        *parentMatch47 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch46 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.SetPos(save);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        *parentMatch45 = match;
                                    }
                                    *parentMatch41 = match;
                                }
                                *parentMatch40 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch39 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch38 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch49 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch50 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch51 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == HASH)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch51 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch52 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == ENDIF)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch52 = match;
                            }
                            *parentMatch51 = match;
                        }
                        *parentMatch50 = match;
                    }
                    *parentMatch49 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationStatement");
                #endif
                return soul::parser::Match(true, condCompStmt.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationExpression(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationExpression");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776738);
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> disjunction;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationDisjunction(lexer, context);
            disjunction.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationExpression");
                #endif
                return soul::parser::Match(true, disjunction.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationExpression");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationExpression");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationDisjunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationDisjunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776739);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> expr = std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode>();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> left;
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> right;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationConjunction(lexer, context);
                    left.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch6 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == DISJUNCTION)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationConjunction(lexer, context);
                                                right.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::ConditionalCompilationDisjunctionNode(s, context->ModuleId(), expr.release(), right.release()));
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationDisjunction");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationDisjunction");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationDisjunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationConjunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationConjunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776740);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> expr = std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode>();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> left;
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> right;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationPrefix(lexer, context);
                    left.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSourcePos(pos);
                        expr.reset(left.release());
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch6 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == AMPAMP)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationPrefix(lexer, context);
                                                right.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::ConditionalCompilationConjunctionNode(s, context->ModuleId(), expr.release(), right.release()));
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch5 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationConjunction");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationConjunction");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationConjunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationPrefix(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationPrefix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776741);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> expr = std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode>();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> prefix;
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> primary;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case EXCLAMATION:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == EXCLAMATION)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    s = lexer.GetSourcePos(pos);
                                }
                                *parentMatch5 = match;
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationPrefix(lexer, context);
                                    prefix.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                                    if (match.hit)
                                    {
                                        expr.reset(new cmajor::ast::ConditionalCompilationNotNode(s, context->ModuleId(), prefix.release()));
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case ID:
                    case LPAREN:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationPrimary(lexer, context);
                            primary.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                            if (match.hit)
                            {
                                expr.reset(primary.release());
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationPrefix");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationPrefix");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationPrefix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ConditionalCompilationPrimary(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConditionalCompilationPrimary");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1627074267608776742);
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::ConditionalCompilationExpressionNode> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        switch (*lexer)
        {
            case ID:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == ID)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationPrimary");
                            #endif
                            return soul::parser::Match(true, new cmajor::ast::ConditionalCompilationPrimaryNode(lexer.GetSourcePos(pos), context->ModuleId(), lexer.GetToken(pos).ToString()));
                        }
                    }
                    *parentMatch2 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case LPAREN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSourcePos(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match = StatementParser<LexerT>::ConditionalCompilationExpression(lexer, context);
                            expr.reset(static_cast<cmajor::ast::ConditionalCompilationExpressionNode*>(match.value));
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationPrimary");
                                    #endif
                                    return soul::parser::Match(true, new cmajor::ast::ParenthesizedConditionalCompilationExpressionNode(s, context->ModuleId(), expr.release()));
                                }
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConditionalCompilationPrimary");
        else soul::lexer::WriteFailureToLog(lexer, "ConditionalCompilationPrimary");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct StatementParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::statement::parser
