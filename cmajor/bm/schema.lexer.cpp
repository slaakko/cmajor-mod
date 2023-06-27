// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bm/schema.lexer' using soul lexer generator slg version 4.1.0

module cmajor.binary.message.schema.lexer;

using namespace cmajor::binary::message::schema::token;

namespace cmajor::binary::message::schema::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.binary.message.schema.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(CLASS, "CLASS", "'class'"));
        tokens.AddToken(new soul::ast::slg::Token(ENUM, "ENUM", "'enum'"));
        tokens.AddToken(new soul::ast::slg::Token(BOOL, "BOOL", "'bool'"));
        tokens.AddToken(new soul::ast::slg::Token(SBYTE, "SBYTE", "'sbyte'"));
        tokens.AddToken(new soul::ast::slg::Token(BYTE, "BYTE", "'byte'"));
        tokens.AddToken(new soul::ast::slg::Token(SHORT, "SHORT", "'short'"));
        tokens.AddToken(new soul::ast::slg::Token(USHORT, "USHORT", "'ushort'"));
        tokens.AddToken(new soul::ast::slg::Token(INT, "INT", "'int'"));
        tokens.AddToken(new soul::ast::slg::Token(UINT, "UINT", "'uint'"));
        tokens.AddToken(new soul::ast::slg::Token(LONG, "LONG", "'long'"));
        tokens.AddToken(new soul::ast::slg::Token(ULONG, "ULONG", "'ulong'"));
        tokens.AddToken(new soul::ast::slg::Token(FLOAT, "FLOAT", "'float'"));
        tokens.AddToken(new soul::ast::slg::Token(DOUBLE, "DOUBLE", "'double'"));
        tokens.AddToken(new soul::ast::slg::Token(CHAR, "CHAR", "'char'"));
        tokens.AddToken(new soul::ast::slg::Token(WCHAR, "WCHAR", "'wchar'"));
        tokens.AddToken(new soul::ast::slg::Token(UCHAR, "UCHAR", "'uchar'"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "'string'"));
        tokens.AddToken(new soul::ast::slg::Token(NUMBER, "NUMBER", "'number'"));
        tokens.AddToken(new soul::ast::slg::Token(UUID, "UUID", "'uuid'"));
        tokens.AddToken(new soul::ast::slg::Token(DATE, "DATE", "'date'"));
        tokens.AddToken(new soul::ast::slg::Token(DATETIME, "DATETIME", "'datetime'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(AST, "AST", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(LANGLE, "LANGLE", "'<'"));
        tokens.AddToken(new soul::ast::slg::Token(RANGLE, "RANGLE", "'>'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON_COLON, "COLON_COLON", "'::'"));
        tokens.AddToken(new soul::ast::slg::Token(NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::slg::Token(USING, "USING", "'using'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
    }
    return &tokens;
}

BinaryMessageSchemaLexer_Variables::BinaryMessageSchemaLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "class", CLASS },
         { "enum", ENUM },
         { "bool", BOOL },
         { "sbyte", SBYTE },
         { "byte", BYTE },
         { "short", SHORT },
         { "ushort", USHORT },
         { "int", INT },
         { "uint", UINT },
         { "long", LONG },
         { "ulong", ULONG },
         { "float", FLOAT },
         { "double", DOUBLE },
         { "char", CHAR },
         { "wchar", WCHAR },
         { "uchar", UCHAR },
         { "uuid", UUID },
         { "string", STRING },
         { "number", NUMBER },
         { "date", DATE },
         { "datetime", DATETIME },
         { "namespace", NAMESPACE },
         { "using", USING },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"class", CLASS },
         { u8"enum", ENUM },
         { u8"bool", BOOL },
         { u8"sbyte", SBYTE },
         { u8"byte", BYTE },
         { u8"short", SHORT },
         { u8"ushort", USHORT },
         { u8"int", INT },
         { u8"uint", UINT },
         { u8"long", LONG },
         { u8"ulong", ULONG },
         { u8"float", FLOAT },
         { u8"double", DOUBLE },
         { u8"char", CHAR },
         { u8"wchar", WCHAR },
         { u8"uchar", UCHAR },
         { u8"uuid", UUID },
         { u8"string", STRING },
         { u8"number", NUMBER },
         { u8"date", DATE },
         { u8"datetime", DATETIME },
         { u8"namespace", NAMESPACE },
         { u8"using", USING },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"class", CLASS },
         { u"enum", ENUM },
         { u"bool", BOOL },
         { u"sbyte", SBYTE },
         { u"byte", BYTE },
         { u"short", SHORT },
         { u"ushort", USHORT },
         { u"int", INT },
         { u"uint", UINT },
         { u"long", LONG },
         { u"ulong", ULONG },
         { u"float", FLOAT },
         { u"double", DOUBLE },
         { u"char", CHAR },
         { u"wchar", WCHAR },
         { u"uchar", UCHAR },
         { u"uuid", UUID },
         { u"string", STRING },
         { u"number", NUMBER },
         { u"date", DATE },
         { u"datetime", DATETIME },
         { u"namespace", NAMESPACE },
         { u"using", USING },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"class", CLASS },
         { U"enum", ENUM },
         { U"bool", BOOL },
         { U"sbyte", SBYTE },
         { U"byte", BYTE },
         { U"short", SHORT },
         { U"ushort", USHORT },
         { U"int", INT },
         { U"uint", UINT },
         { U"long", LONG },
         { U"ulong", ULONG },
         { U"float", FLOAT },
         { U"double", DOUBLE },
         { U"char", CHAR },
         { U"wchar", WCHAR },
         { U"uchar", UCHAR },
         { U"uuid", UUID },
         { U"string", STRING },
         { U"number", NUMBER },
         { U"date", DATE },
         { U"datetime", DATETIME },
         { U"namespace", NAMESPACE },
         { U"using", USING },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::binary::message::schema::lexer
