// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT lic§se
// =================================

export module cmajor.fault.tolerant.ast.keyword;

import cmajor.fault.tolerant.ast.node;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class UsingKeywordNode: public KeywordNode
{
public:
    UsingKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class TypedefKeywordNode : public KeywordNode
{
public:
    TypedefKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NamespaceKeywordNode : public KeywordNode
{
public:
    NamespaceKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ConstKeywordNode : public KeywordNode
{
public:
    ConstKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AutoKeywordNode : public KeywordNode
{
public:
    AutoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BoolKeywordNode : public KeywordNode
{
public:
    BoolKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class SByteKeywordNode : public KeywordNode
{
public:
    SByteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ByteKeywordNode : public KeywordNode
{
public:
    ByteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    bool IsUnsignedTypeNode() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ShortKeywordNode : public KeywordNode
{
public:
    ShortKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class UShortKeywordNode : public KeywordNode
{
public:
    UShortKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    bool IsUnsignedTypeNode() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class IntKeywordNode : public KeywordNode
{
public:
    IntKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class UIntKeywordNode : public KeywordNode
{
public:
    UIntKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    bool IsUnsignedTypeNode() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LongKeywordNode : public KeywordNode
{
public:
    LongKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ULongKeywordNode : public KeywordNode
{
public:
    ULongKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    bool IsUnsignedTypeNode() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class FloatKeywordNode : public KeywordNode
{
public:
    FloatKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DoubleKeywordNode : public KeywordNode
{
public:
    DoubleKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CharKeywordNode : public KeywordNode
{
public:
    CharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class WCharKeywordNode : public KeywordNode
{
public:
    WCharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class UCharKeywordNode : public KeywordNode
{
public:
    UCharKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class VoidKeywordNode : public KeywordNode
{
public:
    VoidKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ThisKeywordNode : public KeywordNode
{
public:
    ThisKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BaseKeywordNode : public KeywordNode
{
public:
    BaseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class SizeOfKeywordNode : public KeywordNode
{
public:
    SizeOfKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class TypeNameKeywordNode : public KeywordNode
{
public:
    TypeNameKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class TypeIdKeywordNode : public KeywordNode
{
public:
    TypeIdKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CastKeywordNode : public KeywordNode
{
public:
    CastKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ConstructKeywordNode : public KeywordNode
{
public:
    ConstructKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NewKeywordNode : public KeywordNode
{
public:
    NewKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class TrueKeywordNode : public KeywordNode
{
public:
    TrueKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class FalseKeywordNode : public KeywordNode
{
public:
    FalseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NullKeywordNode : public KeywordNode
{
public:
    NullKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ConceptKeywordNode : public KeywordNode
{
public:
    ConceptKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class WhereKeywordNode : public KeywordNode
{
public:
    WhereKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class OrKeywordNode : public KeywordNode
{
public:
    OrKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AndKeywordNode : public KeywordNode
{
public:
    AndKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class IsKeywordNode : public KeywordNode
{
public:
    IsKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AsKeywordNode : public KeywordNode
{
public:
    AsKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class OperatorKeywordNode : public KeywordNode
{
public:
    OperatorKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AxiomKeywordNode : public KeywordNode
{
public:
    AxiomKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ReturnKeywordNode : public KeywordNode
{
public:
    ReturnKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class IfKeywordNode : public KeywordNode
{
public:
    IfKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ElseKeywordNode : public KeywordNode
{
public:
    ElseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class WhileKeywordNode : public KeywordNode
{
public:
    WhileKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DoKeywordNode : public KeywordNode
{
public:
    DoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ForKeywordNode : public KeywordNode
{
public:
    ForKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BreakKeywordNode : public KeywordNode
{
public:
    BreakKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ContinueKeywordNode : public KeywordNode
{
public:
    ContinueKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class GotoKeywordNode : public KeywordNode
{
public:
    GotoKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CaseKeywordNode : public KeywordNode
{
public:
    CaseKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DefaultKeywordNode : public KeywordNode
{
public:
    DefaultKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class SwitchKeywordNode : public KeywordNode
{
public:
    SwitchKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DeleteKeywordNode : public KeywordNode
{
public:
    DeleteKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DestroyKeywordNode : public KeywordNode
{
public:
    DestroyKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ThrowKeywordNode : public KeywordNode
{
public:
    ThrowKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class TryKeywordNode : public KeywordNode
{
public:
    TryKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CatchKeywordNode : public KeywordNode
{
public:
    CatchKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AssertKeywordNode : public KeywordNode
{
public:
    AssertKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ElifKeywordNode : public KeywordNode
{
public:
    ElifKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class EndifKeywordNode : public KeywordNode
{
public:
    EndifKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ClassKeywordNode : public KeywordNode
{
public:
    ClassKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DelegateKeywordNode : public KeywordNode
{
public:
    DelegateKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class EnumKeywordNode : public KeywordNode
{
public:
    EnumKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class InterfaceKeywordNode : public KeywordNode
{
public:
    InterfaceKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class PublicKeywordNode : public KeywordNode
{
public:
    PublicKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ProtectedKeywordNode : public KeywordNode
{
public:
    ProtectedKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class PrivateKeywordNode : public KeywordNode
{
public:
    PrivateKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class InternalKeywordNode : public KeywordNode
{
public:
    InternalKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class StaticKeywordNode : public KeywordNode
{
public:
    StaticKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class VirtualKeywordNode : public KeywordNode
{
public:
    VirtualKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class OverrideKeywordNode : public KeywordNode
{
public:
    OverrideKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AbstractKeywordNode : public KeywordNode
{
public:
    AbstractKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class InlineKeywordNode : public KeywordNode
{
public:
    InlineKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ExplicitKeywordNode : public KeywordNode
{
public:
    ExplicitKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ExternKeywordNode : public KeywordNode
{
public:
    ExternKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class SuppressKeywordNode : public KeywordNode
{
public:
    SuppressKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ConstExprKeywordNode : public KeywordNode
{
public:
    ConstExprKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CDeclKeywordNode : public KeywordNode
{
public:
    CDeclKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NothrowKeywordNode : public KeywordNode
{
public:
    NothrowKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class UnitTestKeywordNode : public KeywordNode
{
public:
    UnitTestKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class WinApiKeywordNode : public KeywordNode
{
public:
    WinApiKeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

} // namespace cmajor::fault::tolerant::ast
