// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.data;

import cmajor.systemx.intermediate.value;
import cmajor.systemx.intermediate.types;
import soul.ast.source.pos;
import util;
import std.core;

export namespace cmajor::systemx::intermediate {

class Data;
class Function;
class GlobalVariable;

class AddressValue : public ConstantValue
{
public:
    AddressValue(const soul::ast::SourcePos& sourcePos_, GlobalVariable* globalVariable_, Type* type);
    AddressValue(const soul::ast::SourcePos& sourcePos_, const std::string& id_, Type* type);
    const std::string& Id() const { return id; }
    void SetValue(GlobalVariable* globalVariable_) { globalVariable = globalVariable_; }
    GlobalVariable* Value() const { return globalVariable; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string id;
    GlobalVariable* globalVariable;
};

class ArrayValue : public ConstantValue
{
public:
    ArrayValue(const soul::ast::SourcePos& sourcePos_, const std::vector<ConstantValue*>& elements_, const std::string& prefix_);
    const std::vector<ConstantValue*>& Elements() const { return elements; }
    void SetType(Type* type) override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<ConstantValue*> elements;
    std::string prefix;
};

class StructureValue : public ConstantValue
{
public:
    StructureValue(const soul::ast::SourcePos& sourcePos_, const std::vector<ConstantValue*>& fieldValues_);
    const std::vector<ConstantValue*>& FieldValues() const { return fieldValues; }
    void SetType(Type* type) override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<ConstantValue*> fieldValues;
};

class StringValue : public ConstantValue
{
public:
    StringValue(const soul::ast::SourcePos& sourcePos_, const std::string& value_);
    const std::string& Value() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string value;
};

class StringArrayValue : public ConstantValue
{
public:
    StringArrayValue(const soul::ast::SourcePos& sourcePos_, char prefix_, const std::vector<ConstantValue*>& elements_);
    char Prefix() const { return prefix; }
    const std::vector<ConstantValue*>& Elements() const { return elements; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    char prefix;
    std::vector<ConstantValue*> elements;
};

class ConversionValue : public ConstantValue
{
public:
    ConversionValue(const soul::ast::SourcePos& sourcePos_, Type* type_, ConstantValue* from_);
    ConstantValue* From() const { return from; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    ConstantValue* from;
};

class ClsIdValue : public ConstantValue
{
public:
    ClsIdValue(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& typeId_);
    const std::string& TypeId() const { return typeId; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string typeId;
};

class SymbolValue : public ConstantValue
{
public:
    SymbolValue(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& symbol_);
    const std::string& Symbol() const { return symbol; }
    Function* GetFunction() const { return function; }
    void SetFunction(Function* function_) { function = function_; }
    GlobalVariable* GetGlobalVariable() const { return globalVariable; }
    void SetGlobalVariable(GlobalVariable* globalVariable_) { globalVariable = globalVariable_; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string symbol;
    Function* function;
    GlobalVariable* globalVariable;
};

class GlobalVariable : public Value
{
public:
    GlobalVariable(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& name_, ConstantValue* initializer_, bool once_);
    GlobalVariable(Type* type_, const std::string& name_);
    virtual void Accept(Visitor& visitor);
    const std::string& Name() const { return name; }
    ConstantValue* Initializer() const { return initializer; }
    void SetInitializer(ConstantValue* initializer_) { initializer = initializer_; }
    bool Once() const { return once; }
    void SetOnce() { once = true; }
    std::string ToString() const override;
    void Write(util::CodeFormatter& formatter);
private:
    std::string name;
    ConstantValue* initializer;
    bool once;
};

template<class T>
class ConstantValueMap
{
public:
    ConstantValueMap();
    ConstantValue* Get(const T& value, Data* data, const Types& types);
private:
    std::map<T, ConstantValue*> valueMap;
};

class Data
{
public:
    Data();
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    GlobalVariable* AddGlobalVariable(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& variableName, ConstantValue* initializer, bool once);
    GlobalVariable* AddGlobalStringPtr(const std::string& stringValue);
    GlobalVariable* GetOrInsertGlobal(const std::string& name, Type* type);
    ConstantValue* GetTrueValue(const Types& types);
    ConstantValue* GetFalseValue(const Types& types);
    ConstantValue* GetSByteValue(int8_t value, const Types& types);
    ConstantValue* GetByteValue(uint8_t value, const Types& types);
    ConstantValue* GetShortValue(int16_t value, const Types& types);
    ConstantValue* GetUShortValue(uint16_t value, const Types& types);
    ConstantValue* GetIntValue(int32_t value, const Types& types);
    ConstantValue* GetUIntValue(uint32_t value, const Types& types);
    ConstantValue* GetLongValue(int64_t value, const Types& types);
    ConstantValue* GetULongValue(uint64_t value, const Types& types);
    ConstantValue* GetFloatValue(float value, const Types& types);
    ConstantValue* GetDoubleValue(float value, const Types& types);
    ConstantValue* GetCharValue(char value, const Types& types);
    ConstantValue* GetWCharValue(char16_t value, const Types& types);
    ConstantValue* GetUCharValue(char32_t value, const Types& types);
    ConstantValue* GetNullValue(Type* type);
    ConstantValue* MakeValue(int8_t value, const Types& types);
    ConstantValue* MakeValue(uint8_t value, const Types& types);
    ConstantValue* MakeValue(int16_t value, const Types& types);
    ConstantValue* MakeValue(uint16_t value, const Types& types);
    ConstantValue* MakeValue(int32_t value, const Types& types);
    ConstantValue* MakeValue(uint32_t value, const Types& types);
    ConstantValue* MakeValue(int64_t value, const Types& types);
    ConstantValue* MakeValue(uint64_t value, const Types& types);
    ConstantValue* MakeValue(float value, const Types& types);
    ConstantValue* MakeValue(double value, const Types& types);
    ConstantValue* MakeValue(char value, const Types& types);
    ConstantValue* MakeValue(char16_t value, const Types& types);
    ConstantValue* MakeValue(char32_t value, const Types& types);
    ConstantValue* MakeArrayValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& elements, const std::string& prefix);
    ConstantValue* MakeStructureValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& fieldValues);
    ConstantValue* MakeStringValue(const soul::ast::SourcePos& sourcePos, const std::string& value, bool crop);
    ConstantValue* MakeStringArrayValue(const soul::ast::SourcePos& sourcePos, char prefix, const std::vector<ConstantValue*>& strings);
    ConstantValue* MakeConversionValue(const soul::ast::SourcePos& sourcePos, Type* type, ConstantValue* from);
    ConstantValue* MakeClsIdValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& clsIdStr, bool crop);
    ConstantValue* MakeSymbolValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& symbol);
    ConstantValue* MakeNumericLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& strValue, const Types& types, Context* context);
    ConstantValue* MakeAddressLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& id, Context* context, bool resolve);
    void VisitGlobalVariables(Visitor& visitor);
    void ResolveAddressValue(AddressValue* addressValue);
    void Resolve();
    void Write(util::CodeFormatter& formatter);
private:
    Context* context;
    std::vector<std::unique_ptr<Value>> values;
    std::vector<GlobalVariable*> globalVariables;
    std::vector<AddressValue*> addressValues;
    std::map<std::string, GlobalVariable*> globalVariableMap;
    std::map<std::string, GlobalVariable*> stringValueMap;
    std::unique_ptr<BoolValue> trueValue;
    std::unique_ptr<BoolValue> falseValue;
    ConstantValueMap<int8_t> sbyteValueMap;
    ConstantValueMap<uint8_t> byteValueMap;
    ConstantValueMap<int16_t> shortValueMap;
    ConstantValueMap<uint16_t> ushortValueMap;
    ConstantValueMap<int32_t> intValueMap;
    ConstantValueMap<uint32_t> uintValueMap;
    ConstantValueMap<int64_t> longValueMap;
    ConstantValueMap<uint64_t> ulongValueMap;
    ConstantValueMap<float> floatValueMap;
    ConstantValueMap<double> doubleValueMap;
    ConstantValueMap<char> charValueMap;
    ConstantValueMap<char16_t> wcharValueMap;
    ConstantValueMap<char32_t> ucharValueMap;
    std::map<Type*, NullValue*> nullValueMap;
    int32_t nextStringId;
};

template<class T>
ConstantValueMap<T>::ConstantValueMap()
{
}

template<class T>
ConstantValue* ConstantValueMap<T>::Get(const T& value, Data* data, const Types& types)
{
    auto it = valueMap.find(value);
    if (it != valueMap.cend())
    {
        return it->second;
    }
    else
    {
        ConstantValue* constantValue = data->MakeValue(value, types);
        valueMap[value] = constantValue;
        return constantValue;
    }
}

} // cmajor::systemx::intermediate
