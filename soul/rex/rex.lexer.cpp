// this file has been automatically generated from 'C:/work/soul-mod/soul/rex/rex.lexer' using soul lexer generator slg version 4.1.0

module soul.rex.lexer;

using namespace soul::rex::token;

namespace soul::rex::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("soul.rex.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(ALT, "ALT", "'|'"));
        tokens.AddToken(new soul::ast::slg::Token(STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(QUEST, "QUEST", "'?'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::slg::Token(ESCAPE, "ESCAPE", "escape"));
        tokens.AddToken(new soul::ast::slg::Token(INVERSE, "INVERSE", "'^'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::slg::Token(CHAR, "CHAR", "character"));
    }
    return &tokens;
}

RexLexer_Variables::RexLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::rex::lexer
