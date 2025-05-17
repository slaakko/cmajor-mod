// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.value;

import cmajor.systemx.object.expr;
import cmajor.systemx.object.type;
import cmajor.systemx.object.expr.lexer;
import cmajor.systemx.object.expr.parser;
import util;

namespace cmajor::systemx::object {

EvaluationContext::EvaluationContext(int64_t address_, uint64_t rv_, cmajor::systemx::machine::Memory& memory_, StringTable* stringTable_, SymbolTable* symbolTable_, 
    TypeTable* typeTable_, std::vector<std::unique_ptr<TypedValue>>* indexedValues_, std::vector<std::unique_ptr<TypedValue>>* values_,
    std::vector<std::unique_ptr<TypedValue>>* stack_, int64_t pageSize_) :
    address(address_), rv(rv_), memory(memory_), stringTable(stringTable_), symbolTable(symbolTable_), typeTable(typeTable_), 
    indexedValues(indexedValues_), values(values_), stack(stack_), start(0), count(0), content(false), pageSize(pageSize_)
{
}

void EvaluationContext::Push(TypedValue* value)
{
    stack->push_back(std::unique_ptr<TypedValue>(value));
}

std::unique_ptr<TypedValue> EvaluationContext::Pop()
{
    if (stack->empty())
    {
        throw std::runtime_error("evaluation stack is empty");
    }
    std::unique_ptr<TypedValue> top = std::move(stack->back());
    stack->pop_back();
    return top;
}

TypedValue::TypedValue(TypedValueKind kind_) : kind(kind_), index(-1), type(nullptr), address(0), start(0)
{
}

TypedValue::~TypedValue()
{
}

std::string TypedValue::ToString(bool crop) const
{
    if (index != -1)
    {
        std::string s(1, '$');
        s.append(std::to_string(index)).append(" = ");
        return s;
    }
    else
    {
        return std::string();
    }
}

ValueRef::ValueRef(TypedValue* value_) : TypedValue(TypedValueKind::valueRef), value(value_)
{
    SetType(value->GetType());
}

std::string ValueRef::ToString(bool crop) const
{
    return value->ToString(crop);
}

void ValueRef::SetType(Type* type_)
{
    TypedValue::SetType(type_);
    value->SetType(type_);
}

ErrorValue::ErrorValue() : TypedValue(TypedValueKind::errorValue)
{
}

std::string ErrorValue::ToString(bool crop) const
{
    return "<error>";
}

BoolValue::BoolValue(bool value_) : TypedValue(TypedValueKind::boolValue), value(value_)
{
}

std::string BoolValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("bool").append(1, ' ').append(value ? "true" : "false").append(", #").
        append(util::ToHexString(static_cast<uint8_t>(value))).append(1, '>');
    return s;
}

SByteValue::SByteValue(int8_t value_) : TypedValue(TypedValueKind::sbyteValue), value(value_)
{
}

std::string SByteValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("sbyte").append(1, ' ').append(std::to_string(static_cast<int>(value))).append(", #").
        append(util::ToHexString(static_cast<uint8_t>(value))).append(1, '>');
    return s;
}

ByteValue::ByteValue(uint8_t value_) : TypedValue(TypedValueKind::byteValue), value(value_)
{
}

std::string ByteValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("byte").append(1, ' ').append(std::to_string(static_cast<int>(value))).append(", #").
        append(util::ToHexString(value)).append(1, '>');
    return s;
}

ShortValue::ShortValue(int16_t value_) : TypedValue(TypedValueKind::shortValue), value(value_)
{
}

std::string ShortValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("short").append(1, ' ').append(std::to_string(static_cast<int>(value))).append(", #").
        append(util::ToHexString(static_cast<uint16_t>(value))).append(1, '>');
    return s;
}

UShortValue::UShortValue(uint16_t value_) : TypedValue(TypedValueKind::ushortValue), value(value_)
{
}

std::string UShortValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("ushort").append(1, ' ').append(std::to_string(static_cast<int>(value))).append(", #").
        append(util::ToHexString(value)).append(1, '>');
    return s;
}

IntValue::IntValue(int32_t value_) : TypedValue(TypedValueKind::intValue), value(value_)
{
}

std::string IntValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("int").append(1, ' ').append(std::to_string(static_cast<int>(value))).append(", #").
        append(util::ToHexString(static_cast<uint32_t>(value))).append(1, '>');
    return s;
}

UIntValue::UIntValue(uint32_t value_) : TypedValue(TypedValueKind::uintValue), value(value_)
{
}

std::string UIntValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("uint").append(1, ' ').append(std::to_string(value)).append(", #").
        append(util::ToHexString(value)).append(1, '>');
    return s;
}

LongValue::LongValue(int64_t value_) : TypedValue(TypedValueKind::longValue), value(value_)
{
}

std::string LongValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("long").append(1, ' ').append(std::to_string(value)).append(", #").
        append(util::ToHexString(static_cast<uint64_t>(value))).append(1, '>');
    return s;
}

ULongValue::ULongValue(uint64_t value_) : TypedValue(TypedValueKind::ulongValue), value(value_)
{
}

std::string ULongValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("ulong").append(1, ' ').append(std::to_string(value)).append(", #").
        append(util::ToHexString(value)).append(1, '>');
    return s;
}

FloatValue::FloatValue(float value_) : TypedValue(TypedValueKind::floatValue), value(value_)
{
}

std::string FloatValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("float").append(1, ' ').append(std::to_string(value)).append(1, '>');
    return s;
}

DoubleValue::DoubleValue(double value_) : TypedValue(TypedValueKind::doubleValue), value(value_)
{
}

std::string DoubleValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("double").append(1, ' ').append(std::to_string(value)).append(1, '>');
    return s;
}

CharValue::CharValue(char value_) : TypedValue(TypedValueKind::charValue), value(value_)
{
}

std::string CharValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("char").append(" '").append(util::CharStr(value)).append("'>");
    return s;
}

WCharValue::WCharValue(char16_t value_) : TypedValue(TypedValueKind::wcharValue), value(value_)
{
}

std::string WCharValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("wchar").append(" '").append(util::ToUtf8(util::CharStr(static_cast<char32_t>(value)))).append("'>");
    return s;
}

UCharValue::UCharValue(char32_t value_) : TypedValue(TypedValueKind::ucharValue), value(value_)
{
}

std::string UCharValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<');
    s.append("uchar").append(" '").append(util::ToUtf8(util::CharStr(value))).append("'>");
    return s;
}

FieldValue::FieldValue(const std::string& name_, TypedValue* value_) : name(name_), value(value_)
{
}

std::string FieldValue::ToString(bool crop) const
{
    std::string s = name;
    s.append(1, '=').append(value->ToString(crop));
    return s;
}

StructureValue::StructureValue() : TypedValue(TypedValueKind::structureValue)
{
}

void StructureValue::AddFieldValue(FieldValue&& fieldValue)
{
    fieldIndexMap[fieldValue.Name()] = static_cast<int64_t>(fieldValues.size());
    fieldValues.push_back(std::move(fieldValue));
}

const FieldValue& StructureValue::GetFieldValue(int64_t index) const
{
    if (index >= 0 && index < static_cast<int64_t>(fieldValues.size()))
    {
        return fieldValues[index];
    }
    else
    {
        throw std::runtime_error("invalid field index");
    }
}

const FieldValue& StructureValue::GetFieldValue(const std::string& fieldName) const
{
    auto it = fieldIndexMap.find(fieldName);
    if (it != fieldIndexMap.end())
    {
        int64_t index = it->second;
        return GetFieldValue(index);
    }
    else
    {
        throw std::runtime_error("field name '" + fieldName + "' not found");
    }
}

std::string StructureValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append("{ ");
    bool first = true;
    for (const auto& fieldValue : fieldValues)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        s.append(fieldValue.ToString(crop));
    }
    s.append(" }");
    return s;
}

ElementValue::ElementValue(int64_t index_, TypedValue* value_) : TypedValue(TypedValueKind::elementValue), index(index_), value(value_)
{
    SetType(value->GetType());
}

std::string ElementValue::ToString(bool crop) const
{
    std::string s(1, '[');
    s.append(std::to_string(index)).append(1, ']').append(" = ");
    s.append(value->ToString(crop));
    return s;
}

ArrayValue::ArrayValue() : TypedValue(TypedValueKind::arrayValue)
{
}

void ArrayValue::AddElementValue(ElementValue&& elementValue)
{
    elementValues.push_back(std::move(elementValue));
}

std::string ArrayValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    for (const auto& elementValue : elementValues)
    {
        s.append(1, '\n').append(elementValue.ToString(crop));
    }
    return s;
}

FunctionValue::FunctionValue() : TypedValue(TypedValueKind::functionValue), name()
{
}

void FunctionValue::SetName(const std::string& name_)
{
    name = name_;
}

std::string FunctionValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append("<function ");
    s.append(name).append(1, '>');
    return s;
}

PointerValue::PointerValue() : TypedValue(TypedValueKind::pointerValue), strValue(nullptr)
{
}

std::string PointerValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append("<ptr");
    s.append(1, ' ').append(1, '#');
    s.append(util::ToHexString(Address()));
    if (strValue)
    {
        s.append(" value=\"").append(strValue->ToString(true)).append(1, '"');
    }
    s.append(1, '>');
    return s;
}

StringValue::StringValue() : TypedValue(TypedValueKind::stringValue)
{
}

void StringValue::SetStr(const std::string& str_)
{
    str = str_;
}

std::string StringValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    if (crop)
    {
        s.append(util::StringStr(str));
    }
    else
    {
        s.append(1, '<');
        s.append("string length=").append(std::to_string(str.length())).append(", value=").append(1, '"');
        s.append(util::StringStr(str)).append(1, '"').append(1, '>');
    }
    return s;
}

WStringValue::WStringValue() : TypedValue(TypedValueKind::wstringValue)
{
}

void WStringValue::SetStr(const std::u16string& str_)
{
    str = str_;
}

std::string WStringValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    if (crop)
    {
        s.append(util::StringStr(util::ToUtf8(str)));
    }
    else
    {
        s.append(1, '<');
        s.append("wstring length=").append(std::to_string(str.length())).append(", value=").append(1, '"');
        s.append(util::StringStr(util::ToUtf8(str))).append(1, '"').append(1, '>');
    }
    return s;
}

UStringValue::UStringValue() : TypedValue(TypedValueKind::ustringValue)
{
}

void UStringValue::SetStr(const std::u32string& str_)
{
    str = str_;
}

std::string UStringValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    if (crop)
    {
        s.append(util::StringStr(util::ToUtf8(str)));
    }
    else
    {
        s.append(1, '<');
        s.append("ustring length=").append(std::to_string(str.length())).append(", value=").append(1, '"');
        s.append(util::StringStr(util::ToUtf8(str))).append(1, '"').append(1, '>');
    }
    return s;
}

ContainerValue::ContainerValue() : TypedValue(TypedValueKind::containerValue), count(0), content(false), nextStart(-1), elements()
{
}

void ContainerValue::AddElement(TypedValue* element)
{
    elements.push_back(std::unique_ptr<TypedValue>(element));
}

std::string ContainerValue::ToString(bool crop) const
{
    std::string s = TypedValue::ToString(crop);
    s.append(1, '<').append(GetType()->Name()).append(" count=").append(std::to_string(count)).append(1, '>');
    if (content)
    {
        for (const auto& element : elements)
        {
            s.append(1, '\n').append(element->ToString(crop));
        }
        if (nextStart != -1)
        {
            s.append("\nENTER=next page");
        }
    }
    return s;
}

TypedValue* Evaluate(const std::string& expr, EvaluationContext& context)
{
    std::u32string xpr = util::ToUtf32(expr);
    auto lexer = cmajor::systemx::object::expr::lexer::MakeLexer(xpr.c_str(), xpr.c_str() + xpr.length(), "<expr>");
    using LexerType = decltype(lexer);
    std::unique_ptr<Expr> expression = cmajor::systemx::object::expr::parser::ExprParser<LexerType>::Parse(lexer);
    expression->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    TypedValue* result = value->Get();
    value->SetIndex(context.indexedValues->size());
    context.indexedValues->push_back(std::move(value));
    return result;
}

} // namespace cmajor::systemx::object
