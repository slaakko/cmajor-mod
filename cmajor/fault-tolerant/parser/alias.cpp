// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.alias;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseUsingAlias(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::UsingAliasNode> usingAliasNode(
        new cmajor::fault::tolerant::ast::UsingAliasNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    soul::parser::Match specifierMatch = ParseSpecifiers(context, lexer);
    if (specifierMatch.hit)
    {
        cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifierMatch.value);
        usingAliasNode->SetSpecifiers(specifiersNode);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == USING)
    {
        usingAliasNode->SetUsingKeyword(new cmajor::fault::tolerant::ast::UsingKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        cmajor::fault::tolerant::ast::IdentifierNode* identifierNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value);
        usingAliasNode->SetIdentifier(identifierNode);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == ASSIGN)
    {
        usingAliasNode->SetAssign(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        usingAliasNode->SetTypeExpr(static_cast<cmajor::fault::tolerant::ast::TypeExprNode*>(typeExprMatch.value)); 
    }
    else
    {
        usingAliasNode->SetTypeExpr(new cmajor::fault::tolerant::ast::TypeExprNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::typeOrNamespaceName));
    }
    if (*lexer == SEMICOLON)
    {
        usingAliasNode->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        usingAliasNode->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    usingAliasNode->Make();
    return soul::parser::Match(true, usingAliasNode.release());
}

soul::parser::Match ParseTypedef(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TypedefAliasNode> typedefAliasNode(
        new cmajor::fault::tolerant::ast::TypedefAliasNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    soul::parser::Match specifierMatch = ParseSpecifiers(context, lexer);
    if (specifierMatch.hit)
    {
        cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifierMatch.value);
        typedefAliasNode->SetSpecifiersNode(specifiersNode);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == TYPEDEF)
    {
        typedefAliasNode->SetTypedefKeyword(new cmajor::fault::tolerant::ast::TypedefKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typedefAliasNode->SetTypeExprNode(static_cast<cmajor::fault::tolerant::ast::TypeExprNode*>(typeExprMatch.value));
    }
    else
    {
        typedefAliasNode->SetTypeExprNode(new cmajor::fault::tolerant::ast::TypeExprNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::typeOrNamespaceName));
    }
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        typedefAliasNode->SetIdentifierNode(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        typedefAliasNode->SetIdentifierNode(new cmajor::fault::tolerant::ast::IdentifierNode(
            soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none, std::u16string()));
    }
    if (*lexer == SEMICOLON)
    {
        typedefAliasNode->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        typedefAliasNode->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    typedefAliasNode->Make();
    return soul::parser::Match(true, typedefAliasNode.release());
}

} // namespace cmajor::fault::tolerant::parser
