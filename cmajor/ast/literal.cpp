// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.literal;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

LiteralNode::LiteralNode(NodeType nodeType_, const soul::ast::Span& span_) : Node(nodeType_, span_)
{
}

void LiteralNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(text);
}

void LiteralNode::Read(AstReader& reader)
{
    Node::Read(reader);
    text = reader.GetBinaryStreamReader().ReadUtf32String();
}

void LiteralNode::SetText(const std::u32string& text_)
{
    text = text_;
}

LiteralNode* CreateIntegerLiteralNode(const soul::ast::Span& span, uint64_t value, bool unsignedSuffix)
{
    if (unsignedSuffix)
    {
        if (value <= std::numeric_limits<uint8_t>::max()) return new ByteLiteralNode(span, static_cast<uint8_t>(value));
        if (value <= std::numeric_limits<uint16_t>::max()) return new UShortLiteralNode(span, static_cast<uint16_t>(value));
        if (value <= std::numeric_limits<uint32_t>::max()) return new UIntLiteralNode(span, static_cast<uint32_t>(value));
        return new ULongLiteralNode(span, value);
    }
    else
    {
        if (value <= std::numeric_limits<int8_t>::max()) return new SByteLiteralNode(span, static_cast<int8_t>(value));
        if (value <= std::numeric_limits<uint8_t>::max()) return new ByteLiteralNode(span, static_cast<uint8_t>(value));
        if (value <= std::numeric_limits<int16_t>::max()) return new ShortLiteralNode(span, static_cast<int16_t>(value));
        if (value <= std::numeric_limits<uint16_t>::max()) return new UShortLiteralNode(span, static_cast<uint16_t>(value));
        if (value <= std::numeric_limits<int32_t>::max()) return new IntLiteralNode(span, static_cast<int32_t>(value));
        if (value <= std::numeric_limits<uint32_t>::max()) return new UIntLiteralNode(span, static_cast<uint32_t>(value));
#pragma warning(disable : 4018)
        if (value <= std::numeric_limits<int64_t>::max()) return new LongLiteralNode(span, static_cast<int64_t>(value));
#pragma warning(default : 4018)
        return new ULongLiteralNode(span, value);
    }
}

LiteralNode* CreateFloatingLiteralNode(const soul::ast::Span& span, double value, bool float_)
{
    if (float_)
    {
        return new FloatLiteralNode(span, static_cast<float>(value));
    }
    else
    {
        return new DoubleLiteralNode(span, value);
    }
}

LiteralNode* CreateCharacterLiteralNode(const soul::ast::Span& span, char32_t value, CharLiteralPrefix prefix)
{
    switch (prefix)
    {
        case CharLiteralPrefix::none:
        {
            return new CharLiteralNode(span, static_cast<char>(value));
        }
        case CharLiteralPrefix::utf16Prefix:
        {
            return new WCharLiteralNode(span, static_cast<char16_t>(value));
        }
        case CharLiteralPrefix::utf32Prefix:
        {
            return new UCharLiteralNode(span, value);
        }
    }
    return nullptr;
}

LiteralNode* CreateStringLiteralNode(const soul::ast::Span& span, const std::u32string& value, StringLiteralPrefix prefix)
{
    switch (prefix)
    {
        case StringLiteralPrefix::none:
        {
            return new StringLiteralNode(span, util::ToUtf8(value));
        }
        case StringLiteralPrefix::utf16Prefix:
        {
            return new WStringLiteralNode(span, util::ToUtf16(value));
        }
        case StringLiteralPrefix::utf32Prefix:
        {
            return new UStringLiteralNode(span, value);
        }
    }
    return nullptr;
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::Span& span_) : 
    LiteralNode(NodeType::booleanLiteralNode, span_), value(false)
{
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::Span& span_, bool value_) :
    LiteralNode(NodeType::booleanLiteralNode, span_), value(value_)
{
}

Node* BooleanLiteralNode::Clone(CloneContext& cloneContext) const
{
    BooleanLiteralNode* clone = new BooleanLiteralNode(GetSpan(), value);
    return clone;
}

void BooleanLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BooleanLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void BooleanLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadBool();
}

std::string BooleanLiteralNode::ToString() const
{
    if (value) return "true"; else return "false";
}

SByteLiteralNode::SByteLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::sbyteLiteralNode, span_), value(0)
{
}

SByteLiteralNode::SByteLiteralNode(const soul::ast::Span& span_, int8_t value_) :
    LiteralNode(NodeType::sbyteLiteralNode, span_), value(value_)
{
}

Node* SByteLiteralNode::Clone(CloneContext& cloneContext) const
{
    SByteLiteralNode* clone = new SByteLiteralNode(GetSpan(), value);
    return clone;
}

void SByteLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SByteLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void SByteLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadSByte();
}

std::string SByteLiteralNode::ToString() const
{
    return std::to_string(value);
}

ByteLiteralNode::ByteLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::byteLiteralNode, span_), value(0u)
{
}

ByteLiteralNode::ByteLiteralNode(const soul::ast::Span& span_, uint8_t value_) :
    LiteralNode(NodeType::byteLiteralNode, span_), value(value_)
{
}

Node* ByteLiteralNode::Clone(CloneContext& cloneContext) const
{
    ByteLiteralNode* clone = new ByteLiteralNode(GetSpan(), value);
    return clone;
}

void ByteLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ByteLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void ByteLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadByte();
}

std::string ByteLiteralNode::ToString() const
{
    return std::to_string(value) + "u";
}

ShortLiteralNode::ShortLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::shortLiteralNode, span_), value(0)
{
}

ShortLiteralNode::ShortLiteralNode(const soul::ast::Span& span_, int16_t value_) :
    LiteralNode(NodeType::shortLiteralNode, span_), value(value_)
{
}

Node* ShortLiteralNode::Clone(CloneContext& cloneContext) const
{
    ShortLiteralNode* clone = new ShortLiteralNode(GetSpan(), value);
    return clone;
}

void ShortLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ShortLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void ShortLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadShort();
}

std::string ShortLiteralNode::ToString() const
{
    return std::to_string(value);
}

UShortLiteralNode::UShortLiteralNode(const soul::ast::Span& span_) : 
    LiteralNode(NodeType::ushortLiteralNode, span_), value(0u)
{
}

UShortLiteralNode::UShortLiteralNode(const soul::ast::Span& span_, uint16_t value_) :
    LiteralNode(NodeType::ushortLiteralNode, span_), value(value_)
{
}

Node* UShortLiteralNode::Clone(CloneContext& cloneContext) const
{
    UShortLiteralNode* clone = new UShortLiteralNode(GetSpan(), value);
    return clone;
}

void UShortLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UShortLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void UShortLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUShort();
}

std::string UShortLiteralNode::ToString() const
{
    return std::to_string(value) + "u";
}

IntLiteralNode::IntLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::intLiteralNode, span_), value(0)
{
}

IntLiteralNode::IntLiteralNode(const soul::ast::Span& span_, int32_t value_) :
    LiteralNode(NodeType::intLiteralNode, span_), value(value_)
{
}

Node* IntLiteralNode::Clone(CloneContext& cloneContext) const
{
    IntLiteralNode* clone = new IntLiteralNode(GetSpan(), value);
    return clone;
}

void IntLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void IntLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadInt();
}

std::string IntLiteralNode::ToString() const
{
    return std::to_string(value);
}

UIntLiteralNode::UIntLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::uintLiteralNode, span_), value(0u)
{
}

UIntLiteralNode::UIntLiteralNode(const soul::ast::Span& span_, uint32_t value_) :
    LiteralNode(NodeType::uintLiteralNode, span_), value(value_)
{
}

Node* UIntLiteralNode::Clone(CloneContext& cloneContext) const
{
    UIntLiteralNode* clone = new UIntLiteralNode(GetSpan(), value);
    return clone;
}

void UIntLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UIntLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void UIntLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUInt();
}

std::string UIntLiteralNode::ToString() const
{
    return std::to_string(value) + "u";
}

LongLiteralNode::LongLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::longLiteralNode, span_), value(0)
{
}

LongLiteralNode::LongLiteralNode(const soul::ast::Span& span_, int64_t value_) :
    LiteralNode(NodeType::longLiteralNode, span_), value(value_)
{
}

Node* LongLiteralNode::Clone(CloneContext& cloneContext) const
{
    LongLiteralNode* clone = new LongLiteralNode(GetSpan(), value);
    return clone;
}

void LongLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LongLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void LongLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadLong();
}

std::string LongLiteralNode::ToString() const
{
    return std::to_string(value);
}

ULongLiteralNode::ULongLiteralNode(const soul::ast::Span& span_) : 
    LiteralNode(NodeType::ulongLiteralNode, span_), value(0u)
{
}

ULongLiteralNode::ULongLiteralNode(const soul::ast::Span& span_, uint64_t value_) :
    LiteralNode(NodeType::ulongLiteralNode, span_), value(value_)
{
}

Node* ULongLiteralNode::Clone(CloneContext& cloneContext) const
{
    ULongLiteralNode* clone = new ULongLiteralNode(GetSpan(), value);
    return clone;
}

void ULongLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ULongLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void ULongLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadULong();
}

std::string ULongLiteralNode::ToString() const
{
    return std::to_string(value) + "u";
}

FloatLiteralNode::FloatLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::floatLiteralNode, span_), value(0)
{
}

FloatLiteralNode::FloatLiteralNode(const soul::ast::Span& span_, float value_) :
    LiteralNode(NodeType::floatLiteralNode, span_), value(value_)
{
}

Node* FloatLiteralNode::Clone(CloneContext& cloneContext) const
{
    FloatLiteralNode* clone = new FloatLiteralNode(GetSpan(), value);
    return clone;
}

void FloatLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FloatLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void FloatLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadFloat();
}

std::string FloatLiteralNode::ToString() const
{
    return std::to_string(value) + "f";
}

DoubleLiteralNode::DoubleLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::doubleLiteralNode, span_), value(0)
{
}

DoubleLiteralNode::DoubleLiteralNode(const soul::ast::Span& span_, double value_) :
    LiteralNode(NodeType::doubleLiteralNode, span_), value(value_)
{
}

Node* DoubleLiteralNode::Clone(CloneContext& cloneContext) const
{
    DoubleLiteralNode* clone = new DoubleLiteralNode(GetSpan(), value);
    return clone;
}

void DoubleLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DoubleLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void DoubleLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadDouble();
}

std::string DoubleLiteralNode::ToString() const
{
    return std::to_string(value);
}

CharLiteralNode::CharLiteralNode(const soul::ast::Span& span_) : 
    LiteralNode(NodeType::charLiteralNode, span_), value('\0')
{
}

CharLiteralNode::CharLiteralNode(const soul::ast::Span& span_, char value_) :
    LiteralNode(NodeType::charLiteralNode, span_), value(value_)
{
}

Node* CharLiteralNode::Clone(CloneContext& cloneContext) const
{
    CharLiteralNode* clone = new CharLiteralNode(GetSpan(), value);
    return clone;
}

void CharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void CharLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadChar();
}

std::string CharLiteralNode::ToString() const
{
    return "'" + util::CharStr(value) + "'";
}

WCharLiteralNode::WCharLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::wcharLiteralNode, span_), value('\0')
{
}

WCharLiteralNode::WCharLiteralNode(const soul::ast::Span& span_, char16_t value_) :
    LiteralNode(NodeType::wcharLiteralNode, span_), value(value_)
{
}

Node* WCharLiteralNode::Clone(CloneContext& cloneContext) const
{
    WCharLiteralNode* clone = new WCharLiteralNode(GetSpan(), value);
    return clone;
}

void WCharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WCharLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void WCharLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadWChar();
}

std::string WCharLiteralNode::ToString() const
{
    return "w'" + util::ToUtf8(util::CharStr(char32_t(value))) + "'";
}

UCharLiteralNode::UCharLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::ucharLiteralNode, span_), value('\0')
{
}

UCharLiteralNode::UCharLiteralNode(const soul::ast::Span& span_, char32_t value_) :
    LiteralNode(NodeType::ucharLiteralNode, span_), value(value_)
{
}

Node* UCharLiteralNode::Clone(CloneContext& cloneContext) const
{
    UCharLiteralNode* clone = new UCharLiteralNode(GetSpan(), value);
    return clone;
}

void UCharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UCharLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void UCharLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUChar();
}

std::string UCharLiteralNode::ToString() const
{
    return "u'" + util::ToUtf8(util::CharStr(value)) + "'";
}

StringLiteralNode::StringLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::stringLiteralNode, span_), value()
{
}

StringLiteralNode::StringLiteralNode(const soul::ast::Span& span_, const std::string& value_) :
    LiteralNode(NodeType::stringLiteralNode, span_), value(value_)
{
}

Node* StringLiteralNode::Clone(CloneContext& cloneContext) const
{
    StringLiteralNode* clone = new StringLiteralNode(GetSpan(), value);
    return clone;
}

void StringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void StringLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUtf8String();
}

std::string StringLiteralNode::ToString() const
{
    return "\"" + util::StringStr(value) + "\"";
}

WStringLiteralNode::WStringLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::wstringLiteralNode, span_), value()
{
}

WStringLiteralNode::WStringLiteralNode(const soul::ast::Span& span_, const std::u16string& value_) :
    LiteralNode(NodeType::wstringLiteralNode, span_), value(value_)
{
}

Node* WStringLiteralNode::Clone(CloneContext& cloneContext) const
{
    WStringLiteralNode* clone = new WStringLiteralNode(GetSpan(), value);
    return clone;
}

void WStringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WStringLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void WStringLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUtf16String();
}

std::string WStringLiteralNode::ToString() const
{
    return "\"" + util::StringStr(util::ToUtf8(value)) + "\"";
}

UStringLiteralNode::UStringLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::ustringLiteralNode, span_), value()
{
}

UStringLiteralNode::UStringLiteralNode(const soul::ast::Span& span_, const std::u32string& value_) :
    LiteralNode(NodeType::ustringLiteralNode, span_), value(value_)
{
}

Node* UStringLiteralNode::Clone(CloneContext& cloneContext) const
{
    UStringLiteralNode* clone = new UStringLiteralNode(GetSpan(), value);
    return clone;
}

void UStringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UStringLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void UStringLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUtf32String();
}

std::string UStringLiteralNode::ToString() const
{
    return "\"" + util::StringStr(util::ToUtf8(value)) + "\"";
}

NullLiteralNode::NullLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::nullLiteralNode, span_)
{
}

Node* NullLiteralNode::Clone(CloneContext& cloneContext) const
{
    NullLiteralNode* clone = new NullLiteralNode(GetSpan());
    return clone;
}

void NullLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayLiteralNode::ArrayLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::arrayLiteralNode, span_)
{
}

Node* ArrayLiteralNode::Clone(CloneContext& cloneContext) const
{
    ArrayLiteralNode* clone = new ArrayLiteralNode(GetSpan());
    int n = values.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddValue(values[i]->Clone(cloneContext));
    }
    return clone;
}

void ArrayLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    values.Write(writer);
}

void ArrayLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    values.Read(reader);
}

void ArrayLiteralNode::AddValue(Node* value)
{
    value->SetParent(this);
    values.Add(value);
}

StructuredLiteralNode::StructuredLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::structuredLiteralNode, span_)
{
}

Node* StructuredLiteralNode::Clone(CloneContext& cloneContext) const
{
    StructuredLiteralNode* clone = new StructuredLiteralNode(GetSpan());
    int n = members.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddMember(members[i]->Clone(cloneContext));
    }
    return clone;
}

void StructuredLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StructuredLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    members.Write(writer);
}

void StructuredLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    members.Read(reader);
}

void StructuredLiteralNode::AddMember(Node* member)
{
    member->SetParent(this);
    members.Add(member);
}

UuidLiteralNode::UuidLiteralNode(const soul::ast::Span& span_) :
    LiteralNode(NodeType::uuidLiteralNode, span_), uuid(util::nil_uuid())
{
}

UuidLiteralNode::UuidLiteralNode(const soul::ast::Span& span_, const util::uuid& uuid_) :
    LiteralNode(NodeType::uuidLiteralNode, span_), uuid(uuid_)
{
}

Node* UuidLiteralNode::Clone(CloneContext& cloneContext) const
{
    UuidLiteralNode* clone = new UuidLiteralNode(GetSpan(), uuid);
    return clone;
}

void UuidLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UuidLiteralNode::Write(AstWriter& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(uuid);
}

void UuidLiteralNode::Read(AstReader& reader)
{
    LiteralNode::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(uuid);
}

} // namespace cmajor::ast
