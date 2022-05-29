// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.base;

import soul.lexer.lexeme;
import soul.lexer.token;
import soul.lexer.keyword;
import soul.lexer.classmap;
import soul.lexer.variables;
import soul.ast.slg;

export namespace soul::lexer {

template<typename Char>
class LexerBase
{
public:
    using Self = LexerBase<Char>;
    virtual void Retract() = 0;
    virtual int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const = 0;
    virtual Token<Char, Self>& CurrentToken() = 0;
    virtual const Lexeme<Char>& CurrentLexeme() const = 0;
    virtual const Token<Char, Self>& GetToken(int64_t pos) const = 0;
    virtual const std::string& FileName() const = 0;
    virtual int File() const = 0;
    virtual int Line() const = 0;
    virtual ClassMap<Char>* GetClassMap() const = 0;
    virtual void SetClassMap(ClassMap<Char>* classMap) = 0;
    virtual Variables* GetVariables() const = 0;
    //virtual void SetVariables(Variables* vars) = 0;
    virtual soul::ast::slg::TokenCollection* GetTokenCollection() const = 0;
    virtual void SetTokenCollection(soul::ast::slg::TokenCollection* tokenCollection_) = 0;
    virtual KeywordMap<Char>* GetKeywordMap() const = 0;
    virtual void SetKeywordMap(KeywordMap<Char>* keywordMap_) = 0;
    virtual std::vector<std::string>* GetRuleNameVecPtr() const = 0;
    virtual void SetRuleNameVecPtr(std::vector<std::string>* ruleNameVecPtr) = 0;
};

} // namespace soul::lexer
