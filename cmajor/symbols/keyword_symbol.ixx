export module cmajor.symbols.keyword.symbol;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/Symbol.hpp>
//#include <sngcm/cmnothrowlexer/CmajorNothrowTokens.hpp>
import cmajor.symbols.symbol;
import std.core;

export namespace cmajor::symbols {

//using namespace CmajorNothrowTokens;

class KeywordSymbol : public Symbol
{
public:
    KeywordSymbol(int tokenId_, const std::u32string& name_);
    std::string GetSymbolCategoryStr() const override { return "KW"; }
    std::string GetSymbolCategoryDescription() const override { return "keyword"; }
    std::string TypeString() const override { return "keyword_symbol"; }
    std::u32string FullName() const override { return Name(); }
    std::u32string FullNameWithSpecifiers() const override { return Name(); }
    int TokenId() const { return tokenId; }
private:
    int tokenId;
};

KeywordSymbol* GetKeywordSymbol(int tokenId);

void InitKeywordSymbol();
void DoneKeywordSymbol();
} // namespace cmajor::symbols
