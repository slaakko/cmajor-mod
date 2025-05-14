
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/object/expr.parser' using soul parser generator spg version 5.0.0

module cmajor.systemx.object.expr.parser;

import util;
import soul.ast.spg;
import cmajor.systemx.object.expr.token;
import cmajor.systemx.object.expr.lexer;
import util;

using namespace cmajor::systemx::object::expr::token;
using namespace cmajor::systemx::object::expr::lexer;
using namespace util;

namespace cmajor::systemx::object::expr::parser {

template<typename LexerT>
std::unique_ptr<cmajor::systemx::object::Expr> ExprParser<LexerT>::Parse(LexerT& lexer)
{
    std::unique_ptr<cmajor::systemx::object::Expr> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = ExprParser<LexerT>::Expr(lexer);
    value.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
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
soul::parser::Match ExprParser<LexerT>::Expr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8093789852685828097);
    std::unique_ptr<cmajor::systemx::object::Expr> unary;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = ExprParser<LexerT>::UnaryExpr(lexer);
        unary.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expr");
                #endif
                return soul::parser::Match(true, unary.release());
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
soul::parser::Match ExprParser<LexerT>::UnaryExpr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8093789852685828098);
    std::unique_ptr<cmajor::systemx::object::Expr> expr = std::unique_ptr<cmajor::systemx::object::Expr>();
    std::unique_ptr<cmajor::systemx::object::Expr> unary;
    std::unique_ptr<cmajor::systemx::object::Expr> postfix;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case DEREF:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == DEREF)
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
                        soul::parser::Match match = ExprParser<LexerT>::UnaryExpr(lexer);
                        unary.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::object::DerefExpr(unary.release()));
                            }
                        }
                        *parentMatch3 = match;
                    }
                    *parentMatch2 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DOLLAR:
        case LPAREN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = ExprParser<LexerT>::PostfixExpr(lexer);
                postfix.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                        #endif
                        return soul::parser::Match(true, postfix.release());
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
soul::parser::Match ExprParser<LexerT>::PostfixExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PostfixExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8093789852685828099);
    std::unique_ptr<cmajor::systemx::object::Expr> expr = std::unique_ptr<cmajor::systemx::object::Expr>();
    int64_t index = int64_t();
    int64_t count = int64_t();
    std::unique_ptr<cmajor::systemx::object::Expr> primary;
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
                    soul::parser::Match match = ExprParser<LexerT>::PrimaryExpr(lexer);
                    primary.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
                    if (match.hit)
                    {
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
                                    {
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        switch (*lexer)
                                        {
                                            case DOT:
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch9 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == DOT)
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
                                                            soul::parser::Match match(false);
                                                            if (*lexer == ID)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                expr.reset(new cmajor::systemx::object::MemberExpr(expr.release(), util::ToUtf8(lexer.GetToken(pos).ToString())));
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
                                                break;
                                            }
                                            case ARROW:
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch12 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == ARROW)
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
                                                            soul::parser::Match match(false);
                                                            if (*lexer == ID)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                expr.reset(new cmajor::systemx::object::ArrowExpr(expr.release(), util::ToUtf8(lexer.GetToken(pos).ToString())));
                                                            }
                                                            *parentMatch14 = match;
                                                        }
                                                        *parentMatch13 = match;
                                                    }
                                                    *parentMatch12 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch8 = match;
                                                }
                                                break;
                                            }
                                            case LBRACKET:
                                            {
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
                                                                soul::parser::Match match(false);
                                                                if (*lexer == LBRACKET)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                *parentMatch18 = match;
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
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == INTEGER)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        if (match.hit)
                                                                        {
                                                                            index = cmajor::systemx::object::ParseInteger(util::ToUtf8(lexer.GetToken(pos).ToString()));
                                                                        }
                                                                        *parentMatch20 = match;
                                                                    }
                                                                    *parentMatch19 = match;
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch21 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                if (*lexer == COMMA)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                *parentMatch21 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch22 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch23 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match(false);
                                                                if (*lexer == INTEGER)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    count = cmajor::systemx::object::ParseInteger(util::ToUtf8(lexer.GetToken(pos).ToString()));
                                                                }
                                                                *parentMatch23 = match;
                                                            }
                                                            *parentMatch22 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch24 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch25 = &match;
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
                                                                expr.reset(new cmajor::systemx::object::RangeExpr(expr.release(), index, count));
                                                            }
                                                            *parentMatch25 = match;
                                                        }
                                                        *parentMatch24 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch8 = match;
                                                }
                                                break;
                                            }
                                        }
                                        *parentMatch7 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch26 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch27 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch28 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        if (*lexer == LBRACKET)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        *parentMatch28 = match;
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
                                                                if (*lexer == INTEGER)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    index = cmajor::systemx::object::ParseInteger(util::ToUtf8(lexer.GetToken(pos).ToString()));
                                                                }
                                                                *parentMatch30 = match;
                                                            }
                                                            *parentMatch29 = match;
                                                        }
                                                        *parentMatch28 = match;
                                                    }
                                                    *parentMatch27 = match;
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
                                                            soul::parser::Match match(false);
                                                            if (*lexer == RBRACKET)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                expr.reset(new cmajor::systemx::object::IndexExpr(expr.release(), index));
                                                            }
                                                            *parentMatch32 = match;
                                                        }
                                                        *parentMatch31 = match;
                                                    }
                                                    *parentMatch27 = match;
                                                }
                                                *parentMatch26 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PostfixExpr");
                #endif
                return soul::parser::Match(true, expr.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PostfixExpr");
        else soul::lexer::WriteFailureToLog(lexer, "PostfixExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ExprParser<LexerT>::PrimaryExpr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8093789852685828100);
    std::unique_ptr<cmajor::systemx::object::Expr> dollarExpr;
    std::unique_ptr<cmajor::systemx::object::Expr> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case DOLLAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = ExprParser<LexerT>::DollarExpr(lexer);
                dollarExpr.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, dollarExpr.release());
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
        case LPAREN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == LPAREN)
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
                        soul::parser::Match match = ExprParser<LexerT>::Expr(lexer);
                        expr.reset(static_cast<cmajor::systemx::object::Expr*>(match.value));
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
                    soul::parser::Match* parentMatch6 = &match;
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
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                                #endif
                                return soul::parser::Match(true, expr.release());
                            }
                        }
                        *parentMatch6 = match;
                    }
                    *parentMatch5 = match;
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
soul::parser::Match ExprParser<LexerT>::DollarExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DollarExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8093789852685828101);
    int64_t index = int64_t();
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == INTEGER)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            index = cmajor::systemx::object::ParseInteger(util::ToUtf8(lexer.GetToken(pos).ToString()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DollarExpr");
                #endif
                return soul::parser::Match(true, new cmajor::systemx::object::DollarExpr(index));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DollarExpr");
        else soul::lexer::WriteFailureToLog(lexer, "DollarExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ExprParser<soul::lexer::Lexer<cmajor::systemx::object::expr::lexer::ExprLexer<char32_t>, char32_t>>;

} // namespace cmajor::systemx::object::expr::parser
