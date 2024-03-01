// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.type_expr;

import cmajor.fault.tolerant.parser.templates;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.expression;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseBasicType(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case AUTO:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::AutoKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case BOOL:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::BoolKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case SBYTE:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SByteKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case BYTE:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::ByteKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case SHORT:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::ShortKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case USHORT:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::UShortKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case INT:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::IntKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case UINT:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::UIntKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case LONG:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::LongKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case ULONG:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::ULongKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case FLOAT:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::FloatKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case DOUBLE:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::DoubleKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case CHAR:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::CharKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case WCHAR:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::WCharKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case UCHAR:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::UCharKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
        case VOID:
        {
            soul::ast::Span span = lexer.GetSpan(lexer.GetPos());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::VoidKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParsePrimaryTypeExpr(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match basicTypeMatch = ParseBasicType(context, lexer);
    if (basicTypeMatch.hit)
    {
        return basicTypeMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match templateIdMatch = ParseTemplateId(context, lexer);
    if (templateIdMatch.hit)
    {
        return templateIdMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        return idMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParsePostfixTypeExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match primaryTypeExprMatch = ParsePrimaryTypeExpr(context, lexer);
    if (primaryTypeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(primaryTypeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    bool end = false;
    while (!end)
    {
        int save = lexer.GetPos();
        switch (*lexer)
        {
            case DOT:
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::DotNode> dotNode(
                    new cmajor::fault::tolerant::ast::DotNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::dot);
                if (idMatch.hit)
                {
                    typeExpr.reset(
                        new cmajor::fault::tolerant::ast::DotMemberNode(
                            typeExpr.release(), 
                            dotNode.release(),
                            static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value)));
                    typeExpr->Make();
                }
                else
                {
                    typeExpr.reset(
                        new cmajor::fault::tolerant::ast::DotMemberNode(
                            typeExpr.release(),
                            dotNode.release(),
                            new cmajor::fault::tolerant::ast::IdentifierNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::dot, std::u16string())));
                    lexer.SetPos(save);
                    typeExpr->Make();
                    end = true;
                }
                break;
            }
            case STAR:
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::StarNode> starNode(
                    new cmajor::fault::tolerant::ast::StarNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                typeExpr.reset(
                    new cmajor::fault::tolerant::ast::PointerNode(
                        typeExpr.release(),
                        starNode.release()));
                typeExpr->Make();
                break;
            }
            case AMPAMP:
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::AmpAmpNode> ampAmpNode(
                    new cmajor::fault::tolerant::ast::AmpAmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                typeExpr.reset(
                    new cmajor::fault::tolerant::ast::RValueRefNode(
                        typeExpr.release(),
                        ampAmpNode.release()));
                typeExpr->Make();
                break;
            }
            case AMP:
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::AmpNode> ampNode(
                    new cmajor::fault::tolerant::ast::AmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                typeExpr.reset(
                    new cmajor::fault::tolerant::ast::LValueRefNode(
                        typeExpr.release(),
                        ampNode.release()));
                typeExpr->Make();
                break;
            }
            case LBRACKET:
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::LBracketNode> lbracket(
                    new cmajor::fault::tolerant::ast::LBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> size;
                soul::parser::Match exprMatch = ParseExpression(context, lexer);
                if (exprMatch.hit)
                {
                    size.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
                }
                if (*lexer == RBRACKET)
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket(
                        new cmajor::fault::tolerant::ast::RBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    typeExpr.reset(
                        new cmajor::fault::tolerant::ast::ArrayNode(
                            typeExpr.release(),
                            lbracket.release(),
                            size.release(),
                            rbracket.release()));
                    typeExpr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    end = true;
                }
                break;
            }
            default:
            {
                lexer.SetPos(save);
                end = true;
                break;
            }
        }
    }
    return soul::parser::Match(true, typeExpr.release());
}

soul::parser::Match ParseTypeExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeExprNode> typeExprNode(
        new cmajor::fault::tolerant::ast::TypeExprNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::typeOrNamespaceName));
    if (*lexer == CONST)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ConstKeywordNode> constNode(
            new cmajor::fault::tolerant::ast::ConstKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        typeExprNode->AddNode(constNode.release());
        ++lexer;
    }
    soul::parser::Match postfixTypeExprMatch = ParsePostfixTypeExpr(context, lexer);
    if (postfixTypeExprMatch.hit)
    {
        typeExprNode->AddNode(static_cast<cmajor::fault::tolerant::ast::Node*>(postfixTypeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    typeExprNode->Make();
    return soul::parser::Match(true, typeExprNode.release());
}

} // namespace cmajor::fault::tolerant::parser
