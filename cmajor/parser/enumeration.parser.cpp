
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/enumeration.parser' using soul parser generator spg version 5.0.0

module cmajor.enumeration.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.specifier.parser;
import cmajor.identifier.parser;
import cmajor.type.expr.parser;
import cmajor.expression.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::specifier::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::expression::parser;

namespace cmajor::enumeration::parser {

template<typename LexerT>
soul::parser::Match EnumerationParser<LexerT>::EnumType(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EnumType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2745111412182351873);
    std::unique_ptr<cmajor::ast::EnumTypeNode> enumTypeNode = std::unique_ptr<cmajor::ast::EnumTypeNode>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> enumTypeId;
    std::unique_ptr<cmajor::ast::Node> underlyingType;
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
                                        if (*lexer == ENUM)
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
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                        enumTypeId.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                        if (match.hit)
                                        {
                                            enumTypeNode.reset(new cmajor::ast::EnumTypeNode(s, specifiers->value, enumTypeId.release()));
                                        }
                                        *parentMatch11 = match;
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
                                            soul::parser::Match match = EnumerationParser<LexerT>::UnderlyingType(lexer, context);
                                            underlyingType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                enumTypeNode->SetUnderlyingType(underlyingType.release());
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
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                if (*lexer == LBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    enumTypeNode->SetBeginBraceSpan(lexer.GetSpan(pos));
                                }
                                *parentMatch17 = match;
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch18 = &match;
                    {
                        soul::parser::Match match = EnumerationParser<LexerT>::EnumConstants(lexer, context, enumTypeNode.get());
                        *parentMatch18 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        if (*lexer == RBRACE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            enumTypeNode->SetEndBraceSpan(lexer.GetSpan(pos));
                        }
                        *parentMatch20 = match;
                    }
                    *parentMatch19 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumType");
                #endif
                return soul::parser::Match(true, enumTypeNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumType");
        else soul::lexer::WriteFailureToLog(lexer, "EnumType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match EnumerationParser<LexerT>::UnderlyingType(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnderlyingType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2745111412182351874);
    std::unique_ptr<cmajor::ast::Node> type;
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
                if (*lexer == COLON)
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
                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                    type.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnderlyingType");
                #endif
                return soul::parser::Match(true, type.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnderlyingType");
        else soul::lexer::WriteFailureToLog(lexer, "UnderlyingType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match EnumerationParser<LexerT>::EnumConstants(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::EnumTypeNode* enumType)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EnumConstants");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2745111412182351875);
    std::unique_ptr<cmajor::ast::EnumConstantNode> enumConstant;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = EnumerationParser<LexerT>::EnumConstant(lexer, context, enumType);
                enumConstant.reset(static_cast<cmajor::ast::EnumConstantNode*>(match.value));
                if (match.hit)
                {
                    enumType->AddConstant(enumConstant.release());
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch4 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch5 = match;
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
                                        soul::parser::Match match = EnumerationParser<LexerT>::EnumConstant(lexer, context, enumType);
                                        enumConstant.reset(static_cast<cmajor::ast::EnumConstantNode*>(match.value));
                                        if (match.hit)
                                        {
                                            enumType->AddConstant(enumConstant.release());
                                        }
                                        *parentMatch7 = match;
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
                                break;
                            }
                        }
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumConstants");
        else soul::lexer::WriteFailureToLog(lexer, "EnumConstants");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match EnumerationParser<LexerT>::EnumConstant(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::EnumTypeNode* enumType)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EnumConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2745111412182351876);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::IdentifierNode> constantId;
    std::unique_ptr<cmajor::ast::Node> constantValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                constantId.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                if (match.hit)
                {
                    s = lexer.GetSpan(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
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
                            if (*lexer == ASSIGN)
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
                                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                    constantValue.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        cmajor::ast::EnumConstantNode *enumConstant = new cmajor::ast::EnumConstantNode(s, constantId.release(), constantValue.release());
                                        enumConstant->SetHasValue();
                                        enumConstant->SetStrValue(enumConstant->GetValue()->ToString());
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumConstant");
                                            #endif
                                            return soul::parser::Match(true, enumConstant);
                                        }
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
                                        cmajor::ast::EnumConstantNode *enumConstant = new cmajor::ast::EnumConstantNode(s, constantId.release(), cmajor::ast::MakeNextEnumConstantValue(s, enumType));
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumConstant");
                                            #endif
                                            return soul::parser::Match(true, enumConstant);
                                        }
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumConstant");
        else soul::lexer::WriteFailureToLog(lexer, "EnumConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct EnumerationParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::enumeration::parser
