// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/object/expr.lexer' using soul lexer generator slg version 5.0.0

module cmajor.systemx.object.expr.lexer;

using namespace cmajor::systemx::object::expr::token;

namespace cmajor::systemx::object::expr::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.systemx.object.expr.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(INTEGER, "INTEGER", "integer"));
        tokens.AddToken(new soul::ast::slg::Token(DOLLAR, "DOLLAR", "$"));
        tokens.AddToken(new soul::ast::slg::Token(DEREF, "DEREF", "*"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "."));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", ","));
        tokens.AddToken(new soul::ast::slg::Token(ARROW, "ARROW", "->"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "["));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "]"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "("));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", ")"));
    }
    return &tokens;
}

ExprLexer_Variables::ExprLexer_Variables()
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

} // namespace cmajor::systemx::object::expr::lexer
