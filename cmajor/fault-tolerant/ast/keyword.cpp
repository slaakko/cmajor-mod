// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.keyword;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

UsingKeywordNode::UsingKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"using")
{
}

Node* UsingKeywordNode::Clone() const
{
    UsingKeywordNode* clone = new UsingKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void UsingKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypedefKeywordNode::TypedefKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"typedef")
{
}

Node* TypedefKeywordNode::Clone() const
{
    TypedefKeywordNode* clone = new TypedefKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void TypedefKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NamespaceKeywordNode::NamespaceKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"namespace")
{
}

Node* NamespaceKeywordNode::Clone() const
{
    NamespaceKeywordNode* clone = new NamespaceKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NamespaceKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstKeywordNode::ConstKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"const")
{
}

Node* ConstKeywordNode::Clone() const
{
    ConstKeywordNode* clone = new ConstKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ConstKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AutoKeywordNode::AutoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"auto")
{
}

Node* AutoKeywordNode::Clone() const
{
    AutoKeywordNode* clone = new AutoKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AutoKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BoolKeywordNode::BoolKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"bool")
{
}

Node* BoolKeywordNode::Clone() const
{
    BoolKeywordNode* clone = new BoolKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void BoolKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SByteKeywordNode::SByteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"sbyte")
{
}

Node* SByteKeywordNode::Clone() const
{
    SByteKeywordNode* clone = new SByteKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void SByteKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByteKeywordNode::ByteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"byte")
{
}

Node* ByteKeywordNode::Clone() const
{
    ByteKeywordNode* clone = new ByteKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ByteKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShortKeywordNode::ShortKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"short")
{
}

Node* ShortKeywordNode::Clone() const
{
    ShortKeywordNode* clone = new ShortKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ShortKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UShortKeywordNode::UShortKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"ushort")
{
}

Node* UShortKeywordNode::Clone() const
{
    UShortKeywordNode* clone = new UShortKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void UShortKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntKeywordNode::IntKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"int")
{
}

Node* IntKeywordNode::Clone() const
{
    IntKeywordNode* clone = new IntKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void IntKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UIntKeywordNode::UIntKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"uint")
{
}

Node* UIntKeywordNode::Clone() const
{
    UIntKeywordNode* clone = new UIntKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void UIntKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LongKeywordNode::LongKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"long")
{
}

Node* LongKeywordNode::Clone() const
{
    LongKeywordNode* clone = new LongKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LongKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ULongKeywordNode::ULongKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"ulong")
{
}

Node* ULongKeywordNode::Clone() const
{
    ULongKeywordNode* clone = new ULongKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ULongKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatKeywordNode::FloatKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"float")
{
}

Node* FloatKeywordNode::Clone() const
{
    FloatKeywordNode* clone = new FloatKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void FloatKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoubleKeywordNode::DoubleKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"double")
{
}

Node* DoubleKeywordNode::Clone() const
{
    DoubleKeywordNode* clone = new DoubleKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DoubleKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharKeywordNode::CharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"char")
{
}

Node* CharKeywordNode::Clone() const
{
    CharKeywordNode* clone = new CharKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CharKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WCharKeywordNode::WCharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"wchar")
{
}

Node* WCharKeywordNode::Clone() const
{
    WCharKeywordNode* clone = new WCharKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void WCharKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UCharKeywordNode::UCharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"uchar")
{
}

Node* UCharKeywordNode::Clone() const
{
    UCharKeywordNode* clone = new UCharKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void UCharKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VoidKeywordNode::VoidKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"void")
{
}

Node* VoidKeywordNode::Clone() const
{
    VoidKeywordNode* clone = new VoidKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void VoidKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThisKeywordNode::ThisKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"this")
{
}

Node* ThisKeywordNode::Clone() const
{
    ThisKeywordNode* clone = new ThisKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ThisKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BaseKeywordNode::BaseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"base")
{
}

Node* BaseKeywordNode::Clone() const
{
    BaseKeywordNode* clone = new BaseKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void BaseKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SizeOfKeywordNode::SizeOfKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"sizeof")
{
}

Node* SizeOfKeywordNode::Clone() const
{
    SizeOfKeywordNode* clone = new SizeOfKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void SizeOfKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNameKeywordNode::TypeNameKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"typename")
{
}

Node* TypeNameKeywordNode::Clone() const
{
    TypeNameKeywordNode* clone = new TypeNameKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void TypeNameKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeIdKeywordNode::TypeIdKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"typeid")
{
}

Node* TypeIdKeywordNode::Clone() const
{
    TypeIdKeywordNode* clone = new TypeIdKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void TypeIdKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CastKeywordNode::CastKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"cast")
{
}

Node* CastKeywordNode::Clone() const
{
    CastKeywordNode* clone = new CastKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CastKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstructKeywordNode::ConstructKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"consstruct")
{
}

Node* ConstructKeywordNode::Clone() const
{
    ConstructKeywordNode* clone = new ConstructKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ConstructKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NewKeywordNode::NewKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"new")
{
}

Node* NewKeywordNode::Clone() const
{
    NewKeywordNode* clone = new NewKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NewKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TrueKeywordNode::TrueKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"true")
{
}

Node* TrueKeywordNode::Clone() const
{
    TrueKeywordNode* clone = new TrueKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void TrueKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FalseKeywordNode::FalseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"false")
{
}

Node* FalseKeywordNode::Clone() const
{
    FalseKeywordNode* clone = new FalseKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void FalseKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NullKeywordNode::NullKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"null")
{
}

Node* NullKeywordNode::Clone() const
{
    NullKeywordNode* clone = new NullKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NullKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConceptKeywordNode::ConceptKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"concept")
{
}

Node* ConceptKeywordNode::Clone() const
{
    ConceptKeywordNode* clone = new ConceptKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ConceptKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WhereKeywordNode::WhereKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"where")
{
}

Node* WhereKeywordNode::Clone() const
{
    WhereKeywordNode* clone = new WhereKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void WhereKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OrKeywordNode::OrKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"or")
{
}

Node* OrKeywordNode::Clone() const
{
    OrKeywordNode* clone = new OrKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void OrKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AndKeywordNode::AndKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"and")
{
}

Node* AndKeywordNode::Clone() const
{
    AndKeywordNode* clone = new AndKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AndKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IsKeywordNode::IsKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"is")
{
}

Node* IsKeywordNode::Clone() const
{
    IsKeywordNode* clone = new IsKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void IsKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AsKeywordNode::AsKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"as")
{
}

Node* AsKeywordNode::Clone() const
{
    AsKeywordNode* clone = new AsKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AsKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OperatorKeywordNode::OperatorKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"operator")
{
}

Node* OperatorKeywordNode::Clone() const
{
    OperatorKeywordNode* clone = new OperatorKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void OperatorKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AxiomKeywordNode::AxiomKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"axiom")
{
}

Node* AxiomKeywordNode::Clone() const
{
    AxiomKeywordNode* clone = new AxiomKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AxiomKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ReturnKeywordNode::ReturnKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"return")
{
}

Node* ReturnKeywordNode::Clone() const
{
    ReturnKeywordNode* clone = new ReturnKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ReturnKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IfKeywordNode::IfKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"id")
{
}

Node* IfKeywordNode::Clone() const
{
    IfKeywordNode* clone = new IfKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void IfKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ElseKeywordNode::ElseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"else")
{
}

Node* ElseKeywordNode::Clone() const
{
    ElseKeywordNode* clone = new ElseKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ElseKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WhileKeywordNode::WhileKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"while")
{
}

Node* WhileKeywordNode::Clone() const
{
    WhileKeywordNode* clone = new WhileKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void WhileKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoKeywordNode::DoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"do")
{
}

Node* DoKeywordNode::Clone() const
{
    DoKeywordNode* clone = new DoKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DoKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ForKeywordNode::ForKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"for")
{
}

Node* ForKeywordNode::Clone() const
{
    ForKeywordNode* clone = new ForKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ForKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BreakKeywordNode::BreakKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"break")
{
}

Node* BreakKeywordNode::Clone() const
{
    BreakKeywordNode* clone = new BreakKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void BreakKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ContinueKeywordNode::ContinueKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"continue")
{
}

Node* ContinueKeywordNode::Clone() const
{
    ContinueKeywordNode* clone = new ContinueKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ContinueKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GotoKeywordNode::GotoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"goto")
{
}

Node* GotoKeywordNode::Clone() const
{
    GotoKeywordNode* clone = new GotoKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void GotoKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CaseKeywordNode::CaseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"case")
{
}

Node* CaseKeywordNode::Clone() const
{
    CaseKeywordNode* clone = new CaseKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CaseKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultKeywordNode::DefaultKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"default")
{
}

Node* DefaultKeywordNode::Clone() const
{
    DefaultKeywordNode* clone = new DefaultKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DefaultKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SwitchKeywordNode::SwitchKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"switch")
{
}

Node* SwitchKeywordNode::Clone() const
{
    SwitchKeywordNode* clone = new SwitchKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void SwitchKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeleteKeywordNode::DeleteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"delete")
{
}

Node* DeleteKeywordNode::Clone() const
{
    DeleteKeywordNode* clone = new DeleteKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DeleteKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DestroyKeywordNode::DestroyKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"destroy")
{
}

Node* DestroyKeywordNode::Clone() const
{
    DestroyKeywordNode* clone = new DestroyKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DestroyKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThrowKeywordNode::ThrowKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"throw")
{
}

Node* ThrowKeywordNode::Clone() const
{
    ThrowKeywordNode* clone = new ThrowKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ThrowKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TryKeywordNode::TryKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"try")
{
}

Node* TryKeywordNode::Clone() const
{
    TryKeywordNode* clone = new TryKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void TryKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CatchKeywordNode::CatchKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"catch")
{
}

Node* CatchKeywordNode::Clone() const
{
    CatchKeywordNode* clone = new CatchKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CatchKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AssertKeywordNode::AssertKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"assert")
{
}

Node* AssertKeywordNode::Clone() const
{
    AssertKeywordNode* clone = new AssertKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AssertKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ElifKeywordNode::ElifKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"elif")
{
}

Node* ElifKeywordNode::Clone() const
{
    ElifKeywordNode* clone = new ElifKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ElifKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EndifKeywordNode::EndifKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"endif")
{
}

Node* EndifKeywordNode::Clone() const
{
    EndifKeywordNode* clone = new EndifKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void EndifKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ClassKeywordNode::ClassKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"class")
{
}

Node* ClassKeywordNode::Clone() const
{
    ClassKeywordNode* clone = new ClassKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ClassKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DelegateKeywordNode::DelegateKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"delegate")
{
}

Node* DelegateKeywordNode::Clone() const
{
    DelegateKeywordNode* clone = new DelegateKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DelegateKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EnumKeywordNode::EnumKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"enum")
{
}

Node* EnumKeywordNode::Clone() const
{
    EnumKeywordNode* clone = new EnumKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void EnumKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InterfaceKeywordNode::InterfaceKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"interface")
{
}

Node* InterfaceKeywordNode::Clone() const
{
    InterfaceKeywordNode* clone = new InterfaceKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void InterfaceKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PublicKeywordNode::PublicKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"public")
{
}

Node* PublicKeywordNode::Clone() const
{
    PublicKeywordNode* clone = new PublicKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void PublicKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ProtectedKeywordNode::ProtectedKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"protected")
{
}

Node* ProtectedKeywordNode::Clone() const
{
    ProtectedKeywordNode* clone = new ProtectedKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ProtectedKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PrivateKeywordNode::PrivateKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"private")
{
}

Node* PrivateKeywordNode::Clone() const
{
    PrivateKeywordNode* clone = new PrivateKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void PrivateKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InternalKeywordNode::InternalKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"internal")
{
}

Node* InternalKeywordNode::Clone() const
{
    InternalKeywordNode* clone = new InternalKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void InternalKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StaticKeywordNode::StaticKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"static")
{
}

Node* StaticKeywordNode::Clone() const
{
    StaticKeywordNode* clone = new StaticKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void StaticKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VirtualKeywordNode::VirtualKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"virtual")
{
}

Node* VirtualKeywordNode::Clone() const
{
    VirtualKeywordNode* clone = new VirtualKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void VirtualKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OverrideKeywordNode::OverrideKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"override")
{
}

Node* OverrideKeywordNode::Clone() const
{
    OverrideKeywordNode* clone = new OverrideKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void OverrideKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AbstractKeywordNode::AbstractKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"abstract")
{
}

Node* AbstractKeywordNode::Clone() const
{
    AbstractKeywordNode* clone = new AbstractKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AbstractKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InlineKeywordNode::InlineKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"inline")
{
}

Node* InlineKeywordNode::Clone() const
{
    InlineKeywordNode* clone = new InlineKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void InlineKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExplicitKeywordNode::ExplicitKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"explicit")
{
}

Node* ExplicitKeywordNode::Clone() const
{
    ExplicitKeywordNode* clone = new ExplicitKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ExplicitKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExternKeywordNode::ExternKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"extern")
{
}

Node* ExternKeywordNode::Clone() const
{
    ExternKeywordNode* clone = new ExternKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ExternKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SuppressKeywordNode::SuppressKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"suppress")
{
}

Node* SuppressKeywordNode::Clone() const
{
    SuppressKeywordNode* clone = new SuppressKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void SuppressKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstExprKeywordNode::ConstExprKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"const")
{
}

Node* ConstExprKeywordNode::Clone() const
{
    ConstExprKeywordNode* clone = new ConstExprKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ConstExprKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CDeclKeywordNode::CDeclKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"cdecl")
{
}

Node* CDeclKeywordNode::Clone() const
{
    CDeclKeywordNode* clone = new CDeclKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CDeclKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NothrowKeywordNode::NothrowKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"nothrow")
{
}

Node* NothrowKeywordNode::Clone() const
{
    NothrowKeywordNode* clone = new NothrowKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NothrowKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UnitTestKeywordNode::UnitTestKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"unit_test")
{
}

Node* UnitTestKeywordNode::Clone() const
{
    UnitTestKeywordNode* clone = new UnitTestKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void UnitTestKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WinApiKeywordNode::WinApiKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_) : KeywordNode(span_, completionContext_), str(u"winapi")
{
}

Node* WinApiKeywordNode::Clone() const
{
    WinApiKeywordNode* clone = new WinApiKeywordNode(GetSpan(), GetCompletionContext());
    return clone;
}

void WinApiKeywordNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
