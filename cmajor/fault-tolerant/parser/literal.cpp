// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.literal;

import cmajor.lexers;
import cmajor.fault.tolerant.parser.token.value;
import cmajor.fault.tolerant.parser.expression;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseStringLiteral(Context& context, Lexer& lexer)
{
    if (*lexer == STRINGLIT)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LiteralTokenNode> literalTokenNode(
            new cmajor::fault::tolerant::ast::LiteralTokenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none,
                lexer.GetToken(lexer.GetPos()).ToString()));
        cmajor::fault::tolerant::ast::StringLiteralPrefix prefix = cmajor::fault::tolerant::ast::StringLiteralPrefix::none;
        std::u16string value = ParseStringLiteral(lexer.FileName(), lexer.GetToken(lexer.GetPos()), prefix);
        ++lexer;
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> literalNode;
        literalNode.reset(cmajor::fault::tolerant::ast::CreateStringLiteralNode(literalTokenNode.release(), value, prefix));
        return soul::parser::Match(true, literalNode.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
}

soul::parser::Match ParseSimpleLiteral(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> literalNode;
    switch (*lexer)
    {
        case TRUE:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::KeywordNode> keywordNode(
                new cmajor::fault::tolerant::ast::TrueKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            literalNode.reset(new cmajor::fault::tolerant::ast::BooleanLiteralNode(keywordNode.release(), true));
            literalNode->Make();
            ++lexer;
            break;
        }
        case FALSE:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::KeywordNode> keywordNode(
                new cmajor::fault::tolerant::ast::FalseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            literalNode.reset(new cmajor::fault::tolerant::ast::BooleanLiteralNode(keywordNode.release(), false));
            literalNode->Make();
            ++lexer;
            break;
        }
        case FLOATINGLIT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LiteralTokenNode> literalTokenNode(
                new cmajor::fault::tolerant::ast::LiteralTokenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none,
                    lexer.GetToken(lexer.GetPos()).ToString()));
            bool isFloat = false;
            double value = ParseFloatingLiteral(lexer.FileName(), lexer.GetToken(lexer.GetPos()), isFloat);
            ++lexer;
            literalNode.reset(cmajor::fault::tolerant::ast::CreateFloatingLiteralNode(literalTokenNode.release(), isFloat, value));
            break;
        }
        case INTLIT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LiteralTokenNode> literalTokenNode(
                new cmajor::fault::tolerant::ast::LiteralTokenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none,
                    lexer.GetToken(lexer.GetPos()).ToString()));
            bool isUnsigned = false;
            uint64_t value = ParseIntegerLiteral(lexer.FileName(), lexer.GetToken(lexer.GetPos()), isUnsigned);
            ++lexer;
            literalNode.reset(cmajor::fault::tolerant::ast::CreateIntegerLiteralNode(literalTokenNode.release(), isUnsigned, value));
            break;
        }
        case CHARLIT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LiteralTokenNode> literalTokenNode(
                new cmajor::fault::tolerant::ast::LiteralTokenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none,
                    lexer.GetToken(lexer.GetPos()).ToString()));
            cmajor::fault::tolerant::ast::CharLiteralPrefix prefix = cmajor::fault::tolerant::ast::CharLiteralPrefix::none;
            char16_t value = ParseCharacterLiteral(lexer.FileName(), lexer.GetToken(lexer.GetPos()), prefix);
            ++lexer;
            literalNode.reset(cmajor::fault::tolerant::ast::CreateCharacterLiteralNode(literalTokenNode.release(), value, prefix));
            break;
        }
        case STRINGLIT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LiteralTokenNode> literalTokenNode(
                new cmajor::fault::tolerant::ast::LiteralTokenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none,
                    lexer.GetToken(lexer.GetPos()).ToString()));
            cmajor::fault::tolerant::ast::StringLiteralPrefix prefix = cmajor::fault::tolerant::ast::StringLiteralPrefix::none;
            std::u16string value = ParseStringLiteral(lexer.FileName(), lexer.GetToken(lexer.GetPos()), prefix);
            ++lexer;
            literalNode.reset(cmajor::fault::tolerant::ast::CreateStringLiteralNode(literalTokenNode.release(), value, prefix));
            break;
        }
        case NULLLIT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::KeywordNode> keywordNode(
                new cmajor::fault::tolerant::ast::NullKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            literalNode.reset(new cmajor::fault::tolerant::ast::NullLiteralNode(keywordNode.release()));
            literalNode->Make();
            break;
        }
        default:
        {
            return soul::parser::Match(false);
        }
    }
    return soul::parser::Match(true, literalNode.release());
}

soul::parser::Match ParseArrayLiteral(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ArrayLiteralNode> arrayLiteralNode(new cmajor::fault::tolerant::ast::ArrayLiteralNode());
    std::unique_ptr<cmajor::fault::tolerant::ast::LBracketNode> lbracket;
    if (*lexer == LBRACKET)
    {
        lbracket.reset(new cmajor::fault::tolerant::ast::LBracketNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        while (true)
        {
            int64_t save = lexer.GetPos();
            std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
            if (*lexer == COMMA)
            {
                comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
            soul::parser::Match nextExprMatch = ParseExpression(context, lexer);
            if (nextExprMatch.hit)
            {
                arrayLiteralNode->AddNode(comma.release());
                arrayLiteralNode->AddNode(static_cast<cmajor::fault::tolerant::ast::Node*>(nextExprMatch.value));
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket;
    if (*lexer == RBRACKET)
    {
        rbracket.reset(new cmajor::fault::tolerant::ast::RBracketNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    arrayLiteralNode->SetLBracket(lbracket.release());
    arrayLiteralNode->SetRBracket(rbracket.release());
    arrayLiteralNode->Make();
    return soul::parser::Match(true, arrayLiteralNode.release());
}

soul::parser::Match ParseStructuredLiteral(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::StructuredLiteralNode> structuredLiteralNode(new cmajor::fault::tolerant::ast::StructuredLiteralNode());
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        while (true)
        {
            int64_t save = lexer.GetPos();
            std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
            if (*lexer == COMMA)
            {
                comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
            soul::parser::Match nextExprMatch = ParseExpression(context, lexer);
            if (nextExprMatch.hit)
            {
                structuredLiteralNode->AddNode(comma.release());
                structuredLiteralNode->AddNode(static_cast<cmajor::fault::tolerant::ast::Node*>(nextExprMatch.value));
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    structuredLiteralNode->SetLBrace(lbrace.release());
    structuredLiteralNode->SetRBrace(rbrace.release());
    structuredLiteralNode->Make();
    return soul::parser::Match(true, structuredLiteralNode.release());
}

soul::parser::Match ParseComplexLiteral(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case LBRACKET:
        {
            soul::parser::Match arrayLiteralMatch = ParseArrayLiteral(context, lexer);
            return arrayLiteralMatch;
        }
        case LBRACE:
        {
            soul::parser::Match structuredLiteralMatch = ParseStructuredLiteral(context, lexer);
            return structuredLiteralMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseLiteral(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match simpleLiteralMatch = ParseSimpleLiteral(context, lexer);
    if (simpleLiteralMatch.hit)
    {
        return simpleLiteralMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match complexLiteralMatch = ParseComplexLiteral(context, lexer);
    if (complexLiteralMatch.hit)
    {
        return complexLiteralMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

} // namespace cmajor::fault::tolerant::parser
