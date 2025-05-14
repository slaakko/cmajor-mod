
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/intermediate/intermediate_code.parser' using soul parser generator spg version 5.0.0

module cmajor.systemx.intermediate.code.parser;

import util;
import soul.ast.spg;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.code.token;
import cmajor.systemx.intermediate.code.lexer;

using namespace cmajor::systemx::intermediate::error;
using namespace cmajor::systemx::intermediate::code::token;
using namespace cmajor::systemx::intermediate::code::lexer;

namespace cmajor::systemx::intermediate::code::parser {

template<typename LexerT>
void IntermediateCodeParser<LexerT>::Parse(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = IntermediateCodeParser<LexerT>::IntermediateCodeFile(lexer, context);
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
            return;
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
    return;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::IntermediateCodeFile(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IntermediateCodeFile");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469441);
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
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                context->SetFilePath(lexer.FileName());
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::CompileUnitHeader(lexer, context);
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
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeDeclarations(lexer, context);
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::DataDefinitions(lexer, context);
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                soul::parser::Match match(true);
                int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Functions(lexer, context);
                    if (match.hit)
                    {
                        *parentMatch12 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch11 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Metadata(lexer, context);
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
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntermediateCodeFile");
        else soul::lexer::WriteFailureToLog(lexer, "IntermediateCodeFile");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::CompileUnitHeader(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompileUnitHeader");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469442);
    std::unique_ptr<soul::parser::Value<std::string>> cuid;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdref;
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
                        if (*lexer == CU)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
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
                    soul::parser::Match* parentMatch6 = &match;
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::CompileUnitId(lexer);
                        cuid.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                        *parentMatch6 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == COMMA)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                mdref.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
                *parentMatch8 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match(false);
                if (*lexer == RPAREN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->SetCompileUnitInfo(cuid->value, mdref.release());
                }
                *parentMatch10 = match;
            }
            *parentMatch9 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompileUnitHeader");
        else soul::lexer::WriteFailureToLog(lexer, "CompileUnitHeader");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::CompileUnitId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompileUnitId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469443);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case HEXNUM:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == HEXNUM)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompileUnitId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::string>(util::ToUtf8(lexer.GetToken(pos).ToString())));
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
        case NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompileUnitId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::string>(util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompileUnitId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::string>(util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch3 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompileUnitId");
        else soul::lexer::WriteFailureToLog(lexer, "CompileUnitId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::TypeDeclarations(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclarations");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469444);
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
                if (*lexer == TYPES)
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
                    if (*lexer == LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
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
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeDeclaration(lexer, context);
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                if (*lexer == RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->ResolveTypes();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclarations");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclarations");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::TypeDeclaration(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469445);
    int32_t tid = int32_t();
    std::unique_ptr<soul::parser::Value<int32_t>> typeId;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeId(lexer);
                    typeId.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                    if (match.hit)
                    {
                        tid = typeId->value;
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
                    if (*lexer == ASSIGN)
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == TYPE)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                    case LBRACE:
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::StructureType(lexer, context, tid);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        break;
                    }
                    case LBRACKET:
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ArrayType(lexer, context, tid);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        break;
                    }
                    case FUNCTION:
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::FunctionType(lexer, context, tid);
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
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::TypeId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469446);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == TYPEID)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeId");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::MakeUserTypeId(util::ToUtf8(lexer.GetToken(pos).ToString()))));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeId");
        else soul::lexer::WriteFailureToLog(lexer, "TypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FundamentalTypeId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FundamentalTypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469447);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case VOID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == VOID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::voidTypeId));
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
        case BOOL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BOOL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::boolTypeId));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SBYTE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SBYTE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::sbyteTypeId));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BYTE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BYTE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::byteTypeId));
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
        case SHORT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SHORT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::shortTypeId));
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case USHORT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == USHORT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::ushortTypeId));
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case INT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == INT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::intTypeId));
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case UINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == UINT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::uintTypeId));
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case LONG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == LONG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::longTypeId));
                    }
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ULONG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ULONG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::ulongTypeId));
                    }
                }
                *parentMatch10 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case FLOAT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == FLOAT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::floatTypeId));
                    }
                }
                *parentMatch11 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DOUBLE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DOUBLE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(cmajor::systemx::intermediate::doubleTypeId));
                    }
                }
                *parentMatch12 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
        else soul::lexer::WriteFailureToLog(lexer, "FundamentalTypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::TypeRef(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeRef");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469448);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    int32_t baseTypeId = int32_t();
    int8_t pointerCount = int8_t();
    std::unique_ptr<soul::parser::Value<int32_t>> typeId;
    std::unique_ptr<soul::parser::Value<int32_t>> fundamentalTypeId;
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
                    switch (*lexer)
                    {
                        case TYPEID:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeId(lexer);
                                typeId.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                if (match.hit)
                                {
                                    sp = lexer.GetSourcePos(pos);
                                    baseTypeId = typeId->value;
                                }
                                *parentMatch5 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch4 = match;
                            }
                            break;
                        }
                        case BOOL:
                        case BYTE:
                        case DOUBLE:
                        case FLOAT:
                        case INT:
                        case LONG:
                        case SBYTE:
                        case SHORT:
                        case UINT:
                        case ULONG:
                        case USHORT:
                        case VOID:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::FundamentalTypeId(lexer);
                                fundamentalTypeId.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                if (match.hit)
                                {
                                    sp = lexer.GetSourcePos(pos);
                                    baseTypeId = fundamentalTypeId->value;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch4 = match;
                            }
                            break;
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == AST)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            ++pointerCount;
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeRef");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<cmajor::systemx::intermediate::TypeRef>(cmajor::systemx::intermediate::MakeTypeRef(sp, baseTypeId, pointerCount)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeRef");
        else soul::lexer::WriteFailureToLog(lexer, "TypeRef");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StructureType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StructureType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469449);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::vector<cmajor::systemx::intermediate::TypeRef> fieldTypeRefs = std::vector<cmajor::systemx::intermediate::TypeRef>();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> first;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> next;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdref;
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
                            if (*lexer == LBRACE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                sp = lexer.GetSourcePos(pos);
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
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
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
                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                            first.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                            if (match.hit)
                                            {
                                                fieldTypeRefs.push_back(first->value);
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
                                                            soul::parser::Match* parentMatch14 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                if (*lexer == COMMA)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                *parentMatch14 = match;
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
                                                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                                                        next.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            fieldTypeRefs.push_back(next->value);
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
                                if (match.hit)
                                {
                                    *parentMatch7 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
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
                    soul::parser::Match* parentMatch17 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RBRACE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
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
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch19 = &match;
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                        mdref.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->AddStructureType(sp, typeId, fieldTypeRefs, mdref.release());
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureType");
        else soul::lexer::WriteFailureToLog(lexer, "StructureType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ArrayType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArrayType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469450);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    int64_t size = int64_t();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> elementTypeRef;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdref;
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
                                    soul::parser::Match match(false);
                                    if (*lexer == LBRACKET)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
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
                                        soul::parser::Match match(false);
                                        if (*lexer == NUMBER)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            size = lexer.GetToken(pos).ToLong();
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
                                    bool pass = true;
                                    soul::parser::Match match(false);
                                    if (*lexer == ID)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        pass = util::ToUtf8(lexer.GetToken(pos).ToString()) == "x";
                                    }
                                    if (match.hit && !pass)
                                    {
                                        match = soul::parser::Match(false);
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                            elementTypeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
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
                        soul::parser::Match match(false);
                        if (*lexer == RBRACKET)
                        {
                            ++lexer;
                            match.hit = true;
                        }
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
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch15 = &match;
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                        mdref.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->AddArrayType(sp, typeId, size, elementTypeRef->value, mdref.release());
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayType");
        else soul::lexer::WriteFailureToLog(lexer, "ArrayType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FunctionType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469451);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::vector<cmajor::systemx::intermediate::TypeRef> paramTypeRefs = std::vector<cmajor::systemx::intermediate::TypeRef>();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> returnTypeRef;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> first;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> next;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdref;
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
                                    soul::parser::Match match(false);
                                    if (*lexer == FUNCTION)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                    returnTypeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
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
                                if (*lexer == LPAREN)
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
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
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
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                            first.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                            if (match.hit)
                                            {
                                                paramTypeRefs.push_back(first->value);
                                            }
                                            *parentMatch14 = match;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch15 = &match;
                                        {
                                            soul::parser::Match match(true);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                while (true)
                                                {
                                                    int64_t save = lexer.GetPos();
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch18 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                if (*lexer == COMMA)
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
                                                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                                                        next.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            paramTypeRefs.push_back(next->value);
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
                                                            *parentMatch16 = match;
                                                        }
                                                        else
                                                        {
                                                            lexer.SetPos(save);
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch12 = match;
                                }
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch21 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch21 = match;
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
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch23 = &match;
                    {
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                        mdref.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->AddFunctionType(sp, typeId, returnTypeRef->value, paramTypeRefs, mdref.release());
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionType");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::DataDefinitions(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataDefinitions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469452);
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
                if (*lexer == DATA)
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
                    if (*lexer == LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
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
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::DataDefinition(lexer, context);
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                if (*lexer == RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->GetData().Resolve();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinitions");
        else soul::lexer::WriteFailureToLog(lexer, "DataDefinitions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::DataDefinition(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataDefinition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469453);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    bool once = bool();
    std::string variableName = std::string();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> initializer;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == ONCE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    once = true;
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
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
                        sp = lexer.GetSourcePos(pos);
                        variableName = util::ToUtf8(lexer.GetToken(pos).ToString());
                    }
                    *parentMatch9 = match;
                }
                *parentMatch8 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch10 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch12 = &match;
                switch (*lexer)
                {
                    case SEMICOLON:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch13 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == SEMICOLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                context->AddGlobalVariable(sp, tref.GetType(), variableName, nullptr, once);
                            }
                            *parentMatch13 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch12 = match;
                        }
                        break;
                    }
                    case ASSIGN:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == ASSIGN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch14 = match;
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                    initializer.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                    if (match.hit)
                                    {
                                        context->AddGlobalVariable(sp, tref.GetType(), variableName, initializer.release(), once);
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch12 = match;
                        }
                        break;
                    }
                }
                *parentMatch11 = match;
            }
            *parentMatch10 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinition");
        else soul::lexer::WriteFailureToLog(lexer, "DataDefinition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Constant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469454);
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> boolConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> sbyteConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> byteConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> shortConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> ushortConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> intConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> uintConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> longConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> ulongConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> floatConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> doubleConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> addressConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> arrayConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> structureConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> stringConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> stringArrayConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> conversionConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> clsidConstant;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> symbolConstant;
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
                switch (*lexer)
                {
                    case BOOL:
                    case BYTE:
                    case DOUBLE:
                    case FLOAT:
                    case INT:
                    case LONG:
                    case SBYTE:
                    case SHORT:
                    case TYPEID:
                    case UINT:
                    case ULONG:
                    case USHORT:
                    case VOID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            switch (*lexer)
                            {
                                case BOOL:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch6 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::BoolConstant(lexer, context);
                                        boolConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, boolConstant.release());
                                            }
                                        }
                                        *parentMatch6 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case SBYTE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::SByteConstant(lexer, context);
                                        sbyteConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, sbyteConstant.release());
                                            }
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case BYTE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ByteConstant(lexer, context);
                                        byteConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, byteConstant.release());
                                            }
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case SHORT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ShortConstant(lexer, context);
                                        shortConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, shortConstant.release());
                                            }
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case USHORT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::UShortConstant(lexer, context);
                                        ushortConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, ushortConstant.release());
                                            }
                                        }
                                        *parentMatch10 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case INT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::IntConstant(lexer, context);
                                        intConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, intConstant.release());
                                            }
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case UINT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::UIntConstant(lexer, context);
                                        uintConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, uintConstant.release());
                                            }
                                        }
                                        *parentMatch12 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case LONG:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::LongConstant(lexer, context);
                                        longConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, longConstant.release());
                                            }
                                        }
                                        *parentMatch13 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case ULONG:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ULongConstant(lexer, context);
                                        ulongConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, ulongConstant.release());
                                            }
                                        }
                                        *parentMatch14 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case FLOAT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::FloatConstant(lexer, context);
                                        floatConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, floatConstant.release());
                                            }
                                        }
                                        *parentMatch15 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case DOUBLE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::DoubleConstant(lexer, context);
                                        doubleConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, doubleConstant.release());
                                            }
                                        }
                                        *parentMatch16 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                            }
                            *parentMatch4 = match;
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
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::AddressConstant(lexer, context);
                                        addressConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, addressConstant.release());
                                            }
                                        }
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch4 = match;
                            }
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case LBRACKET:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch19 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::ArrayConstant(lexer, context);
                            arrayConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, arrayConstant.release());
                                }
                            }
                            *parentMatch19 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case LBRACE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::StructureConstant(lexer, context);
                            structureConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, structureConstant.release());
                                }
                            }
                            *parentMatch20 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case STRING:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch21 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::StringConstant(lexer, context);
                            stringConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, stringConstant.release());
                                }
                            }
                            *parentMatch21 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case ID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch22 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::StringArrayConstant(lexer, context);
                            stringArrayConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, stringArrayConstant.release());
                                }
                            }
                            *parentMatch22 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::ConversionConstant(lexer, context);
                            conversionConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, conversionConstant.release());
                                }
                            }
                            *parentMatch24 = match;
                        }
                        *parentMatch23 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ClsIdConstant(lexer, context);
                        clsidConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                #endif
                                return soul::parser::Match(true, clsidConstant.release());
                            }
                        }
                        *parentMatch26 = match;
                    }
                    *parentMatch25 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::SymbolConstant(lexer, context);
                    symbolConstant.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                            #endif
                            return soul::parser::Match(true, symbolConstant.release());
                        }
                    }
                    *parentMatch28 = match;
                }
                *parentMatch27 = match;
            }
            *parentMatch0 = match;
        }
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

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::BoolConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BoolConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469455);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == BOOL)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case TRUE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == TRUE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetTrueValue());
                                }
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case FALSE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == FALSE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetFalseValue());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
        else soul::lexer::WriteFailureToLog(lexer, "BoolConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::SByteConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SByteConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469456);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == SBYTE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SByteConstant");
                        #endif
                        return soul::parser::Match(true, context->GetSByteValue(lexer.GetToken(pos).ToSByte()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SByteConstant");
        else soul::lexer::WriteFailureToLog(lexer, "SByteConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ByteConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ByteConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469457);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == BYTE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ByteConstant");
                        #endif
                        return soul::parser::Match(true, context->GetByteValue(lexer.GetToken(pos).ToByte()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ByteConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ByteConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ShortConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ShortConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469458);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == SHORT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShortConstant");
                        #endif
                        return soul::parser::Match(true, context->GetShortValue(lexer.GetToken(pos).ToShort()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShortConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ShortConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::UShortConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UShortConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469459);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == USHORT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UShortConstant");
                        #endif
                        return soul::parser::Match(true, context->GetUShortValue(lexer.GetToken(pos).ToUShort()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UShortConstant");
        else soul::lexer::WriteFailureToLog(lexer, "UShortConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::IntConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IntConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469460);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == INT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntConstant");
                        #endif
                        return soul::parser::Match(true, context->GetIntValue(lexer.GetToken(pos).ToInt()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntConstant");
        else soul::lexer::WriteFailureToLog(lexer, "IntConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::UIntConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UIntConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469461);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == UINT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UIntConstant");
                        #endif
                        return soul::parser::Match(true, context->GetUIntValue(lexer.GetToken(pos).ToUInt()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UIntConstant");
        else soul::lexer::WriteFailureToLog(lexer, "UIntConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::LongConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LongConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469462);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == LONG)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LongConstant");
                        #endif
                        return soul::parser::Match(true, context->GetLongValue(lexer.GetToken(pos).ToLong()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LongConstant");
        else soul::lexer::WriteFailureToLog(lexer, "LongConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ULongConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ULongConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469463);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == ULONG)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ULongConstant");
                        #endif
                        return soul::parser::Match(true, context->GetULongValue(lexer.GetToken(pos).ToULong()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ULongConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ULongConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FloatConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FloatConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469464);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == FLOAT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FloatConstant");
                        #endif
                        return soul::parser::Match(true, context->GetFloatValue(lexer.GetToken(pos).ToFloat()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FloatConstant");
        else soul::lexer::WriteFailureToLog(lexer, "FloatConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::DoubleConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DoubleConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469465);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == DOUBLE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoubleConstant");
                        #endif
                        return soul::parser::Match(true, context->GetDoubleValue(lexer.GetToken(pos).ToDouble()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoubleConstant");
        else soul::lexer::WriteFailureToLog(lexer, "DoubleConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::AddressConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AddressConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469466);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                switch (*lexer)
                {
                    case NULL_TOKEN:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == NULL_TOKEN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetNullValue(lexer.GetSourcePos(pos), tref.GetType()));
                                }
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch4 = match;
                        }
                        break;
                    }
                    case ID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
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
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
                                    #endif
                                    return soul::parser::Match(true, context->MakeAddressLiteral(lexer.GetSourcePos(pos), tref.GetType(), util::ToUtf8(lexer.GetToken(pos).ToString()), false));
                                }
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch4 = match;
                        }
                        break;
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
        else soul::lexer::WriteFailureToLog(lexer, "AddressConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ArrayConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArrayConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469467);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::vector<cmajor::systemx::intermediate::ConstantValue*> elements = std::vector<cmajor::systemx::intermediate::ConstantValue*>();
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> first;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> next;
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
                soul::parser::Match match(false);
                if (*lexer == LBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    sp = lexer.GetSourcePos(pos);
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
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                first.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                if (match.hit)
                                {
                                    elements.push_back(first.release());
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
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                                            next.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                elements.push_back(next.release());
                                                            }
                                                            *parentMatch13 = match;
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match(false);
                if (*lexer == RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeArrayValue(sp, elements, std::string()));
                    }
                }
                *parentMatch15 = match;
            }
            *parentMatch14 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ArrayConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StructureConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StructureConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469468);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::vector<cmajor::systemx::intermediate::ConstantValue*> fieldValues = std::vector<cmajor::systemx::intermediate::ConstantValue*>();
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> first;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> next;
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
                soul::parser::Match match(false);
                if (*lexer == LBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    sp = lexer.GetSourcePos(pos);
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
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                first.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                if (match.hit)
                                {
                                    fieldValues.push_back(first.release());
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
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                                            next.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                fieldValues.push_back(next.release());
                                                            }
                                                            *parentMatch13 = match;
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match(false);
                if (*lexer == RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeStructureValue(sp, fieldValues));
                    }
                }
                *parentMatch15 = match;
            }
            *parentMatch14 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StructureConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StringConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469469);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == STRING)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringConstant");
                #endif
                return soul::parser::Match(true, context->MakeStringValue(lexer.GetSourcePos(pos), util::ToUtf8(lexer.GetToken(pos).ToString()), true));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StringConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StringArrayConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringArrayConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469470);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::vector<cmajor::systemx::intermediate::ConstantValue*> strings = std::vector<cmajor::systemx::intermediate::ConstantValue*>();
    std::unique_ptr<soul::parser::Value<char>> prefix;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> first;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> next;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::StringArrayPrefix(lexer);
                    prefix.reset(static_cast<soul::parser::Value<char>*>(match.value));
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                    if (*lexer == LBRACKET)
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(true);
                int64_t save = lexer.GetPos();
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
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                first.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                if (match.hit)
                                {
                                    strings.push_back(first.release());
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
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    while (true)
                                    {
                                        int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch12 = &match;
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
                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                                                            next.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                strings.push_back(next.release());
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch13 = match;
                                                }
                                                *parentMatch12 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch11 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
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
                        *parentMatch6 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                if (*lexer == RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeStringArrayValue(sp, prefix->value, strings));
                    }
                }
                *parentMatch17 = match;
            }
            *parentMatch16 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StringArrayConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StringArrayPrefix(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringArrayPrefix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469471);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == ID)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            std::string p = util::ToUtf8(lexer.GetToken(pos).ToString());
            pass = p == "u" || p == "w" || p == "b";
            if (pass)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayPrefix");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char>(p[0]));
                }
            }
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayPrefix");
        else soul::lexer::WriteFailureToLog(lexer, "StringArrayPrefix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ConversionConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469472);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> from;
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
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                        typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                        if (match.hit)
                        {
                            tref = typeRef->value;
                            context->ResolveType(tref);
                            sp = lexer.GetSourcePos(pos);
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
                        if (*lexer == CONV)
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
                    soul::parser::Match match(false);
                    if (*lexer == LPAREN)
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Constant(lexer, context);
                from.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                *parentMatch7 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeConversionValue(sp, tref.GetType(), from.release()));
                    }
                }
                *parentMatch9 = match;
            }
            *parentMatch8 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ClsIdConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ClsIdConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469473);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
                sp = lexer.GetSourcePos(pos);
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
                if (*lexer == CLSID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClsIdConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeClsIdValue(sp, tref.GetType(), util::ToUtf8(lexer.GetToken(pos).ToString()), true));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClsIdConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ClsIdConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::SymbolConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SymbolConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469474);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> symbolValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::SymbolValue(lexer, context, tref.GetType());
                symbolValue.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolConstant");
                        #endif
                        return soul::parser::Match(true, symbolValue.release());
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolConstant");
        else soul::lexer::WriteFailureToLog(lexer, "SymbolConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Value(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Value");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469475);
    std::unique_ptr<cmajor::systemx::intermediate::Value> regValue;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> symbolValue;
    std::unique_ptr<cmajor::systemx::intermediate::ConstantValue> literalValue;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::RegValue(lexer, context, type);
                regValue.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, regValue.release());
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
        case AT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::SymbolValue(lexer, context, type);
                symbolValue.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, symbolValue.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case FALSE:
        case ID:
        case NULL_TOKEN:
        case NUMBER:
        case TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::LiteralValue(lexer, context, type);
                literalValue.reset(static_cast<cmajor::systemx::intermediate::ConstantValue*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, literalValue.release());
                    }
                }
                *parentMatch3 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
        else soul::lexer::WriteFailureToLog(lexer, "Value");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::RegValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RegValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469476);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == DOLLAR)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    int32_t reg = lexer.GetToken(pos).ToInt();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegValue");
                        #endif
                        return soul::parser::Match(true, context->CurrentFunction()->GetRegRef(sp, type, reg, context));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegValue");
        else soul::lexer::WriteFailureToLog(lexer, "RegValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ResultRegValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ResultRegValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469477);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == DOLLAR)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    int32_t reg = lexer.GetToken(pos).ToInt();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ResultRegValue");
                        #endif
                        return soul::parser::Match(true, context->CurrentFunction()->MakeRegValue(sp, type, reg, context));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ResultRegValue");
        else soul::lexer::WriteFailureToLog(lexer, "ResultRegValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::SymbolValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SymbolValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469478);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == AT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
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
                    sp = lexer.GetSourcePos(pos);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolValue");
                        #endif
                        return soul::parser::Match(true, context->MakeSymbolValue(sp, type, util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolValue");
        else soul::lexer::WriteFailureToLog(lexer, "SymbolValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::LiteralValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LiteralValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469479);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == TRUE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetBooleanLiteral(lexer.GetSourcePos(pos), type, true));
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
        case FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == FALSE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetBooleanLiteral(lexer.GetSourcePos(pos), type, false));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NULL_TOKEN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NULL_TOKEN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetNullValue(lexer.GetSourcePos(pos), type));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->MakeNumericLiteral(lexer.GetSourcePos(pos), type, util::ToUtf8(lexer.GetToken(pos).ToString())));
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
        case ID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->MakeAddressLiteral(lexer.GetSourcePos(pos), type, util::ToUtf8(lexer.GetToken(pos).ToString()), true));
                    }
                }
                *parentMatch5 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
        else soul::lexer::WriteFailureToLog(lexer, "LiteralValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Functions(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Functions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469480);
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::FunctionDeclaration(lexer, context);
                        *parentMatch2 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch3 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::FunctionDefinition(lexer, context);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Functions");
        else soul::lexer::WriteFailureToLog(lexer, "Functions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FunctionDeclaration(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469481);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef functionTypeRef = cmajor::systemx::intermediate::TypeRef();
    std::string functionId = std::string();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
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
                            if (*lexer == EXTERN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                sp = lexer.GetSourcePos(pos);
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
                            if (*lexer == FUNCTION)
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                            typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                            if (match.hit)
                            {
                                functionTypeRef = typeRef->value;
                                context->ResolveType(functionTypeRef);
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
                    soul::parser::Match* parentMatch10 = &match;
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
                            functionId = util::ToUtf8(lexer.GetToken(pos).ToString());
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->AddFunctionDeclaration(sp, functionTypeRef.GetType(), functionId);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FunctionDefinition(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDefinition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469482);
    cmajor::systemx::intermediate::Function* function = nullptr;
    std::unique_ptr<cmajor::systemx::intermediate::Function> header;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::FunctionHeader(lexer, context);
                    header.reset(static_cast<cmajor::systemx::intermediate::Function*>(match.value));
                    if (match.hit)
                    {
                        function = header.release();
                        context->SetCurrentFunction(function);
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
                    if (*lexer == LBRACE)
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch6 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::BasicBlock(lexer, context, function);
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch7 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == RBRACE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch7 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDefinition");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDefinition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FunctionHeader(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionHeader");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469483);
    cmajor::systemx::intermediate::TypeRef functionTypeRef = cmajor::systemx::intermediate::TypeRef();
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    bool once = bool();
    bool main_ = bool();
    bool inline_ = bool();
    std::string functionId = std::string();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdRef;
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == ONCE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    once = true;
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
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch14 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    if (*lexer == INLINE)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        inline_ = true;
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
                                            }
                                        }
                                        *parentMatch11 = match;
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
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch17 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == MAIN)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    main_ = true;
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch17 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch16 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
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
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == FUNCTION)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch20 = match;
                                }
                                *parentMatch19 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch21 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch22 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    functionTypeRef = typeRef->value;
                                    context->ResolveType(functionTypeRef);
                                }
                                *parentMatch22 = match;
                            }
                            *parentMatch21 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
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
                            if (*lexer == ID)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                functionId = util::ToUtf8(lexer.GetToken(pos).ToString());
                            }
                            *parentMatch24 = match;
                        }
                        *parentMatch23 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                        mdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeader");
                #endif
                return soul::parser::Match(true, context->AddFunctionDefinition(sp, functionTypeRef.GetType(), functionId, once, main_, inline_, mdRef.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeader");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionHeader");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::BasicBlock(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Function* function)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BasicBlock");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469484);
    cmajor::systemx::intermediate::BasicBlock* basicBlock = nullptr;
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<int32_t>> id;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
            id.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
                basicBlock = function->AddBasicBlock(sp, id->value, context);
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
            soul::parser::Match match = IntermediateCodeParser<LexerT>::Instructions(lexer, context, basicBlock);
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BasicBlock");
        else soul::lexer::WriteFailureToLog(lexer, "BasicBlock");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Label(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Label");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469485);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == AT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<int32_t>(lexer.GetToken(pos).ToInt()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
        else soul::lexer::WriteFailureToLog(lexer, "Label");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Operand(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Operand");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469486);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> value;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                value.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operand");
                        #endif
                        return soul::parser::Match(true, value.release());
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operand");
        else soul::lexer::WriteFailureToLog(lexer, "Operand");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Instructions(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::BasicBlock* basicBlock)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Instructions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469487);
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> instruction;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdRef;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> secondaryMdRef;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Instruction(lexer, context);
                            instruction.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                                    mdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
                                    if (match.hit)
                                    {
                                        *parentMatch7 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
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
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                                secondaryMdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    cmajor::systemx::intermediate::Instruction *inst = instruction.release();
                    basicBlock->AddInstruction(inst, mdRef.release());
                    cmajor::systemx::intermediate::MetadataRef *secMdRef = secondaryMdRef.release();
                    if (secMdRef)
                    {
                        inst->SetSecondaryMdRef(secMdRef);
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch10 = &match;
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch13 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch14 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Instruction(lexer, context);
                                    instruction.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                                    *parentMatch15 = match;
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
                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                                            mdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                                    *parentMatch15 = match;
                                }
                                *parentMatch14 = match;
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
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                                        secondaryMdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
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
                                *parentMatch14 = match;
                            }
                            *parentMatch13 = match;
                        }
                        if (match.hit)
                        {
                            cmajor::systemx::intermediate::Instruction *inst = instruction.release();
                            basicBlock->AddInstruction(inst, mdRef.release());
                            cmajor::systemx::intermediate::MetadataRef *secMdRef = secondaryMdRef.release();
                            if (secMdRef)
                            {
                                inst->SetSecondaryMdRef(secMdRef);
                            }
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
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instructions");
        else soul::lexer::WriteFailureToLog(lexer, "Instructions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Instruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Instruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469488);
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> store;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> arg;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> jmp;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> branch;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> procedureCall;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> ret;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> switch_;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> valueInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> nopInst;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case STORE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::StoreInstruction(lexer, context);
                store.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, store.release());
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
        case ARG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::ArgInstruction(lexer, context);
                arg.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, arg.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case JMP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::JmpInstruction(lexer, context);
                jmp.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, jmp.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BRANCH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::BranchInstruction(lexer, context);
                branch.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, branch.release());
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
        case CALL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::ProcedureCallInstruction(lexer, context);
                procedureCall.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, procedureCall.release());
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::RetInstruction(lexer, context);
                ret.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, ret.release());
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SWITCH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::SwitchInstruction(lexer, context);
                switch_.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, switch_.release());
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BOOL:
        case BYTE:
        case DOUBLE:
        case FLOAT:
        case INT:
        case LONG:
        case SBYTE:
        case SHORT:
        case TYPEID:
        case UINT:
        case ULONG:
        case USHORT:
        case VOID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::ValueInstruction(lexer, context);
                valueInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, valueInst.release());
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NOP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::NoOperationInstruction(lexer, context);
                nopInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, nopInst.release());
                    }
                }
                *parentMatch9 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
        else soul::lexer::WriteFailureToLog(lexer, "Instruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::StoreInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StoreInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469489);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    cmajor::systemx::intermediate::TypeRef ptref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> value;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> ptrTypeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> ptr;
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
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == STORE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                sp = lexer.GetSourcePos(pos);
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
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    tref = typeRef->value;
                                    context->ResolveType(tref);
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                        value.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                    if (*lexer == COMMA)
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    ptrTypeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        ptref = ptrTypeRef->value;
                        context->ResolveType(ptref);
                    }
                    *parentMatch11 = match;
                }
                *parentMatch10 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, ptref.GetType());
                ptr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StoreInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::StoreInstruction(sp, value.release(), ptr.release()));
                    }
                }
                *parentMatch13 = match;
            }
            *parentMatch12 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StoreInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "StoreInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ArgInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArgInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469490);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> arg;
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
                soul::parser::Match match(false);
                if (*lexer == ARG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    sp = lexer.GetSourcePos(pos);
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                arg.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArgInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ArgInstruction(sp, arg.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArgInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ArgInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::JmpInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "JmpInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469491);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<int32_t>> target;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == JMP)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                target.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JmpInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::JmpInstruction(sp, target->value));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JmpInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "JmpInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::BranchInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BranchInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469492);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> cond;
    std::unique_ptr<soul::parser::Value<int32_t>> trueTarget;
    std::unique_ptr<soul::parser::Value<int32_t>> falseTarget;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == BRANCH)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    sp = lexer.GetSourcePos(pos);
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                    if (match.hit)
                                    {
                                        tref = typeRef->value;
                                        context->ResolveType(tref);
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                            cond.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                        if (*lexer == COMMA)
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
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                    trueTarget.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                    *parentMatch11 = match;
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
                if (*lexer == COMMA)
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
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch13 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch14 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                falseTarget.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BranchInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::BranchInstruction(sp, cond.release(), trueTarget->value, falseTarget->value));
                    }
                }
                *parentMatch14 = match;
            }
            *parentMatch13 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BranchInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "BranchInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ProcedureCallInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcedureCallInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469493);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> callee;
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
                soul::parser::Match match(false);
                if (*lexer == CALL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    sp = lexer.GetSourcePos(pos);
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                callee.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureCallInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ProcedureCallInstruction(sp, callee.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureCallInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ProcedureCallInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::RetInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RetInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469494);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> returnValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == RET)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                if (*lexer == VOID)
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
                                    int64_t tmp = lexer.GetPos();
                                    lexer.SetPos(save);
                                    save = tmp;
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == VOID)
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
                                                if (*lexer == AST)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                if (!match.hit)
                                {
                                    lexer.SetPos(save);
                                }
                                *parentMatch7 = soul::parser::Match(!match.hit, match.value);
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::RetInstruction(sp, nullptr));
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                    if (match.hit)
                                    {
                                        tref = typeRef->value;
                                        context->ResolveType(tref);
                                    }
                                    *parentMatch14 = match;
                                }
                                *parentMatch13 = match;
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
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                                        returnValue.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
                                                #endif
                                                return soul::parser::Match(true, new cmajor::systemx::intermediate::RetInstruction(sp, returnValue.release()));
                                            }
                                        }
                                        *parentMatch16 = match;
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch4 = match;
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "RetInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::SwitchInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SwitchInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469495);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    cmajor::systemx::intermediate::TypeRef caseTref = cmajor::systemx::intermediate::TypeRef();
    cmajor::systemx::intermediate::SwitchInstruction* inst = nullptr;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> cond;
    std::unique_ptr<soul::parser::Value<int32_t>> defaultTarget;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> caseTypeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> caseValue;
    std::unique_ptr<soul::parser::Value<int32_t>> caseTarget;
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
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == SWITCH)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                sp = lexer.GetSourcePos(pos);
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
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                                typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                                if (match.hit)
                                                {
                                                    tref = typeRef->value;
                                                    context->ResolveType(tref);
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
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                                        cond.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                                        defaultTarget.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                        if (match.hit)
                                        {
                                            inst = new cmajor::systemx::intermediate::SwitchInstruction(sp, cond.release(), defaultTarget->value);
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
                                if (*lexer == COMMA)
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
                            if (*lexer == LBRACKET)
                            {
                                ++lexer;
                                match.hit = true;
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
                                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                                    caseTypeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        caseTref = caseTypeRef->value;
                                                        context->ResolveType(caseTref);
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
                                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, caseTref.GetType());
                                                    caseValue.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                                    *parentMatch25 = match;
                                                }
                                                *parentMatch23 = match;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch26 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == COMMA)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch26 = match;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        *parentMatch21 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch27 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch28 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                                                caseTarget.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                                if (match.hit)
                                                {
                                                    inst->AddCaseTarget(cmajor::systemx::intermediate::CaseTarget(caseValue.release(), caseTarget->value));
                                                }
                                                *parentMatch28 = match;
                                            }
                                            *parentMatch27 = match;
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch20 = match;
                                }
                                *parentMatch19 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch29 = &match;
                                {
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch30 = &match;
                                    {
                                        while (true)
                                        {
                                            int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch31 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == COLON)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch31 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch32 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch33 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch34 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch35 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch36 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch37 = &match;
                                                                        {
                                                                            int64_t pos = lexer.GetPos();
                                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                                                                            caseTypeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                                                                            if (match.hit)
                                                                            {
                                                                                caseTref = caseTypeRef->value;
                                                                                context->ResolveType(caseTref);
                                                                            }
                                                                            *parentMatch37 = match;
                                                                        }
                                                                        *parentMatch36 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch38 = &match;
                                                                        {
                                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, caseTref.GetType());
                                                                            caseValue.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                                                            *parentMatch38 = match;
                                                                        }
                                                                        *parentMatch36 = match;
                                                                    }
                                                                    *parentMatch35 = match;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch39 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == COMMA)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        *parentMatch39 = match;
                                                                    }
                                                                    *parentMatch35 = match;
                                                                }
                                                                *parentMatch34 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch40 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch41 = &match;
                                                                    {
                                                                        int64_t pos = lexer.GetPos();
                                                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Label(lexer);
                                                                        caseTarget.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            inst->AddCaseTarget(cmajor::systemx::intermediate::CaseTarget(caseValue.release(), caseTarget->value));
                                                                        }
                                                                        *parentMatch41 = match;
                                                                    }
                                                                    *parentMatch40 = match;
                                                                }
                                                                *parentMatch34 = match;
                                                            }
                                                            *parentMatch33 = match;
                                                        }
                                                        *parentMatch32 = match;
                                                    }
                                                    *parentMatch31 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch30 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch29 = match;
                                }
                                *parentMatch19 = match;
                            }
                            *parentMatch18 = match;
                        }
                        *parentMatch17 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch42 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch42 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchInstruction");
                #endif
                return soul::parser::Match(true, inst);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "SwitchInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ValueInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ValueInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469496);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    cmajor::systemx::intermediate::RegValue* rslt = nullptr;
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::RegValue> result;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> inst;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
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
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::ResultRegValue(lexer, context, tref.GetType());
                        result.reset(static_cast<cmajor::systemx::intermediate::RegValue*>(match.value));
                        if (match.hit)
                        {
                            rslt = result.release();
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operation(lexer, context, rslt);
                inst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ValueInstruction");
                        #endif
                        return soul::parser::Match(true, inst.release());
                    }
                }
                *parentMatch8 = match;
            }
            *parentMatch7 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ValueInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ValueInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Operation(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Operation");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469497);
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> unaryInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> binaryInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> paramInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> localInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> loadInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> elemAddrInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> ptrOffsetInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> ptrDiffInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> functionCallInst;
    std::unique_ptr<cmajor::systemx::intermediate::Instruction> trapInst;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case BITCAST:
        case FLOATTOINT:
        case INTTOFLOAT:
        case INTTOPTR:
        case NEG:
        case NOT:
        case PTRTOINT:
        case SIGNEXTEND:
        case TRUNCATE:
        case ZEROEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::UnaryInstruction(lexer, context, result);
                unaryInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, unaryInst.release());
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
        case ADD:
        case AND:
        case DIV:
        case EQUAL:
        case LESS:
        case MOD:
        case MUL:
        case OR:
        case SHL:
        case SHR:
        case SUB:
        case XOR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::BinaryInstruction(lexer, context, result);
                binaryInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, binaryInst.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PARAM:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::ParamInstruction(lexer, context, result);
                paramInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, paramInst.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case LOCAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::LocalInstruction(lexer, context, result);
                localInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, localInst.release());
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
        case LOAD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::LoadInstruction(lexer, context, result);
                loadInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, loadInst.release());
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ELEMADDR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::ElemAddrInstruction(lexer, context, result);
                elemAddrInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, elemAddrInst.release());
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PTROFFSET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::PtrOffsetInstruction(lexer, context, result);
                ptrOffsetInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, ptrOffsetInst.release());
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PTRDIFF:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::PtrDiffInstruction(lexer, context, result);
                ptrDiffInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, ptrDiffInst.release());
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CALL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::FunctionCallInstruction(lexer, context, result);
                functionCallInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, functionCallInst.release());
                    }
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case TRAP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::TrapInstruction(lexer, context, result);
                trapInst.reset(static_cast<cmajor::systemx::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, trapInst.release());
                    }
                }
                *parentMatch10 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
        else soul::lexer::WriteFailureToLog(lexer, "Operation");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::UnaryInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnaryInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469498);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> notOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> negOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> sextOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> zextOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> truncOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> bcOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> ifOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> fiOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> ipOp;
    std::unique_ptr<cmajor::systemx::intermediate::Value> piOp;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case NOT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == NOT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        notOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::NotInstruction(sp, result, notOp.release()));
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NEG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == NEG)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        negOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::NegInstruction(sp, result, negOp.release()));
                            }
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SIGNEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch10 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == SIGNEXTEND)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        sextOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::SignExtendInstruction(sp, result, sextOp.release()));
                            }
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ZEROEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch14 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == ZEROEXTEND)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch14 = match;
                }
                *parentMatch13 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        zextOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::ZeroExtendInstruction(sp, result, zextOp.release()));
                            }
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch15 = match;
                }
                *parentMatch13 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case TRUNCATE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch18 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == TRUNCATE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch18 = match;
                }
                *parentMatch17 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        truncOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::TruncateInstruction(sp, result, truncOp.release()));
                            }
                        }
                        *parentMatch20 = match;
                    }
                    *parentMatch19 = match;
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BITCAST:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch21 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch22 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == BITCAST)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch22 = match;
                }
                *parentMatch21 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        bcOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::BitcastInstruction(sp, result, bcOp.release()));
                            }
                        }
                        *parentMatch24 = match;
                    }
                    *parentMatch23 = match;
                }
                *parentMatch21 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case INTTOFLOAT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch25 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch26 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == INTTOFLOAT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch26 = match;
                }
                *parentMatch25 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch27 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch28 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        ifOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::IntToFloatInstruction(sp, result, ifOp.release()));
                            }
                        }
                        *parentMatch28 = match;
                    }
                    *parentMatch27 = match;
                }
                *parentMatch25 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case FLOATTOINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch29 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch30 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == FLOATTOINT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch30 = match;
                }
                *parentMatch29 = match;
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
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        fiOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::FloatToIntInstruction(sp, result, fiOp.release()));
                            }
                        }
                        *parentMatch32 = match;
                    }
                    *parentMatch31 = match;
                }
                *parentMatch29 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case INTTOPTR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch33 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch34 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == INTTOPTR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch34 = match;
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch35 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch36 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        ipOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::IntToPtrInstruction(sp, result, ipOp.release()));
                            }
                        }
                        *parentMatch36 = match;
                    }
                    *parentMatch35 = match;
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PTRTOINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == PTRTOINT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
                    }
                    *parentMatch38 = match;
                }
                *parentMatch37 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch39 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch40 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                        piOp.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new cmajor::systemx::intermediate::PtrToIntInstruction(sp, result, piOp.release()));
                            }
                        }
                        *parentMatch40 = match;
                    }
                    *parentMatch39 = match;
                }
                *parentMatch37 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "UnaryInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::BinaryInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BinaryInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469499);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftAdd;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightAdd;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftSub;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightSub;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftMul;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightMul;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftDiv;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightDiv;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftMod;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightMod;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftAnd;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightAnd;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftOr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightOr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftXor;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightXor;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftShl;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightShl;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftShr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightShr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftEqual;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightEqual;
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftLess;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightLess;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case ADD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
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
                                    if (*lexer == ADD)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftAdd.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightAdd.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch3 = match;
                    }
                    *parentMatch2 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::AddInstruction(sp, result, leftAdd.release(), rightAdd.release()));
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
        case SUB:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                int64_t pos = lexer.GetPos();
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
                                    soul::parser::Match match(false);
                                    if (*lexer == SUB)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftSub.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
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
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightSub.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch18 = match;
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::SubInstruction(sp, result, leftSub.release(), rightSub.release()));
                    }
                }
                *parentMatch10 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MUL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch19 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch20 = &match;
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
                                    soul::parser::Match match(false);
                                    if (*lexer == MUL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
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
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftMul.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch25 = match;
                                }
                                *parentMatch23 = match;
                            }
                            *parentMatch22 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch26 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch26 = match;
                            }
                            *parentMatch22 = match;
                        }
                        *parentMatch21 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch27 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightMul.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch27 = match;
                        }
                        *parentMatch21 = match;
                    }
                    *parentMatch20 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::MulInstruction(sp, result, leftMul.release(), rightMul.release()));
                    }
                }
                *parentMatch19 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DIV:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch28 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch29 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch30 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch31 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch32 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch33 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == DIV)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch33 = match;
                                }
                                *parentMatch32 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch34 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftDiv.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch34 = match;
                                }
                                *parentMatch32 = match;
                            }
                            *parentMatch31 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch35 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch35 = match;
                            }
                            *parentMatch31 = match;
                        }
                        *parentMatch30 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch36 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightDiv.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch36 = match;
                        }
                        *parentMatch30 = match;
                    }
                    *parentMatch29 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::DivInstruction(sp, result, leftDiv.release(), rightDiv.release()));
                    }
                }
                *parentMatch28 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch39 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch40 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch41 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch42 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == MOD)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch42 = match;
                                }
                                *parentMatch41 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch43 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftMod.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch43 = match;
                                }
                                *parentMatch41 = match;
                            }
                            *parentMatch40 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch44 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch44 = match;
                            }
                            *parentMatch40 = match;
                        }
                        *parentMatch39 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch45 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightMod.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch45 = match;
                        }
                        *parentMatch39 = match;
                    }
                    *parentMatch38 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ModInstruction(sp, result, leftMod.release(), rightMod.release()));
                    }
                }
                *parentMatch37 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case AND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch46 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch47 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch48 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch49 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch50 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch51 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == AND)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch51 = match;
                                }
                                *parentMatch50 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch52 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftAnd.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch52 = match;
                                }
                                *parentMatch50 = match;
                            }
                            *parentMatch49 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch53 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch53 = match;
                            }
                            *parentMatch49 = match;
                        }
                        *parentMatch48 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch54 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightAnd.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch54 = match;
                        }
                        *parentMatch48 = match;
                    }
                    *parentMatch47 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::AndInstruction(sp, result, leftAnd.release(), rightAnd.release()));
                    }
                }
                *parentMatch46 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case OR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch55 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch56 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch57 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch58 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch59 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch60 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == OR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch60 = match;
                                }
                                *parentMatch59 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch61 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftOr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch61 = match;
                                }
                                *parentMatch59 = match;
                            }
                            *parentMatch58 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch62 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch62 = match;
                            }
                            *parentMatch58 = match;
                        }
                        *parentMatch57 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch63 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightOr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch63 = match;
                        }
                        *parentMatch57 = match;
                    }
                    *parentMatch56 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::OrInstruction(sp, result, leftOr.release(), rightOr.release()));
                    }
                }
                *parentMatch55 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XOR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch64 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch65 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch66 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch67 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch68 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch69 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == XOR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch69 = match;
                                }
                                *parentMatch68 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch70 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftXor.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch70 = match;
                                }
                                *parentMatch68 = match;
                            }
                            *parentMatch67 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch71 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch71 = match;
                            }
                            *parentMatch67 = match;
                        }
                        *parentMatch66 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch72 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightXor.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch72 = match;
                        }
                        *parentMatch66 = match;
                    }
                    *parentMatch65 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::XorInstruction(sp, result, leftXor.release(), rightXor.release()));
                    }
                }
                *parentMatch64 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SHL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch73 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch74 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch75 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch76 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch77 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch78 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == SHL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch78 = match;
                                }
                                *parentMatch77 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch79 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftShl.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch79 = match;
                                }
                                *parentMatch77 = match;
                            }
                            *parentMatch76 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch80 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch80 = match;
                            }
                            *parentMatch76 = match;
                        }
                        *parentMatch75 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch81 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightShl.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch81 = match;
                        }
                        *parentMatch75 = match;
                    }
                    *parentMatch74 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ShlInstruction(sp, result, leftShl.release(), rightShl.release()));
                    }
                }
                *parentMatch73 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SHR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch82 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch83 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch84 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch85 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch86 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch87 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == SHR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch87 = match;
                                }
                                *parentMatch86 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch88 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftShr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch88 = match;
                                }
                                *parentMatch86 = match;
                            }
                            *parentMatch85 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch89 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch89 = match;
                            }
                            *parentMatch85 = match;
                        }
                        *parentMatch84 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch90 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightShr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch90 = match;
                        }
                        *parentMatch84 = match;
                    }
                    *parentMatch83 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ShrInstruction(sp, result, leftShr.release(), rightShr.release()));
                    }
                }
                *parentMatch82 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EQUAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch91 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch92 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch93 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch94 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch95 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch96 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == EQUAL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch96 = match;
                                }
                                *parentMatch95 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch97 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftEqual.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch97 = match;
                                }
                                *parentMatch95 = match;
                            }
                            *parentMatch94 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch98 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch98 = match;
                            }
                            *parentMatch94 = match;
                        }
                        *parentMatch93 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch99 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightEqual.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch99 = match;
                        }
                        *parentMatch93 = match;
                    }
                    *parentMatch92 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::EqualInstruction(sp, result, leftEqual.release(), rightEqual.release()));
                    }
                }
                *parentMatch91 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case LESS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch100 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch101 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch102 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch103 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch104 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch105 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == LESS)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        sp = lexer.GetSourcePos(pos);
                                    }
                                    *parentMatch105 = match;
                                }
                                *parentMatch104 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch106 = &match;
                                {
                                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                                    leftLess.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                                    *parentMatch106 = match;
                                }
                                *parentMatch104 = match;
                            }
                            *parentMatch103 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch107 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch107 = match;
                            }
                            *parentMatch103 = match;
                        }
                        *parentMatch102 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch108 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            rightLess.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                            *parentMatch108 = match;
                        }
                        *parentMatch102 = match;
                    }
                    *parentMatch101 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::LessInstruction(sp, result, leftLess.release(), rightLess.release()));
                    }
                }
                *parentMatch100 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "BinaryInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ParamInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParamInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469500);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == PARAM)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            sp = lexer.GetSourcePos(pos);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParamInstruction");
                #endif
                return soul::parser::Match(true, new cmajor::systemx::intermediate::ParamInstruction(sp, result));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParamInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ParamInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::LocalInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LocalInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469501);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == LOCAL)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                if (match.hit)
                {
                    tref = typeRef->value;
                    context->ResolveType(tref);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::LocalInstruction(sp, result, tref.GetType()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "LocalInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::LoadInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LoadInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469502);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> ptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == LOAD)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                sp = lexer.GetSourcePos(pos);
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                ptr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LoadInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::LoadInstruction(sp, result, ptr.release()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LoadInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "LoadInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::ElemAddrInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ElemAddrInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469503);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> ptr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> index;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == ELEMADDR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                    ptr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                index.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElemAddrInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::ElemAddrInstruction(sp, result, ptr.release(), index.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElemAddrInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ElemAddrInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::PtrOffsetInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PtrOffsetInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469504);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> ptr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> offset;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == PTROFFSET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                    ptr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                offset.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrOffsetInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::PtrOffsetInstruction(sp, result, ptr.release(), offset.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrOffsetInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "PtrOffsetInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::PtrDiffInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PtrDiffInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469505);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> leftPtr;
    std::unique_ptr<cmajor::systemx::intermediate::Value> rightPtr;
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == PTRDIFF)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        sp = lexer.GetSourcePos(pos);
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
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                    leftPtr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                rightPtr.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrDiffInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::PtrDiffInstruction(sp, result, leftPtr.release(), rightPtr.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrDiffInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "PtrDiffInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::FunctionCallInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionCallInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469506);
    cmajor::systemx::intermediate::TypeRef tref = cmajor::systemx::intermediate::TypeRef();
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>> typeRef;
    std::unique_ptr<cmajor::systemx::intermediate::Value> callee;
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
                soul::parser::Match match(false);
                if (*lexer == CALL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    sp = lexer.GetSourcePos(pos);
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
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<cmajor::systemx::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Value(lexer, context, tref.GetType());
                callee.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCallInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::FunctionCallInstruction(sp, result, callee.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCallInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionCallInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::TrapInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TrapInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469507);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    std::unique_ptr<cmajor::systemx::intermediate::Value> op1;
    std::unique_ptr<cmajor::systemx::intermediate::Value> op2;
    std::unique_ptr<cmajor::systemx::intermediate::Value> op3;
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
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == TRAP)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                sp = lexer.GetSourcePos(pos);
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
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                            op1.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
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
                        if (*lexer == COMMA)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                    op2.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = IntermediateCodeParser<LexerT>::Operand(lexer, context);
                op3.reset(static_cast<cmajor::systemx::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TrapInstruction");
                        #endif
                        return soul::parser::Match(true, new cmajor::systemx::intermediate::TrapInstruction(sp, result, op1.release(), op2.release(), op3.release()));
                    }
                }
                *parentMatch11 = match;
            }
            *parentMatch10 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TrapInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "TrapInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::NoOperationInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NoOperationInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469508);
    soul::ast::SourcePos sp = soul::ast::SourcePos();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == NOP)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            sp = lexer.GetSourcePos(pos);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoOperationInstruction");
                #endif
                return soul::parser::Match(true, new cmajor::systemx::intermediate::NoOperationInstruction(sp));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoOperationInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "NoOperationInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::Metadata(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Metadata");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469509);
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
                if (*lexer == METADATA)
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
                    if (*lexer == LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
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
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataStruct(lexer, context);
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                if (*lexer == RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->ResolveMetadataReferences();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Metadata");
        else soul::lexer::WriteFailureToLog(lexer, "Metadata");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataStruct(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataStruct");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469510);
    cmajor::systemx::intermediate::MetadataStruct* value = nullptr;
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
                        if (*lexer == EXCLAMATION)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch4 = match;
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
                                if (*lexer == NUMBER)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    value = context->AddMetadataStruct(lexer.GetSourcePos(pos), lexer.GetToken(pos).ToInt(), context);
                                }
                                *parentMatch6 = match;
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
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == ASSIGN)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataField(lexer, context, value);
                            *parentMatch12 = match;
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
                                                soul::parser::Match* parentMatch16 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch17 = &match;
                                                    {
                                                        soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataField(lexer, context, value);
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
                    }
                }
                *parentMatch9 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch18 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == RBRACE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch18 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataStruct");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataStruct");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataField(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::MetadataStruct* parent)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataField");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469511);
    std::string fieldName = std::string();
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> item;
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
                soul::parser::Match match(false);
                if (*lexer == ID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    fieldName = util::ToUtf8(lexer.GetToken(pos).ToString());
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
                if (*lexer == COLON)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch4 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataItem(lexer, context);
                item.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    parent->AddItem(fieldName, item.release());
                }
                *parentMatch5 = match;
            }
            *parentMatch4 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataField");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataField");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataItem(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataItem");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469512);
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> mdBool;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> mdLong;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> mdString;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> mdArray;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataRef> mdRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case FALSE:
        case TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataBool(lexer, context);
                mdBool.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdBool.release());
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
        case NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataLong(lexer, context);
                mdLong.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdLong.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case STRING:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataString(lexer, context);
                mdString.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdString.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case LBRACKET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataArray(lexer, context);
                mdArray.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdArray.release());
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
        case EXCLAMATION:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataRef(lexer, context);
                mdRef.reset(static_cast<cmajor::systemx::intermediate::MetadataRef*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdRef.release());
                    }
                }
                *parentMatch5 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataItem");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataBool(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataBool");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469513);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == TRUE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataBool(true));
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
        case FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == FALSE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataBool(false));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataBool");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataLong(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataLong");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469514);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == NUMBER)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataLong");
                #endif
                return soul::parser::Match(true, context->CreateMetadataLong(lexer.GetToken(pos).ToLong()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataLong");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataLong");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataString(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataString");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469515);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == STRING)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataString");
                #endif
                return soul::parser::Match(true, context->CreateMetadataString(util::ToUtf8(lexer.GetToken(pos).ToString()), true));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataString");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataString");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataArray(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataArray");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469516);
    cmajor::systemx::intermediate::MetadataArray* array = nullptr;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> first;
    std::unique_ptr<cmajor::systemx::intermediate::MetadataItem> next;
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
                soul::parser::Match match(false);
                if (*lexer == LBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    array = context->CreateMetadataArray();
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
                                soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataItem(lexer, context);
                                first.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                                if (match.hit)
                                {
                                    array->AddItem(first.release());
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
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = IntermediateCodeParser<LexerT>::MetadataItem(lexer, context);
                                                            next.reset(static_cast<cmajor::systemx::intermediate::MetadataItem*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                array->AddItem(next.release());
                                                            }
                                                            *parentMatch13 = match;
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match(false);
                if (*lexer == RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataArray");
                        #endif
                        return soul::parser::Match(true, array);
                    }
                }
                *parentMatch15 = match;
            }
            *parentMatch14 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataArray");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataArray");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateCodeParser<LexerT>::MetadataRef(LexerT& lexer, cmajor::systemx::intermediate::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataRef");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4396680239057469517);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == EXCLAMATION)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataRef");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataRef(lexer.GetSourcePos(pos), lexer.GetToken(pos).ToInt()));
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataRef");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataRef");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct IntermediateCodeParser<soul::lexer::Lexer<cmajor::systemx::intermediate::code::lexer::IntermediateCodeLexer<char32_t>, char32_t>>;

} // namespace cmajor::systemx::intermediate::code::parser
