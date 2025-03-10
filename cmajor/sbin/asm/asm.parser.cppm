
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/sbin/asm/asm.parser' using soul parser generator spg version 5.0.0

export module cmajor.sbin.assembly.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.sbin.machine_x64;
import cmajor.sbin.assembly.asm_file;

using namespace cmajor::sbin::machine_x64;
using namespace cmajor::sbin::assembly::asm_file;

export namespace cmajor::sbin::assembly::parser {

template<typename LexerT>
struct AsmParser
{
    static std::unique_ptr<cmajor::sbin::assembly::AsmFileNode> Parse(LexerT& lexer);
    static soul::parser::Match AsmFile(LexerT& lexer);
    static soul::parser::Match EmptyLine(LexerT& lexer);
    static soul::parser::Match End(LexerT& lexer);
    static soul::parser::Match DeclarationSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode);
    static soul::parser::Match Declaration(LexerT& lexer);
    static soul::parser::Match SymbolKind(LexerT& lexer);
    static soul::parser::Match Symbol(LexerT& lexer);
    static soul::parser::Match TypeField(LexerT& lexer);
    static soul::parser::Match Type(LexerT& lexer);
    static soul::parser::Match DataSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode);
    static soul::parser::Match DataSectionHeader(LexerT& lexer);
    static soul::parser::Match DataDefinitionLine(LexerT& lexer);
    static soul::parser::Match MacroAssignmentLine(LexerT& lexer);
    static soul::parser::Match DataDefinition(LexerT& lexer);
    static soul::parser::Match Label(LexerT& lexer);
    static soul::parser::Match DataInst(LexerT& lexer);
    static soul::parser::Match OperandList(LexerT& lexer, cmajor::sbin::assembly::InstructionBaseNode* instNode);
    static soul::parser::Match Operand(LexerT& lexer);
    static soul::parser::Match Expr(LexerT& lexer);
    static soul::parser::Match AddExpr(LexerT& lexer);
    static soul::parser::Match MulExpr(LexerT& lexer);
    static soul::parser::Match AddOp(LexerT& lexer);
    static soul::parser::Match MulOp(LexerT& lexer);
    static soul::parser::Match UnaryExpr(LexerT& lexer);
    static soul::parser::Match UnaryOp(LexerT& lexer);
    static soul::parser::Match PrimaryExpr(LexerT& lexer);
    static soul::parser::Match ContentExpr(LexerT& lexer);
    static soul::parser::Match SizeExpr(LexerT& lexer);
    static soul::parser::Match SizePrefix(LexerT& lexer);
    static soul::parser::Match ParenthesizedExpr(LexerT& lexer);
    static soul::parser::Match Register(LexerT& lexer);
    static soul::parser::Match Value(LexerT& lexer);
    static soul::parser::Match CodeSection(LexerT& lexer, cmajor::sbin::assembly::AsmFileNode* asmFileNode);
    static soul::parser::Match CodeSectionHeader(LexerT& lexer);
    static soul::parser::Match FunctionDefinition(LexerT& lexer);
    static soul::parser::Match FunctionHeader(LexerT& lexer);
    static soul::parser::Match FunctionTrailer(LexerT& lexer);
    static soul::parser::Match FunctionBody(LexerT& lexer, cmajor::sbin::assembly::FunctionDefinitionNode* functionDefinitionNode);
    static soul::parser::Match InstructionLine(LexerT& lexer);
    static soul::parser::Match Instruction(LexerT& lexer);
    static soul::parser::Match OpCode(LexerT& lexer);
};

} // namespace cmajor::sbin::assembly::parser
