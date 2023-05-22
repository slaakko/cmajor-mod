
// this file has been automatically generated from 'C:/work/soul-mod/soul/rex/rex.parser' using soul parser generator spg version 4.1.0

export module soul.rex.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.rex.context;
import soul.rex.nfa;

using namespace soul::rex::context;
using namespace soul::rex::nfa;

export namespace soul::rex::parser {

template<typename LexerT>
struct RexParser
{
    static soul::rex::nfa::Nfa Parse(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match RegularExpression(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Alternative(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Catenation(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Repetition(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Primary(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Class(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Range(LexerT& lexer, soul::rex::context::Context* context);
    static soul::parser::Match Char(LexerT& lexer);
};

} // namespace soul::rex::parser
