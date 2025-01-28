// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.constant;

import cmajor.systemx.assembler.visitor;
import cmajor.systemx.assembler.expression;
import util;

namespace cmajor::systemx::assembler {

Constant::Constant(NodeKind kind_, const soul::ast::SourcePos& sourcePos_) : Node(kind_, sourcePos_)
{
}

IntegralConstant::IntegralConstant(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, uint64_t value_) : Constant(kind_, sourcePos_), value(value_)
{
}

DecimalConstant::DecimalConstant(const soul::ast::SourcePos& sourcePos_, uint64_t value_) : IntegralConstant(NodeKind::decimalConstantNode, sourcePos_, value_)
{
}

void DecimalConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DecimalConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write(std::to_string(Value()));
}

HexadecimalConstant::HexadecimalConstant(const soul::ast::SourcePos& sourcePos_, uint64_t value_) : IntegralConstant(NodeKind::hexConstantNode, sourcePos_, value_)
{
}

void HexadecimalConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void HexadecimalConstant::Write(util::CodeFormatter& formatter)
{
    if (Value() <= std::numeric_limits<uint8_t>::max())
    {
        formatter.Write("#" + util::ToHexString(static_cast<uint8_t>(Value())));
    }
    else if (Value() <= std::numeric_limits<uint16_t>::max())
    {
        formatter.Write("#" + util::ToHexString(static_cast<uint16_t>(Value())));
    }
    else if (Value() <= std::numeric_limits<uint32_t>::max())
    {
        formatter.Write("#" + util::ToHexString(static_cast<uint32_t>(Value())));
    }
    else
    {
        formatter.Write("#" + util::ToHexString(Value()));
    }
}

ByteConstant::ByteConstant(const soul::ast::SourcePos& sourcePos_, uint8_t value_) : IntegralConstant(NodeKind::byteConstantNode, sourcePos_, value_)
{
}

void ByteConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ByteConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write("#" + util::ToHexString(static_cast<uint8_t>(Value())));
}

WydeConstant::WydeConstant(const soul::ast::SourcePos& sourcePos_, uint16_t value_) : IntegralConstant(NodeKind::wydeConstantNode, sourcePos_, value_)
{
}

void WydeConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WydeConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write("#" + util::ToHexString(static_cast<uint16_t>(Value())));
}

TetraConstant::TetraConstant(const soul::ast::SourcePos& sourcePos_, uint32_t value_) : IntegralConstant(NodeKind::tetraConstantNode, sourcePos_, value_)
{
}

void TetraConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TetraConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write("#" + util::ToHexString(static_cast<uint32_t>(Value())));
}

CharacterConstant::CharacterConstant(const soul::ast::SourcePos& sourcePos_, char32_t value_) : Constant(NodeKind::characterConstantNode, sourcePos_), value(value_)
{
}

void CharacterConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharacterConstant::Write(util::CodeFormatter& formatter)
{
    std::u32string c(1, value);
    formatter.Write("'" + util::ToUtf8(c) + "'");
}

StringConstant::StringConstant(const soul::ast::SourcePos& sourcePos_, const std::u32string& value_) : Constant(NodeKind::stringConstantNode, sourcePos_), value(value_)
{
}

void StringConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write("\"" + util::ToUtf8(value) + "\"");
}

ClsIdConstant::ClsIdConstant(const soul::ast::SourcePos& sourcePos_, const std::string& typeId_) : Constant(NodeKind::clsIdConstantNode, sourcePos_), typeId(typeId_)
{
}

void ClsIdConstant::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ClsIdConstant::Write(util::CodeFormatter& formatter)
{
    formatter.Write("$CLSID(" + typeId + ")");
}

Node* MakeConstantExpr(bool value)
{
    if (value)
    {
        return new ByteConstant(soul::ast::SourcePos(), static_cast<uint8_t>(1));
    }
    else
    {
        return new ByteConstant(soul::ast::SourcePos(), static_cast<uint8_t>(0));
    }
}

Node* MakeConstantExpr(int8_t value)
{
    return MakeConstantExpr(static_cast<uint8_t>(value));
}

Node* MakeConstantExpr(uint8_t value)
{
    return new ByteConstant(soul::ast::SourcePos(), value);
}

Node* MakeConstantExpr(int16_t value)
{
    return new WydeConstant(soul::ast::SourcePos(), static_cast<uint16_t>(value));
}

Node* MakeConstantExpr(uint16_t value)
{
    return new WydeConstant(soul::ast::SourcePos(), value);
}

Node* MakeConstantExpr(int32_t value)
{
    return MakeConstantExpr(static_cast<uint32_t>(value));
}

Node* MakeConstantExpr(uint32_t value)
{
    return new TetraConstant(soul::ast::SourcePos(), value);
}

Node* MakeConstantExpr(int64_t value)
{
    return new HexadecimalConstant(soul::ast::SourcePos(), static_cast<uint64_t>(value));
}

Node* MakeConstantExpr(uint64_t value)
{
    return new HexadecimalConstant(soul::ast::SourcePos(), value);
}

Node* MakeConstantExpr(uint64_t value, bool hex)
{
    if (hex)
    {
        return new HexadecimalConstant(soul::ast::SourcePos(), value);
    }
    else
    {
        return new DecimalConstant(soul::ast::SourcePos(), value);
    }
}

Node* MakeConstantExpr(float value)
{
    double v = value;
    return MakeConstantExpr(v);
}

Node* MakeConstantExpr(double value)
{
    uint64_t x = *static_cast<uint64_t*>(static_cast<void*>(&value));
    return MakeConstantExpr(x, true);
}

Node* MakeConstantExpr(const std::string& str)
{
    return new StringConstant(soul::ast::SourcePos(), util::ToUtf32(str));
}

} // namespace cmajor::systemx::assembler
