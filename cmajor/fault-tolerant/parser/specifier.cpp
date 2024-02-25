// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.specifier;

import cmajor.lexers;
import cmajor.fault.tolerant.ast.specifier;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseSpecifier(Context& context, Lexer& lexer)
{
    switch (*lexer)
    {
        case PUBLIC:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::PublicKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::public_));
        }
        case PROTECTED:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::ProtectedKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::protected_));
        }
        case PRIVATE:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::PrivateKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::private_));
        }
        case INTERNAL:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::InternalKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::internal_));
        }
        case STATIC:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::StaticKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::static_));
        }
        case VIRTUAL:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::VirtualKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::virtual_));
        }
        case OVERRIDE:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::OverrideKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::override_));
        }
        case ABSTRACT:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::AbstractKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::abstract_));
        }
        case INLINE:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::InlineKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::inline_));
        }
        case EXPLICIT:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::ExplicitKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::explicit_));
        }
        case EXTERN:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::ExternKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::external_));
        }
        case SUPPRESS:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::SuppressKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::suppress_));
        }
        case DEFAULT:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::DefaultKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::default_));
        }
        case CONSTEXPR:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::ConstExprKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::constexpr_));
        }
        case CDECL:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::CDeclKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::cdecl_));
        }
        case NOTHROW:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::NothrowKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::nothrow_));
        }
        case THROW:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::ThrowKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::throw_));
        }
        case NEW:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::NewKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::new_));
        }
        case UNIT_TEST:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::UnitTestKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::unit_test_));
        }
        case WINAPI:
        {
            soul::ast::Span span(lexer.GetSpan());
            ++lexer;
            return soul::parser::Match(true, new cmajor::fault::tolerant::ast::SpecifierNode(
                new cmajor::fault::tolerant::ast::WinApiKeywordNode(span, cmajor::fault::tolerant::ast::CompletionContext::none),
                cmajor::fault::tolerant::ast::Specifiers::winapi));
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseSpecifiers(Context& context, Lexer& lexer)
{
    cmajor::fault::tolerant::ast::Specifiers specifiers = cmajor::fault::tolerant::ast::Specifiers::none;
    std::unique_ptr<cmajor::fault::tolerant::ast::SpecifiersNode> specifiersNode(new cmajor::fault::tolerant::ast::SpecifiersNode(
        soul::ast::Span(), cmajor::fault::tolerant::ast::Specifiers::none));
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match specifierMatch = ParseSpecifier(context, lexer);
        if (specifierMatch.hit)
        {
            cmajor::fault::tolerant::ast::SpecifierNode* specifierNode = static_cast<cmajor::fault::tolerant::ast::SpecifierNode*>(specifierMatch.value);
            specifiers = specifiers | specifierNode->GetSpecifier();
            specifiersNode->AddSpecifierNode(specifierNode);
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    specifiersNode->SetSpecifiers(specifiers);
    specifiersNode->Make();
    return soul::parser::Match(true, specifiersNode.release());
}

} // namespace cmajor::fault::tolerant::parser
