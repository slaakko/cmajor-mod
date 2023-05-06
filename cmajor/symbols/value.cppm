// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.value;

import soul.ast.source.pos;
import cmajor.ir.emitter;
import cmajor.symbols.type.symbol;
import util.binary.stream.reader;
import util.binary.stream.writer;
import soul.xml.element;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::symbols {

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
    Value(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ValueType valueType_);
    virtual ~Value();
    virtual Value* Clone() const = 0;
    virtual Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const = 0;
    virtual void* IrValue(cmajor::ir::Emitter& emitter) = 0;
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
    virtual TypeSymbol* GetType(SymbolTable* symbolTable) = 0;
    virtual void SetType(TypeSymbol* type_) {}
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    const util::uuid& ModuleId() const { return moduleId; }
    ValueType GetValueType() const { return valueType; }
    std::unique_ptr<soul::xml::Element> ToDomElement();
    virtual const char* ClassName() const { return "Value"; }
private:
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    ValueType valueType;
};

class BoolValue : public Value
{
public:
    typedef bool OperandType;
    BoolValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, bool value_);
    Value* Clone() const override { return new BoolValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return value ? "true" : "false"; }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    bool GetValue() const { return value; }
    const char* ClassName() const override { return "BoolValue"; }
private:
    bool value;
};

class SByteValue : public Value
{
public:
    typedef int8_t OperandType;
    SByteValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int8_t value_);
    Value* Clone() const override { return new SByteValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    int8_t GetValue() const { return value; }
    const char* ClassName() const override { return "SByteValue"; }
private:
    int8_t value;
};

class ByteValue : public Value
{
public:
    typedef uint8_t OperandType;
    ByteValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint8_t value_);
    Value* Clone() const override { return new ByteValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    uint8_t GetValue() const { return value; }
    const char* ClassName() const override { return "ByteValue"; }
private:
    uint8_t value;
};

class ShortValue : public Value
{
public:
    typedef int16_t OperandType;
    ShortValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int16_t value_);
    Value* Clone() const override { return new ShortValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    int16_t GetValue() const { return value; }
    const char* ClassName() const override { return "ShortValue"; }
private:
    int16_t value;
};

class UShortValue : public Value
{
public:
    typedef uint16_t OperandType;
    UShortValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint16_t value_);
    Value* Clone() const override { return new UShortValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    uint16_t GetValue() const { return value; }
    const char* ClassName() const override { return "UShortValue"; }
private:
    uint16_t value;
};

class IntValue : public Value
{
public:
    typedef int32_t OperandType;
    IntValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int32_t value_);
    Value* Clone() const override { return new IntValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    int32_t GetValue() const { return value; }
    const char* ClassName() const override { return "IntValue"; }
private:
    int32_t value;
};

class UIntValue : public Value
{
public:
    typedef uint32_t OperandType;
    UIntValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint32_t value_);
    Value* Clone() const override { return new UIntValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    uint32_t GetValue() const { return value; }
    const char* ClassName() const override { return "UIntValue"; }
private:
    uint32_t value;
};

class LongValue : public Value
{
public:
    typedef int64_t OperandType;
    LongValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int64_t value_);
    Value* Clone() const override { return new LongValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    int64_t GetValue() const { return value; }
    const char* ClassName() const override { return "LongValue"; }
private:
    int64_t value;
};

class ULongValue : public Value
{
public:
    typedef uint64_t OperandType;
    ULongValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint64_t value_);
    Value* Clone() const override { return new ULongValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    uint64_t GetValue() const { return value; }
    const char* ClassName() const override { return "ULongValue"; }
private:
    uint64_t value;
};

class FloatValue : public Value
{
public:
    typedef float OperandType;
    FloatValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, float value_);
    Value* Clone() const override { return new FloatValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    float GetValue() const { return value; }
    const char* ClassName() const override { return "FloatValue"; }
private:
    float value;
};

class DoubleValue : public Value
{
public:
    typedef double OperandType;
    DoubleValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, double value_);
    Value* Clone() const override { return new DoubleValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    double GetValue() const { return value; }
    const char* ClassName() const override { return "DoubleValue"; }
private:
    double value;
};

class CharValue : public Value
{
public:
    typedef unsigned char OperandType;
    CharValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, unsigned char value_);
    Value* Clone() const override { return new CharValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    unsigned char GetValue() const { return value; }
    const char* ClassName() const override { return "CharValue"; }
private:
    unsigned char value;
};

class WCharValue : public Value
{
public:
    typedef char16_t OperandType;
    WCharValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char16_t value_);
    Value* Clone() const override { return new WCharValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    char16_t GetValue() const { return value; }
    const char* ClassName() const override { return "WCharValue"; }
private:
    char16_t value;
};

class UCharValue : public Value
{
public:
    typedef char32_t OperandType;
    UCharValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char32_t value_);
    Value* Clone() const override { return new UCharValue(GetSourcePos(), ModuleId(), value); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return std::to_string(value); }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    char32_t GetValue() const { return value; }
    const char* ClassName() const override { return "UCharValue"; }
private:
    char32_t value;
};

class StringValue : public Value
{
public:
    StringValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int stringId_, const std::string& str_);
    Value* Clone() const override { return new StringValue(GetSourcePos(), ModuleId(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
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
    WStringValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int stringId_, const std::u16string& str_);
    Value* Clone() const override { return new WStringValue(GetSourcePos(), ModuleId(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
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
    UStringValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int stringId_, const std::u32string& str_);
    Value* Clone() const override { return new UStringValue(GetSourcePos(), ModuleId(), stringId, str); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
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
    NullValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, TypeSymbol* nullPtrType_);
    Value* Clone() const override { return new NullValue(GetSourcePos(), ModuleId(), nullPtrType); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    std::string ToString() const override { return "null"; }
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    const char* ClassName() const override { return "NullValue"; }
private:
    TypeSymbol* nullPtrType;
};

class PointerValue : public Value
{
public:
    typedef const void* OperandType;
    PointerValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, TypeSymbol* type, const void* ptr_);
    Value* Clone() const override { return new PointerValue(GetSourcePos(), ModuleId(), type, ptr); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
    void SetType(TypeSymbol* type_) override { type = type_; }
    TypeSymbol* PointeeType() const;
    const void* GetValue() const { return ptr; }
    Value* Add(int64_t offset) const;
    Value* Sub(int64_t offset) const;
    Value* Sub(const void* thatPtr) const;
    Value* Deref() const;
    const char* ClassName() const override { return "PointerValue"; }
private:
    TypeSymbol* type;
    const void* ptr;
};

class ArrayValue : public Value
{
public:
    ArrayValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, TypeSymbol* type_, std::vector<std::unique_ptr<Value>>&& elementValues_);
    ArrayValue(const ArrayValue&) = delete;
    ArrayValue& operator=(const ArrayValue&) = delete;
    Value* Clone() const override;
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    bool IsComplexValue() const override { return true; }
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override { return type; }
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
    StructuredValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, TypeSymbol* type_, std::vector<std::unique_ptr<Value>>&& memberValues_);
    StructuredValue(const StructuredValue&) = delete;
    StructuredValue& operator=(const StructuredValue&) = delete;
    Value* Clone() const override;
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    bool IsComplexValue() const override { return true; }
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    void SetType(TypeSymbol* type_) override { type = type_; }
    TypeSymbol* GetType(SymbolTable* symbolTable) override { return type; }
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
    UuidValue(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int uuidId_);
    Value* Clone() const override { return new UuidValue(GetSourcePos(), ModuleId(), uuidId); }
    void* IrValue(cmajor::ir::Emitter& emitter) override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    Value* As(TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override;
    TypeSymbol* GetType(SymbolTable* symbolTable) override;
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
std::unique_ptr<Value> ReadValue(util::BinaryStreamReader& reader, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
} // namespace cmajor::symbols
