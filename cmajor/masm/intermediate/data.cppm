// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.data;

import cmajor.masm.assembly;
import cmajor.masm.intermediate.types;
import soul.ast.span;
import util;
import std.core;

export namespace cmajor::masm::intermediate {

class Data;
class Function;
class BasicBlock;
class Instruction;
class RegValue;
class GlobalVariable;

class CloneContext
{
public:
    CloneContext();
    void SetContext(Context* context_) { context = context_; }
    Context* GetContext() const { return context; }
    void SetCurrentFunction(Function* currentFunction_) { currentFunction = currentFunction_; }
    Function* CurrentFunction() const { return currentFunction; }
    void MapInstruction(Instruction* inst, Instruction* clone);
    Instruction* GetMappedInstruction(Instruction* inst) const;
    void AddUnmappedInstruction(Instruction* inst, RegValue* regValue);
    const std::set<std::pair<Instruction*, RegValue*>>& UnmappedInstructions() const { return unmappedInstructions; }
    BasicBlock* GetMappedBasicBlock(BasicBlock* bb) const;
    void MapBasicBlock(BasicBlock* bb, BasicBlock* clone);
private:
    Context* context;
    Function* currentFunction;
    std::map<Instruction*, Instruction*> instMap;
    std::set<std::pair<Instruction*, RegValue*>> unmappedInstructions;
    std::map<BasicBlock*, BasicBlock*> bbMap;
};

enum class ValueKind
{
    boolValue, sbyteValue, byteValue, shortValue, ushortValue, intValue, uintValue, longValue, ulongValue, floatValue, doubleValue, nullValue, addressValue,
    arrayValue, structureValue, stringValue, stringArrayValue, conversionValue, clsIdValue, symbolValue, globalVariable, regValue, instruction
};

class Value
{
public:
    Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_);
    virtual ~Value();
    virtual Value* Clone(CloneContext& cloneContext) const { return const_cast<Value*>(this); }
    virtual void Accept(Visitor& visitor);
    bool IsRegValue() const { return kind == ValueKind::regValue; }
    bool IsInstruction() const { return kind == ValueKind::instruction; }
    bool IsSymbolValue() const { return kind == ValueKind::symbolValue; }
    bool IsAddressValue() const { return kind == ValueKind::addressValue; }
    bool IsGlobalVariable() const { return kind == ValueKind::globalVariable; }
    bool IsIntegerValue() const;
    bool IsFloatingPointValue() const;
    bool IsFloatValue() const { return kind == ValueKind::floatValue; }
    bool IsDoubleValue() const { return kind == ValueKind::doubleValue; }
    bool IsArrayValue() const { return kind == ValueKind::arrayValue; }
    bool IsStructureValue() const { return kind == ValueKind::structureValue; }
    bool IsAggregateValue() const { return IsArrayValue() || IsStructureValue(); }
    bool IsStringValue() const { return kind == ValueKind::stringValue; }
    int64_t GetIntegerValue() const;
    const soul::ast::Span& Span() const { return span; }
    ValueKind Kind() const { return kind; }
    std::string KindStr() const;
    Type* GetType() const { return type; }
    virtual void SetType(Type* type_);
    virtual std::string ToString() const { return std::string(); }
private:
    soul::ast::Span span;
    ValueKind kind;
    Type* type;
};

class BoolValue : public Value
{
public:
    BoolValue(bool value_, Type* type_);
    bool GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    int64_t ToInteger() const { return value ? 1 : 0; }
    std::string ToString() const override;
private:
    bool value;
};

class SByteValue : public Value
{
public:
    SByteValue(int8_t value_, Type* type_);
    int8_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int8_t value;
};

class ByteValue : public Value
{
public:
    ByteValue(uint8_t value_, Type* type_);
    uint8_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint8_t value;
};

class ShortValue : public Value
{
public:
    ShortValue(int16_t value_, Type* type_);
    int16_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int16_t value;
};

class UShortValue : public Value
{
public:
    UShortValue(uint16_t value_, Type* type_);
    uint16_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint16_t value;
};

class IntValue : public Value
{
public:
    IntValue(int32_t value_, Type* type_);
    int32_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int32_t value;
};

class UIntValue : public Value
{
public:
    UIntValue(uint32_t value_, Type* type_);
    uint32_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint32_t value;
};

class LongValue : public Value
{
public:
    LongValue(int64_t value_, Type* type_);
    int64_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int64_t value;
};

class ULongValue : public Value
{
public:
    ULongValue(uint64_t value_, Type* type_);
    uint64_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint64_t value;
};

class FloatValue : public Value
{
public:
    FloatValue(float value_, Type* type_);
    float GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    float value;
};

class DoubleValue : public Value
{
public:
    DoubleValue(double value_, Type* type_);
    double GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    double value;
};

class NullValue : public Value
{
public:
    NullValue(Type* type_);
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class AddressValue : public Value
{
public:
    AddressValue(const soul::ast::Span& span_, GlobalVariable* globalVariable_, Type* type);
    GlobalVariable* GetValue() const { return globalVariable; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    GlobalVariable* globalVariable;
};

class ArrayValue : public Value
{
public:
    ArrayValue(const soul::ast::Span& span_, const std::vector<Value*>& elements_);
    const std::vector<Value*>& Elements() const { return elements; }
    void SetType(Type* type) override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<Value*> elements;
};

class StructureValue : public Value
{
public:
    StructureValue(const soul::ast::Span& span_, const std::vector<Value*>& fieldValues_);
    const std::vector<Value*>& FieldValues() const { return fieldValues; }
    void SetType(Type* type) override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<Value*> fieldValues;
};

class StringValue : public Value
{
public:
    StringValue(const soul::ast::Span& span_, const std::string& value_);
    const std::string& GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string value;
};

class StringArrayValue : public Value
{
public:
    StringArrayValue(const soul::ast::Span& span_, char prefix_, const std::vector<Value*>& strings_);
    char Prefix() const { return prefix; }
    const std::vector<Value*>& Strings() const { return strings; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    char prefix;
    std::vector<Value*> strings;
};

class ConversionValue : public Value
{
public:
    ConversionValue(const soul::ast::Span& span_, Type* type_, Value* from_);
    Value* From() const { return from; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    Value* from;
};

class ClsIdValue : public Value
{
public:
    ClsIdValue(const soul::ast::Span& span_, Type* type_, const std::string& typeId_);
    const std::string& TypeId() const { return typeId; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string typeId;
};

class SymbolValue : public Value
{
public:
    SymbolValue(const soul::ast::Span& span_, Type* type_, const std::string& symbol_);
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
    GlobalVariable(const soul::ast::Span& span_, Type* type_, const std::string& name_, Value* initializer_);
    virtual void Accept(Visitor& visitor);
    const std::string& Name() const { return name; }
    Value* Initializer() const { return initializer; }
    void Write(util::CodeFormatter& formatter);
    std::string ToString() const override;
private:
    std::string name;
    Value* initializer;
};

template<class T>
class ValueMap
{
public:
    ValueMap();
    Value* Get(const T& value, Data* data, const Types& types);
private:
    std::map<T, Value*> valueMap;
};

class Data
{
public:
    Data();
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    void AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer, Context* context);
    Value* GetTrueValue(const Types& types);
    Value* GetFalseValue(const Types& types);
    Value* GetSByteValue(int8_t value, const Types& types);
    Value* GetByteValue(uint8_t value, const Types& types);
    Value* GetShortValue(int16_t value, const Types& types);
    Value* GetUShortValue(uint16_t value, const Types& types);
    Value* GetIntValue(int32_t value, const Types& types);
    Value* GetUIntValue(uint32_t value, const Types& types);
    Value* GetLongValue(int64_t value, const Types& types);
    Value* GetULongValue(uint64_t value, const Types& types);
    Value* GetFloatValue(float value, const Types& types);
    Value* GetDoubleValue(double value, const Types& types);
    Value* GetNullValue(Type* type);
    Value* MakeValue(int8_t value, const Types& types);
    Value* MakeValue(uint8_t value, const Types& types);
    Value* MakeValue(int16_t value, const Types& types);
    Value* MakeValue(uint16_t value, const Types& types);
    Value* MakeValue(int32_t value, const Types& types);
    Value* MakeValue(uint32_t value, const Types& types);
    Value* MakeValue(int64_t value, const Types& types);
    Value* MakeValue(uint64_t value, const Types& types);
    Value* MakeValue(float value, const Types& types);
    Value* MakeValue(double value, const Types& types);
    Value* MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements);
    Value* MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues);
    Value* MakeStringValue(const soul::ast::Span& span, const std::string& value);
    Value* MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& strings);
    Value* MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from);
    Value* MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr);
    Value* MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol);
    Value* MakeIntegerLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue, const Types& types, Context* context);
    Value* MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id, Context* context);
    void VisitGlobalVariables(Visitor& visitor);
    void Write(util::CodeFormatter& formatter);
private:
    Context* context;
    std::vector<std::unique_ptr<Value>> values;
    std::vector<GlobalVariable*> globalVariables;
    std::map<std::string, GlobalVariable*> globalVariableMap;
    std::unique_ptr<BoolValue> trueValue;
    std::unique_ptr<BoolValue> falseValue;
    ValueMap<int8_t> sbyteValueMap;
    ValueMap<uint8_t> byteValueMap;
    ValueMap<int16_t> shortValueMap;
    ValueMap<uint16_t> ushortValueMap;
    ValueMap<int32_t> intValueMap;
    ValueMap<uint32_t> uintValueMap;
    ValueMap<int64_t> longValueMap;
    ValueMap<uint64_t> ulongValueMap;
    ValueMap<float> floatValueMap;
    ValueMap<double> doubleValueMap;
    std::map<Type*, NullValue*> nullValueMap;
};

template<class T>
ValueMap<T>::ValueMap()
{
}

template<class T>
Value* ValueMap<T>::Get(const T& value, Data* data, const Types& types)
{
    auto it = valueMap.find(value);
    if (it != valueMap.cend())
    {
        return it->second;
    }
    else
    {
        Value* constantValue = data->MakeValue(value, types);
        valueMap[value] = constantValue;
        return constantValue;
    }
}

} // cmajor::masm::intermediate
