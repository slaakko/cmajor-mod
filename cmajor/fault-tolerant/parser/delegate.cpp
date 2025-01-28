// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.delegate;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.parameter;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseDelegate(Context& context, Lexer& lexer)
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
    std::unique_ptr<cmajor::fault::tolerant::ast::DelegateKeywordNode> delegateKeyword;
    if (*lexer == DELEGATE)
    {
        delegateKeyword.reset(new cmajor::fault::tolerant::ast::DelegateKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
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
    std::unique_ptr<cmajor::fault::tolerant::ast::DelegateNode> delegate;
    delegate.reset(new cmajor::fault::tolerant::ast::DelegateNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    delegate->SetSpecifiers(specifiers.release());
    delegate->SetDelegateKeyword(delegateKeyword.release());
    delegate->SetReturnType(returnType.release());
    delegate->SetId(id.release());
    ParseParameterList(context, lexer, delegate->ParameterList());
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
    delegate->SetSemicolon(semicolon.release());
    delegate->Make();
    return soul::parser::Match(true, delegate.release());
}

soul::parser::Match ParseClassDelegate(Context& context, Lexer& lexer)
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
    std::unique_ptr<cmajor::fault::tolerant::ast::ClassKeywordNode> classKeyword;
    if (*lexer == CLASS)
    {
        classKeyword.reset(new cmajor::fault::tolerant::ast::ClassKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::DelegateKeywordNode> delegateKeyword;
    if (*lexer == DELEGATE)
    {
        delegateKeyword.reset(new cmajor::fault::tolerant::ast::DelegateKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
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
    std::unique_ptr<cmajor::fault::tolerant::ast::ClassDelegateNode> classDelegate;
    classDelegate.reset(new cmajor::fault::tolerant::ast::ClassDelegateNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    classDelegate->SetSpecifiers(specifiers.release());
    classDelegate->SetClassKeyword(classKeyword.release());
    classDelegate->SetDelegateKeyword(delegateKeyword.release());
    classDelegate->SetReturnType(returnType.release());
    classDelegate->SetId(id.release());
    ParseParameterList(context, lexer, classDelegate->ParameterList());
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
    classDelegate->SetSemicolon(semicolon.release());
    classDelegate->Make();
    return soul::parser::Match(true, classDelegate.release());
}

} // namespace cmajor::fault::tolerant::parser
