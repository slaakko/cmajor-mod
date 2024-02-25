// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.classes;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

ClassNode::ClassNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), templateParameterList(), baseClassAndInterfaceList(soul::ast::Span(), CompletionContext::none)
{
}

void ClassNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void ClassNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void ClassNode::SetClassKeyword(ClassKeywordNode* classKeyword_)
{
    classKeyword.reset(classKeyword_);
    AddChildNode(classKeyword.get());
}

void ClassNode::SetIdentifier(IdentifierNode* identifier_)
{
    identifier.reset(identifier_);
    AddChildNode(identifier.get());
}

void ClassNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void ClassNode::AddComma(CommaNode* comma)
{
    baseClassAndInterfaceList.AddComma(comma);
}

void ClassNode::AddBaseClassOrInterface(Node* baseClassOrInterface)
{
    baseClassAndInterfaceList.AddNode(baseClassOrInterface);
}

void ClassNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void ClassNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void ClassNode::AddMember(Node* member)
{
    members.AddNode(member);
}

void ClassNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* ClassNode::Clone() const
{
    ClassNode* clone = new ClassNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetClassKeyword(static_cast<ClassKeywordNode*>(classKeyword->Clone()));
    clone->SetIdentifier(static_cast<IdentifierNode*>(identifier->Clone()));
    clone->templateParameterList.CloneFrom(templateParameterList);
    if (colon)
    {
        clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    }
    clone->baseClassAndInterfaceList.CloneFrom(baseClassAndInterfaceList);
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& node : members.Nodes())
    {
        clone->AddMember(node->Clone());
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void ClassNode::Make()
{
    templateParameterList.Make();
    AddChildNode(&templateParameterList);
    baseClassAndInterfaceList.Make();
    AddChildNode(&baseClassAndInterfaceList);
    members.Make();
    AddChildNode(&members);
    SyntaxNode::Make();
}

void ClassNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitializerNode::InitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), argumentList(soul::ast::Span(), CompletionContext::none)
{
}

void InitializerNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void InitializerNode::AddNode(Node* argument)
{
    argumentList.AddNode(argument);
}

void InitializerNode::AddComma(CommaNode* comma)
{
    argumentList.AddComma(comma);
}

void InitializerNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

ThisInitializerNode::ThisInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_) : InitializerNode(span_, completionContext_)
{
}

void ThisInitializerNode::SetThisKeyword(ThisKeywordNode* thisKeyword_)
{
    thisKeyword.reset(thisKeyword_);
    AddChildNode(thisKeyword.get());
}

Node* ThisInitializerNode::Clone() const
{
    ThisInitializerNode* clone = new ThisInitializerNode(GetSpan(), GetCompletionContext());
    clone->SetThisKeyword(static_cast<ThisKeywordNode*>(thisKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(LParen()->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(RParen()->Clone()));
    clone->ArgumentList().CloneFrom(ArgumentList());
    clone->Make();
    return clone;
}

void ThisInitializerNode::Make()
{
    ArgumentList().Make();
    AddChildNode(&ArgumentList());
    InitializerNode::Make();
}

void ThisInitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BaseInitializerNode::BaseInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_) : InitializerNode(span_, completionContext_)
{
}

void BaseInitializerNode::SetBaseKeyword(BaseKeywordNode* baseKeyword_)
{
    baseKeyword.reset(baseKeyword_);
    AddChildNode(baseKeyword.get());
}

Node* BaseInitializerNode::Clone() const
{
    BaseInitializerNode* clone = new BaseInitializerNode(GetSpan(), GetCompletionContext());
    clone->SetBaseKeyword(static_cast<BaseKeywordNode*>(baseKeyword->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(LParen()->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(RParen()->Clone()));
    clone->ArgumentList().CloneFrom(ArgumentList());
    clone->Make();
    return clone;
}

void BaseInitializerNode::Make()
{
    ArgumentList().Make();
    AddChildNode(&ArgumentList());
    InitializerNode::Make();
}

void BaseInitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberInitializerNode::MemberInitializerNode(const soul::ast::Span& span_, CompletionContext completionContext_) : InitializerNode(span_, completionContext_)
{
}

void MemberInitializerNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

Node* MemberInitializerNode::Clone() const
{
    MemberInitializerNode* clone = new MemberInitializerNode(GetSpan(), GetCompletionContext());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(LParen()->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(RParen()->Clone()));
    clone->ArgumentList().CloneFrom(ArgumentList());
    clone->Make();
    return clone;
}

void MemberInitializerNode::Make()
{
    ArgumentList().Make();
    AddChildNode(&ArgumentList());
    InitializerNode::Make();
}

void MemberInitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitializerListNode::InitializerListNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ListNode(span_, completionContext_)
{
}

void InitializerListNode::AddInitializer(InitializerNode* initializer)
{
    AddNode(initializer);
    initializers.push_back(initializer);
}

void InitializerListNode::CloneFrom(const InitializerListNode& that)
{
    for (const auto& node : that.Nodes())
    {
        if (node->IsComma())
        {
            AddComma(static_cast<CommaNode*>(node->Clone()));
        }
        else
        {
            AddInitializer(static_cast<InitializerNode*>(node->Clone()));
        }
    }
}

StaticConstructorNode::StaticConstructorNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), initializerList(soul::ast::Span(), CompletionContext::none)
{
}

void StaticConstructorNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void StaticConstructorNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void StaticConstructorNode::SetClassName(IdentifierNode* className_)
{
    className.reset(className_);
    AddChildNode(className.get());
}

void StaticConstructorNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void StaticConstructorNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void StaticConstructorNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.release());
}

void StaticConstructorNode::AddInitializer(InitializerNode* initializer)
{
    initializerList.AddInitializer(initializer);
}

void StaticConstructorNode::AddComma(CommaNode* comma)
{
    initializerList.AddComma(comma);
}

void StaticConstructorNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void StaticConstructorNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void StaticConstructorNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* StaticConstructorNode::Clone() const
{
    StaticConstructorNode* clone = new StaticConstructorNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetClassName(static_cast<IdentifierNode*>(className->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    if (colon)
    {
        clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    }
    clone->initializerList.CloneFrom(initializerList);
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void StaticConstructorNode::Make()
{
    initializerList.Make();
    AddChildNode(&initializerList);
    SyntaxNode::Make();
}

void StaticConstructorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstructorNode::ConstructorNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none), initializerList(soul::ast::Span(), CompletionContext::none)
{
}

void ConstructorNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void ConstructorNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void ConstructorNode::SetClassName(IdentifierNode* className_)
{
    className.reset(className_);
    AddChildNode(className.get());
}

void ConstructorNode::SetColon(ColonNode* colon_)
{
    colon.reset(colon_);
    AddChildNode(colon.get());
}

void ConstructorNode::AddInitializer(InitializerNode* initializer)
{
    initializerList.AddInitializer(initializer);
}

void ConstructorNode::AddComma(CommaNode* comma)
{
    initializerList.AddComma(comma);
}

void ConstructorNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void ConstructorNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void ConstructorNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ConstructorNode::Clone() const
{
    ConstructorNode* clone = new ConstructorNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetClassName(static_cast<IdentifierNode*>(className->Clone()));
    clone->parameterList.CloneFrom(parameterList);
    if (colon)
    {
        clone->SetColon(static_cast<ColonNode*>(colon->Clone()));
    }
    clone->initializerList.CloneFrom(initializerList);
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void ConstructorNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    initializerList.Make();
    AddChildNode(&initializerList);
    SyntaxNode::Make();
}

void ConstructorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DestructorNode::DestructorNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void DestructorNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void DestructorNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void DestructorNode::SetComplement(ComplementNode* complement_)
{
    complement.reset(complement_);
    AddChildNode(complement.get());
}

void DestructorNode::SetClassName(IdentifierNode* className_)
{
    className.reset(className_);
    AddChildNode(className.get());
}

void DestructorNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void DestructorNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void DestructorNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void DestructorNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void DestructorNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DestructorNode::Clone() const
{
    DestructorNode* clone = new DestructorNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetClassName(static_cast<IdentifierNode*>(className->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void DestructorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberFunctionNode::MemberFunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void MemberFunctionNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void MemberFunctionNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void MemberFunctionNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void MemberFunctionNode::SetFunctionGroupId(Node* functionGroupId_)
{
    functionGroupId.reset(functionGroupId_);
    AddChildNode(functionGroupId.get());
}

void MemberFunctionNode::SetConstKeyword(ConstKeywordNode* constKeyword_)
{
    constKeyword.reset(constKeyword_);
    AddChildNode(constKeyword.get());
}

void MemberFunctionNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void MemberFunctionNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void MemberFunctionNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* MemberFunctionNode::Clone() const
{
    MemberFunctionNode* clone = new MemberFunctionNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    if (specifiers)
    {
        clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    }
    clone->SetReturnType(returnType->Clone());
    clone->SetFunctionGroupId(functionGroupId->Clone());
    clone->parameterList.CloneFrom(parameterList);
    if (constKeyword)
    {
        clone->SetConstKeyword(static_cast<ConstKeywordNode*>(constKeyword->Clone()));
    }
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void MemberFunctionNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    SyntaxNode::Make();
}

void MemberFunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConversionFunctionNode::ConversionFunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void ConversionFunctionNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void ConversionFunctionNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void ConversionFunctionNode::SetOperatorKeyword(OperatorKeywordNode* operatorKeyword_)
{
    operatorKeyword.reset(operatorKeyword_);
    AddChildNode(operatorKeyword.get());
}

void ConversionFunctionNode::SetType(Node* type_)
{
    type.reset(type_);
    AddChildNode(type.get());
}

void ConversionFunctionNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ConversionFunctionNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void ConversionFunctionNode::SetConstKeyword(ConstKeywordNode* constKeyword_)
{
    constKeyword.reset(constKeyword_);
    AddChildNode(constKeyword.get());
}

void ConversionFunctionNode::SetConstraint(WhereConstraintNode* constraint_)
{
    constraint.reset(constraint_);
    AddChildNode(constraint.get());
}

void ConversionFunctionNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    AddChildNode(body.get());
}

void ConversionFunctionNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ConversionFunctionNode::Clone() const
{
    ConversionFunctionNode* clone = new ConversionFunctionNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetOperatorKeyword(static_cast<OperatorKeywordNode*>(operatorKeyword->Clone()));
    clone->SetType(type->Clone());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    if (constKeyword)
    {
        clone->SetConstKeyword(static_cast<ConstKeywordNode*>(constKeyword->Clone()));
    }
    if (constraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(constraint->Clone()));
    }
    if (body)
    {
        clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone()));
    }
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void ConversionFunctionNode::Make()
{
    SyntaxNode::Make();
    name = OperatorKeyword()->Str();
    name.append(1, ' ').append(type->Str());
}

void ConversionFunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberVariableNode::MemberVariableNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void MemberVariableNode::SetAttributes(AttributesNode* attributes_)
{
    attributes.reset(attributes_);
    AddChildNode(attributes.get());
}

void MemberVariableNode::SetSpecifiers(SpecifiersNode* specifiers_)
{
    specifiers.reset(specifiers_);
    AddChildNode(specifiers.get());
}

void MemberVariableNode::SetType(Node* type_)
{
    type.reset(type_);
    AddChildNode(type.get());
}

void MemberVariableNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    AddChildNode(id.get());
}

void MemberVariableNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* MemberVariableNode::Clone() const
{
    MemberVariableNode* clone = new MemberVariableNode(GetSpan(), GetCompletionContext());
    if (attributes)
    {
        clone->SetAttributes(static_cast<AttributesNode*>(attributes->Clone()));
    }
    clone->SetSpecifiers(static_cast<SpecifiersNode*>(specifiers->Clone()));
    clone->SetType(type->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    if (semicolon)
    {
        clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    }
    clone->Make();
    return clone;
}

void MemberVariableNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
