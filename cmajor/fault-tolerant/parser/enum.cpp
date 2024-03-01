// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.enums;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.expression;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseEnumConstant(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::EnumTypeNode* enumType)
{
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
    std::unique_ptr<cmajor::fault::tolerant::ast::EnumConstantNode> enumConstant;
    enumConstant.reset(new cmajor::fault::tolerant::ast::EnumConstantNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    enumConstant->SetId(id.release());
    if (*lexer == ASSIGN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> value;
        soul::parser::Match valueMatch = ParseExpression(context, lexer);
        if (valueMatch.hit)
        {
            value.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(valueMatch.value));
        }
        else
        {
            return soul::parser::Match(false);
        }
        enumConstant->SetAssign(assign.release());
        enumConstant->SetValue(value.release());
    }
    else
    {
        enumConstant->SetValue(cmajor::fault::tolerant::ast::NextEnumConstantValue(enumType));
    }
    enumConstant->Make();
    return soul::parser::Match(true, enumConstant.release());
}

void ParseEnumConstants(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::EnumTypeNode* enumType)
{
    soul::parser::Match enumConstantMatch = ParseEnumConstant(context, lexer, enumType);
    if (enumConstantMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::EnumConstantNode> enumConstant;
        enumConstant.reset(static_cast<cmajor::fault::tolerant::ast::EnumConstantNode*>(enumConstantMatch.value));
        enumType->AddEnumConstant(enumConstant.release());
    }
    else
    {
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
        soul::parser::Match enumConstantMatch = ParseEnumConstant(context, lexer, enumType);
        if (enumConstantMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::EnumConstantNode> enumConstant;
            enumConstant.reset(static_cast<cmajor::fault::tolerant::ast::EnumConstantNode*>(enumConstantMatch.value));
            enumType->AddComma(comma.release());
            enumType->AddEnumConstant(enumConstant.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseEnumType(Context& context, Lexer& lexer)
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
    std::unique_ptr<cmajor::fault::tolerant::ast::EnumKeywordNode> enumKeyword;
    if (*lexer == ENUM)
    {
        enumKeyword.reset(new cmajor::fault::tolerant::ast::EnumKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
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
    save = lexer.GetPos();
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> underlyingType;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        soul::parser::Match underlyingTypeMatch = ParseTypeExpr(context, lexer);
        if (underlyingTypeMatch.hit)
        {
            underlyingType.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(underlyingTypeMatch.value));
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
    if (*lexer == LBRACE)
    {
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::EnumTypeNode> enumType;
    enumType.reset(new cmajor::fault::tolerant::ast::EnumTypeNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    enumType->SetSpecifiers(specifiers.release());
    enumType->SetEnumKeyword(enumKeyword.release());
    enumType->SetId(id.release());
    if (colon)
    {
        enumType->SetColon(colon.release());
        enumType->SetUnderlyingType(underlyingType.release());
    }
    else
    {
        enumType->SetUnderlyingType(new cmajor::fault::tolerant::ast::IntKeywordNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    enumType->SetLBrace(lbrace.release());
    ParseEnumConstants(context, lexer, enumType.get());
    std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
    if (*lexer == RBRACE)
    {
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    enumType->SetRBrace(rbrace.release());
    enumType->Make();
    return soul::parser::Match(true, enumType.release());
}

} // namespace cmajor::fault::tolerant::parser
