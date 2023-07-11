// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/debug/debug_command.lexer' using soul lexer generator slg version 4.1.0

module cmajor.debug.command.lexer;

using namespace cmajor::debug::command::token;

namespace cmajor::debug::command::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.debug.command.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(NL, "NL", "'newline'"));
        tokens.AddToken(new soul::ast::slg::Token(EXIT, "EXIT", "'exit'"));
        tokens.AddToken(new soul::ast::slg::Token(E, "E", "'e'"));
        tokens.AddToken(new soul::ast::slg::Token(QUIT, "QUIT", "'quit'"));
        tokens.AddToken(new soul::ast::slg::Token(Q, "Q", "'q'"));
        tokens.AddToken(new soul::ast::slg::Token(HELP, "HELP", "'help'"));
        tokens.AddToken(new soul::ast::slg::Token(H, "H", "'h"));
        tokens.AddToken(new soul::ast::slg::Token(NEXT, "NEXT", "'next'"));
        tokens.AddToken(new soul::ast::slg::Token(N, "N", "'n'"));
        tokens.AddToken(new soul::ast::slg::Token(STEP, "STEP", "'step'"));
        tokens.AddToken(new soul::ast::slg::Token(S, "S", "'s'"));
        tokens.AddToken(new soul::ast::slg::Token(CONTINUE, "CONTINUE", "'continue'"));
        tokens.AddToken(new soul::ast::slg::Token(C, "C", "'c'"));
        tokens.AddToken(new soul::ast::slg::Token(FINISH, "FINISH", "'finish'"));
        tokens.AddToken(new soul::ast::slg::Token(F, "F", "'f'"));
        tokens.AddToken(new soul::ast::slg::Token(UNTIL, "UNTIL", "'until'"));
        tokens.AddToken(new soul::ast::slg::Token(U, "U", "'u'"));
        tokens.AddToken(new soul::ast::slg::Token(BREAK, "BREAK", "'break'"));
        tokens.AddToken(new soul::ast::slg::Token(B, "B", "'b'"));
        tokens.AddToken(new soul::ast::slg::Token(DELETE, "DELETE", "'delete'"));
        tokens.AddToken(new soul::ast::slg::Token(D, "D", "'d'"));
        tokens.AddToken(new soul::ast::slg::Token(PRINT, "PRINT", "'print'"));
        tokens.AddToken(new soul::ast::slg::Token(P, "P", "'p'"));
        tokens.AddToken(new soul::ast::slg::Token(DEPTH, "DEPTH", "'depth'"));
        tokens.AddToken(new soul::ast::slg::Token(FRAMES, "FRAMES", "'frames'"));
        tokens.AddToken(new soul::ast::slg::Token(LIST, "LIST", "'list'"));
        tokens.AddToken(new soul::ast::slg::Token(L, "L", "'l'"));
        tokens.AddToken(new soul::ast::slg::Token(SHOW, "SHOW", "'show'"));
        tokens.AddToken(new soul::ast::slg::Token(BREAKPOINT, "BREAKPOINT", "'breakpoint'"));
        tokens.AddToken(new soul::ast::slg::Token(BREAKPOINTS, "BREAKPOINTS", "'breakpoints'"));
        tokens.AddToken(new soul::ast::slg::Token(CAST, "CAST", "'cast'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPEID, "TYPEID", "'typeid'"));
        tokens.AddToken(new soul::ast::slg::Token(BASE, "BASE", "'base'"));
        tokens.AddToken(new soul::ast::slg::Token(SET, "SET", "'set'"));
        tokens.AddToken(new soul::ast::slg::Token(BREAKONTHROW, "BREAKONTHROW", "'breakOnThrow'"));
        tokens.AddToken(new soul::ast::slg::Token(CONDITION, "CONDITION", "'condition'"));
        tokens.AddToken(new soul::ast::slg::Token(TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::slg::Token(FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "id"));
        tokens.AddToken(new soul::ast::slg::Token(PATH, "PATH", "'path'"));
        tokens.AddToken(new soul::ast::slg::Token(INTEGER, "INTEGER", "'integer'"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "'string'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(AST, "AST", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(AMP, "AMP", "'&'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(LANGLE, "LANGLE", "'<'"));
        tokens.AddToken(new soul::ast::slg::Token(RANGLE, "RANGLE", "'>'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::slg::Token(EQ, "EQ", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(DISJUNCTION, "DISJUNCTION", "'||'"));
        tokens.AddToken(new soul::ast::slg::Token(CONJUNCTION, "CONJUNCTION", "'&&'"));
        tokens.AddToken(new soul::ast::slg::Token(BITOR, "BITOR", "'|'"));
        tokens.AddToken(new soul::ast::slg::Token(BITXOR, "BITXOR", "'^'"));
        tokens.AddToken(new soul::ast::slg::Token(EQUAL, "EQUAL", "'=='"));
        tokens.AddToken(new soul::ast::slg::Token(NOTEQUAL, "NOTEQUAL", "'!='"));
        tokens.AddToken(new soul::ast::slg::Token(LESSEQUAL, "LESSEQUAL", "'<='"));
        tokens.AddToken(new soul::ast::slg::Token(GREATEREQUAL, "GREATEREQUAL", "'>='"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFTLEFT, "SHIFTLEFT", "'<<'"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFTRIGHT, "SHIFTRIGHT", "'>>'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASH, "SLASH", "'/'"));
        tokens.AddToken(new soul::ast::slg::Token(PERCENT, "PERCENT", "'%'"));
    }
    return &tokens;
}

DebugCommandLexer_Variables::DebugCommandLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "exit", EXIT },
         { "e", EXIT },
         { "quit", EXIT },
         { "q", EXIT },
         { "help", HELP },
         { "h", HELP },
         { "next", NEXT },
         { "n", NEXT },
         { "step", STEP },
         { "s", STEP },
         { "continue", CONTINUE },
         { "c", CONTINUE },
         { "finish", FINISH },
         { "f", FINISH },
         { "until", UNTIL },
         { "u", UNTIL },
         { "break", BREAK },
         { "b", BREAK },
         { "delete", DELETE },
         { "d", DELETE },
         { "print", PRINT },
         { "p", PRINT },
         { "depth", DEPTH },
         { "frames", FRAMES },
         { "list", LIST },
         { "l", LIST },
         { "show", SHOW },
         { "breakpoint", BREAKPOINT },
         { "breakpoints", BREAKPOINTS },
         { "cast", CAST },
         { "typeid", TYPEID },
         { "base", BASE },
         { "set", SET },
         { "breakOnThrow", BREAKONTHROW },
         { "condition", CONDITION },
         { "true", TRUE },
         { "false", FALSE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"exit", EXIT },
         { u8"e", EXIT },
         { u8"quit", EXIT },
         { u8"q", EXIT },
         { u8"help", HELP },
         { u8"h", HELP },
         { u8"next", NEXT },
         { u8"n", NEXT },
         { u8"step", STEP },
         { u8"s", STEP },
         { u8"continue", CONTINUE },
         { u8"c", CONTINUE },
         { u8"finish", FINISH },
         { u8"f", FINISH },
         { u8"until", UNTIL },
         { u8"u", UNTIL },
         { u8"break", BREAK },
         { u8"b", BREAK },
         { u8"delete", DELETE },
         { u8"d", DELETE },
         { u8"print", PRINT },
         { u8"p", PRINT },
         { u8"depth", DEPTH },
         { u8"frames", FRAMES },
         { u8"list", LIST },
         { u8"l", LIST },
         { u8"show", SHOW },
         { u8"breakpoint", BREAKPOINT },
         { u8"breakpoints", BREAKPOINTS },
         { u8"cast", CAST },
         { u8"typeid", TYPEID },
         { u8"base", BASE },
         { u8"set", SET },
         { u8"breakOnThrow", BREAKONTHROW },
         { u8"condition", CONDITION },
         { u8"true", TRUE },
         { u8"false", FALSE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"exit", EXIT },
         { u"e", EXIT },
         { u"quit", EXIT },
         { u"q", EXIT },
         { u"help", HELP },
         { u"h", HELP },
         { u"next", NEXT },
         { u"n", NEXT },
         { u"step", STEP },
         { u"s", STEP },
         { u"continue", CONTINUE },
         { u"c", CONTINUE },
         { u"finish", FINISH },
         { u"f", FINISH },
         { u"until", UNTIL },
         { u"u", UNTIL },
         { u"break", BREAK },
         { u"b", BREAK },
         { u"delete", DELETE },
         { u"d", DELETE },
         { u"print", PRINT },
         { u"p", PRINT },
         { u"depth", DEPTH },
         { u"frames", FRAMES },
         { u"list", LIST },
         { u"l", LIST },
         { u"show", SHOW },
         { u"breakpoint", BREAKPOINT },
         { u"breakpoints", BREAKPOINTS },
         { u"cast", CAST },
         { u"typeid", TYPEID },
         { u"base", BASE },
         { u"set", SET },
         { u"breakOnThrow", BREAKONTHROW },
         { u"condition", CONDITION },
         { u"true", TRUE },
         { u"false", FALSE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"exit", EXIT },
         { U"e", EXIT },
         { U"quit", EXIT },
         { U"q", EXIT },
         { U"help", HELP },
         { U"h", HELP },
         { U"next", NEXT },
         { U"n", NEXT },
         { U"step", STEP },
         { U"s", STEP },
         { U"continue", CONTINUE },
         { U"c", CONTINUE },
         { U"finish", FINISH },
         { U"f", FINISH },
         { U"until", UNTIL },
         { U"u", UNTIL },
         { U"break", BREAK },
         { U"b", BREAK },
         { U"delete", DELETE },
         { U"d", DELETE },
         { U"print", PRINT },
         { U"p", PRINT },
         { U"depth", DEPTH },
         { U"frames", FRAMES },
         { U"list", LIST },
         { U"l", LIST },
         { U"show", SHOW },
         { U"breakpoint", BREAKPOINT },
         { U"breakpoints", BREAKPOINTS },
         { U"cast", CAST },
         { U"typeid", TYPEID },
         { U"base", BASE },
         { U"set", SET },
         { U"breakOnThrow", BREAKONTHROW },
         { U"condition", CONDITION },
         { U"true", TRUE },
         { U"false", FALSE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::debug::command::lexer
