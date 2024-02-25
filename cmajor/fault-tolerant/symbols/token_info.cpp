// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.token.info;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

class TokenInfoVisitor : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    TokenInfoVisitor(SymbolTable* symbolTable_, cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit_);
    void VisitIdentifier(cmajor::fault::tolerant::ast::IdentifierNode* identifier) override;
    void VisitOperator(cmajor::fault::tolerant::ast::Node* operatorNode) override;
    void VisitParen(cmajor::fault::tolerant::ast::Node* paren) override;
    void VisitKeyword(cmajor::fault::tolerant::ast::KeywordNode* keyword) override;
    void VisitNumber(cmajor::fault::tolerant::ast::LiteralTokenNode* number) override;
    void VisitChar(cmajor::fault::tolerant::ast::LiteralTokenNode* chr) override;
    void VisitString(cmajor::fault::tolerant::ast::LiteralTokenNode* str) override;
    std::vector<TokenInfo> GetTokenInfos() { return std::move(tokenInfos); }
private:
    SymbolTable* symbolTable;
    cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit;
    std::vector<TokenInfo> tokenInfos;
};

TokenModifiers GetTokenModifiers(cmajor::fault::tolerant::ast::Specifiers specifiers)
{
    TokenModifiers modifiers = TokenModifiers::none;
    if ((specifiers & cmajor::fault::tolerant::ast::Specifiers::static_) != cmajor::fault::tolerant::ast::Specifiers::none)
    {
        modifiers = modifiers | TokenModifiers::static_;
    }
    if ((specifiers & cmajor::fault::tolerant::ast::Specifiers::abstract_) != cmajor::fault::tolerant::ast::Specifiers::none)
    {
        modifiers = modifiers | TokenModifiers::abstract;
    }
    return modifiers;
}

TokenInfoVisitor::TokenInfoVisitor(SymbolTable* symbolTable_, cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit_) : symbolTable(symbolTable_), compileUnit(compileUnit_)
{
}

void TokenInfoVisitor::VisitIdentifier(cmajor::fault::tolerant::ast::IdentifierNode* identifier)
{
    bool set = true;
    TokenInfo tokenInfo;
    Symbol* symbol = symbolTable->GetMappedSymbol(identifier);
    if (!symbol)
    {
        set = false;
    }
    else
    {
        switch (symbol->Kind())
        {
            case SymbolKind::namespaceSymbol:
            {
                tokenInfo.tokenType = TokenType::namespace_;
                tokenInfo.tokenModifiers = TokenModifiers::none;
                break;
            }
            case SymbolKind::aliasTypeSymbol:
            case SymbolKind::delegateSymbol:
            case SymbolKind::classDelegateSymbol:
            case SymbolKind::conceptSymbol:
            {
                tokenInfo.tokenType = TokenType::type;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::constantSymbol:
            {
                tokenInfo.tokenType = TokenType::variable;
                tokenInfo.tokenModifiers = TokenModifiers::readonly;
                break;
            }
            case SymbolKind::enumTypeSymbol:
            {
                tokenInfo.tokenType = TokenType::enum_;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::enumConstantSymbol:
            {
                tokenInfo.tokenType = TokenType::enumMember;
                tokenInfo.tokenModifiers = TokenModifiers::none;
                break;
            }
            case SymbolKind::parameterSymbol:
            {
                tokenInfo.tokenType = TokenType::parameter;
                tokenInfo.tokenModifiers = TokenModifiers::none;
                break;
            }
            case SymbolKind::localVariableSymbol:
            case SymbolKind::memberVariableSymbol:
            case SymbolKind::globalVariableSymbol:
            {
                tokenInfo.tokenType = TokenType::variable;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::typeParameterSymbol: 
            case SymbolKind::templateParameterSymbol:
            {
                tokenInfo.tokenType = TokenType::typeParameter;
                tokenInfo.tokenModifiers = TokenModifiers::none;
                break;
            }
            case SymbolKind::functionSymbol:
            {
                tokenInfo.tokenType = TokenType::function;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::staticConstructorSymbol:
            case SymbolKind::constructorSymbol:
            case SymbolKind::destructorSymbol:
            case SymbolKind::memberFunctionSymbol:
            case SymbolKind::conversionFunctionSymbol:
            {
                tokenInfo.tokenType = TokenType::method;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::classTypeSymbol:
            {
                tokenInfo.tokenType = TokenType::class_;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            case SymbolKind::interfaceTypeSymbol:
            {
                tokenInfo.tokenType = TokenType::interface;
                tokenInfo.tokenModifiers = GetTokenModifiers(symbol->Specifiers());
                break;
            }
            default:
            {
                set = false;
                break;
            }
        }
    }
    if (set)
    {
        tokenInfo.lineColLen = soul::ast::SpanToLineColLen(identifier->GetSpan(), compileUnit->LineStarts());
        tokenInfos.push_back(tokenInfo);
    }
}

void TokenInfoVisitor::VisitOperator(cmajor::fault::tolerant::ast::Node* operatorNode)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(operatorNode->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::operator_;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

void TokenInfoVisitor::VisitParen(cmajor::fault::tolerant::ast::Node* paren)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(paren->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::operator_;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

void TokenInfoVisitor::VisitKeyword(cmajor::fault::tolerant::ast::KeywordNode* keyword)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(keyword->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::keyword;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

void TokenInfoVisitor::VisitNumber(cmajor::fault::tolerant::ast::LiteralTokenNode* number)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(number->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::number;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

void TokenInfoVisitor::VisitChar(cmajor::fault::tolerant::ast::LiteralTokenNode* chr)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(chr->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::string;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

void TokenInfoVisitor::VisitString(cmajor::fault::tolerant::ast::LiteralTokenNode* str)
{
    TokenInfo tokenInfo;
    tokenInfo.lineColLen = soul::ast::SpanToLineColLen(str->GetSpan(), compileUnit->LineStarts());
    tokenInfo.tokenType = TokenType::string;
    tokenInfo.tokenModifiers = TokenModifiers::none;
    tokenInfos.push_back(tokenInfo);
}

std::vector<TokenInfo> GetTokenInfo(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit, SymbolTable* symbolTable)
{
    TokenInfoVisitor visitor(symbolTable, compileUnit);
    compileUnit->Accept(visitor);
    return visitor.GetTokenInfos();
}

} // namespace cmajor::fault::tolerant::symbols
