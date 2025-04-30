// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.data;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.visitor;

namespace cmajor::systemx::intermediate {

AddressValue::AddressValue(const soul::ast::SourcePos& sourcePos_, GlobalVariable* globalVariable_, Type* type) :
    ConstantValue(sourcePos_, ValueKind::addressValue, type), globalVariable(globalVariable_)
{
}

AddressValue::AddressValue(const soul::ast::SourcePos& sourcePos_, const std::string& id_, Type* type) : 
    ConstantValue(sourcePos_, ValueKind::addressValue, type), id(id_), globalVariable(nullptr)
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

ArrayValue::ArrayValue(const soul::ast::SourcePos& sourcePos_, const std::vector<ConstantValue*>& elements_, const std::string& prefix_) :
    ConstantValue(sourcePos_, ValueKind::arrayValue, nullptr), elements(elements_), prefix(prefix_)
{
}

void ArrayValue::SetType(Type* type)
{
    ConstantValue::SetType(type);
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
    std::string s = prefix + "[ ";
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

StructureValue::StructureValue(const soul::ast::SourcePos& sourcePos_, const std::vector<ConstantValue*>& fieldValues_) :
    ConstantValue(sourcePos_, ValueKind::structureValue, nullptr), fieldValues(fieldValues_)
{
}

void StructureValue::SetType(Type* type)
{
    ConstantValue::SetType(type);
    if (type->IsStructureType())
    {
        StructureType* structureType = static_cast<StructureType*>(type);
        int n = fieldValues.size();
        for (int i = 0; i < n; ++i)
        {
            ConstantValue* fieldValue = fieldValues[i];
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

StringValue::StringValue(const soul::ast::SourcePos& sourcePos_, const std::string& value_) :
    ConstantValue(sourcePos_, ValueKind::stringValue, nullptr), value(value_)
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
        if (c == '"' || c == '\\')
        {
            s.append(util::ToHexString(static_cast<uint8_t>(c)));
        }
        else if (static_cast<uint8_t>(c) >= 32 && static_cast<uint8_t>(c) < 127)
        {
            s.append(1, c);
        }
        else
        {
            s.append(util::ToHexString(static_cast<uint8_t>(c)));
        }
    }
    s.append("\\").append(util::ToHexString(static_cast<uint8_t>(0)));
    s.append("\"");
    return s;
}

StringArrayValue::StringArrayValue(const soul::ast::SourcePos& sourcePos_, char prefix_, const std::vector<ConstantValue*>& elements_) :
    ConstantValue(sourcePos_, ValueKind::stringArrayValue, nullptr), prefix(prefix_), elements(elements_)
{
}

void StringArrayValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StringArrayValue::ToString() const
{
    std::string s = std::string(1, prefix) + "[ ";
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
        s.append(element->GetType()->Name()).append(" ").append(element->ToString());
    }
    s.append(" ]");
    return s;
}

ConversionValue::ConversionValue(const soul::ast::SourcePos& sourcePos_, Type* type_, ConstantValue* from_) :
    ConstantValue(sourcePos_, ValueKind::conversionValue, type_), from(from_)
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

ClsIdValue::ClsIdValue(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& typeId_) :
    ConstantValue(sourcePos_, ValueKind::clsIdValue, type_), typeId(typeId_)
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

SymbolValue::SymbolValue(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& symbol_) :
    ConstantValue(sourcePos_, ValueKind::symbolValue, type_), symbol(symbol_), function(nullptr), globalVariable(nullptr)
{
}

void SymbolValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SymbolValue::ToString() const
{
    return "@" + symbol;
}

GlobalVariable::GlobalVariable(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& name_, ConstantValue* initializer_, bool once_) :
    Value(sourcePos_, ValueKind::globalVariable, type_), name(name_), initializer(initializer_), once(once_)
{
}

GlobalVariable::GlobalVariable(Type* type_, const std::string& name_) : 
    Value(soul::ast::SourcePos(), ValueKind::globalVariable, type_), name(name_), initializer(nullptr), once(false)
{
}

void GlobalVariable::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string GlobalVariable::ToString() const
{
    return name;
}

void GlobalVariable::Write(util::CodeFormatter& formatter)
{
    Type* type = GetType();
    if (type->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        type = pointerType->BaseType();
    }
    formatter.Write(type->Name());
    formatter.Write(" ");
    formatter.Write(name);
    if (initializer)
    {
        formatter.Write(" = ");
        if (initializer->IsAggregateValue() || initializer->IsStringValue() || initializer->IsStringArrayValue())
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

Data::Data() : context(nullptr), nextStringId(0)
{
}

GlobalVariable* Data::AddGlobalVariable(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& variableName, ConstantValue* initializer, bool once)
{
    try
    {
        if (initializer)
        {
            initializer->SetType(type);
        }
        GlobalVariable* globalVariable = new GlobalVariable(sourcePos, type->AddPointer(context), variableName, initializer, once);
        values.push_back(std::unique_ptr<Value>(globalVariable));
        globalVariables.push_back(globalVariable);
        globalVariableMap[variableName] = globalVariable;
        return globalVariable;
    }
    catch (const std::exception& ex)
    {
        Error("error adding global variable: " + std::string(ex.what()), sourcePos, context);
    }
    return nullptr;
}

GlobalVariable* Data::AddGlobalStringPtr(const std::string& stringValue)
{
    auto it = stringValueMap.find(stringValue);
    if (it != stringValueMap.end())
    {
        return it->second;
    }
    std::string variableName = "string" + std::to_string(nextStringId++) + "_" + context->GetCompileUnit().Id();
    GlobalVariable* globalVariable = new GlobalVariable(soul::ast::SourcePos(), context->GetTypes().GetByteType()->AddPointer(context), variableName,
        context->GetData().MakeStringValue(soul::ast::SourcePos(), stringValue, false), false);
    values.push_back(std::unique_ptr<Value>(globalVariable));
    globalVariables.push_back(globalVariable);
    globalVariableMap[variableName] = globalVariable;
    stringValueMap[stringValue] = globalVariable;
    return globalVariable;
}

GlobalVariable* Data::GetOrInsertGlobal(const std::string& name, Type* type)
{
    auto it = globalVariableMap.find(name);
    if (it != globalVariableMap.cend())
    {
        return it->second;
    }
    context->GetTypes().Map(type);
    GlobalVariable* globalVariable = new GlobalVariable(type->AddPointer(context), name);
    values.push_back(std::unique_ptr<GlobalVariable>(globalVariable));
    globalVariables.push_back(globalVariable);
    globalVariableMap[name] = globalVariable;
    return globalVariable;
}

ConstantValue* Data::GetTrueValue(const Types& types)
{
    if (!trueValue)
    {
        trueValue.reset(new BoolValue(true, types.Get(boolTypeId)));
    }
    return trueValue.get();
}

ConstantValue* Data::GetFalseValue(const Types& types)
{
    if (!falseValue)
    {
        falseValue.reset(new BoolValue(false, types.Get(boolTypeId)));
    }
    return falseValue.get();
}

ConstantValue* Data::GetSByteValue(int8_t value, const Types& types)
{
    return sbyteValueMap.Get(value, this, types);
}

ConstantValue* Data::GetByteValue(uint8_t value, const Types& types)
{
    return byteValueMap.Get(value, this, types);
}

ConstantValue* Data::GetShortValue(int16_t value, const Types& types)
{
    return shortValueMap.Get(value, this, types);
}

ConstantValue* Data::GetUShortValue(uint16_t value, const Types& types)
{
    return ushortValueMap.Get(value, this, types);
}

ConstantValue* Data::GetIntValue(int32_t value, const Types& types)
{
    return intValueMap.Get(value, this, types);
}

ConstantValue* Data::GetUIntValue(uint32_t value, const Types& types)
{
    return uintValueMap.Get(value, this, types);
}

ConstantValue* Data::GetLongValue(int64_t value, const Types& types)
{
    return longValueMap.Get(value, this, types);
}

ConstantValue* Data::GetULongValue(uint64_t value, const Types& types)
{
    return ulongValueMap.Get(value, this, types);
}

ConstantValue* Data::GetFloatValue(float value, const Types& types)
{
    return floatValueMap.Get(value, this, types);
}

ConstantValue* Data::GetDoubleValue(float value, const Types& types)
{
    return doubleValueMap.Get(value, this, types);
}

ConstantValue* Data::GetNullValue(Type* type)
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

ConstantValue* Data::MakeValue(int8_t value, const Types& types)
{
    SByteValue* constantValue = new SByteValue(value, types.Get(sbyteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(uint8_t value, const Types& types)
{
    ByteValue* constantValue = new ByteValue(value, types.Get(byteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(int16_t value, const Types& types)
{
    ShortValue* constantValue = new ShortValue(value, types.Get(shortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(uint16_t value, const Types& types)
{
    UShortValue* constantValue = new UShortValue(value, types.Get(ushortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(int32_t value, const Types& types)
{
    IntValue* constantValue = new IntValue(value, types.Get(intTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(uint32_t value, const Types& types)
{
    UIntValue* constantValue = new UIntValue(value, types.Get(uintTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(int64_t value, const Types& types)
{
    LongValue* constantValue = new LongValue(value, types.Get(longTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(uint64_t value, const Types& types)
{
    ULongValue* constantValue = new ULongValue(value, types.Get(ulongTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(float value, const Types& types)
{
    FloatValue* constantValue = new FloatValue(value, types.Get(floatTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeValue(double value, const Types& types)
{
    DoubleValue* constantValue = new DoubleValue(value, types.Get(doubleTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

ConstantValue* Data::MakeArrayValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& elements, const std::string& prefix)
{
    ArrayValue* arrayValue = new ArrayValue(sourcePos, elements, prefix);
    values.push_back(std::unique_ptr<Value>(arrayValue));
    return arrayValue;
}

ConstantValue* Data::MakeStructureValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& fieldValues)
{
    StructureValue* structureValue = new StructureValue(sourcePos, fieldValues);
    values.push_back(std::unique_ptr<Value>(structureValue));
    return structureValue;
}

ConstantValue* Data::MakeStringValue(const soul::ast::SourcePos& sourcePos, const std::string& value, bool crop)
{
    std::string val = value;
    if (crop)
    {
        val = value.substr(1, value.length() - 2);
    }
    std::string s;
    std::string hex;
    int state = 0;
    for (char c : val)
    {
        switch (state)
        {
            case 0:
            {
                if (c == '\\')
                {
                    hex.clear();
                    state = 1;
                }
                else
                {
                    s.append(1, c);
                }
                break;
            }
            case 1:
            {
                hex.append(1, c);
                state = 2;
                break;
            }
            case 2:
            {
                hex.append(1, c);
                uint8_t x = util::ParseHexByte(hex);
                char ch = static_cast<char>(x);
                s.append(1, ch);
                state = 0;
                break;
            }
        }
    }
    StringValue* stringValue = new StringValue(sourcePos, s);
    values.push_back(std::unique_ptr<Value>(stringValue));
    return stringValue;
}

ConstantValue* Data::MakeStringArrayValue(const soul::ast::SourcePos& sourcePos, char prefix, const std::vector<ConstantValue*>& strings)
{
    StringArrayValue* stringArrayValue = new StringArrayValue(sourcePos, prefix, strings);
    values.push_back(std::unique_ptr<Value>(stringArrayValue));
    return stringArrayValue;
}

ConstantValue* Data::MakeConversionValue(const soul::ast::SourcePos& sourcePos, Type* type, ConstantValue* from)
{
    ConversionValue* conversionValue = new ConversionValue(sourcePos, type, from);
    values.push_back(std::unique_ptr<Value>(conversionValue));
    return conversionValue;
}

ConstantValue* Data::MakeClsIdValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& clsIdStr, bool crop)
{
    std::string typeId = clsIdStr;
    if (crop)
    {
        typeId = clsIdStr.substr(6, clsIdStr.length() - 6 - 1);
    }
    ClsIdValue* clsIdValue = new ClsIdValue(sourcePos, type, typeId);
    values.push_back(std::unique_ptr<Value>(clsIdValue));
    return clsIdValue;
}

ConstantValue* Data::MakeSymbolValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& symbol)
{
    if (type->IsFunctionType())
    {
        type = type->AddPointer(context);
    }
    SymbolValue* symbolValue = new SymbolValue(sourcePos, type, symbol);
    values.push_back(std::unique_ptr<Value>(symbolValue));
    return symbolValue;
}

ConstantValue* Data::MakeNumericLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& strValue, const Types& types, Context* context)
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
                Error("error making literal: Boolean value expected", sourcePos, context);
            }
        }
        case sbyteTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<int8_t>::min() || value > std::numeric_limits<int8_t>::max())
            {
                Error("error making literal: range error: sbyte value expected", sourcePos, context);
            }
            return GetSByteValue(static_cast<int8_t>(value), types);
        }
        case byteTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<uint8_t>::min() || value > std::numeric_limits<uint8_t>::max())
            {
                Error("error making literal: range error: byte value expected", sourcePos, context);
            }
            return GetByteValue(static_cast<uint8_t>(value), types);
        }
        case shortTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<int16_t>::min() || value > std::numeric_limits<int16_t>::max())
            {
                Error("error making literal: range error: short value expected", sourcePos, context);
            }
            return GetShortValue(static_cast<int16_t>(value), types);
        }
        case ushortTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<uint16_t>::min() || value > std::numeric_limits<uint16_t>::max())
            {
                Error("error making literal: range error: ushort value expected", sourcePos, context);
            }
            return GetUShortValue(static_cast<uint16_t>(value), types);
        }
        case intTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<int32_t>::min() || value > std::numeric_limits<int32_t>::max())
            {
                Error("error making literal: range error: int value expected", sourcePos, context);
            }
            return GetIntValue(static_cast<int32_t>(value), types);
        }
        case uintTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<uint32_t>::min() || value > std::numeric_limits<uint32_t>::max())
            {
                Error("error making literal: range error: uint value expected", sourcePos, context);
            }
            return GetUIntValue(static_cast<uint32_t>(value), types);
        }
        case longTypeId:
        {
            int64_t value = std::stoll(strValue);
            if (value < std::numeric_limits<int64_t>::min() || value > std::numeric_limits<int64_t>::max())
            {
                Error("error making literal: range error: long value expected", sourcePos, context);
            }
            return GetLongValue(static_cast<int64_t>(value), types);
        }
        case ulongTypeId:
        {
            uint64_t value = std::stoull(strValue);
            if (value < std::numeric_limits<uint64_t>::min() || value > std::numeric_limits<uint64_t>::max())
            {
                Error("error making literal: range error: ulong value expected", sourcePos, context);
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
            Error("error making literal: invalid numeric value", sourcePos, context);
        }
    }
    return nullptr;
}

ConstantValue* Data::MakeAddressLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& id, Context* context, bool resolve)
{
    AddressValue* addressValue = new AddressValue(sourcePos, id, type);
    values.push_back(std::unique_ptr<Value>(addressValue));
    addressValues.push_back(addressValue);
    if (resolve)
    {
        ResolveAddressValue(addressValue);
    }
    return addressValue;
}

void Data::ResolveAddressValue(AddressValue* addressValue)
{
    auto it = globalVariableMap.find(addressValue->Id());
    if (it != globalVariableMap.end())
    {
        GlobalVariable* globalVariable = it->second;
        addressValue->SetValue(globalVariable);
    }
    else
    {
        Error("error resolving address value '" + addressValue->Id() + "': no such global variable found", addressValue->GetSourcePos(), context);
    }
}

void Data::Resolve()
{
    for (AddressValue* addressValue : addressValues)
    {
        ResolveAddressValue(addressValue);
    }
    addressValues.clear();
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
