// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.value;

import std.core;

export namespace cmajor::masm::ir {

class Type;
class PtrType;
class Context;
class GlobalVariable;

class Value
{
public:
    Value();
    virtual ~Value();
    virtual std::string Name(Context& context) = 0;
    virtual Type* GetType(Context& context) = 0;
    virtual bool IsAggregateValue() const { return false; }
    virtual bool IsStringValue() const { return false; }
    virtual bool IsLongValue() const { return false; }
};

class BoolValue: public Value
{
public:
    BoolValue();
    BoolValue(bool value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    bool value;
};

class SByteValue : public Value
{
public:
    SByteValue();
    SByteValue(int8_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    int8_t value;
};

class ByteValue : public Value
{
public:
    ByteValue();
    ByteValue(uint8_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    uint8_t value;
};

class ShortValue : public Value
{
public:
    ShortValue();
    ShortValue(int16_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    int16_t value;
};

class UShortValue : public Value
{
public:
    UShortValue();
    UShortValue(uint16_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    uint16_t value;
};

class IntValue : public Value
{
public:
    IntValue();
    IntValue(int32_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    int32_t value;
};

class UIntValue : public Value
{
public:
    UIntValue();
    UIntValue(uint32_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    uint32_t value;
};

class LongValue : public Value
{
public:
    LongValue();
    LongValue(int64_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    bool IsLongValue() const override { return true; }
    int64_t GetValue() const { return value; }
private:
    int64_t value;
};

class ULongValue : public Value
{
public:
    ULongValue();
    ULongValue(uint64_t value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    uint64_t value;
};

class FloatValue : public Value
{
public:
    FloatValue();
    FloatValue(float value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    float value;
};

class DoubleValue : public Value
{
public:
    DoubleValue();
    DoubleValue(double value_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    double value;
};

class NullValue : public Value
{
public:
    NullValue(PtrType* ptrType_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    PtrType* ptrType;
};

class ArrayValue : public Value
{
public:
    ArrayValue(Type* type_, const std::vector<Value*>& elements_, const std::string& prefix_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override { return type; }
    void AddElement(Value* element);
    bool IsAggregateValue() const override { return true; }
private:
    Type* type;
    std::vector<Value*> elements;
    std::string prefix;
};

class StructureValue : public Value
{
public:
    StructureValue(Type* type_, const std::vector<Value*>& members_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void AddMember(Value* member);
    bool IsAggregateValue() const override { return true; }
private:
    Type* type;
    std::vector<Value*> members;
};

class StringValue : public Value
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

class ConversionValue : public Value
{
public:
    ConversionValue(Type* type_, Value* from_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    Type* type;
    Value* from;
};

class ClsIdValue : public Value
{
public:
    ClsIdValue(const std::string& typeId_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
private:
    std::string typeId;
};

} // namespace cmajor::masm::ir

