
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/project.parser' using soul parser generator spg version 5.0.0

export module cmajor.projects.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::projects::parser {

template<typename LexerT>
struct ProjectParser
{
    static std::unique_ptr<cmajor::ast::Project> Parse(LexerT& lexer, std::string config, cmajor::ast::BackEnd backend);
    static soul::parser::Match Project(LexerT& lexer, std::string config, cmajor::ast::BackEnd backend);
    static soul::parser::Match QualifiedId(LexerT& lexer);
    static soul::parser::Match Declaration(LexerT& lexer);
    static soul::parser::Match ReferenceDeclaration(LexerT& lexer);
    static soul::parser::Match SourceFileDeclaration(LexerT& lexer);
    static soul::parser::Match ResourceFileDeclaration(LexerT& lexer);
    static soul::parser::Match ResourceScriptFileDeclaration(LexerT& lexer);
    static soul::parser::Match TextFileDeclaration(LexerT& lexer);
    static soul::parser::Match ActionFileDeclaration(LexerT& lexer);
    static soul::parser::Match TargetDeclaration(LexerT& lexer);
    static soul::parser::Match Target(LexerT& lexer);
};

} // namespace cmajor::projects::parser
