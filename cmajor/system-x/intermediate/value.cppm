// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.value;

import soul.ast.source.pos;
import std.core;

export namespace cmajor::systemx::intermediate {

enum class ValueKind
{
    boolValue, sbyteValue, byteValue, shortValue, ushortValue, intValue, uintValue, longValue, ulongValue, floatValue, doubleValue, nullValue, addressValue,
    arrayValue, structureValue, stringValue, stringArrayValue, conversionValue, clsIdValue, symbolValue,
    globalVariable,
    regValue,
    instruction, 
    function
};

class Type;
class Visitor;

class Value
{
public:
    Value(const soul::ast::SourcePos& sourcePos_, ValueKind kind_, Type* type_);
    virtual ~Value();
    bool IsRegValue() const { return kind == ValueKind::regValue; }
    bool IsInstruction() const { return kind == ValueKind::instruction; }
    bool IsSymbolValue() const { return kind == ValueKind::symbolValue; }
    bool IsAddressValue() const { return kind == ValueKind::addressValue; }
    bool IsGlobalVariable() const { return kind == ValueKind::globalVariable; }
    bool IsArrayValue() const { return kind == ValueKind::arrayValue; }
    bool IsStructureValue() const { return kind == ValueKind::structureValue; }
    bool IsAggregateValue() const { return IsArrayValue() || IsStructureValue(); }
    bool IsStringValue() const { return kind == ValueKind::stringValue; }
    bool IsStringArrayValue() const { return kind == ValueKind::stringArrayValue; }
    bool IsLongValue() const { return kind == ValueKind::longValue; }
    bool IsFunction() const { return kind == ValueKind::function; }
    bool IsIntegerValue() const;
    int64_t GetIntegerValue() const;
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    ValueKind Kind() const { return kind; }
    std::string KindStr() const;
    Type* GetType() const { return type; }
    virtual void SetType(Type* type_);
    virtual std::string ToString() const { return std::string(); }
private:
    soul::ast::SourcePos sourcePos;
    ValueKind kind;
    Type* type;
};

class ConstantValue : public Value
{
public:
    ConstantValue(const soul::ast::SourcePos& sourcePos_, ValueKind kind_, Type* type_);
    virtual void Accept(Visitor& visitor) = 0;
};

class BoolValue : public ConstantValue
{
public:
    BoolValue(bool value_, Type* type_);
    bool GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    bool value;
};

class SByteValue : public ConstantValue
{
public:
    SByteValue(int8_t value_, Type* type_);
    int8_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int8_t value;
};

class ByteValue : public ConstantValue
{
public:
    ByteValue(uint8_t value_, Type* type_);
    uint8_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint8_t value;
};

class ShortValue : public ConstantValue
{
public:
    ShortValue(int16_t value_, Type* type_);
    int16_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int16_t value;
};

class UShortValue : public ConstantValue
{
public:
    UShortValue(uint16_t value_, Type* type_);
    uint16_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint16_t value;
};

class IntValue : public ConstantValue
{
public:
    IntValue(int32_t value_, Type* type_);
    int32_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int32_t value;
};

class UIntValue : public ConstantValue
{
public:
    UIntValue(uint32_t value_, Type* type_);
    uint32_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint32_t value;
};

class LongValue : public ConstantValue
{
public:
    LongValue(int64_t value_, Type* type_);
    int64_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    int64_t value;
};

class ULongValue : public ConstantValue
{
public:
    ULongValue(uint64_t value_, Type* type_);
    uint64_t GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    uint64_t value;
};

class FloatValue : public ConstantValue
{
public:
    FloatValue(float value_, Type* type_);
    float GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    float value;
};

class DoubleValue : public ConstantValue
{
public:
    DoubleValue(double value_, Type* type_);
    double GetValue() const { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    double value;
};

class NullValue : public ConstantValue
{
public:
    NullValue(Type* type_);
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

} // cmajor::systemx::intermediate
