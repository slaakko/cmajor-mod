// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.constant;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.expression;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseConstant(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::SpecifiersNode> specifiers;
    soul::parser::Match specifiersMatch = ParseSpecifiers(context, lexer);
    if (specifiersMatch.hit)
    {
        specifiers.reset(static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifiersMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstKeywordNode> constKeyword;
    if (*lexer == CONST)
    {
        constKeyword.reset(new cmajor::fault::tolerant::ast::ConstKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> type;
    soul::parser::Match typeMatch = ParseTypeExpr(context, lexer);
    if (typeMatch.hit)
    {
        type.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
    if (*lexer == ASSIGN)
    {
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstantNode> constant;
    constant.reset(new cmajor::fault::tolerant::ast::ConstantNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    constant->SetSpecifiers(specifiers.release());
    constant->SetConstKeyword(constKeyword.release());
    constant->SetType(type.release());
    constant->SetId(id.release());
    constant->SetAssign(assign.release());
    constant->SetExpr(expr.release());
    constant->SetSemicolon(semicolon.release());
    constant->Make();
    return soul::parser::Match(true, constant.release());
}

} // namespace cmajor::fault::tolerant::parser
