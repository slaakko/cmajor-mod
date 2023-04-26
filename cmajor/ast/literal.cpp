// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.literal;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

LiteralNode::LiteralNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(nodeType_, sourcePos_, moduleId_)
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

LiteralNode* CreateIntegerLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, uint64_t value, bool unsignedSuffix)
{
    if (unsignedSuffix)
    {
        if (value <= std::numeric_limits<uint8_t>::max()) return new ByteLiteralNode(sourcePos, moduleId, static_cast<uint8_t>(value));
        if (value <= std::numeric_limits<uint16_t>::max()) return new UShortLiteralNode(sourcePos, moduleId, static_cast<uint16_t>(value));
        if (value <= std::numeric_limits<uint32_t>::max()) return new UIntLiteralNode(sourcePos, moduleId, static_cast<uint32_t>(value));
        return new ULongLiteralNode(sourcePos, moduleId, value);
    }
    else
    {
        if (value <= std::numeric_limits<int8_t>::max()) return new SByteLiteralNode(sourcePos, moduleId, static_cast<int8_t>(value));
        if (value <= std::numeric_limits<uint8_t>::max()) return new ByteLiteralNode(sourcePos, moduleId, static_cast<uint8_t>(value));
        if (value <= std::numeric_limits<int16_t>::max()) return new ShortLiteralNode(sourcePos, moduleId, static_cast<int16_t>(value));
        if (value <= std::numeric_limits<uint16_t>::max()) return new UShortLiteralNode(sourcePos, moduleId, static_cast<uint16_t>(value));
        if (value <= std::numeric_limits<int32_t>::max()) return new IntLiteralNode(sourcePos, moduleId, static_cast<int32_t>(value));
        if (value <= std::numeric_limits<uint32_t>::max()) return new UIntLiteralNode(sourcePos, moduleId, static_cast<uint32_t>(value));
#pragma warning(disable : 4018)
        if (value <= std::numeric_limits<int64_t>::max()) return new LongLiteralNode(sourcePos, moduleId, static_cast<int64_t>(value));
#pragma warning(default : 4018)
        return new ULongLiteralNode(sourcePos, moduleId, value);
    }
}

LiteralNode* CreateFloatingLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, double value, bool float_)
{
    if (float_)
    {
        return new FloatLiteralNode(sourcePos, moduleId, static_cast<float>(value));
    }
    else
    {
        return new DoubleLiteralNode(sourcePos, moduleId, value);
    }
}

LiteralNode* CreateCharacterLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, char32_t value, int chrLitPrefix)
{
    switch (chrLitPrefix)
    {
    case 0:
    {
        return new CharLiteralNode(sourcePos, moduleId, static_cast<char>(value));
    }
    case 1:
    {
        return new WCharLiteralNode(sourcePos, moduleId, static_cast<char16_t>(value));
    }
    case 2:
    {
        return new UCharLiteralNode(sourcePos, moduleId, value);
    }
    }
    return nullptr;
}

LiteralNode* CreateStringLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::u32string& value, int strLitPrefix)
{
    switch (strLitPrefix)
    {
    case 0:
    {
        return new StringLiteralNode(sourcePos, moduleId, util::ToUtf8(value));
    }
    case 1:
    {
        return new WStringLiteralNode(sourcePos, moduleId, util::ToUtf16(value));
    }
    case 2:
    {
        return new UStringLiteralNode(sourcePos, moduleId, value);
    }
    }
    return nullptr;
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : LiteralNode(NodeType::booleanLiteralNode, sourcePos_, moduleId_), value(false)
{
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, bool value_) :
    LiteralNode(NodeType::booleanLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* BooleanLiteralNode::Clone(CloneContext& cloneContext) const
{
    BooleanLiteralNode* clone = new BooleanLiteralNode(GetSourcePos(), ModuleId(), value);
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

SByteLiteralNode::SByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::sbyteLiteralNode, sourcePos_, moduleId_), value(0)
{
}

SByteLiteralNode::SByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int8_t value_) :
    LiteralNode(NodeType::sbyteLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* SByteLiteralNode::Clone(CloneContext& cloneContext) const
{
    SByteLiteralNode* clone = new SByteLiteralNode(GetSourcePos(), ModuleId(), value);
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

ByteLiteralNode::ByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::byteLiteralNode, sourcePos_, moduleId_), value(0u)
{
}

ByteLiteralNode::ByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint8_t value_) :
    LiteralNode(NodeType::byteLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* ByteLiteralNode::Clone(CloneContext& cloneContext) const
{
    ByteLiteralNode* clone = new ByteLiteralNode(GetSourcePos(), ModuleId(), value);
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

ShortLiteralNode::ShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::shortLiteralNode, sourcePos_, moduleId_), value(0)
{
}

ShortLiteralNode::ShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int16_t value_) :
    LiteralNode(NodeType::shortLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* ShortLiteralNode::Clone(CloneContext& cloneContext) const
{
    ShortLiteralNode* clone = new ShortLiteralNode(GetSourcePos(), ModuleId(), value);
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

UShortLiteralNode::UShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : LiteralNode(NodeType::ushortLiteralNode, sourcePos_, moduleId_), value(0u)
{
}

UShortLiteralNode::UShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint16_t value_) :
    LiteralNode(NodeType::ushortLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* UShortLiteralNode::Clone(CloneContext& cloneContext) const
{
    UShortLiteralNode* clone = new UShortLiteralNode(GetSourcePos(), ModuleId(), value);
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

IntLiteralNode::IntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::intLiteralNode, sourcePos_, moduleId_), value(0)
{
}

IntLiteralNode::IntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int32_t value_) :
    LiteralNode(NodeType::intLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* IntLiteralNode::Clone(CloneContext& cloneContext) const
{
    IntLiteralNode* clone = new IntLiteralNode(GetSourcePos(), ModuleId(), value);
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

UIntLiteralNode::UIntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::uintLiteralNode, sourcePos_, moduleId_), value(0u)
{
}

UIntLiteralNode::UIntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint32_t value_) :
    LiteralNode(NodeType::uintLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* UIntLiteralNode::Clone(CloneContext& cloneContext) const
{
    UIntLiteralNode* clone = new UIntLiteralNode(GetSourcePos(), ModuleId(), value);
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

LongLiteralNode::LongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::longLiteralNode, sourcePos_, moduleId_), value(0)
{
}

LongLiteralNode::LongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int64_t value_) :
    LiteralNode(NodeType::longLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* LongLiteralNode::Clone(CloneContext& cloneContext) const
{
    LongLiteralNode* clone = new LongLiteralNode(GetSourcePos(), ModuleId(), value);
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

ULongLiteralNode::ULongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : LiteralNode(NodeType::ulongLiteralNode, sourcePos_, moduleId_), value(0u)
{
}

ULongLiteralNode::ULongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint64_t value_) :
    LiteralNode(NodeType::ulongLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* ULongLiteralNode::Clone(CloneContext& cloneContext) const
{
    ULongLiteralNode* clone = new ULongLiteralNode(GetSourcePos(), ModuleId(), value);
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

FloatLiteralNode::FloatLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::floatLiteralNode, sourcePos_, moduleId_), value(0)
{
}

FloatLiteralNode::FloatLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, float value_) :
    LiteralNode(NodeType::floatLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* FloatLiteralNode::Clone(CloneContext& cloneContext) const
{
    FloatLiteralNode* clone = new FloatLiteralNode(GetSourcePos(), ModuleId(), value);
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

DoubleLiteralNode::DoubleLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::doubleLiteralNode, sourcePos_, moduleId_), value(0)
{
}

DoubleLiteralNode::DoubleLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, double value_) :
    LiteralNode(NodeType::doubleLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* DoubleLiteralNode::Clone(CloneContext& cloneContext) const
{
    DoubleLiteralNode* clone = new DoubleLiteralNode(GetSourcePos(), ModuleId(), value);
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

CharLiteralNode::CharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : LiteralNode(NodeType::charLiteralNode, sourcePos_, moduleId_), value('\0')
{
}

CharLiteralNode::CharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char value_) :
    LiteralNode(NodeType::charLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* CharLiteralNode::Clone(CloneContext& cloneContext) const
{
    CharLiteralNode* clone = new CharLiteralNode(GetSourcePos(), ModuleId(), value);
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

WCharLiteralNode::WCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::wcharLiteralNode, sourcePos_, moduleId_), value('\0')
{
}

WCharLiteralNode::WCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char16_t value_) :
    LiteralNode(NodeType::wcharLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* WCharLiteralNode::Clone(CloneContext& cloneContext) const
{
    WCharLiteralNode* clone = new WCharLiteralNode(GetSourcePos(), ModuleId(), value);
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

UCharLiteralNode::UCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::ucharLiteralNode, sourcePos_, moduleId_), value('\0')
{
}

UCharLiteralNode::UCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char32_t value_) :
    LiteralNode(NodeType::ucharLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* UCharLiteralNode::Clone(CloneContext& cloneContext) const
{
    UCharLiteralNode* clone = new UCharLiteralNode(GetSourcePos(), ModuleId(), value);
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

StringLiteralNode::StringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::stringLiteralNode, sourcePos_, moduleId_), value()
{
}

StringLiteralNode::StringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::string& value_) :
    LiteralNode(NodeType::stringLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* StringLiteralNode::Clone(CloneContext& cloneContext) const
{
    StringLiteralNode* clone = new StringLiteralNode(GetSourcePos(), ModuleId(), value);
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

WStringLiteralNode::WStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::wstringLiteralNode, sourcePos_, moduleId_), value()
{
}

WStringLiteralNode::WStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u16string& value_) :
    LiteralNode(NodeType::wstringLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* WStringLiteralNode::Clone(CloneContext& cloneContext) const
{
    WStringLiteralNode* clone = new WStringLiteralNode(GetSourcePos(), ModuleId(), value);
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

UStringLiteralNode::UStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::ustringLiteralNode, sourcePos_, moduleId_), value()
{
}

UStringLiteralNode::UStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& value_) :
    LiteralNode(NodeType::ustringLiteralNode, sourcePos_, moduleId_), value(value_)
{
}

Node* UStringLiteralNode::Clone(CloneContext& cloneContext) const
{
    UStringLiteralNode* clone = new UStringLiteralNode(GetSourcePos(), ModuleId(), value);
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

NullLiteralNode::NullLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::nullLiteralNode, sourcePos_, moduleId_)
{
}

Node* NullLiteralNode::Clone(CloneContext& cloneContext) const
{
    NullLiteralNode* clone = new NullLiteralNode(GetSourcePos(), ModuleId());
    return clone;
}

void NullLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayLiteralNode::ArrayLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::arrayLiteralNode, sourcePos_, moduleId_)
{
}

Node* ArrayLiteralNode::Clone(CloneContext& cloneContext) const
{
    ArrayLiteralNode* clone = new ArrayLiteralNode(GetSourcePos(), ModuleId());
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

StructuredLiteralNode::StructuredLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::structuredLiteralNode, sourcePos_, moduleId_)
{
}

Node* StructuredLiteralNode::Clone(CloneContext& cloneContext) const
{
    StructuredLiteralNode* clone = new StructuredLiteralNode(GetSourcePos(), ModuleId());
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

UuidLiteralNode::UuidLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    LiteralNode(NodeType::uuidLiteralNode, sourcePos_, moduleId_), uuid(util::nil_uuid())
{
}

UuidLiteralNode::UuidLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const util::uuid& uuid_) :
    LiteralNode(NodeType::uuidLiteralNode, sourcePos_, moduleId_), uuid(uuid_)
{
}

Node* UuidLiteralNode::Clone(CloneContext& cloneContext) const
{
    UuidLiteralNode* clone = new UuidLiteralNode(GetSourcePos(), ModuleId(), uuid);
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
