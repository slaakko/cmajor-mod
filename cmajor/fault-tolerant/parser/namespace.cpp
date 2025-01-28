// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.namespaces;

import cmajor.lexers;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.compile.unit;
import util;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

void ParseNamespaceContent(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::NamespaceDefinitionNode* ns)
{
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match namespaceImportMatch = ParseNamespaceImport(context, lexer);
        if (namespaceImportMatch.hit)
        {
            ns->AddNode(static_cast<cmajor::fault::tolerant::ast::SyntaxNode*>(namespaceImportMatch.value));
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match definitionMatch = ParseDefinition(context, lexer);
        if (definitionMatch.hit)
        {
            ns->AddNode(static_cast<cmajor::fault::tolerant::ast::SyntaxNode*>(definitionMatch.value));
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseNamespaceImport(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::NamespaceImportNode> namespaceImport;
    if (*lexer == USING)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::UsingKeywordNode> usingKeywordNode(
            new cmajor::fault::tolerant::ast::UsingKeywordNode(
                lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::global));
        namespaceImport.reset(new cmajor::fault::tolerant::ast::NamespaceImportNode());
        namespaceImport->SetUsingKeyword(usingKeywordNode.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match qidMatch = ParseQualifiedId(context, lexer);
    if (qidMatch.hit)
    {
        namespaceImport->SetQualifiedId(static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(qidMatch.value));
        if (*lexer == SEMICOLON)
        {
            namespaceImport->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        namespaceImport->SetQualifiedId(new cmajor::fault::tolerant::ast::QualifiedIdNode(
            new cmajor::fault::tolerant::ast::IdentifierNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::nsName, std::u16string())));
        namespaceImport->SetSemicolon(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    namespaceImport->Make();
    return soul::parser::Match(true, namespaceImport.release());
}

soul::parser::Match ParseNamespaceDefinition(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::NamespaceDefinitionNode> namespaceDefinitionNode;
    if (*lexer == NAMESPACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::NamespaceKeywordNode> namespaceNode(
            new cmajor::fault::tolerant::ast::NamespaceKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        namespaceDefinitionNode.reset(new cmajor::fault::tolerant::ast::NamespaceDefinitionNode(namespaceNode.release()));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match namespaceNameMatch = ParseQualifiedId(context, lexer);
    if (namespaceNameMatch.hit)
    {
        namespaceDefinitionNode->SetNamespaceName(static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(namespaceNameMatch.value));
    }
    else
    {
        namespaceDefinitionNode->SetNamespaceName(new cmajor::fault::tolerant::ast::QualifiedIdNode(
            new cmajor::fault::tolerant::ast::IdentifierNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none, std::u16string())));
    }
    if (*lexer == LBRACE)
    {
        namespaceDefinitionNode->SetLBrace(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        if (!namespaceNameMatch.hit)
        {
            namespaceDefinitionNode->SetUnnamedNs();
            util::uuid nsId = util::uuid::random();
            util::Sha1 sha1;
            for (auto byte : nsId)
            {
                sha1.Process(byte);
            }
            std::u16string nsName = util::ToUtf16("ns_" + sha1.GetDigest());
            cmajor::fault::tolerant::ast::NamespaceImportNode* nsImport = new cmajor::fault::tolerant::ast::NamespaceImportNode();
            cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedId = new cmajor::fault::tolerant::ast::QualifiedIdNode(
                new cmajor::fault::tolerant::ast::IdentifierNode(namespaceDefinitionNode->GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none, nsName));
            nsImport->SetQualifiedId(qualifiedId);
            context.CompileUnit()->GlobalNs()->AddNode(nsImport);
            namespaceDefinitionNode->SetNamespaceName(static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(qualifiedId->Clone()));
        }
    }
    else
    {
        namespaceDefinitionNode->SetLBrace(new cmajor::fault::tolerant::ast::LBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    ParseNamespaceContent(context, lexer, namespaceDefinitionNode.get());
    if (*lexer == RBRACE)
    {
        namespaceDefinitionNode->SetRBrace(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        namespaceDefinitionNode->SetRBrace(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    }
    namespaceDefinitionNode->Make();
    return soul::parser::Match(true, namespaceDefinitionNode.release());
}

} // namespace cmajor::fault::tolerant::parser
