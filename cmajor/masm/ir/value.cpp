// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.ir.value;

import cmajor.masm.ir.context;
import util;

namespace cmajor::masm::ir {

Value::Value()
{
}

Value::~Value()
{
}

BoolValue::BoolValue() : value(false)
{
}

BoolValue::BoolValue(bool value_) : value(value_)
{
}

std::string BoolValue::Name(Context& context)
{
    return value ? "true" : "false";
}

Type* BoolValue::GetType(Context& context)
{
    return context.GetBoolType();
}

SByteValue::SByteValue() : value()
{
}

SByteValue::SByteValue(int8_t value_) : value(value_)
{
}

std::string SByteValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* SByteValue::GetType(Context& context)
{
    return context.GetSByteType();
}

ByteValue::ByteValue() : value()
{
}

ByteValue::ByteValue(uint8_t value_) : value(value_)
{
}

std::string ByteValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* ByteValue::GetType(Context& context)
{
    return context.GetByteType();
}

ShortValue::ShortValue() : value()
{
}

ShortValue::ShortValue(int16_t value_) : value(value_)
{
}

std::string ShortValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* ShortValue::GetType(Context& context)
{
    return context.GetShortType();
}

UShortValue::UShortValue() : value()
{
}

UShortValue::UShortValue(uint16_t value_) : value(value_)
{
}

std::string UShortValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* UShortValue::GetType(Context& context)
{
    return context.GetUShortType();
}

IntValue::IntValue() : value()
{
}

IntValue::IntValue(int32_t value_) : value(value_)
{
}

std::string IntValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* IntValue::GetType(Context& context)
{
    return context.GetIntType();
}

UIntValue::UIntValue() : value()
{
}

UIntValue::UIntValue(uint32_t value_) : value(value_)
{
}

std::string UIntValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* UIntValue::GetType(Context& context)
{
    return context.GetUIntType();
}

LongValue::LongValue() : value()
{
}

LongValue::LongValue(int64_t value_) : value(value_)
{
}

std::string LongValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* LongValue::GetType(Context& context)
{
    return context.GetLongType();
}

ULongValue::ULongValue() : value()
{
}

ULongValue::ULongValue(uint64_t value_) : value(value_)
{
}

std::string ULongValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* ULongValue::GetType(Context& context)
{
    return context.GetULongType();
}

FloatValue::FloatValue() : value()
{
}

FloatValue::FloatValue(float value_) : value(value_)
{
}

std::string FloatValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* FloatValue::GetType(Context& context)
{
    return context.GetFloatType();
}

DoubleValue::DoubleValue() : value()
{
}

DoubleValue::DoubleValue(double value_) : value(value_)
{
}

std::string DoubleValue::Name(Context& context)
{
    return std::to_string(value);
}

Type* DoubleValue::GetType(Context& context)
{
    return context.GetDoubleType();
}

NullValue::NullValue(PtrType* ptrType_) : ptrType(ptrType_)
{
}

std::string NullValue::Name(Context& context)
{
    return "null";
}

Type* NullValue::GetType(Context& context)
{
    return ptrType;
}

ArrayValue::ArrayValue(Type* type_, const std::vector<Value*>& elements_, const std::string& prefix_) : Value(), type(type_), elements(elements_), prefix(prefix_)
{
}

std::string ArrayValue::Name(Context& context)
{
    std::string name = prefix + "[ ";
    bool first = true;
    for (Value* element : elements)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            name.append(", ");
        }
        if (element->IsAggregateValue())
        {
            name.append(element->Name(context));
        }
        else
        {
            name.append(element->GetType(context)->Name()).append(" ").append(element->Name(context));
        }
    }
    name.append(" ]");
    return name;
}

void ArrayValue::AddElement(Value* element)
{
    elements.push_back(element);
}

StructureValue::StructureValue(Type* type_, const std::vector<Value*>& members_) : Value(), type(type_), members(members_)
{
}

Type* StructureValue::GetType(Context& context)
{
    return type;
}

std::string StructureValue::Name(Context& context)
{
    std::string name = "{ ";
    bool first = true;
    for (Value* member : members)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            name.append(", ");
        }
        if (member->IsAggregateValue())
        {
            name.append(member->Name(context));
        }
        else
        {
            name.append(member->GetType(context)->Name()).append(" ").append(member->Name(context));
        }
    }
    name.append(" }");
    return name;
}

void StructureValue::AddMember(Value* member)
{
    members.push_back(member);
}

StringValue::StringValue(Type* type_, const std::string& value_) : Value(), type(type_), value(value_)
{
}

std::string StringValue::Name(Context& context)
{
    std::string s("\"");
    for (char c : value)
    {
        if (c == '"')
        {
            s.append("\\").append(util::ToHexString(static_cast<uint8_t>(c)));
        }
        else if (c == '\\')
        {
            s.append("\\").append(util::ToHexString(static_cast<uint8_t>(c)));
        }
        else if (c >= 32 && c < 127)
        {
            s.append(1, c);
        }
        else
        {
            s.append("\\").append(util::ToHexString(static_cast<uint8_t>(c)));
        }
    }
    s.append("\\").append(util::ToHexString(static_cast<uint8_t>(0)));
    s.append("\"");
    return s;
}

Type* StringValue::GetType(Context& context)
{
    return type;
}

ConversionValue::ConversionValue(Type* type_, Value* from_) : Value(), type(type_), from(from_)
{
}

std::string ConversionValue::Name(Context& context)
{
    std::string name = "conv(";
    name.append(from->GetType(context)->Name()).append(1, ' ').append(from->Name(context)).append(1, ')');
    return name;
}

Type* ConversionValue::GetType(Context& context)
{
    return type;
}

ClsIdValue::ClsIdValue(const std::string& typeId_) : Value(), typeId(typeId_)
{
}

std::string ClsIdValue::Name(Context& context)
{
    std::string name = "clsid(" + typeId + ")";
    return name;
}

Type* ClsIdValue::GetType(Context& context)
{
    return context.GetPtrType(context.GetVoidType());
}

} // namespace cmajor::masm::ir
