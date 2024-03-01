// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.token.info;

import cmajor.fault.tolerant.ast;
import soul.ast.span;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class SymbolTable;

enum class TokenType
{
    namespace_, type, class_, enum_, interface, typeParameter, parameter, variable, enumMember, function, method, keyword, modifier, comment, string, number, operator_
};

enum class TokenModifiers
{
    none = 0, declaration = 1 << 0, definition = 1 << 1, readonly = 1 << 2, static_ = 1 << 3, abstract = 1 << 4
};

constexpr int tokenModifierBitCount = 4;

constexpr TokenModifiers operator|(TokenModifiers left, TokenModifiers right)
{
    return TokenModifiers(int(left) | int(right));
}

constexpr TokenModifiers operator&(TokenModifiers left, TokenModifiers right)
{
    return TokenModifiers(int(left) & int(right));
}

constexpr TokenModifiers operator~(TokenModifiers modifiers)
{
    return TokenModifiers(~int(modifiers));
}

struct TokenInfo
{
    TokenInfo() : lineColLen(), tokenType(), tokenModifiers() {}
    soul::ast::LineColLen lineColLen;
    TokenType tokenType;
    TokenModifiers tokenModifiers;
};

std::vector<TokenInfo> GetTokenInfo(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit, SymbolTable* symbolTable);

} // namespace cmajor::fault::tolerant::symbols
