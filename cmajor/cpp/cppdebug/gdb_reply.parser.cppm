
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/debug/gdb_reply.parser' using soul parser generator spg version 4.1.0

export module cmajor.debug.gdb.reply.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.debug.gdb;

using namespace cmajor::debug::gdb;

export namespace cmajor::debug::gdb::reply::parser {

template<typename LexerT>
struct GdbReplyParser
{
    static std::unique_ptr<cmajor::debug::GdbReplyRecord> Parse(LexerT& lexer);
    static soul::parser::Match GdbReplyRecord(LexerT& lexer);
    static soul::parser::Match GdbResultRecord(LexerT& lexer);
    static soul::parser::Match GdbDoneRecord(LexerT& lexer);
    static soul::parser::Match GdbRunningRecord(LexerT& lexer);
    static soul::parser::Match GdbConnectedRecord(LexerT& lexer);
    static soul::parser::Match GdbErrorRecord(LexerT& lexer);
    static soul::parser::Match GdbExitRecord(LexerT& lexer);
    static soul::parser::Match GdbAsyncRecord(LexerT& lexer);
    static soul::parser::Match GdbExecAsyncRecord(LexerT& lexer);
    static soul::parser::Match GdbExecRunningRecord(LexerT& lexer);
    static soul::parser::Match GdbExecStoppedRecord(LexerT& lexer);
    static soul::parser::Match GdbNotifyAsyncRecord(LexerT& lexer);
    static soul::parser::Match GdbStreamRecord(LexerT& lexer);
    static soul::parser::Match GdbPrompt(LexerT& lexer);
    static soul::parser::Match GdbResults(LexerT& lexer);
    static soul::parser::Match GdbResult(LexerT& lexer);
    static soul::parser::Match GdbVariable(LexerT& lexer);
    static soul::parser::Match GdbValue(LexerT& lexer);
    static soul::parser::Match GdbStringValue(LexerT& lexer);
    static soul::parser::Match GdbTupleValue(LexerT& lexer);
    static soul::parser::Match GdbListValue(LexerT& lexer);
};

} // namespace cmajor::debug::gdb::reply::parser
