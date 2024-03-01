// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.interface;

import cmajor.fault.tolerant.parser.attribute;
import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.parameter;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseInterfaceMemberFunction(Context& context, Lexer& lexer)
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
    std::unique_ptr<cmajor::fault::tolerant::ast::MemberFunctionNode> memberFunction;
    memberFunction.reset(new cmajor::fault::tolerant::ast::MemberFunctionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        memberFunction->SetAttributes(attributes.release());
    }
    memberFunction->SetReturnType(returnType.release());
    memberFunction->SetFunctionGroupId(id.release());
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, memberFunction->ParameterList());
    if (!parameterListMatch.hit)
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    memberFunction->SetSemicolon(semicolon.release());
    memberFunction->Make();
    return soul::parser::Match(true, memberFunction.release());
}

void ParseInterfaceContent(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::InterfaceNode* interface)
{
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match interfaceMemberFunctionMatch = ParseInterfaceMemberFunction(context, lexer);
        if (interfaceMemberFunctionMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::MemberFunctionNode> memberFunction;
            memberFunction.reset(static_cast<cmajor::fault::tolerant::ast::MemberFunctionNode*>(interfaceMemberFunctionMatch.value));
            interface->AddMemberFunction(memberFunction.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseInterface(Context& context, Lexer& lexer)
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
    if (specifiersMatch.hit)
    {
        specifiers.reset(static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifiersMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::InterfaceKeywordNode> interfaceKeyword;
    if (*lexer == INTERFACE)
    {
        interfaceKeyword.reset(new cmajor::fault::tolerant::ast::InterfaceKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::InterfaceNode> interface;
    interface.reset(new cmajor::fault::tolerant::ast::InterfaceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
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
    if (attributes)
    {
        interface->SetAttributes(attributes.release());
    }
    interface->SetSpecifiers(specifiers.release());
    interface->SetInterfaceKeyword(interfaceKeyword.release());
    interface->SetId(id.release());
    interface->SetLBrace(lbrace.release());
    ParseInterfaceContent(context, lexer, interface.get());
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
    interface->SetRBrace(rbrace.release());
    interface->Make();
    return soul::parser::Match(true, interface.release());
}

} // namespace cmajor::fault::tolerant::parser
