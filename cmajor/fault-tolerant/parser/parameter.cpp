// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.parameter;

import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseParameter(Context& context, Lexer& lexer)
{
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::ParameterNode> parameter;
    if (typeExprMatch.hit)
    {
        parameter.reset(new cmajor::fault::tolerant::ast::ParameterNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        parameter->SetTypeExpr(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        parameter->SetName(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    parameter->Make();
    return soul::parser::Match(true, parameter.release());
}

soul::parser::Match ParseParameterList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::ParameterListNode* parameterList)
{
    if (*lexer == LPAREN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        parameterList->SetLParen(lparen.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match parameterMatch = ParseParameter(context, lexer);
    bool first = false;
    if (parameterMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ParameterNode> parameter;
        parameter.reset(static_cast<cmajor::fault::tolerant::ast::ParameterNode*>(parameterMatch.value));
        parameterList->AddParameter(parameter.release());
    }
    else
    {
        lexer.SetPos(save);
        first = true;
    }
    while (!first)
    {
        save = lexer.GetPos();
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
        std::unique_ptr<cmajor::fault::tolerant::ast::ParameterNode> parameter;
        soul::parser::Match parameterMatch = ParseParameter(context, lexer);
        if (parameterMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ParameterNode> parameter;
            parameter.reset(static_cast<cmajor::fault::tolerant::ast::ParameterNode*>(parameterMatch.value));
            parameterList->AddComma(comma.release());
            parameterList->AddParameter(parameter.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    if (*lexer == RPAREN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        parameterList->SetRParen(rparen.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true);
}

} // namespace cmajor::fault::tolerant::parser

