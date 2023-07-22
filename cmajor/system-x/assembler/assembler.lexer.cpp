// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/system-x/assembler/assembler.lexer' using soul lexer generator slg version 5.0.0

module cmajor.systemx.assembler.lexer;

using namespace cmajor::systemx::assembler::token;

namespace cmajor::systemx::assembler::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.systemx.assembler.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(DECIMAL_CONSTANT, "DECIMAL_CONSTANT", "'decimal constant'"));
        tokens.AddToken(new soul::ast::slg::Token(HEX_CONSTANT, "HEX_CONSTANT", "'hex constant'"));
        tokens.AddToken(new soul::ast::slg::Token(CHAR_CONSTANT, "CHAR_CONSTANT", "'char constant'"));
        tokens.AddToken(new soul::ast::slg::Token(STRING_CONSTANT, "STRING_CONSTANT", "'string constant'"));
        tokens.AddToken(new soul::ast::slg::Token(CLSID_CONSTANT, "CLSID_CONSTANT", "'clsid constant'"));
        tokens.AddToken(new soul::ast::slg::Token(LOCAL_SYMBOL, "LOCAL_SYMBOL", "'local symbol'"));
        tokens.AddToken(new soul::ast::slg::Token(SYMBOL, "SYMBOL", "'symbol'"));
        tokens.AddToken(new soul::ast::slg::Token(AT, "AT", "'at'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'plus'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'minus'"));
        tokens.AddToken(new soul::ast::slg::Token(TILDE, "TILDE", "'tilde'"));
        tokens.AddToken(new soul::ast::slg::Token(DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::slg::Token(AMP, "AMP", "'amp'"));
        tokens.AddToken(new soul::ast::slg::Token(AST, "AST", "'ast'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASHSLASH, "SLASHSLASH", "'slashslash'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASH, "SLASH", "'slash'"));
        tokens.AddToken(new soul::ast::slg::Token(PERCENT, "PERCENT", "'percent'"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFT_LEFT, "SHIFT_LEFT", "'shift left'"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFT_RIGHT, "SHIFT_RIGHT", "'shift right'"));
        tokens.AddToken(new soul::ast::slg::Token(BAR, "BAR", "'bar'"));
        tokens.AddToken(new soul::ast::slg::Token(CARET, "CARET", "'caret'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'dot'"));
        tokens.AddToken(new soul::ast::slg::Token(SPACE, "SPACE", "'space'"));
        tokens.AddToken(new soul::ast::slg::Token(NEWLINE, "NEWLINE", "'newline'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(CHAR, "CHAR", "'char'"));
    }
    return &tokens;
}

AssemblyLexer_Variables::AssemblyLexer_Variables()
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

} // namespace cmajor::systemx::assembler::lexer
