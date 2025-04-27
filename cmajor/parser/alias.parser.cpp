
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/alias.parser' using soul parser generator spg version 5.0.0

module cmajor.alias.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import cmajor.specifier.parser;
import cmajor.identifier.parser;
import cmajor.type.expr.parser;
import soul.ast.source.pos;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace cmajor::specifier::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::type::expr::parser;
using namespace soul::ast::source::pos;

namespace cmajor::alias::parser {

template<typename LexerT>
soul::parser::Match AliasParser<LexerT>::UsingAlias(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UsingAlias");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7445274796289425409);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                    specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                    if (match.hit)
                                    {
                                        s = lexer.GetSpan(pos);
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
                                    if (*lexer == USING)
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
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                            if (*lexer == ASSIGN)
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
                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                        typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UsingAlias");
                #endif
                return soul::parser::Match(true, new cmajor::ast::AliasNode(s, specifiers->value, id.release(), typeExpr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UsingAlias");
        else soul::lexer::WriteFailureToLog(lexer, "UsingAlias");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AliasParser<LexerT>::Typedef(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Typedef");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7445274796289425410);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::Node> typeExpr;
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
                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
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
                                if (*lexer == TYPEDEF)
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
                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                            typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                        id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Typedef");
                #endif
                return soul::parser::Match(true, new cmajor::ast::TypedefNode(s, specifiers->value, typeExpr.release(), id.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Typedef");
        else soul::lexer::WriteFailureToLog(lexer, "Typedef");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct AliasParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::alias::parser
