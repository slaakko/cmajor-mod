
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/debugger/reply_parser.parser' using soul parser generator spg version 5.0.0

export module cmajor.debugger.reply.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.debugger.value;
import cmajor.debugger.record;
import cmajor.debugger.reply;

using namespace cmajor::debugger::value;
using namespace cmajor::debugger::record;
using namespace cmajor::debugger::reply;

export namespace cmajor::debugger::reply::parser {

template<typename LexerT>
struct ReplyParser
{
    static std::unique_ptr<cmajor::debugger::Reply> Parse(LexerT& lexer);
    static soul::parser::Match Reply(LexerT& lexer);
    static soul::parser::Match Record(LexerT& lexer);
    static soul::parser::Match Prompt(LexerT& lexer);
    static soul::parser::Match ResultRecord(LexerT& lexer);
    static soul::parser::Match StreamRecord(LexerT& lexer);
    static soul::parser::Match AsyncRecord(LexerT& lexer);
    static soul::parser::Match ExecRecord(LexerT& lexer);
    static soul::parser::Match StatusRecord(LexerT& lexer);
    static soul::parser::Match NotifyRecord(LexerT& lexer);
    static soul::parser::Match Class(LexerT& lexer);
    static soul::parser::Match Results(LexerT& lexer);
    static soul::parser::Match Item(LexerT& lexer);
    static soul::parser::Match Value(LexerT& lexer);
    static soul::parser::Match StringValue(LexerT& lexer);
    static soul::parser::Match TupleValue(LexerT& lexer);
    static soul::parser::Match ListValue(LexerT& lexer);
    static soul::parser::Match Output(LexerT& lexer);
};

} // namespace cmajor::debugger::reply::parser
