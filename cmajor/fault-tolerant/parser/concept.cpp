// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.concepts;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.expression;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.parameter;
import cmajor.fault.tolerant.parser.function;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseConstraintExpr(Context& context, Lexer& lexer);

soul::parser::Match ParseRefinement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ConceptIdNode> refinement;
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
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(context, lexer);
    if (qualifiedIdMatch.hit)
    {
        refinement.reset(new cmajor::fault::tolerant::ast::ConceptIdNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        refinement->SetRefinedConcept(static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(qualifiedIdMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == LANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LAngleNode> langle(
            new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        refinement->SetLAngle(langle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> typeParameter;
        typeParameter.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
        refinement->AddTypeParameter(typeParameter.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
        if (*lexer == COMMA)
        {
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
        if (identifierMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> typeParameter;
            typeParameter.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
            refinement->AddComma(comma.release());
            refinement->AddTypeParameter(typeParameter.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    if (*lexer == RANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle(
            new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        refinement->SetRAngle(rangle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    refinement->Make();
    return soul::parser::Match(true, refinement.release());
}

soul::parser::Match ParseParenthesizedConstraintExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ParenthesizedConstraintNode> constraint;
    constraint.reset(new cmajor::fault::tolerant::ast::ParenthesizedConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstraintNode> constraintExpr;
    soul::parser::Match constraintExprMatch = ParseConstraintExpr(context, lexer);
    if (constraintExprMatch.hit)
    {
        constraintExpr.reset(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(constraintExprMatch.value));
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
    constraint->SetLParen(lparen.release());
    constraint->SetConstraintExpr(constraintExpr.release());
    constraint->SetRParen(rparen.release());
    constraint->Make();
    return soul::parser::Match(true, constraint.release());
}

soul::parser::Match ParsePredicateConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::PredicateConstraintNode> constraint;
    soul::parser::Match invokeExprMatch = ParseInvokeExpr(context, lexer);
    if (invokeExprMatch.hit)
    {
        constraint.reset(new cmajor::fault::tolerant::ast::PredicateConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        constraint->SetInvokeExpr(static_cast<cmajor::fault::tolerant::ast::Node*>(invokeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    constraint->Make();
    return soul::parser::Match(true, constraint.release());
}

soul::parser::Match ParseIsConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IsKeywordNode> isKeyword;
    if (*lexer == IS)
    {
        isKeyword.reset(new cmajor::fault::tolerant::ast::IsKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> conceptOrTypeName;
    soul::parser::Match conceptOrTypeNameMatch = ParseTypeExpr(context, lexer);
    if (conceptOrTypeNameMatch.hit)
    {
        conceptOrTypeName.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(conceptOrTypeNameMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IsConstraintNode> isConstraintNode(
        new cmajor::fault::tolerant::ast::IsConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    isConstraintNode->SetTypeExpr(typeExpr.release());
    isConstraintNode->SetIsKeyword(isKeyword.release());
    isConstraintNode->SetConceptOrTypeName(conceptOrTypeName.release());
    isConstraintNode->Make();
    return soul::parser::Match(true, isConstraintNode.release());
}

soul::parser::Match ParseMultiparamConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::MultiparamConstraintNode> constraint;
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(context, lexer);
    if (qualifiedIdMatch.hit)
    {
        constraint.reset(new cmajor::fault::tolerant::ast::MultiparamConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        std::unique_ptr<cmajor::fault::tolerant::ast::QualifiedIdNode> conceptName;
        conceptName.reset(static_cast<cmajor::fault::tolerant::ast::QualifiedIdNode*>(qualifiedIdMatch.value));
        constraint->SetConceptName(conceptName.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::LAngleNode> langle;
    if (*lexer == LANGLE)
    {
        langle.reset(new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        constraint->SetLAngle(langle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
        constraint->AddTypeExpr(typeExpr.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
        if (*lexer == COMMA)
        {
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
        if (typeExprMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
            typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
            constraint->AddComma(comma.release());
            constraint->AddTypeExpr(typeExpr.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle;
    if (*lexer == RANGLE)
    {
        rangle.reset(new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        constraint->SetRAngle(rangle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    constraint->Make();
    return soul::parser::Match(true, constraint.release());
}

soul::parser::Match ParseAtomicConstraintExpr(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match predicateConstraintMatch = ParsePredicateConstraint(context, lexer);
    if (predicateConstraintMatch.hit)
    {
        return predicateConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match isConstraintMatch = ParseIsConstraint(context, lexer);
    if (isConstraintMatch.hit)
    {
        return isConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match multiparamConstraintMatch = ParseMultiparamConstraint(context, lexer);
    if (multiparamConstraintMatch.hit)
    {
        return multiparamConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParsePrimaryConstraintExpr(Context& context, Lexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match parenthesizeExprMatch = ParseParenthesizedConstraintExpr(context, lexer);
    if (parenthesizeExprMatch.hit)
    {
        return parenthesizeExprMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match atomicExprMatch = ParseAtomicConstraintExpr(context, lexer);
    if (atomicExprMatch.hit)
    {
        return atomicExprMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseConjunctiveConstraintExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstraintNode> constraint;
    soul::parser::Match primaryConstraintExprMatch = ParsePrimaryConstraintExpr(context, lexer);
    if (primaryConstraintExprMatch.hit)
    {
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(primaryConstraintExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::AndKeywordNode> andKeyword;
        if (*lexer == AND)
        {
            andKeyword.reset(new cmajor::fault::tolerant::ast::AndKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match primaryConstraintExprMatch = ParsePrimaryConstraintExpr(context, lexer);
        if (primaryConstraintExprMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ConjunctiveConstraintNode> conjunctiveConstraint;
            conjunctiveConstraint.reset(new cmajor::fault::tolerant::ast::ConjunctiveConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            conjunctiveConstraint->SetLeft(constraint.release());
            conjunctiveConstraint->SetOp(andKeyword.release());
            conjunctiveConstraint->SetRight(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(primaryConstraintExprMatch.value));
            constraint.reset(conjunctiveConstraint.release());
            constraint->Make();
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true, constraint.release());
}

soul::parser::Match ParseDisjunctiveConstraintExpr(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ConstraintNode> constraint;
    soul::parser::Match conjunctiveConstraintExprMatch = ParseConjunctiveConstraintExpr(context, lexer);
    if (conjunctiveConstraintExprMatch.hit)
    {
        constraint.reset(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(conjunctiveConstraintExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::OrKeywordNode> orKeyword;
        if (*lexer == OR)
        {
            orKeyword.reset(new cmajor::fault::tolerant::ast::OrKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match conjunctiveConstraintExprMatch = ParseConjunctiveConstraintExpr(context, lexer);
        if (conjunctiveConstraintExprMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::DisjunctiveConstraintNode> disjunctivceConstraint;
            disjunctivceConstraint.reset(new cmajor::fault::tolerant::ast::DisjunctiveConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            disjunctivceConstraint->SetLeft(constraint.release());
            disjunctivceConstraint->SetOp(orKeyword.release());
            disjunctivceConstraint->SetRight(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(conjunctiveConstraintExprMatch.value));
            constraint.reset(disjunctivceConstraint.release());
            constraint->Make();
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true, constraint.release());
}

soul::parser::Match ParseConstraintExpr(Context& context, Lexer& lexer)
{
    soul::parser::Match disjunctiveConstraintExprMatch = ParseDisjunctiveConstraintExpr(context, lexer);
    return disjunctiveConstraintExprMatch;
}

soul::parser::Match ParseWhereConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> whereConstraint;
    std::unique_ptr<cmajor::fault::tolerant::ast::WhereKeywordNode> whereKeyword;
    if (*lexer == WHERE)
    {
        whereKeyword.reset(new cmajor::fault::tolerant::ast::WhereKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match constraintExprMatch = ParseConstraintExpr(context, lexer);
    if (constraintExprMatch.hit)
    {
        whereConstraint.reset(new cmajor::fault::tolerant::ast::WhereConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
        whereConstraint->SetWhereKeyword(whereKeyword.release());
        whereConstraint->SetConstraintExpr(static_cast<cmajor::fault::tolerant::ast::ConstraintNode*>(constraintExprMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    whereConstraint->Make();
    return soul::parser::Match(true, whereConstraint.release());
}

soul::parser::Match ParseTypeNameConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeNameKeywordNode> typeNameKeyword;
    if (*lexer == TYPENAME)
    {
        typeNameKeyword.reset(new cmajor::fault::tolerant::ast::TypeNameKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> typeExpr;
    soul::parser::Match typeExprMatch = ParseTypeExpr(context, lexer);
    if (typeExprMatch.hit)
    {
        typeExpr.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeExprMatch.value));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::TypeNameConstraintNode> typeNameConstraint;
    typeNameConstraint.reset(new cmajor::fault::tolerant::ast::TypeNameConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    typeNameConstraint->SetTypeNameKeyword(typeNameKeyword.release());
    typeNameConstraint->SetTypeExpr(typeExpr.release());
    typeNameConstraint->SetSemicolon(semicolon.release());
    typeNameConstraint->Make();
    return soul::parser::Match(true, typeNameConstraint.release());
}

soul::parser::Match ParseConstructorConstraint(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* firstTypeParameter)
{
    if (*lexer == EXPLICIT)
    {
        ++lexer;
    }
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value);
        if (idNode->Str() == firstTypeParameter->Str())
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ConstructorConstraintNode> constructorConstraint;
            constructorConstraint.reset(new cmajor::fault::tolerant::ast::ConstructorConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            constructorConstraint->SetTypeParamId(idNode);
            soul::parser::Match pararameterListMatch = ParseParameterList(context, lexer, constructorConstraint->ParameterList());
            if (pararameterListMatch.hit)
            {
                if (*lexer == SEMICOLON)
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
                    semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                    constructorConstraint->SetSemicolon(semicolon.release());
                    ++lexer;
                    constructorConstraint->Make();
                    return soul::parser::Match(true, constructorConstraint.release());
                }
            }
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseDestructorConstraint(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* firstTypeParameter)
{
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
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        cmajor::fault::tolerant::ast::IdentifierNode* idNode = static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value);
        if (idNode->Str() == firstTypeParameter->Str())
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::DestructorConstraintNode> destructorConstraint;
            destructorConstraint.reset(new cmajor::fault::tolerant::ast::DestructorConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
            destructorConstraint->SetComplement(complement.release());
            destructorConstraint->SetTypeParamId(idNode);
            if (*lexer == LPAREN)
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::LParenNode> lparen;
                lparen.reset(new cmajor::fault::tolerant::ast::LParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                ++lexer;
                if (*lexer == RPAREN)
                {
                    std::unique_ptr<cmajor::fault::tolerant::ast::RParenNode> rparen;
                    rparen.reset(new cmajor::fault::tolerant::ast::RParenNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                    ++lexer;
                    if (*lexer == SEMICOLON)
                    {
                        std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
                        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
                        ++lexer;
                        destructorConstraint->SetLParen(lparen.release());
                        destructorConstraint->SetRParen(rparen.release());
                        destructorConstraint->SetSemicolon(semicolon.release());
                        destructorConstraint->Make();
                        return soul::parser::Match(true, destructorConstraint.release());
                    }
                }
            }
        }
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseMemberFunctionConstraint(Context& context, Lexer& lexer)
{
    soul::parser::Match returnTypeMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> returnType;
    if (returnTypeMatch.hit)
    {
        returnType.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(returnTypeMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> typeParamId;
    soul::parser::Match typeParamMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (typeParamMatch.hit)
    {
        typeParamId.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(typeParamMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::DotNode> dot;
    if (*lexer == DOT)
    {
        dot.reset(new cmajor::fault::tolerant::ast::DotNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
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
    std::unique_ptr<cmajor::fault::tolerant::ast::MemberFunctionConstraintNode> memberFunctionConstraint;
    memberFunctionConstraint.reset(new cmajor::fault::tolerant::ast::MemberFunctionConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    memberFunctionConstraint->SetReturnType(returnType.release());
    memberFunctionConstraint->SetTypeParamId(typeParamId.release());
    memberFunctionConstraint->SetDot(dot.release());
    memberFunctionConstraint->SetFunctionGroupId(functionGroupId.release());
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, memberFunctionConstraint->ParameterList());
    if (!parameterListMatch.hit)
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
    memberFunctionConstraint->SetSemicolon(semicolon.release());
    memberFunctionConstraint->Make();
    return soul::parser::Match(true, memberFunctionConstraint.release());
}

soul::parser::Match ParseFunctionConstraint(Context& context, Lexer& lexer)
{
    soul::parser::Match returnTypeMatch = ParseTypeExpr(context, lexer);
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> returnType;
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
    std::unique_ptr<cmajor::fault::tolerant::ast::FunctionConstraintNode> functionConstraint;
    functionConstraint.reset(new cmajor::fault::tolerant::ast::FunctionConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    functionConstraint->SetReturnType(returnType.release());
    functionConstraint->SetFunctionGroupId(functionGroupId.release());
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, functionConstraint->ParameterList());
    if (!parameterListMatch.hit)
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
    functionConstraint->SetSemicolon(semicolon.release());
    functionConstraint->Make();
    return soul::parser::Match(true, functionConstraint.release());
}

soul::parser::Match ParseSignatureConstraint(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* firstTypeParameter)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match constructorConstraintMatch = ParseConstructorConstraint(context, lexer, firstTypeParameter);
    if (constructorConstraintMatch.hit)
    {
        return constructorConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match destructorConstraintMatch = ParseDestructorConstraint(context, lexer, firstTypeParameter);
    if (destructorConstraintMatch.hit)
    {
        return destructorConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match memberFunctionConstraintMatch = ParseMemberFunctionConstraint(context, lexer);
    if (memberFunctionConstraintMatch.hit)
    {
        return memberFunctionConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match functionConstraintMatch = ParseFunctionConstraint(context, lexer);
    if (functionConstraintMatch.hit)
    {
        return functionConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseEmbeddedConstraint(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::WhereConstraintNode> whereConstraint;
    soul::parser::Match whereConstraintMatch = ParseWhereConstraint(context, lexer);
    if (whereConstraintMatch.hit)
    {
        whereConstraint.reset(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(whereConstraintMatch.value));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::EmbeddedConstraintNode> embeddedConstraint;
    embeddedConstraint.reset(new cmajor::fault::tolerant::ast::EmbeddedConstraintNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    embeddedConstraint->SetWhereConstraint(whereConstraint.release());
    embeddedConstraint->SetSemicolon(semicolon.release());
    embeddedConstraint->Make();
    return soul::parser::Match(true, embeddedConstraint.release());
}

soul::parser::Match ParseConceptBodyConstraint(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* firstTypeParameter)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match typeNameConstraintMatch = ParseTypeNameConstraint(context, lexer);
    if (typeNameConstraintMatch.hit)
    {
        return typeNameConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match signatureConstraintMatch = ParseSignatureConstraint(context, lexer, firstTypeParameter);
    if (signatureConstraintMatch.hit)
    {
        return signatureConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match embeddedConstraintMatch = ParseEmbeddedConstraint(context, lexer);
    if (embeddedConstraintMatch.hit)
    {
        return embeddedConstraintMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseAxiomStatement(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> expression;
    soul::parser::Match expressionMatch = ParseExpression(context, lexer);
    if (expressionMatch.hit)
    {
        expression.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(expressionMatch.value));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::AxiomStatementNode> axiomStatement;
    axiomStatement.reset(new cmajor::fault::tolerant::ast::AxiomStatementNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    axiomStatement->SetExpression(expression.release());
    axiomStatement->SetSemicolon(semicolon.release());
    axiomStatement->Make();
    return soul::parser::Match(true, axiomStatement.release());
}

void ParseAxiomBody(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::AxiomNode* axiomNode)
{
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::AxiomStatementNode> axiomStatement;
        soul::parser::Match axiomStatementMatch = ParseAxiomStatement(context, lexer);
        if (axiomStatementMatch.hit)
        {
            axiomStatement.reset(static_cast<cmajor::fault::tolerant::ast::AxiomStatementNode*>(axiomStatementMatch.value));
            axiomNode->AddAxiomStatement(axiomStatement.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
}

soul::parser::Match ParseAxiom(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::AxiomKeywordNode> axiomKeyword;
    if (*lexer == AXIOM)
    {
        axiomKeyword.reset(new cmajor::fault::tolerant::ast::AxiomKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> axiomId;
    int64_t save = lexer.GetPos();
    soul::parser::Match axiomIdMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (axiomIdMatch.hit)
    {
        axiomId.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(axiomIdMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::AxiomNode> axiomNode;
    axiomNode.reset(new cmajor::fault::tolerant::ast::AxiomNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    save = lexer.GetPos();
    soul::parser::Match parameterListMatch = ParseParameterList(context, lexer, axiomNode->ParameterList());
    if (!parameterListMatch.hit)
    {
        lexer.SetPos(save);
    }
    if (*lexer == LBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace(
            new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        axiomNode->SetLBrace(lbrace.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    ParseAxiomBody(context, lexer, axiomNode.get());
    if (*lexer == RBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace(
            new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        axiomNode->SetRBrace(rbrace.release());
    }
    else
    {
        return soul::parser::Match(false);
    }
    axiomNode->Make();
    return soul::parser::Match(true, axiomNode.release());
}

void ParseConceptBody(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::ConceptNode* conceptNode)
{
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match conceptBodyConstraintMatch = ParseConceptBodyConstraint(context, lexer, conceptNode->GetFirstTypeParameter());
        if (conceptBodyConstraintMatch.hit)
        {
            conceptNode->AddNode(static_cast<cmajor::fault::tolerant::ast::Node*>(conceptBodyConstraintMatch.value));
        }
        else
        {
            lexer.SetPos(save);
            soul::parser::Match axiomMatch = ParseAxiom(context, lexer);
            if (axiomMatch.hit)
            {
                conceptNode->AddNode(static_cast<cmajor::fault::tolerant::ast::Node*>(axiomMatch.value));
            }
            else
            {
                lexer.SetPos(save);
                break;
            }
        }
    }
}

struct ParsingConceptGuard
{
    ParsingConceptGuard(Context& context_) : context(context_) { context.BeginParsingConcept(); }
    ~ParsingConceptGuard() { context.EndParsingConcept(); }
    Context& context;
};

soul::parser::Match ParseConcept(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::ConceptNode> conceptNode(
        new cmajor::fault::tolerant::ast::ConceptNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    ParsingConceptGuard guard(context);
    soul::parser::Match specifiersMatch = ParseSpecifiers(context, lexer);
    if (specifiersMatch.hit)
    {
        cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifiersMatch.value);
        conceptNode->SetSpecifiersNode(specifiersNode);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == CONCEPT)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::ConceptKeywordNode> conceptKeyword(
            new cmajor::fault::tolerant::ast::ConceptKeywordNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conceptNode->SetConceptKeyword(conceptKeyword.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match identifierMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (identifierMatch.hit)
    {
        conceptNode->SetConceptId(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(identifierMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == LANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LAngleNode> langle(
            new cmajor::fault::tolerant::ast::LAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conceptNode->SetLAngle(langle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    soul::parser::Match typeParamMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (typeParamMatch.hit)
    {
        conceptNode->AddTypeParameter(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(typeParamMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        std::unique_ptr<cmajor::fault::tolerant::ast::CommaNode> comma;
        if (*lexer == COMMA)
        {
            comma.reset(new cmajor::fault::tolerant::ast::CommaNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        soul::parser::Match typeParamMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
        if (typeParamMatch.hit)
        {
            conceptNode->AddComma(comma.release());
            conceptNode->AddTypeParameter(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(typeParamMatch.value));
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    if (*lexer == RANGLE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RAngleNode> rangle(
            new cmajor::fault::tolerant::ast::RAngleNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conceptNode->SetRAngle(rangle.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    int64_t save = lexer.GetPos();
    soul::parser::Match refinementMatch = ParseRefinement(context, lexer);
    if (refinementMatch.hit)
    {
        conceptNode->SetRefinement(static_cast<cmajor::fault::tolerant::ast::ConceptIdNode*>(refinementMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    save = lexer.GetPos();
    soul::parser::Match whereConstraintMatch = ParseWhereConstraint(context, lexer);
    if (whereConstraintMatch.hit)
    {
        conceptNode->SetWhereConstraint(static_cast<cmajor::fault::tolerant::ast::WhereConstraintNode*>(whereConstraintMatch.value));
    }
    else
    {
        lexer.SetPos(save);
    }
    if (*lexer == LBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::LBraceNode> lbrace(
            new cmajor::fault::tolerant::ast::LBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conceptNode->SetLBrace(lbrace.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    ParseConceptBody(context, lexer, conceptNode.get());
    if (*lexer == RBRACE)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::RBraceNode> rbrace(
            new cmajor::fault::tolerant::ast::RBraceNode(lexer.GetSpan(), cmajor::fault::tolerant::ast::CompletionContext::none));
        conceptNode->SetRBrace(rbrace.release());
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    conceptNode->Make();
    return soul::parser::Match(true, conceptNode.release());
}

} // namespace cmajor::fault::tolerant::parser
