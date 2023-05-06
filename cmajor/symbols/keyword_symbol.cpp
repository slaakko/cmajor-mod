// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.keyword.symbol;

import cmajor.token;

namespace cmajor::symbols {

KeywordSymbol::KeywordSymbol(int64_t tokenId_, const std::u32string& name_) : Symbol(SymbolType::keywordSymbol, soul::ast::SourcePos(), util::uuid(), name_), tokenId(tokenId_)
{
}

class KeywordSymbols
{
public:
    static KeywordSymbols& Instance();
    KeywordSymbol* GetKeywordSymbol(int64_t tokenId) const;
private:
    KeywordSymbols();
    std::vector<std::unique_ptr<KeywordSymbol>> symbols;
    std::map<int64_t, KeywordSymbol*> symbolMap;
};

KeywordSymbols& KeywordSymbols::Instance()
{
    static KeywordSymbols instance;
    return instance;
}

KeywordSymbols::KeywordSymbols()
{
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::AUTO, U"auto")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ASSERT, U"assert")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ELIF, U"elif")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ENDIF, U"endif")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::BOOL, U"bool")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::TRUE, U"true")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::FALSE, U"false")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::SBYTE, U"sbyte")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::BYTE, U"byte")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::SHORT, U"short")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::USHORT, U"ushort")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::INT, U"int")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::UINT, U"uint")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::LONG, U"long")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ULONG, U"ulong")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::FLOAT, U"float")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DOUBLE, U"double")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CHAR, U"char")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::WCHAR, U"wchar")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::UCHAR, U"uchar")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::VOID, U"void")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ENUM, U"enum")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CAST, U"cast")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::INTERFACE, U"interface")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::NAMESPACE, U"namespace")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::USING, U"using")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::STATIC, U"static")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::EXTERN, U"extern")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::AS, U"as")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::IS, U"is")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::EXPLICIT, U"explicit")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DELEGATE, U"delegate")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::INLINE, U"inline")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CDECL, U"cdecl")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::NOTHROW, U"nothrow")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::PUBLIC, U"public")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::PROTECTED, U"protected")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::PRIVATE, U"private")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::INTERNAL, U"internal")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::VIRTUAL, U"virtual")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ABSTRACT, U"abstract")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::OVERRIDE, U"override")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::SUPPRESS, U"suppress")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::WINAPI, U"winapi")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::OPERATOR, U"operator")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CLASS, U"class")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::RETURN, U"return")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::IF, U"if")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::ELSE, U"else")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::SWITCH, U"switch")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CASE, U"case")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DEFAULT, U"default")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::WHILE, U"while")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DO, U"do")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::FOR, U"for")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::BREAK, U"break")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CONTINUE, U"continue")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::GOTO, U"goto")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::TYPEDEF, U"typedef")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::TYPENAME, U"typename")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::TYPEID, U"typeid")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CONST, U"const")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CONSTEXPR, U"constexpr")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::NULLLIT, U"null")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::THIS, U"this")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::BASE, U"base")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CONSTRUCT, U"construct")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DESTROY, U"destroy")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::NEW, U"new")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::DELETE, U"delete")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::SIZEOF, U"sizeof")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::TRY, U"try")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CATCH, U"catch")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::THROW, U"throw")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::CONCEPT, U"concept")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::WHERE, U"where")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::AXIOM, U"axiom")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::AND, U"and")));
    symbols.push_back(std::unique_ptr<KeywordSymbol>(new KeywordSymbol(cmajor::token::OR, U"or")));
    for (auto& symbol : symbols)
    {
        symbolMap[symbol->TokenId()] = symbol.get();
    }
}

KeywordSymbol* KeywordSymbols::GetKeywordSymbol(int64_t tokenId) const
{
    auto it = symbolMap.find(tokenId);
    if (it != symbolMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

KeywordSymbol* GetKeywordSymbol(int64_t tokenId)
{
    return KeywordSymbols::Instance().GetKeywordSymbol(tokenId);
}

} // namespace cmajor::symbols
