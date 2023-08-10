
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/function.parser' using soul parser generator spg version 5.0.0

module cmajor.function.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.attribute.parser;
import cmajor.specifier.parser;
import cmajor.type.expr.parser;
import cmajor.templates.parser;
import cmajor.concepts.parser;
import cmajor.parameter.parser;
import cmajor.identifier.parser;
import cmajor.statement.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::attribute::parser;
using namespace cmajor::specifier::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::templates::parser;
using namespace cmajor::concepts::parser;
using namespace cmajor::parameter::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::statement::parser;

namespace cmajor::function::parser {

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::Function(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Function");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1238071297129316353);
    std::unique_ptr<cmajor::ast::FunctionNode> functionNode = std::unique_ptr<cmajor::ast::FunctionNode>();
    soul::ast::SourcePos s = soul::ast::SourcePos();
    std::unique_ptr<cmajor::ast::AttributesNode> attributes;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::Node> returnType;
    std::unique_ptr<soul::parser::Value<std::u32string>> functionGroupId;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                            soul::parser::Match match(true);
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
                                            soul::parser::Match match(true);
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                                attributes.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
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
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                        specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch13 = &match;
                                {
                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                    returnType.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    *parentMatch13 = match;
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
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = FunctionParser<LexerT>::FunctionGroupId(lexer, context);
                                    functionGroupId.reset(static_cast<soul::parser::Value<std::u32string>*>(match.value));
                                    if (match.hit)
                                    {
                                        functionNode.reset(new cmajor::ast::FunctionNode(s, context->ModuleId(), specifiers->value, returnType.release(), functionGroupId->value, attributes.release()));
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch14 = match;
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
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match = TemplateParser<LexerT>::TemplateParameterList(lexer, context, functionNode.get());
                            if (match.hit)
                            {
                                *parentMatch17 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
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
                soul::parser::Match* parentMatch18 = &match;
                {
                    soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, functionNode.get());
                    *parentMatch18 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch19 = &match;
            {
                soul::parser::Match match(true);
                int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch20 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch21 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch22 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                            constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                            if (match.hit)
                            {
                                functionNode->SetConstraint(constraint.release());
                            }
                            *parentMatch22 = match;
                        }
                        *parentMatch21 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch20 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch19 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch23 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch24 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch25 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch26 = &match;
                    switch (*lexer)
                    {
                        case LBRACE:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch27 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                if (match.hit)
                                {
                                    functionNode->SetBody(body.release());
                                }
                                *parentMatch27 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch26 = match;
                            }
                            break;
                        }
                        case SEMICOLON:
                        {
                            soul::parser::Match match(false);
                            if (*lexer == SEMICOLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                *parentMatch26 = match;
                            }
                            break;
                        }
                    }
                    *parentMatch25 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Function");
                        #endif
                        return soul::parser::Match(true, functionNode.release());
                    }
                }
                *parentMatch24 = match;
            }
            *parentMatch23 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Function");
        else soul::lexer::WriteFailureToLog(lexer, "Function");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::FunctionGroupId(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionGroupId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1238071297129316354);
    std::unique_ptr<soul::parser::Value<std::u32string>> operatorFunctionGroupId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
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
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionGroupId");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(lexer.GetToken(pos).ToString()));
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
                    soul::parser::Match match = FunctionParser<LexerT>::OperatorFunctionGroupId(lexer, context);
                    operatorFunctionGroupId.reset(static_cast<soul::parser::Value<std::u32string>*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionGroupId");
                            #endif
                            return soul::parser::Match(true, new soul::parser::Value<std::u32string>(operatorFunctionGroupId->value));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionGroupId");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionGroupId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::OperatorFunctionGroupId(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OperatorFunctionGroupId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1238071297129316355);
    std::unique_ptr<cmajor::ast::Node> typeExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
            soul::parser::Match match(true);
            if (match.hit)
            {
                vars->parsingOperatorFunctionGroupId = true;
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
                auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == OPERATOR)
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                switch (*lexer)
                                {
                                    case ASSIGN:
                                    case EQ:
                                    case LANGLE:
                                    case SHIFTRIGHT:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            switch (*lexer)
                                            {
                                                case LANGLE:
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
                                                            int64_t save = lexer.GetPos();
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch14 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch15 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == LANGLE)
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
                                                                            soul::parser::Match match(false);
                                                                            if (*lexer == LANGLE)
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
                                                                *parentMatch13 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch17 = &match;
                                                                {
                                                                    int64_t tmp = lexer.GetPos();
                                                                    lexer.SetPos(save);
                                                                    save = tmp;
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch18 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch19 = &match;
                                                                        {
                                                                            soul::parser::Match match(false);
                                                                            soul::parser::Match* parentMatch20 = &match;
                                                                            {
                                                                                soul::parser::Match match(false);
                                                                                soul::parser::Match* parentMatch21 = &match;
                                                                                {
                                                                                    soul::parser::Match match(false);
                                                                                    if (*lexer == LANGLE)
                                                                                    {
                                                                                        ++lexer;
                                                                                        match.hit = true;
                                                                                    }
                                                                                    *parentMatch21 = match;
                                                                                }
                                                                                if (match.hit)
                                                                                {
                                                                                    soul::parser::Match match(false);
                                                                                    soul::parser::Match* parentMatch22 = &match;
                                                                                    {
                                                                                        soul::parser::Match match(false);
                                                                                        if (*lexer == LANGLE)
                                                                                        {
                                                                                            ++lexer;
                                                                                            match.hit = true;
                                                                                        }
                                                                                        *parentMatch22 = match;
                                                                                    }
                                                                                    *parentMatch21 = match;
                                                                                }
                                                                                *parentMatch20 = match;
                                                                            }
                                                                            if (match.hit)
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
                                                                                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                                                            typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                                                            *parentMatch25 = match;
                                                                                        }
                                                                                        if (match.hit)
                                                                                        {
                                                                                            soul::parser::Match match(false);
                                                                                            soul::parser::Match* parentMatch26 = &match;
                                                                                            {
                                                                                                soul::parser::Match match(true);
                                                                                                soul::parser::Match* parentMatch27 = &match;
                                                                                                {
                                                                                                    while (true)
                                                                                                    {
                                                                                                        int64_t save = lexer.GetPos();
                                                                                                        {
                                                                                                            soul::parser::Match match(false);
                                                                                                            soul::parser::Match* parentMatch28 = &match;
                                                                                                            {
                                                                                                                soul::parser::Match match(false);
                                                                                                                if (*lexer == COMMA)
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
                                                                                                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                                                                                    typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                                                                                    *parentMatch29 = match;
                                                                                                                }
                                                                                                                *parentMatch28 = match;
                                                                                                            }
                                                                                                            if (match.hit)
                                                                                                            {
                                                                                                                *parentMatch27 = match;
                                                                                                            }
                                                                                                            else
                                                                                                            {
                                                                                                                lexer.SetPos(save);
                                                                                                                break;
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                                *parentMatch26 = match;
                                                                                            }
                                                                                            *parentMatch25 = match;
                                                                                        }
                                                                                        *parentMatch24 = match;
                                                                                    }
                                                                                    *parentMatch23 = match;
                                                                                }
                                                                                *parentMatch20 = match;
                                                                            }
                                                                            *parentMatch19 = match;
                                                                        }
                                                                        if (match.hit)
                                                                        {
                                                                            soul::parser::Match match(false);
                                                                            soul::parser::Match* parentMatch30 = &match;
                                                                            {
                                                                                soul::parser::Match match(false);
                                                                                if (*lexer == RANGLE)
                                                                                {
                                                                                    ++lexer;
                                                                                    match.hit = true;
                                                                                }
                                                                                *parentMatch30 = match;
                                                                            }
                                                                            *parentMatch19 = match;
                                                                        }
                                                                        *parentMatch18 = match;
                                                                    }
                                                                    *parentMatch17 = match;
                                                                }
                                                                if (!match.hit)
                                                                {
                                                                    lexer.SetPos(save);
                                                                }
                                                                *parentMatch13 = soul::parser::Match(!match.hit, match.value);
                                                            }
                                                            *parentMatch12 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            vars->parsingOperatorFunctionGroupId = false;
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator<<"));
                                                            }
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                                case SHIFTRIGHT:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch31 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                                        soul::parser::Match match(false);
                                                        if (*lexer == SHIFTRIGHT)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            vars->parsingOperatorFunctionGroupId = false;
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator>>"));
                                                            }
                                                        }
                                                        *parentMatch31 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                                case EQ:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch32 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                                        soul::parser::Match match(false);
                                                        if (*lexer == EQ)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            vars->parsingOperatorFunctionGroupId = false;
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator=="));
                                                            }
                                                        }
                                                        *parentMatch32 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch10 = match;
                                                    }
                                                    break;
                                                }
                                                case ASSIGN:
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch33 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                                        soul::parser::Match match(false);
                                                        if (*lexer == ASSIGN)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            vars->parsingOperatorFunctionGroupId = false;
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator="));
                                                            }
                                                        }
                                                        *parentMatch33 = match;
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
                                                soul::parser::Match* parentMatch34 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch35 = &match;
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
                                                            vars->parsingOperatorFunctionGroupId = false;
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                                #endif
                                                                return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator<"));
                                                            }
                                                        }
                                                        *parentMatch35 = match;
                                                    }
                                                    *parentMatch34 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
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
                                        soul::parser::Match* parentMatch36 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == ARROW)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator->"));
                                                }
                                            }
                                            *parentMatch36 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case PLUSPLUS:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch37 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == PLUSPLUS)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator++"));
                                                }
                                            }
                                            *parentMatch37 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case MINUSMINUS:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch38 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == MINUSMINUS)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator--"));
                                                }
                                            }
                                            *parentMatch38 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case PLUS:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch39 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == PLUS)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator+"));
                                                }
                                            }
                                            *parentMatch39 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case MINUS:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch40 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == MINUS)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator-"));
                                                }
                                            }
                                            *parentMatch40 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case STAR:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch41 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == STAR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator*"));
                                                }
                                            }
                                            *parentMatch41 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case DIV:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch42 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == DIV)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator/"));
                                                }
                                            }
                                            *parentMatch42 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case REM:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch43 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == REM)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator%"));
                                                }
                                            }
                                            *parentMatch43 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case AMP:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch44 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == AMP)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator&"));
                                                }
                                            }
                                            *parentMatch44 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case BITOR:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch45 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == BITOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator|"));
                                                }
                                            }
                                            *parentMatch45 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case BITXOR:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch46 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == BITXOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator^"));
                                                }
                                            }
                                            *parentMatch46 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case EXCLAMATION:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch47 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == EXCLAMATION)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator!"));
                                                }
                                            }
                                            *parentMatch47 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case CPL:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch48 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == CPL)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                vars->parsingOperatorFunctionGroupId = false;
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                    #endif
                                                    return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator~"));
                                                }
                                            }
                                            *parentMatch48 = match;
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
                                        soul::parser::Match* parentMatch49 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == LBRACKET)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch49 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch50 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch51 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                                    soul::parser::Match match(false);
                                                    if (*lexer == RBRACKET)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        vars->parsingOperatorFunctionGroupId = false;
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                            #endif
                                                            return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator[]"));
                                                        }
                                                    }
                                                    *parentMatch51 = match;
                                                }
                                                *parentMatch50 = match;
                                            }
                                            *parentMatch49 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                    case LPAREN:
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch52 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch52 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch53 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch54 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                                    soul::parser::Match match(false);
                                                    if (*lexer == RPAREN)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        vars->parsingOperatorFunctionGroupId = false;
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
                                                            #endif
                                                            return soul::parser::Match(true, new soul::parser::Value<std::u32string>(U"operator()"));
                                                        }
                                                    }
                                                    *parentMatch54 = match;
                                                }
                                                *parentMatch53 = match;
                                            }
                                            *parentMatch52 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        break;
                                    }
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    vars->parsingOperatorFunctionGroupId = false;
                }
                else
                {
                    vars->parsingOperatorFunctionGroupId = false;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionGroupId");
        else soul::lexer::WriteFailureToLog(lexer, "OperatorFunctionGroupId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct FunctionParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::function::parser
