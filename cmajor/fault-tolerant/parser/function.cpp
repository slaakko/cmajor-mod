// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.function;

import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.attribute;
import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.templates;
import cmajor.fault.tolerant.parser.parameter;
import cmajor.fault.tolerant.parser.concepts;
import cmajor.fault.tolerant.parser.statement;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseFunction(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::AttributesNode> attributes;
    soul::parser::Match attributesMatch = ParseAttributes(context, lexer);
    if (attributesMatch.hit)
    {
        attributes.reset(static_cast<cmajor::fault::tolerant::ast::AttributesNode*>(attributesMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SpecifiersNode> specifiers;
    soul::parser::Match specifiersMatch = ParseSpecifiers(context, lexer);
    save = lexer.GetPos();
    if (specifiersMatch.hit)
    {
        specifiers.reset(static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifiersMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> returnType;
    soul::parser::Match returnTypeMatch = ParseTypeExpr(context, lexer);
    if (returnTypeMatch.hit)
    {
        returnType.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(returnTypeMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> functionGroupId;
    soul::parser::Match functionGroupIdMatch = ParseFunctionGroupId(context, lexer);
    if (functionGroupIdMatch.hit)
    {
        functionGroupId.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(functionGroupIdMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::FunctionNode> function;
    function.reset(new cmajor::fault::tolerant::ast::FunctionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        function->SetAttributes(attributes.release());
    }
    function->SetSpecifiers(specifiers.release());
    function->SetReturnType(returnType.release());
    function->SetFunctionGroupId(functionGroupId.release());
    soul::parser::Match templateParameterListMatch = ParseTemplateParameterList(context, lexer, function->TemplateParameterList());
    if (!templateParameterListMatch.hit)
    {
        lexer.SetPos(save);
    }
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, function->ParameterList());
    if (!parameterListMatch.hit)
    {
        lexer.SetPos(save);
        return soul::parser::Match(false);
    }
    save = lexer.GetPos();
    soul::parser::Match whereConstraintMatch = ParseWhereConstraint(context, lexer);
    if (whereConstraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> whereConstraint;
        whereConstraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(whereConstraintMatch.value));
        function->SetConstraint(whereConstraint.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match compoundStatementMatch = ParseCompoundStatement(context, lexer);
    if (compoundStatementMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> compoundStatement;
        compoundStatement.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(compoundStatementMatch.value));
        function->SetBody(compoundStatement.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            function->SetSemicolon(semicolon.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    function->Make();
    return soul::parser::Match(true, function.release());
}

struct OperatorFunctionGroupIdGuard
{
    OperatorFunctionGroupIdGuard(cmajor::lexer::CmajorLexer_Variables* vars_) : vars(vars_)
    {
        vars->parsingOperatorFunctionGroupId = true;
    }
    ~OperatorFunctionGroupIdGuard()
    {
        vars->parsingOperatorFunctionGroupId = false;
    }
    cmajor::lexer::CmajorLexer_Variables* vars;
};

soul::parser::Match ParseOperatorFunctionGroupId(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::OperatorFunctionGroupIdNode> operatorFunctionGroupId;
    operatorFunctionGroupId.reset(new cmajor::fault::tolerant::ast::OperatorFunctionGroupIdNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    auto vars = static_cast<cmajor::lexer::CmajorLexer_Variables*>(lexer.GetVariables());
    OperatorFunctionGroupIdGuard guard(vars);
    std::unique_ptr<cmajor::fault::tolerant::ast::OperatorKeywordNode> operatorKeyword;
    if (*lexer == OPERATOR)
    {
        operatorKeyword.reset(new cmajor::fault::tolerant::ast::OperatorKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        operatorFunctionGroupId->SetOperatorKeyword(operatorKeyword.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    if (*lexer == LANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ShiftLeftNode> shiftLeft;
        soul::ast::Span span(lexer.GetSpan(lexer.GetPos()));
        ++span.len;
        shiftLeft.reset(new cmajor::fault::tolerant::ast::ShiftLeftNode(span, cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        if (*lexer == LANGLE)
        {
            ++lexer;
            int64_t save2 = lexer.GetPos();
            soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
            if (typeExprMatch.hit)
            {
                bool matched = true;
                while (true)
                {
                    int64_t save3 = lexer.GetPos();
                    if (*lexer == COMMA)
                    {
                        ++lexer;
                    }
                    else
                    {
                        lexer.SetPos(save3);
                        break;
                    }
                    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
                    if (!typeExprMatch.hit)
                    {
                        lexer.SetPos(save3);
                        matched = false;
                        break;
                    }
                }
                if (matched)
                {
                    if (*lexer == RANGLE)
                    {
                        lexer.SetPos(save);
                    }
                    else
                    {
                        lexer.SetPos(save2);
                        operatorFunctionGroupId->AddOperatorNode(shiftLeft.release());
                        operatorFunctionGroupId->Make();
                        return soul::parser::Match(true, operatorFunctionGroupId.release());
                    }
                }
                else
                {
                    lexer.SetPos(save);
                }
            }
            else
            {
                lexer.SetPos(save2);
                operatorFunctionGroupId->AddOperatorNode(shiftLeft.release());
                operatorFunctionGroupId->Make();
                return soul::parser::Match(true, operatorFunctionGroupId.release());
            }
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
    switch (*lexer)
    {
        case SHIFTRIGHT:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ShiftRightNode> shiftRight;
            shiftRight.reset(new cmajor::fault::tolerant::ast::ShiftRightNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(shiftRight.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case EQ:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::EqualNode> eq;
            eq.reset(new cmajor::fault::tolerant::ast::EqualNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(eq.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case ASSIGN:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
            assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(assign.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case LANGLE:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LAngleNode> langle;
            langle.reset(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(langle.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case ARROW:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ArrowNode> arrow;
            arrow.reset(new cmajor::fault::tolerant::ast::ArrowNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(arrow.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case PLUSPLUS:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::IncrementNode> increment;
            increment.reset(new cmajor::fault::tolerant::ast::IncrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(increment.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case MINUSMINUS:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::DecrementNode> decrement;
            decrement.reset(new cmajor::fault::tolerant::ast::DecrementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(decrement.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case PLUS:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::PlusNode> plus;
            plus.reset(new cmajor::fault::tolerant::ast::PlusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(plus.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case MINUS:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::MinusNode> minus;
            minus.reset(new cmajor::fault::tolerant::ast::MinusNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(minus.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case STAR:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::StarNode> star;
            star.reset(new cmajor::fault::tolerant::ast::StarNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(star.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case DIV:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::DivNode> div;
            div.reset(new cmajor::fault::tolerant::ast::DivNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(div.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case REM:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RemNode> rem;
            rem.reset(new cmajor::fault::tolerant::ast::RemNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(rem.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case AMP:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::AmpNode> amp;
            amp.reset(new cmajor::fault::tolerant::ast::AmpNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(amp.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case BITOR:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::BitOrNode> bitOr;
            bitOr.reset(new cmajor::fault::tolerant::ast::BitOrNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(bitOr.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case BITXOR:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::BitXorNode> bitXor;
            bitXor.reset(new cmajor::fault::tolerant::ast::BitXorNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(bitXor.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case EXCLAMATION:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::NotNode> notNode;
            notNode.reset(new cmajor::fault::tolerant::ast::NotNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(notNode.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case CPL:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ComplementNode> complement;
            complement.reset(new cmajor::fault::tolerant::ast::ComplementNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            operatorFunctionGroupId->AddOperatorNode(complement.release());
            operatorFunctionGroupId->Make();
            return soul::parser::Match(true, operatorFunctionGroupId.release());
        }
        case LBRACKET:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LBracketNode> lbracket;
            lbracket.reset(new cmajor::fault::tolerant::ast::LBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            if (*lexer == RBRACKET)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket;
                rbracket.reset(new cmajor::fault::tolerant::ast::RBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                operatorFunctionGroupId->AddOperatorNode(lbracket.release());
                operatorFunctionGroupId->AddOperatorNode(rbracket.release());
                operatorFunctionGroupId->Make();
                return soul::parser::Match(true, operatorFunctionGroupId.release());
            }
            else
            {
                lexer.SetPos(save);
            }
            break;
        }
        case LPAREN:
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
            lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            if (*lexer == RPAREN)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
                rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                operatorFunctionGroupId->AddOperatorNode(lparen.release());
                operatorFunctionGroupId->AddOperatorNode(rparen.release());
                operatorFunctionGroupId->Make();
                return soul::parser::Match(true, operatorFunctionGroupId.release());
            }
            else
            {
                lexer.SetPos(save);
            }
            break;
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseFunctionGroupId(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        return idMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match operatorFunctionGroupIdMatch = ParseOperatorFunctionGroupId(context, lexer);
    if (operatorFunctionGroupIdMatch.hit)
    {
        return operatorFunctionGroupIdMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

} // namespace cmajor::fault::tolerant::parser
