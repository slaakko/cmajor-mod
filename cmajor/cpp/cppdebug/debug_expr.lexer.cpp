// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/debug/debug_expr.lexer' using soul lexer generator slg version 4.1.0

module cmajor.debug.expr.lexer;

using namespace cmajor::debug::expr::token;

namespace cmajor::debug::expr::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.debug.expr.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(CAST, "CAST", "'cast'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPEID, "TYPEID", "'typeid'"));
        tokens.AddToken(new soul::ast::slg::Token(BASE, "BASE", "'base'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "id"));
        tokens.AddToken(new soul::ast::slg::Token(INTEGER, "INTEGER", "'integer'"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "'string'"));
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

DebugExprLexer_Variables::DebugExprLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "cast", CAST },
         { "typeid", TYPEID },
         { "base", BASE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"cast", CAST },
         { u8"typeid", TYPEID },
         { u8"base", BASE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"cast", CAST },
         { u"typeid", TYPEID },
         { u"base", BASE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"cast", CAST },
         { U"typeid", TYPEID },
         { U"base", BASE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::debug::expr::lexer
