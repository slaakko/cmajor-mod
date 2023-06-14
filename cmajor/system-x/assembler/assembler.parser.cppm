
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/system-x/assembler/assembler.parser' using soul parser generator spg version 4.1.0

export module cmajor.systemx.assembler.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.systemx.assembler.assembler;

using namespace cmajor::systemx::assembler::assembler;

export namespace cmajor::systemx::assembler::parser {

template<typename LexerT>
struct AssemblyParser
{
    static void Parse(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match AssemblyFile(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match AssemblyLine(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match CommentLine(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Comment(LexerT& lexer);
    static soul::parser::Match EmptyLine(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match ModeLine(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match InstructionLine(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Instruction(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Label(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match SplitLine(LexerT& lexer, Assembler* assembler, bool hasLabel);
    static soul::parser::Match OpCode(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Operands(LexerT& lexer, Assembler* assembler, cmajor::systemx::assembler::Instruction* instruction);
    static soul::parser::Match Expression(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match WeakOperator(LexerT& lexer);
    static soul::parser::Match Term(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match StrongOperator(LexerT& lexer);
    static soul::parser::Match PrimaryExpression(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match UnaryOperator(LexerT& lexer);
    static soul::parser::Match LocalSymbol(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Constant(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match Symbol(LexerT& lexer, Assembler* assembler);
    static soul::parser::Match At(LexerT& lexer, Assembler* assembler);
};

} // namespace cmajor::systemx::assembler::parser
