
// this file has been automatically generated from 'C:/work/cmajor-mod/soul/rex/rex.parser' using soul parser generator spg version 5.0.0

module soul.rex.parser;

import util;
import soul.ast.spg;
import soul.rex.token;
import soul.rex.lexer;
import soul.rex.token.parser;
import soul.rex.symbol;

using namespace soul::rex::token;
using namespace soul::rex::lexer;
using namespace soul::rex::token::parser;

namespace soul::rex::parser {

template<typename LexerT>
soul::rex::nfa::Nfa RexParser<LexerT>::Parse(LexerT& lexer, soul::rex::context::Context* context)
{
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = RexParser<LexerT>::RegularExpression(lexer, context);
    value.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
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
            return value->value;
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
    return value->value;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::RegularExpression(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RegularExpression");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655809);
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> alternative;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match = RexParser<LexerT>::Alternative(lexer, context);
            alternative.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegularExpression");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<soul::rex::nfa::Nfa>(alternative->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegularExpression");
        else soul::lexer::WriteFailureToLog(lexer, "RegularExpression");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Alternative(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Alternative");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655810);
    soul::rex::nfa::Nfa value = soul::rex::nfa::Nfa();
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> left;
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> right;
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
                    soul::parser::Match match = RexParser<LexerT>::Catenation(lexer, context);
                    left.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                                        if (*lexer == ALT)
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
                                                soul::parser::Match match(true);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = RexParser<LexerT>::Catenation(lexer, context);
                                                    right.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.ThrowExpectationFailure(pos, "Catenation");
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    value = soul::rex::nfa::Alt(*context, value, right->value);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Alternative");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<soul::rex::nfa::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Alternative");
        else soul::lexer::WriteFailureToLog(lexer, "Alternative");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Catenation(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Catenation");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655811);
    soul::rex::nfa::Nfa value = soul::rex::nfa::Nfa();
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> left;
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> right;
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
                    soul::parser::Match match = RexParser<LexerT>::Repetition(lexer, context);
                    left.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                                        soul::parser::Match match = RexParser<LexerT>::Repetition(lexer, context);
                                        right.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                                        if (match.hit)
                                        {
                                            value = soul::rex::nfa::Cat(value, right->value);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Catenation");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<soul::rex::nfa::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Catenation");
        else soul::lexer::WriteFailureToLog(lexer, "Catenation");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Repetition(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Repetition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655812);
    soul::rex::nfa::Nfa value = soul::rex::nfa::Nfa();
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> left;
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
                    soul::parser::Match match = RexParser<LexerT>::Primary(lexer, context);
                    left.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                    if (match.hit)
                    {
                        value = left->value;
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
                                int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
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
                                            value = soul::rex::nfa::Kleene(*context, value);
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
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
                                                    value = soul::rex::nfa::Pos(*context, value);
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                }
                                *parentMatch7 = match;
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
                                            soul::parser::Match match(false);
                                            if (*lexer == QUEST)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                value = soul::rex::nfa::Opt(*context, value);
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Repetition");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<soul::rex::nfa::Nfa>(value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Repetition");
        else soul::lexer::WriteFailureToLog(lexer, "Repetition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Primary(LexerT& lexer, soul::rex::context::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655813);
    soul::rex::nfa::Nfa value = soul::rex::nfa::Nfa();
    std::unique_ptr<soul::parser::Value<soul::rex::nfa::Nfa>> alt;
    std::unique_ptr<soul::rex::symbol::Class> cls;
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
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = RexParser<LexerT>::Alternative(lexer, context);
                                                        alt.reset(static_cast<soul::parser::Value<soul::rex::nfa::Nfa>*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            *parentMatch12 = match;
                                                        }
                                                        else
                                                        {
                                                            lexer.ThrowExpectationFailure(pos, "Alternative");
                                                        }
                                                    }
                                                    if (match.hit)
                                                    {
                                                        value = alt->value;
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
                                        soul::parser::Match* parentMatch13 = &match;
                                        {
                                            soul::parser::Match match(true);
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
                                                    *parentMatch14 = match;
                                                }
                                                else
                                                {
                                                    lexer.ThrowExpectationFailure(pos, lexer.GetTokenInfo(RPAREN));
                                                }
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch15 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == ESCAPE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    value = soul::rex::nfa::MakeNfa(*context, context->MakeChar(soul::rex::token::parser::MakeEscapeValue(lexer.FileName(), lexer.GetToken(pos))));
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                }
                                *parentMatch6 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch18 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == INVERSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                value = soul::rex::nfa::MakeNfa(*context, context->MakeChar('^'));
                                            }
                                            *parentMatch18 = match;
                                        }
                                        *parentMatch17 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
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
                                        soul::parser::Match match(false);
                                        if (*lexer == MINUS)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            value = soul::rex::nfa::MakeNfa(*context, context->MakeChar('-'));
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
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
                                    soul::parser::Match match(false);
                                    if (*lexer == CHAR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        auto token = lexer.GetToken(pos);
                                        value = soul::rex::nfa::MakeNfa(*context, context->MakeChar(token.Chr()));
                                    }
                                    *parentMatch22 = match;
                                }
                                *parentMatch21 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
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
                                if (*lexer == DOT)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    value = soul::rex::nfa::MakeNfa(*context, context->MakeAny());
                                }
                                *parentMatch24 = match;
                            }
                            *parentMatch23 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
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
                            soul::parser::Match match = RexParser<LexerT>::Class(lexer, context);
                            cls.reset(static_cast<soul::rex::symbol::Class*>(match.value));
                            if (match.hit)
                            {
                                value = soul::rex::nfa::MakeNfa(*context, cls.release());
                            }
                            *parentMatch26 = match;
                        }
                        *parentMatch25 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Primary");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<soul::rex::nfa::Nfa>(value));
            }
        }
        *parentMatch0 = match;
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
soul::parser::Match RexParser<LexerT>::Class(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Class");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655814);
    soul::rex::symbol::Class* cls = nullptr;
    std::unique_ptr<soul::rex::symbol::Symbol> r;
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
                        if (*lexer == LBRACKET)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            cls = context->MakeClass();
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
                                            soul::parser::Match match(false);
                                            if (*lexer == INVERSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                cls->SetInverse();
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
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        while (true)
                                        {
                                            int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch13 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = RexParser<LexerT>::Range(lexer, context);
                                                        r.reset(static_cast<soul::rex::symbol::Symbol*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            cls->AddSymbol(r.release());
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch13 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch12 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch7 = match;
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
                soul::parser::Match* parentMatch15 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch16 = &match;
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
                            *parentMatch16 = match;
                        }
                        else
                        {
                            lexer.ThrowExpectationFailure(pos, lexer.GetTokenInfo(RBRACKET));
                        }
                    }
                    *parentMatch15 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Class");
                #endif
                return soul::parser::Match(true, cls);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Class");
        else soul::lexer::WriteFailureToLog(lexer, "Class");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Range(LexerT& lexer, soul::rex::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Range");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655815);
    soul::rex::symbol::Symbol* symbol = nullptr;
    std::unique_ptr<soul::parser::Value<char32_t>> s;
    std::unique_ptr<soul::parser::Value<char32_t>> e;
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
                soul::parser::Match match = RexParser<LexerT>::Char(lexer);
                s.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
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
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == MINUS)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch6 = match;
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
                                        soul::parser::Match match = RexParser<LexerT>::Char(lexer);
                                        e.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                        if (match.hit)
                                        {
                                            symbol = context->MakeRange(s->value, e->value);
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                        soul::parser::Match match(true);
                                        if (match.hit)
                                        {
                                            symbol = context->MakeChar(s->value);
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Range");
                #endif
                return soul::parser::Match(true, symbol);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Range");
        else soul::lexer::WriteFailureToLog(lexer, "Range");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match RexParser<LexerT>::Char(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Char");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7261371920241655816);
    soul::parser::Match match(false);
    int64_t pos = lexer.GetPos();
    switch (*lexer)
    {
        case LPAREN:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('('));
                }
            }
            break;
        }
        case RPAREN:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>(')'));
                }
            }
            break;
        }
        case LBRACKET:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('['));
                }
            }
            break;
        }
        case ALT:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('|'));
                }
            }
            break;
        }
        case STAR:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('*'));
                }
            }
            break;
        }
        case PLUS:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('+'));
                }
            }
            break;
        }
        case QUEST:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('?'));
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
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('.'));
                }
            }
            break;
        }
        case ESCAPE:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>(soul::rex::token::parser::MakeEscapeValue(lexer.FileName(), lexer.GetToken(pos))));
                }
            }
            break;
        }
        case INVERSE:
        {
            ++lexer;
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('^'));
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
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>('-'));
                }
            }
            break;
        }
        case CHAR:
        {
            ++lexer;
            {
                auto token = lexer.GetToken(pos);
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char32_t>(token.Chr()));
                }
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
        else soul::lexer::WriteFailureToLog(lexer, "Char");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct RexParser<soul::lexer::Lexer<soul::rex::lexer::RexLexer<char32_t>, char32_t>>;

} // namespace soul::rex::parser
