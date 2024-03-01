// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.keyword.symbol;

import cmajor.symbols.symbol;
import std.core;

export namespace cmajor::symbols {

class KeywordSymbol : public Symbol
{
public:
    KeywordSymbol(int64_t tokenId_, const std::u32string& name_);
    std::string GetSymbolCategoryStr() const override { return "KW"; }
    std::string GetSymbolCategoryDescription() const override { return "keyword"; }
    std::string TypeString() const override { return "keyword_symbol"; }
    std::u32string FullName() const override { return Name(); }
    std::u32string FullNameWithSpecifiers() const override { return Name(); }
    int64_t TokenId() const { return tokenId; }
private:
    int64_t tokenId;
};

KeywordSymbol* GetKeywordSymbol(int64_t tokenId);

} // namespace cmajor::symbols
