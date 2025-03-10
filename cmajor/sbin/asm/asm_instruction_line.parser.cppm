
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/sbin/asm/asm_instruction_line.parser' using soul parser generator spg version 5.0.0

export module cmajor.sbin.assembly.instruction.line.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.sbin.machine_x64;
import cmajor.sbin.assembly.asm_file;

using namespace cmajor::sbin::machine_x64;
using namespace cmajor::sbin::assembly::asm_file;

export namespace cmajor::sbin::assembly::instruction::line::parser {

template<typename LexerT>
struct AsmInstructionLineParser
{
    static std::unique_ptr<cmajor::sbin::assembly::InstructionNode> Parse(LexerT& lexer);
    static soul::parser::Match InstructionLine(LexerT& lexer);
};

} // namespace cmajor::sbin::assembly::instruction::line::parser
