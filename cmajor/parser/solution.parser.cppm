
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/solution.parser' using soul parser generator spg version 4.1.0

export module cmajor.solutions.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.ast;

using namespace cmajor::ast;

export namespace cmajor::solutions::parser {

template<typename LexerT>
struct SolutionParser
{
    static std::unique_ptr<cmajor::ast::Solution> Parse(LexerT& lexer);
    static soul::parser::Match Solution(LexerT& lexer);
    static soul::parser::Match QualifiedId(LexerT& lexer);
    static soul::parser::Match Declaration(LexerT& lexer);
    static soul::parser::Match SolutionProjectDeclaration(LexerT& lexer);
    static soul::parser::Match ActiveProjectDeclaration(LexerT& lexer);
};

} // namespace cmajor::solutions::parser