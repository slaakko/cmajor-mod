
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/sbin/asm/asm_instruction_line.parser' using soul parser generator spg version 5.0.0

module cmajor.sbin.assembly.instruction.line.parser;

import util;
import soul.ast.spg;
import cmajor.sbin.assembly.parser;
import cmajor.sbin.assembly.lexer;
import cmajor.sbin.assembly.token;

using namespace cmajor::sbin::assembly::parser;
using namespace cmajor::sbin::assembly::lexer;
using namespace cmajor::sbin::assembly::token;

namespace cmajor::sbin::assembly::instruction::line::parser {

template<typename LexerT>
std::unique_ptr<cmajor::sbin::assembly::InstructionNode> AsmInstructionLineParser<LexerT>::Parse(LexerT& lexer)
{
    std::unique_ptr<cmajor::sbin::assembly::InstructionNode> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = AsmInstructionLineParser<LexerT>::InstructionLine(lexer);
    value.reset(static_cast<cmajor::sbin::assembly::InstructionNode*>(match.value));
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
soul::parser::Match AsmInstructionLineParser<LexerT>::InstructionLine(LexerT& lexer)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 5311107904844070913);
    std::unique_ptr<cmajor::sbin::assembly::InstructionNode> inst;
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
                auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                soul::parser::Match match(true);
                if (match.hit)
                {
                    vars->parsingDump = true;
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
                soul::parser::Match match = AsmParser<LexerT>::Instruction(lexer);
                inst.reset(static_cast<cmajor::sbin::assembly::InstructionNode*>(match.value));
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
                soul::parser::Match match(false);
                if (*lexer == NEWLINE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InstructionLine");
                        #endif
                        return soul::parser::Match(true, inst.release());
                    }
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

template struct AsmInstructionLineParser<soul::lexer::Lexer<cmajor::sbin::assembly::lexer::AssemblyLexer<char32_t>, char32_t>>;

} // namespace cmajor::sbin::assembly::instruction::line::parser
