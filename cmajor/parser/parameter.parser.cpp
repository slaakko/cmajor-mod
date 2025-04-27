
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/parameter.parser' using soul parser generator spg version 5.0.0

module cmajor.parameter.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import cmajor.type.expr.parser;
import cmajor.identifier.parser;
import soul.ast.source.pos;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace cmajor::type::expr::parser;
using namespace cmajor::identifier::parser;
using namespace soul::ast::source::pos;

namespace cmajor::parameter::parser {

template<typename LexerT>
soul::parser::Match ParameterParser<LexerT>::ParameterList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* owner)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParameterList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2143120652716998657);
    std::unique_ptr<cmajor::ast::ParameterNode> parameter;
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
                if (*lexer == LPAREN)
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
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
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
                                    soul::parser::Match match = ParameterParser<LexerT>::Parameter(lexer, context);
                                    parameter.reset(static_cast<cmajor::ast::ParameterNode*>(match.value));
                                    if (match.hit)
                                    {
                                        owner->AddParameter(parameter.release());
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
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        while (true)
                                        {
                                            int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == COMMA)
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
                                                            soul::parser::Match match = ParameterParser<LexerT>::Parameter(lexer, context);
                                                            parameter.reset(static_cast<cmajor::ast::ParameterNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                owner->AddParameter(parameter.release());
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch9 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch4 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParameterList");
        else soul::lexer::WriteFailureToLog(lexer, "ParameterList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ParameterParser<LexerT>::Parameter(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Parameter");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2143120652716998658);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
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
                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                    type.reset(static_cast<cmajor::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        s = lexer.GetSpan(pos);
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
                            soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                            id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Parameter");
                #endif
                return soul::parser::Match(true, new cmajor::ast::ParameterNode(s, type.release(), id.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Parameter");
        else soul::lexer::WriteFailureToLog(lexer, "Parameter");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ParameterParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::parameter::parser
