
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/cppdebug/debug_command.parser' using soul parser generator spg version 5.0.0

export module cmajor.debug.command.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.debug.debugger;

using namespace cmajor::debug::debugger;

export namespace cmajor::debug::command::parser {

template<typename LexerT>
struct DebugCommandParser
{
    static std::unique_ptr<cmajor::debug::DebuggerCommand> Parse(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match DebuggerCommand(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match DebuggerExitCommand(LexerT& lexer);
    static soul::parser::Match DebuggerHelpCommand(LexerT& lexer);
    static soul::parser::Match DebuggerNextCommand(LexerT& lexer);
    static soul::parser::Match DebuggerStepCommand(LexerT& lexer);
    static soul::parser::Match DebuggerContinueCommand(LexerT& lexer);
    static soul::parser::Match DebuggerFinishCommand(LexerT& lexer);
    static soul::parser::Match DebuggerUntilCommand(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match DebuggerBreakCommand(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match DebuggerDeleteCommand(LexerT& lexer);
    static soul::parser::Match DebuggerDepthCommand(LexerT& lexer);
    static soul::parser::Match DebuggerFramesCommand(LexerT& lexer);
    static soul::parser::Match DebuggerShowBreakpointCommand(LexerT& lexer);
    static soul::parser::Match DebuggerShowBreakpointsCommand(LexerT& lexer);
    static soul::parser::Match DebuggerListCommand(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match DebuggerPrintCommand(LexerT& lexer);
    static soul::parser::Match DebuggerSetConditionCommand(LexerT& lexer);
    static soul::parser::Match DebuggerSetBreakOnThrowCommand(LexerT& lexer);
    static soul::parser::Match DebuggerRepeatLatestCommand(LexerT& lexer);
    static soul::parser::Match SourceLocation(LexerT& lexer, std::string* currentSourceFilePath);
    static soul::parser::Match Line(LexerT& lexer);
};

} // namespace cmajor::debug::command::parser
