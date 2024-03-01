// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.value;

import std.core;

export namespace cmajor::systemx::ir {

class Type;
class PtrType;
class Context;
class GlobalVariable;

class Value
{
public:
    Value();
    virtual ~Value();
    virtual Type* GetType(Context& context) = 0;
    virtual std::string Name(Context& context) = 0;
    virtual bool IsLongValue() const { return false; }
    virtual bool IsAggregateValue() const { return false; }
    virtual bool IsStringValue() const { return false; }
    virtual void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context);
};

class ConstantValue : public Value
{
public:
    std::string Name(Context& context) override { return "constant"; }
};

class BoolValue : public ConstantValue
{
public:
    BoolValue();
    BoolValue(bool value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    bool value;
};

class SByteValue : public ConstantValue
{
public:
    SByteValue();
    SByteValue(int8_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    int8_t value;
};

class ByteValue : public ConstantValue
{
public:
    ByteValue();
    ByteValue(uint8_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    uint8_t value;
};

class ShortValue : public ConstantValue
{
public:
    ShortValue();
    ShortValue(int16_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    int16_t value;
};

class UShortValue : public ConstantValue
{
public:
    UShortValue();
    UShortValue(uint16_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    uint16_t value;
};

class IntValue : public ConstantValue
{
public:
    IntValue();
    IntValue(int32_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    int32_t value;
};

class UIntValue : public ConstantValue
{
public:
    UIntValue();
    UIntValue(uint32_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    uint32_t value;
};

class LongValue : public ConstantValue
{
public:
    LongValue();
    LongValue(int64_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
    bool IsLongValue() const override { return true; }
    int64_t GetValue() const { return value; }
private:
    int64_t value;
};

class ULongValue : public ConstantValue
{
public:
    ULongValue();
    ULongValue(uint64_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    uint64_t value;
};

class FloatValue : public ConstantValue
{
public:
    FloatValue();
    FloatValue(float value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    float value;
};

class DoubleValue : public ConstantValue
{
public:
    DoubleValue();
    DoubleValue(double value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    double value;
};

class NullValue : public ConstantValue
{
public:
    NullValue(PtrType* ptrType_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    PtrType* ptrType;
};

class ArrayValue : public ConstantValue
{
public:
    ArrayValue(Type* type_, const std::vector<ConstantValue*>& elements_, const std::string& prefix_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override { return type; }
    void AddElement(ConstantValue* element);
    bool IsAggregateValue() const override { return true; }
    void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context) override;
private:
    Type* type;
    std::vector<ConstantValue*> elements;
    std::string prefix;
};

class StructureValue : public ConstantValue
{
public:
    StructureValue(Type* type_, const std::vector<ConstantValue*>& members_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void AddMember(ConstantValue* member);
    bool IsAggregateValue() const override { return true; }
    void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context) override;
private:
    Type* type;
    std::vector<ConstantValue*> members;
};

class StringValue : public ConstantValue
{
public:
    StringValue(Type* type_, const std::string& value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    bool IsStringValue() const override { return true; }
private:
    Type* type;
    std::string value;
};

class ConversionValue : public ConstantValue
{
public:
    ConversionValue(Type* type_, ConstantValue* from_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context) override;
private:
    Type* type;
    ConstantValue* from;
};

class ClsIdValue : public ConstantValue
{
public:
    ClsIdValue(const std::string& typeId_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    std::string typeId;
};

} // namespace cmajor::systemx::ir
