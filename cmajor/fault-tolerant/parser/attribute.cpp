// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.attribute;

import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.literal;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseAttribute(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> name;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        name.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> value;
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
    if (*lexer == ASSIGN)
    {
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        soul::parser::Match valueMatch = ParseStringLiteral(context, lexer);
        if (valueMatch.hit)
        {
            value.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(valueMatch.value));
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        value.reset(cmajor::fault::tolerant::ast::CreateStringLiteralNode(
            new cmajor::fault::tolerant::ast::LiteralTokenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none, u"true"),
            u"true", cmajor::fault::tolerant::ast::StringLiteralPrefix::utf16Prefix));
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AttributeNode> attribute;
    attribute.reset(new cmajor::fault::tolerant::ast::AttributeNode(name.release(), value.release()));
    if (assign)
    {
        attribute->SetAssign(assign.release());
    }
    attribute->Make();
    return soul::parser::Match(true, attribute.release());
}

void ParseAttributes(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::AttributesNode* attributes)
{
    soul::parser::Match attributeMatch = ParseAttribute(context, lexer);
    if (attributeMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::AttributeNode> attribute;
        attribute.reset(static_cast<cmajor::fault::tolerant::ast::AttributeNode*>(attributeMatch.value));
        attributes->AddAttribute(attribute.release());
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
        soul::parser::Match attributeMatch = ParseAttribute(context, lexer);
        if (attributeMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::AttributeNode> attribute;
            attribute.reset(static_cast<cmajor::fault::tolerant::ast::AttributeNode*>(attributeMatch.value));
            attributes->AddComma(comma.release());
            attributes->AddAttribute(attribute.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseAttributes(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::LBracketNode> lbracket;
    if (*lexer == LBRACKET)
    {
        lbracket.reset(new cmajor::fault::tolerant::ast::LBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AttributesNode> attributes;
    attributes.reset(new cmajor::fault::tolerant::ast::AttributesNode());
    attributes->SetLBracket(lbracket.release());
    ParseAttributes(context, lexer, attributes.get());
    std::unique_ptr<cmajor::fault::tolerant::ast::RBracketNode> rbracket;
    if (*lexer == RBRACKET)
    {
        rbracket.reset(new cmajor::fault::tolerant::ast::RBracketNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    attributes->SetRBracket(rbracket.release());
    attributes->Make();
    return soul::parser::Match(true, attributes.release());
}

} // namespace cmajor::fault::tolerant::parser
