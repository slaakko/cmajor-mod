
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/constant.parser' using soul parser generator spg version 5.0.0

module cmajor.constant.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import cmajor.specifier.parser;
import cmajor.identifier.parser;
import cmajor.type.expr.parser;
import cmajor.expression.parser;
import soul.ast.source.pos;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace cmajor::specifier::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::expression::parser;
using namespace soul::ast::source::pos;

namespace cmajor::constant::parser {

template<typename LexerT>
soul::parser::Match ConstantParser<LexerT>::Constant(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Constant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4312610854148243457);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
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
                                        soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                        specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
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
                                        if (*lexer == CONST)
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
                                    type.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                            if (*lexer == ASSIGN)
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
            cmajor::ast::ConstantNode *node = new cmajor::ast::ConstantNode(s, specifiers->value, type.release(), id.release(), expr.release());
            node->SetStrValue(node->Value()->ToString());
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                #endif
                return soul::parser::Match(true, node);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
        else soul::lexer::WriteFailureToLog(lexer, "Constant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ConstantParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::constant::parser
