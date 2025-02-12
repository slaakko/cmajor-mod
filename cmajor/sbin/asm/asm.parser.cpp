
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/sbin/asm/asm.parser' using soul parser generator spg version 5.0.0

module cmajor.sbin.assembly.parser;

import util;
import soul.ast.spg;
import cmajor.sbin.assembly.asm_util;
import cmajor.sbin.assembly.lexer;
import cmajor.sbin.assembly.token;

using namespace cmajor::sbin::assembly::asm_util;
using namespace cmajor::sbin::assembly::lexer;
using namespace cmajor::sbin::assembly::token;

namespace cmajor::sbin::assembly::parser {

template<typename LexerT>
std::unique_ptr<cmajor::sbin::assembly::AsmFileNode> AsmParser<LexerT>::Parse(LexerT& lexer)
{
    std::unique_ptr<cmajor::sbin::assembly::AsmFileNode> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = AsmParser<LexerT>::AsmFile(lexer);
    value.reset(static_cast<cmajor::sbin::assembly::AsmFileNode*>(match.value));
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
soul::parser::Match AsmParser<LexerT>::AsmFile(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AsmFile");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364097);
    cmajor::sbin::assembly::AsmFileNode* asmFileNode = nullptr;
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
                                soul::parser::Match match(true);
                                if (match.hit)
                                {
                                    asmFileNode = new cmajor::sbin::assembly::AsmFileNode(lexer.GetSpan(pos));
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
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match = AsmParser<LexerT>::DeclarationSection(lexer, asmFileNode);
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
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                soul::parser::Match match = AsmParser<LexerT>::DataSection(lexer, asmFileNode);
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
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
                            soul::parser::Match match = AsmParser<LexerT>::CodeSection(lexer, asmFileNode);
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                {
                    soul::parser::Match match = AsmParser<LexerT>::End(lexer);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AsmFile");
                #endif
                return soul::parser::Match(true, asmFileNode);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AsmFile");
        else soul::lexer::WriteFailureToLog(lexer, "AsmFile");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::EmptyLine(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EmptyLine");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364098);
    soul::parser::Match match(false);
    if (*lexer == NEWLINE)
    {
        ++lexer;
        match.hit = true;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EmptyLine");
        else soul::lexer::WriteFailureToLog(lexer, "EmptyLine");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::End(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "End");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364099);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch3 = &match;
                {
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
                            if (match.hit)
                            {
                                *parentMatch3 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == END)
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
                if (*lexer == NEWLINE)
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
            soul::parser::Match match(true);
            soul::parser::Match* parentMatch7 = &match;
            {
                while (true)
                {
                    int64_t save = lexer.GetPos();
                    {
                        soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
                        if (match.hit)
                        {
                            *parentMatch7 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                            break;
                        }
                    }
                }
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "End");
        else soul::lexer::WriteFailureToLog(lexer, "End");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::DeclarationSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeclarationSection");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364100);
    std::unique_ptr<cmajor::sbin::assembly::DeclarationNode> declaration;
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
                    switch (*lexer)
                    {
                        case EXTRN:
                        case PUBLIC:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch3 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch4 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = AsmParser<LexerT>::Declaration(lexer);
                                    declaration.reset(static_cast<cmajor::sbin::assembly::DeclarationNode*>(match.value));
                                    if (match.hit)
                                    {
                                        asmFileNode->AddDeclaration(declaration.release());
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
                                    if (*lexer == NEWLINE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch5 = match;
                                }
                                *parentMatch3 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch2 = match;
                            }
                            break;
                        }
                        case NEWLINE:
                        {
                            soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclarationSection");
        else soul::lexer::WriteFailureToLog(lexer, "DeclarationSection");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Declaration(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Declaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364101);
    cmajor::sbin::assembly::Type type = cmajor::sbin::assembly::Type();
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::SymbolKind>> kind;
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> symbol;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Type>> typeField;
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
                    soul::parser::Match match = AsmParser<LexerT>::SymbolKind(lexer);
                    kind.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::SymbolKind>*>(match.value));
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match = AsmParser<LexerT>::Symbol(lexer);
                        symbol.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch7 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch8 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = AsmParser<LexerT>::TypeField(lexer);
                                typeField.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Type>*>(match.value));
                                if (match.hit)
                                {
                                    type = typeField->value;
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch7 = match;
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
                                            type = cmajor::sbin::assembly::Type::none;
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                                *parentMatch7 = match;
                            }
                        }
                        *parentMatch6 = match;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Declaration");
                #endif
                return soul::parser::Match(true, new cmajor::sbin::assembly::DeclarationNode(lexer.GetSpan(pos), kind->value, symbol.release(), type));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Declaration");
        else soul::lexer::WriteFailureToLog(lexer, "Declaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::SymbolKind(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SymbolKind");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364102);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PUBLIC:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PUBLIC)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolKind");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::SymbolKind>(cmajor::sbin::assembly::SymbolKind::public_));
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
        case EXTRN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EXTRN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolKind");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::SymbolKind>(cmajor::sbin::assembly::SymbolKind::external));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolKind");
        else soul::lexer::WriteFailureToLog(lexer, "SymbolKind");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Symbol(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Symbol");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364103);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == SYMBOL)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Symbol");
                #endif
                return soul::parser::Match(true, new cmajor::sbin::assembly::SymbolNode(lexer.GetSpan(pos), util::ToUtf8(lexer.GetToken(pos).ToString())));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Symbol");
        else soul::lexer::WriteFailureToLog(lexer, "Symbol");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::TypeField(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeField");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364104);
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Type>> type;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == COLON)
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
                soul::parser::Match match = AsmParser<LexerT>::Type(lexer);
                type.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Type>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeField");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(type->value));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeField");
        else soul::lexer::WriteFailureToLog(lexer, "TypeField");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Type(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Type");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364105);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case BYTE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::byte_));
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
        case WORD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == WORD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::word));
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
        case DWORD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DWORD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::dword));
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
        case QWORD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == QWORD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::qword));
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
        case OWORD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == OWORD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::oword));
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
        case PROC:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PROC)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(cmajor::sbin::assembly::Type::proc));
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
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
        else soul::lexer::WriteFailureToLog(lexer, "Type");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::DataSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataSection");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364106);
    std::unique_ptr<cmajor::sbin::assembly::DataDefinitionNode> dataDefinitionLine;
    std::unique_ptr<cmajor::sbin::assembly::MacroDefinitionNode> macroAssignmentLine;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match = AsmParser<LexerT>::DataSectionHeader(lexer);
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(true);
            soul::parser::Match* parentMatch2 = &match;
            {
                while (true)
                {
                    int64_t save = lexer.GetPos();
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
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch6 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = AsmParser<LexerT>::DataDefinitionLine(lexer);
                                        dataDefinitionLine.reset(static_cast<cmajor::sbin::assembly::DataDefinitionNode*>(match.value));
                                        if (match.hit)
                                        {
                                            asmFileNode->AddDataDefinition(dataDefinitionLine.release());
                                        }
                                        *parentMatch6 = match;
                                    }
                                    *parentMatch5 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch7 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match = AsmParser<LexerT>::MacroAssignmentLine(lexer);
                                            macroAssignmentLine.reset(static_cast<cmajor::sbin::assembly::MacroDefinitionNode*>(match.value));
                                            *parentMatch7 = match;
                                        }
                                        *parentMatch5 = match;
                                    }
                                }
                                *parentMatch4 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch4 = match;
                                }
                            }
                            *parentMatch3 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch2 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                            break;
                        }
                    }
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataSection");
        else soul::lexer::WriteFailureToLog(lexer, "DataSection");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::DataSectionHeader(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataSectionHeader");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364107);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == DOT)
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
                if (*lexer == DATA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch3 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == NEWLINE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch3 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataSectionHeader");
        else soul::lexer::WriteFailureToLog(lexer, "DataSectionHeader");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::DataDefinitionLine(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataDefinitionLine");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364108);
    std::unique_ptr<cmajor::sbin::assembly::DataDefinitionNode> definition;
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
                soul::parser::Match match = AsmParser<LexerT>::DataDefinition(lexer);
                definition.reset(static_cast<cmajor::sbin::assembly::DataDefinitionNode*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == NEWLINE)
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinitionLine");
                #endif
                return soul::parser::Match(true, definition.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinitionLine");
        else soul::lexer::WriteFailureToLog(lexer, "DataDefinitionLine");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::MacroAssignmentLine(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MacroAssignmentLine");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364109);
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> symbol;
    std::unique_ptr<cmajor::sbin::assembly::Node> expr;
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
                        soul::parser::Match match = AsmParser<LexerT>::Symbol(lexer);
                        symbol.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == ASSIGN)
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
                        soul::parser::Match match = AsmParser<LexerT>::Expr(lexer);
                        expr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
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
                    if (*lexer == NEWLINE)
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MacroAssignmentLine");
                #endif
                return soul::parser::Match(true, new cmajor::sbin::assembly::MacroDefinitionNode(lexer.GetSpan(pos), symbol.release(), expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MacroAssignmentLine");
        else soul::lexer::WriteFailureToLog(lexer, "MacroAssignmentLine");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::DataDefinition(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364110);
    cmajor::sbin::assembly::DataDefinitionNode* dataDefinitionNode = nullptr;
    std::unique_ptr<cmajor::sbin::assembly::LabelNode> label;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::DataInstKind>> inst;
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
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match = AsmParser<LexerT>::Label(lexer);
                                label.reset(static_cast<cmajor::sbin::assembly::LabelNode*>(match.value));
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
                                soul::parser::Match match = AsmParser<LexerT>::DataInst(lexer);
                                inst.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::DataInstKind>*>(match.value));
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        dataDefinitionNode = new cmajor::sbin::assembly::DataDefinitionNode(lexer.GetSpan(pos), label.release(), inst->value);
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
                    soul::parser::Match match = AsmParser<LexerT>::OperandList(lexer, dataDefinitionNode);
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinition");
                #endif
                return soul::parser::Match(true, dataDefinitionNode);
            }
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
soul::parser::Match AsmParser<LexerT>::Label(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364111);
    cmajor::sbin::assembly::LabelNode* labelNode = nullptr;
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
                    if (*lexer == SYMBOL)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        labelNode = new cmajor::sbin::assembly::LabelNode(lexer.GetSpan(pos), util::ToUtf8(lexer.GetToken(pos).ToString()));
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
                                soul::parser::Match match(false);
                                if (*lexer == COLON)
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
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == NEWLINE)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
                #endif
                return soul::parser::Match(true, labelNode);
            }
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
soul::parser::Match AsmParser<LexerT>::DataInst(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataInst");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364112);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case DB:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DB)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::db));
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
        case DW:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DW)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::dw));
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
        case DD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::dd));
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
        case DQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::dq));
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
        case REAL4:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == REAL4)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::real4));
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
        case REAL8:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == REAL8)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::DataInstKind>(cmajor::sbin::assembly::DataInstKind::real8));
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
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataInst");
        else soul::lexer::WriteFailureToLog(lexer, "DataInst");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::OperandList(LexerT& lexer, cmajor::sbin::assembly::InstructionBaseNode* instNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OperandList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364113);
    std::unique_ptr<cmajor::sbin::assembly::Node> first;
    std::unique_ptr<cmajor::sbin::assembly::Node> next;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = AsmParser<LexerT>::Operand(lexer);
            first.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
            if (match.hit)
            {
                instNode->AddOperand(first.release());
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
            soul::parser::Match match(true);
            soul::parser::Match* parentMatch3 = &match;
            {
                while (true)
                {
                    int64_t save = lexer.GetPos();
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
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
                                        soul::parser::Match match = AsmParser<LexerT>::Operand(lexer);
                                        next.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            instNode->AddOperand(next.release());
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
                            *parentMatch3 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                            break;
                        }
                    }
                }
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperandList");
        else soul::lexer::WriteFailureToLog(lexer, "OperandList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Operand(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364114);
    std::unique_ptr<cmajor::sbin::assembly::Node> expr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = AsmParser<LexerT>::Expr(lexer);
        expr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operand");
                #endif
                return soul::parser::Match(true, expr.release());
            }
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
soul::parser::Match AsmParser<LexerT>::Expr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364115);
    std::unique_ptr<cmajor::sbin::assembly::Node> binaryExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match = AsmParser<LexerT>::BinaryExpr(lexer);
        binaryExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Expr");
                #endif
                return soul::parser::Match(true, binaryExpr.release());
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
soul::parser::Match AsmParser<LexerT>::BinaryExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BinaryExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364116);
    cmajor::sbin::assembly::Node* expr = nullptr;
    std::unique_ptr<cmajor::sbin::assembly::Node> left;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Operator>> op;
    std::unique_ptr<cmajor::sbin::assembly::Node> right;
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
                    soul::parser::Match match = AsmParser<LexerT>::UnaryExpr(lexer);
                    left.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                    if (match.hit)
                    {
                        expr = left.release();
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
                                        soul::parser::Match match = AsmParser<LexerT>::BinaryOp(lexer);
                                        op.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Operator>*>(match.value));
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
                                                soul::parser::Match match = AsmParser<LexerT>::UnaryExpr(lexer);
                                                right.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    expr = new cmajor::sbin::assembly::BinaryExprNode(lexer.GetSpan(pos), expr, op->value, right.release());
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryExpr");
                #endif
                return soul::parser::Match(true, expr);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryExpr");
        else soul::lexer::WriteFailureToLog(lexer, "BinaryExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::BinaryOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BinaryOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364117);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PLUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Operator>(cmajor::sbin::assembly::Operator::plus));
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
        case MINUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MINUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Operator>(cmajor::sbin::assembly::Operator::minus));
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
        case TIMES:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == TIMES)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Operator>(cmajor::sbin::assembly::Operator::times));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryOp");
        else soul::lexer::WriteFailureToLog(lexer, "BinaryOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::UnaryExpr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364118);
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Operator>> op;
    std::unique_ptr<cmajor::sbin::assembly::Node> unaryExpr;
    std::unique_ptr<cmajor::sbin::assembly::Node> primaryExpr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case MINUS:
        case PLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                soul::parser::Match match = AsmParser<LexerT>::UnaryOp(lexer);
                op.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Operator>*>(match.value));
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
                        soul::parser::Match match = AsmParser<LexerT>::UnaryExpr(lexer);
                        unaryExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                                #endif
                                return soul::parser::Match(true, new cmajor::sbin::assembly::UnaryExprNode(lexer.GetSpan(pos), op->value, unaryExpr.release()));
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
        case AH:
        case AL:
        case AX:
        case BH:
        case BL:
        case BP:
        case BPL:
        case BX:
        case BYTE:
        case CH:
        case CL:
        case CX:
        case DH:
        case DI:
        case DIL:
        case DL:
        case DWORD:
        case DX:
        case EAX:
        case EBP:
        case EBX:
        case ECX:
        case EDI:
        case EDX:
        case ESI:
        case ESP:
        case HEXNUMBER:
        case INTEGER:
        case LBRACKET:
        case LPAREN:
        case OWORD:
        case PROC:
        case QWORD:
        case R10:
        case R10B:
        case R10D:
        case R10W:
        case R11:
        case R11B:
        case R11D:
        case R11W:
        case R12:
        case R12B:
        case R12D:
        case R12W:
        case R13:
        case R13B:
        case R13D:
        case R13W:
        case R14:
        case R14B:
        case R14D:
        case R14W:
        case R15:
        case R15B:
        case R15D:
        case R15W:
        case R8:
        case R8B:
        case R8D:
        case R8W:
        case R9:
        case R9B:
        case R9D:
        case R9W:
        case RAX:
        case RBP:
        case RBX:
        case RCX:
        case RDI:
        case RDX:
        case REAL:
        case RSI:
        case RSP:
        case SI:
        case SIL:
        case SP:
        case SPL:
        case STRING:
        case SYMBOL:
        case WORD:
        case XMM0:
        case XMM1:
        case XMM10:
        case XMM11:
        case XMM12:
        case XMM13:
        case XMM14:
        case XMM15:
        case XMM2:
        case XMM3:
        case XMM4:
        case XMM5:
        case XMM6:
        case XMM7:
        case XMM8:
        case XMM9:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::PrimaryExpr(lexer);
                primaryExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryExpr");
                        #endif
                        return soul::parser::Match(true, primaryExpr.release());
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
soul::parser::Match AsmParser<LexerT>::UnaryOp(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnaryOp");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364119);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PLUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PLUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Operator>(cmajor::sbin::assembly::Operator::plus));
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
        case MINUS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MINUS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryOp");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Operator>(cmajor::sbin::assembly::Operator::minus));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryOp");
        else soul::lexer::WriteFailureToLog(lexer, "UnaryOp");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::PrimaryExpr(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364120);
    std::unique_ptr<cmajor::sbin::assembly::Node> contentExpr;
    std::unique_ptr<cmajor::sbin::assembly::Node> sizeExpr;
    std::unique_ptr<cmajor::sbin::assembly::Node> parenthesizedExpr;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::machine_x64::Register>> reg;
    std::unique_ptr<cmajor::sbin::assembly::Node> value;
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> symbol;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case LBRACKET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::ContentExpr(lexer);
                contentExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, contentExpr.release());
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
        case BYTE:
        case DWORD:
        case OWORD:
        case PROC:
        case QWORD:
        case WORD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::SizeExpr(lexer);
                sizeExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, sizeExpr.release());
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
        case LPAREN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::ParenthesizedExpr(lexer);
                parenthesizedExpr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, parenthesizedExpr.release());
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
        case AH:
        case AL:
        case AX:
        case BH:
        case BL:
        case BP:
        case BPL:
        case BX:
        case CH:
        case CL:
        case CX:
        case DH:
        case DI:
        case DIL:
        case DL:
        case DX:
        case EAX:
        case EBP:
        case EBX:
        case ECX:
        case EDI:
        case EDX:
        case ESI:
        case ESP:
        case R10:
        case R10B:
        case R10D:
        case R10W:
        case R11:
        case R11B:
        case R11D:
        case R11W:
        case R12:
        case R12B:
        case R12D:
        case R12W:
        case R13:
        case R13B:
        case R13D:
        case R13W:
        case R14:
        case R14B:
        case R14D:
        case R14W:
        case R15:
        case R15B:
        case R15D:
        case R15W:
        case R8:
        case R8B:
        case R8D:
        case R8W:
        case R9:
        case R9B:
        case R9D:
        case R9W:
        case RAX:
        case RBP:
        case RBX:
        case RCX:
        case RDI:
        case RDX:
        case RSI:
        case RSP:
        case SI:
        case SIL:
        case SP:
        case SPL:
        case XMM0:
        case XMM1:
        case XMM10:
        case XMM11:
        case XMM12:
        case XMM13:
        case XMM14:
        case XMM15:
        case XMM2:
        case XMM3:
        case XMM4:
        case XMM5:
        case XMM6:
        case XMM7:
        case XMM8:
        case XMM9:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::Register(lexer);
                reg.reset(static_cast<soul::parser::Value<cmajor::sbin::machine_x64::Register>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, new cmajor::sbin::assembly::RegisterNode(lexer.GetSpan(pos), reg->value));
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
        case HEXNUMBER:
        case INTEGER:
        case REAL:
        case STRING:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::Value(lexer);
                value.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, value.release());
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
        case SYMBOL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = AsmParser<LexerT>::Symbol(lexer);
                symbol.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PrimaryExpr");
                        #endif
                        return soul::parser::Match(true, symbol.release());
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
soul::parser::Match AsmParser<LexerT>::ContentExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ContentExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364121);
    cmajor::sbin::assembly::ContentExprNode* contentExprNode = nullptr;
    std::unique_ptr<cmajor::sbin::assembly::Node> expr;
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
                    if (*lexer == LBRACKET)
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = AsmParser<LexerT>::Expr(lexer);
                            expr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
                            if (match.hit)
                            {
                                contentExprNode = new cmajor::sbin::assembly::ContentExprNode(lexer.GetSpan(pos), expr.release());
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                    if (*lexer == RBRACKET)
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContentExpr");
                #endif
                return soul::parser::Match(true, contentExprNode);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContentExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ContentExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::SizeExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SizeExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364122);
    cmajor::sbin::assembly::SizeExprNode* sizeExprNode = nullptr;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Type>> sizePrefix;
    std::unique_ptr<cmajor::sbin::assembly::Node> expr;
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
                soul::parser::Match match = AsmParser<LexerT>::SizePrefix(lexer);
                sizePrefix.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Type>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match = AsmParser<LexerT>::Expr(lexer);
                    expr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizeExpr");
                #endif
                return soul::parser::Match(true, new cmajor::sbin::assembly::SizeExprNode(lexer.GetSpan(pos), sizePrefix->value, expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizeExpr");
        else soul::lexer::WriteFailureToLog(lexer, "SizeExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::SizePrefix(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SizePrefix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364123);
    std::unique_ptr<soul::parser::Value<cmajor::sbin::assembly::Type>> type;
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
                soul::parser::Match match = AsmParser<LexerT>::Type(lexer);
                type.reset(static_cast<soul::parser::Value<cmajor::sbin::assembly::Type>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == PTR)
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizePrefix");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::assembly::Type>(type->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SizePrefix");
        else soul::lexer::WriteFailureToLog(lexer, "SizePrefix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::ParenthesizedExpr(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParenthesizedExpr");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364124);
    std::unique_ptr<cmajor::sbin::assembly::Node> expr;
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
                        soul::parser::Match match = AsmParser<LexerT>::Expr(lexer);
                        expr.reset(static_cast<cmajor::sbin::assembly::Node*>(match.value));
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
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParenthesizedExpr");
                #endif
                return soul::parser::Match(true, new cmajor::sbin::assembly::ParenthesizedExprNode(lexer.GetSpan(pos), expr.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParenthesizedExpr");
        else soul::lexer::WriteFailureToLog(lexer, "ParenthesizedExpr");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Register(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Register");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364125);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case AL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == AL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::al));
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
        case BL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::bl));
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
        case CL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::cl));
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
        case DL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::dl));
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
        case SIL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SIL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::sil));
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
        case DIL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DIL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::dil));
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
        case BPL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BPL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::bpl));
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
        case SPL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SPL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::spl));
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
        case R8B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R8B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r8b));
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
        case R9B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R9B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r9b));
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
        case R10B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R10B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r10b));
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
        case R11B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R11B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r11b));
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
        case R12B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R12B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r12b));
                    }
                }
                *parentMatch13 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R13B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch14 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R13B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r13b));
                    }
                }
                *parentMatch14 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R14B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch15 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R14B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r14b));
                    }
                }
                *parentMatch15 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R15B:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch16 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R15B)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r15b));
                    }
                }
                *parentMatch16 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case AH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == AH)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ah));
                    }
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch18 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BH)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::bh));
                    }
                }
                *parentMatch18 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch19 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CH)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ch));
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
        case DH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch20 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DH)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::dh));
                    }
                }
                *parentMatch20 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case AX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch21 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == AX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ax));
                    }
                }
                *parentMatch21 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch22 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::bx));
                    }
                }
                *parentMatch22 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch23 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::cx));
                    }
                }
                *parentMatch23 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch24 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::dx));
                    }
                }
                *parentMatch24 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch25 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::si));
                    }
                }
                *parentMatch25 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch26 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::di));
                    }
                }
                *parentMatch26 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch27 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::bp));
                    }
                }
                *parentMatch27 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch28 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::sp));
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
        case R8W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch29 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R8W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r8w));
                    }
                }
                *parentMatch29 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R9W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch30 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R9W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r9w));
                    }
                }
                *parentMatch30 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R10W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch31 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R10W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r10w));
                    }
                }
                *parentMatch31 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R11W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch32 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R11W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r11w));
                    }
                }
                *parentMatch32 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R12W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch33 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R12W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r12w));
                    }
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R13W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch34 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R13W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r13w));
                    }
                }
                *parentMatch34 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R14W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch35 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R14W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r14w));
                    }
                }
                *parentMatch35 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R15W:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch36 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R15W)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r15w));
                    }
                }
                *parentMatch36 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EAX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EAX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::eax));
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
        case EBX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch38 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EBX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ebx));
                    }
                }
                *parentMatch38 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ECX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch39 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ECX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ecx));
                    }
                }
                *parentMatch39 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EDX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch40 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EDX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::edx));
                    }
                }
                *parentMatch40 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ESI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch41 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ESI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::esi));
                    }
                }
                *parentMatch41 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EDI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch42 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EDI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::edi));
                    }
                }
                *parentMatch42 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case EBP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch43 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == EBP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::ebp));
                    }
                }
                *parentMatch43 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ESP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch44 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ESP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::esp));
                    }
                }
                *parentMatch44 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R8D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch45 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R8D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r8d));
                    }
                }
                *parentMatch45 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R9D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch46 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R9D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r9d));
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
        case R10D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch47 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R10D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r10d));
                    }
                }
                *parentMatch47 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R11D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch48 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R11D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r11d));
                    }
                }
                *parentMatch48 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R12D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch49 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R12D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r12d));
                    }
                }
                *parentMatch49 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R13D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch50 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R13D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r13d));
                    }
                }
                *parentMatch50 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R14D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch51 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R14D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r14d));
                    }
                }
                *parentMatch51 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R15D:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch52 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R15D)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r15d));
                    }
                }
                *parentMatch52 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RAX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch53 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RAX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rax));
                    }
                }
                *parentMatch53 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RBX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch54 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RBX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rbx));
                    }
                }
                *parentMatch54 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RCX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch55 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RCX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rcx));
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
        case RDX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch56 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RDX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rdx));
                    }
                }
                *parentMatch56 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RSI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch57 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RSI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rsi));
                    }
                }
                *parentMatch57 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RDI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch58 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RDI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rdi));
                    }
                }
                *parentMatch58 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RBP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch59 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RBP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rbp));
                    }
                }
                *parentMatch59 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case RSP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch60 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == RSP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::rsp));
                    }
                }
                *parentMatch60 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R8:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch61 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R8)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r8));
                    }
                }
                *parentMatch61 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R9:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch62 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R9)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r9));
                    }
                }
                *parentMatch62 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R10:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch63 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R10)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r10));
                    }
                }
                *parentMatch63 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R11:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch64 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R11)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r11));
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
        case R12:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch65 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R12)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r12));
                    }
                }
                *parentMatch65 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R13:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch66 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R13)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r13));
                    }
                }
                *parentMatch66 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R14:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch67 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R14)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r14));
                    }
                }
                *parentMatch67 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case R15:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch68 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == R15)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::r15));
                    }
                }
                *parentMatch68 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM0:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch69 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM0)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm0));
                    }
                }
                *parentMatch69 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM1:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch70 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM1)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm1));
                    }
                }
                *parentMatch70 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM2:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch71 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM2)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm2));
                    }
                }
                *parentMatch71 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM3:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch72 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM3)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm3));
                    }
                }
                *parentMatch72 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM4:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch73 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM4)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm4));
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
        case XMM5:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch74 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM5)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm5));
                    }
                }
                *parentMatch74 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM6:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch75 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM6)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm6));
                    }
                }
                *parentMatch75 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM7:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch76 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM7)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm7));
                    }
                }
                *parentMatch76 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM8:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch77 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM8)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm8));
                    }
                }
                *parentMatch77 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM9:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch78 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM9)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm9));
                    }
                }
                *parentMatch78 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM10:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch79 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM10)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm10));
                    }
                }
                *parentMatch79 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM11:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch80 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM11)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm11));
                    }
                }
                *parentMatch80 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM12:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch81 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM12)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm12));
                    }
                }
                *parentMatch81 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM13:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch82 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM13)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm13));
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
        case XMM14:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch83 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM14)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm14));
                    }
                }
                *parentMatch83 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case XMM15:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch84 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == XMM15)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::Register>(cmajor::sbin::machine_x64::Register::xmm15));
                    }
                }
                *parentMatch84 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Register");
        else soul::lexer::WriteFailureToLog(lexer, "Register");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Value(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364126);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case HEXNUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == HEXNUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, new cmajor::sbin::assembly::HexNumberNode(lexer.GetSpan(pos), cmajor::sbin::assembly::ParseHexNumber(util::ToUtf8(lexer.GetToken(pos).ToString()))));
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
        case REAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == REAL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, new cmajor::sbin::assembly::RealNode(lexer.GetSpan(pos), cmajor::sbin::assembly::ParseReal(util::ToUtf8(lexer.GetToken(pos).ToString()))));
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
        case INTEGER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, new cmajor::sbin::assembly::IntegerNode(lexer.GetSpan(pos), cmajor::sbin::assembly::ParseInteger(util::ToUtf8(lexer.GetToken(pos).ToString()))));
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
        case STRING:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, new cmajor::sbin::assembly::StringNode(lexer.GetSpan(pos), cmajor::sbin::assembly::ParseString(util::ToUtf8(lexer.GetToken(pos).ToString()))));
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
soul::parser::Match AsmParser<LexerT>::CodeSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CodeSection");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364127);
    std::unique_ptr<cmajor::sbin::assembly::FunctionDefinitionNode> functionDefinition;
    std::unique_ptr<cmajor::sbin::assembly::MacroDefinitionNode> macroAssignmentLine;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match = AsmParser<LexerT>::CodeSectionHeader(lexer);
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(true);
            soul::parser::Match* parentMatch2 = &match;
            {
                while (true)
                {
                    int64_t save = lexer.GetPos();
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch3 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch4 = &match;
                            switch (*lexer)
                            {
                                case SYMBOL:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch5 = &match;
                                    {
                                        int64_t save = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch6 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = AsmParser<LexerT>::FunctionDefinition(lexer);
                                            functionDefinition.reset(static_cast<cmajor::sbin::assembly::FunctionDefinitionNode*>(match.value));
                                            if (match.hit)
                                            {
                                                asmFileNode->AddDefinition(functionDefinition.release());
                                            }
                                            *parentMatch6 = match;
                                        }
                                        *parentMatch5 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch7 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch8 = &match;
                                                {
                                                    int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = AsmParser<LexerT>::MacroAssignmentLine(lexer);
                                                    macroAssignmentLine.reset(static_cast<cmajor::sbin::assembly::MacroDefinitionNode*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        asmFileNode->AddDefinition(macroAssignmentLine.release());
                                                    }
                                                    *parentMatch8 = match;
                                                }
                                                *parentMatch7 = match;
                                            }
                                            *parentMatch5 = match;
                                        }
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch4 = match;
                                    }
                                    break;
                                }
                                case NEWLINE:
                                {
                                    soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
                                    if (match.hit)
                                    {
                                        *parentMatch4 = match;
                                    }
                                    break;
                                }
                            }
                            *parentMatch3 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch2 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                            break;
                        }
                    }
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CodeSection");
        else soul::lexer::WriteFailureToLog(lexer, "CodeSection");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::CodeSectionHeader(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CodeSectionHeader");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364128);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == DOT)
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
                if (*lexer == CODE)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch3 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == NEWLINE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch3 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CodeSectionHeader");
        else soul::lexer::WriteFailureToLog(lexer, "CodeSectionHeader");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::FunctionDefinition(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364129);
    cmajor::sbin::assembly::FunctionDefinitionNode* functionDefinitionNode = nullptr;
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> header;
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> trailer;
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
                        soul::parser::Match match = AsmParser<LexerT>::FunctionHeader(lexer);
                        header.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                        if (match.hit)
                        {
                            functionDefinitionNode = new cmajor::sbin::assembly::FunctionDefinitionNode(lexer.GetSpan(pos), header.release());
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
                        soul::parser::Match match = AsmParser<LexerT>::FunctionBody(lexer, functionDefinitionNode);
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
                        soul::parser::Match match = AsmParser<LexerT>::FunctionTrailer(lexer);
                        trailer.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                        if (match.hit)
                        {
                            functionDefinitionNode->SetEndpSymbol(trailer.release());
                        }
                        *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDefinition");
                #endif
                return soul::parser::Match(true, functionDefinitionNode);
            }
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
soul::parser::Match AsmParser<LexerT>::FunctionHeader(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364130);
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> symbol;
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
                    soul::parser::Match match = AsmParser<LexerT>::Symbol(lexer);
                    symbol.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == PROC)
                        {
                            ++lexer;
                            match.hit = true;
                        }
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
                    if (*lexer == NEWLINE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
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
                return soul::parser::Match(true, symbol.release());
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
soul::parser::Match AsmParser<LexerT>::FunctionTrailer(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionTrailer");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364131);
    std::unique_ptr<cmajor::sbin::assembly::SymbolNode> symbol;
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
                    soul::parser::Match match = AsmParser<LexerT>::Symbol(lexer);
                    symbol.reset(static_cast<cmajor::sbin::assembly::SymbolNode*>(match.value));
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == ENDP)
                        {
                            ++lexer;
                            match.hit = true;
                        }
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
                    if (*lexer == NEWLINE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionTrailer");
                #endif
                return soul::parser::Match(true, symbol.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionTrailer");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionTrailer");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::FunctionBody(LexerT& lexer, cmajor::sbin::assembly::FunctionDefinitionNode* functionDefinitionNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionBody");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364132);
    std::unique_ptr<cmajor::sbin::assembly::InstructionNode> instructionLine;
    std::unique_ptr<cmajor::sbin::assembly::DataDefinitionNode> dataDefinitionLine;
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch3 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch4 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = AsmParser<LexerT>::InstructionLine(lexer);
                                instructionLine.reset(static_cast<cmajor::sbin::assembly::InstructionNode*>(match.value));
                                if (match.hit)
                                {
                                    functionDefinitionNode->AddInstruction(instructionLine.release());
                                }
                                *parentMatch4 = match;
                            }
                            *parentMatch3 = match;
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
                                        soul::parser::Match match = AsmParser<LexerT>::DataDefinitionLine(lexer);
                                        dataDefinitionLine.reset(static_cast<cmajor::sbin::assembly::DataDefinitionNode*>(match.value));
                                        if (match.hit)
                                        {
                                            functionDefinitionNode->AddInstruction(dataDefinitionLine.release());
                                        }
                                        *parentMatch6 = match;
                                    }
                                    *parentMatch5 = match;
                                }
                                *parentMatch3 = match;
                            }
                        }
                        *parentMatch2 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match = AsmParser<LexerT>::EmptyLine(lexer);
                                *parentMatch7 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionBody");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::InstructionLine(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InstructionLine");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364133);
    cmajor::sbin::assembly::InstructionNode* inst = nullptr;
    std::unique_ptr<cmajor::sbin::assembly::LabelNode> label;
    std::unique_ptr<cmajor::sbin::assembly::InstructionNode> instruction;
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
                    soul::parser::Match match(true);
                    int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match = AsmParser<LexerT>::Label(lexer);
                        label.reset(static_cast<cmajor::sbin::assembly::LabelNode*>(match.value));
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
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = AsmParser<LexerT>::Instruction(lexer);
                            instruction.reset(static_cast<cmajor::sbin::assembly::InstructionNode*>(match.value));
                            if (match.hit)
                            {
                                inst = instruction.release();
                                inst->SetLabel(label.release());
                            }
                            *parentMatch6 = match;
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
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == NEWLINE)
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InstructionLine");
                #endif
                return soul::parser::Match(true, inst);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InstructionLine");
        else soul::lexer::WriteFailureToLog(lexer, "InstructionLine");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match AsmParser<LexerT>::Instruction(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364134);
    cmajor::sbin::assembly::InstructionNode* inst = nullptr;
    std::unique_ptr<soul::parser::Value<cmajor::sbin::machine_x64::OpCode>> opCode;
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
                    soul::parser::Match match = AsmParser<LexerT>::OpCode(lexer);
                    opCode.reset(static_cast<soul::parser::Value<cmajor::sbin::machine_x64::OpCode>*>(match.value));
                    if (match.hit)
                    {
                        inst = new InstructionNode(lexer.GetSpan(pos), opCode->value);
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
                        soul::parser::Match match = AsmParser<LexerT>::OperandList(lexer, inst);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                #endif
                return soul::parser::Match(true, inst);
            }
        }
        *parentMatch0 = match;
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
soul::parser::Match AsmParser<LexerT>::OpCode(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OpCode");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4519251244111364135);
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
                if (*lexer == ADD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::ADD));
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
        case ADDSD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ADDSD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::ADDSD));
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
        case ADDSS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == ADDSS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::ADDSS));
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
        case AND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::AND));
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
                soul::parser::Match match(false);
                if (*lexer == CALL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CALL));
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
        case CBW:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CBW)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CBW));
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
        case CDQ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CDQ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CDQ));
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
        case CMP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CMP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CMP));
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
        case COMISD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == COMISD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::COMISD));
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
        case COMISS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == COMISS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::COMISS));
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
        case CQO:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CQO)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CQO));
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
        case CVTSD2SS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTSD2SS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTSD2SS));
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
        case CVTSI2SD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTSI2SD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTSI2SD));
                    }
                }
                *parentMatch13 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CVTSI2SS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch14 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTSI2SS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTSI2SS));
                    }
                }
                *parentMatch14 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CVTSS2SD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch15 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTSS2SD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTSS2SD));
                    }
                }
                *parentMatch15 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CVTTSD2SI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch16 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTTSD2SI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTTSD2SI));
                    }
                }
                *parentMatch16 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CVTTSS2SI:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CVTTSS2SI)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CVTTSS2SI));
                    }
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CWD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch18 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CWD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::CWD));
                    }
                }
                *parentMatch18 = match;
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
            soul::parser::Match* parentMatch19 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::DIV));
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
        case DIVSD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch20 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DIVSD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::DIVSD));
                    }
                }
                *parentMatch20 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case DIVSS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch21 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == DIVSS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::DIVSS));
                    }
                }
                *parentMatch21 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case IDIV:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch22 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == IDIV)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::IDIV));
                    }
                }
                *parentMatch22 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case IMUL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch23 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == IMUL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::IMUL));
                    }
                }
                *parentMatch23 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case JAE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch24 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == JAE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::JAE));
                    }
                }
                *parentMatch24 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case JE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch25 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == JE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::JE));
                    }
                }
                *parentMatch25 = match;
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
            soul::parser::Match* parentMatch26 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::JMP));
                    }
                }
                *parentMatch26 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case JNZ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch27 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == JNZ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::JNZ));
                    }
                }
                *parentMatch27 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case JZ:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch28 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == JZ)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::JZ));
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
        case LEA:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch29 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == LEA)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::LEA));
                    }
                }
                *parentMatch29 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOV:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch30 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOV)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOV));
                    }
                }
                *parentMatch30 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOVSD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch31 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOVSD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOVSD));
                    }
                }
                *parentMatch31 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOVSS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch32 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOVSS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOVSS));
                    }
                }
                *parentMatch32 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOVSX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch33 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOVSX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOVSX));
                    }
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOVSXD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch34 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOVSXD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOVSXD));
                    }
                }
                *parentMatch34 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MOVZX:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch35 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MOVZX)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MOVZX));
                    }
                }
                *parentMatch35 = match;
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
            soul::parser::Match* parentMatch36 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MUL));
                    }
                }
                *parentMatch36 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case MULSD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MULSD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MULSD));
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
        case MULSS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch38 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == MULSS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::MULSS));
                    }
                }
                *parentMatch38 = match;
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
            soul::parser::Match* parentMatch39 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::NEG));
                    }
                }
                *parentMatch39 = match;
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
            soul::parser::Match* parentMatch40 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::NOP));
                    }
                }
                *parentMatch40 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case NOT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch41 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::NOT));
                    }
                }
                *parentMatch41 = match;
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
            soul::parser::Match* parentMatch42 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::OR));
                    }
                }
                *parentMatch42 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case POP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch43 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == POP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::POP));
                    }
                }
                *parentMatch43 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case PUSH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch44 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == PUSH)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::PUSH));
                    }
                }
                *parentMatch44 = match;
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
            soul::parser::Match* parentMatch45 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::RET));
                    }
                }
                *parentMatch45 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch46 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SAR));
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
        case SETC:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch47 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SETC)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SETC));
                    }
                }
                *parentMatch47 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SETE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch48 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SETE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SETE));
                    }
                }
                *parentMatch48 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SETL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch49 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SETL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SETL));
                    }
                }
                *parentMatch49 = match;
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
            soul::parser::Match* parentMatch50 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SHL));
                    }
                }
                *parentMatch50 = match;
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
            soul::parser::Match* parentMatch51 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SHR));
                    }
                }
                *parentMatch51 = match;
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
            soul::parser::Match* parentMatch52 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SUB));
                    }
                }
                *parentMatch52 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SUBSD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch53 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SUBSD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SUBSD));
                    }
                }
                *parentMatch53 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case SUBSS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch54 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == SUBSS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::SUBSS));
                    }
                }
                *parentMatch54 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case UCOMISD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch55 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == UCOMISD)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::UCOMISD));
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
        case UCOMISS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch56 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == UCOMISS)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::UCOMISS));
                    }
                }
                *parentMatch56 = match;
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
            soul::parser::Match* parentMatch57 = &match;
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
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<cmajor::sbin::machine_x64::OpCode>(cmajor::sbin::machine_x64::OpCode::XOR));
                    }
                }
                *parentMatch57 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OpCode");
        else soul::lexer::WriteFailureToLog(lexer, "OpCode");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct AsmParser<soul::lexer::Lexer<cmajor::sbin::assembly::lexer::AssemblyLexer<char32_t>, char32_t>>;

} // namespace cmajor::sbin::assembly::parser
