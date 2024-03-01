
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/template.parser' using soul parser generator spg version 5.0.0

module cmajor.templates.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.identifier.parser;
import cmajor.type.expr.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::identifier::parser;
using namespace cmajor::type::expr::parser;

namespace cmajor::templates::parser {

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateId(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 559183602110169089);
    std::unique_ptr<cmajor::ast::TemplateIdNode> templateId = std::unique_ptr<cmajor::ast::TemplateIdNode>();
    std::unique_ptr<cmajor::ast::IdentifierNode> primary;
    std::unique_ptr<cmajor::ast::Node> templateArg;
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
                context->BeginParsingTemplateId();
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = IdentifierParser<LexerT>::QualifiedId(lexer, context);
                                primary.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                if (match.hit)
                                {
                                    templateId.reset(new cmajor::ast::TemplateIdNode(lexer.GetSpan(pos), primary.release()));
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
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                    soul::parser::Match match(false);
                                    if (*lexer == LANGLE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        ++vars->leftAngleCount;
                                    }
                                    *parentMatch9 = match;
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
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
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
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                    templateArg.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        templateId->AddTemplateArgument(templateArg.release());
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                *parentMatch15 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    soul::parser::Match match(true);
                                                    soul::parser::Match* parentMatch18 = &match;
                                                    {
                                                        while (true)
                                                        {
                                                            int64_t save = lexer.GetPos();
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch19 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    if (*lexer == COMMA)
                                                                    {
                                                                        ++lexer;
                                                                        match.hit = true;
                                                                    }
                                                                    *parentMatch19 = match;
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
                                                                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                                            templateArg.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                                            if (match.hit)
                                                                            {
                                                                                templateId->AddTemplateArgument(templateArg.release());
                                                                            }
                                                                            *parentMatch21 = match;
                                                                        }
                                                                        *parentMatch20 = match;
                                                                    }
                                                                    *parentMatch19 = match;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    *parentMatch18 = match;
                                                                }
                                                                else
                                                                {
                                                                    lexer.SetPos(save);
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    *parentMatch17 = match;
                                                }
                                                *parentMatch15 = match;
                                            }
                                            *parentMatch14 = match;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch22 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == RANGLE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch12 = match;
                                }
                                if (match.hit)
                                {
                                    --vars->leftAngleCount;
                                }
                                else
                                {
                                    --vars->leftAngleCount;
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    context->EndParsingTemplateId();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateId");
                        #endif
                        return soul::parser::Match(true, templateId.release());
                    }
                }
                else
                {
                    context->EndParsingTemplateId();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateId");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateParameterList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* owner)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateParameterList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 559183602110169090);
    std::unique_ptr<cmajor::ast::TemplateParameterNode> tp;
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
                if (*lexer == LANGLE)
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = TemplateParser<LexerT>::TemplateParameter(lexer, context);
                                tp.reset(static_cast<cmajor::ast::TemplateParameterNode*>(match.value));
                                if (match.hit)
                                {
                                    owner->AddTemplateParameter(tp.release());
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
                                                        soul::parser::Match match = TemplateParser<LexerT>::TemplateParameter(lexer, context);
                                                        tp.reset(static_cast<cmajor::ast::TemplateParameterNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            owner->AddTemplateParameter(tp.release());
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == RANGLE)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch12 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameterList");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateParameterList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateParameter(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateParameter");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 559183602110169091);
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
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
                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                    type.reset(static_cast<cmajor::ast::Node*>(match.value));
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameter");
                #endif
                return soul::parser::Match(true, new cmajor::ast::TemplateParameterNode(lexer.GetSpan(pos), id.release(), type.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameter");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateParameter");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::InstantiationRequest(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InstantiationRequest");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 559183602110169092);
    std::unique_ptr<cmajor::ast::FullInstantiationRequestNode> requestNode = std::unique_ptr<cmajor::ast::FullInstantiationRequestNode>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::TemplateIdNode> templateId;
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
                            if (*lexer == NEW)
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
                            if (*lexer == CLASS)
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = TemplateParser<LexerT>::TemplateId(lexer, context);
                            templateId.reset(static_cast<cmajor::ast::TemplateIdNode*>(match.value));
                            if (match.hit)
                            {
                                requestNode.reset(new cmajor::ast::FullInstantiationRequestNode(s, templateId.release()));
                            }
                            *parentMatch8 = match;
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
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InstantiationRequest");
                #endif
                return soul::parser::Match(true, requestNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InstantiationRequest");
        else soul::lexer::WriteFailureToLog(lexer, "InstantiationRequest");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct TemplateParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::templates::parser
