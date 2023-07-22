// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/lexer/cmajor.lexer' using soul lexer generator slg version 5.0.0

module cmajor.lexer;

using namespace cmajor::token;

namespace cmajor::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(AUTO, "AUTO", "'auto'"));
        tokens.AddToken(new soul::ast::slg::Token(ASSERT, "ASSERT", "'assert'"));
        tokens.AddToken(new soul::ast::slg::Token(ELIF, "ELIF", "'elif'"));
        tokens.AddToken(new soul::ast::slg::Token(ENDIF, "ENDIF", "'endif'"));
        tokens.AddToken(new soul::ast::slg::Token(BOOL, "BOOL", "'bool'"));
        tokens.AddToken(new soul::ast::slg::Token(TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::slg::Token(FALSE, "FALSE", "'false'"));
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
        tokens.AddToken(new soul::ast::slg::Token(VOID, "VOID", "'void'"));
        tokens.AddToken(new soul::ast::slg::Token(ENUM, "ENUM", "'enum'"));
        tokens.AddToken(new soul::ast::slg::Token(CAST, "CAST", "'cast'"));
        tokens.AddToken(new soul::ast::slg::Token(INTERFACE, "INTERFACE", "'interface'"));
        tokens.AddToken(new soul::ast::slg::Token(NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::slg::Token(USING, "USING", "'using'"));
        tokens.AddToken(new soul::ast::slg::Token(STATIC, "STATIC", "'static'"));
        tokens.AddToken(new soul::ast::slg::Token(EXTERN, "EXTERN", "'extern'"));
        tokens.AddToken(new soul::ast::slg::Token(AS, "AS", "'as'"));
        tokens.AddToken(new soul::ast::slg::Token(IS, "IS", "'is'"));
        tokens.AddToken(new soul::ast::slg::Token(EXPLICIT, "EXPLICIT", "'explicit'"));
        tokens.AddToken(new soul::ast::slg::Token(DELEGATE, "DELEGATE", "'delegate'"));
        tokens.AddToken(new soul::ast::slg::Token(INLINE, "INLINE", "'inline'"));
        tokens.AddToken(new soul::ast::slg::Token(CDECL, "CDECL", "'cdecl'"));
        tokens.AddToken(new soul::ast::slg::Token(NOTHROW, "NOTHROW", "'nothrow'"));
        tokens.AddToken(new soul::ast::slg::Token(PUBLIC, "PUBLIC", "'public'"));
        tokens.AddToken(new soul::ast::slg::Token(PROTECTED, "PROTECTED", "'protected'"));
        tokens.AddToken(new soul::ast::slg::Token(PRIVATE, "PRIVATE", "'private'"));
        tokens.AddToken(new soul::ast::slg::Token(INTERNAL, "INTERNAL", "'internal'"));
        tokens.AddToken(new soul::ast::slg::Token(VIRTUAL, "VIRTUAL", "'virtual'"));
        tokens.AddToken(new soul::ast::slg::Token(ABSTRACT, "ABSTRACT", "'abstract'"));
        tokens.AddToken(new soul::ast::slg::Token(OVERRIDE, "OVERRIDE", "'override'"));
        tokens.AddToken(new soul::ast::slg::Token(SUPPRESS, "SUPPRESS", "'suppress'"));
        tokens.AddToken(new soul::ast::slg::Token(WINAPI, "WINAPI", "'winapi'"));
        tokens.AddToken(new soul::ast::slg::Token(OPERATOR, "OPERATOR", "'operator'"));
        tokens.AddToken(new soul::ast::slg::Token(CLASS, "CLASS", "'class'"));
        tokens.AddToken(new soul::ast::slg::Token(RETURN, "RETURN", "'return'"));
        tokens.AddToken(new soul::ast::slg::Token(IF, "IF", "'if'"));
        tokens.AddToken(new soul::ast::slg::Token(ELSE, "ELSE", "'else'"));
        tokens.AddToken(new soul::ast::slg::Token(SWITCH, "SWITCH", "'switch'"));
        tokens.AddToken(new soul::ast::slg::Token(CASE, "CASE", "'case'"));
        tokens.AddToken(new soul::ast::slg::Token(DEFAULT, "DEFAULT", "'default'"));
        tokens.AddToken(new soul::ast::slg::Token(WHILE, "WHILE", "'while'"));
        tokens.AddToken(new soul::ast::slg::Token(DO, "DO", "'do'"));
        tokens.AddToken(new soul::ast::slg::Token(FOR, "FOR", "'for'"));
        tokens.AddToken(new soul::ast::slg::Token(BREAK, "BREAK", "'break'"));
        tokens.AddToken(new soul::ast::slg::Token(CONTINUE, "CONTINUE", "'continue'"));
        tokens.AddToken(new soul::ast::slg::Token(GOTO, "GOTO", "'goto'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPEDEF, "TYPEDEF", "'typedef'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPENAME, "TYPENAME", "'typename'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPEID, "TYPEID", "'typeid'"));
        tokens.AddToken(new soul::ast::slg::Token(CONST, "CONST", "'const'"));
        tokens.AddToken(new soul::ast::slg::Token(CONSTEXPR, "CONSTEXPR", "'constexpr'"));
        tokens.AddToken(new soul::ast::slg::Token(NULLLIT, "NULLLIT", "'null'"));
        tokens.AddToken(new soul::ast::slg::Token(THIS, "THIS", "'this'"));
        tokens.AddToken(new soul::ast::slg::Token(BASE, "BASE", "'base'"));
        tokens.AddToken(new soul::ast::slg::Token(CONSTRUCT, "CONSTRUCT", "'construct'"));
        tokens.AddToken(new soul::ast::slg::Token(DESTROY, "DESTROY", "'destroy'"));
        tokens.AddToken(new soul::ast::slg::Token(NEW, "NEW", "'new'"));
        tokens.AddToken(new soul::ast::slg::Token(DELETE, "DELETE", "'delete'"));
        tokens.AddToken(new soul::ast::slg::Token(SIZEOF, "SIZEOF", "'sizeof'"));
        tokens.AddToken(new soul::ast::slg::Token(TRY, "TRY", "'try'"));
        tokens.AddToken(new soul::ast::slg::Token(CATCH, "CATCH", "'catch'"));
        tokens.AddToken(new soul::ast::slg::Token(THROW, "THROW", "'throw'"));
        tokens.AddToken(new soul::ast::slg::Token(CONCEPT, "CONCEPT", "'concept'"));
        tokens.AddToken(new soul::ast::slg::Token(WHERE, "WHERE", "'where'"));
        tokens.AddToken(new soul::ast::slg::Token(AXIOM, "AXIOM", "'axiom'"));
        tokens.AddToken(new soul::ast::slg::Token(AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::slg::Token(OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::slg::Token(EXCLAMATION, "EXCLAMATION", "'!'"));
        tokens.AddToken(new soul::ast::slg::Token(UNIT_TEST, "UNIT_TEST", "'unit_test'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(FLOATINGLIT, "FLOATINGLIT", "floating literal"));
        tokens.AddToken(new soul::ast::slg::Token(INTLIT, "INTLIT", "integer literal"));
        tokens.AddToken(new soul::ast::slg::Token(CHARLIT, "CHARLIT", "character literal"));
        tokens.AddToken(new soul::ast::slg::Token(STRINGLIT, "STRINGLIT", "string literal"));
        tokens.AddToken(new soul::ast::slg::Token(EQUIVALENCE, "EQUIVALENCE", "'<=>'"));
        tokens.AddToken(new soul::ast::slg::Token(IMPLICATION, "IMPLICATION", "'=>'"));
        tokens.AddToken(new soul::ast::slg::Token(DISJUNCTION, "DISJUNCTION", "'||'"));
        tokens.AddToken(new soul::ast::slg::Token(AMPAMP, "AMPAMP", "'&&'"));
        tokens.AddToken(new soul::ast::slg::Token(BITOR, "BITOR", "'|'"));
        tokens.AddToken(new soul::ast::slg::Token(BITXOR, "BITXOR", "'^'"));
        tokens.AddToken(new soul::ast::slg::Token(AMP, "AMP", "'&'"));
        tokens.AddToken(new soul::ast::slg::Token(EQ, "EQ", "'=='"));
        tokens.AddToken(new soul::ast::slg::Token(NEQ, "NEQ", "'!='"));
        tokens.AddToken(new soul::ast::slg::Token(LEQ, "LEQ", "'<='"));
        tokens.AddToken(new soul::ast::slg::Token(GEQ, "GEQ", "'>='"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFTLEFT, "SHIFTLEFT", "'<<'"));
        tokens.AddToken(new soul::ast::slg::Token(SHIFTRIGHT, "SHIFTRIGHT", "'>>'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::slg::Token(STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(DIV, "DIV", "'/'"));
        tokens.AddToken(new soul::ast::slg::Token(REM, "REM", "'%'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUSPLUS, "PLUSPLUS", "'++'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUSMINUS, "MINUSMINUS", "'--'"));
        tokens.AddToken(new soul::ast::slg::Token(CPL, "CPL", "'~'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::slg::Token(ARROW, "ARROW", "'->'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(LANGLE, "LANGLE", "'<'"));
        tokens.AddToken(new soul::ast::slg::Token(RANGLE, "RANGLE", "'>'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(HASH, "HASH", "'#'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMENT, "COMMENT", "comment"));
        tokens.AddToken(new soul::ast::slg::Token(WS, "WS", "ws"));
    }
    return &tokens;
}

CmajorLexer_Variables::CmajorLexer_Variables() :
    leftAngleCount(), parsingOperatorFunctionGroupId()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "auto", AUTO },
         { "assert", ASSERT },
         { "elif", ELIF },
         { "endif", ENDIF },
         { "bool", BOOL },
         { "true", TRUE },
         { "false", FALSE },
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
         { "void", VOID },
         { "enum", ENUM },
         { "cast", CAST },
         { "interface", INTERFACE },
         { "namespace", NAMESPACE },
         { "using", USING },
         { "static", STATIC },
         { "extern", EXTERN },
         { "as", AS },
         { "is", IS },
         { "explicit", EXPLICIT },
         { "delegate", DELEGATE },
         { "inline", INLINE },
         { "cdecl", CDECL },
         { "nothrow", NOTHROW },
         { "public", PUBLIC },
         { "protected", PROTECTED },
         { "private", PRIVATE },
         { "internal", INTERNAL },
         { "virtual", VIRTUAL },
         { "abstract", ABSTRACT },
         { "override", OVERRIDE },
         { "suppress", SUPPRESS },
         { "winapi", WINAPI },
         { "operator", OPERATOR },
         { "class", CLASS },
         { "return", RETURN },
         { "if", IF },
         { "else", ELSE },
         { "switch", SWITCH },
         { "case", CASE },
         { "default", DEFAULT },
         { "while", WHILE },
         { "do", DO },
         { "for", FOR },
         { "break", BREAK },
         { "continue", CONTINUE },
         { "goto", GOTO },
         { "typedef", TYPEDEF },
         { "typename", TYPENAME },
         { "typeid", TYPEID },
         { "const", CONST },
         { "constexpr", CONSTEXPR },
         { "null", NULLLIT },
         { "this", THIS },
         { "base", BASE },
         { "construct", CONSTRUCT },
         { "destroy", DESTROY },
         { "new", NEW },
         { "delete", DELETE },
         { "sizeof", SIZEOF },
         { "try", TRY },
         { "catch", CATCH },
         { "throw", THROW },
         { "concept", CONCEPT },
         { "where", WHERE },
         { "axiom", AXIOM },
         { "and", AND },
         { "or", OR },
         { "unit_test", UNIT_TEST },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"auto", AUTO },
         { u8"assert", ASSERT },
         { u8"elif", ELIF },
         { u8"endif", ENDIF },
         { u8"bool", BOOL },
         { u8"true", TRUE },
         { u8"false", FALSE },
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
         { u8"void", VOID },
         { u8"enum", ENUM },
         { u8"cast", CAST },
         { u8"interface", INTERFACE },
         { u8"namespace", NAMESPACE },
         { u8"using", USING },
         { u8"static", STATIC },
         { u8"extern", EXTERN },
         { u8"as", AS },
         { u8"is", IS },
         { u8"explicit", EXPLICIT },
         { u8"delegate", DELEGATE },
         { u8"inline", INLINE },
         { u8"cdecl", CDECL },
         { u8"nothrow", NOTHROW },
         { u8"public", PUBLIC },
         { u8"protected", PROTECTED },
         { u8"private", PRIVATE },
         { u8"internal", INTERNAL },
         { u8"virtual", VIRTUAL },
         { u8"abstract", ABSTRACT },
         { u8"override", OVERRIDE },
         { u8"suppress", SUPPRESS },
         { u8"winapi", WINAPI },
         { u8"operator", OPERATOR },
         { u8"class", CLASS },
         { u8"return", RETURN },
         { u8"if", IF },
         { u8"else", ELSE },
         { u8"switch", SWITCH },
         { u8"case", CASE },
         { u8"default", DEFAULT },
         { u8"while", WHILE },
         { u8"do", DO },
         { u8"for", FOR },
         { u8"break", BREAK },
         { u8"continue", CONTINUE },
         { u8"goto", GOTO },
         { u8"typedef", TYPEDEF },
         { u8"typename", TYPENAME },
         { u8"typeid", TYPEID },
         { u8"const", CONST },
         { u8"constexpr", CONSTEXPR },
         { u8"null", NULLLIT },
         { u8"this", THIS },
         { u8"base", BASE },
         { u8"construct", CONSTRUCT },
         { u8"destroy", DESTROY },
         { u8"new", NEW },
         { u8"delete", DELETE },
         { u8"sizeof", SIZEOF },
         { u8"try", TRY },
         { u8"catch", CATCH },
         { u8"throw", THROW },
         { u8"concept", CONCEPT },
         { u8"where", WHERE },
         { u8"axiom", AXIOM },
         { u8"and", AND },
         { u8"or", OR },
         { u8"unit_test", UNIT_TEST },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"auto", AUTO },
         { u"assert", ASSERT },
         { u"elif", ELIF },
         { u"endif", ENDIF },
         { u"bool", BOOL },
         { u"true", TRUE },
         { u"false", FALSE },
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
         { u"void", VOID },
         { u"enum", ENUM },
         { u"cast", CAST },
         { u"interface", INTERFACE },
         { u"namespace", NAMESPACE },
         { u"using", USING },
         { u"static", STATIC },
         { u"extern", EXTERN },
         { u"as", AS },
         { u"is", IS },
         { u"explicit", EXPLICIT },
         { u"delegate", DELEGATE },
         { u"inline", INLINE },
         { u"cdecl", CDECL },
         { u"nothrow", NOTHROW },
         { u"public", PUBLIC },
         { u"protected", PROTECTED },
         { u"private", PRIVATE },
         { u"internal", INTERNAL },
         { u"virtual", VIRTUAL },
         { u"abstract", ABSTRACT },
         { u"override", OVERRIDE },
         { u"suppress", SUPPRESS },
         { u"winapi", WINAPI },
         { u"operator", OPERATOR },
         { u"class", CLASS },
         { u"return", RETURN },
         { u"if", IF },
         { u"else", ELSE },
         { u"switch", SWITCH },
         { u"case", CASE },
         { u"default", DEFAULT },
         { u"while", WHILE },
         { u"do", DO },
         { u"for", FOR },
         { u"break", BREAK },
         { u"continue", CONTINUE },
         { u"goto", GOTO },
         { u"typedef", TYPEDEF },
         { u"typename", TYPENAME },
         { u"typeid", TYPEID },
         { u"const", CONST },
         { u"constexpr", CONSTEXPR },
         { u"null", NULLLIT },
         { u"this", THIS },
         { u"base", BASE },
         { u"construct", CONSTRUCT },
         { u"destroy", DESTROY },
         { u"new", NEW },
         { u"delete", DELETE },
         { u"sizeof", SIZEOF },
         { u"try", TRY },
         { u"catch", CATCH },
         { u"throw", THROW },
         { u"concept", CONCEPT },
         { u"where", WHERE },
         { u"axiom", AXIOM },
         { u"and", AND },
         { u"or", OR },
         { u"unit_test", UNIT_TEST },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"auto", AUTO },
         { U"assert", ASSERT },
         { U"elif", ELIF },
         { U"endif", ENDIF },
         { U"bool", BOOL },
         { U"true", TRUE },
         { U"false", FALSE },
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
         { U"void", VOID },
         { U"enum", ENUM },
         { U"cast", CAST },
         { U"interface", INTERFACE },
         { U"namespace", NAMESPACE },
         { U"using", USING },
         { U"static", STATIC },
         { U"extern", EXTERN },
         { U"as", AS },
         { U"is", IS },
         { U"explicit", EXPLICIT },
         { U"delegate", DELEGATE },
         { U"inline", INLINE },
         { U"cdecl", CDECL },
         { U"nothrow", NOTHROW },
         { U"public", PUBLIC },
         { U"protected", PROTECTED },
         { U"private", PRIVATE },
         { U"internal", INTERNAL },
         { U"virtual", VIRTUAL },
         { U"abstract", ABSTRACT },
         { U"override", OVERRIDE },
         { U"suppress", SUPPRESS },
         { U"winapi", WINAPI },
         { U"operator", OPERATOR },
         { U"class", CLASS },
         { U"return", RETURN },
         { U"if", IF },
         { U"else", ELSE },
         { U"switch", SWITCH },
         { U"case", CASE },
         { U"default", DEFAULT },
         { U"while", WHILE },
         { U"do", DO },
         { U"for", FOR },
         { U"break", BREAK },
         { U"continue", CONTINUE },
         { U"goto", GOTO },
         { U"typedef", TYPEDEF },
         { U"typename", TYPENAME },
         { U"typeid", TYPEID },
         { U"const", CONST },
         { U"constexpr", CONSTEXPR },
         { U"null", NULLLIT },
         { U"this", THIS },
         { U"base", BASE },
         { U"construct", CONSTRUCT },
         { U"destroy", DESTROY },
         { U"new", NEW },
         { U"delete", DELETE },
         { U"sizeof", SIZEOF },
         { U"try", TRY },
         { U"catch", CATCH },
         { U"throw", THROW },
         { U"concept", CONCEPT },
         { U"where", WHERE },
         { U"axiom", AXIOM },
         { U"and", AND },
         { U"or", OR },
         { U"unit_test", UNIT_TEST },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::lexer
