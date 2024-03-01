// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.templates;

import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

struct TemplateIdGuard
{
    TemplateIdGuard(Context& context_, Lexer& lexer_) : context(context_), lexer(lexer_), decLeftAngleCount(false) 
    { 
        context.BeginParsingTemplateId(); 
    }
    ~TemplateIdGuard() 
    { 
        context.EndParsingTemplateId(); 
        if (decLeftAngleCount)
        {
            auto vars = static_cast<cmajor::lexer::CmajorLexer_Variables*>(lexer.GetVariables());
            --vars->leftAngleCount;
        }
    }
    void BeginLeftAngle() 
    { 
        auto vars = static_cast<cmajor::lexer::CmajorLexer_Variables*>(lexer.GetVariables());
        ++vars->leftAngleCount;
        decLeftAngleCount = true; 
    }
    Context& context;
    Lexer& lexer;
    bool decLeftAngleCount;
};

soul::parser::Match ParseTemplateId(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TemplateIdNode> templateIdNode;
    TemplateIdGuard templateIdGuard(context, lexer);
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(context, lexer);
    if (qualifiedIdMatch.hit)
    {
        templateIdNode.reset(new cmajor::fault::tolerant::ast::TemplateIdNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none,
            static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(qualifiedIdMatch.value)));
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == LANGLE)
    {
        templateIdNode->SetLAngle(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        templateIdGuard.BeginLeftAngle();
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match templateArgMatch = ParseTypeExpr(context, lexer);
    if (templateArgMatch.hit)
    {
        templateIdNode->AddTemplateArgument(static_cast<cmajor::fault::tolerant::ast::TypeExprNode*>(templateArgMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> commaNode;
        int64_t save = lexer.GetPos();
        if (*lexer == COMMA)
        {
            commaNode.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match templateArgMatch = ParseTypeExpr(context, lexer);
        if (templateArgMatch.hit)
        {
            templateIdNode->AddComma(commaNode.release());
            templateIdNode->AddTemplateArgument(static_cast<cmajor::fault::tolerant::ast::TypeExprNode*>(templateArgMatch.value));
        }
        else
        {
            return soul::parser::Match(false);
        }
    }
    if (*lexer == RANGLE)
    {
        templateIdNode->SetRAngle(new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    templateIdNode->Make();
    return soul::parser::Match(true, templateIdNode.release());
}

soul::parser::Match ParseTemplateParameter(Context& context, Lexer& lexer)
{
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
    std::unique_ptr<cmajor::fault::tolerant::ast::TemplateParameterNode> templateParameter;
    templateParameter.reset(new cmajor::fault::tolerant::ast::TemplateParameterNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    templateParameter->SetIdentifier(identifier.release());
    std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
    if (*lexer == ASSIGN)
    {
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        templateParameter->SetAssign(assign.release());
        ++lexer;
    }
    else
    {
        templateParameter->Make();
        return soul::parser::Match(true, templateParameter.release());
    }
    soul::parser::Match defaultValueMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> defaultValue;
    if (defaultValueMatch.hit)
    {
        defaultValue.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(defaultValueMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    templateParameter->SetDefaultValue(defaultValue.release());
    templateParameter->Make();
    return soul::parser::Match(true, templateParameter.release());
}

soul::parser::Match ParseTemplateParameterList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::TemplateParameterListNode* templateParameterList)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::LAngleNode> langle;
    if (*lexer == LANGLE)
    {
        langle.reset(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        templateParameterList->SetLAngle(langle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match templateParameterMatch = ParseTemplateParameter(context, lexer);
    if (templateParameterMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::TemplateParameterNode> templateParameter;
        templateParameter.reset(static_cast<cmajor::fault::tolerant::ast::TemplateParameterNode*>(templateParameterMatch.value));
        templateParameterList->AddTemplateParameter(templateParameter.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
        int64_t save = lexer.GetPos();
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
        soul::parser::Match templateParameterMatch = ParseTemplateParameter(context, lexer);
        if (templateParameterMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::TemplateParameterNode> templateParameter;
            templateParameter.reset(static_cast<cmajor::fault::tolerant::ast::TemplateParameterNode*>(templateParameterMatch.value));
            templateParameterList->AddComma(comma.release());
            templateParameterList->AddTemplateParameter(templateParameter.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    if (*lexer == RANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle;
        rangle.reset(new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        templateParameterList->SetRAngle(rangle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true);
}

soul::parser::Match ParseInstantiationRequest(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::NewKeywordNode> newKeyword;
    if (*lexer == NEW)
    {
        newKeyword.reset(new cmajor::fault::tolerant::ast::NewKeywordNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
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
    std::unique_ptr<cmajor::fault::tolerant::ast::TemplateIdNode> templateId;
    soul::parser::Match templateIdMatch = ParseTemplateId(context, lexer);
    if (templateIdMatch.hit)
    {
        templateId.reset(static_cast<cmajor::fault::tolerant::ast::TemplateIdNode*>(templateIdMatch.value));
    }
    else
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
    std::unique_ptr<cmajor::fault::tolerant::ast::FullInstantiationRequest> instantiationRequest;
    instantiationRequest.reset(new cmajor::fault::tolerant::ast::FullInstantiationRequest(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    instantiationRequest->SetNewKeyword(newKeyword.release());
    instantiationRequest->SetClassKeyword(classKeyword.release());
    instantiationRequest->SetTemplateId(templateId.release());
    instantiationRequest->SetSemicolon(semicolon.release());
    instantiationRequest->Make();
    return soul::parser::Match(true, instantiationRequest.release());
}

} // namespace cmajor::fault::tolerant::parser
