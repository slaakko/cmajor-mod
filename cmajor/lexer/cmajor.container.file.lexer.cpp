// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/lexer/cmajor.container.file.lexer' using soul lexer generator slg version 4.1.0

module cmajor.container.file.lexer;

using namespace cmajor::container::file::token;

namespace cmajor::container::file::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("cmajor.container.file.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(PROJECT, "PROJECT", "'project'"));
        tokens.AddToken(new soul::ast::slg::Token(SOLUTION, "SOLUTION", "'solution'"));
        tokens.AddToken(new soul::ast::slg::Token(REFERENCE, "REFERENCE", "'reference'"));
        tokens.AddToken(new soul::ast::slg::Token(SOURCE, "SOURCE", "'source'"));
        tokens.AddToken(new soul::ast::slg::Token(RESOURCE, "RESOURCE", "'resource'"));
        tokens.AddToken(new soul::ast::slg::Token(TEXT, "TEXT", "'text'"));
        tokens.AddToken(new soul::ast::slg::Token(ACTION, "ACTION", "'action'"));
        tokens.AddToken(new soul::ast::slg::Token(TARGET, "TARGET", "'target'"));
        tokens.AddToken(new soul::ast::slg::Token(PROGRAM, "PROGRAM", "'program'"));
        tokens.AddToken(new soul::ast::slg::Token(WINGUIAPP, "WINGUIAPP", "'winguiapp'"));
        tokens.AddToken(new soul::ast::slg::Token(WINAPP, "WINAPP", "'winapp'"));
        tokens.AddToken(new soul::ast::slg::Token(LIBRARY, "LIBRARY", "'library'"));
        tokens.AddToken(new soul::ast::slg::Token(WINLIB, "WINLIB", "'winlib'"));
        tokens.AddToken(new soul::ast::slg::Token(UNITTEST, "UNITTEST", "'unittest'"));
        tokens.AddToken(new soul::ast::slg::Token(ACTIVEPROJECT, "ACTIVEPROJECT", "'activeproject'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(FILEPATH, "FILEPATH", "file path"));
        tokens.AddToken(new soul::ast::slg::Token(ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'dot'"));
    }
    return &tokens;
}

CmajorContainerFileLexer_Variables::CmajorContainerFileLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "project", PROJECT },
         { "solution", SOLUTION },
         { "reference", REFERENCE },
         { "source", SOURCE },
         { "resource", RESOURCE },
         { "text", TEXT },
         { "action", ACTION },
         { "target", TARGET },
         { "program", PROGRAM },
         { "winguiapp", WINGUIAPP },
         { "winapp", WINAPP },
         { "library", LIBRARY },
         { "winlib", WINLIB },
         { "unitTest", UNITTEST },
         { "activeProject", ACTIVEPROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"project", PROJECT },
         { u8"solution", SOLUTION },
         { u8"reference", REFERENCE },
         { u8"source", SOURCE },
         { u8"resource", RESOURCE },
         { u8"text", TEXT },
         { u8"action", ACTION },
         { u8"target", TARGET },
         { u8"program", PROGRAM },
         { u8"winguiapp", WINGUIAPP },
         { u8"winapp", WINAPP },
         { u8"library", LIBRARY },
         { u8"winlib", WINLIB },
         { u8"unitTest", UNITTEST },
         { u8"activeProject", ACTIVEPROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"project", PROJECT },
         { u"solution", SOLUTION },
         { u"reference", REFERENCE },
         { u"source", SOURCE },
         { u"resource", RESOURCE },
         { u"text", TEXT },
         { u"action", ACTION },
         { u"target", TARGET },
         { u"program", PROGRAM },
         { u"winguiapp", WINGUIAPP },
         { u"winapp", WINAPP },
         { u"library", LIBRARY },
         { u"winlib", WINLIB },
         { u"unitTest", UNITTEST },
         { u"activeProject", ACTIVEPROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"project", PROJECT },
         { U"solution", SOLUTION },
         { U"reference", REFERENCE },
         { U"source", SOURCE },
         { U"resource", RESOURCE },
         { U"text", TEXT },
         { U"action", ACTION },
         { U"target", TARGET },
         { U"program", PROGRAM },
         { U"winguiapp", WINGUIAPP },
         { U"winapp", WINAPP },
         { U"library", LIBRARY },
         { U"winlib", WINLIB },
         { U"unitTest", UNITTEST },
         { U"activeProject", ACTIVEPROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace cmajor::container::file::lexer
