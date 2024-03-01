// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.identifier;

import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseIdentifier(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::CompletionContext completionContext)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifierNode;
    if (*lexer == ID)
    {
        const auto& token = lexer.GetToken(lexer.GetPos());
        identifierNode.reset(new cmajor::fault::tolerant::ast::IdentifierNode(lexer.GetSpan(lexer.GetPos()), completionContext, token.ToString()));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    identifierNode->Make();
    return soul::parser::Match(true, identifierNode.release());
}

soul::parser::Match ParseQualifiedId(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::QualifiedIdNode> qualifiedIdNode;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::nsName);
    if (idMatch.hit)
    {
        cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value);
        qualifiedIdNode.reset(new cmajor::fault::tolerant::ast::QualifiedIdNode(idNode));
        while (true)
        {
            int64_t save = lexer.GetPos();
            if (*lexer == DOT)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::DotNode> dotNode(
                    new cmajor::fault::tolerant::ast::DotNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
                qualifiedIdNode->AddDotNode(dotNode.release());
                ++lexer;
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
            idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::dot);
            if (idMatch.hit)
            {
                cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value);
                qualifiedIdNode->AddIdentifierNode(idNode);
            }
            else
            {
                qualifiedIdNode->AddIdentifierNode(new cmajor::fault::tolerant::ast::IdentifierNode(
                    soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::dot, std::u16string()));
                lexer.SetPos(save);
                break;
            }
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    qualifiedIdNode->Make();
    return soul::parser::Match(true, qualifiedIdNode.release());
}

} // namespace cmajor::fault::tolerant::parser
