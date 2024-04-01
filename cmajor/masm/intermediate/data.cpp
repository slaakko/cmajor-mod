// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.data;

import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.error;
import cmajor.masm.intermediate.visitor;

namespace cmajor::masm::intermediate {

CloneContext::CloneContext() : context(nullptr), currentFunction(nullptr)
{
}

void CloneContext::MapInstruction(Instruction* inst, Instruction* clone)
{
    instMap[inst] = clone;
}

Instruction* CloneContext::GetMappedInstruction(Instruction* inst) const
{
    auto it = instMap.find(inst);
    if (it != instMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CloneContext::AddUnmappedInstruction(Instruction* inst, RegValue* regValue)
{
    unmappedInstructions.insert(std::make_pair(inst, regValue));
}

BasicBlock* CloneContext::GetMappedBasicBlock(BasicBlock* bb) const
{
    auto it = bbMap.find(bb);
    if (it != bbMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CloneContext::MapBasicBlock(BasicBlock* bb, BasicBlock* clone)
{
    bbMap[bb] = clone;
}

const char* valueKindStr[]
{
    "boolValue", "sbyteValue", "byteValue", "shortValue", "ushortValue", "intValue", "uintValue", "longValue", "ulongValue", "floatValue", "doubleValue", "nullValue",
    "addressValue",
    "arrayValue", "structureValue", "stringValue", "stringArrayValue", "conversionValue", "clsIdValue", "symbolValue",
    "globalVariable",
    "regValue",
    "instruction"
};

Value::Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_) : span(span_), kind(kind_), type(type_)
{
}

Value::~Value()
{
}

void Value::Accept(Visitor& visitor)
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

bool Value::IsFloatingPointValue() const
{
    return kind == ValueKind::floatValue || kind == ValueKind::doubleValue;
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

BoolValue::BoolValue(bool value_, Type* type_) : Value(soul::ast::Span(), ValueKind::boolValue, type_), value(value_)
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

SByteValue::SByteValue(int8_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::sbyteValue, type_), value(value_)
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

ByteValue::ByteValue(uint8_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::byteValue, type_), value(value_)
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

ShortValue::ShortValue(int16_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::shortValue, type_), value(value_)
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

UShortValue::UShortValue(uint16_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::ushortValue, type_), value(value_)
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

IntValue::IntValue(int32_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::intValue, type_), value(value_)
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

UIntValue::UIntValue(uint32_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::uintValue, type_), value(value_)
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

LongValue::LongValue(int64_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::longValue, type_), value(value_)
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

ULongValue::ULongValue(uint64_t value_, Type* type_) : Value(soul::ast::Span(), ValueKind::ulongValue, type_), value(value_)
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

FloatValue::FloatValue(float value_, Type* type_) : Value(soul::ast::Span(), ValueKind::floatValue, type_), value(value_)
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

DoubleValue::DoubleValue(double value_, Type* type_) : Value(soul::ast::Span(), ValueKind::doubleValue, type_), value(value_)
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

NullValue::NullValue(Type* type_) : Value(soul::ast::Span(), ValueKind::nullValue, type_)
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

AddressValue::AddressValue(const soul::ast::Span& span_, GlobalVariable* globalVariable_, Type* type) :
    Value(span_, ValueKind::addressValue, type), globalVariable(globalVariable_)
{
}

void AddressValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string AddressValue::ToString() const
{
    return globalVariable->Name();
}

ArrayValue::ArrayValue(const soul::ast::Span& span_, const std::vector<Value*>& elements_) :
    Value(span_, ValueKind::arrayValue, nullptr), elements(elements_)
{
}

void ArrayValue::SetType(Type* type)
{
    Value::SetType(type);
    if (type->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(type);
        for (auto& element : elements)
        {
            element->SetType(arrayType->ElementType());
        }
    }
    else
    {
        throw std::runtime_error("array type expected");
    }
}

void ArrayValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ArrayValue::ToString() const
{
    std::string s = "[ ";
    bool first = true;
    for (Value* element : elements)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        if (element->IsAggregateValue())
        {
            s.append(element->ToString());
        }
        else
        {
            s.append(element->GetType()->Name()).append(" ").append(element->ToString());
        }
    }
    s.append(" ]");
    return s;
}

StructureValue::StructureValue(const soul::ast::Span& span_, const std::vector<Value*>& fieldValues_) :
    Value(span_, ValueKind::structureValue, nullptr), fieldValues(fieldValues_)
{
}

void StructureValue::SetType(Type* type)
{
    Value::SetType(type);
    if (type->IsStructureType())
    {
        StructureType* structureType = static_cast<StructureType*>(type);
        int n = fieldValues.size();
        for (int i = 0; i < n; ++i)
        {
            Value* fieldValue = fieldValues[i];
            fieldValue->SetType(structureType->FieldType(i));
        }
    }
    else
    {
        throw std::runtime_error("structure type expected");
    }
}

void StructureValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StructureValue::ToString() const
{
    std::string s = "{ ";
    bool first = true;
    for (Value* field : fieldValues)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        if (field->IsAggregateValue())
        {
            s.append(field->ToString());
        }
        else
        {
            s.append(field->GetType()->Name()).append(" ").append(field->ToString());
        }
    }
    s.append(" }");
    return s;
}

StringValue::StringValue(const soul::ast::Span& span_, const std::string& value_) :
    Value(span_, ValueKind::stringValue, nullptr), value(value_)
{
}

void StringValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StringValue::ToString() const
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

StringArrayValue::StringArrayValue(const soul::ast::Span& span_, char prefix_, const std::vector<Value*>& strings_) :
    Value(span_, ValueKind::stringArrayValue, nullptr), prefix(prefix_), strings(strings_)
{
}

void StringArrayValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StringArrayValue::ToString() const
{
    std::string s = prefix + "[ ";
    bool first = true;
    for (Value* string : strings)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        s.append(string->GetType()->Name()).append(" ").append(string->ToString());
    }
    s.append(" ]");
    return s;
}

ConversionValue::ConversionValue(const soul::ast::Span& span_, Type* type_, Value* from_) :
    Value(span_, ValueKind::conversionValue, type_), from(from_)
{
}

void ConversionValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ConversionValue::ToString() const
{
    std::string s = "conv(";
    s.append(from->GetType()->Name()).append(1, ' ').append(from->ToString()).append(1, ')');
    return s;
}

ClsIdValue::ClsIdValue(const soul::ast::Span& span_, Type* type_, const std::string& typeId_) :
    Value(span_, ValueKind::clsIdValue, type_), typeId(typeId_)
{
}

void ClsIdValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ClsIdValue::ToString() const
{
    std::string s = "clsid(" + typeId + ")";
    return s;
}

SymbolValue::SymbolValue(const soul::ast::Span& span_, Type* type_, const std::string& symbol_) :
    Value(span_, ValueKind::symbolValue, type_), symbol(symbol_), function(nullptr), globalVariable(nullptr)
{
}

void SymbolValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SymbolValue::ToString() const
{
    if (function)
    {
        return "@" + symbol;
    }
    else
    {
        return symbol;
    }
}

GlobalVariable::GlobalVariable(const soul::ast::Span& span_, Type* type_, const std::string& name_, Value* initializer_) :
    Value(span_, ValueKind::globalVariable, type_), name(name_), initializer(initializer_)
{
    if (initializer)
    {
        initializer->SetType(GetType());
    }
}

void GlobalVariable::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GlobalVariable::Write(util::CodeFormatter& formatter)
{
    formatter.Write(GetType()->Name());
    formatter.Write(" ");
    formatter.Write(name);
    if (initializer)
    {
        formatter.Write(" = ");
        if (initializer->IsAggregateValue() || initializer->IsStringValue())
        {
            formatter.Write(initializer->ToString());
        }
        else
        {
            formatter.Write(initializer->GetType()->Name());
            formatter.Write(" ");
            formatter.Write(initializer->ToString());
        }
    }
    else
    {
        formatter.Write(";");
    }
}

std::string GlobalVariable::ToString() const
{
    return name;
}

Data::Data() : context(nullptr)
{
}

void Data::AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer, Context* context)
{
    try
    {
        GlobalVariable* globalVariable = new GlobalVariable(span, type, variableName, initializer);
        values.push_back(std::unique_ptr<Value>(globalVariable));
        globalVariableMap[variableName] = globalVariable;
        globalVariables.push_back(globalVariable);
    }
    catch (const std::exception& ex)
    {
        Error("error adding global variable: " + std::string(ex.what()), span, context);
    }
}

Value* Data::GetTrueValue(const Types& types)
{
    if (!trueValue)
    {
        trueValue.reset(new BoolValue(true, types.Get(boolTypeId)));
    }
    return trueValue.get();
}

Value* Data::GetFalseValue(const Types& types)
{
    if (!falseValue)
    {
        falseValue.reset(new BoolValue(false, types.Get(boolTypeId)));
    }
    return falseValue.get();
}

Value* Data::GetSByteValue(int8_t value, const Types& types)
{
    return sbyteValueMap.Get(value, this, types);
}

Value* Data::GetByteValue(uint8_t value, const Types& types)
{
    return byteValueMap.Get(value, this, types);
}

Value* Data::GetShortValue(int16_t value, const Types& types)
{
    return shortValueMap.Get(value, this, types);
}

Value* Data::GetUShortValue(uint16_t value, const Types& types)
{
    return ushortValueMap.Get(value, this, types);
}

Value* Data::GetIntValue(int32_t value, const Types& types)
{
    return intValueMap.Get(value, this, types);
}

Value* Data::GetUIntValue(uint32_t value, const Types& types)
{
    return uintValueMap.Get(value, this, types);
}

Value* Data::GetLongValue(int64_t value, const Types& types)
{
    return longValueMap.Get(value, this, types);
}

Value* Data::GetULongValue(uint64_t value, const Types& types)
{
    return ulongValueMap.Get(value, this, types);
}

Value* Data::GetFloatValue(float value, const Types& types)
{
    return floatValueMap.Get(value, this, types);
}

Value* Data::GetDoubleValue(double value, const Types& types)
{
    return doubleValueMap.Get(value, this, types);
}

Value* Data::GetNullValue(Type* type)
{
    auto it = nullValueMap.find(type);
    if (it != nullValueMap.cend())
    {
        return it->second;
    }
    else
    {
        NullValue* nullValue = new NullValue(type);
        nullValueMap[type] = nullValue;
        values.push_back(std::unique_ptr<Value>(nullValue));
        return nullValue;
    }
}

Value* Data::MakeValue(int8_t value, const Types& types)
{
    SByteValue* constantValue = new SByteValue(value, types.Get(sbyteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(uint8_t value, const Types& types)
{
    ByteValue* constantValue = new ByteValue(value, types.Get(byteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(int16_t value, const Types& types)
{
    ShortValue* constantValue = new ShortValue(value, types.Get(shortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(uint16_t value, const Types& types)
{
    UShortValue* constantValue = new UShortValue(value, types.Get(ushortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(int32_t value, const Types& types)
{
    IntValue* constantValue = new IntValue(value, types.Get(intTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(uint32_t value, const Types& types)
{
    UIntValue* constantValue = new UIntValue(value, types.Get(uintTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(int64_t value, const Types& types)
{
    LongValue* constantValue = new LongValue(value, types.Get(longTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(uint64_t value, const Types& types)
{
    ULongValue* constantValue = new ULongValue(value, types.Get(ulongTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(float value, const Types& types)
{
    FloatValue* constantValue = new FloatValue(value, types.Get(floatTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(double value, const Types& types)
{
    DoubleValue* constantValue = new DoubleValue(value, types.Get(doubleTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements)
{
    ArrayValue* arrayValue = new ArrayValue(span, elements);
    values.push_back(std::unique_ptr<Value>(arrayValue));
    return arrayValue;
}

Value* Data::MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues)
{
    StructureValue* structureValue = new StructureValue(span, fieldValues);
    values.push_back(std::unique_ptr<Value>(structureValue));
    return structureValue;
}

Value* Data::MakeStringValue(const soul::ast::Span& span, const std::string& value)
{
    StringValue* stringValue = new StringValue(span, value.substr(1, value.length() - 2));
    values.push_back(std::unique_ptr<Value>(stringValue));
    return stringValue;
}

Value* Data::MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& strings)
{
    StringArrayValue* stringArrayValue = new StringArrayValue(span, prefix, strings);
    values.push_back(std::unique_ptr<Value>(stringArrayValue));
    return stringArrayValue;
}

Value* Data::MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from)
{
    ConversionValue* conversionValue = new ConversionValue(span, type, from);
    values.push_back(std::unique_ptr<Value>(conversionValue));
    return conversionValue;
}

Value* Data::MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr)
{
    std::string typeId = clsIdStr.substr(6, clsIdStr.length() - 6 - 1);
    ClsIdValue* clsIdValue = new ClsIdValue(span, type, typeId);
    values.push_back(std::unique_ptr<Value>(clsIdValue));
    return clsIdValue;
}

Value* Data::MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol)
{
    SymbolValue* symbolValue = new SymbolValue(span, type, symbol);
    values.push_back(std::unique_ptr<Value>(symbolValue));
    return symbolValue;
}

Value* Data::MakeIntegerLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue, const Types& types, Context* context)
{
    switch (type->Id())
    {
    case boolTypeId:
    {
        if (strValue == "true")
        {
            return GetTrueValue(types);
        }
        else if (strValue == "false")
        {
            return GetFalseValue(types);
        }
        else
        {
            Error("error making literal: Boolean value expected", span, context);
        }
    }
    case sbyteTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<int8_t>::min() || value > std::numeric_limits<int8_t>::max())
        {
            Error("error making literal: range error: sbyte value expected", span, context);
        }
        return GetSByteValue(static_cast<int8_t>(value), types);
    }
    case byteTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<uint8_t>::min() || value > std::numeric_limits<uint8_t>::max())
        {
            Error("error making literal: range error: byte value expected", span, context);
        }
        return GetByteValue(static_cast<uint8_t>(value), types);
    }
    case shortTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<int16_t>::min() || value > std::numeric_limits<int16_t>::max())
        {
            Error("error making literal: range error: short value expected", span, context);
        }
        return GetShortValue(static_cast<int16_t>(value), types);
    }
    case ushortTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<uint16_t>::min() || value > std::numeric_limits<uint16_t>::max())
        {
            Error("error making literal: range error: ushort value expected", span, context);
        }
        return GetUShortValue(static_cast<uint16_t>(value), types);
    }
    case intTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<int32_t>::min() || value > std::numeric_limits<int32_t>::max())
        {
            Error("error making literal: range error: int value expected", span, context);
        }
        return GetIntValue(static_cast<int32_t>(value), types);
    }
    case uintTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<uint32_t>::min() || value > std::numeric_limits<uint32_t>::max())
        {
            Error("error making literal: range error: uint value expected", span, context);
        }
        return GetUIntValue(static_cast<uint32_t>(value), types);
    }
    case longTypeId:
    {
        int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<int64_t>::min() || value > std::numeric_limits<int64_t>::max())
        {
            Error("error making literal: range error: long value expected", span, context);
        }
        return GetLongValue(static_cast<int64_t>(value), types);
    }
    case ulongTypeId:
    {
        uint64_t value = std::stoull(strValue);
        if (value < std::numeric_limits<uint64_t>::min() || value > std::numeric_limits<uint64_t>::max())
        {
            Error("error making literal: range error: ulong value expected", span, context);
        }
        return GetULongValue(static_cast<int64_t>(value), types);
    }
    case floatTypeId:
    {
        float value = std::stof(strValue);
        return GetFloatValue(value, types);
    }
    case doubleTypeId:
    {
        double value = std::stod(strValue);
        return GetDoubleValue(value, types);
    }
    default:
    {
        Error("error making literal: invalid numeric value", span, context);
    }
    }
    return nullptr;
}

Value* Data::MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id, Context* context)
{
    auto it = globalVariableMap.find(id);
    if (it != globalVariableMap.cend())
    {
        GlobalVariable* globalVariable = it->second;
        AddressValue* addressValue = new AddressValue(span, globalVariable, globalVariable->GetType()->AddPointer(context));
        values.push_back(std::unique_ptr<Value>(addressValue));
        return addressValue;
    }
    else
    {
        Error("error making address literal: global variable id '" + id + "' not found", span, context);
    }
    return nullptr;
}

void Data::VisitGlobalVariables(Visitor& visitor)
{
    for (GlobalVariable* globalVariable : globalVariables)
    {
        globalVariable->Accept(visitor);
    }
}

void Data::Write(util::CodeFormatter& formatter)
{
    if (globalVariables.empty()) return;
    formatter.WriteLine("data");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& globalVariable : globalVariables)
    {
        globalVariable->Write(formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

} // cmajor::systemx::intermediate
