// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.classes;

import cmajor.fault.tolerant.parser.attribute;
import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.templates;
import cmajor.fault.tolerant.parser.concepts;
import cmajor.fault.tolerant.parser.alias;
import cmajor.fault.tolerant.parser.enums;
import cmajor.fault.tolerant.parser.constant;
import cmajor.fault.tolerant.parser.delegate;
import cmajor.fault.tolerant.parser.statement;
import cmajor.fault.tolerant.parser.expression;
import cmajor.fault.tolerant.parser.parameter;
import cmajor.fault.tolerant.parser.function;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseBaseClassOrInterface(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match templateIdMatch = ParseTemplateId(context, lexer);
    if (templateIdMatch.hit)
    {
        return templateIdMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(context, lexer);
    if (qualifiedIdMatch.hit)
    {
        return qualifiedIdMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseInheritance(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::ClassNode* classNode)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
    if (*lexer == COLON)
    {
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match baseClassMatch = ParseBaseClassOrInterface(context, lexer);
    if (baseClassMatch.hit)
    {
        classNode->SetColon(colon.release());
        classNode->AddBaseClassOrInterface(static_cast<cmajor::fault::tolerant::ast::Node*>(baseClassMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        if (*lexer == COMMA)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            classNode->AddComma(comma.release());
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match baseClassMatch = ParseBaseClassOrInterface(context, lexer);
        if (baseClassMatch.hit)
        {
            classNode->AddBaseClassOrInterface(static_cast<cmajor::fault::tolerant::ast::Node*>(baseClassMatch.value));
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true);
}

soul::parser::Match ParseInitializer(Context& context, Lexer& lexer)
{
    if (*lexer == THIS)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ThisKeywordNode> thisKeyword;
        thisKeyword.reset(new cmajor::fault::tolerant::ast::ThisKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        std::unique_ptr<cmajor::fault::tolerant::ast::ThisInitializerNode> initializer;
        initializer.reset(new cmajor::fault::tolerant::ast::ThisInitializerNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        initializer->SetThisKeyword(thisKeyword.release());
        if (*lexer == LPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
            lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetLParen(lparen.release());
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
        ParseArgumentList(context, lexer, initializer.get());
        if (*lexer == RPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
            ++lexer;
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
        }
        initializer->Make();
        return soul::parser::Match(true, initializer.release());
    }
    else if (*lexer == BASE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::BaseKeywordNode> baseKeyword;
        baseKeyword.reset(new cmajor::fault::tolerant::ast::BaseKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        std::unique_ptr<cmajor::fault::tolerant::ast::BaseInitializerNode> initializer;
        initializer.reset(new cmajor::fault::tolerant::ast::BaseInitializerNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        initializer->SetBaseKeyword(baseKeyword.release());
        if (*lexer == LPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
            lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetLParen(lparen.release());
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
        ParseArgumentList(context, lexer, initializer.get());
        if (*lexer == RPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
            ++lexer;
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
        }
        initializer->Make();
        return soul::parser::Match(true, initializer.release());
    }
    else if (*lexer == ID)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::MemberInitializerNode> initializer;
        initializer.reset(new cmajor::fault::tolerant::ast::MemberInitializerNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
        if (identifierMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
            id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
            initializer->SetId(id.release());
        }
        if (*lexer == LPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
            lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetLParen(lparen.release());
            ++lexer;
        }
        else
        {
            return soul::parser::Match(false);
        }
        ParseArgumentList(context, lexer, initializer.get());
        if (*lexer == RPAREN)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
            ++lexer;
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
            rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            initializer->SetRParen(rparen.release());
        }
        initializer->Make();
        return soul::parser::Match(true, initializer.release());
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseStaticConstructor(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::IdentifierNode* classId)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifier;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        identifier.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
        if (identifier->Str() != classId->Str())
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::StaticConstructorNode> staticConstructor;
    staticConstructor.reset(new cmajor::fault::tolerant::ast::StaticConstructorNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        staticConstructor->SetAttributes(attributes.release());
    }
    staticConstructor->SetSpecifiers(specifiers.release());
    staticConstructor->SetClassName(identifier.release());
    staticConstructor->SetLParen(lparen.release());
    staticConstructor->SetRParen(rparen.release());
    save = lexer.GetPos();
    if (*lexer == COLON)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        staticConstructor->SetColon(colon.release());
        ++lexer;
        soul::parser::Match initializerMatch = ParseInitializer(context, lexer);
        if (initializerMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::InitializerNode> initializer;
            initializer.reset(static_cast<cmajor::fault::tolerant::ast::InitializerNode*>(initializerMatch.value));
            staticConstructor->AddInitializer(initializer.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
        while (true)
        {
            int64_t save = lexer.GetPos();
            if (*lexer == COMMA)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
                comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                staticConstructor->AddComma(comma.release());
                ++lexer;
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
            soul::parser::Match initializerMatch = ParseInitializer(context, lexer);
            if (initializerMatch.hit)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::InitializerNode> initializer;
                initializer.reset(static_cast<cmajor::fault::tolerant::ast::InitializerNode*>(initializerMatch.value));
                staticConstructor->AddInitializer(initializer.release());
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match constraintMatch = ParseWhereConstraint(context, lexer);
    if (constraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> constraint;
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(constraintMatch.value));
        staticConstructor->SetConstraint(constraint.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match bodyMatch = ParseCompoundStatement(context, lexer);
    if (bodyMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> body;
        body.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(bodyMatch.value));
        staticConstructor->SetBody(body.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            staticConstructor->SetSemicolon(semicolon.release());
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            staticConstructor->SetSemicolon(semicolon.release());
        }
    }
    staticConstructor->Make();
    return soul::parser::Match(true, staticConstructor.release());
}

soul::parser::Match ParseConstructor(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::IdentifierNode* classId)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifier;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        identifier.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
        if (identifier->Str() != classId->Str())
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstructorNode> constructor;
    constructor.reset(new cmajor::fault::tolerant::ast::ConstructorNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        constructor->SetAttributes(attributes.release());
    }
    constructor->SetSpecifiers(specifiers.release());
    constructor->SetClassName(identifier.release());
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, constructor->ParameterList());
    save = lexer.GetPos();
    if (*lexer == COLON)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ColonNode> colon;
        colon.reset(new cmajor::fault::tolerant::ast::ColonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        constructor->SetColon(colon.release());
        soul::parser::Match initializerMatch = ParseInitializer(context, lexer);
        if (initializerMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::InitializerNode> initializer;
            initializer.reset(static_cast<cmajor::fault::tolerant::ast::InitializerNode*>(initializerMatch.value));
            constructor->AddInitializer(initializer.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
        while (true)
        {
            int64_t save = lexer.GetPos();
            if (*lexer == COMMA)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
                comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                constructor->AddComma(comma.release());
                ++lexer;
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
            soul::parser::Match initializerMatch = ParseInitializer(context, lexer);
            if (initializerMatch.hit)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::InitializerNode> initializer;
                initializer.reset(static_cast<cmajor::fault::tolerant::ast::InitializerNode*>(initializerMatch.value));
                constructor->AddInitializer(initializer.release());
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match constraintMatch = ParseWhereConstraint(context, lexer);
    if (constraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> constraint;
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(constraintMatch.value));
        constructor->SetConstraint(constraint.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match bodyMatch = ParseCompoundStatement(context, lexer);
    if (bodyMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> body;
        body.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(bodyMatch.value));
        constructor->SetBody(body.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            constructor->SetSemicolon(semicolon.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    constructor->Make();
    return soul::parser::Match(true, constructor.release());
}

soul::parser::Match ParseDestructor(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::IdentifierNode* classId)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::ComplementNode> complement;
    if (*lexer == CPL)
    {
        complement.reset(new cmajor::fault::tolerant::ast::ComplementNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifier;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        identifier.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
        if (identifier->Str() != classId->Str())
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::DestructorNode> destructor;
    destructor.reset(new cmajor::fault::tolerant::ast::DestructorNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        destructor->SetAttributes(attributes.release());
    }
    destructor->SetSpecifiers(specifiers.release());
    destructor->SetComplement(complement.release());
    destructor->SetClassName(identifier.release());
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        destructor->SetLParen(lparen.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        destructor->SetRParen(rparen.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    save = lexer.GetPos();
    soul::parser::Match constraintMatch = ParseWhereConstraint(context, lexer);
    if (constraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> constraint;
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(constraintMatch.value));
        destructor->SetConstraint(constraint.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match bodyMatch = ParseCompoundStatement(context, lexer);
    if (bodyMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CompoundStatementNode> body;
        body.reset(static_cast<cmajor::fault::tolerant::ast::CompoundStatementNode*>(bodyMatch.value));
        destructor->SetBody(body.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            destructor->SetSemicolon(semicolon.release());
        }
        else
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            destructor->SetSemicolon(semicolon.release());
        }
    }
    destructor->Make();
    return soul::parser::Match(true, destructor.release());
}

soul::parser::Match ParseMemberFunction(Context& context, Lexer& lexer)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::MemberFunctionNode> memberFunction;
    memberFunction.reset(new cmajor::fault::tolerant::ast::MemberFunctionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        memberFunction->SetAttributes(attributes.release());
    }
    memberFunction->SetSpecifiers(specifiers.release());
    memberFunction->SetReturnType(returnType.release());
    memberFunction->SetFunctionGroupId(functionGroupId.release());
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, memberFunction->ParameterList());
    if (!parameterListMatch.hit)
    {
        lexer.SetPos(save);
        return soul::parser::Match(false);
    }
    save = lexer.GetPos();
    if (*lexer == CONST)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ConstKeywordNode> constKeyword;
        constKeyword.reset(new cmajor::fault::tolerant::ast::ConstKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        memberFunction->SetConstKeyword(constKeyword.release());
        ++lexer;
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match whereConstraintMatch = ParseWhereConstraint(context, lexer);
    if (whereConstraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> whereConstraint;
        whereConstraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(whereConstraintMatch.value));
        memberFunction->SetConstraint(whereConstraint.release());
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
        memberFunction->SetBody(compoundStatement.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            memberFunction->SetSemicolon(semicolon.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    memberFunction->Make();
    return soul::parser::Match(true, memberFunction.release());
}

soul::parser::Match ParseConversionFunction(Context& context, Lexer& lexer)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::OperatorKeywordNode> operatorKeyword;
    if (*lexer == OPERATOR)
    {
        operatorKeyword.reset(new cmajor::fault::tolerant::ast::OperatorKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
    if (*lexer == LPAREN)
    {
        lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
    if (*lexer == RPAREN)
    {
        rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ConversionFunctionNode> conversionFunction;
    conversionFunction.reset(new cmajor::fault::tolerant::ast::ConversionFunctionNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        conversionFunction->SetAttributes(attributes.release());
    }
    conversionFunction->SetSpecifiers(specifiers.release());
    conversionFunction->SetOperatorKeyword(operatorKeyword.release());
    conversionFunction->SetType(type.release());
    conversionFunction->SetLParen(lparen.release());
    conversionFunction->SetRParen(rparen.release());
    save = lexer.GetPos();
    if (*lexer == CONST)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ConstKeywordNode> constKeyword;
        constKeyword.reset(new cmajor::fault::tolerant::ast::ConstKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conversionFunction->SetConstKeyword(constKeyword.release());
        ++lexer;
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match whereConstraintMatch = ParseWhereConstraint(context, lexer);
    if (whereConstraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> whereConstraint;
        whereConstraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(whereConstraintMatch.value));
        conversionFunction->SetConstraint(whereConstraint.release());
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
        conversionFunction->SetBody(compoundStatement.release());
    }
    else
    {
        lexer.SetPos(save);
        if (*lexer == SEMICOLON)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
            semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
            conversionFunction->SetSemicolon(semicolon.release());
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    conversionFunction->Make();
    return soul::parser::Match(true, conversionFunction.release());
}

soul::parser::Match ParseMemberVariable(Context& context, Lexer& lexer)
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
    save = lexer.GetPos();
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
    std::unique_ptr<cmajor::fault::tolerant::ast::MemberVariableNode> memberVariable;
    memberVariable.reset(new cmajor::fault::tolerant::ast::MemberVariableNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        memberVariable->SetAttributes(attributes.release());
    }
    memberVariable->SetSpecifiers(specifiers.release());
    memberVariable->SetType(type.release());
    memberVariable->SetId(id.release());
    memberVariable->SetSemicolon(semicolon.release());
    memberVariable->Make();
    return soul::parser::Match(true, memberVariable.release());
}

soul::parser::Match ParseClassMember(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::IdentifierNode* classId)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match staticConstructorMatch = ParseStaticConstructor(context, lexer, classId);
    if (staticConstructorMatch.hit)
    {
        return staticConstructorMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match constructorMatch = ParseConstructor(context, lexer, classId);
    if (constructorMatch.hit)
    {
        return constructorMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match destructorMatch = ParseDestructor(context, lexer, classId);
    if (destructorMatch.hit)
    {
        return destructorMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match memberFunctionMatch = ParseMemberFunction(context, lexer);
    if (memberFunctionMatch.hit)
    {
        return memberFunctionMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match conversionFunction = ParseConversionFunction(context, lexer);
    if (conversionFunction.hit)
    {
        return conversionFunction;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match memberVariableMatch = ParseMemberVariable(context, lexer);
    if (memberVariableMatch.hit)
    {
        return memberVariableMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match usingAliasMatch = ParseUsingAlias(context, lexer);
    if (usingAliasMatch.hit)
    {
        return usingAliasMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match typedefMatch = ParseTypedef(context, lexer);
    if (typedefMatch.hit)
    {
        return typedefMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match classMatch = ParseClass(context, lexer);
    if (classMatch.hit)
    {
        return classMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match enumTypeMatch = ParseEnumType(context, lexer);
    if (enumTypeMatch.hit)
    {
        return enumTypeMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match constantMatch = ParseConstant(context, lexer);
    if (constantMatch.hit)
    {
        return constantMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match delegateMatch = ParseDelegate(context, lexer);
    if (delegateMatch.hit)
    {
        return delegateMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match classDelegateMatch = ParseClassDelegate(context, lexer);
    if (classDelegateMatch.hit)
    {
        return classDelegateMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

void ParseClassContent(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::ClassNode* classNode)
{
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match classMemberMatch = ParseClassMember(context, lexer, classNode->Identifier());
        if (classMemberMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::Node> member;
            member.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(classMemberMatch.value));
            classNode->AddMember(member.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseClass(Context& context, Lexer& lexer)
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
        if ((specifiers->GetSpecifiers() & cmajor::fault::tolerant::ast::Specifiers::new_) != cmajor::fault::tolerant::ast::Specifiers::none)
        {
            return soul::parser::Match(false);
        }
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ClassKeywordNode> classKeyword;
    if (*lexer == CLASS)
    {
        classKeyword.reset(new cmajor::fault::tolerant::ast::ClassKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> identifier;
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        identifier.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ClassNode> classNode;
    classNode.reset(new cmajor::fault::tolerant::ast::ClassNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
    if (attributes)
    {
        classNode->SetAttributes(attributes.release());
    }
    classNode->SetSpecifiers(specifiers.release());
    classNode->SetClassKeyword(classKeyword.release());
    classNode->SetIdentifier(identifier.release());
    save = lexer.GetPos();
    soul::parser::Match templateParemeterListMatch = ParseTemplateParameterList(context, lexer, classNode->TemplateParameterList());
    if (!templateParemeterListMatch.hit)
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match inheritanceMatch = ParseInheritance(context, lexer, classNode.get());
    if (!inheritanceMatch.hit)
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match constraintMatch = ParseWhereConstraint(context, lexer);
    if (constraintMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> constraint;
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(constraintMatch.value));
        classNode->SetConstraint(constraint.release());
    }
    else
    {
        lexer.SetPos(save);
    }
    if (*lexer == LBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace;
        lbrace.reset(new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        classNode->SetLBrace(lbrace.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    ParseClassContent(context, lexer, classNode.get());
    if (*lexer == RBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        classNode->SetRBrace(rbrace.release());
        ++lexer;
    }
    else
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace;
        rbrace.reset(new cmajor::fault::tolerant::ast::RBraceNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        classNode->SetRBrace(rbrace.release());
    }
    classNode->Make();
    return soul::parser::Match(true, classNode.release());
}

} // namespace cmajor::fault::tolerant::parser

