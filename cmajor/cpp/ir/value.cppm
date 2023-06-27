// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifndef __cpp_char8_t
using char8_t = unsigned char;
#endif

export module cmajor.cpp.ir.value;

import std.core;

export namespace cmajor::cpp::ir {

class Type;
class Context;
class PtrType;
class GlobalVariable;

class Value
{
public:
    Value();
    virtual ~Value();
    virtual Type* GetType(Context& context) = 0;
    virtual std::string Name(Context& context) = 0;
    virtual bool IsLongValue() const { return false; }
    virtual bool IsNullValue() const { return false; }
    virtual bool IsAggregateValue() const { return false; }
    virtual bool IsStringValue() const { return false; }
    virtual bool IsWStringValue() const { return false; }
    virtual bool IsUStringValue() const { return false; }
    virtual bool IsLocal() const { return false; }
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

class CharValue : public ConstantValue
{
public:
    CharValue();
    CharValue(char value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    char value;
};

class WCharValue : public ConstantValue
{
public:
    WCharValue();
    WCharValue(char16_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    char16_t value;
};

class UCharValue : public ConstantValue
{
public:
    UCharValue();
    UCharValue(char32_t value_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
private:
    char32_t value;
};

class NullValue : public ConstantValue
{
public:
    NullValue(PtrType* ptrType_);
    Type* GetType(Context& context) override;
    std::string Name(Context& context) override;
    bool IsNullValue() const override { return true; }
private:
    PtrType* ptrType;
};

class ArrayValue : public ConstantValue
{
public:
    ArrayValue(Type* type_, const std::vector<Value*>& elements_, const std::string& prefix_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override { return type; }
    void AddElement(ConstantValue* element);
    bool IsAggregateValue() const override { return true; }
    void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context) override;
private:
    Type* type;
    std::vector<Value*> elements;
    std::string prefix;
};

class StructureValue : public ConstantValue
{
public:
    StructureValue(Type* type_, const std::vector<Value*>& members_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void AddMember(ConstantValue* member);
    bool IsAggregateValue() const override { return true; }
    void AddDependencies(GlobalVariable* variable, const std::unordered_map<std::string, GlobalVariable*>& nameMap, std::unordered_map<GlobalVariable*, std::set<GlobalVariable*>>& dependencies,
        Context& context) override;
private:
    Type* type;
    std::vector<Value*> members;
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

class WStringValue : public ConstantValue
{
public:
    WStringValue(Type* type_, const std::u16string& value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    bool IsWStringValue() const override { return true; }
private:
    Type* type;
    std::u16string value;
};

class UStringValue : public ConstantValue
{
public:
    UStringValue(Type* type_, const std::u32string& value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    bool IsUStringValue() const override { return true; }
private:
    Type* type;
    std::u32string value;
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

} // namespace cmajor::cpp::ir
