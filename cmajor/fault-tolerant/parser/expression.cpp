// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.expression;

import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.literal;
import cmajor.fault.tolerant.parser.templates;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseSizeOfExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::SizeOfKeywordNode> sizeOfNode;
    if (*lexer == SIZEOF)
    {
        sizeOfNode.reset(new cmajor::fault::tolerant::ast::SizeOfKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparenNode;
    if (*lexer == LPAREN)
    {
        lparenNode.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparenNode;
    if (*lexer == RPAREN)
    {
        rparenNode.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SizeOfExprNode> sizeOfExprNode;
    sizeOfExprNode.reset(new cmajor::fault::tolerant::ast::SizeOfExprNode(sizeOfNode.release(), expr.release()));
    sizeOfExprNode->SetLParen(lparenNode.release());
    sizeOfExprNode->SetRParen(rparenNode.release());
    sizeOfExprNode->Make();
    return soul::parser::Match(true, sizeOfExprNode.release());
}

soul::parser::Match ParseTypeNameExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeNameKeywordNode> typeNameNode;
    if (*lexer == TYPENAME)
    {
        typeNameNode.reset(new cmajor::fault::tolerant::ast::TypeNameKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparenNode;
    if (*lexer == LPAREN)
    {
        lparenNode.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparenNode;
    if (*lexer == RPAREN)
    {
        rparenNode.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeNameExprNode> typeNameExprNode;
    typeNameExprNode.reset(new cmajor::fault::tolerant::ast::TypeNameExprNode(typeNameNode.release(), expr.release()));
    typeNameExprNode->SetLParen(lparenNode.release());
    typeNameExprNode->SetRParen(rparenNode.release());
    typeNameExprNode->Make();
    return soul::parser::Match(true, typeNameExprNode.release());
}

soul::parser::Match ParseTypeIdExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeIdKeywordNode> typeIdNode;
    if (*lexer == TYPEID)
    {
        typeIdNode.reset(new cmajor::fault::tolerant::ast::TypeIdKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparenNode;
    if (*lexer == LPAREN)
    {
        lparenNode.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparenNode;
    if (*lexer == RPAREN)
    {
        rparenNode.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeIdExprNode> typeIdExprNode;
    typeIdExprNode.reset(new cmajor::fault::tolerant::ast::TypeIdExprNode(typeIdNode.release(), expr.release()));
    typeIdExprNode->SetLParen(lparenNode.release());
    typeIdExprNode->SetRParen(rparenNode.release());
    typeIdExprNode->Make();
    return soul::parser::Match(true, typeIdExprNode.release());
}

soul::parser::Match ParseCastExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::CastKeywordNode> castNode;
    if (*lexer == CAST)
    {
        castNode.reset(new cmajor::fault::tolerant::ast::CastKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr< cmajor::fault::tolerant::ast::LAngleNode> langle;
    if (*lexer == LANGLE)
    {
        langle.reset(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle;
    if (*lexer == RANGLE)
    {
        rangle.reset(new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr< cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match exprMatch = ParseExpression(context, lexer);
    if (exprMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::CastExprNode> castExprNode(
        new cmajor::fault::tolerant::ast::CastExprNode(castNode.release(), langle.release(), typeExpr.release(), rangle.release(),
            lparen.release(), expr.release(), rparen.release()));
    castExprNode->Make();
    return soul::parser::Match(true, castExprNode.release());
}

soul::parser::Match ParseConstructExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstructKeywordNode> constructNode;
    if (*lexer == CONSTRUCT)
    {
        constructNode.reset(new cmajor::fault::tolerant::ast::ConstructKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr< cmajor::fault::tolerant::ast::LAngleNode> langle;
    if (*lexer == LANGLE)
    {
        langle.reset(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle;
    if (*lexer == RANGLE)
    {
        rangle.reset(new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstructExprNode> constructExprNode(
        new cmajor::fault::tolerant::ast::ConstructExprNode(constructNode.release(), langle.release(), typeExpr.release(), rangle.release(), lparen.release()));
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    soul::parser::Match exprListMatch = ParseExpressionList(context, lexer, constructExprNode.get());
    if (exprListMatch.hit)
    {
        if (*lexer == RPAREN)
        {
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    constructExprNode->SetRParen(rparen.release());
    constructExprNode->Make();
    return soul::parser::Match(true, constructExprNode.release());
}

soul::parser::Match ParseNewExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::NewKeywordNode> newNode;
    if (*lexer == NEW)
    {
        newNode.reset(new cmajor::fault::tolerant::ast::NewKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::NewExprNode> newExprNode;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        newExprNode.reset(new cmajor::fault::tolerant::ast::NewExprNode(newNode.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value)));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        newExprNode->SetLParen(lparen.release());
        ++lexer;
        ParseArgumentList(context, lexer, newExprNode.get());
        std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
        if (*lexer == RPAREN)
        {
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            newExprNode->SetRParen(rparen.release());
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    newExprNode->Make();
    return soul::parser::Match(true, newExprNode.release());
}

void ParseArgumentList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* container)
{
    context.BeginParsingArguments();
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> arg;
    soul::parser::Match argMatch = ParseExpression(context, lexer);
    if (argMatch.hit)
    {
        arg.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(argMatch.value));
        container->AddNode(arg.release());
    }
    else
    {
        lexer.SetPos(save);
        return;
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
        if (*lexer == COMMA)
        {
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> arg;
        soul::parser::Match argMatch = ParseExpression(context, lexer);
        if (argMatch.hit)
        {
            arg.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(argMatch.value));
            container->AddComma(comma.release());
            container->AddNode(arg.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    context.EndParsingArguments();
}

soul::parser::Match ParseExpressionList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* container)
{
    context.BeginParsingArguments();
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> arg;
    soul::parser::Match argMatch = ParseExpression(context, lexer);
    if (argMatch.hit)
    {
        arg.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(argMatch.value));
        container->AddNode(arg.release());
    }
    else
    {
        lexer.SetPos(save);
        context.EndParsingArguments();
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> comma;
        if (*lexer == COMMA)
        {
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> arg;
        soul::parser::Match argMatch = ParseExpression(context, lexer);
        if (argMatch.hit)
        {
            arg.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(argMatch.value));
            container->AddNode(comma.release());
            container->AddNode(arg.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    context.EndParsingArguments();
    return soul::parser::Match(true);
}

soul::parser::Match ParsePrimary(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    switch (*lexer)
    {
        case LPAREN:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen(
                new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            soul::parser::Match exprMatch = ParseExpression(context, lexer);
            if (exprMatch.hit)
            {
                if (*lexer == RPAREN)
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                        new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    std::unique_ptr<cmajor::fault::tolerant::ast::ParenthesizedExprNode> expr(
                        new cmajor::fault::tolerant::ast::ParenthesizedExprNode(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value)));
                    expr->SetLParen(lparen.release());
                    expr->SetRParen(rparen.release());
                    expr->Make();
                    return soul::parser::Match(true, expr.release());
                }
                else
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                        new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
                    std::unique_ptr<cmajor::fault::tolerant::ast::ParenthesizedExprNode> expr(
                        new cmajor::fault::tolerant::ast::ParenthesizedExprNode(static_cast<cmajor::fault::tolerant::ast::Node*>(exprMatch.value)));
                    expr->SetLParen(lparen.release());
                    expr->SetRParen(rparen.release());
                    expr->Make();
                    return soul::parser::Match(true, expr.release());
                }
            }
            else
            { 
                return soul::parser::Match(false);
            }
            break;
        }
        case TRUE: case FALSE: case FLOATINGLIT: case INTLIT: case CHARLIT: case STRINGLIT: case NULLLIT: case LBRACKET: case LBRACE:
        {
            soul::parser::Match literalMatch = ParseLiteral(context, lexer);
            return literalMatch;
        }
        case AUTO: case BOOL: case SBYTE: case BYTE: case SHORT: case USHORT: case INT: case UINT: case LONG: case ULONG: case FLOAT: case DOUBLE: 
        case CHAR: case WCHAR: case UCHAR: case VOID:
        {
            soul::parser::Match basicTypeMatch = ParseBasicType(context, lexer);
            return basicTypeMatch;
        }
        case ID:
        {
            soul::parser::Match templateIdMatch = ParseTemplateId(context, lexer);
            if (templateIdMatch.hit)
            {
                return templateIdMatch;
            }
            else
            {
                lexer.SetPos(save);
            }
            soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
            return identifierMatch;
        }
        case THIS:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ThisKeywordNode> thisNode(
                new cmajor::fault::tolerant::ast::ThisKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return soul::parser::Match(true, thisNode.release());
        }
        case BASE:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::BaseKeywordNode> baseNode(
                new cmajor::fault::tolerant::ast::BaseKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return soul::parser::Match(true, baseNode.release());
        }
        case SIZEOF: 
        {
            soul::parser::Match sizeofMatch = ParseSizeOfExpr(context, lexer);
            return sizeofMatch;
        }
        case TYPENAME:
        {
            soul::parser::Match typenameMatch = ParseTypeNameExpr(context, lexer);
            return typenameMatch;
        }
        case TYPEID:
        {
            soul::parser::Match typeIdMatch = ParseTypeIdExpr(context, lexer);
            return typeIdMatch;
        }
        case CAST:
        {
            soul::parser::Match castMatch = ParseCastExpr(context, lexer);
            return castMatch;
        }
        case CONSTRUCT:
        {
            soul::parser::Match constructMatch = ParseConstructExpr(context, lexer);
            return constructMatch;
        }
        case NEW:
        {
            soul::parser::Match newMatch = ParseNewExpr(context, lexer);
            return newMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseInvokeExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match primaryMatch = ParsePrimary(context, lexer);
    if (primaryMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(primaryMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == LPAREN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen(
            new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        expr.reset(new cmajor::fault::tolerant::ast::InvokeExprNode(expr.release(), lparen.release()));
        ParseArgumentList(context, lexer, expr.get());
        if (*lexer == RPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            static_cast<cmajor::fault::tolerant::ast::InvokeExprNode*>(expr.get())->SetRParen(rparen.release());
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            static_cast<cmajor::fault::tolerant::ast::InvokeExprNode*>(expr.get())->SetRParen(rparen.release());
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    expr->Make();
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParsePostfix(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match primaryMatch = ParsePrimary(context, lexer);
    if (primaryMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(primaryMatch.value));
        bool end = false;
        while (!end)
        {
            int64_t save = lexer.GetPos();
            switch (*lexer)
            {
                case PLUSPLUS:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::Node> op(
                        new cmajor::fault::tolerant::ast::IncrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    expr.reset(new cmajor::fault::tolerant::ast::PostfixExprNode(expr.release(), op.release()));
                    expr->Make();
                    ++lexer;
                    break;
                }
                case MINUSMINUS:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::Node> op(
                        new cmajor::fault::tolerant::ast::DecrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    expr.reset(new cmajor::fault::tolerant::ast::PostfixExprNode(expr.release(), op.release()));
                    expr->Make();
                    ++lexer;
                    break;
                }
                case DOT:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::DotNode> dot(
                        new cmajor::fault::tolerant::ast::DotNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::dot);
                    if (identifierMatch.hit)
                    {
                        cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value);
                        expr.reset(new cmajor::fault::tolerant::ast::DotMemberNode(expr.release(), dot.release(), idNode));
                        expr->Make();
                    }
                    else
                    {
                        cmajor::fault::tolerant::ast::IdentifierNode* idNode = new cmajor::fault::tolerant::ast::IdentifierNode(soul::ast::Span(),
                            cmajor::fault::tolerant::ast::CompletionContext::dot, std::u16string());
                        expr.reset(new cmajor::fault::tolerant::ast::DotMemberNode(expr.release(), dot.release(), idNode));
                        expr->Make();
                        end = true;
                        lexer.SetPos(save);
                    }
                    break;
                }
                case ARROW:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::ArrowNode> arrow(
                        new cmajor::fault::tolerant::ast::ArrowNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::arrow);
                    if (identifierMatch.hit)
                    {
                        cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value);
                        expr.reset(new cmajor::fault::tolerant::ast::ArrowMemberNode(expr.release(), arrow.release(), idNode));
                        expr->Make();
                    }
                    else
                    {
                        cmajor::fault::tolerant::ast::IdentifierNode* idNode = new cmajor::fault::tolerant::ast::IdentifierNode(soul::ast::Span(),
                            cmajor::fault::tolerant::ast::CompletionContext::arrow, std::u16string());
                        expr.reset(new cmajor::fault::tolerant::ast::ArrowMemberNode(expr.release(), arrow.release(), idNode));
                        expr->Make();
                        end = true;
                        lexer.SetPos(save);
                    }
                    break;
                }
                case LBRACKET:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::LBracketNode> lbracket(
                        new cmajor::fault::tolerant::ast::LBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    context.PushParsingLvalue(false);
                    context.PushParsingExpressionStatement(false);
                    soul::parser::Match indexMatch = ParseExpression(context, lexer);
                    context.PopParsingLvalue();
                    context.PopParsingExpressionStatement();
                    if (indexMatch.hit)
                    {
                        if (*lexer == RBRACKET)
                        {
                            std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket(
                                new cmajor::fault::tolerant::ast::RBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                            ++lexer;
                            expr.reset(new cmajor::fault::tolerant::ast::SubscriptExprNode(expr.release(), lbracket.release(),
                                static_cast<cmajor::fault::tolerant::ast::Node*>(indexMatch.value), rbracket.release()));
                            expr->Make();
                        }
                        else
                        {
                            std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket(
                                new cmajor::fault::tolerant::ast::RBracketNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
                            expr.reset(new cmajor::fault::tolerant::ast::SubscriptExprNode(expr.release(), lbracket.release(),
                                static_cast<cmajor::fault::tolerant::ast::Node*>(indexMatch.value), rbracket.release()));
                            expr->Make();
                        }
                    }
                    else
                    {
                        lexer.SetPos(save);
                        end = true;
                    }
                    break;
                }
                case LPAREN:
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen(
                        new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    expr.reset(new cmajor::fault::tolerant::ast::InvokeExprNode(expr.release(), lparen.release()));
                    ParseArgumentList(context, lexer, expr.get());
                    if (*lexer == RPAREN)
                    {
                        std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                            new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                        ++lexer;
                        static_cast<cmajor::fault::tolerant::ast::InvokeExprNode*>(expr.get())->SetRParen(rparen.release());
                        expr->Make();
                    }
                    else
                    {
                        std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen(
                            new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
                        static_cast<cmajor::fault::tolerant::ast::InvokeExprNode*>(expr.get())->SetRParen(rparen.release());
                        lexer.SetPos(save);
                        expr->Make();
                        end = true;
                    }
                    break;
                }
                default:
                {
                    end = true;
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParsePrefixOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case PLUSPLUS:
        {
            soul::parser::Match incrementMatch(true, new cmajor::fault::tolerant::ast::IncrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return incrementMatch;
        }
        case MINUSMINUS:
        {
            soul::parser::Match decrementMatch(true, new cmajor::fault::tolerant::ast::DecrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return decrementMatch;
        }
        case PLUS:
        {
            soul::parser::Match plusMatch(true, new cmajor::fault::tolerant::ast::PlusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return plusMatch;
        }
        case MINUS:
        {
            soul::parser::Match minusMatch(true, new cmajor::fault::tolerant::ast::MinusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return minusMatch;
        }
        case EXCLAMATION:
        {
            soul::parser::Match notMatch(true, new cmajor::fault::tolerant::ast::NotNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return notMatch;
        }
        case CPL:
        {
            soul::parser::Match complementMatch(true, new cmajor::fault::tolerant::ast::ComplementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return complementMatch;
        }
        case STAR:
        {
            soul::parser::Match derefMatch(true, new cmajor::fault::tolerant::ast::StarNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return derefMatch;
        }
        case AMP:
        {
            soul::parser::Match addrOfMatch(true, new cmajor::fault::tolerant::ast::AmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return addrOfMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParsePrefix(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match opMatch = ParsePrefixOp(context, lexer);
    if (opMatch.hit)
    {
        soul::parser::Match prefixMatch = ParsePrefix(context, lexer);
        if (prefixMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::Node> prefix(static_cast<cmajor::fault::tolerant::ast::Node*>(prefixMatch.value));
            std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr(
                new cmajor::fault::tolerant::ast::PrefixExprNode(prefix.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value)));
            expr->Make();
            return soul::parser::Match(true, expr.release());
        }
        else
        {
            lexer.SetPos(save);
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match postfixMatch = ParsePostfix(context, lexer);
    if (postfixMatch.hit)
    {
        return postfixMatch;
    }
    else
    {
        return soul::parser::Match(false);
    }
}

soul::parser::Match ParseMultiplicativeOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case STAR:
        {
            soul::parser::Match mulMatch(true, new cmajor::fault::tolerant::ast::StarNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return mulMatch;
        }
        case DIV:
        {
            soul::parser::Match divMatch(true, new cmajor::fault::tolerant::ast::DivNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return divMatch;
        }
        case REM:
        {
            soul::parser::Match remMatch(true, new cmajor::fault::tolerant::ast::RemNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return remMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseMultiplicative(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParsePrefix(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match opMatch = ParseMultiplicativeOp(context, lexer);
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> op;
                if (opMatch.hit)
                {
                    op.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value));
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParsePrefix(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), op.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseAdditiveOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case PLUS:
        {
            soul::parser::Match plusMatch(true, new cmajor::fault::tolerant::ast::PlusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return plusMatch;
        }
        case MINUS:
        {
            soul::parser::Match minusMatch(true, new cmajor::fault::tolerant::ast::MinusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return minusMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseAdditive(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseMultiplicative(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match opMatch = ParseAdditiveOp(context, lexer);
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> op;
                if (opMatch.hit)
                {
                    op.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value));
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseMultiplicative(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), op.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

struct ParsingArgumentsGuard
{
    ParsingArgumentsGuard(Context& context_) : context(context_) { context.BeginParsingArguments(); }
    ~ParsingArgumentsGuard() { context.EndParsingArguments(); }
    Context& context;
};

soul::parser::Match ParseShiftOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case SHIFTLEFT:
        {
            soul::parser::Match shiftLeftMatch(true, new cmajor::fault::tolerant::ast::ShiftLeftNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return shiftLeftMatch;
        }
        case SHIFTRIGHT:
        {
            soul::parser::Match shiftRightMatch(true, new cmajor::fault::tolerant::ast::ShiftRightNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return shiftRightMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseShift(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseAdditive(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!context.ParsingLvalue())
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match opMatch = ParseShiftOp(context, lexer);
                ParsingArgumentsGuard guard(context);
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> op;
                if (opMatch.hit)
                {
                    op.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value));
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseAdditive(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), op.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseRelationalOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case LEQ: 
        {
            soul::parser::Match leqMatch(true, new cmajor::fault::tolerant::ast::LessEqualNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return leqMatch;
        }
        case GEQ:
        {
            soul::parser::Match geqMatch(true, new cmajor::fault::tolerant::ast::GreaterEqualNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return geqMatch;
        }
        case LANGLE:
        {
            soul::parser::Match lessMatch(true, new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return lessMatch;
        }
        case RANGLE:
        {
            soul::parser::Match greaterMatch(true, new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return greaterMatch;
        }
    }
    return soul::parser::Match(false);
}

struct ParsingIsOrAsGuard
{
    ParsingIsOrAsGuard(Context& context_) : context(context_), pop(false) {}
    ~ParsingIsOrAsGuard() { if (pop) context.PopParsingIsOrAs(); }
    void SetPop() { pop = true; }
    Context& context;
    bool pop;
};

soul::parser::Match ParseRelational(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseShift(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                ParsingIsOrAsGuard guard(context);
                int64_t save = lexer.GetPos();
                soul::parser::Match opMatch = ParseRelationalOp(context, lexer);
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> op;
                if (opMatch.hit)
                {
                    op.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value));
                    context.PushParsingIsOrAs(false);
                    guard.SetPop();
                }
                else if (*lexer == IS)
                {
                    op.reset(new cmajor::fault::tolerant::ast::IsKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    context.PushParsingIsOrAs(true);
                    guard.SetPop();
                    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
                    if (typeExprMatch.hit)
                    {
                        expr.reset(new cmajor::fault::tolerant::ast::IsExprNode(
                            expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value), op.release()));
                        expr->Make();
                    }
                    else
                    {
                        lexer.SetPos(save);
                        break;
                    }
                }
                else if (*lexer == AS)
                {
                    op.reset(new cmajor::fault::tolerant::ast::AsKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    context.PushParsingIsOrAs(true);
                    guard.SetPop();
                    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
                    if (typeExprMatch.hit)
                    {
                        expr.reset(new cmajor::fault::tolerant::ast::AsExprNode(
                            expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value), op.release()));
                        expr->Make();
                    }
                    else
                    {
                        lexer.SetPos(save);
                        break;
                    }
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                if (!context.ParsingIsOrAs())
                {
                    soul::parser::Match rightMatch = ParseShift(context, lexer);
                    if (rightMatch.hit)
                    {
                        expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                            expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), op.release()));
                        expr->Make();
                    }
                    else
                    {
                        lexer.SetPos(save);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseEqualityOp(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case EQ:
        {
            soul::parser::Match eqMatch(true, new cmajor::fault::tolerant::ast::EqualNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return eqMatch;
        }
        case NEQ:
        {
            soul::parser::Match notEqMatch(true, new cmajor::fault::tolerant::ast::NotEqualNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            return notEqMatch;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseEquality(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseRelational(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match opMatch = ParseEqualityOp(context, lexer);
                std::unique_ptr<cmajor::fault::tolerant::ast::Node> op;
                if (opMatch.hit)
                {
                    op.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(opMatch.value));
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseRelational(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), op.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseBitAnd(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseEquality(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::AmpNode> bitAndNode;
                if (*lexer == AMP)
                {
                    bitAndNode.reset(new cmajor::fault::tolerant::ast::AmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseEquality(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), bitAndNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseBitXor(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseBitAnd(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::BitXorNode> bitXorNode;
                if (*lexer == BITXOR)
                {
                    bitXorNode.reset(new cmajor::fault::tolerant::ast::BitXorNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseBitAnd(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), bitXorNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseBitOr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseBitXor(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::BitOrNode> bitOrNode;
                if (*lexer == BITOR)
                {
                    bitOrNode.reset(new cmajor::fault::tolerant::ast::BitOrNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseBitXor(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), bitOrNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseConjunction(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseBitOr(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::AmpAmpNode> conjunctionNode;
                if (*lexer == AMPAMP)
                {
                    conjunctionNode.reset(new cmajor::fault::tolerant::ast::AmpAmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseBitOr(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), conjunctionNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseDisjunction(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseConjunction(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (!(context.ParsingLvalue() || context.ParsingExpressionStatement() && !context.ParsingArguments()))
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::DisjunctionNode> disjunctionNode;
                if (*lexer == DISJUNCTION)
                {
                    disjunctionNode.reset(new cmajor::fault::tolerant::ast::DisjunctionNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseConjunction(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), disjunctionNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseImplication(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseDisjunction(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (context.ParsingConcept() || context.ParsingTemplateId())
        {
            int64_t save = lexer.GetPos();
            std::unique_ptr<cmajor::fault::tolerant::ast::ImplicationNode> implicationNode;
            if (*lexer == IMPLICATION)
            {
                implicationNode.reset(new cmajor::fault::tolerant::ast::ImplicationNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                soul::parser::Match rightMatch = ParseImplication(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), implicationNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                }
            }
            else
            {
                lexer.SetPos(save);
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

soul::parser::Match ParseExpression(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expr;
    soul::parser::Match leftMatch = ParseImplication(context, lexer);
    if (leftMatch.hit)
    {
        expr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(leftMatch.value));
        if (context.ParsingConcept() || context.ParsingTemplateId())
        {
            while (true)
            {
                int64_t save = lexer.GetPos();
                std::unique_ptr<cmajor::fault::tolerant::ast::EquivalenceNode> equivalenceNode;
                if (*lexer == EQUIVALENCE)
                {
                    equivalenceNode.reset(new cmajor::fault::tolerant::ast::EquivalenceNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
                soul::parser::Match rightMatch = ParseImplication(context, lexer);
                if (rightMatch.hit)
                {
                    expr.reset(new cmajor::fault::tolerant::ast::BinaryExprNode(
                        expr.release(), static_cast<cmajor::fault::tolerant::ast::Node*>(rightMatch.value), equivalenceNode.release()));
                    expr->Make();
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, expr.release());
}

} // namespace cmajor::fault::tolerant::parser
