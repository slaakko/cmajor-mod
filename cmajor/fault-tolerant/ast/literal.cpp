// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.literal;

import cmajor.fault.tolerant.ast.visitor;
import util;

namespace cmajor::fault::tolerant::ast {

LiteralTokenNode::LiteralTokenNode(const soul::ast::Span& span_, CompletionContext completionContext_, const std::u16string& str_) : TokenNode(span_, completionContext_), str(str_)
{
}

Node* LiteralTokenNode::Clone() const
{
    LiteralTokenNode* clone = new LiteralTokenNode(GetSpan(), GetCompletionContext(), str);
    return clone;
}

void LiteralTokenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BooleanLiteralNode::BooleanLiteralNode(KeywordNode* rep_, bool value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* BooleanLiteralNode::Clone() const
{
    BooleanLiteralNode* clone = new BooleanLiteralNode(static_cast<KeywordNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void BooleanLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SByteLiteralNode::SByteLiteralNode(LiteralTokenNode* rep_, int8_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* SByteLiteralNode::Clone() const
{
    SByteLiteralNode* clone = new SByteLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void SByteLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByteLiteralNode::ByteLiteralNode(LiteralTokenNode* rep_, uint8_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* ByteLiteralNode::Clone() const
{
    ByteLiteralNode* clone = new ByteLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void ByteLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShortLiteralNode::ShortLiteralNode(LiteralTokenNode* rep_, int16_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* ShortLiteralNode::Clone() const
{
    ShortLiteralNode* clone = new ShortLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void ShortLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UShortLiteralNode::UShortLiteralNode(LiteralTokenNode* rep_, uint16_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* UShortLiteralNode::Clone() const
{
    UShortLiteralNode* clone = new UShortLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void UShortLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntLiteralNode::IntLiteralNode(LiteralTokenNode* rep_, int32_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* IntLiteralNode::Clone() const
{
    IntLiteralNode* clone = new IntLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void IntLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UIntLiteralNode::UIntLiteralNode(LiteralTokenNode* rep_, uint32_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* UIntLiteralNode::Clone() const
{
    UIntLiteralNode* clone = new UIntLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void UIntLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LongLiteralNode::LongLiteralNode(LiteralTokenNode* rep_, int64_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* LongLiteralNode::Clone() const
{
    LongLiteralNode* clone = new LongLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void LongLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ULongLiteralNode::ULongLiteralNode(LiteralTokenNode* rep_, uint64_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* ULongLiteralNode::Clone() const
{
    ULongLiteralNode* clone = new ULongLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void ULongLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatLiteralNode::FloatLiteralNode(LiteralTokenNode* rep_, float value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* FloatLiteralNode::Clone() const
{
    FloatLiteralNode* clone = new FloatLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void FloatLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoubleLiteralNode::DoubleLiteralNode(LiteralTokenNode* rep_, double value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* DoubleLiteralNode::Clone() const
{
    DoubleLiteralNode* clone = new DoubleLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void DoubleLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharLiteralNode::CharLiteralNode(LiteralTokenNode* rep_, char value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* CharLiteralNode::Clone() const
{
    CharLiteralNode* clone = new CharLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void CharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WCharLiteralNode::WCharLiteralNode(LiteralTokenNode* rep_, char16_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* WCharLiteralNode::Clone() const
{
    WCharLiteralNode* clone = new WCharLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void WCharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UCharLiteralNode::UCharLiteralNode(LiteralTokenNode* rep_, char32_t value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* UCharLiteralNode::Clone() const
{
    UCharLiteralNode* clone = new UCharLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void UCharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StringLiteralNode::StringLiteralNode(LiteralTokenNode* rep_, const std::string& value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* StringLiteralNode::Clone() const
{
    StringLiteralNode* clone = new StringLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void StringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WStringLiteralNode::WStringLiteralNode(LiteralTokenNode* rep_, const std::u16string& value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* WStringLiteralNode::Clone() const
{
    WStringLiteralNode* clone = new WStringLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void WStringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UStringLiteralNode::UStringLiteralNode(LiteralTokenNode* rep_, const std::u32string& value_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_), value(value_)
{
    AddChildNode(rep.get());
}

Node* UStringLiteralNode::Clone() const
{
    UStringLiteralNode* clone = new UStringLiteralNode(static_cast<LiteralTokenNode*>(rep->Clone()), value);
    clone->Make();
    return clone;
}

void UStringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NullLiteralNode::NullLiteralNode(KeywordNode* rep_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), rep(rep_)
{
    AddChildNode(rep.get());
}

Node* NullLiteralNode::Clone() const
{
    NullLiteralNode* clone = new NullLiteralNode(static_cast<KeywordNode*>(rep->Clone()));
    clone->Make();
    return clone;
}

void NullLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayLiteralNode::ArrayLiteralNode() : SyntaxNode(soul::ast::Span(), CompletionContext::none), valueList(soul::ast::Span(), CompletionContext::none)
{
}

void ArrayLiteralNode::AddNode(Node* node) 
{
    valueList.AddNode(node);
}

void ArrayLiteralNode::AddComma(CommaNode* comma)
{
    valueList.AddComma(comma);
}

void ArrayLiteralNode::SetLBracket(LBracketNode* lbracket_)
{
    lbracket.reset(lbracket_);
    AddChildNode(lbracket.get());
}

void ArrayLiteralNode::SetRBracket(RBracketNode* rbracket_)
{
    rbracket.reset(rbracket_);
    AddChildNode(rbracket.get());
}

Node* ArrayLiteralNode::Clone() const
{
    ArrayLiteralNode* clone = new ArrayLiteralNode();
    clone->SetLBracket(static_cast<LBracketNode*>(lbracket->Clone()));
    clone->valueList.CloneFrom(valueList);
    clone->SetRBracket(static_cast<RBracketNode*>(rbracket->Clone()));
    clone->Make();
    return clone;
}

void ArrayLiteralNode::Make()
{
    valueList.Make();
    AddChildNode(&valueList);
    SyntaxNode::Make();
}

void ArrayLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StructuredLiteralNode::StructuredLiteralNode() : SyntaxNode(soul::ast::Span(), CompletionContext::none), valueList(soul::ast::Span(), CompletionContext::none)
{
}

void StructuredLiteralNode::AddNode(Node* node)
{
    valueList.AddNode(node);
}

void StructuredLiteralNode::AddComma(CommaNode* comma)
{
    valueList.AddComma(comma);
}

void StructuredLiteralNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void StructuredLiteralNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* StructuredLiteralNode::Clone() const
{
    StructuredLiteralNode* clone = new StructuredLiteralNode();
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    clone->valueList.CloneFrom(valueList);
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void StructuredLiteralNode::Make()
{
    valueList.Make();
    AddChildNode(&valueList);
    SyntaxNode::Make();
}

void StructuredLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* CreateIntegerLiteralNode(LiteralTokenNode* rep, bool unsignedSuffix, uint64_t value)
{
    if (unsignedSuffix)
    {
        if (value <= std::numeric_limits<uint8_t>::max())
        {
            Node* literalNode = new ByteLiteralNode(rep, static_cast<uint8_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<uint16_t>::max())
        {
            Node* literalNode = new UShortLiteralNode(rep, static_cast<uint16_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<uint32_t>::max())
        {
            Node* literalNode = new UIntLiteralNode(rep, static_cast<uint32_t>(value));
            literalNode->Make();
            return literalNode;
        }
        Node* literalNode = new ULongLiteralNode(rep, static_cast<uint32_t>(value));
        literalNode->Make();
        return literalNode;
    }
    else
    {
        if (value <= std::numeric_limits<int8_t>::max())
        {
            Node* literalNode = new SByteLiteralNode(rep, static_cast<int8_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<uint8_t>::max())
        {
            Node* literalNode = new ByteLiteralNode(rep, static_cast<uint8_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<int16_t>::max())
        {
            Node* literalNode = new ShortLiteralNode(rep, static_cast<int16_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<uint16_t>::max())
        {
            Node* literalNode = new UShortLiteralNode(rep, static_cast<uint16_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<int32_t>::max())
        {
            Node* literalNode = new IntLiteralNode(rep, static_cast<int32_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<uint32_t>::max())
        {
            Node* literalNode = new UIntLiteralNode(rep, static_cast<uint32_t>(value));
            literalNode->Make();
            return literalNode;
        }
        if (value <= std::numeric_limits<int64_t>::max())
        {
            Node* literalNode = new LongLiteralNode(rep, static_cast<int64_t>(value));
            literalNode->Make();
            return literalNode;
        }
        Node* literalNode = new LongLiteralNode(rep, value);
        literalNode->Make();
        return literalNode;
    }
}

Node* CreateFloatingLiteralNode(LiteralTokenNode* rep, bool isFloat, double value)
{
    if (isFloat)
    {
        Node* literalNode = new FloatLiteralNode(rep, static_cast<float>(value));
        literalNode->Make();
        return literalNode;

    }
    else
    {
        Node* literalNode = new DoubleLiteralNode(rep, value);
        literalNode->Make();
        return literalNode;
    }
}

Node* CreateCharacterLiteralNode(LiteralTokenNode* rep, char16_t value, CharLiteralPrefix prefix)
{
    switch (prefix)
    {
        case CharLiteralPrefix::none:
        {
            Node* literalNode = new CharLiteralNode(rep, static_cast<char>(value));
            literalNode->Make();
            return literalNode;
        }
        case CharLiteralPrefix::utf16Prefix:
        {
            Node* literalNode = new WCharLiteralNode(rep, value);
            literalNode->Make();
            return literalNode;
        }
        case CharLiteralPrefix::utf32Prefix:
        {
            Node* literalNode = new UCharLiteralNode(rep, static_cast<char32_t>(value));
            literalNode->Make();
            return literalNode;
        }
    }
    return nullptr;
}

Node* CreateStringLiteralNode(LiteralTokenNode* rep, const std::u16string& value, StringLiteralPrefix prefix)
{
    switch (prefix)
    {
        case StringLiteralPrefix::none:
        {
            Node* literalNode = new StringLiteralNode(rep, util::ToUtf8(value));
            literalNode->Make();
            return literalNode;
        }
        case StringLiteralPrefix::utf16Prefix:
        {
            Node* literalNode = new WStringLiteralNode(rep, value);
            literalNode->Make();
            return literalNode;
        }
        case StringLiteralPrefix::utf32Prefix:
        {
            Node* literalNode = new UStringLiteralNode(rep, util::ToUtf32(value));
            literalNode->Make();
            return literalNode;
        }
    }
    return nullptr;
}

} // namespace cmajor::fault::tolerant::ast
