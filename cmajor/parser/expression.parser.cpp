
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/expression.parser' using soul parser generator spg version 5.0.0

module cmajor.expression.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.parser.operators;
import cmajor.type.expr.parser;
import cmajor.identifier.parser;
import cmajor.literal.parser;
import cmajor.basic.type.parser;
import cmajor.templates.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::parser::operators;
using namespace cmajor::type::expr::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::literal::parser;
using namespace cmajor::basic::type::parser;
using namespace cmajor::templates::parser;

namespace cmajor::expression::parser {

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Expression(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Expression");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831553);
    std::unique_ptr<cmajor::ast::Node> equivalence;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = ExpressionParser<LexerT>::Equivalence(lexer, context);
        equivalence.reset(static_cast<cmajor::ast::Node*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expression");
                #endif
                return soul::parser::Match(true, equivalence.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expression");
        else soul::lexer::WriteFailureToLog(lexer, "Expression");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Equivalence(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Equivalence");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831554);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Implication(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == EQUIVALENCE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (!context->ParsingConcept() || context->ParsingTemplateId()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Implication(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::EquivalenceNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Equivalence");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Equivalence");
        else soul::lexer::WriteFailureToLog(lexer, "Equivalence");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Implication(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Implication");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831555);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Disjunction(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                    int64_t save = lexer.GetPos();
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
                                    bool pass = true;
                                    soul::parser::Match match(false);
                                    if (*lexer == IMPLICATION)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        if (!context->ParsingConcept() || context->ParsingTemplateId()) pass = false;
                                    }
                                    if (match.hit && !pass)
                                    {
                                        match = soul::parser::Match(false);
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
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = ExpressionParser<LexerT>::Implication(lexer, context);
                                        right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            expr.reset(new cmajor::ast::ImplicationNode(s, expr.release(), right.release()));
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Implication");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Implication");
        else soul::lexer::WriteFailureToLog(lexer, "Implication");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Disjunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Disjunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831556);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Conjunction(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == DISJUNCTION)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Conjunction(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::DisjunctionNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Disjunction");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Disjunction");
        else soul::lexer::WriteFailureToLog(lexer, "Disjunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Conjunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Conjunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831557);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::BitOr(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == AMPAMP)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::BitOr(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::ConjunctionNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Conjunction");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Conjunction");
        else soul::lexer::WriteFailureToLog(lexer, "Conjunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::BitOr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BitOr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831558);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::BitXor(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == BITOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::BitXor(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::BitOrNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitOr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitOr");
        else soul::lexer::WriteFailureToLog(lexer, "BitOr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::BitXor(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BitXor");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831559);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::BitAnd(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == BITXOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::BitAnd(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::BitXorNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitXor");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitXor");
        else soul::lexer::WriteFailureToLog(lexer, "BitXor");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::BitAnd(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BitAnd");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831560);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Equality(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match(false);
                                            if (*lexer == AMP)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Equality(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr.reset(new cmajor::ast::BitAndNode(s, expr.release(), right.release()));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitAnd");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BitAnd");
        else soul::lexer::WriteFailureToLog(lexer, "BitAnd");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Equality(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Equality");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831561);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> op;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Relational(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match = ExpressionParser<LexerT>::EqOp(lexer);
                                            op.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Relational(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    switch (op->value)
                                                    {
                                                        case Operator::eq: expr.reset(new cmajor::ast::EqualNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::neq: expr.reset(new cmajor::ast::NotEqualNode(s, expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Equality");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Equality");
        else soul::lexer::WriteFailureToLog(lexer, "Equality");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::EqOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EqOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831562);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case EQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::eq));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NEQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NEQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::neq));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EqOp");
        else soul::lexer::WriteFailureToLog(lexer, "EqOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Relational(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Relational");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831563);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    cmajor::parser::operators::Operator op = cmajor::parser::operators::Operator();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> relOp;
    std::unique_ptr<cmajor::ast::Node> isType;
    std::unique_ptr<cmajor::ast::Node> asType;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Shift(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            switch (*lexer)
                                            {
                                                case GEQ:
                                                case LANGLE:
                                                case LEQ:
                                                case RANGLE:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch10 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        bool pass = true;
                                                        soul::parser::Match match = ExpressionParser<LexerT>::RelOp(lexer);
                                                        relOp.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            op = relOp->value;
                                                            if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments())
                                                            {
                                                                pass = false;
                                                            }
                                                            else
                                                            {
                                                                context->PushParsingIsOrAs(false);
                                                            }
                                                        }
                                                        if (match.hit && !pass)
                                                        {
                                                            match = soul::parser::Match(false);
                                                        }
                                                        *parentMatch10 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch9 = match;
                                                    }
                                                    break;
                                                }
                                                case IS:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch11 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch12 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            bool pass = true;
                                                            soul::parser::Match match(false);
                                                            if (*lexer == IS)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments())
                                                                {
                                                                    pass = false;
                                                                }
                                                                else
                                                                {
                                                                    op = Operator::is;
                                                                    context->PushParsingIsOrAs(true);
                                                                }
                                                            }
                                                            if (match.hit && !pass)
                                                            {
                                                                match = soul::parser::Match(false);
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                            isType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch9 = match;
                                                    }
                                                    break;
                                                }
                                                case AS:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch15 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            bool pass = true;
                                                            soul::parser::Match match(false);
                                                            if (*lexer == AS)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments())
                                                                {
                                                                    pass = false;
                                                                }
                                                                else
                                                                {
                                                                    op = Operator::as;
                                                                    context->PushParsingIsOrAs(true);
                                                                }
                                                            }
                                                            if (match.hit && !pass)
                                                            {
                                                                match = soul::parser::Match(false);
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch16 = &match;
                                                        {
                                                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                            asType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                            *parentMatch16 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch9 = match;
                                                    }
                                                    break;
                                                }
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
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
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch19 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch20 = &match;
                                                    {
                                                        int64_t save = lexer.GetPos();
                                                        soul::parser::Match match = ExpressionParser<LexerT>::Shift(lexer, context);
                                                        right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                        *parentMatch20 = match;
                                                        if (!match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch21 = &match;
                                                            lexer.SetPos(save);
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch22 = &match;
                                                                {
                                                                    int64_t pos = lexer.GetPos();
                                                                    bool pass = true;
                                                                    soul::parser::Match match(true);
                                                                    if (match.hit)
                                                                    {
                                                                        if (!context->ParsingIsOrAs()) pass = false;
                                                                    }
                                                                    if (match.hit && !pass)
                                                                    {
                                                                        match = soul::parser::Match(false);
                                                                    }
                                                                    *parentMatch22 = match;
                                                                }
                                                                *parentMatch21 = match;
                                                            }
                                                            *parentMatch20 = match;
                                                        }
                                                    }
                                                    *parentMatch19 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    context->PopParsingIsOrAs();
                                                    switch (op)
                                                    {
                                                        case Operator::lessOrEq: expr.reset(new cmajor::ast::LessOrEqualNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::greaterOrEq: expr.reset(new cmajor::ast::GreaterOrEqualNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::less: expr.reset(new cmajor::ast::LessNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::greater: expr.reset(new cmajor::ast::GreaterNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::is: expr.reset(new cmajor::ast::IsNode(s, expr.release(), isType.release()));
                                                        break;
                                                        case Operator::as: expr.reset(new cmajor::ast::AsNode(s, expr.release(), asType.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch17 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Relational");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Relational");
        else soul::lexer::WriteFailureToLog(lexer, "Relational");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::RelOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RelOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831564);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case LEQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == LEQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::lessOrEq));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case GEQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == GEQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::greaterOrEq));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case LANGLE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == LANGLE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::less));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RANGLE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RANGLE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::greater));
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RelOp");
        else soul::lexer::WriteFailureToLog(lexer, "RelOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Shift(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Shift");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831565);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> op;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Additive(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                bool pass = true;
                                                soul::parser::Match match = ExpressionParser<LexerT>::ShiftOp(lexer);
                                                op.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                                                if (match.hit)
                                                {
                                                    if (context->ParsingLvalue()) pass = false;
                                                }
                                                if (match.hit && !pass)
                                                {
                                                    match = soul::parser::Match(false);
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
                                                    soul::parser::Match match(true);
                                                    if (match.hit)
                                                    {
                                                        context->BeginParsingArguments();
                                                    }
                                                    *parentMatch11 = match;
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
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Additive(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    switch (op->value)
                                                    {
                                                        case Operator::shiftLeft: expr.reset(new cmajor::ast::ShiftLeftNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::shiftRight: expr.reset(new cmajor::ast::ShiftRightNode(s, expr.release(), right.release()));
                                                        break;
                                                    }
                                                    context->EndParsingArguments();
                                                }
                                                else
                                                {
                                                    context->EndParsingArguments();
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Shift");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Shift");
        else soul::lexer::WriteFailureToLog(lexer, "Shift");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::ShiftOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ShiftOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831566);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case SHIFTLEFT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SHIFTLEFT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShiftOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::shiftLeft));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SHIFTRIGHT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SHIFTRIGHT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShiftOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::shiftRight));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShiftOp");
        else soul::lexer::WriteFailureToLog(lexer, "ShiftOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Additive(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Additive");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831567);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> op;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Multiplicative(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match = ExpressionParser<LexerT>::AddOp(lexer);
                                            op.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Multiplicative(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    switch (op->value)
                                                    {
                                                        case Operator::plus: expr.reset(new cmajor::ast::AddNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::minus: expr.reset(new cmajor::ast::SubNode(s, expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Additive");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Additive");
        else soul::lexer::WriteFailureToLog(lexer, "Additive");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::AddOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AddOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831568);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PLUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::plus));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MINUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MINUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::minus));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddOp");
        else soul::lexer::WriteFailureToLog(lexer, "AddOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Multiplicative(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Multiplicative");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831569);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> op;
    std::unique_ptr<cmajor::ast::Node> right;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Prefix(lexer, context);
                    left.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            bool pass = true;
                                            soul::parser::Match match = ExpressionParser<LexerT>::MulOp(lexer);
                                            op.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                                            if (match.hit)
                                            {
                                                if (context->ParsingLvalue() || context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                                            }
                                            if (match.hit && !pass)
                                            {
                                                match = soul::parser::Match(false);
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
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ExpressionParser<LexerT>::Prefix(lexer, context);
                                                right.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    switch (op->value)
                                                    {
                                                        case Operator::mul: expr.reset(new cmajor::ast::MulNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::div: expr.reset(new cmajor::ast::DivNode(s, expr.release(), right.release()));
                                                        break;
                                                        case Operator::rem: expr.reset(new cmajor::ast::RemNode(s, expr.release(), right.release()));
                                                        break;
                                                    }
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Multiplicative");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Multiplicative");
        else soul::lexer::WriteFailureToLog(lexer, "Multiplicative");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::MulOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MulOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831570);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case STAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MulOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::mul));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DIV:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DIV)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MulOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::div));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case REM:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == REM)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MulOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::rem));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MulOp");
        else soul::lexer::WriteFailureToLog(lexer, "MulOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Prefix(LexerT& lexer, cmajor::parser::context::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831571);
    Operator op = Operator();
    std::unique_ptr<cmajor::ast::Node> prefix = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<cmajor::parser::operators::Operator>> prefixOp;
    std::unique_ptr<cmajor::ast::Node> p;
    std::unique_ptr<cmajor::ast::Node> postfix;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    bool pass = true;
                    soul::parser::Match match = ExpressionParser<LexerT>::PrefixOp(lexer);
                    prefixOp.reset(static_cast<soul::parser::Value<cmajor::parser::operators::Operator>*>(match.value));
                    if (match.hit)
                    {
                        op = prefixOp->value;
                        s = lexer.GetSpan(pos);
                        if (op != Operator::plusplus && op != Operator::minusminus && context->ParsingExpressionStatement() && !context->ParsingArguments()) pass = false;
                    }
                    if (match.hit && !pass)
                    {
                        match = soul::parser::Match(false);
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
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = ExpressionParser<LexerT>::Prefix(lexer, context);
                        p.reset(static_cast<cmajor::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            prefix.reset(p.release());
                            switch (op)
                            {
                                case Operator::plusplus: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::PrefixIncrementNode(s, prefix.release()));
                                    }
                                }
                                case Operator::minusminus: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::PrefixDecrementNode(s, prefix.release()));
                                    }
                                }
                                case Operator::plus: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::UnaryPlusNode(s, prefix.release()));
                                    }
                                }
                                case Operator::minus: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::UnaryMinusNode(s, prefix.release()));
                                    }
                                }
                                case Operator::not_: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::NotNode(s, prefix.release()));
                                    }
                                }
                                case Operator::complement: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::ComplementNode(s, prefix.release()));
                                    }
                                }
                                case Operator::deref: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::DerefNode(s, prefix.release()));
                                    }
                                }
                                case Operator::addrOf: {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                                        #endif
                                        return soul::parser::Match(true, new cmajor::ast::AddrOfNode(s, prefix.release()));
                                    }
                                }
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ExpressionParser<LexerT>::Postfix(lexer, context);
                    postfix.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prefix");
                            #endif
                            return soul::parser::Match(true, postfix.release());
                        }
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
soul::parser::Match ExpressionParser<LexerT>::PrefixOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PrefixOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831572);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PLUSPLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PLUSPLUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::plusplus));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MINUSMINUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MINUSMINUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::minusminus));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PLUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::plus));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MINUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MINUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::minus));
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EXCLAMATION:
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::not_));
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CPL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CPL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::complement));
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case STAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::deref));
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case AMP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == AMP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::parser::operators::Operator>(Operator::addrOf));
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrefixOp");
        else soul::lexer::WriteFailureToLog(lexer, "PrefixOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Postfix(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Postfix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831573);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> primary;
    std::unique_ptr<cmajor::ast::IdentifierNode> dotMember;
    std::unique_ptr<cmajor::ast::IdentifierNode> arrowMember;
    std::unique_ptr<cmajor::ast::Node> index;
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
                    soul::parser::Match match = ExpressionParser<LexerT>::Primary(lexer, context);
                    primary.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
                        expr.reset(primary.release());
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
                                    switch (*lexer)
                                    {
                                        case PLUSPLUS:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch8 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == PLUSPLUS)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    a = lexer.GetSpan(pos);
                                                    expr.reset(new cmajor::ast::PostfixIncrementNode(s, expr.release()));
                                                    expr->Union(a);
                                                }
                                                *parentMatch8 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
                                        case MINUSMINUS:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == MINUSMINUS)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    a = lexer.GetSpan(pos);
                                                    expr.reset(new cmajor::ast::PostfixDecrementNode(s, expr.release()));
                                                    expr->Union(a);
                                                }
                                                *parentMatch9 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
                                        case DOT:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == DOT)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch10 = match;
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
                                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                        dotMember.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            expr.reset(new cmajor::ast::DotNode(s, expr.release(), dotMember.release()));
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
                                        case ARROW:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == ARROW)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch14 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch15 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                        arrowMember.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            expr.reset(new cmajor::ast::ArrowNode(s, expr.release(), arrowMember.release()));
                                                        }
                                                        *parentMatch15 = match;
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
                                        case LBRACKET:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch18 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match(false);
                                                        if (*lexer == LBRACKET)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            context->PushParsingLvalue(false);
                                                            context->PushParsingExpressionStatement(false);
                                                        }
                                                        *parentMatch18 = match;
                                                    }
                                                    *parentMatch17 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch19 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch20 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                            index.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                context->PopParsingExpressionStatement();
                                                                context->PopParsingLvalue();
                                                            }
                                                            *parentMatch20 = match;
                                                        }
                                                        *parentMatch19 = match;
                                                    }
                                                    *parentMatch17 = match;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch21 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch22 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match(false);
                                                        if (*lexer == RBRACKET)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            a = lexer.GetSpan(pos);
                                                            expr.reset(new cmajor::ast::IndexingNode(s, expr.release(), index.release()));
                                                            expr->Union(a);
                                                        }
                                                        *parentMatch22 = match;
                                                    }
                                                    *parentMatch21 = match;
                                                }
                                                *parentMatch16 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
                                        case LPAREN:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch23 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
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
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match(false);
                                                                if (*lexer == LPAREN)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    expr.reset(new cmajor::ast::InvokeNode(s, expr.release()));
                                                                }
                                                                *parentMatch27 = match;
                                                            }
                                                            *parentMatch26 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch28 = &match;
                                                            {
                                                                soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, expr.get());
                                                                *parentMatch28 = match;
                                                            }
                                                            *parentMatch26 = match;
                                                        }
                                                        *parentMatch25 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch29 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch30 = &match;
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
                                                                    a = lexer.GetSpan(pos);
                                                                }
                                                                *parentMatch30 = match;
                                                            }
                                                            *parentMatch29 = match;
                                                        }
                                                        *parentMatch25 = match;
                                                    }
                                                    *parentMatch24 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    expr->Union(a);
                                                }
                                                *parentMatch23 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch7 = match;
                                            }
                                            break;
                                        }
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Postfix");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Postfix");
        else soul::lexer::WriteFailureToLog(lexer, "Postfix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::Primary(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Primary");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831574);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> expr;
    std::unique_ptr<cmajor::ast::Node> literal;
    std::unique_ptr<cmajor::ast::Node> basicType;
    std::unique_ptr<cmajor::ast::TemplateIdNode> templateId;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::Node> sizeOf;
    std::unique_ptr<cmajor::ast::Node> typeName;
    std::unique_ptr<cmajor::ast::Node> typeId;
    std::unique_ptr<cmajor::ast::Node> cast;
    std::unique_ptr<cmajor::ast::Node> constructExpr;
    std::unique_ptr<cmajor::ast::Node> newExpr;
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
                                            switch (*lexer)
                                            {
                                                case LPAREN:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch11 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch12 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch13 = &match;
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
                                                                    s = lexer.GetSpan(pos);
                                                                }
                                                                *parentMatch13 = match;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch14 = &match;
                                                            {
                                                                soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                                expr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                                *parentMatch14 = match;
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
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
                                                                soul::parser::Match match(false);
                                                                if (*lexer == RPAREN)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    a = lexer.GetSpan(pos);
                                                                    cmajor::ast::Node *n = new cmajor::ast::ParenthesizedExpressionNode(s, expr.release());
                                                                    n->Union(a);
                                                                    {
                                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                                        #endif
                                                                        return soul::parser::Match(true, n);
                                                                    }
                                                                }
                                                                *parentMatch16 = match;
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                                case CHARLIT:
                                                case FALSE:
                                                case FLOATINGLIT:
                                                case INTLIT:
                                                case LBRACE:
                                                case LBRACKET:
                                                case NULLLIT:
                                                case STRINGLIT:
                                                case TRUE:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch17 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = LiteralParser<LexerT>::Literal(lexer, context);
                                                        literal.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                                #endif
                                                                return soul::parser::Match(true, literal.release());
                                                            }
                                                        }
                                                        *parentMatch17 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                                case AUTO:
                                                case BOOL:
                                                case BYTE:
                                                case CHAR:
                                                case DOUBLE:
                                                case FLOAT:
                                                case INT:
                                                case LONG:
                                                case SBYTE:
                                                case SHORT:
                                                case UCHAR:
                                                case UINT:
                                                case ULONG:
                                                case USHORT:
                                                case VOID:
                                                case WCHAR:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch18 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = BasicTypeParser<LexerT>::BasicType(lexer, context);
                                                        basicType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                                #endif
                                                                return soul::parser::Match(true, basicType.release());
                                                            }
                                                        }
                                                        *parentMatch18 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                            }
                                            *parentMatch9 = match;
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
                                                        soul::parser::Match match = TemplateParser<LexerT>::TemplateId(lexer, context);
                                                        templateId.reset(static_cast<cmajor::ast::TemplateIdNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                                #endif
                                                                return soul::parser::Match(true, templateId.release());
                                                            }
                                                        }
                                                        *parentMatch20 = match;
                                                    }
                                                    *parentMatch19 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
                                        }
                                        *parentMatch8 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch21 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch22 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                    id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                            #endif
                                                            return soul::parser::Match(true, id.release());
                                                        }
                                                    }
                                                    *parentMatch22 = match;
                                                }
                                                *parentMatch21 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                    }
                                    *parentMatch7 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch23 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch24 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == THIS)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    {
                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                        #endif
                                                        return soul::parser::Match(true, new cmajor::ast::ThisNode(lexer.GetSpan(pos)));
                                                    }
                                                }
                                                *parentMatch24 = match;
                                            }
                                            *parentMatch23 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                }
                                *parentMatch6 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch25 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch26 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == BASE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                    #endif
                                                    return soul::parser::Match(true, new cmajor::ast::BaseNode(lexer.GetSpan(pos)));
                                                }
                                            }
                                            *parentMatch26 = match;
                                        }
                                        *parentMatch25 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch27 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch28 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = ExpressionParser<LexerT>::SizeOfExpr(lexer, context);
                                        sizeOf.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                                #endif
                                                return soul::parser::Match(true, sizeOf.release());
                                            }
                                        }
                                        *parentMatch28 = match;
                                    }
                                    *parentMatch27 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch29 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch30 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ExpressionParser<LexerT>::TypeNameExpr(lexer, context);
                                    typeName.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                            #endif
                                            return soul::parser::Match(true, typeName.release());
                                        }
                                    }
                                    *parentMatch30 = match;
                                }
                                *parentMatch29 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch31 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch32 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = ExpressionParser<LexerT>::TypeIdExpr(lexer, context);
                                typeId.reset(static_cast<cmajor::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                        #endif
                                        return soul::parser::Match(true, typeId.release());
                                    }
                                }
                                *parentMatch32 = match;
                            }
                            *parentMatch31 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch33 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch34 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ExpressionParser<LexerT>::CastExpr(lexer, context);
                            cast.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                    #endif
                                    return soul::parser::Match(true, cast.release());
                                }
                            }
                            *parentMatch34 = match;
                        }
                        *parentMatch33 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch35 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch36 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = ExpressionParser<LexerT>::ConstructExpr(lexer, context);
                        constructExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                                #endif
                                return soul::parser::Match(true, constructExpr.release());
                            }
                        }
                        *parentMatch36 = match;
                    }
                    *parentMatch35 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ExpressionParser<LexerT>::NewExpr(lexer, context);
                    newExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                            #endif
                            return soul::parser::Match(true, newExpr.release());
                        }
                    }
                    *parentMatch38 = match;
                }
                *parentMatch37 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
        else soul::lexer::WriteFailureToLog(lexer, "Primary");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::ArgumentList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* node)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArgumentList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831575);
    soul::parser::Match match(true);
    int64_t save = lexer.GetPos();
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match = ExpressionParser<LexerT>::ExpressionList(lexer, context, node);
        if (match.hit)
        {
            *parentMatch0 = match;
        }
        else
        {
            lexer.SetPos(save);
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArgumentList");
        else soul::lexer::WriteFailureToLog(lexer, "ArgumentList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::ExpressionList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* node)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExpressionList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831576);
    std::unique_ptr<cmajor::ast::Node> arg;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(true);
            if (match.hit)
            {
                context->BeginParsingArguments();
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                            arg.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                node->AddArgument(arg.release());
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
                                            if (*lexer == COMMA)
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
                                                    arg.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        node->AddArgument(arg.release());
                                                    }
                                                    *parentMatch11 = match;
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
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    context->EndParsingArguments();
                }
                else
                {
                    context->EndParsingArguments();
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExpressionList");
        else soul::lexer::WriteFailureToLog(lexer, "ExpressionList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::SizeOfExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SizeOfExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831577);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
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
                            if (*lexer == SIZEOF)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSpan(pos);
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
                            if (*lexer == LPAREN)
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
                    soul::parser::Match* parentMatch9 = &match;
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
                            a = lexer.GetSpan(pos);
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            cmajor::ast::Node *n = new cmajor::ast::SizeOfNode(s, expr.release());
            n->Union(a);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizeOfExpr");
                #endif
                return soul::parser::Match(true, n);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizeOfExpr");
        else soul::lexer::WriteFailureToLog(lexer, "SizeOfExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::TypeNameExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeNameExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831578);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
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
                            if (*lexer == TYPENAME)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSpan(pos);
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
                            if (*lexer == LPAREN)
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
                    soul::parser::Match* parentMatch9 = &match;
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
                            a = lexer.GetSpan(pos);
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            cmajor::ast::Node *n = new cmajor::ast::TypeNameNode(s, expr.release());
            n->Union(a);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeNameExpr");
                #endif
                return soul::parser::Match(true, n);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeNameExpr");
        else soul::lexer::WriteFailureToLog(lexer, "TypeNameExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::TypeIdExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeIdExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831579);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
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
                            if (*lexer == TYPEID)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                s = lexer.GetSpan(pos);
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
                            if (*lexer == LPAREN)
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
                    soul::parser::Match* parentMatch9 = &match;
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
                            a = lexer.GetSpan(pos);
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            cmajor::ast::Node *n = new cmajor::ast::TypeIdNode(s, expr.release());
            n->Union(a);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeIdExpr");
                #endif
                return soul::parser::Match(true, n);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeIdExpr");
        else soul::lexer::WriteFailureToLog(lexer, "TypeIdExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::CastExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CastExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831580);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> typeExpr;
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
                                        if (*lexer == CAST)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            s = lexer.GetSpan(pos);
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
                                        if (*lexer == LANGLE)
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
                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                    typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                                if (*lexer == RANGLE)
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
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
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
                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                        expr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                    soul::parser::Match* parentMatch15 = &match;
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
                            a = lexer.GetSpan(pos);
                        }
                        *parentMatch15 = match;
                    }
                    *parentMatch14 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            cmajor::ast::Node *n = new cmajor::ast::CastNode(s, typeExpr.release(), expr.release());
            n->Union(a);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CastExpr");
                #endif
                return soul::parser::Match(true, n);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CastExpr");
        else soul::lexer::WriteFailureToLog(lexer, "CastExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::ConstructExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831581);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> typeExpr;
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
                                if (*lexer == CONSTRUCT)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    s = lexer.GetSpan(pos);
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
                                                if (*lexer == LANGLE)
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
                                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                    typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                                                if (*lexer == RANGLE)
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
                                        expr.reset(new cmajor::ast::ConstructNode(s, typeExpr.release()));
                                    }
                                    *parentMatch8 = match;
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
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
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
                        soul::parser::Match match = ExpressionParser<LexerT>::ExpressionList(lexer, context, expr.get());
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch17 = &match;
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
                            a = lexer.GetSpan(pos);
                        }
                        *parentMatch17 = match;
                    }
                    *parentMatch16 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            expr->Union(a);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::NewExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NewExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831582);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> typeExpr;
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
                        if (*lexer == NEW)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            s = lexer.GetSpan(pos);
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
                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                            typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                expr.reset(new cmajor::ast::NewNode(s, typeExpr.release()));
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
                                        soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, expr.get());
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
                                    soul::parser::Match* parentMatch14 = &match;
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
                                            a = lexer.GetSpan(pos);
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch13 = match;
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            if (a.IsValid())
            {
                expr->Union(a);
            }
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NewExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NewExpr");
        else soul::lexer::WriteFailureToLog(lexer, "NewExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExpressionParser<LexerT>::InvokeExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InvokeExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7570458120711831583);
    std::unique_ptr<cmajor::ast::Node> expr = std::unique_ptr<cmajor::ast::Node>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span a = soul::ast::Span();
    std::unique_ptr<cmajor::ast::TemplateIdNode> templateId;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::IdentifierNode> dotMemberId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TemplateParser<LexerT>::TemplateId(lexer, context);
                        templateId.reset(static_cast<cmajor::ast::TemplateIdNode*>(match.value));
                        if (match.hit)
                        {
                            s = lexer.GetSpan(pos);
                            expr.reset(templateId.release());
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
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                if (match.hit)
                                {
                                    s = lexer.GetSpan(pos);
                                    expr.reset(id.release());
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
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
                                    soul::parser::Match match(false);
                                    if (*lexer == DOT)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch10 = match;
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
                                            soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                            dotMemberId.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                            if (match.hit)
                                            {
                                                expr.reset(new cmajor::ast::DotNode(s, expr.release(), dotMemberId.release()));
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch15 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch18 = &match;
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
                                    expr.reset(new cmajor::ast::InvokeNode(s, expr.release()));
                                }
                                *parentMatch18 = match;
                            }
                            *parentMatch17 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, expr.get());
                                *parentMatch19 = match;
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch16 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch21 = &match;
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
                                    a = lexer.GetSpan(pos);
                                }
                                *parentMatch21 = match;
                            }
                            *parentMatch20 = match;
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch15 = match;
                }
                if (match.hit)
                {
                    expr->Union(a);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InvokeExpr");
                        #endif
                        return soul::parser::Match(true, expr.release());
                    }
                }
                *parentMatch14 = match;
            }
            *parentMatch13 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InvokeExpr");
        else soul::lexer::WriteFailureToLog(lexer, "InvokeExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ExpressionParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::expression::parser
