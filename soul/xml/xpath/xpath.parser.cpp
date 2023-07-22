
// this file has been automatically generated from 'C:/work/cmajor-mod/soul/xml/xpath/xpath.parser' using soul parser generator spg version 5.0.0

module soul.xml.xpath.parser;

import util;
import soul.ast.spg;
import soul.xml.xpath.lexer;
import soul.xml.xpath.token;
import soul.xml.xpath.token.parser;

using namespace soul::xml::xpath::lexer;
using namespace soul::xml::xpath::token;
using namespace soul::xml::xpath::token::parser;

namespace soul::xml::xpath::parser {

template<typename LexerT>
std::unique_ptr<soul::xml::xpath::expr::Expr> XPathParser<LexerT>::Parse(LexerT& lexer)
{
    std::unique_ptr<soul::xml::xpath::expr::Expr> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = XPathParser<LexerT>::Expr(lexer);
    value.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->DecIndent();
        lexer.Log()->WriteEndRule("parse");
    }
    #endif
    if (match.hit)
    {
        if (*lexer == soul::lexer::END_TOKEN)
        {
            return value;
        }
        else
        {
            lexer.ThrowFarthestError();
        }
    }
    else
    {
        lexer.ThrowFarthestError();
    }
    return value;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Expr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Expr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127425);
    std::unique_ptr<soul::xml::xpath::expr::Expr> orExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match = XPathParser<LexerT>::OrExpr(lexer);
            orExpr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expr");
                #endif
                return soul::parser::Match(true, orExpr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expr");
        else soul::lexer::WriteFailureToLog(lexer, "Expr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::OrExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OrExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127426);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::AndExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::OrKeyword(lexer);
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
                                                soul::parser::Match match = XPathParser<LexerT>::AndExpr(lexer);
                                                right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::or_, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrExpr");
        else soul::lexer::WriteFailureToLog(lexer, "OrExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AndExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AndExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127427);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::EqualityExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::AndKeyword(lexer);
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
                                                soul::parser::Match match = XPathParser<LexerT>::EqualityExpr(lexer);
                                                right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::and_, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AndExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AndExpr");
        else soul::lexer::WriteFailureToLog(lexer, "AndExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::EqualityExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EqualityExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127428);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::RelationalExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::EqualityOp(lexer);
                                        op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
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
                                                soul::parser::Match match(true);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = XPathParser<LexerT>::RelationalExpr(lexer);
                                                    right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(pos, "RelationalExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(op->value, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqualityExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqualityExpr");
        else soul::lexer::WriteFailureToLog(lexer, "EqualityExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::RelationalExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RelationalExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127429);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::AdditiveExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::RelationalOp(lexer);
                                        op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
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
                                                soul::parser::Match match(true);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = XPathParser<LexerT>::AdditiveExpr(lexer);
                                                    right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(pos, "AdditiveExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(op->value, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalExpr");
        else soul::lexer::WriteFailureToLog(lexer, "RelationalExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AdditiveExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AdditiveExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127430);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::MultiplicativeExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::AdditiveOp(lexer);
                                        op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
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
                                                soul::parser::Match match(true);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = XPathParser<LexerT>::MultiplicativeExpr(lexer);
                                                    right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(pos, "MultiplicativeExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(op->value, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AdditiveExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AdditiveExpr");
        else soul::lexer::WriteFailureToLog(lexer, "AdditiveExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::MultiplicativeExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MultiplicativeExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127431);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::UnaryExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        soul::parser::Match match = XPathParser<LexerT>::MultiplicativeOp(lexer);
                                        op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
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
                                                soul::parser::Match match(true);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = XPathParser<LexerT>::UnaryExpr(lexer);
                                                    right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(pos, "UnaryExpr");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(op->value, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeExpr");
        else soul::lexer::WriteFailureToLog(lexer, "MultiplicativeExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::UnaryExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnaryExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127432);
    std::unique_ptr<soul::xml::xpath::expr::Expr> subject;
    std::unique_ptr<soul::xml::xpath::expr::Expr> unionExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == MINUS)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::UnaryExpr(lexer);
                    subject.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                            #endif
                            return soul::parser::Match(true, new soul::xml::xpath::expr::UnaryExpr(soul::xml::xpath::expr::Operator::minus, subject.release()));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = XPathParser<LexerT>::UnionExpr(lexer);
                    unionExpr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                            #endif
                            return soul::parser::Match(true, unionExpr.release());
                        }
                    }
                    *parentMatch5 = match;
                }
                *parentMatch4 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
        else soul::lexer::WriteFailureToLog(lexer, "UnaryExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::UnionExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnionExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127433);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::PathExpr(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        if (*lexer == UNION)
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
                                                soul::parser::Match match = XPathParser<LexerT>::PathExpr(lexer);
                                                right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::union_, expr.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnionExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnionExpr");
        else soul::lexer::WriteFailureToLog(lexer, "UnionExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::PathExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PathExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127434);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> locationPath;
    std::unique_ptr<soul::xml::xpath::expr::Expr> functionCall;
    std::unique_ptr<soul::xml::xpath::expr::Expr> filterExpr;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    int64_t save = lexer.GetPos();
                    {
                        soul::parser::Match match = XPathParser<LexerT>::LocationPath(lexer);
                        locationPath.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t tmp = lexer.GetPos();
                            lexer.SetPos(save);
                            save = tmp;
                            soul::parser::Match match = XPathParser<LexerT>::FunctionCall(lexer);
                            functionCall.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                            *parentMatch5 = match;
                        }
                        if (!match.hit)
                        {
                            lexer.SetPos(save);
                        }
                        *parentMatch4 = soul::parser::Match(!match.hit, match.value);
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PathExpr");
                        #endif
                        return soul::parser::Match(true, locationPath.release());
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            lexer.SetPos(save);
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
                            soul::parser::Match match = XPathParser<LexerT>::FilterExpr(lexer);
                            filterExpr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                            if (match.hit)
                            {
                                expr.reset(filterExpr.release());
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
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                int64_t pos = lexer.GetPos();
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
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch16 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch17 = &match;
                                                    {
                                                        soul::parser::Match match = XPathParser<LexerT>::CombinePathOp(lexer);
                                                        op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
                                                        *parentMatch17 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch18 = &match;
                                                        {
                                                            soul::parser::Match match = XPathParser<LexerT>::RelativeLocationPath(lexer);
                                                            right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                            *parentMatch18 = match;
                                                        }
                                                        *parentMatch17 = match;
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    switch (op->value)
                                                    {
                                                        case soul::xml::xpath::expr::Operator::slash: {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PathExpr");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::slash, expr.release(), right.release()));
                                                            }
                                                        }
                                                        case soul::xml::xpath::expr::Operator::slashSlash: {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PathExpr");
                                                                #endif
                                                                return soul::parser::Match(true, soul::xml::xpath::expr::MakeSlashSlashExpr(expr.release(), right.release()));
                                                            }
                                                        }
                                                    }
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
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PathExpr");
                                        #endif
                                        return soul::parser::Match(true, expr.release());
                                    }
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch6 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PathExpr");
        else soul::lexer::WriteFailureToLog(lexer, "PathExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::LocationPath(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LocationPath");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127435);
    std::unique_ptr<soul::xml::xpath::expr::Expr> absoluteLocationPath;
    std::unique_ptr<soul::xml::xpath::expr::Expr> relativeLocationPath;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = XPathParser<LexerT>::AbsoluteLocationPath(lexer);
            absoluteLocationPath.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocationPath");
                    #endif
                    return soul::parser::Match(true, absoluteLocationPath.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::RelativeLocationPath(lexer);
                    relativeLocationPath.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocationPath");
                            #endif
                            return soul::parser::Match(true, relativeLocationPath.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocationPath");
        else soul::lexer::WriteFailureToLog(lexer, "LocationPath");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AbsoluteLocationPath(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AbsoluteLocationPath");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127436);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> abbreviatedAbsoluteLocationPath;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::AbbreviatedAbsoluteLocationPath(lexer);
                    abbreviatedAbsoluteLocationPath.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(abbreviatedAbsoluteLocationPath.release());
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == SLASH)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    expr.reset(new soul::xml::xpath::expr::Root);
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
                                soul::parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = XPathParser<LexerT>::RelativeLocationPath(lexer);
                                            right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                            if (match.hit)
                                            {
                                                expr.reset(new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::slash, expr.release(), right.release()));
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
                                    }
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbsoluteLocationPath");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbsoluteLocationPath");
        else soul::lexer::WriteFailureToLog(lexer, "AbsoluteLocationPath");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AbbreviatedAbsoluteLocationPath(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AbbreviatedAbsoluteLocationPath");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127437);
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == SLASH_SLASH)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = XPathParser<LexerT>::RelativeLocationPath(lexer);
                right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedAbsoluteLocationPath");
                        #endif
                        return soul::parser::Match(true, soul::xml::xpath::expr::MakeSlashSlashExpr(new soul::xml::xpath::expr::Root, right.release()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedAbsoluteLocationPath");
        else soul::lexer::WriteFailureToLog(lexer, "AbbreviatedAbsoluteLocationPath");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::RelativeLocationPath(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RelativeLocationPath");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127438);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> left;
    std::unique_ptr<soul::parser::Value<soul::xml::xpath::expr::Operator>> op;
    std::unique_ptr<soul::xml::xpath::expr::Expr> right;
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
                    soul::parser::Match match = XPathParser<LexerT>::Step(lexer);
                    left.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                soul::parser::Match match = XPathParser<LexerT>::CombinePathOp(lexer);
                                                op.reset(static_cast<soul::parser::Value<soul::xml::xpath::expr::Operator>*>(match.value));
                                                *parentMatch9 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    soul::parser::Match match = XPathParser<LexerT>::Step(lexer);
                                                    right.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                        if (match.hit)
                                        {
                                            switch (op->value)
                                            {
                                                case soul::xml::xpath::expr::Operator::slash: {
                                                    expr.reset(new soul::xml::xpath::expr::BinaryExpr(soul::xml::xpath::expr::Operator::slash, expr.release(), right.release()));
                                                    break;
                                                }
                                                case soul::xml::xpath::expr::Operator::slashSlash: {
                                                    expr.reset(soul::xml::xpath::expr::MakeSlashSlashExpr(expr.release(), right.release()));
                                                    break;
                                                }
                                            }
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelativeLocationPath");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelativeLocationPath");
        else soul::lexer::WriteFailureToLog(lexer, "RelativeLocationPath");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Step(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Step");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127439);
    std::unique_ptr<soul::xml::xpath::expr::LocationStepExpr> expr = std::unique_ptr<soul::xml::xpath::expr::LocationStepExpr>();
    std::unique_ptr<soul::parser::Value<soul::xml::Axis>> axis;
    std::unique_ptr<soul::xml::xpath::expr::NodeTest> nodeTest;
    std::unique_ptr<soul::xml::xpath::expr::Expr> predicate;
    std::unique_ptr<soul::xml::xpath::expr::LocationStepExpr> abbreviatedStep;
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
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match = XPathParser<LexerT>::AxisSpecifier(lexer);
                        axis.reset(static_cast<soul::parser::Value<soul::xml::Axis>*>(match.value));
                        *parentMatch4 = match;
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
                                soul::parser::Match match = XPathParser<LexerT>::NodeTest(lexer);
                                nodeTest.reset(static_cast<soul::xml::xpath::expr::NodeTest*>(match.value));
                                if (match.hit)
                                {
                                    expr.reset(new soul::xml::xpath::expr::LocationStepExpr(axis->value, nodeTest.release()));
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                            soul::parser::Match match = XPathParser<LexerT>::Predicate(lexer);
                                            predicate.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                            if (match.hit)
                                            {
                                                expr->AddPredicate(predicate.release());
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = XPathParser<LexerT>::AbbreviatedStep(lexer);
                            abbreviatedStep.reset(static_cast<soul::xml::xpath::expr::LocationStepExpr*>(match.value));
                            if (match.hit)
                            {
                                expr.reset(abbreviatedStep.release());
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Step");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Step");
        else soul::lexer::WriteFailureToLog(lexer, "Step");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AxisSpecifier(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AxisSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127440);
    std::unique_ptr<soul::parser::Value<soul::xml::Axis>> axis;
    std::unique_ptr<soul::parser::Value<soul::xml::Axis>> abbreviatedAxisSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match = XPathParser<LexerT>::AxisName(lexer);
            axis.reset(static_cast<soul::parser::Value<soul::xml::Axis>*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == COLON_COLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisSpecifier");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(axis->value));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = XPathParser<LexerT>::AbbreviatedAxisSpecifier(lexer);
                    abbreviatedAxisSpecifier.reset(static_cast<soul::parser::Value<soul::xml::Axis>*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisSpecifier");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(abbreviatedAxisSpecifier->value));
                        }
                    }
                    *parentMatch5 = match;
                }
                *parentMatch4 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "AxisSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AxisName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AxisName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127441);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            switch (lexer.GetKeywordToken(lexer.GetToken(pos).match))
            {
                case ANCESTOR: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::ancestor));
                    }
                }
                case ANCESTOR_OR_SELF: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::ancestorOrSelf));
                    }
                }
                case ATTRIBUTE: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::attribute));
                    }
                }
                case CHILD: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::child));
                    }
                }
                case DESCENDANT: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::descendant));
                    }
                }
                case DESCENDANT_OR_SELF: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::descendantOrSelf));
                    }
                }
                case FOLLOWING: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::following));
                    }
                }
                case FOLLOWING_SIBLING: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::followingSibling));
                    }
                }
                case NAMESPACE: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::ns));
                    }
                }
                case PARENT: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::parent));
                    }
                }
                case PRECEDING: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::preceding));
                    }
                }
                case PRECEDING_SIBLING: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::precedingSibling));
                    }
                }
                case SELF: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::self));
                    }
                }
                default: {
                    pass = false;
                    break;
                }
            }
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxisName");
        else soul::lexer::WriteFailureToLog(lexer, "AxisName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AbbreviatedAxisSpecifier(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AbbreviatedAxisSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127442);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == AT)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedAxisSpecifier");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::attribute));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(true);
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedAxisSpecifier");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<soul::xml::Axis>(soul::xml::Axis::child));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedAxisSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "AbbreviatedAxisSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::NodeTest(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NodeTest");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127443);
    std::unique_ptr<soul::xml::xpath::expr::Literal> pi;
    std::unique_ptr<soul::xml::xpath::expr::NodeTest> nodeType;
    std::unique_ptr<soul::xml::xpath::expr::NodeTest> nameTest;
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
                                    soul::parser::Match match = XPathParser<LexerT>::ProcessingInstructionKeyword(lexer);
                                    *parentMatch7 = match;
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
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    soul::parser::Match match = XPathParser<LexerT>::Literal(lexer);
                                    pi.reset(static_cast<soul::xml::xpath::expr::Literal*>(match.value));
                                    *parentMatch9 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeTest");
                            #endif
                            return soul::parser::Match(true, new soul::xml::xpath::expr::PILiteralNodeTest(pi.release()));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        soul::parser::Match match = XPathParser<LexerT>::NodeType(lexer);
                                        nodeType.reset(static_cast<soul::xml::xpath::expr::NodeTest*>(match.value));
                                        *parentMatch15 = match;
                                    }
                                    if (match.hit)
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
                                        *parentMatch15 = match;
                                    }
                                    *parentMatch14 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == RPAREN)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch17 = match;
                                    }
                                    *parentMatch14 = match;
                                }
                                *parentMatch13 = match;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeTest");
                                    #endif
                                    return soul::parser::Match(true, nodeType.release());
                                }
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            soul::parser::Match match = XPathParser<LexerT>::NameTest(lexer);
                            nameTest.reset(static_cast<soul::xml::xpath::expr::NodeTest*>(match.value));
                            *parentMatch20 = match;
                        }
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeTest");
                                #endif
                                return soul::parser::Match(true, nameTest.release());
                            }
                        }
                        *parentMatch19 = match;
                    }
                    *parentMatch18 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeTest");
        else soul::lexer::WriteFailureToLog(lexer, "NodeTest");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Literal(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Literal");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127444);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case DQ_STRING:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                    #endif
                    return soul::parser::Match(true, new soul::xml::xpath::expr::Literal(soul::xml::xpath::token::parser::ParseDQString(lexer.FileName(), lexer.GetToken(pos))));
                }
            }
            break;
        }
        case SQ_STRING:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                    #endif
                    return soul::parser::Match(true, new soul::xml::xpath::expr::Literal(soul::xml::xpath::token::parser::ParseSQString(lexer.FileName(), lexer.GetToken(pos))));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
        else soul::lexer::WriteFailureToLog(lexer, "Literal");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::NodeType(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NodeType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127445);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            switch (lexer.GetKeywordToken(lexer.GetToken(pos).match))
            {
                case COMMENT: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeType");
                        #endif
                        return soul::parser::Match(true, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::commentNodeTest));
                    }
                }
                case TEXT: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeType");
                        #endif
                        return soul::parser::Match(true, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::textNodeTest));
                    }
                }
                case PROCESSING_INSTRUCTION: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeType");
                        #endif
                        return soul::parser::Match(true, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::piNodeTest));
                    }
                }
                case NODE: {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeType");
                        #endif
                        return soul::parser::Match(true, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::anyNodeTest));
                    }
                }
                default: {
                    pass = false;
                    break;
                }
            }
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NodeType");
        else soul::lexer::WriteFailureToLog(lexer, "NodeType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::NameTest(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NameTest");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127446);
    std::unique_ptr<soul::parser::Value<std::string>> ncname;
    std::unique_ptr<soul::parser::Value<std::string>> qname;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == STAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameTest");
                        #endif
                        return soul::parser::Match(true, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::principalNodeTest));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match = XPathParser<LexerT>::NCName(lexer);
                            ncname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                if (*lexer == STAR)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameTest");
                                        #endif
                                        return soul::parser::Match(true, new soul::xml::xpath::expr::PrefixNodeTest(ncname->value));
                                    }
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch7 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch10 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::QName(lexer);
                    qname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameTest");
                            #endif
                            return soul::parser::Match(true, new soul::xml::xpath::expr::NameNodeTest(qname->value));
                        }
                    }
                    *parentMatch10 = match;
                }
                *parentMatch9 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameTest");
        else soul::lexer::WriteFailureToLog(lexer, "NameTest");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::NCName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NCName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127447);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NCName");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(util::ToUtf8(lexer.GetToken(pos).ToString())));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NCName");
        else soul::lexer::WriteFailureToLog(lexer, "NCName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::QName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "QName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127448);
    std::unique_ptr<soul::parser::Value<std::string>> prefixedName;
    std::unique_ptr<soul::parser::Value<std::string>> unprefixedName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = XPathParser<LexerT>::PrefixedName(lexer);
            prefixedName.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "QName");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<std::string>(prefixedName->value));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::UnprefixedName(lexer);
                    unprefixedName.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "QName");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<std::string>(unprefixedName->value));
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "QName");
        else soul::lexer::WriteFailureToLog(lexer, "QName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::PrefixedName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PrefixedName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127449);
    std::unique_ptr<soul::parser::Value<std::string>> prefix;
    std::unique_ptr<soul::parser::Value<std::string>> localPart;
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
                    soul::parser::Match match = XPathParser<LexerT>::Prefix(lexer);
                    prefix.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    *parentMatch3 = match;
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch5 = &match;
                {
                    soul::parser::Match match = XPathParser<LexerT>::LocalPart(lexer);
                    localPart.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixedName");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(prefix->value + ":" + localPart->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixedName");
        else soul::lexer::WriteFailureToLog(lexer, "PrefixedName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Prefix(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Prefix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127450);
    std::unique_ptr<soul::parser::Value<std::string>> ncname;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = XPathParser<LexerT>::NCName(lexer);
        ncname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(ncname->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
        else soul::lexer::WriteFailureToLog(lexer, "Prefix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::UnprefixedName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnprefixedName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127451);
    std::unique_ptr<soul::parser::Value<std::string>> localPart;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = XPathParser<LexerT>::LocalPart(lexer);
        localPart.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnprefixedName");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(localPart->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnprefixedName");
        else soul::lexer::WriteFailureToLog(lexer, "UnprefixedName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::LocalPart(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LocalPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127452);
    std::unique_ptr<soul::parser::Value<std::string>> ncname;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = XPathParser<LexerT>::NCName(lexer);
        ncname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalPart");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(ncname->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalPart");
        else soul::lexer::WriteFailureToLog(lexer, "LocalPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Predicate(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Predicate");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127453);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr;
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
                    if (*lexer == LBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match = XPathParser<LexerT>::Expr(lexer);
                        expr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch5 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Predicate");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Predicate");
        else soul::lexer::WriteFailureToLog(lexer, "Predicate");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AbbreviatedStep(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AbbreviatedStep");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127454);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case DOT_DOT:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedStep");
                    #endif
                    return soul::parser::Match(true, new soul::xml::xpath::expr::LocationStepExpr(soul::xml::Axis::parent, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::anyNodeTest)));
                }
            }
            break;
        }
        case DOT:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedStep");
                    #endif
                    return soul::parser::Match(true, new soul::xml::xpath::expr::LocationStepExpr(soul::xml::Axis::self, new soul::xml::xpath::expr::NodeTest(soul::xml::xpath::expr::NodeTestKind::anyNodeTest)));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AbbreviatedStep");
        else soul::lexer::WriteFailureToLog(lexer, "AbbreviatedStep");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::FunctionCall(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionCall");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127455);
    std::unique_ptr<soul::xml::xpath::expr::FunctionCall> functionCall = std::unique_ptr<soul::xml::xpath::expr::FunctionCall>();
    std::unique_ptr<soul::parser::Value<std::string>> functionName;
    std::unique_ptr<soul::xml::xpath::expr::Expr> arg;
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
                        soul::parser::Match match = XPathParser<LexerT>::FunctionName(lexer);
                        functionName.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                        *parentMatch4 = match;
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
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    functionCall.reset(new soul::xml::xpath::expr::FunctionCall(functionName->value));
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = XPathParser<LexerT>::Argument(lexer);
                                        arg.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                        if (match.hit)
                                        {
                                            functionCall->AddArgument(arg.release());
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        soul::parser::Match* parentMatch13 = &match;
                                        {
                                            while (true)
                                            {
                                                int64_t save = lexer.GetPos();
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        if (*lexer == COMMA)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch15 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch16 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = XPathParser<LexerT>::Argument(lexer);
                                                                arg.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    functionCall->AddArgument(arg.release());
                                                                }
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
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        *parentMatch12 = match;
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
                            }
                        }
                        *parentMatch7 = match;
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
                    soul::parser::Match match(false);
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCall");
                #endif
                return soul::parser::Match(true, functionCall.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCall");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionCall");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::FunctionName(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127456);
    std::unique_ptr<soul::parser::Value<std::string>> qname;
    std::unique_ptr<soul::xml::xpath::expr::NodeTest> nodeType;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match = XPathParser<LexerT>::QName(lexer);
                qname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t tmp = lexer.GetPos();
                    lexer.SetPos(save);
                    save = tmp;
                    soul::parser::Match match = XPathParser<LexerT>::NodeType(lexer);
                    nodeType.reset(static_cast<soul::xml::xpath::expr::NodeTest*>(match.value));
                    *parentMatch3 = match;
                }
                if (!match.hit)
                {
                    lexer.SetPos(save);
                }
                *parentMatch2 = soul::parser::Match(!match.hit, match.value);
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionName");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<std::string>(qname->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionName");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Argument(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Argument");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127457);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = XPathParser<LexerT>::Expr(lexer);
        expr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Argument");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Argument");
        else soul::lexer::WriteFailureToLog(lexer, "Argument");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::FilterExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FilterExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127458);
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr = std::unique_ptr<soul::xml::xpath::expr::Expr>();
    std::unique_ptr<soul::xml::xpath::expr::Expr> primaryExpr;
    std::unique_ptr<soul::xml::xpath::expr::Expr> predicate;
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
                    soul::parser::Match match = XPathParser<LexerT>::PrimaryExpr(lexer);
                    primaryExpr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        expr.reset(primaryExpr.release());
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = XPathParser<LexerT>::Predicate(lexer);
                                        predicate.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                        if (match.hit)
                                        {
                                            expr.reset(new soul::xml::xpath::expr::FilterExpr(expr.release(), predicate.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FilterExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FilterExpr");
        else soul::lexer::WriteFailureToLog(lexer, "FilterExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::PrimaryExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PrimaryExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127459);
    std::unique_ptr<soul::xml::xpath::expr::Expr> functionCall;
    std::unique_ptr<soul::xml::xpath::expr::Expr> variableReference;
    std::unique_ptr<soul::xml::xpath::expr::Expr> expr;
    std::unique_ptr<soul::xml::xpath::expr::Literal> literal;
    std::unique_ptr<soul::xml::xpath::expr::Expr> number;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
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
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = XPathParser<LexerT>::FunctionCall(lexer);
                        functionCall.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                                #endif
                                return soul::parser::Match(true, functionCall.release());
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = XPathParser<LexerT>::VariableReference(lexer);
                                variableReference.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                                        #endif
                                        return soul::parser::Match(true, variableReference.release());
                                    }
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
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
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match = XPathParser<LexerT>::Expr(lexer);
                                            expr.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
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
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                                    #endif
                                    return soul::parser::Match(true, new soul::xml::xpath::expr::UnaryExpr(soul::xml::xpath::expr::Operator::parens, expr.release()));
                                }
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = XPathParser<LexerT>::Literal(lexer);
                        literal.reset(static_cast<soul::xml::xpath::expr::Literal*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                                #endif
                                return soul::parser::Match(true, literal.release());
                            }
                        }
                        *parentMatch15 = match;
                    }
                    *parentMatch14 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = XPathParser<LexerT>::Number(lexer);
                    number.reset(static_cast<soul::xml::xpath::expr::Expr*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                            #endif
                            return soul::parser::Match(true, number.release());
                        }
                    }
                    *parentMatch17 = match;
                }
                *parentMatch16 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
        else soul::lexer::WriteFailureToLog(lexer, "PrimaryExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::VariableReference(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VariableReference");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127460);
    std::unique_ptr<soul::parser::Value<std::string>> qname;
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
                if (*lexer == DOLLAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match = XPathParser<LexerT>::QName(lexer);
                    qname.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VariableReference");
                #endif
                return soul::parser::Match(true, new soul::xml::xpath::expr::VariableReference(qname->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VariableReference");
        else soul::lexer::WriteFailureToLog(lexer, "VariableReference");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::Number(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Number");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127461);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == NUMBER)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Number");
                #endif
                return soul::parser::Match(true, new soul::xml::xpath::expr::NumberExpr(lexer.GetToken(pos).ToDouble()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Number");
        else soul::lexer::WriteFailureToLog(lexer, "Number");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::EqualityOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EqualityOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127462);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case EQ:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqualityOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::equal));
                }
            }
            break;
        }
        case NEQ:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqualityOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::notEqual));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqualityOp");
        else soul::lexer::WriteFailureToLog(lexer, "EqualityOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::RelationalOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RelationalOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127463);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case LESS:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::less));
                }
            }
            break;
        }
        case GREATER:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::greater));
                }
            }
            break;
        }
        case LEQ:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::lessOrEqual));
                }
            }
            break;
        }
        case GEQ:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::greaterOrEqual));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelationalOp");
        else soul::lexer::WriteFailureToLog(lexer, "RelationalOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AdditiveOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AdditiveOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127464);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case PLUS:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AdditiveOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::plus));
                }
            }
            break;
        }
        case MINUS:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AdditiveOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::minus));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AdditiveOp");
        else soul::lexer::WriteFailureToLog(lexer, "AdditiveOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::MultiplicativeOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MultiplicativeOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127465);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == STAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::mul));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = XPathParser<LexerT>::DivKeyword(lexer);
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeOp");
                                #endif
                                return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::div));
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XPathParser<LexerT>::ModKeyword(lexer);
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeOp");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::mod));
                        }
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiplicativeOp");
        else soul::lexer::WriteFailureToLog(lexer, "MultiplicativeOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::CombinePathOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CombinePathOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127466);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case SLASH:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CombinePathOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::slash));
                }
            }
            break;
        }
        case SLASH_SLASH:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CombinePathOp");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<soul::xml::xpath::expr::Operator>(soul::xml::xpath::expr::Operator::slashSlash));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CombinePathOp");
        else soul::lexer::WriteFailureToLog(lexer, "CombinePathOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::OrKeyword(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OrKeyword");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127467);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            pass = lexer.GetKeywordToken(lexer.GetToken(pos).match) == OR;
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrKeyword");
        else soul::lexer::WriteFailureToLog(lexer, "OrKeyword");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::AndKeyword(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AndKeyword");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127468);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            pass = lexer.GetKeywordToken(lexer.GetToken(pos).match) == AND;
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AndKeyword");
        else soul::lexer::WriteFailureToLog(lexer, "AndKeyword");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::DivKeyword(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DivKeyword");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127469);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            pass = lexer.GetKeywordToken(lexer.GetToken(pos).match) == DIV;
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DivKeyword");
        else soul::lexer::WriteFailureToLog(lexer, "DivKeyword");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::ModKeyword(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ModKeyword");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127470);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            pass = lexer.GetKeywordToken(lexer.GetToken(pos).match) == MOD;
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ModKeyword");
        else soul::lexer::WriteFailureToLog(lexer, "ModKeyword");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match XPathParser<LexerT>::ProcessingInstructionKeyword(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcessingInstructionKeyword");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6774979419569127471);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == NAME)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            pass = lexer.GetKeywordToken(lexer.GetToken(pos).match) == PROCESSING_INSTRUCTION;
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcessingInstructionKeyword");
        else soul::lexer::WriteFailureToLog(lexer, "ProcessingInstructionKeyword");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct XPathParser<soul::lexer::Lexer<soul::xml::xpath::lexer::XPathLexer<char32_t>, char32_t>>;

} // namespace soul::xml::xpath::parser
