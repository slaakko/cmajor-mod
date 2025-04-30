// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/intermediate/intermediate_code.lexer' using soul lexer generator slg version 5.0.0

module cmajor.systemx.intermediate.code.lexer;

using namespace cmajor::systemx::intermediate::code::token;

namespace cmajor::systemx::intermediate::code::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.systemx.intermediate.code.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(CU, "CU", "'cu'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(TYPES, "TYPES", "'types'"));
        tokens.AddToken(new soul::ast::slg::Token(DATA, "DATA", "'data'"));
        tokens.AddToken(new soul::ast::slg::Token(FUNCTION, "FUNCTION", "'function'"));
        tokens.AddToken(new soul::ast::slg::Token(ONCE, "ONCE", "'once'"));
        tokens.AddToken(new soul::ast::slg::Token(MAIN, "MAIN", "'main'"));
        tokens.AddToken(new soul::ast::slg::Token(EXTERN, "EXTERN", "'extern'"));
        tokens.AddToken(new soul::ast::slg::Token(INLINE, "INLINE", "'inline'"));
        tokens.AddToken(new soul::ast::slg::Token(STORE, "STORE", "'store'"));
        tokens.AddToken(new soul::ast::slg::Token(ARG, "ARG", "'arg'"));
        tokens.AddToken(new soul::ast::slg::Token(JMP, "JMP", "'jmp'"));
        tokens.AddToken(new soul::ast::slg::Token(BRANCH, "BRANCH", "'branch'"));
        tokens.AddToken(new soul::ast::slg::Token(CALL, "CALL", "'call'"));
        tokens.AddToken(new soul::ast::slg::Token(RET, "RET", "'ret'"));
        tokens.AddToken(new soul::ast::slg::Token(SWITCH, "SWITCH", "'switch'"));
        tokens.AddToken(new soul::ast::slg::Token(NOP, "NOP", "'nop'"));
        tokens.AddToken(new soul::ast::slg::Token(SAVE, "SAVE", "'save'"));
        tokens.AddToken(new soul::ast::slg::Token(NOT, "NOT", "'not'"));
        tokens.AddToken(new soul::ast::slg::Token(NEG, "NEG", "'neg'"));
        tokens.AddToken(new soul::ast::slg::Token(SIGNEXTEND, "SIGNEXTEND", "'signextend'"));
        tokens.AddToken(new soul::ast::slg::Token(ZEROEXTEND, "ZEROEXTEND", "'zeroextend'"));
        tokens.AddToken(new soul::ast::slg::Token(TRUNCATE, "TRUNCATE", "'truncate'"));
        tokens.AddToken(new soul::ast::slg::Token(BITCAST, "BITCAST", "'bitcast'"));
        tokens.AddToken(new soul::ast::slg::Token(INTTOFLOAT, "INTTOFLOAT", "'inttofloat'"));
        tokens.AddToken(new soul::ast::slg::Token(FLOATTOINT, "FLOATTOINT", "'floattoint'"));
        tokens.AddToken(new soul::ast::slg::Token(INTTOPTR, "INTTOPTR", "'inttoptr'"));
        tokens.AddToken(new soul::ast::slg::Token(PTRTOINT, "PTRTOINT", "'ptrtoint'"));
        tokens.AddToken(new soul::ast::slg::Token(ADD, "ADD", "'add'"));
        tokens.AddToken(new soul::ast::slg::Token(SUB, "SUB", "'sub'"));
        tokens.AddToken(new soul::ast::slg::Token(MUL, "MUL", "'mul'"));
        tokens.AddToken(new soul::ast::slg::Token(DIV, "DIV", "'div'"));
        tokens.AddToken(new soul::ast::slg::Token(MOD, "MOD", "'mod'"));
        tokens.AddToken(new soul::ast::slg::Token(AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::slg::Token(OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::slg::Token(XOR, "XOR", "'xor'"));
        tokens.AddToken(new soul::ast::slg::Token(SHL, "SHL", "'shl'"));
        tokens.AddToken(new soul::ast::slg::Token(SHR, "SHR", "'shr'"));
        tokens.AddToken(new soul::ast::slg::Token(EQUAL, "EQUAL", "'equal'"));
        tokens.AddToken(new soul::ast::slg::Token(LESS, "LESS", "'less'"));
        tokens.AddToken(new soul::ast::slg::Token(PARAM, "PARAM", "'param'"));
        tokens.AddToken(new soul::ast::slg::Token(LOCAL, "LOCAL", "'local'"));
        tokens.AddToken(new soul::ast::slg::Token(LOAD, "LOAD", "'load'"));
        tokens.AddToken(new soul::ast::slg::Token(ELEMADDR, "ELEMADDR", "'elemaddr'"));
        tokens.AddToken(new soul::ast::slg::Token(PTROFFSET, "PTROFFSET", "'ptroffset'"));
        tokens.AddToken(new soul::ast::slg::Token(PTRDIFF, "PTRDIFF", "'ptrdiff'"));
        tokens.AddToken(new soul::ast::slg::Token(TRAP, "TRAP", "'trap'"));
        tokens.AddToken(new soul::ast::slg::Token(PHI, "PHI", "'phi'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPE, "TYPE", "'type'"));
        tokens.AddToken(new soul::ast::slg::Token(OFFSET, "OFFSET", "'offset'"));
        tokens.AddToken(new soul::ast::slg::Token(SIZE, "SIZE", "'size'"));
        tokens.AddToken(new soul::ast::slg::Token(ALIGNMENT, "ALIGNMENT", "'alignment'"));
        tokens.AddToken(new soul::ast::slg::Token(VOID, "VOID", "'void'"));
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
        tokens.AddToken(new soul::ast::slg::Token(TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::slg::Token(FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::slg::Token(NULL_TOKEN, "NULL_TOKEN", "'null'"));
        tokens.AddToken(new soul::ast::slg::Token(CONV, "CONV", "'conv'"));
        tokens.AddToken(new soul::ast::slg::Token(METADATA, "METADATA", "'metadata'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::slg::Token(AST, "AST", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(AT, "AT", "'@'"));
        tokens.AddToken(new soul::ast::slg::Token(DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::slg::Token(EXCLAMATION, "EXCLAMATION", "'!'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPEID, "TYPEID", "type identifier"));
        tokens.AddToken(new soul::ast::slg::Token(CLSID, "CLSID", "class identifier"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "string"));
        tokens.AddToken(new soul::ast::slg::Token(NUMBER, "NUMBER", "number"));
        tokens.AddToken(new soul::ast::slg::Token(HEXNUM, "HEXNUM", "hex number"));
    }
    return &tokens;
}

IntermediateCodeLexer_Variables::IntermediateCodeLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "cu", CU },
         { "types", TYPES },
         { "data", DATA },
         { "function", FUNCTION },
         { "once", ONCE },
         { "main_fn", MAIN },
         { "extern", EXTERN },
         { "inline", INLINE },
         { "store", STORE },
         { "arg", ARG },
         { "jmp", JMP },
         { "branch", BRANCH },
         { "call", CALL },
         { "ret", RET },
         { "switch", SWITCH },
         { "nop", NOP },
         { "save", SAVE },
         { "not", NOT },
         { "neg", NEG },
         { "signextend", SIGNEXTEND },
         { "zeroextend", ZEROEXTEND },
         { "truncate", TRUNCATE },
         { "bitcast", BITCAST },
         { "inttofloat", INTTOFLOAT },
         { "floattoint", FLOATTOINT },
         { "inttoptr", INTTOPTR },
         { "ptrtoint", PTRTOINT },
         { "add", ADD },
         { "sub", SUB },
         { "mul", MUL },
         { "div", DIV },
         { "mod", MOD },
         { "and", AND },
         { "or", OR },
         { "xor", XOR },
         { "shl", SHL },
         { "shr", SHR },
         { "equal", EQUAL },
         { "less", LESS },
         { "param", PARAM },
         { "local", LOCAL },
         { "load", LOAD },
         { "elemaddr", ELEMADDR },
         { "ptroffset", PTROFFSET },
         { "ptrdiff", PTRDIFF },
         { "trap", TRAP },
         { "phi", PHI },
         { "type", TYPE },
         { "offset", OFFSET },
         { "size", SIZE },
         { "alignment", ALIGNMENT },
         { "void", VOID },
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
         { "true", TRUE },
         { "false", FALSE },
         { "null", NULL_TOKEN },
         { "conv", CONV },
         { "metadata", METADATA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"cu", CU },
         { u8"types", TYPES },
         { u8"data", DATA },
         { u8"function", FUNCTION },
         { u8"once", ONCE },
         { u8"main_fn", MAIN },
         { u8"extern", EXTERN },
         { u8"inline", INLINE },
         { u8"store", STORE },
         { u8"arg", ARG },
         { u8"jmp", JMP },
         { u8"branch", BRANCH },
         { u8"call", CALL },
         { u8"ret", RET },
         { u8"switch", SWITCH },
         { u8"nop", NOP },
         { u8"save", SAVE },
         { u8"not", NOT },
         { u8"neg", NEG },
         { u8"signextend", SIGNEXTEND },
         { u8"zeroextend", ZEROEXTEND },
         { u8"truncate", TRUNCATE },
         { u8"bitcast", BITCAST },
         { u8"inttofloat", INTTOFLOAT },
         { u8"floattoint", FLOATTOINT },
         { u8"inttoptr", INTTOPTR },
         { u8"ptrtoint", PTRTOINT },
         { u8"add", ADD },
         { u8"sub", SUB },
         { u8"mul", MUL },
         { u8"div", DIV },
         { u8"mod", MOD },
         { u8"and", AND },
         { u8"or", OR },
         { u8"xor", XOR },
         { u8"shl", SHL },
         { u8"shr", SHR },
         { u8"equal", EQUAL },
         { u8"less", LESS },
         { u8"param", PARAM },
         { u8"local", LOCAL },
         { u8"load", LOAD },
         { u8"elemaddr", ELEMADDR },
         { u8"ptroffset", PTROFFSET },
         { u8"ptrdiff", PTRDIFF },
         { u8"trap", TRAP },
         { u8"phi", PHI },
         { u8"type", TYPE },
         { u8"offset", OFFSET },
         { u8"size", SIZE },
         { u8"alignment", ALIGNMENT },
         { u8"void", VOID },
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
         { u8"true", TRUE },
         { u8"false", FALSE },
         { u8"null", NULL_TOKEN },
         { u8"conv", CONV },
         { u8"metadata", METADATA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"cu", CU },
         { u"types", TYPES },
         { u"data", DATA },
         { u"function", FUNCTION },
         { u"once", ONCE },
         { u"main_fn", MAIN },
         { u"extern", EXTERN },
         { u"inline", INLINE },
         { u"store", STORE },
         { u"arg", ARG },
         { u"jmp", JMP },
         { u"branch", BRANCH },
         { u"call", CALL },
         { u"ret", RET },
         { u"switch", SWITCH },
         { u"nop", NOP },
         { u"save", SAVE },
         { u"not", NOT },
         { u"neg", NEG },
         { u"signextend", SIGNEXTEND },
         { u"zeroextend", ZEROEXTEND },
         { u"truncate", TRUNCATE },
         { u"bitcast", BITCAST },
         { u"inttofloat", INTTOFLOAT },
         { u"floattoint", FLOATTOINT },
         { u"inttoptr", INTTOPTR },
         { u"ptrtoint", PTRTOINT },
         { u"add", ADD },
         { u"sub", SUB },
         { u"mul", MUL },
         { u"div", DIV },
         { u"mod", MOD },
         { u"and", AND },
         { u"or", OR },
         { u"xor", XOR },
         { u"shl", SHL },
         { u"shr", SHR },
         { u"equal", EQUAL },
         { u"less", LESS },
         { u"param", PARAM },
         { u"local", LOCAL },
         { u"load", LOAD },
         { u"elemaddr", ELEMADDR },
         { u"ptroffset", PTROFFSET },
         { u"ptrdiff", PTRDIFF },
         { u"trap", TRAP },
         { u"phi", PHI },
         { u"type", TYPE },
         { u"offset", OFFSET },
         { u"size", SIZE },
         { u"alignment", ALIGNMENT },
         { u"void", VOID },
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
         { u"true", TRUE },
         { u"false", FALSE },
         { u"null", NULL_TOKEN },
         { u"conv", CONV },
         { u"metadata", METADATA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"cu", CU },
         { U"types", TYPES },
         { U"data", DATA },
         { U"function", FUNCTION },
         { U"once", ONCE },
         { U"main_fn", MAIN },
         { U"extern", EXTERN },
         { U"inline", INLINE },
         { U"store", STORE },
         { U"arg", ARG },
         { U"jmp", JMP },
         { U"branch", BRANCH },
         { U"call", CALL },
         { U"ret", RET },
         { U"switch", SWITCH },
         { U"nop", NOP },
         { U"save", SAVE },
         { U"not", NOT },
         { U"neg", NEG },
         { U"signextend", SIGNEXTEND },
         { U"zeroextend", ZEROEXTEND },
         { U"truncate", TRUNCATE },
         { U"bitcast", BITCAST },
         { U"inttofloat", INTTOFLOAT },
         { U"floattoint", FLOATTOINT },
         { U"inttoptr", INTTOPTR },
         { U"ptrtoint", PTRTOINT },
         { U"add", ADD },
         { U"sub", SUB },
         { U"mul", MUL },
         { U"div", DIV },
         { U"mod", MOD },
         { U"and", AND },
         { U"or", OR },
         { U"xor", XOR },
         { U"shl", SHL },
         { U"shr", SHR },
         { U"equal", EQUAL },
         { U"less", LESS },
         { U"param", PARAM },
         { U"local", LOCAL },
         { U"load", LOAD },
         { U"elemaddr", ELEMADDR },
         { U"ptroffset", PTROFFSET },
         { U"ptrdiff", PTRDIFF },
         { U"trap", TRAP },
         { U"phi", PHI },
         { U"type", TYPE },
         { U"offset", OFFSET },
         { U"size", SIZE },
         { U"alignment", ALIGNMENT },
         { U"void", VOID },
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
         { U"true", TRUE },
         { U"false", FALSE },
         { U"null", NULL_TOKEN },
         { U"conv", CONV },
         { U"metadata", METADATA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::systemx::intermediate::code::lexer
