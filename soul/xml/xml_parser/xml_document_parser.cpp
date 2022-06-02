
// this file has been automatically generated from 'C:/work/soul-mod/soul/xml/xml_parser/xml_document_parser.parser' using soul parser generator spg version 4.0.0

module soul.xml.document.parser;

import util;
import soul.ast.spg;

namespace soul::xml::document::parser {

static constexpr int32_t s0[] = {60, 63, 120, 109, 108};

static constexpr int32_t s1[] = {63, 62};

static constexpr int32_t s2[] = {118, 101, 114, 115, 105, 111, 110};

static constexpr soul::ast::spg::Range s3[] = {{ 32, 32 }, { 9, 9 }, { 13, 13 }, { 10, 10 }};

static constexpr soul::ast::spg::Range s4[] = {{ 48, 57 }};

static constexpr int32_t s5[] = {101, 110, 99, 111, 100, 105, 110, 103};

static constexpr soul::ast::spg::Range s6[] = {{ 65, 90 }, { 97, 122 }};

static constexpr soul::ast::spg::Range s7[] = {{ 65, 90 }, { 97, 122 }, { 48, 57 }, { 46, 46 }, { 95, 95 }, { 45, 45 }};

static constexpr int32_t s8[] = {115, 116, 97, 110, 100, 97, 108, 111, 110, 101};

static constexpr int32_t s9[] = {34, 121, 101, 115, 34};

static constexpr int32_t s10[] = {39, 121, 101, 115, 39};

static constexpr int32_t s11[] = {34, 110, 111, 34};

static constexpr int32_t s12[] = {39, 110, 111, 39};

static constexpr int32_t s13[] = {60, 33, 45, 45};

static constexpr int32_t s14[] = {45, 45, 62};

static constexpr int32_t s15[] = {45, 45, 62};

static constexpr soul::ast::spg::Range s16[] = {{ 9, 9 }, { 10, 10 }, { 13, 13 }, { 32, 55295 }, { 57344, 65533 }, { 65536, 1114111 }};

static constexpr int32_t s17[] = {60, 63};

static constexpr int32_t s18[] = {63, 62};

static constexpr int32_t s19[] = {63, 62};

static constexpr soul::ast::spg::Range s20[] = {{ 58, 58 }, { 65, 90 }, { 95, 95 }, { 97, 122 }, { 192, 214 }, { 216, 246 }, { 248, 767 }, { 880, 893 }, { 895, 8191 }, { 8204, 8205 }, { 8304, 8591 }, { 11264, 12271 }, { 12289, 55295 }, { 63744, 64975 }, { 65008, 65533 }, { 65536, 983039 }};

static constexpr soul::ast::spg::Range s21[] = {{ 45, 45 }, { 46, 46 }, { 48, 57 }, { 183, 183 }, { 768, 879 }, { 8255, 8256 }};

static constexpr soul::ast::spg::Range s22[] = {{ 120, 120 }, { 88, 88 }};

static constexpr soul::ast::spg::Range s23[] = {{ 109, 109 }, { 77, 77 }};

static constexpr soul::ast::spg::Range s24[] = {{ 108, 108 }, { 76, 76 }};

static constexpr int32_t s25[] = {47, 62};

static constexpr soul::ast::spg::Range s26[] = {{ 60, 60 }, { 38, 38 }, { 34, 34 }};

static constexpr soul::ast::spg::Range s27[] = {{ 60, 60 }, { 38, 38 }, { 39, 39 }};

static constexpr int32_t s28[] = {38, 35};

static constexpr int32_t s29[] = {38, 35, 120};

static constexpr soul::ast::spg::Range s30[] = {{ 48, 57 }};

static constexpr soul::ast::spg::Range s31[] = {{ 48, 57 }, { 97, 102 }, { 65, 70 }};

static constexpr int32_t s32[] = {93, 93, 62};

static constexpr soul::ast::spg::Range s33[] = {{ 60, 60 }, { 38, 38 }};

static constexpr int32_t s34[] = {60, 33, 91, 67, 68, 65, 84, 65, 91};

static constexpr int32_t s35[] = {93, 93, 62};

static constexpr int32_t s36[] = {93, 93, 62};

static constexpr int32_t s37[] = {60, 47};

template<typename Lexer>
void XmlParser<Lexer>::Parse(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    ++lexer;
    soul::parser::Match match = XmlParser<Lexer>::Document(lexer, processor);
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->DecIndent();
        lexer.Log()->WriteEndRule("parse");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
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

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Document(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Document");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953153);
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
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                processor->StartDocument(&lexer, lexer.GetSourcePos(pos));
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
                            soul::parser::Match match = XmlParser<Lexer>::Prolog(lexer, processor);
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
                        soul::parser::Match match = XmlParser<Lexer>::Element(lexer, processor);
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
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match = XmlParser<Lexer>::Misc(lexer, processor);
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->EndDocument();
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Document");
        else soul::lexer::WriteFailureToLog(lexer, "Document");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Prolog(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Prolog");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953154);
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
                int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match = XmlParser<Lexer>::XmlDecl(lexer, processor);
                    if (match.hit)
                    {
                        *parentMatch3 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
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
                                soul::parser::Match match = XmlParser<Lexer>::Misc(lexer, processor);
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
                            soul::parser::Match match = XmlParser<Lexer>::DocTypeDecl(lexer, processor);
                            *parentMatch9 = match;
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
                                            soul::parser::Match match = XmlParser<Lexer>::Misc(lexer, processor);
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Prolog");
        else soul::lexer::WriteFailureToLog(lexer, "Prolog");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::XmlDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "XmlDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953155);
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
                            soul::parser::Match match(true);
                            for (int32_t i : s0)
                            {
                                if (*lexer == i)
                                {
                                    ++lexer;
                                }
                                else
                                {
                                    match.hit = false;
                                    break;
                                }
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match = XmlParser<Lexer>::VersionInfo(lexer, processor);
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
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
                                soul::parser::Match match = XmlParser<Lexer>::EncodingDecl(lexer, processor);
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
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match = XmlParser<Lexer>::StandaloneDecl(lexer, processor);
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match = XmlParser<Lexer>::S(lexer);
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                soul::parser::Match match(true);
                for (int32_t i : s1)
                {
                    if (*lexer == i)
                    {
                        ++lexer;
                    }
                    else
                    {
                        match.hit = false;
                        break;
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "XmlDecl");
        else soul::lexer::WriteFailureToLog(lexer, "XmlDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::VersionInfo(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VersionInfo");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953156);
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
                    soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(true);
                        for (int32_t i : s2)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
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
                    soul::parser::Match match = XmlParser<Lexer>::Eq(lexer);
                    *parentMatch5 = match;
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
                soul::parser::Match match = XmlParser<Lexer>::VersionNumber(lexer, processor);
                *parentMatch6 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VersionInfo");
        else soul::lexer::WriteFailureToLog(lexer, "VersionInfo");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::S(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "S");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953157);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        for (const soul::ast::spg::Range& range : s3)
        {
            if (*lexer >= range.first && *lexer <= range.last)
            {
                match.hit = true;
                ++lexer;
                break;
            }
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch1 = &match;
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                for (const soul::ast::spg::Range& range : s3)
                {
                    if (*lexer >= range.first && *lexer <= range.last)
                    {
                        match.hit = true;
                        ++lexer;
                        break;
                    }
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "S");
        else soul::lexer::WriteFailureToLog(lexer, "S");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Eq(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Eq");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953158);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(true);
            int64_t save = lexer.GetPos();
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                if (match.hit)
                {
                    *parentMatch2 = match;
                }
                else
                {
                    lexer.SetPos(save);
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == 61)
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
            soul::parser::Match match(true);
            int64_t save = lexer.GetPos();
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match = XmlParser<Lexer>::S(lexer);
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
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Eq");
        else soul::lexer::WriteFailureToLog(lexer, "Eq");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::VersionNumber(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VersionNumber");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953159);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match = XmlParser<Lexer>::VersionNumDQ(lexer, processor);
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match = XmlParser<Lexer>::VersionNumSQ(lexer, processor);
                *parentMatch1 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VersionNumber");
        else soul::lexer::WriteFailureToLog(lexer, "VersionNumber");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::VersionNumDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VersionNumDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953160);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == 34)
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
                soul::parser::Match match = XmlParser<Lexer>::VersionNum(lexer, processor);
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
            if (*lexer == 34)
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VersionNumDQ");
        else soul::lexer::WriteFailureToLog(lexer, "VersionNumDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::VersionNumSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VersionNumSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953161);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == 39)
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
                soul::parser::Match match = XmlParser<Lexer>::VersionNum(lexer, processor);
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
            if (*lexer == 39)
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VersionNumSQ");
        else soul::lexer::WriteFailureToLog(lexer, "VersionNumSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::VersionNum(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VersionNum");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953162);
    std::u32string xmlVersion = std::u32string();
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
                        if (*lexer == 49)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            xmlVersion.append(1, '1');
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
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == 46)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                xmlVersion.append(1, '.');
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
                                for (const soul::ast::spg::Range& range : s4)
                                {
                                    if (*lexer >= range.first && *lexer <= range.last)
                                    {
                                        match.hit = true;
                                        ++lexer;
                                        break;
                                    }
                                }
                                if (match.hit)
                                {
                                    xmlVersion.append(1, lexer.GetToken(pos).Chr());
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch8 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch11 = &match;
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        for (const soul::ast::spg::Range& range : s4)
                                        {
                                            if (*lexer >= range.first && *lexer <= range.last)
                                            {
                                                match.hit = true;
                                                ++lexer;
                                                break;
                                            }
                                        }
                                        if (match.hit)
                                        {
                                            xmlVersion.append(1, lexer.GetToken(pos).Chr());
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
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->XmlVersion(util::ToUtf8(xmlVersion));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VersionNum");
        else soul::lexer::WriteFailureToLog(lexer, "VersionNum");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EncodingDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EncodingDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953163);
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
                    soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(true);
                        for (int32_t i : s5)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
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
                    soul::parser::Match match = XmlParser<Lexer>::Eq(lexer);
                    *parentMatch5 = match;
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
                soul::parser::Match match = XmlParser<Lexer>::EncodingName(lexer, processor);
                *parentMatch6 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EncodingDecl");
        else soul::lexer::WriteFailureToLog(lexer, "EncodingDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EncodingName(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EncodingName");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953164);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match = XmlParser<Lexer>::EncNameDQ(lexer, processor);
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match = XmlParser<Lexer>::EncNameSQ(lexer, processor);
                *parentMatch1 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EncodingName");
        else soul::lexer::WriteFailureToLog(lexer, "EncodingName");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EncNameDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EncNameDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953165);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == 34)
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
                soul::parser::Match match = XmlParser<Lexer>::EncName(lexer, processor);
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
            if (*lexer == 34)
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EncNameDQ");
        else soul::lexer::WriteFailureToLog(lexer, "EncNameDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EncNameSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EncNameSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953166);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == 39)
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
                soul::parser::Match match = XmlParser<Lexer>::EncName(lexer, processor);
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
            if (*lexer == 39)
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EncNameSQ");
        else soul::lexer::WriteFailureToLog(lexer, "EncNameSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EncName(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EncName");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953167);
    std::u32string xmlEncoding = std::u32string();
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
                    for (const soul::ast::spg::Range& range : s6)
                    {
                        if (*lexer >= range.first && *lexer <= range.last)
                        {
                            match.hit = true;
                            ++lexer;
                            break;
                        }
                    }
                    if (match.hit)
                    {
                        xmlEncoding.append(1, lexer.GetToken(pos).Chr());
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        for (const soul::ast::spg::Range& range : s7)
                                        {
                                            if (*lexer >= range.first && *lexer <= range.last)
                                            {
                                                match.hit = true;
                                                ++lexer;
                                                break;
                                            }
                                        }
                                        if (match.hit)
                                        {
                                            xmlEncoding.append(1, lexer.GetToken(pos).Chr());
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
            processor->XmlEncoding(util::ToUtf8(xmlEncoding));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EncName");
        else soul::lexer::WriteFailureToLog(lexer, "EncName");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::StandaloneDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StandaloneDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953168);
    std::unique_ptr<soul::parser::Value<bool>> yn;
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
                    soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(true);
                        for (int32_t i : s8)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
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
                    soul::parser::Match match = XmlParser<Lexer>::Eq(lexer);
                    *parentMatch5 = match;
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
                soul::parser::Match* parentMatch7 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = XmlParser<Lexer>::YesNo(lexer);
                    yn.reset(static_cast<soul::parser::Value<bool>*>(match.value));
                    if (match.hit)
                    {
                        processor->Standalone(yn->value);
                    }
                    *parentMatch7 = match;
                }
                *parentMatch6 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StandaloneDecl");
        else soul::lexer::WriteFailureToLog(lexer, "StandaloneDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::YesNo(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "YesNo");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953169);
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
                    soul::parser::Match match(true);
                    for (int32_t i : s9)
                    {
                        if (*lexer == i)
                        {
                            ++lexer;
                        }
                        else
                        {
                            match.hit = false;
                            break;
                        }
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "YesNo");
                            #endif SOUL_PARSER_DEBUG_SUPPORT
                            return soul::parser::Match(true, new soul::parser::Value<bool>(true));
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
                            soul::parser::Match match(true);
                            for (int32_t i : s10)
                            {
                                if (*lexer == i)
                                {
                                    ++lexer;
                                }
                                else
                                {
                                    match.hit = false;
                                    break;
                                }
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "YesNo");
                                    #endif SOUL_PARSER_DEBUG_SUPPORT
                                    return soul::parser::Match(true, new soul::parser::Value<bool>(true));
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
                        soul::parser::Match match(true);
                        for (int32_t i : s11)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "YesNo");
                                #endif SOUL_PARSER_DEBUG_SUPPORT
                                return soul::parser::Match(true, new soul::parser::Value<bool>(false));
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
                    soul::parser::Match match(true);
                    for (int32_t i : s12)
                    {
                        if (*lexer == i)
                        {
                            ++lexer;
                        }
                        else
                        {
                            match.hit = false;
                            break;
                        }
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "YesNo");
                            #endif SOUL_PARSER_DEBUG_SUPPORT
                            return soul::parser::Match(true, new soul::parser::Value<bool>(false));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "YesNo");
        else soul::lexer::WriteFailureToLog(lexer, "YesNo");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Misc(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Misc");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953170);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match = XmlParser<Lexer>::Comment(lexer, processor);
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match = XmlParser<Lexer>::PI(lexer, processor);
                    *parentMatch2 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                *parentMatch3 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Misc");
        else soul::lexer::WriteFailureToLog(lexer, "Misc");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Comment(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Comment");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953171);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::u32string comment = std::u32string();
    std::unique_ptr<soul::parser::Value<char32_t>> chr;
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
                        soul::parser::Match match(true);
                        for (int32_t i : s13)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            sourcePos = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                int64_t save = lexer.GetPos();
                                                {
                                                    soul::parser::Match match = XmlParser<Lexer>::Char(lexer);
                                                    chr.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                                    *parentMatch10 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch11 = &match;
                                                    {
                                                        int64_t tmp = lexer.GetPos();
                                                        lexer.SetPos(save);
                                                        save = tmp;
                                                        soul::parser::Match match(true);
                                                        for (int32_t i : s14)
                                                        {
                                                            if (*lexer == i)
                                                            {
                                                                ++lexer;
                                                            }
                                                            else
                                                            {
                                                                match.hit = false;
                                                                break;
                                                            }
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (!match.hit)
                                                    {
                                                        lexer.SetPos(save);
                                                    }
                                                    *parentMatch10 = soul::parser::Match(!match.hit, match.value);
                                                }
                                                *parentMatch9 = match;
                                            }
                                            if (match.hit)
                                            {
                                                comment.append(1, chr->value);
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
                                        break;
                                    }
                                }
                            }
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
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match(true);
                    for (int32_t i : s15)
                    {
                        if (*lexer == i)
                        {
                            ++lexer;
                        }
                        else
                        {
                            match.hit = false;
                            break;
                        }
                    }
                    *parentMatch12 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->Comment(sourcePos, util::ToUtf8(comment));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Comment");
        else soul::lexer::WriteFailureToLog(lexer, "Comment");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Char(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Char");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953172);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        for (const soul::ast::spg::Range& range : s16)
        {
            if (*lexer >= range.first && *lexer <= range.last)
            {
                match.hit = true;
                ++lexer;
                break;
            }
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<char32_t>(lexer.GetToken(pos).Chr()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Char");
        else soul::lexer::WriteFailureToLog(lexer, "Char");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::PI(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PI");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953173);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::u32string data = std::u32string();
    std::unique_ptr<soul::parser::Value<std::string>> target;
    std::unique_ptr<soul::parser::Value<char32_t>> chr;
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
                            soul::parser::Match match(true);
                            for (int32_t i : s17)
                            {
                                if (*lexer == i)
                                {
                                    ++lexer;
                                }
                                else
                                {
                                    match.hit = false;
                                    break;
                                }
                            }
                            if (match.hit)
                            {
                                sourcePos = lexer.GetSourcePos(pos);
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
                            soul::parser::Match match = XmlParser<Lexer>::PITarget(lexer);
                            target.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                                    *parentMatch10 = match;
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
                                                            int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch15 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch16 = &match;
                                                                int64_t save = lexer.GetPos();
                                                                {
                                                                    soul::parser::Match match = XmlParser<Lexer>::Char(lexer);
                                                                    chr.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                                                    *parentMatch16 = match;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch17 = &match;
                                                                    {
                                                                        int64_t tmp = lexer.GetPos();
                                                                        lexer.SetPos(save);
                                                                        save = tmp;
                                                                        soul::parser::Match match(true);
                                                                        for (int32_t i : s18)
                                                                        {
                                                                            if (*lexer == i)
                                                                            {
                                                                                ++lexer;
                                                                            }
                                                                            else
                                                                            {
                                                                                match.hit = false;
                                                                                break;
                                                                            }
                                                                        }
                                                                        *parentMatch17 = match;
                                                                    }
                                                                    if (!match.hit)
                                                                    {
                                                                        lexer.SetPos(save);
                                                                    }
                                                                    *parentMatch16 = soul::parser::Match(!match.hit, match.value);
                                                                }
                                                                *parentMatch15 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                data.append(1, chr->value);
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
                    for (int32_t i : s19)
                    {
                        if (*lexer == i)
                        {
                            ++lexer;
                        }
                        else
                        {
                            match.hit = false;
                            break;
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
            processor->PI(sourcePos, target->value, util::ToUtf8(data));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PI");
        else soul::lexer::WriteFailureToLog(lexer, "PI");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::PITarget(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PITarget");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953174);
    std::unique_ptr<soul::parser::Value<std::string>> name;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match = XmlParser<Lexer>::Name(lexer);
                name.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t tmp = lexer.GetPos();
                    lexer.SetPos(save);
                    save = tmp;
                    soul::parser::Match match = XmlParser<Lexer>::Xml(lexer);
                    *parentMatch3 = match;
                }
                if (!match.hit)
                {
                    lexer.SetPos(save);
                }
                *parentMatch2 = soul::parser::Match(!match.hit, match.value);
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PITarget");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<std::string>(name->value));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PITarget");
        else soul::lexer::WriteFailureToLog(lexer, "PITarget");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Name(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Name");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953175);
    std::u32string name = std::u32string();
    std::unique_ptr<soul::parser::Value<char32_t>> nameStartChar;
    std::unique_ptr<soul::parser::Value<char32_t>> nameChar;
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
                    soul::parser::Match match = XmlParser<Lexer>::NameStartChar(lexer);
                    nameStartChar.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                    if (match.hit)
                    {
                        name.append(1, nameStartChar->value);
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = XmlParser<Lexer>::NameChar(lexer);
                                        nameChar.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                        if (match.hit)
                                        {
                                            name.append(1, nameChar->value);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Name");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<std::string>(util::ToUtf8(name)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Name");
        else soul::lexer::WriteFailureToLog(lexer, "Name");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::NameStartChar(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NameStartChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953176);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            for (const soul::ast::spg::Range& range : s20)
            {
                if (*lexer >= range.first && *lexer <= range.last)
                {
                    match.hit = true;
                    ++lexer;
                    break;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameStartChar");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<char32_t>(lexer.GetToken(pos).Chr()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameStartChar");
        else soul::lexer::WriteFailureToLog(lexer, "NameStartChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::NameChar(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NameChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953177);
    std::unique_ptr<soul::parser::Value<char32_t>> nameStartChar;
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
                soul::parser::Match match = XmlParser<Lexer>::NameStartChar(lexer);
                nameStartChar.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameChar");
                        #endif SOUL_PARSER_DEBUG_SUPPORT
                        return soul::parser::Match(true, new soul::parser::Value<char32_t>(nameStartChar->value));
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
                        soul::parser::Match match(false);
                        for (const soul::ast::spg::Range& range : s21)
                        {
                            if (*lexer >= range.first && *lexer <= range.last)
                            {
                                match.hit = true;
                                ++lexer;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameChar");
                                #endif SOUL_PARSER_DEBUG_SUPPORT
                                return soul::parser::Match(true, new soul::parser::Value<char32_t>(lexer.GetToken(pos).Chr()));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NameChar");
        else soul::lexer::WriteFailureToLog(lexer, "NameChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Xml(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Xml");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953178);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            for (const soul::ast::spg::Range& range : s22)
            {
                if (*lexer >= range.first && *lexer <= range.last)
                {
                    match.hit = true;
                    ++lexer;
                    break;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                for (const soul::ast::spg::Range& range : s23)
                {
                    if (*lexer >= range.first && *lexer <= range.last)
                    {
                        match.hit = true;
                        ++lexer;
                        break;
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
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch3 = &match;
        {
            soul::parser::Match match(false);
            for (const soul::ast::spg::Range& range : s24)
            {
                if (*lexer >= range.first && *lexer <= range.last)
                {
                    match.hit = true;
                    ++lexer;
                    break;
                }
            }
            *parentMatch3 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Xml");
        else soul::lexer::WriteFailureToLog(lexer, "Xml");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Element(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Element");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953179);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::string tagName = std::string();
    std::unique_ptr<soul::parser::Value<std::string>> name;
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
                        if (*lexer == 60)
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
                                soul::parser::Match match = XmlParser<Lexer>::Name(lexer);
                                name.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                                if (match.hit)
                                {
                                    sourcePos = lexer.GetSourcePos(pos);
                                    tagName = name->value;
                                    processor->BeginStartTag(sourcePos, tagName);
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
                                            soul::parser::Match match = XmlParser<Lexer>::S(lexer);
                                            *parentMatch10 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                soul::parser::Match match = XmlParser<Lexer>::Attribute(lexer, processor);
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match = XmlParser<Lexer>::S(lexer);
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
                        int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(true);
                            for (int32_t i : s25)
                            {
                                if (*lexer == i)
                                {
                                    ++lexer;
                                }
                                else
                                {
                                    match.hit = false;
                                    break;
                                }
                            }
                            if (match.hit)
                            {
                                processor->EndStartTag();
                                processor->EndTag(sourcePos, tagName);
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch16 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch18 = &match;
                            lexer.SetPos(save);
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
                                            soul::parser::Match match(false);
                                            if (*lexer == 62)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                processor->EndStartTag();
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
                                            soul::parser::Match match = XmlParser<Lexer>::Content(lexer, processor);
                                            *parentMatch22 = match;
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch23 = &match;
                                    {
                                        soul::parser::Match match = XmlParser<Lexer>::ETag(lexer, processor);
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch18 = match;
                            }
                            *parentMatch16 = match;
                        }
                    }
                    *parentMatch15 = match;
                }
                *parentMatch14 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Element");
        else soul::lexer::WriteFailureToLog(lexer, "Element");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Attribute(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Attribute");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953180);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<std::string>> attName;
    std::unique_ptr<soul::parser::Value<std::string>> attValue;
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
                        soul::parser::Match match = XmlParser<Lexer>::Name(lexer);
                        attName.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                        if (match.hit)
                        {
                            sourcePos = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match = XmlParser<Lexer>::Eq(lexer);
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
                    soul::parser::Match match = XmlParser<Lexer>::AttValue(lexer, processor);
                    attValue.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->AddAttribute(sourcePos, attName->value, attValue->value);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Attribute");
        else soul::lexer::WriteFailureToLog(lexer, "Attribute");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::AttValue(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AttValue");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953181);
    std::unique_ptr<soul::parser::Value<std::string>> attValueDQ;
    std::unique_ptr<soul::parser::Value<std::string>> attValueSQ;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = XmlParser<Lexer>::AttValueDQ(lexer, processor);
            attValueDQ.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValue");
                    #endif SOUL_PARSER_DEBUG_SUPPORT
                    return soul::parser::Match(true, new soul::parser::Value<std::string>(attValueDQ->value));
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
                    soul::parser::Match match = XmlParser<Lexer>::AttValueSQ(lexer, processor);
                    attValueSQ.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValue");
                            #endif SOUL_PARSER_DEBUG_SUPPORT
                            return soul::parser::Match(true, new soul::parser::Value<std::string>(attValueSQ->value));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValue");
        else soul::lexer::WriteFailureToLog(lexer, "AttValue");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::AttValueDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AttValueDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953182);
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
                        if (*lexer == 34)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            processor->BeginAttributeValue();
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
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(lexer.Pos() != lexer.End());
                                                for (const soul::ast::spg::Range& range : s26)
                                                {
                                                    if (*lexer >= range.first && *lexer <= range.last)
                                                    {
                                                        match.hit = false;
                                                        break;
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    ++lexer;
                                                }
                                                if (match.hit)
                                                {
                                                    processor->AddAttValueChar(lexer.GetToken(pos).Chr());
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match = XmlParser<Lexer>::Reference(lexer, processor);
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch8 = match;
                                            }
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
                                        break;
                                    }
                                }
                            }
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
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == 34)
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
            std::string attValue = processor->AttValue();
            processor->EndAttributeValue();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValueDQ");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<std::string>(attValue));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValueDQ");
        else soul::lexer::WriteFailureToLog(lexer, "AttValueDQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::AttValueSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AttValueSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953183);
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
                        if (*lexer == 39)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            processor->BeginAttributeValue();
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
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(lexer.Pos() != lexer.End());
                                                for (const soul::ast::spg::Range& range : s27)
                                                {
                                                    if (*lexer >= range.first && *lexer <= range.last)
                                                    {
                                                        match.hit = false;
                                                        break;
                                                    }
                                                }
                                                if (match.hit)
                                                {
                                                    ++lexer;
                                                }
                                                if (match.hit)
                                                {
                                                    processor->AddAttValueChar(lexer.GetToken(pos).Chr());
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match = XmlParser<Lexer>::Reference(lexer, processor);
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch8 = match;
                                            }
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
                                        break;
                                    }
                                }
                            }
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
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == 39)
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
            std::string attValue = processor->AttValue();
            processor->EndAttributeValue();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValueSQ");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<std::string>(attValue));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AttValueSQ");
        else soul::lexer::WriteFailureToLog(lexer, "AttValueSQ");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Reference(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Reference");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953184);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match = XmlParser<Lexer>::CharRef(lexer, processor);
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match = XmlParser<Lexer>::EntityRef(lexer, processor);
                *parentMatch1 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Reference");
        else soul::lexer::WriteFailureToLog(lexer, "Reference");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::CharRef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CharRef");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953185);
    int32_t codePoint = int32_t();
    std::unique_ptr<soul::parser::Value<int32_t>> decCodePoint;
    std::unique_ptr<soul::parser::Value<int32_t>> hexCodePoint;
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
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(true);
                        for (int32_t i : s28)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match = XmlParser<Lexer>::DecCodePoint(lexer);
                            decCodePoint.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch7 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == 59)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                codePoint = decCodePoint->value;
                            }
                            *parentMatch7 = match;
                        }
                        *parentMatch6 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                soul::parser::Match match(true);
                                for (int32_t i : s29)
                                {
                                    if (*lexer == i)
                                    {
                                        ++lexer;
                                    }
                                    else
                                    {
                                        match.hit = false;
                                        break;
                                    }
                                }
                                *parentMatch10 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match = XmlParser<Lexer>::HexCodePoint(lexer);
                                    hexCodePoint.reset(static_cast<soul::parser::Value<int32_t>*>(match.value));
                                    *parentMatch11 = match;
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
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
                                    if (*lexer == 59)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        codePoint = hexCodePoint->value;
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->CharRef(lexer.GetSourcePos(pos), static_cast<char32_t>(codePoint));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CharRef");
        else soul::lexer::WriteFailureToLog(lexer, "CharRef");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::DecCodePoint(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DecCodePoint");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953186);
    int32_t val = int32_t();
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
                        for (const soul::ast::spg::Range& range : s30)
                        {
                            if (*lexer >= range.first && *lexer <= range.last)
                            {
                                match.hit = true;
                                ++lexer;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            val = 10 * val + (lexer.GetToken(pos).Chr() - '0');
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                for (const soul::ast::spg::Range& range : s30)
                                {
                                    if (*lexer >= range.first && *lexer <= range.last)
                                    {
                                        match.hit = true;
                                        ++lexer;
                                        break;
                                    }
                                }
                                if (match.hit)
                                {
                                    val = 10 * val + (lexer.GetToken(pos).Chr() - '0');
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
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DecCodePoint");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<int32_t>(val));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DecCodePoint");
        else soul::lexer::WriteFailureToLog(lexer, "DecCodePoint");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::HexCodePoint(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "HexCodePoint");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953187);
    int32_t val = int32_t();
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
                        for (const soul::ast::spg::Range& range : s31)
                        {
                            if (*lexer >= range.first && *lexer <= range.last)
                            {
                                match.hit = true;
                                ++lexer;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            char32_t chr = lexer.GetToken(pos).Chr();
                            if (chr >= '0' && chr <= '9')
                            {
                                val = 16 * val + (chr - '0');
                            }
                            else if (chr >= 'a' && chr <= 'f')
                            {
                                val = 16 * val + 10 + (chr - 'a');
                            }
                            else if (chr >= 'A' && chr <= 'F')
                            {
                                val = 16 * val + 10 + (chr - 'A');
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                for (const soul::ast::spg::Range& range : s31)
                                {
                                    if (*lexer >= range.first && *lexer <= range.last)
                                    {
                                        match.hit = true;
                                        ++lexer;
                                        break;
                                    }
                                }
                                if (match.hit)
                                {
                                    char32_t chr = lexer.GetToken(pos).Chr();
                                    if (chr >= '0' && chr <= '9')
                                    {
                                        val = 16 * val + (chr - '0');
                                    }
                                    else if (chr >= 'a' && chr <= 'f')
                                    {
                                        val = 16 * val + 10 + (chr - 'a');
                                    }
                                    else if (chr >= 'A' && chr <= 'F')
                                    {
                                        val = 16 * val + 10 + (chr - 'A');
                                    }
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
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "HexCodePoint");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<int32_t>(val));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "HexCodePoint");
        else soul::lexer::WriteFailureToLog(lexer, "HexCodePoint");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::EntityRef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EntityRef");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953188);
    std::unique_ptr<soul::parser::Value<std::string>> name;
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
                    if (*lexer == 38)
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
                        soul::parser::Match match = XmlParser<Lexer>::Name(lexer);
                        name.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
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
                    if (*lexer == 59)
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
            processor->EntityRef(lexer.GetSourcePos(pos), name->value);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EntityRef");
        else soul::lexer::WriteFailureToLog(lexer, "EntityRef");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::Content(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Content");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953189);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(true);
            int64_t save = lexer.GetPos();
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match = XmlParser<Lexer>::CharData(lexer, processor);
                if (match.hit)
                {
                    *parentMatch2 = match;
                }
                else
                {
                    lexer.SetPos(save);
                }
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
                                soul::parser::Match* parentMatch6 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
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
                                                        soul::parser::Match match = XmlParser<Lexer>::Element(lexer, processor);
                                                        *parentMatch11 = match;
                                                        if (!match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch12 = &match;
                                                            lexer.SetPos(save);
                                                            {
                                                                soul::parser::Match match = XmlParser<Lexer>::Reference(lexer, processor);
                                                                *parentMatch12 = match;
                                                            }
                                                            *parentMatch11 = match;
                                                        }
                                                    }
                                                    *parentMatch10 = match;
                                                    if (!match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            soul::parser::Match match = XmlParser<Lexer>::CDSect(lexer, processor);
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch10 = match;
                                                    }
                                                }
                                                *parentMatch9 = match;
                                                if (!match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        soul::parser::Match match = XmlParser<Lexer>::PI(lexer, processor);
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch9 = match;
                                                }
                                            }
                                            *parentMatch8 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch15 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match = XmlParser<Lexer>::Comment(lexer, processor);
                                                    *parentMatch15 = match;
                                                }
                                                *parentMatch8 = match;
                                            }
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
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
                                            soul::parser::Match match = XmlParser<Lexer>::CharData(lexer, processor);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Content");
        else soul::lexer::WriteFailureToLog(lexer, "Content");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::CharData(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CharData");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953190);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::u32string text = std::u32string();
    std::unique_ptr<soul::parser::Value<char32_t>> charDataChar;
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
                    soul::parser::Match match(true);
                    if (match.hit)
                    {
                        sourcePos = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    int64_t save = lexer.GetPos();
                                    {
                                        soul::parser::Match match = XmlParser<Lexer>::CharDataChar(lexer);
                                        charDataChar.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t tmp = lexer.GetPos();
                                            lexer.SetPos(save);
                                            save = tmp;
                                            soul::parser::Match match(true);
                                            for (int32_t i : s32)
                                            {
                                                if (*lexer == i)
                                                {
                                                    ++lexer;
                                                }
                                                else
                                                {
                                                    match.hit = false;
                                                    break;
                                                }
                                            }
                                            *parentMatch10 = match;
                                        }
                                        if (!match.hit)
                                        {
                                            lexer.SetPos(save);
                                        }
                                        *parentMatch9 = soul::parser::Match(!match.hit, match.value);
                                    }
                                    *parentMatch8 = match;
                                }
                                if (match.hit)
                                {
                                    text.append(1, charDataChar->value);
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch11 = &match;
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
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch14 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch15 = &match;
                                            int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match = XmlParser<Lexer>::CharDataChar(lexer);
                                                charDataChar.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
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
                                                    soul::parser::Match match(true);
                                                    for (int32_t i : s32)
                                                    {
                                                        if (*lexer == i)
                                                        {
                                                            ++lexer;
                                                        }
                                                        else
                                                        {
                                                            match.hit = false;
                                                            break;
                                                        }
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
                                            text.append(1, charDataChar->value);
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
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->Text(sourcePos, util::ToUtf8(text));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CharData");
        else soul::lexer::WriteFailureToLog(lexer, "CharData");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::CharDataChar(Lexer& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CharDataChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953191);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(lexer.Pos() != lexer.End());
        for (const soul::ast::spg::Range& range : s33)
        {
            if (*lexer >= range.first && *lexer <= range.last)
            {
                match.hit = false;
                break;
            }
        }
        if (match.hit)
        {
            ++lexer;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CharDataChar");
                #endif SOUL_PARSER_DEBUG_SUPPORT
                return soul::parser::Match(true, new soul::parser::Value<char32_t>(lexer.GetToken(pos).Chr()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CharDataChar");
        else soul::lexer::WriteFailureToLog(lexer, "CharDataChar");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::CDSect(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CDSect");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953192);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::u32string cdata = std::u32string();
    std::unique_ptr<soul::parser::Value<char32_t>> chr;
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
                        soul::parser::Match match(true);
                        for (int32_t i : s34)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
                        }
                        if (match.hit)
                        {
                            sourcePos = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match(true);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                int64_t save = lexer.GetPos();
                                                {
                                                    soul::parser::Match match = XmlParser<Lexer>::Char(lexer);
                                                    chr.reset(static_cast<soul::parser::Value<char32_t>*>(match.value));
                                                    *parentMatch10 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch11 = &match;
                                                    {
                                                        int64_t tmp = lexer.GetPos();
                                                        lexer.SetPos(save);
                                                        save = tmp;
                                                        soul::parser::Match match(true);
                                                        for (int32_t i : s35)
                                                        {
                                                            if (*lexer == i)
                                                            {
                                                                ++lexer;
                                                            }
                                                            else
                                                            {
                                                                match.hit = false;
                                                                break;
                                                            }
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    if (!match.hit)
                                                    {
                                                        lexer.SetPos(save);
                                                    }
                                                    *parentMatch10 = soul::parser::Match(!match.hit, match.value);
                                                }
                                                *parentMatch9 = match;
                                            }
                                            if (match.hit)
                                            {
                                                cdata.append(1, chr->value);
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
                                        break;
                                    }
                                }
                            }
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
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match(true);
                    for (int32_t i : s36)
                    {
                        if (*lexer == i)
                        {
                            ++lexer;
                        }
                        else
                        {
                            match.hit = false;
                            break;
                        }
                    }
                    *parentMatch12 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            processor->CDataSection(sourcePos, util::ToUtf8(cdata));
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CDSect");
        else soul::lexer::WriteFailureToLog(lexer, "CDSect");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::ETag(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ETag");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953193);
    soul::ast::SourcePos sourcePos = soul::ast::SourcePos();
    std::unique_ptr<soul::parser::Value<std::string>> name;
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
                        for (int32_t i : s37)
                        {
                            if (*lexer == i)
                            {
                                ++lexer;
                            }
                            else
                            {
                                match.hit = false;
                                break;
                            }
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
                                soul::parser::Match match = XmlParser<Lexer>::Name(lexer);
                                name.reset(static_cast<soul::parser::Value<std::string>*>(match.value));
                                if (match.hit)
                                {
                                    sourcePos = lexer.GetSourcePos(pos);
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
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match = XmlParser<Lexer>::S(lexer);
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
                    soul::parser::Match match(false);
                    if (*lexer == 62)
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
            processor->EndTag(sourcePos, name->value);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ETag");
        else soul::lexer::WriteFailureToLog(lexer, "ETag");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename Lexer>
soul::parser::Match XmlParser<Lexer>::DocTypeDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DocTypeDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    soul::lexer::RuleGuard ruleGuard(lexer, 4556688588539953194);
    soul::parser::Match match(true);
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DocTypeDecl");
        else soul::lexer::WriteFailureToLog(lexer, "DocTypeDecl");
    }
    #endif // SOUL_PARSER_DEBUG_SUPPORT
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct XmlParser<soul::lexer::Lexer<soul::lexer::trivial::TrivialLexer<char32_t>, char32_t>>;

} // namespace soul::xml::document::parser
