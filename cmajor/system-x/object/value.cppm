// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.value;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::object {

enum class TypedValueKind
{
    errorValue = 0, valueRef = 1, boolValue = 2, sbyteValue = 3, byteValue = 4, shortValue = 5, ushortValue = 6, intValue = 7, uintValue = 8, longValue = 9, ulongValue = 10,
    floatValue = 11, doubleValue = 12, structureValue = 13, elementValue = 14, arrayValue = 15, functionValue = 16, pointerValue = 17, 
    stringValue = 18, wstringValue = 19, ustringValue = 20, containerValue = 21
};

class StringTable;
class SymbolTable;
class TypeTable;
class TypedValue;
class Type;
class StructureType;
class PointerType;
class SystemType;

struct EvaluationContext
{
    EvaluationContext(
        int64_t address_, uint64_t rv_, cmajor::systemx::machine::Memory& memory_, StringTable* stringTable_, SymbolTable* symbolTable_, 
        TypeTable* typeTable_, std::vector<std::unique_ptr<TypedValue>>* indexedValues_, std::vector<std::unique_ptr<TypedValue>>* values_, 
        std::vector<std::unique_ptr<TypedValue>>* stack_, int64_t pageSize_);
    void Push(TypedValue* value);
    std::unique_ptr<TypedValue> Pop();
    int64_t address;
    uint64_t rv;
    cmajor::systemx::machine::Memory& memory;
    StringTable* stringTable;
    SymbolTable* symbolTable;
    TypeTable* typeTable;
    int64_t start;
    int64_t count;
    bool content;
    int64_t pageSize;
    std::vector<std::unique_ptr<TypedValue>>* indexedValues;
    std::vector<std::unique_ptr<TypedValue>>* values;
    std::vector<std::unique_ptr<TypedValue>>* stack;
};

class TypedValue
{
public:
    TypedValue(TypedValueKind kind_);
    virtual ~TypedValue();
    TypedValueKind Kind() const { return kind; }
    virtual TypedValue* Get() { return this; }
    virtual std::string ToString() const;
    virtual bool Content() const { return true; }
    void SetIndex(int64_t index_) { index = index_; }
    int64_t Index() const { return index; }
    int64_t Start() const { return start; }
    void SetStart(int64_t start_) { start = start_; }
    virtual int64_t NextStart() const { return -1; }
    Type* GetType() const { return type; }
    virtual void SetType(Type* type_) { type = type_; }
    uint64_t Address() const { return address; }
    void SetAddress(uint64_t address_) { address = address_; }
    bool IsValueRef() const { return kind == TypedValueKind::valueRef; }
    bool IsBoolValue() const { return kind == TypedValueKind::boolValue; }
    bool IsSByteValue() const { return kind == TypedValueKind::sbyteValue; }
    bool IsByteValue() const { return kind == TypedValueKind::byteValue; }
    bool IsShortValue() const { return kind == TypedValueKind::shortValue; }
    bool IsUShortValue() const { return kind == TypedValueKind::ushortValue; }
    bool IsIntValue() const { return kind == TypedValueKind::intValue; }
    bool IsUIntValue() const { return kind == TypedValueKind::uintValue; }
    bool IsLongValue() const { return kind == TypedValueKind::longValue; }
    bool IsULongValue() const { return kind == TypedValueKind::ulongValue; }
    bool IsFloatValue() const { return kind == TypedValueKind::floatValue; }
    bool IsDoubleValue() const { return kind == TypedValueKind::doubleValue; }
    bool IsPointerValue() const { return kind == TypedValueKind::pointerValue; }
    bool IsElementValue() const { return kind == TypedValueKind::elementValue; }
    bool IsStructureValue() const { return kind == TypedValueKind::structureValue; }
    bool IsContainerValue() const { return kind == TypedValueKind::containerValue; }
private:
    TypedValueKind kind;
    int64_t index;
    Type* type;
    uint64_t address;
    int64_t start;
};

class ValueRef : public TypedValue
{
public:
    ValueRef(TypedValue* value_);
    TypedValue* Get() override { return value->Get(); }
    TypedValue* Value() const { return value; }
    void SetType(Type* type_) override;
    std::string ToString() const override;
private:
    TypedValue* value;
};

class ErrorValue : public TypedValue
{
public:
    ErrorValue();
    std::string ToString() const;
};

class BoolValue : public TypedValue
{
public:
    BoolValue(bool value_);
    std::string ToString() const override;
    bool Value() const { return value; }
private:
    bool value;
};

class SByteValue : public TypedValue
{
public:
    SByteValue(int8_t value_);
    std::string ToString() const override;
    int8_t Value() const { return value; }
private:
    int8_t value;
};

class ByteValue : public TypedValue
{
public:
    ByteValue(uint8_t value_);
    std::string ToString() const override;
    uint8_t Value() const { return value; }
private:
    uint8_t value;
};

class ShortValue : public TypedValue
{
public:
    ShortValue(int16_t value_);
    std::string ToString() const override;
    int16_t Value() const { return value; }
private:
    int16_t value;
};

class UShortValue : public TypedValue
{
public:
    UShortValue(uint16_t value_);
    std::string ToString() const override;
    uint16_t Value() const { return value; }
private:
    uint16_t value;
};

class IntValue : public TypedValue
{
public:
    IntValue(int32_t value_);
    std::string ToString() const override;
    int32_t Value() const { return value; }
private:
    int32_t value;
};

class UIntValue : public TypedValue
{
public:
    UIntValue(uint32_t value_);
    std::string ToString() const override;
    uint32_t Value() const { return value; }
private:
    uint32_t value;
};

class LongValue : public TypedValue
{
public:
    LongValue(int64_t value_);
    std::string ToString() const override;
    int64_t Value() const { return value; }
private:
    int64_t value;
};

class ULongValue : public TypedValue
{
public:
    ULongValue(uint64_t value_);
    std::string ToString() const override;
    uint64_t Value() const { return value; }
private:
    uint64_t value;
};

class FloatValue : public TypedValue
{
public:
    FloatValue(float value_);
    std::string ToString() const override;
    float Value() const { return value; }
private:
    float value;
};

class DoubleValue : public TypedValue
{
public:
    DoubleValue(double value_);
    std::string ToString() const override;
    double Value() const { return value; }
private:
    double value;
};

class FieldValue
{
public:
    FieldValue(const std::string& name_, TypedValue* value_);
    const std::string& Name() const { return name; }
    TypedValue* Value() const { return value; }
    std::string ToString() const;
private:
    std::string name;
    TypedValue* value;
};

class StructureValue : public TypedValue
{
public:
    StructureValue();
    void AddFieldValue(FieldValue&& fieldValue);
    const FieldValue& GetFieldValue(int64_t index) const;
    const FieldValue& GetFieldValue(const std::string& fieldName) const;
    std::string ToString() const override;
private:
    std::vector<FieldValue> fieldValues;
    std::map<std::string, int64_t> fieldIndexMap;
};

class ElementValue : public TypedValue
{
public:
    ElementValue(int64_t index_, TypedValue* value_);
    TypedValue* GetValue() const { return value; }
    std::string ToString() const;
private:
    int64_t index;
    TypedValue* value;
};

class ArrayValue : public TypedValue
{
public:
    ArrayValue();
    void AddElementValue(ElementValue&& elementValue);
    std::string ToString() const override;
private:
    std::vector<ElementValue> elementValues;
};

class FunctionValue : public TypedValue
{
public:
    FunctionValue();
    void SetName(const std::string& name_);
    std::string ToString() const override;
private:
    std::string name;
};

class PointerValue : public TypedValue
{
public:
    PointerValue();
    std::string ToString() const override;
    bool IsNull() const { return Address() == 0; }
};

class StringValue : public TypedValue
{
public:
    StringValue();
    void SetStr(const std::string& str_);
    std::string ToString() const override;
private:
    std::string str;
};

class WStringValue : public TypedValue
{
public:
    WStringValue();
    void SetStr(const std::u16string& str_);
    std::string ToString() const override;
private:
    std::u16string str;
};

class UStringValue : public TypedValue
{
public:
    UStringValue();
    void SetStr(const std::u32string& str_);
    std::string ToString() const override;
private:
    std::u32string str;
};

class ContainerValue : public TypedValue
{
public:
    ContainerValue();
    void SetCount(int64_t count_) { count = count_; }
    int64_t Count() const { return count; }
    void SetContent() { content = true; }
    bool Content() const override { return content; }
    void AddElement(TypedValue* element);
    const std::vector<std::unique_ptr<TypedValue>>& Elements() const { return elements; }
    std::string ToString() const override;
    int64_t NextStart() const override { return nextStart; }
    void SetNextStart(int64_t nextStart_) { nextStart = nextStart_; }
private:
    int64_t count;
    bool content;
    int64_t nextStart;
    std::vector<std::unique_ptr<TypedValue>> elements;
};

TypedValue* Evaluate(const std::string& expr, EvaluationContext& context);

} // namespace cmajor::systemx::object
