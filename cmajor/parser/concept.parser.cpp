
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/concept.parser' using soul parser generator spg version 5.0.0

module cmajor.concepts.parser;

import util;
import soul.ast.spg;
import cmajor.token;
import soul.lexer;
import cmajor.lexer;
import soul.ast.source.pos;
import cmajor.specifier.parser;
import cmajor.identifier.parser;
import cmajor.parameter.parser;
import cmajor.expression.parser;
import cmajor.type.expr.parser;
import cmajor.function.parser;

using namespace cmajor::token;
using namespace soul::lexer;
using namespace cmajor::lexer;
using namespace soul::ast::source::pos;
using namespace cmajor::specifier::parser;
using namespace cmajor::identifier::parser;
using namespace cmajor::parameter::parser;
using namespace cmajor::expression::parser;
using namespace cmajor::type::expr::parser;
using namespace cmajor::function::parser;

namespace cmajor::concepts::parser {

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::Concept(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Concept");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625665);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConceptNode> conceptNode = std::unique_ptr<cmajor::ast::ConceptNode>();
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraintNode = std::unique_ptr<cmajor::ast::WhereConstraintNode>();
    std::unique_ptr<soul::parser::Value<cmajor::ast::Specifiers>> specifiers;
    std::unique_ptr<cmajor::ast::IdentifierNode> conceptName;
    std::unique_ptr<cmajor::ast::IdentifierNode> typeParam;
    std::unique_ptr<cmajor::ast::ConceptIdNode> refinement;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> whereConstraint;
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
                context->BeginParsingConcept();
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
                                soul::parser::Match match(false);
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
                                                                soul::parser::Match match = SpecifierParser<LexerT>::Specifiers(lexer);
                                                                specifiers.reset(static_cast<soul::parser::Value<cmajor::ast::Specifiers>*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    s = lexer.GetSpan(pos);
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
                                                                if (*lexer == CONCEPT)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                *parentMatch16 = match;
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
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch18 = &match;
                                                            {
                                                                int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                                conceptName.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    conceptNode.reset(new cmajor::ast::ConceptNode(s, specifiers->value, conceptName.release()));
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch13 = match;
                                                    }
                                                    *parentMatch12 = match;
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
                                                            if (*lexer == LANGLE)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                conceptNode->SetBeginBraceSpan(lexer.GetSpan(pos));
                                                            }
                                                            *parentMatch20 = match;
                                                        }
                                                        *parentMatch19 = match;
                                                    }
                                                    *parentMatch12 = match;
                                                }
                                                *parentMatch11 = match;
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
                                                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                                typeParam.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    conceptNode->AddTypeParameter(typeParam.release());
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
                                                                                        soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                                                        typeParam.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                                                        if (match.hit)
                                                                                        {
                                                                                            conceptNode->AddTypeParameter(typeParam.release());
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
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch30 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch31 = &match;
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
                                                        conceptNode->SetEndBraceSpan(lexer.GetSpan(pos));
                                                    }
                                                    *parentMatch31 = match;
                                                }
                                                *parentMatch30 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch32 = &match;
                                        {
                                            soul::parser::Match match(true);
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match* parentMatch33 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch34 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch35 = &match;
                                                    {
                                                        int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = ConceptParser<LexerT>::Refinement(lexer, context);
                                                        refinement.reset(static_cast<cmajor::ast::ConceptIdNode*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            conceptNode->SetRefinement(refinement.release());
                                                        }
                                                        *parentMatch35 = match;
                                                    }
                                                    *parentMatch34 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch33 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                }
                                            }
                                            *parentMatch32 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch36 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch37 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch38 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch39 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                                                    whereConstraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        constraintNode.reset(whereConstraint.release());
                                                        constraintNode->SetHeaderConstraint();
                                                        conceptNode->AddConstraint(constraintNode.release());
                                                    }
                                                    *parentMatch39 = match;
                                                }
                                                *parentMatch38 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch37 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
                                        }
                                        *parentMatch36 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch40 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == LBRACE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch40 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch41 = &match;
                            {
                                soul::parser::Match match = ConceptParser<LexerT>::ConceptBody(lexer, context, conceptNode.get());
                                *parentMatch41 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch42 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == RBRACE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch42 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    context->EndParsingConcept();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Concept");
                        #endif
                        return soul::parser::Match(true, conceptNode.release());
                    }
                }
                else
                {
                    context->EndParsingConcept();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Concept");
        else soul::lexer::WriteFailureToLog(lexer, "Concept");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::Refinement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Refinement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625666);
    std::unique_ptr<cmajor::ast::ConceptIdNode> conceptIdNode = std::unique_ptr<cmajor::ast::ConceptIdNode>();
    std::unique_ptr<cmajor::ast::IdentifierNode> refinedConcept;
    std::unique_ptr<cmajor::ast::IdentifierNode> typeParam;
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
                            if (*lexer == COLON)
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
                                    soul::parser::Match match = IdentifierParser<LexerT>::QualifiedId(lexer, context);
                                    refinedConcept.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                    if (match.hit)
                                    {
                                        conceptIdNode.reset(new cmajor::ast::ConceptIdNode(lexer.GetSpan(pos), refinedConcept.release()));
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == LANGLE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
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
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch12 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                    typeParam.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                    if (match.hit)
                                    {
                                        conceptIdNode->AddTypeParameter(typeParam.release());
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
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        while (true)
                                        {
                                            int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch15 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == COMMA)
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
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                                            typeParam.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                conceptIdNode->AddTypeParameter(typeParam.release());
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch14 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch9 = match;
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
                    soul::parser::Match match(false);
                    if (*lexer == RANGLE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch18 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Refinement");
                #endif
                return soul::parser::Match(true, conceptIdNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Refinement");
        else soul::lexer::WriteFailureToLog(lexer, "Refinement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConceptBody(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConceptBody");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625667);
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
                        int64_t save = lexer.GetPos();
                        soul::parser::Match match = ConceptParser<LexerT>::ConceptBodyConstraint(lexer, context, conceptNode);
                        *parentMatch2 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch3 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match = ConceptParser<LexerT>::Axiom(lexer, context, conceptNode);
                                *parentMatch3 = match;
                            }
                            *parentMatch2 = match;
                        }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConceptBody");
        else soul::lexer::WriteFailureToLog(lexer, "ConceptBody");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConceptBodyConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConceptBodyConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625668);
    std::unique_ptr<cmajor::ast::ConstraintNode> typeNameConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> signatureConstraint;
    std::unique_ptr<cmajor::ast::WhereConstraintNode> embeddedConstraint;
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
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = ConceptParser<LexerT>::TypeNameConstraint(lexer, context);
                typeNameConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                if (match.hit)
                {
                    conceptNode->AddConstraint(typeNameConstraint.release());
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
                        soul::parser::Match match = ConceptParser<LexerT>::SignatureConstraint(lexer, context, conceptNode->TypeParameters()[0]);
                        signatureConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                        if (match.hit)
                        {
                            conceptNode->AddConstraint(signatureConstraint.release());
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = ConceptParser<LexerT>::EmbeddedConstraint(lexer, context);
                    embeddedConstraint.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        conceptNode->AddConstraint(embeddedConstraint.release());
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConceptBodyConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "ConceptBodyConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::TypeNameConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeNameConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625669);
    soul::ast::Span s = soul::ast::Span();
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
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                        type.reset(static_cast<cmajor::ast::Node*>(match.value));
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeNameConstraint");
                #endif
                return soul::parser::Match(true, new cmajor::ast::TypeNameConstraintNode(s, type.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeNameConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "TypeNameConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::SignatureConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SignatureConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625670);
    std::unique_ptr<cmajor::ast::ConstraintNode> constructorConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> destructorConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> memberFunctionConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> functionConstraint;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ConceptParser<LexerT>::ConstructorConstraint(lexer, context, firstTypeParameter);
                    constructorConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SignatureConstraint");
                            #endif
                            return soul::parser::Match(true, constructorConstraint.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ConceptParser<LexerT>::DestructorConstraint(lexer, context, firstTypeParameter);
                            destructorConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SignatureConstraint");
                                    #endif
                                    return soul::parser::Match(true, destructorConstraint.release());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = ConceptParser<LexerT>::MemberFunctionConstraint(lexer, context);
                        memberFunctionConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SignatureConstraint");
                                #endif
                                return soul::parser::Match(true, memberFunctionConstraint.release());
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ConceptParser<LexerT>::FunctionConstraint(lexer, context);
                    functionConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SignatureConstraint");
                            #endif
                            return soul::parser::Match(true, functionConstraint.release());
                        }
                    }
                    *parentMatch9 = match;
                }
                *parentMatch8 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SignatureConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "SignatureConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConstructorConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructorConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625671);
    std::unique_ptr<cmajor::ast::ConstraintNode> ctorConstraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
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
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == EXPLICIT)
                            {
                                ++lexer;
                                match.hit = true;
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
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                bool pass = true;
                                soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                if (match.hit)
                                {
                                    std::u32string className = lexer.GetToken(pos).ToString();
                                    pass = className == firstTypeParameter->Str();
                                    if (pass)
                                    {
                                        ctorConstraintNode.reset(new cmajor::ast::ConstructorConstraintNode(lexer.GetSpan(pos), id.release()));
                                    }
                                }
                                if (match.hit && !pass)
                                {
                                    match = soul::parser::Match(false);
                                }
                                *parentMatch7 = match;
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
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, ctorConstraintNode.get());
                        *parentMatch8 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorConstraint");
                #endif
                return soul::parser::Match(true, ctorConstraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructorConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::DestructorConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DestructorConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625672);
    soul::ast::Span s = soul::ast::Span();
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
                                soul::parser::Match match(false);
                                if (*lexer == CPL)
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
                                    bool pass = true;
                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                    id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                                    if (match.hit)
                                    {
                                        std::u32string className = lexer.GetToken(pos).ToString();
                                        pass = className == firstTypeParameter->Str();
                                    }
                                    if (match.hit && !pass)
                                    {
                                        match = soul::parser::Match(false);
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
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
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
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestructorConstraint");
                #endif
                return soul::parser::Match(true, new cmajor::ast::DestructorConstraintNode(s, id.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestructorConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "DestructorConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::MemberFunctionConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MemberFunctionConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625673);
    std::unique_ptr<cmajor::ast::ConstraintNode> memberFunctionConstraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> returnType;
    std::unique_ptr<cmajor::ast::IdentifierNode> typeParam;
    std::unique_ptr<soul::parser::Value<std::u32string>> functionGroupId;
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
                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                    returnType.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                                    soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                    typeParam.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                                soul::parser::Match match(false);
                                if (*lexer == DOT)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
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
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = FunctionParser<LexerT>::FunctionGroupId(lexer, context);
                                functionGroupId.reset(static_cast<soul::parser::Value<std::u32string>*>(match.value));
                                if (match.hit)
                                {
                                    memberFunctionConstraintNode.reset(new cmajor::ast::MemberFunctionConstraintNode(s, returnType.release(), typeParam.release(), functionGroupId->value));
                                }
                                *parentMatch11 = match;
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
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, memberFunctionConstraintNode.get());
                        *parentMatch12 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch13 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberFunctionConstraint");
                #endif
                return soul::parser::Match(true, memberFunctionConstraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MemberFunctionConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "MemberFunctionConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::FunctionConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625674);
    std::unique_ptr<cmajor::ast::ConstraintNode> functionConstraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> returnType;
    std::unique_ptr<soul::parser::Value<std::u32string>> functionGroupId;
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
                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                            returnType.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = FunctionParser<LexerT>::FunctionGroupId(lexer, context);
                                functionGroupId.reset(static_cast<soul::parser::Value<std::u32string>*>(match.value));
                                if (match.hit)
                                {
                                    functionConstraintNode.reset(new cmajor::ast::FunctionConstraintNode(s, returnType.release(), functionGroupId->value));
                                }
                                *parentMatch7 = match;
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
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, functionConstraintNode.get());
                        *parentMatch8 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionConstraint");
                #endif
                return soul::parser::Match(true, functionConstraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::EmbeddedConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EmbeddedConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625675);
    std::unique_ptr<cmajor::ast::WhereConstraintNode> constraint = std::unique_ptr<cmajor::ast::WhereConstraintNode>();
    std::unique_ptr<cmajor::ast::WhereConstraintNode> c;
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
                    soul::parser::Match match = ConceptParser<LexerT>::WhereConstraint(lexer, context);
                    c.reset(static_cast<cmajor::ast::WhereConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        constraint.reset(c.release());
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
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            constraint->SetSemicolon();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EmbeddedConstraint");
                #endif
                return soul::parser::Match(true, constraint.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EmbeddedConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "EmbeddedConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::WhereConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "WhereConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625676);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConstraintNode> constraint;
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
                    soul::parser::Match match(false);
                    if (*lexer == WHERE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
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
                    soul::parser::Match match = ConceptParser<LexerT>::ConstraintExpr(lexer, context);
                    constraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhereConstraint");
                #endif
                return soul::parser::Match(true, new cmajor::ast::WhereConstraintNode(s, constraint.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhereConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "WhereConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstraintExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625677);
    std::unique_ptr<cmajor::ast::ConstraintNode> disjunctiveConstraint;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match = ConceptParser<LexerT>::DisjunctiveConstraintExpr(lexer, context);
            disjunctiveConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstraintExpr");
                #endif
                return soul::parser::Match(true, disjunctiveConstraint.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstraintExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ConstraintExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::DisjunctiveConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DisjunctiveConstraintExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625678);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConstraintNode> constraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
    std::unique_ptr<cmajor::ast::ConstraintNode> left;
    std::unique_ptr<cmajor::ast::ConstraintNode> right;
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
                    soul::parser::Match match = ConceptParser<LexerT>::ConjunctiveConstraintExpr(lexer, context);
                    left.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        constraintNode.reset(left.release());
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
                                        if (*lexer == OR)
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
                                                soul::parser::Match match = ConceptParser<LexerT>::ConjunctiveConstraintExpr(lexer, context);
                                                right.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    constraintNode.reset(new cmajor::ast::DisjunctiveConstraintNode(s, constraintNode.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DisjunctiveConstraintExpr");
                #endif
                return soul::parser::Match(true, constraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DisjunctiveConstraintExpr");
        else soul::lexer::WriteFailureToLog(lexer, "DisjunctiveConstraintExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConjunctiveConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConjunctiveConstraintExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625679);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConstraintNode> constraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
    std::unique_ptr<cmajor::ast::ConstraintNode> left;
    std::unique_ptr<cmajor::ast::ConstraintNode> right;
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
                    soul::parser::Match match = ConceptParser<LexerT>::PrimaryConstraintExpr(lexer, context);
                    left.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        constraintNode.reset(left.release());
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
                                        if (*lexer == AND)
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
                                                soul::parser::Match match = ConceptParser<LexerT>::PrimaryConstraintExpr(lexer, context);
                                                right.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    constraintNode.reset(new cmajor::ast::ConjunctiveConstraintNode(s, constraintNode.release(), right.release()));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConjunctiveConstraintExpr");
                #endif
                return soul::parser::Match(true, constraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConjunctiveConstraintExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ConjunctiveConstraintExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::PrimaryConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PrimaryConstraintExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625680);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::ConstraintNode> constraintNode = std::unique_ptr<cmajor::ast::ConstraintNode>();
    std::unique_ptr<cmajor::ast::ConstraintNode> constraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> atomicConstraint;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match = ConceptParser<LexerT>::ConstraintExpr(lexer, context);
                        constraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryConstraintExpr");
                                #endif
                                return soul::parser::Match(true, new cmajor::ast::ParenthesizedConstraintNode(s, constraint.release()));
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = ConceptParser<LexerT>::AtomicConstraintExpr(lexer, context);
                        atomicConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryConstraintExpr");
                                #endif
                                return soul::parser::Match(true, atomicConstraint.release());
                            }
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryConstraintExpr");
        else soul::lexer::WriteFailureToLog(lexer, "PrimaryConstraintExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::AtomicConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AtomicConstraintExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625681);
    std::unique_ptr<cmajor::ast::ConstraintNode> predicateConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> isConstraint;
    std::unique_ptr<cmajor::ast::ConstraintNode> multiParamConstraint;
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
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ConceptParser<LexerT>::PredicateConstraint(lexer, context);
                    predicateConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AtomicConstraintExpr");
                            #endif
                            return soul::parser::Match(true, predicateConstraint.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = ConceptParser<LexerT>::IsConstraint(lexer, context);
                            isConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AtomicConstraintExpr");
                                    #endif
                                    return soul::parser::Match(true, isConstraint.release());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = ConceptParser<LexerT>::MultiParamConstraint(lexer, context);
                        multiParamConstraint.reset(static_cast<cmajor::ast::ConstraintNode*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AtomicConstraintExpr");
                                #endif
                                return soul::parser::Match(true, multiParamConstraint.release());
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AtomicConstraintExpr");
        else soul::lexer::WriteFailureToLog(lexer, "AtomicConstraintExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::PredicateConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PredicateConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625682);
    std::unique_ptr<cmajor::ast::Node> invokeExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = ExpressionParser<LexerT>::InvokeExpr(lexer, context);
            invokeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PredicateConstraint");
                    #endif
                    return soul::parser::Match(true, new cmajor::ast::PredicateConstraintNode(lexer.GetSpan(pos), invokeExpr.release()));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PredicateConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "PredicateConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::IsConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IsConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625683);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::Node> type;
    std::unique_ptr<cmajor::ast::Node> conceptOrTypeName;
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
                        soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                        type.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                        if (*lexer == IS)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match = ConceptParser<LexerT>::ConceptOrTypeName(lexer, context);
                    conceptOrTypeName.reset(static_cast<cmajor::ast::Node*>(match.value));
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IsConstraint");
                #endif
                return soul::parser::Match(true, new cmajor::ast::IsConstraintNode(s, type.release(), conceptOrTypeName.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IsConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "IsConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::ConceptOrTypeName(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConceptOrTypeName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625684);
    std::unique_ptr<cmajor::ast::Node> conceptOrTypeName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
            conceptOrTypeName.reset(static_cast<cmajor::ast::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConceptOrTypeName");
                    #endif
                    return soul::parser::Match(true, conceptOrTypeName.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConceptOrTypeName");
        else soul::lexer::WriteFailureToLog(lexer, "ConceptOrTypeName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::MultiParamConstraint(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MultiParamConstraint");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625685);
    std::unique_ptr<cmajor::ast::MultiParamConstraintNode> constraintNode = std::unique_ptr<cmajor::ast::MultiParamConstraintNode>();
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
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = IdentifierParser<LexerT>::QualifiedId(lexer, context);
                            id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
                            if (match.hit)
                            {
                                constraintNode.reset(new cmajor::ast::MultiParamConstraintNode(lexer.GetSpan(pos), id.release()));
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
                            if (*lexer == LANGLE)
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                    typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        constraintNode->AddTypeExpr(typeExpr.release());
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
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
                                                    if (*lexer == COMMA)
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
                                                            soul::parser::Match match = TypeExprParser<LexerT>::TypeExpr(lexer, context);
                                                            typeExpr.reset(static_cast<cmajor::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                constraintNode->AddTypeExpr(typeExpr.release());
                                                            }
                                                            *parentMatch15 = match;
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
                                *parentMatch9 = match;
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
                soul::parser::Match* parentMatch16 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RANGLE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch16 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiParamConstraint");
                #endif
                return soul::parser::Match(true, constraintNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MultiParamConstraint");
        else soul::lexer::WriteFailureToLog(lexer, "MultiParamConstraint");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::Axiom(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Axiom");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625686);
    soul::ast::Span s = soul::ast::Span();
    std::unique_ptr<cmajor::ast::AxiomNode> axiomNode = std::unique_ptr<cmajor::ast::AxiomNode>();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == AXIOM)
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
                                        soul::parser::Match match(true);
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match = IdentifierParser<LexerT>::Identifier(lexer, context);
                                            id.reset(static_cast<cmajor::ast::IdentifierNode*>(match.value));
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
                                        soul::parser::Match match(true);
                                        if (match.hit)
                                        {
                                            axiomNode.reset(new cmajor::ast::AxiomNode(s, id.release()));
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
                                soul::parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, axiomNode.get());
                                    if (match.hit)
                                    {
                                        *parentMatch14 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                if (*lexer == LBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    axiomNode->SetBeginBraceSpan(lexer.GetSpan(pos));
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch15 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch17 = &match;
                    {
                        soul::parser::Match match = ConceptParser<LexerT>::AxiomBody(lexer, context, axiomNode.get());
                        *parentMatch17 = match;
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch19 = &match;
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
                            axiomNode->SetEndBraceSpan(lexer.GetSpan(pos));
                        }
                        *parentMatch19 = match;
                    }
                    *parentMatch18 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            conceptNode->AddAxiom(axiomNode.release());
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Axiom");
        else soul::lexer::WriteFailureToLog(lexer, "Axiom");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::AxiomBody(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::AxiomNode* axiomNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AxiomBody");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625687);
    std::unique_ptr<cmajor::ast::AxiomStatementNode> axiomStatement;
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
                        soul::parser::Match match = ConceptParser<LexerT>::AxiomStatement(lexer, context);
                        axiomStatement.reset(static_cast<cmajor::ast::AxiomStatementNode*>(match.value));
                        if (match.hit)
                        {
                            axiomNode->AddStatement(axiomStatement.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxiomBody");
        else soul::lexer::WriteFailureToLog(lexer, "AxiomBody");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ConceptParser<LexerT>::AxiomStatement(LexerT& lexer, cmajor::parser::context::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AxiomStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2278901632848625688);
    soul::ast::Span s = soul::ast::Span();
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                    expr.reset(static_cast<cmajor::ast::Node*>(match.value));
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
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxiomStatement");
                #endif
                return soul::parser::Match(true, new cmajor::ast::AxiomStatementNode(s, expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AxiomStatement");
        else soul::lexer::WriteFailureToLog(lexer, "AxiomStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ConceptParser<soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>>;

} // namespace cmajor::concepts::parser
