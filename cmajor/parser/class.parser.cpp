
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/class.parser' using soul parser generator spg version 5.0.0

module cmajor.classes.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.attribute.parser;
import cmajor.specifier.parser;
import cmajor.identifier.parser;
import cmajor.type.expr.parser;
import cmajor.concepts.parser;
import cmajor.expression.parser;
import cmajor.statement.parser;
import cmajor.parameter.parser;
import cmajor.function.parser;
import cmajor.templates.parser;
import cmajor.alias.parser;
import cmajor.enumeration.parser;
import cmajor.constant.parser;
import cmajor.delegate.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::attribute::parser;
using namespace cmajor::specifier::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::concepts::parser;
using namespace cmajor::expression::parser;
using namespace cmajor::statement::parser;
using namespace cmajor::parameter::parser;
using namespace cmajor::function::parser;
using namespace cmajor::templates::parser;
using namespace cmajor::alias::parser;
using namespace cmajor::enumeration::parser;
using namespace cmajor::constant::parser;
using namespace cmajor::delegate::parser;

namespace cmajor::classes::parser {

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::Class(LexerT& lexer, cmajor::parser::context::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120449);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    soul::ast::Span classSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ClassNode> classNode = std::unique_ptr<cmajor::ast::ClassNode>();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match(true);
                                                int64_t save = lexer.GetPos();
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                                    attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
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
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch12 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch13 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch14 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch15 = &match;
                                                            int64_t save = lexer.GetPos();
                                                            {
                                                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                                *parentMatch15 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch16 = &match;
                                                                {
                                                                    int64_t tmp = lexer.GetPos();
                                                                    lexer.SetPos(save);
                                                                    save = tmp;
                                                                    soul::parser::Match match(false);
                                                                    if (*lexer == NEW)
                                                                    {
                                                                        ++lexer;
                                                                        match.hit = true;
                                                                    }
                                                                    *parentMatch16 = match;
                                                                }
                                                                if (!match.hit)
                                                                {
                                                                    lexer.SetPos(save);
                                                                }
                                                                *parentMatch15 = soul::parser::Match(!match.hit, match.value);
                                                            }
                                                            *parentMatch14 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            specifierSpan = lexer.GetSpan(pos);
                                                        }
                                                        *parentMatch13 = match;
                                                    }
                                                    *parentMatch12 = match;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
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
                                                    if (*lexer == CLASS)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        classSpan = lexer.GetSpan(pos);
                                                    }
                                                    *parentMatch18 = match;
                                                }
                                                *parentMatch17 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
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
                                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    s = lexer.GetSpan(pos);
                                                    classNode.reset(new cmajor::ast::ClassNode(s, specifiers->value, id.release(), attrs.release()));
                                                    classNode->SetSpecifierSpan(specifierSpan);
                                                    classNode->SetClassSpan(classSpan);
                                                }
                                                *parentMatch20 = match;
                                            }
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch21 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch22 = &match;
                                        {
                                            soul::parser::Match match = TemplateParser<LexerT>::TemplateParameterList(lexer, context, classNode.get());
                                            if (match.hit)
                                            {
                                                *parentMatch22 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch23 = &match;
                                {
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch24 = &match;
                                    {
                                        soul::parser::Match match = ClassParser<LexerT>::InheritanceAndInterfaces(lexer, context, classNode.get());
                                        if (match.hit)
                                        {
                                            *parentMatch24 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch23 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch25 = &match;
                            {
                                soul::parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch26 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch27 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch28 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                            constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                            if (match.hit)
                                            {
                                                classNode->SetConstraint(constraint.release());
                                            }
                                            *parentMatch28 = match;
                                        }
                                        *parentMatch27 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch26 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
                                }
                                *parentMatch25 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                if (*lexer == LBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    classNode->SetBeginBraceSpan(lexer.GetSpan(pos));
                                }
                                *parentMatch30 = match;
                            }
                            *parentMatch29 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch31 = &match;
                    {
                        soul::parser::Match match = ClassParser<LexerT>::ClassContent(lexer, context, classNode.get());
                        *parentMatch31 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch32 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch33 = &match;
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
                            classNode->SetEndBraceSpan(lexer.GetSpan(pos));
                        }
                        *parentMatch33 = match;
                    }
                    *parentMatch32 = match;
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
                return soul::parser::Match(true, classNode.release());
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
soul::parser::Match ClassParser<LexerT>::InheritanceAndInterfaces(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InheritanceAndInterfaces");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120450);
    std::unique_ptr<cmajor::ast::Node> baseOrInterface;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == COLON)
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
                                soul::parser::Match match(true);
                                if (match.hit)
                                {
                                    context->BeginParsingTypeExpr();
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
                                    soul::parser::Match match = ClassParser<LexerT>::BaseClassOrInterface(lexer, context);
                                    baseOrInterface.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        classNode->AddBaseClassOrInterface(baseOrInterface.release());
                                        context->EndParsingTypeExpr();
                                    }
                                    else
                                    {
                                        context->EndParsingTypeExpr();
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
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
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == COMMA)
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
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch14 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch15 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match(true);
                                                        if (match.hit)
                                                        {
                                                            context->BeginParsingTypeExpr();
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
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = ClassParser<LexerT>::BaseClassOrInterface(lexer, context);
                                                            baseOrInterface.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                classNode->AddBaseClassOrInterface(baseOrInterface.release());
                                                                context->EndParsingTypeExpr();
                                                            }
                                                            else
                                                            {
                                                                context->EndParsingTypeExpr();
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch10 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                        break;
                                    }
                                }
                            }
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch3 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InheritanceAndInterfaces");
        else soul::lexer::WriteFailureToLog(lexer, "InheritanceAndInterfaces");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::BaseClassOrInterface(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BaseClassOrInterface");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120451);
    std::unique_ptr<cmajor::ast::TemplateIdNode> templateId;
    std::unique_ptr<cmajor::ast::IdentifierNode> qualifiedId;
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
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = TemplateParser<LexerT>::TemplateId(lexer, context);
                templateId.reset(static_cast<cmajor::ast::TemplateIdNode*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BaseClassOrInterface");
                        #endif
                        return soul::parser::Match(true, templateId.release());
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
                        soul::parser::Match match = IdentifierParser<LexerT>::QualifiedId(lexer, context);
                        qualifiedId.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BaseClassOrInterface");
                                #endif
                                return soul::parser::Match(true, qualifiedId.release());
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
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BaseClassOrInterface");
        else soul::lexer::WriteFailureToLog(lexer, "BaseClassOrInterface");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::ClassContent(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ClassContent");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120452);
    std::unique_ptr<cmajor::ast::Node> classMember;
    soul::parser::Match match(true);
    soul::parser::Match* parentMatch0 = &match;
    {
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch1 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch2 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = ClassParser<LexerT>::ClassMember(lexer, context, classNode);
                        classMember.reset(static_cast<cmajor::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            classNode->AddMember(classMember.release());
                        }
                        *parentMatch2 = match;
                    }
                    *parentMatch1 = match;
                }
                if (match.hit)
                {
                    *parentMatch0 = match;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassContent");
        else soul::lexer::WriteFailureToLog(lexer, "ClassContent");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::ClassMember(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ClassMember");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120453);
    std::unique_ptr<cmajor::ast::StaticConstructorNode> staticConstructor;
    std::unique_ptr<cmajor::ast::ConstructorNode> constructor;
    std::unique_ptr<cmajor::ast::DestructorNode> destructor;
    std::unique_ptr<cmajor::ast::MemberFunctionNode> memberFunction;
    std::unique_ptr<cmajor::ast::ConversionFunctionNode> conversionFunction;
    std::unique_ptr<cmajor::ast::MemberVariableNode> memberVariable;
    std::unique_ptr<cmajor::ast::Node> usingAlias;
    std::unique_ptr<cmajor::ast::Node> typedefinition;
    std::unique_ptr<cmajor::ast::ClassNode> cls;
    std::unique_ptr<cmajor::ast::EnumTypeNode> enumType;
    std::unique_ptr<cmajor::ast::Node> constant;
    std::unique_ptr<cmajor::ast::DelegateNode> delegate;
    std::unique_ptr<cmajor::ast::ClassDelegateNode> classDelegate;
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
                                            {
                                                int64_t save = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    int64_t save = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = ClassParser<LexerT>::StaticConstructor(lexer, context, classNode);
                                                        staticConstructor.reset(static_cast<cmajor::ast::StaticConstructorNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                                #endif
                                                                return soul::parser::Match(true, staticConstructor.release());
                                                            }
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                    if (!match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch14 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = ClassParser<LexerT>::Constructor(lexer, context, classNode);
                                                                constructor.reset(static_cast<cmajor::ast::ConstructorNode*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    {
                                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                                        #endif
                                                                        return soul::parser::Match(true, constructor.release());
                                                                    }
                                                                }
                                                                *parentMatch14 = match;
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                }
                                                *parentMatch10 = match;
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
                                                            soul::parser::Match match = ClassParser<LexerT>::Destructor(lexer, context, classNode);
                                                            destructor.reset(static_cast<cmajor::ast::DestructorNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                                    #endif
                                                                    return soul::parser::Match(true, destructor.release());
                                                                }
                                                            }
                                                            *parentMatch16 = match;
                                                        }
                                                        *parentMatch15 = match;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                            }
                                            *parentMatch9 = match;
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
                                                        soul::parser::Match match = ClassParser<LexerT>::MemberFunction(lexer, context);
                                                        memberFunction.reset(static_cast<cmajor::ast::MemberFunctionNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                                #endif
                                                                return soul::parser::Match(true, memberFunction.release());
                                                            }
                                                        }
                                                        *parentMatch18 = match;
                                                    }
                                                    *parentMatch17 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
                                        }
                                        *parentMatch8 = match;
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
                                                    soul::parser::Match match = ClassParser<LexerT>::ConversionFunction(lexer, context);
                                                    conversionFunction.reset(static_cast<cmajor::ast::ConversionFunctionNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                            #endif
                                                            return soul::parser::Match(true, conversionFunction.release());
                                                        }
                                                    }
                                                    *parentMatch20 = match;
                                                }
                                                *parentMatch19 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                    }
                                    *parentMatch7 = match;
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
                                                soul::parser::Match match = ClassParser<LexerT>::MemberVariable(lexer, context);
                                                memberVariable.reset(static_cast<cmajor::ast::MemberVariableNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    {
                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                        #endif
                                                        return soul::parser::Match(true, memberVariable.release());
                                                    }
                                                }
                                                *parentMatch22 = match;
                                            }
                                            *parentMatch21 = match;
                                        }
                                        *parentMatch7 = match;
                                    }
                                }
                                *parentMatch6 = match;
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
                                            soul::parser::Match match = AliasParser<LexerT>::UsingAlias(lexer, context);
                                            usingAlias.reset(static_cast<cmajor::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                    #endif
                                                    return soul::parser::Match(true, usingAlias.release());
                                                }
                                            }
                                            *parentMatch24 = match;
                                        }
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
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
                                        soul::parser::Match match = AliasParser<LexerT>::Typedef(lexer, context);
                                        typedefinition.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                                #endif
                                                return soul::parser::Match(true, typedefinition.release());
                                            }
                                        }
                                        *parentMatch26 = match;
                                    }
                                    *parentMatch25 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
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
                                    soul::parser::Match match = ClassParser<LexerT>::Class(lexer, context);
                                    cls.reset(static_cast<cmajor::ast::ClassNode*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                            #endif
                                            return soul::parser::Match(true, cls.release());
                                        }
                                    }
                                    *parentMatch28 = match;
                                }
                                *parentMatch27 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
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
                                soul::parser::Match match = EnumerationParser<LexerT>::EnumType(lexer, context);
                                enumType.reset(static_cast<cmajor::ast::EnumTypeNode*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                        #endif
                                        return soul::parser::Match(true, enumType.release());
                                    }
                                }
                                *parentMatch30 = match;
                            }
                            *parentMatch29 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
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
                            soul::parser::Match match = ConstantParser<LexerT>::Constant(lexer, context);
                            constant.reset(static_cast<cmajor::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                    #endif
                                    return soul::parser::Match(true, constant.release());
                                }
                            }
                            *parentMatch32 = match;
                        }
                        *parentMatch31 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = DelegateParser<LexerT>::Delegate(lexer, context);
                        delegate.reset(static_cast<cmajor::ast::DelegateNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                                #endif
                                return soul::parser::Match(true, delegate.release());
                            }
                        }
                        *parentMatch34 = match;
                    }
                    *parentMatch33 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = DelegateParser<LexerT>::ClassDelegate(lexer, context);
                    classDelegate.reset(static_cast<cmajor::ast::ClassDelegateNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
                            #endif
                            return soul::parser::Match(true, classDelegate.release());
                        }
                    }
                    *parentMatch36 = match;
                }
                *parentMatch35 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClassMember");
        else soul::lexer::WriteFailureToLog(lexer, "ClassMember");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::StaticConstructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StaticConstructor");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120454);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::StaticConstructorNode> staticConstructorNode = std::unique_ptr<cmajor::ast::StaticConstructorNode>();
    std::u32string className = std::u32string();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::InitializerNode> initializer;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                            attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
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
                                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                if (match.hit)
                                                {
                                                    s = lexer.GetSpan(pos);
                                                    specifierSpan = s;
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
                                            soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                            id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                            if (match.hit)
                                            {
                                                className = lexer.GetToken(pos).ToString();
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
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                    bool pass = true;
                                    soul::parser::Match match(false);
                                    if (*lexer == RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        if (className != classNode->Id()->Str() || !cmajor::ast::StaticConstructorSpecifiers(specifiers->value))
                                        {
                                            pass = false;
                                        }
                                        else
                                        {
                                            staticConstructorNode.reset(new cmajor::ast::StaticConstructorNode(s, specifiers->value, attrs.release()));
                                            staticConstructorNode->SetClassId(id.release());
                                            staticConstructorNode->SetSpecifierSpan(specifierSpan);
                                        }
                                    }
                                    if (match.hit && !pass)
                                    {
                                        match = soul::parser::Match(false);
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch18 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch19 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch20 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch20 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch21 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch22 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch23 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch24 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = ClassParser<LexerT>::Initializer(lexer, context);
                                                        initializer.reset(static_cast<cmajor::ast::InitializerNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            staticConstructorNode->AddInitializer(initializer.release());
                                                        }
                                                        *parentMatch24 = match;
                                                    }
                                                    *parentMatch23 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch25 = &match;
                                                    {
                                                        soul::parser::Match match(true);
                                                        soul::parser::Match* parentMatch26 = &match;
                                                        {
                                                            while (true)
                                                            {
                                                                int64_t save = lexer.GetPos();
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch27 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == COMMA)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        *parentMatch27 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch28 = &match;
                                                                        {
                                                                            soul::parser::Match match(false);
                                                                            soul::parser::Match* parentMatch29 = &match;
                                                                            {
                                                                                int64_t pos = lexer.GetPos();
                                                                                soul::parser::Match match = ClassParser<LexerT>::Initializer(lexer, context);
                                                                                initializer.reset(static_cast<cmajor::ast::InitializerNode*>(match.value));
                                                                                if (match.hit)
                                                                                {
                                                                                    staticConstructorNode->AddInitializer(initializer.release());
                                                                                }
                                                                                *parentMatch29 = match;
                                                                            }
                                                                            *parentMatch28 = match;
                                                                        }
                                                                        *parentMatch27 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        *parentMatch26 = match;
                                                                    }
                                                                    else
                                                                    {
                                                                        lexer.SetPos(save);
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        *parentMatch25 = match;
                                                    }
                                                    *parentMatch23 = match;
                                                }
                                                *parentMatch22 = match;
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
                                }
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch30 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch31 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch32 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch33 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                    constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                    if (match.hit)
                                    {
                                        staticConstructorNode->SetConstraint(constraint.release());
                                    }
                                    *parentMatch33 = match;
                                }
                                *parentMatch32 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch31 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch30 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch34 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch35 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch36 = &match;
                        switch (*lexer)
                        {
                            case LBRACE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch37 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                    body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        staticConstructorNode->SetBody(body.release());
                                    }
                                    *parentMatch37 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch36 = match;
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
                                    *parentMatch36 = match;
                                }
                                break;
                            }
                        }
                        *parentMatch35 = match;
                    }
                    *parentMatch34 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StaticConstructor");
                #endif
                return soul::parser::Match(true, staticConstructorNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StaticConstructor");
        else soul::lexer::WriteFailureToLog(lexer, "StaticConstructor");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::Constructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Constructor");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120455);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConstructorNode> constructorNode = std::unique_ptr<cmajor::ast::ConstructorNode>();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::InitializerNode> initializer;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                        attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
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
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                            specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                            if (match.hit)
                                            {
                                                s = lexer.GetSpan(pos);
                                                specifierSpan = s;
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        bool pass = true;
                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                        id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                        if (match.hit)
                                        {
                                            std::u32string className = lexer.GetToken(pos).ToString();
                                            if (className != classNode->Id()->Str())
                                            {
                                                pass = false;
                                            }
                                            else
                                            {
                                                constructorNode.reset(new cmajor::ast::ConstructorNode(s, specifiers->value, attrs.release()));
                                                constructorNode->SetClassId(id.release());
                                                constructorNode->SetSpecifierSpan(specifierSpan);
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
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, constructorNode.get());
                                *parentMatch13 = match;
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
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch16 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch17 = match;
                                    }
                                    if (match.hit)
                                    {
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
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = ClassParser<LexerT>::Initializer(lexer, context);
                                                        initializer.reset(static_cast<cmajor::ast::InitializerNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            constructorNode->AddInitializer(initializer.release());
                                                        }
                                                        *parentMatch21 = match;
                                                    }
                                                    *parentMatch20 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch22 = &match;
                                                    {
                                                        soul::parser::Match match(true);
                                                        soul::parser::Match* parentMatch23 = &match;
                                                        {
                                                            while (true)
                                                            {
                                                                int64_t save = lexer.GetPos();
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch24 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == COMMA)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        *parentMatch24 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch25 = &match;
                                                                        {
                                                                            soul::parser::Match match(false);
                                                                            soul::parser::Match* parentMatch26 = &match;
                                                                            {
                                                                                int64_t pos = lexer.GetPos();
                                                                                soul::parser::Match match = ClassParser<LexerT>::Initializer(lexer, context);
                                                                                initializer.reset(static_cast<cmajor::ast::InitializerNode*>(match.value));
                                                                                if (match.hit)
                                                                                {
                                                                                    constructorNode->AddInitializer(initializer.release());
                                                                                }
                                                                                *parentMatch26 = match;
                                                                            }
                                                                            *parentMatch25 = match;
                                                                        }
                                                                        *parentMatch24 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        *parentMatch23 = match;
                                                                    }
                                                                    else
                                                                    {
                                                                        lexer.SetPos(save);
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        *parentMatch22 = match;
                                                    }
                                                    *parentMatch20 = match;
                                                }
                                                *parentMatch19 = match;
                                            }
                                            *parentMatch18 = match;
                                        }
                                        *parentMatch17 = match;
                                    }
                                    *parentMatch16 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch15 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
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
                    soul::parser::Match* parentMatch27 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch28 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch29 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch30 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                    constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                    if (match.hit)
                                    {
                                        constructorNode->SetConstraint(constraint.release());
                                    }
                                    *parentMatch30 = match;
                                }
                                *parentMatch29 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch28 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch27 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch31 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch32 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch33 = &match;
                        switch (*lexer)
                        {
                            case LBRACE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch34 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                    body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        constructorNode->SetBody(body.release());
                                    }
                                    *parentMatch34 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch33 = match;
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
                                    *parentMatch33 = match;
                                }
                                break;
                            }
                        }
                        *parentMatch32 = match;
                    }
                    *parentMatch31 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constructor");
                #endif
                return soul::parser::Match(true, constructorNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constructor");
        else soul::lexer::WriteFailureToLog(lexer, "Constructor");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::Destructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Destructor");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120456);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::DestructorNode> destructorNode = std::unique_ptr<cmajor::ast::DestructorNode>();
    std::u32string className = std::u32string();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> id;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> contraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                            attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
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
                                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                if (match.hit)
                                                {
                                                    s = lexer.GetSpan(pos);
                                                    specifierSpan = s;
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
                                        if (*lexer == CPL)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
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
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                        id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                        if (match.hit)
                                        {
                                            className = lexer.GetToken(pos).ToString();
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch15 = match;
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
                                bool pass = true;
                                soul::parser::Match match(false);
                                if (*lexer == RPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    if (className != classNode->Id()->Str())
                                    {
                                        pass = false;
                                    }
                                    else
                                    {
                                        destructorNode.reset(new cmajor::ast::DestructorNode(s, specifiers->value, attrs.release()));
                                        destructorNode->SetClassId(id.release());
                                        destructorNode->SetSpecifierSpan(specifierSpan);
                                    }
                                }
                                if (match.hit && !pass)
                                {
                                    match = soul::parser::Match(false);
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
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch19 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch20 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch21 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                    contraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                    if (match.hit)
                                    {
                                        destructorNode->SetConstraint(contraint.release());
                                    }
                                    *parentMatch21 = match;
                                }
                                *parentMatch20 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch19 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch18 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch22 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch23 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch24 = &match;
                        switch (*lexer)
                        {
                            case LBRACE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch25 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                    body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        destructorNode->SetBody(body.release());
                                    }
                                    *parentMatch25 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch24 = match;
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
                                    *parentMatch24 = match;
                                }
                                break;
                            }
                        }
                        *parentMatch23 = match;
                    }
                    *parentMatch22 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Destructor");
                #endif
                return soul::parser::Match(true, destructorNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Destructor");
        else soul::lexer::WriteFailureToLog(lexer, "Destructor");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::Initializer(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Initializer");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120457);
    std::unique_ptr<cmajor::ast::InitializerNode> initializerNode = std::unique_ptr<cmajor::ast::InitializerNode>();
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
            switch (*lexer)
            {
                case THIS:
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
                                    if (*lexer == THIS)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        initializerNode.reset(new cmajor::ast::ThisInitializerNode(lexer.GetSpan(pos)));
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
                                    if (*lexer == LPAREN)
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
                                soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, initializerNode.get());
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
                            soul::parser::Match match(false);
                            if (*lexer == RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case BASE:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
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
                                    if (*lexer == BASE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        initializerNode.reset(new cmajor::ast::BaseInitializerNode(lexer.GetSpan(pos)));
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
                                    soul::parser::Match match(false);
                                    if (*lexer == LPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
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
                                soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, initializerNode.get());
                                *parentMatch15 = match;
                            }
                            *parentMatch11 = match;
                        }
                        *parentMatch10 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch16 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch10 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case ID:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch17 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                    id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                    if (match.hit)
                                    {
                                        initializerNode.reset(new cmajor::ast::MemberInitializerNode(lexer.GetSpan(pos), id.release()));
                                    }
                                    *parentMatch20 = match;
                                }
                                *parentMatch19 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch21 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == LPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch21 = match;
                                }
                                *parentMatch19 = match;
                            }
                            *parentMatch18 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch22 = &match;
                            {
                                soul::parser::Match match = ExpressionParser<LexerT>::ArgumentList(lexer, context, initializerNode.get());
                                *parentMatch22 = match;
                            }
                            *parentMatch18 = match;
                        }
                        *parentMatch17 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch23 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch23 = match;
                        }
                        *parentMatch17 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Initializer");
                #endif
                return soul::parser::Match(true, initializerNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Initializer");
        else soul::lexer::WriteFailureToLog(lexer, "Initializer");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::MemberFunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MemberFunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120458);
    std::unique_ptr<cmajor::ast::MemberFunctionNode> memberFunctionNode = std::unique_ptr<cmajor::ast::MemberFunctionNode>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    soul::ast::Span groupIdSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::Node> returnType;
    std::unique_ptr<soul::parser::Value<std::u32string>> groupId;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                            attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
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
                                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                if (match.hit)
                                                {
                                                    s = lexer.GetSpan(pos);
                                                    specifierSpan = s;
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
                                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                        returnType.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = FunctionParser<LexerT>::FunctionGroupId(lexer, context);
                                                groupId.reset(static_cast<soul::parser::Value<std::u32string>*>(match.value));
                                                if (match.hit)
                                                {
                                                    groupIdSpan = lexer.GetSpan(pos);
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        if (match.hit)
                                        {
                                            memberFunctionNode.reset(new cmajor::ast::MemberFunctionNode(s, specifiers->value, returnType.release(), groupId->value, attrs.release()));
                                            memberFunctionNode->SetSpecifierSpan(specifierSpan);
                                            memberFunctionNode->SetGroupIdSpan(groupIdSpan);
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch17 = &match;
                            {
                                soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, memberFunctionNode.get());
                                *parentMatch17 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch21 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == CONST)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            memberFunctionNode->SetConst();
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch20 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch19 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch18 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch22 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch23 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch24 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch25 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                    constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                    if (match.hit)
                                    {
                                        memberFunctionNode->SetConstraint(constraint.release());
                                    }
                                    *parentMatch25 = match;
                                }
                                *parentMatch24 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch23 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch22 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch26 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch27 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch28 = &match;
                        switch (*lexer)
                        {
                            case LBRACE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch29 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                    body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        memberFunctionNode->SetBody(body.release());
                                    }
                                    *parentMatch29 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch28 = match;
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
                                    *parentMatch28 = match;
                                }
                                break;
                            }
                        }
                        *parentMatch27 = match;
                    }
                    *parentMatch26 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberFunction");
                #endif
                return soul::parser::Match(true, memberFunctionNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberFunction");
        else soul::lexer::WriteFailureToLog(lexer, "MemberFunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::ConversionFunction(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionFunction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120459);
    std::unique_ptr<cmajor::ast::ConversionFunctionNode> conversionFunctionNode = std::unique_ptr<cmajor::ast::ConversionFunctionNode>();
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> body;
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(true);
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                                attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    *parentMatch10 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                }
                                            }
                                            *parentMatch9 = match;
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
                                                    soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                    specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        s = lexer.GetSpan(pos);
                                                        specifierSpan = s;
                                                    }
                                                    *parentMatch12 = match;
                                                }
                                                *parentMatch11 = match;
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
                                            soul::parser::Match match(false);
                                            if (*lexer == OPERATOR)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                        type.reset(static_cast<cmajor::ast::Node*>(match.value));
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == LPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                        conversionFunctionNode.reset(new cmajor::ast::ConversionFunctionNode(s, specifiers->value, type.release(), attrs.release()));
                                        conversionFunctionNode->SetSpecifierSpan(specifierSpan);
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch21 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == CONST)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            conversionFunctionNode->SetConst();
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch20 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch19 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch18 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch22 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch23 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch24 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch25 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                    constraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                    if (match.hit)
                                    {
                                        conversionFunctionNode->SetConstraint(constraint.release());
                                    }
                                    *parentMatch25 = match;
                                }
                                *parentMatch24 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch23 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch22 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch26 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch27 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch28 = &match;
                        switch (*lexer)
                        {
                            case LBRACE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch29 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
                                    body.reset(static_cast<cmajor::ast::CompoundStatementNode*>(match.value));
                                    if (match.hit)
                                    {
                                        conversionFunctionNode->SetBody(body.release());
                                    }
                                    *parentMatch29 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch28 = match;
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
                                    *parentMatch28 = match;
                                }
                                break;
                            }
                        }
                        *parentMatch27 = match;
                    }
                    *parentMatch26 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionFunction");
                #endif
                return soul::parser::Match(true, conversionFunctionNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionFunction");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionFunction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ClassParser<LexerT>::MemberVariable(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MemberVariable");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 297007137526120460);
    soul::ast::Span s = soul::ast::Span();
    soul::ast::Span specifierSpan = soul::ast::Span();
    std::unique_ptr<cmajor::ast::AttributesNode> attrs;
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match = AttributeParser<LexerT>::Attributes(lexer, context);
                                attrs.reset(static_cast<cmajor::ast::AttributesNode*>(match.value));
                                if (match.hit)
                                {
                                    *parentMatch6 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
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
                                    soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                    specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                    if (match.hit)
                                    {
                                        s = lexer.GetSpan(pos);
                                        specifierSpan = s;
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
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                            type.reset(static_cast<cmajor::ast::Node*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                        id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                        *parentMatch10 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            cmajor::ast::MemberVariableNode *memberVariableNode = new cmajor::ast::MemberVariableNode(s, specifiers->value, type.release(), id.release(), attrs.release());
            memberVariableNode->SetSpecifierSpan(specifierSpan);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberVariable");
                #endif
                return soul::parser::Match(true, memberVariableNode);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberVariable");
        else soul::lexer::WriteFailureToLog(lexer, "MemberVariable");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ClassParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::classes::parser
