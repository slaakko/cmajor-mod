// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.value;

import cmajor.systemx.intermediate.visitor;

namespace cmajor::systemx::intermediate {

const char* valueKindStr[]
{
    "boolValue", "sbyteValue", "byteValue", "shortValue", "ushortValue", "intValue", "uintValue", "longValue", "ulongValue", "floatValue", "doubleValue", 
    "charValue", "wcharValue", "ucharValue",
    "nullValue", "addressValue",
    "arrayValue", "structureValue", "stringValue", "stringArrayValue", "conversionValue", "clsIdValue", "symbolValue",
    "globalVariable",
    "regValue",
    "instruction",
    "function"
};

Value::Value(const soul::ast::SourcePos& sourcePos_, ValueKind kind_, Type* type_) : sourcePos(sourcePos_), kind(kind_), type(type_)
{
}

Value::~Value()
{
}

bool Value::IsIntegerValue() const
{
    switch (kind)
    {
        case ValueKind::sbyteValue:
        case ValueKind::byteValue:
        case ValueKind::shortValue:
        case ValueKind::ushortValue:
        case ValueKind::intValue:
        case ValueKind::uintValue:
        case ValueKind::longValue:
        case ValueKind::ulongValue:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

int64_t Value::GetIntegerValue() const
{
    switch (kind)
    {
        case ValueKind::sbyteValue:
        {
            const SByteValue* sbyteValue = static_cast<const SByteValue*>(this);
            return sbyteValue->GetValue();
        }
        case ValueKind::byteValue:
        {
            const ByteValue* byteValue = static_cast<const ByteValue*>(this);
            return byteValue->GetValue();
        }
        case ValueKind::shortValue:
        {
            const ShortValue* shortValue = static_cast<const ShortValue*>(this);
            return shortValue->GetValue();
        }
        case ValueKind::ushortValue:
        {
            const UShortValue* ushortValue = static_cast<const UShortValue*>(this);
            return ushortValue->GetValue();
        }
        case ValueKind::intValue:
        {
            const IntValue* intValue = static_cast<const IntValue*>(this);
            return intValue->GetValue();
        }
        case ValueKind::uintValue:
        {
            const UIntValue* uintValue = static_cast<const UIntValue*>(this);
            return uintValue->GetValue();
        }
        case ValueKind::longValue:
        {
            const LongValue* longValue = static_cast<const LongValue*>(this);
            return longValue->GetValue();
        }
        case ValueKind::ulongValue:
        {
            const ULongValue* ulongValue = static_cast<const ULongValue*>(this);
            return static_cast<int64_t>(ulongValue->GetValue());
        }
        default:
        {
            return -1;
        }
    }
}

void Value::SetType(Type* type_)
{
    if (!type)
    {
        type = type_;
    }
    if (type != type_)
    {
        throw std::runtime_error("type conflict");
    }
}

std::string Value::KindStr() const
{
    return valueKindStr[static_cast<int>(kind)];
}

ConstantValue::ConstantValue(const soul::ast::SourcePos& sourcePos_, ValueKind kind_, Type* type_) : Value(sourcePos_, kind_, type_)
{
}

BoolValue::BoolValue(bool value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::boolValue, type_), value(value_)
{
}

void BoolValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BoolValue::ToString() const
{
    return value ? "true" : "false";
}

SByteValue::SByteValue(int8_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::sbyteValue, type_), value(value_)
{
}

void SByteValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SByteValue::ToString() const
{
    return std::to_string(value);
}

ByteValue::ByteValue(uint8_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::byteValue, type_), value(value_)
{
}

void ByteValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ByteValue::ToString() const
{
    return std::to_string(value);
}

ShortValue::ShortValue(int16_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::shortValue, type_), value(value_)
{
}

void ShortValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ShortValue::ToString() const
{
    return std::to_string(value);
}

UShortValue::UShortValue(uint16_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::ushortValue, type_), value(value_)
{
}

void UShortValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UShortValue::ToString() const
{
    return std::to_string(value);
}

IntValue::IntValue(int32_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::intValue, type_), value(value_)
{
}

void IntValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string IntValue::ToString() const
{
    return std::to_string(value);
}

UIntValue::UIntValue(uint32_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::uintValue, type_), value(value_)
{
}

void UIntValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UIntValue::ToString() const
{
    return std::to_string(value);
}

LongValue::LongValue(int64_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::longValue, type_), value(value_)
{
}

void LongValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string LongValue::ToString() const
{
    return std::to_string(value);
}

ULongValue::ULongValue(uint64_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::ulongValue, type_), value(value_)
{
}

void ULongValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ULongValue::ToString() const
{
    return std::to_string(value);
}

FloatValue::FloatValue(float value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::floatValue, type_), value(value_)
{
}

void FloatValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string FloatValue::ToString() const
{
    return std::to_string(value);
}

DoubleValue::DoubleValue(double value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::doubleValue, type_), value(value_)
{
}

void DoubleValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DoubleValue::ToString() const
{
    return std::to_string(value);
}

CharValue::CharValue(char value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::charValue, type_), value(value_)
{
}

void CharValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string CharValue::ToString() const
{
    return std::to_string(static_cast<uint8_t>(value));
}

WCharValue::WCharValue(char16_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::wcharValue, type_), value(value_)
{
}

void WCharValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string WCharValue::ToString() const
{
    return std::to_string(static_cast<uint16_t>(value));
}

UCharValue::UCharValue(char32_t value_, Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::ucharValue, type_), value(value_)
{
}

void UCharValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UCharValue::ToString() const
{
    return std::to_string(static_cast<uint32_t>(value));
}

NullValue::NullValue(Type* type_) : ConstantValue(soul::ast::SourcePos(), ValueKind::nullValue, type_)
{
}

void NullValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string NullValue::ToString() const
{
    return "null";
}

} // cmajor::systemx::intermediate
