// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.value;

import cmajor.ast;
import soul.ast.span;
import cmajor.ir.emitter;
import cmajor.symbols.type.symbol;
import util;
import soul.xml.element;
import soul.ast.span;
import std.core;

export namespace cmajor::symbols {

class Context;
class SymbolTable;

enum class ValueType : uint8_t
{
    none, boolValue, sbyteValue, byteValue, shortValue, ushortValue, intValue, uintValue,
    longValue, ulongValue, floatValue, doubleValue, charValue, wcharValue, ucharValue,
    stringValue, wstringValue, ustringValue, nullValue, pointerValue, arrayValue, structuredValue,
    uuidValue,
    maxValue
};

std::string ValueTypeStr(ValueType valueType);

ValueType CommonType(ValueType left, ValueType right);

TypeSymbol* GetTypeFor(ValueType valueType, SymbolTable* symbolTable);

class Value
{
public:
    Value(const soul::ast::Span& span_, ValueType valueType_);
    virtual ~Value();
    virtual Value* Clone() const = 0;
    virtual Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const = 0;
    virtual void* IrValue(cmajor::ir::Emitter& emitter, Context* context) = 0;
    virtual void Write(util::BinaryStreamWriter& writer) = 0;
    virtual void Read(util::BinaryStreamReader& reader) = 0;
    virtual bool IsComplete() const { return true; }
    virtual bool IsScopedValue() const { return false; }
    virtual bool IsFunctionGroupValue() const { return false; }
    virtual bool IsArrayReferenceValue() const { return false; }
    virtual bool IsStructuredReferenceValue() const { return false; }
    virtual bool IsStringReferenceValue() const { return false; }
    virtual bool IsComplexValue() const { return false; }
    virtual Value* GetSubject() { return this; }
    virtual std::string ToString() const { return std::string(); }
    virtual TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) = 0;
    virtual void SetType(TypeSymbol* type_) {}
    const soul::ast::Span& GetSpan() const { return span; }
    ValueType GetValueType() const { return valueType; }
    std::unique_ptr<soul::xml::Element> ToDomElement();
    virtual const char* ClassName() const { return "Value"; }
private:
    soul::ast::Span span;
    ValueType valueType;
};

class BoolValue : public Value
{
public:
    typedef bool OperandType;
    BoolValue(const soul::ast::Span& span_, bool value_);
    Value* Clone() const override { return new BoolValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return value ? "true" : "false"; }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    bool GetValue() const { return value; }
    const char* ClassName() const override { return "BoolValue"; }
private:
    bool value;
};

class SByteValue : public Value
{
public:
    typedef int8_t OperandType;
    SByteValue(const soul::ast::Span& span_, int8_t value_);
    Value* Clone() const override { return new SByteValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int8_t GetValue() const { return value; }
    const char* ClassName() const override { return "SByteValue"; }
private:
    int8_t value;
};

class ByteValue : public Value
{
public:
    typedef uint8_t OperandType;
    ByteValue(const soul::ast::Span& span_, uint8_t value_);
    Value* Clone() const override { return new ByteValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    uint8_t GetValue() const { return value; }
    const char* ClassName() const override { return "ByteValue"; }
private:
    uint8_t value;
};

class ShortValue : public Value
{
public:
    typedef int16_t OperandType;
    ShortValue(const soul::ast::Span& span_, int16_t value_);
    Value* Clone() const override { return new ShortValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int16_t GetValue() const { return value; }
    const char* ClassName() const override { return "ShortValue"; }
private:
    int16_t value;
};

class UShortValue : public Value
{
public:
    typedef uint16_t OperandType;
    UShortValue(const soul::ast::Span& span_, uint16_t value_);
    Value* Clone() const override { return new UShortValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    uint16_t GetValue() const { return value; }
    const char* ClassName() const override { return "UShortValue"; }
private:
    uint16_t value;
};

class IntValue : public Value
{
public:
    typedef int32_t OperandType;
    IntValue(const soul::ast::Span& span_, int32_t value_);
    Value* Clone() const override { return new IntValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int32_t GetValue() const { return value; }
    const char* ClassName() const override { return "IntValue"; }
private:
    int32_t value;
};

class UIntValue : public Value
{
public:
    typedef uint32_t OperandType;
    UIntValue(const soul::ast::Span& span_, uint32_t value_);
    Value* Clone() const override { return new UIntValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    uint32_t GetValue() const { return value; }
    const char* ClassName() const override { return "UIntValue"; }
private:
    uint32_t value;
};

class LongValue : public Value
{
public:
    typedef int64_t OperandType;
    LongValue(const soul::ast::Span& span_, int64_t value_);
    Value* Clone() const override { return new LongValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int64_t GetValue() const { return value; }
    const char* ClassName() const override { return "LongValue"; }
private:
    int64_t value;
};

class ULongValue : public Value
{
public:
    typedef uint64_t OperandType;
    ULongValue(const soul::ast::Span& span_, uint64_t value_);
    Value* Clone() const override { return new ULongValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    uint64_t GetValue() const { return value; }
    const char* ClassName() const override { return "ULongValue"; }
private:
    uint64_t value;
};

class FloatValue : public Value
{
public:
    typedef float OperandType;
    FloatValue(const soul::ast::Span& span_, float value_);
    Value* Clone() const override { return new FloatValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    float GetValue() const { return value; }
    const char* ClassName() const override { return "FloatValue"; }
private:
    float value;
};

class DoubleValue : public Value
{
public:
    typedef double OperandType;
    DoubleValue(const soul::ast::Span& span_, double value_);
    Value* Clone() const override { return new DoubleValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* contexts) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    double GetValue() const { return value; }
    const char* ClassName() const override { return "DoubleValue"; }
private:
    double value;
};

class CharValue : public Value
{
public:
    typedef unsigned char OperandType;
    CharValue(const soul::ast::Span& span_, unsigned char value_);
    Value* Clone() const override { return new CharValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    unsigned char GetValue() const { return value; }
    const char* ClassName() const override { return "CharValue"; }
private:
    unsigned char value;
};

class WCharValue : public Value
{
public:
    typedef char16_t OperandType;
    WCharValue(const soul::ast::Span& span_, char16_t value_);
    Value* Clone() const override { return new WCharValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    char16_t GetValue() const { return value; }
    const char* ClassName() const override { return "WCharValue"; }
private:
    char16_t value;
};

class UCharValue : public Value
{
public:
    typedef char32_t OperandType;
    UCharValue(const soul::ast::Span& span_, char32_t value_);
    Value* Clone() const override { return new UCharValue(GetSpan(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    char32_t GetValue() const { return value; }
    const char* ClassName() const override { return "UCharValue"; }
private:
    char32_t value;
};

class StringValue : public Value
{
public:
    StringValue(const soul::ast::Span& span_, int stringId_, const std::string& str_);
    Value* Clone() const override { return new StringValue(GetSpan(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int StringId() const { return stringId; }
    const std::string& Str() const { return str; }
    const char* ClassName() const override { return "StringValue"; }
private:
    int stringId;
    std::string str;
};

class WStringValue : public Value
{
public:
    WStringValue(const soul::ast::Span& span_, int stringId_, const std::u16string& str_);
    Value* Clone() const override { return new WStringValue(GetSpan(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int StringId() const { return stringId; }
    const std::u16string& Str() const { return str; }
    const char* ClassName() const override { return "WStringValue"; }
private:
    int stringId;
    std::u16string str;
};

class UStringValue : public Value
{
public:
    UStringValue(const soul::ast::Span& span_, int stringId_, const std::u32string& str_);
    Value* Clone() const override { return new UStringValue(GetSpan(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int StringId() const { return stringId; }
    const std::u32string& Str() const { return str; }
    const char* ClassName() const override { return "UStringValue"; }
private:
    int stringId;
    std::u32string str;
};

class NullValue : public Value
{
public:
    NullValue(const soul::ast::Span& span_, TypeSymbol* nullPtrType_);
    Value* Clone() const override { return new NullValue(GetSpan(), nullPtrType); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    std::string ToString() const override { return "null"; }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    const char* ClassName() const override { return "NullValue"; }
private:
    TypeSymbol* nullPtrType;
};

class PointerValue : public Value
{
public:
    typedef const void* OperandType;
    PointerValue(const soul::ast::Span& span_, TypeSymbol* type, const void* ptr_);
    Value* Clone() const override { return new PointerValue(GetSpan(), type, ptr); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    void SetType(TypeSymbol* type_) override { type = type_; }
    TypeSymbol* PointeeType(Context* context) const;
    const void* GetValue() const { return ptr; }
    Value* Add(int64_t offset, Context* context) const;
    Value* Sub(int64_t offset, Context* context) const;
    Value* Sub(const void* thatPtr, Context* context) const;
    Value* Deref(Context* context) const;
    const char* ClassName() const override { return "PointerValue"; }
private:
    TypeSymbol* type;
    const void* ptr;
};

class ArrayValue : public Value
{
public:
    ArrayValue(const soul::ast::Span& span_, TypeSymbol* type_, std::vector<std::unique_ptr<Value>>&& elementValues_);
    ArrayValue(const ArrayValue&) = delete;
    ArrayValue& operator=(const ArrayValue&) = delete;
    Value* Clone() const override;
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    bool IsComplexValue() const override { return true; }
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override { return type; }
    void SetType(TypeSymbol* type_) override { type = type_; }
    const std::vector<std::unique_ptr<Value>>& Elements() const { return elementValues; }
    std::vector<std::unique_ptr<Value>>& Elements() { return elementValues; }
    const char* ClassName() const override { return "ArrayValue"; }
private:
    std::vector<std::unique_ptr<Value>> elementValues;
    TypeSymbol* type;
};

class StructuredValue : public Value
{
public:
    StructuredValue(const soul::ast::Span& span_, TypeSymbol* type_, std::vector<std::unique_ptr<Value>>&& memberValues_);
    StructuredValue(const StructuredValue&) = delete;
    StructuredValue& operator=(const StructuredValue&) = delete;
    Value* Clone() const override;
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    bool IsComplexValue() const override { return true; }
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    void SetType(TypeSymbol* type_) override { type = type_; }
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override { return type; }
    const std::vector<std::unique_ptr<Value>>& Members() const { return memberValues; }
    std::vector<std::unique_ptr<Value>>& Members() { return memberValues; }
    const char* ClassName() const override { return "StructuredValue"; }
private:
    std::vector<std::unique_ptr<Value>> memberValues;
    TypeSymbol* type;
};

class UuidValue : public Value
{
public:
    UuidValue(const soul::ast::Span& span_, int uuidId_);
    Value* Clone() const override { return new UuidValue(GetSpan(), uuidId); }
    void* IrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, cmajor::ast::Node* node, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable, Context* context) override;
    int UuidId() const { return uuidId; }
    const char* ClassName() const override { return "UuidValue"; }
private:
    int uuidId;
};


template<typename ValueT>
inline bool ValuesEqual(const ValueT& left, const ValueT& right)
{
    return left.GetValue() == right.GetValue();
}

template<typename ValueT>
inline size_t GetHashCode(const ValueT& value)
{
    return static_cast<size_t>(value.GetValue());
}

struct IntegralValue
{
    IntegralValue(Value* value_) : value(value_) {}
    Value* value;
};

bool operator==(IntegralValue left, IntegralValue right);

inline bool operator!=(IntegralValue left, IntegralValue right)
{
    return !(left == right);
}

struct IntegralValueHash
{
    size_t operator()(IntegralValue integralValue) const;
};

void WriteValue(Value* value, util::BinaryStreamWriter& writer);
std::unique_ptr<Value> ReadValue(util::BinaryStreamReader& reader);

} // namespace cmajor::symbols
