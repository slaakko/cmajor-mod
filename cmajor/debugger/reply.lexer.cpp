// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/debugger/reply.lexer' using soul lexer generator slg version 5.0.0

module cmajor.debugger.reply.lexer;

using namespace cmajor::debugger::reply::token;

namespace cmajor::debugger::reply::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.debugger.reply.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(GDB, "GDB", "'gdb'"));
        tokens.AddToken(new soul::ast::slg::Token(DONE, "DONE", "'done'"));
        tokens.AddToken(new soul::ast::slg::Token(RUNNING, "RUNNING", "'running'"));
        tokens.AddToken(new soul::ast::slg::Token(CONNECTED, "CONNECTED", "'connected'"));
        tokens.AddToken(new soul::ast::slg::Token(ERROR, "ERROR", "'error'"));
        tokens.AddToken(new soul::ast::slg::Token(EXIT, "EXIT", "'exit'"));
        tokens.AddToken(new soul::ast::slg::Token(STOPPED, "STOPPED", "'stopped'"));
        tokens.AddToken(new soul::ast::slg::Token(NAME, "NAME", "name"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "'string'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(EQ, "EQ", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(TILDE, "TILDE", "'~'"));
        tokens.AddToken(new soul::ast::slg::Token(AT, "AT", "'@'"));
        tokens.AddToken(new soul::ast::slg::Token(AMP, "AMP", "'&'"));
        tokens.AddToken(new soul::ast::slg::Token(AST, "AST", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(CARET, "CARET", "'^'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
    }
    return &tokens;
}

ReplyLexer_Variables::ReplyLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "gdb", GDB },
         { "done", DONE },
         { "running", RUNNING },
         { "connected", CONNECTED },
         { "error", ERROR },
         { "exit", EXIT },
         { "stopped", STOPPED },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"gdb", GDB },
         { u8"done", DONE },
         { u8"running", RUNNING },
         { u8"connected", CONNECTED },
         { u8"error", ERROR },
         { u8"exit", EXIT },
         { u8"stopped", STOPPED },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"gdb", GDB },
         { u"done", DONE },
         { u"running", RUNNING },
         { u"connected", CONNECTED },
         { u"error", ERROR },
         { u"exit", EXIT },
         { u"stopped", STOPPED },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"gdb", GDB },
         { U"done", DONE },
         { U"running", RUNNING },
         { U"connected", CONNECTED },
         { U"error", ERROR },
         { U"exit", EXIT },
         { U"stopped", STOPPED },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::debugger::reply::lexer
