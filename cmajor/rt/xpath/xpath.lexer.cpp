// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/xpath/xpath.lexer' using soul lexer generator slg version 5.0.0

#include <xpath.lexer.hpp>

// module soul.xml.xpath.lexer;

using namespace soul::xml::xpath::token;

namespace soul::xml::xpath::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("soul.xml.xpath.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::slg::Token(AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::slg::Token(EQ, "EQ", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(NEQ, "NEQ", "'!='"));
        tokens.AddToken(new soul::ast::slg::Token(LEQ, "LEQ", "'<='"));
        tokens.AddToken(new soul::ast::slg::Token(GEQ, "GEQ", "'>='"));
        tokens.AddToken(new soul::ast::slg::Token(LESS, "LESS", "'<'"));
        tokens.AddToken(new soul::ast::slg::Token(GREATER, "GREATER", "'>'"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::slg::Token(STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(DIV, "DIV", "'div'"));
        tokens.AddToken(new soul::ast::slg::Token(MOD, "MOD", "'mod'"));
        tokens.AddToken(new soul::ast::slg::Token(UNION, "UNION", "'|'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASH_SLASH, "SLASH_SLASH", "'//'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASH, "SLASH", "'/'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT_DOT, "DOT_DOT", "'..'"));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON_COLON, "COLON_COLON", "'::'"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(ANCESTOR, "ANCESTOR", "'ancestor'"));
        tokens.AddToken(new soul::ast::slg::Token(ANCESTOR_OR_SELF, "ANCESTOR_OR_SELF", "'ancestor-or-self'"));
        tokens.AddToken(new soul::ast::slg::Token(ATTRIBUTE, "ATTRIBUTE", "'attribte'"));
        tokens.AddToken(new soul::ast::slg::Token(CHILD, "CHILD", "'child'"));
        tokens.AddToken(new soul::ast::slg::Token(DESCENDANT, "DESCENDANT", "'descendant"));
        tokens.AddToken(new soul::ast::slg::Token(DESCENDANT_OR_SELF, "DESCENDANT_OR_SELF", "'descendant-or-self'"));
        tokens.AddToken(new soul::ast::slg::Token(FOLLOWING, "FOLLOWING", "'following'"));
        tokens.AddToken(new soul::ast::slg::Token(FOLLOWING_SIBLING, "FOLLOWING_SIBLING", "'following-sibling'"));
        tokens.AddToken(new soul::ast::slg::Token(NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::slg::Token(PARENT, "PARENT", "'parent'"));
        tokens.AddToken(new soul::ast::slg::Token(PRECEDING, "PRECEDING", "'preceding'"));
        tokens.AddToken(new soul::ast::slg::Token(PRECEDING_SIBLING, "PRECEDING_SIBLING", "'preceding-siblibg'"));
        tokens.AddToken(new soul::ast::slg::Token(SELF, "SELF", "'self'"));
        tokens.AddToken(new soul::ast::slg::Token(AT, "AT", "'@'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(PROCESSING_INSTRUCTION, "PROCESSING_INSTRUCTION", "'processing-instructioon'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMENT, "COMMENT", "'comment'"));
        tokens.AddToken(new soul::ast::slg::Token(TEXT, "TEXT", "'text'"));
        tokens.AddToken(new soul::ast::slg::Token(NODE, "NODE", "'node'"));
        tokens.AddToken(new soul::ast::slg::Token(DQ_STRING, "DQ_STRING", "string"));
        tokens.AddToken(new soul::ast::slg::Token(SQ_STRING, "SQ_STRING", "string"));
        tokens.AddToken(new soul::ast::slg::Token(NUMBER, "NUMBER", "number"));
        tokens.AddToken(new soul::ast::slg::Token(NAME, "NAME", "name"));
    }
    return &tokens;
}

XPathLexer_Variables::XPathLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "or", OR },
         { "and", AND },
         { "div", DIV },
         { "mod", MOD },
         { "ancestor", ANCESTOR },
         { "ancestor-or-self", ANCESTOR_OR_SELF },
         { "attribute", ATTRIBUTE },
         { "child", CHILD },
         { "descendant", DESCENDANT },
         { "descendant-or-self", DESCENDANT_OR_SELF },
         { "following", FOLLOWING },
         { "following-sibling", FOLLOWING_SIBLING },
         { "namespace", NAMESPACE },
         { "parent", PARENT },
         { "preceding", PRECEDING },
         { "preceding-sibling", PRECEDING_SIBLING },
         { "self", SELF },
         { "processing-instruction", PROCESSING_INSTRUCTION },
         { "comment", COMMENT },
         { "text", TEXT },
         { "node", NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"or", OR },
         { u8"and", AND },
         { u8"div", DIV },
         { u8"mod", MOD },
         { u8"ancestor", ANCESTOR },
         { u8"ancestor-or-self", ANCESTOR_OR_SELF },
         { u8"attribute", ATTRIBUTE },
         { u8"child", CHILD },
         { u8"descendant", DESCENDANT },
         { u8"descendant-or-self", DESCENDANT_OR_SELF },
         { u8"following", FOLLOWING },
         { u8"following-sibling", FOLLOWING_SIBLING },
         { u8"namespace", NAMESPACE },
         { u8"parent", PARENT },
         { u8"preceding", PRECEDING },
         { u8"preceding-sibling", PRECEDING_SIBLING },
         { u8"self", SELF },
         { u8"processing-instruction", PROCESSING_INSTRUCTION },
         { u8"comment", COMMENT },
         { u8"text", TEXT },
         { u8"node", NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"or", OR },
         { u"and", AND },
         { u"div", DIV },
         { u"mod", MOD },
         { u"ancestor", ANCESTOR },
         { u"ancestor-or-self", ANCESTOR_OR_SELF },
         { u"attribute", ATTRIBUTE },
         { u"child", CHILD },
         { u"descendant", DESCENDANT },
         { u"descendant-or-self", DESCENDANT_OR_SELF },
         { u"following", FOLLOWING },
         { u"following-sibling", FOLLOWING_SIBLING },
         { u"namespace", NAMESPACE },
         { u"parent", PARENT },
         { u"preceding", PRECEDING },
         { u"preceding-sibling", PRECEDING_SIBLING },
         { u"self", SELF },
         { u"processing-instruction", PROCESSING_INSTRUCTION },
         { u"comment", COMMENT },
         { u"text", TEXT },
         { u"node", NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"or", OR },
         { U"and", AND },
         { U"div", DIV },
         { U"mod", MOD },
         { U"ancestor", ANCESTOR },
         { U"ancestor-or-self", ANCESTOR_OR_SELF },
         { U"attribute", ATTRIBUTE },
         { U"child", CHILD },
         { U"descendant", DESCENDANT },
         { U"descendant-or-self", DESCENDANT_OR_SELF },
         { U"following", FOLLOWING },
         { U"following-sibling", FOLLOWING_SIBLING },
         { U"namespace", NAMESPACE },
         { U"parent", PARENT },
         { U"preceding", PRECEDING },
         { U"preceding-sibling", PRECEDING_SIBLING },
         { U"self", SELF },
         { U"processing-instruction", PROCESSING_INSTRUCTION },
         { U"comment", COMMENT },
         { U"text", TEXT },
         { U"node", NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::xml::xpath::lexer
