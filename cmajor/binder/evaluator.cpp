// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.evaluator;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.binder;
import cmajor.binder.type.resolver;
import cmajor.binder.overload.resolution;
import cmajor.binder.statement.binder;
import cmajor.binder.bound.expression;
import util;

namespace cmajor::binder {

void ThrowCannotEvaluateStatically(const soul::ast::SourcePos& defined, const util::uuid& moduleId)
{
    throw cmajor::symbols::Exception("cannot evaluate statically", defined, moduleId);
}

void ThrowCannotEvaluateStatically(const soul::ast::SourcePos& defined, const util::uuid& moduleId, const soul::ast::SourcePos& referenced, const util::uuid& referencedModuleId)
{
    throw cmajor::symbols::Exception("cannot evaluate statically", defined, moduleId, referenced, referencedModuleId);
}

typedef cmajor::symbols::Value* (*BinaryOperatorFun)(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow);
typedef cmajor::symbols::Value* (*UnaryOperatorFun)(cmajor::symbols::Value* operand, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow);

class ScopedValue : public cmajor::symbols::Value
{
public:
    ScopedValue(const soul::ast::SourcePos& span_, const util::uuid& moduleId_, cmajor::symbols::ContainerSymbol* containerSymbol_);
    bool IsComplete() const override { return false; }
    bool IsScopedValue() const override { return true; }
    const cmajor::symbols::ContainerSymbol* GetContainerSymbol() const { return containerSymbol; }
    cmajor::symbols::ContainerSymbol* GetContainerSymbol() { return containerSymbol; }
    cmajor::symbols::Value* Clone() const override 
    { 
        // Assert(false, "scoped value cannot be cloned"); TODO
        return nullptr; 
    }
    void Write(util::BinaryStreamWriter& writer) override {}
    void Read(util::BinaryStreamReader& reader) override {}
    cmajor::symbols::Value* As(cmajor::symbols::TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow) const override 
    { 
        // Assert(false, "scoped value cannot be converted"); TODO 
        return nullptr; 
    }
    void* IrValue(cmajor::ir::Emitter& emitter) override 
    { 
        // Assert(false, "scoped value does not have ir value"); TODO 
        return nullptr; 
    }
    cmajor::symbols::TypeSymbol* GetType(cmajor::symbols::SymbolTable* symbolTable) override { return type; }
    void SetType(cmajor::symbols::TypeSymbol* type_) override { type = type_; }
    cmajor::symbols::Value* GetSubject() override { return subject.get(); }
    void SetSubject(cmajor::symbols::Value* subject_) { subject.reset(subject_); }
private:
    cmajor::symbols::ContainerSymbol* containerSymbol;
    cmajor::symbols::TypeSymbol* type;
    std::unique_ptr<cmajor::symbols::Value> subject;
};

ScopedValue::ScopedValue(const soul::ast::SourcePos& span_, const util::uuid& moduleId_, cmajor::symbols::ContainerSymbol* containerSymbol_) :
    cmajor::symbols::Value(span_, moduleId_, cmajor::symbols::ValueType::none), containerSymbol(containerSymbol_), type(nullptr)
{
}

class FunctionGroupValue : public cmajor::symbols::Value
{
public:
    FunctionGroupValue(cmajor::symbols::FunctionGroupSymbol* functionGroup_, cmajor::symbols::ContainerScope* qualifiedScope_);
    bool IsComplete() const override { return false; }
    bool IsFunctionGroupValue() const override { return true; }
    cmajor::symbols::Value* Clone() const override 
    { 
        // Assert(false, "function group value cannot be cloned"); TODO
        return nullptr; 
    }
    void Write(util::BinaryStreamWriter& writer) override {}
    void Read(util::BinaryStreamReader& reader) override {}
    cmajor::symbols::Value* As(cmajor::symbols::TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, bool dontThrow) const override 
    { 
        /// Assert(false, "function group value cannot be converted"); TODO
        return nullptr; 
    }
    void* IrValue(cmajor::ir::Emitter& emitter) override 
    { 
        // Assert(false, "function group value does not have ir value");  TODO
        return nullptr; 
    }
    cmajor::symbols::FunctionGroupSymbol* FunctionGroup() { return functionGroup; }
    cmajor::symbols::ContainerScope* QualifiedScope() { return qualifiedScope; }
    cmajor::symbols::TypeSymbol* GetType(cmajor::symbols::SymbolTable* symbolTable) override { return nullptr; }
    void SetTemplateTypeArguments(std::vector<cmajor::symbols::TypeSymbol*>&& templateTypeArguments_) { templateTypeArguments = std::move(templateTypeArguments_); }
    std::vector<cmajor::symbols::TypeSymbol*> TemplateTypeArguments() { return std::move(templateTypeArguments); }
    void SetReceiver(std::unique_ptr<cmajor::symbols::Value>&& receiver_) { receiver = std::move(receiver_); }
    cmajor::symbols::Value* Receiver() { return receiver.get(); }
private:
    cmajor::symbols::FunctionGroupSymbol* functionGroup;
    cmajor::symbols::ContainerScope* qualifiedScope;
    std::vector<cmajor::symbols::TypeSymbol*> templateTypeArguments;
    std::unique_ptr<cmajor::symbols::Value> receiver;
};

FunctionGroupValue::FunctionGroupValue(cmajor::symbols::FunctionGroupSymbol* functionGroup_, cmajor::symbols::ContainerScope* qualifiedScope_) : cmajor::symbols::Value(soul::ast::SourcePos(), util::nil_uuid(), cmajor::symbols::ValueType::none), functionGroup(functionGroup_), qualifiedScope(qualifiedScope_)
{
}

class ArrayReferenceValue : public cmajor::symbols::Value
{
public:
    ArrayReferenceValue(cmajor::symbols::ArrayValue* arrayValue_);
    bool IsArrayReferenceValue() const override { return true; }
    cmajor::symbols::Value* Clone() const override { return new ArrayReferenceValue(arrayValue); }
    void Write(util::BinaryStreamWriter& writer) override {}
    void Read(util::BinaryStreamReader& reader) override {}
    cmajor::symbols::Value* As(cmajor::symbols::TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, bool dontThrow) const override 
    {
        // Assert(false, "array reference value cannot be converted"); TODO
        return nullptr; 
    }
    void* IrValue(cmajor::ir::Emitter& emitter) override 
    { 
        // Assert(false, "array reference does not have ir value"); TODO
        return nullptr; 
    }
    cmajor::symbols::TypeSymbol* GetType(cmajor::symbols::SymbolTable* symbolTable) override { return arrayValue->GetType(symbolTable); }
    cmajor::symbols::ArrayValue* GetArrayValue() const { return arrayValue; }
private:
    cmajor::symbols::ArrayValue* arrayValue;
};

ArrayReferenceValue::ArrayReferenceValue(cmajor::symbols::ArrayValue* arrayValue_) : cmajor::symbols::Value(arrayValue_->GetSourcePos(), arrayValue_->ModuleId(), cmajor::symbols::ValueType::none), arrayValue(arrayValue_)
{
}

class StructuredReferenceValue : public cmajor::symbols::Value
{
public:
    StructuredReferenceValue(cmajor::symbols::StructuredValue* structuredValue_);
    bool IsStructuredReferenceValue() const override { return true; }
    cmajor::symbols::Value* Clone() const override { return new StructuredReferenceValue(structuredValue); }
    void Write(util::BinaryStreamWriter& writer) override {}
    void Read(util::BinaryStreamReader& reader) override {}
    cmajor::symbols::Value* As(cmajor::symbols::TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, bool dontThrow) const override 
    {
        // Assert(false, "structured reference value cannot be converted"); TODO
        return nullptr; 
    }
    void* IrValue(cmajor::ir::Emitter& emitter) override 
    { 
        // Assert(false, "structured reference does not have ir value"); TODO
        return nullptr; 
    }
    cmajor::symbols::TypeSymbol* GetType(cmajor::symbols::SymbolTable* symbolTable) override { return structuredValue->GetType(symbolTable); }
    cmajor::symbols::StructuredValue* GetStructuredValue() const { return structuredValue; }
private:
    cmajor::symbols::StructuredValue* structuredValue;
};

StructuredReferenceValue::StructuredReferenceValue(cmajor::symbols::StructuredValue* structuredValue_) : cmajor::symbols::Value(structuredValue_->GetSourcePos(), structuredValue_->ModuleId(), cmajor::symbols::ValueType::none), structuredValue(structuredValue_)
{
}

class StringReferenceValue : public cmajor::symbols::Value
{
public:
    StringReferenceValue(cmajor::symbols::Value* stringValue_);
    bool IsStringReferenceValue() const override { return true; }
    cmajor::symbols::Value* Clone() const override { return new StringReferenceValue(stringValue); }
    void Write(util::BinaryStreamWriter& writer) override {}
    void Read(util::BinaryStreamReader& reader) override {}
    cmajor::symbols::Value* As(cmajor::symbols::TypeSymbol* targetType, bool cast, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, bool dontThrow) const override 
    { 
        // Assert(false, "string reference value cannot be converted"); TODO
        return nullptr; 
    }
    void* IrValue(cmajor::ir::Emitter& emitter) override { return stringValue->IrValue(emitter); }
    cmajor::symbols::TypeSymbol* GetType(cmajor::symbols::SymbolTable* symbolTable) override { return stringValue->GetType(symbolTable); }
    cmajor::symbols::Value* GetSubject() override { return stringValue; }
private:
    cmajor::symbols::Value* stringValue;
};

StringReferenceValue::StringReferenceValue(cmajor::symbols::Value* stringValue_) : cmajor::symbols::Value(stringValue_->GetSourcePos(), stringValue_->ModuleId(), cmajor::symbols::ValueType::none), stringValue(stringValue_)
{
}

class VariableValueSymbol : public cmajor::symbols::VariableSymbol
{
public:
    VariableValueSymbol(const soul::ast::SourcePos& span_, const util::uuid& moduleId_, const std::u32string& name_, std::unique_ptr<cmajor::symbols::Value>&& value_);
    cmajor::symbols::Value* GetValue() { return value.get(); }
    void SetValue(cmajor::symbols::Value* value_) { value.reset(value_); }
    const char* ClassName() const override { return "VariableValueSymbol"; }
private:
    std::unique_ptr<cmajor::symbols::Value> value;
};

VariableValueSymbol::VariableValueSymbol(const soul::ast::SourcePos& span_, const util::uuid& moduleId_, const std::u32string& name_, std::unique_ptr<cmajor::symbols::Value>&& value_) :
    cmajor::symbols::VariableSymbol(cmajor::symbols::SymbolType::variableValueSymbol, span_, moduleId_, name_), value(std::move(value_))
{
}

std::vector<std::unique_ptr<BoundExpression>> ValuesToLiterals(std::vector<std::unique_ptr<cmajor::symbols::Value>>& values, cmajor::symbols::SymbolTable* symbolTable, bool& error)
{
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    for (std::unique_ptr<cmajor::symbols::Value>& value : values)
    {
        cmajor::symbols::ValueType valueType = value->GetValueType();
        cmajor::symbols::TypeSymbol* type = value->GetType(symbolTable);
        BoundLiteral* literal = new BoundLiteral(std::move(value), type);
        arguments.push_back(std::unique_ptr<BoundExpression>(literal));
    }
    return arguments;
}

std::vector<std::unique_ptr<cmajor::symbols::Value>> ArgumentsToValues(const std::vector<std::unique_ptr<BoundExpression>>& arguments, bool& error, bool skipFirst, BoundCompileUnit& boundCompileUnit);

std::vector<std::unique_ptr<cmajor::symbols::Value>> ArgumentsToValues(const std::vector<std::unique_ptr<BoundExpression>>& arguments, bool& error, BoundCompileUnit& boundCompileUnit)
{
    return ArgumentsToValues(arguments, error, false, boundCompileUnit);
}

std::vector<std::unique_ptr<cmajor::symbols::Value>> ArgumentsToValues(const std::vector<std::unique_ptr<BoundExpression>>& arguments, bool& error, bool skipFirst, BoundCompileUnit& boundCompileUnit)
{
    std::vector<std::unique_ptr<cmajor::symbols::Value>> values;
    bool first = true;
    for (const std::unique_ptr<BoundExpression>& argument : arguments)
    {
        if (first)
        {
            first = false;
            if (skipFirst)
            {
                continue;
            }
        }
        std::unique_ptr<cmajor::symbols::Value> value = argument->ToValue(boundCompileUnit);
        if (value)
        {
            values.push_back(std::move(value));
        }
        else
        {
            error = true;
            return values;
        }
    }
    return values;
}

template <typename ValueT, typename Op>
cmajor::symbols::Value* BinaryEvaluate(cmajor::symbols::Value* left, cmajor::symbols::Value* right, Op op, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    ValueT* leftCasted = static_cast<ValueT*>(left);
    ValueT* rightCasted = static_cast<ValueT*>(right);
    return new ValueT(sourcePos, moduleId, op(leftCasted->GetValue(), rightCasted->GetValue()));
}

template <typename ValueT, typename Op>
cmajor::symbols::Value* BinaryPredEvaluate(cmajor::symbols::Value* left, cmajor::symbols::Value* right, Op op, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    ValueT* leftCasted = static_cast<ValueT*>(left);
    ValueT* rightCasted = static_cast<ValueT*>(right);
    return new cmajor::symbols::BoolValue(sourcePos, moduleId, op(leftCasted->GetValue(), rightCasted->GetValue()));
}

template<typename ValueT, typename Op>
cmajor::symbols::Value* UnaryEvaluate(cmajor::symbols::Value* subject, Op op, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    ValueT* subjectCasted = static_cast<ValueT*>(subject);
    return new ValueT(sourcePos, moduleId, op(subjectCasted->GetValue()));
}

cmajor::symbols::Value* NotSupported(cmajor::symbols::Value* subject, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    if (dontThrow)
    {
        return nullptr;
    }
    throw cmajor::symbols::Exception("operation not supported for type " + ValueTypeStr(subject->GetValueType()), sourcePos, moduleId);
}

cmajor::symbols::Value* NotSupported(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    if (dontThrow)
    {
        return nullptr;
    }
    throw cmajor::symbols::Exception("operation not supported for types " + ValueTypeStr(left->GetValueType()) + " and " + ValueTypeStr(right->GetValueType()), sourcePos, moduleId);
}

template<typename ValueT>
cmajor::symbols::Value* Disjunction(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::logical_or<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun disjunction[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, Disjunction<cmajor::symbols::BoolValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Conjunction(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::logical_and<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun conjunction[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, Conjunction<cmajor::symbols::BoolValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* BitOr(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::bit_or<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun bitOr[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, BitOr<cmajor::symbols::SByteValue>, BitOr<cmajor::symbols::ByteValue>, BitOr<cmajor::symbols::ShortValue>, BitOr<cmajor::symbols::UShortValue>, BitOr<cmajor::symbols::IntValue>, BitOr<cmajor::symbols::UIntValue>,
    BitOr<cmajor::symbols::LongValue>, BitOr<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* BitXor(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::bit_xor<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun bitXor[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, BitXor<cmajor::symbols::SByteValue>, BitXor<cmajor::symbols::ByteValue>, BitXor<cmajor::symbols::ShortValue>, BitXor<cmajor::symbols::UShortValue>, BitXor<cmajor::symbols::IntValue>, BitXor<cmajor::symbols::UIntValue>,
    BitXor<cmajor::symbols::LongValue>, BitXor<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* BitAnd(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::bit_and<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun bitAnd[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, BitAnd<cmajor::symbols::SByteValue>, BitAnd<cmajor::symbols::ByteValue>, BitAnd<cmajor::symbols::ShortValue>, BitAnd<cmajor::symbols::UShortValue>, BitAnd<cmajor::symbols::IntValue>, BitAnd<cmajor::symbols::UIntValue>,
    BitAnd<cmajor::symbols::LongValue>, BitAnd<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Equal(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::equal_to<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun equal[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, Equal<cmajor::symbols::BoolValue>, Equal<cmajor::symbols::SByteValue>, Equal<cmajor::symbols::ByteValue>, Equal<cmajor::symbols::ShortValue>, Equal<cmajor::symbols::UShortValue>, Equal<cmajor::symbols::IntValue>, Equal<cmajor::symbols::UIntValue>,
    Equal<cmajor::symbols::LongValue>, Equal<cmajor::symbols::ULongValue>, Equal<cmajor::symbols::FloatValue>, Equal<cmajor::symbols::DoubleValue>, Equal<cmajor::symbols::CharValue>, Equal<cmajor::symbols::WCharValue>, Equal<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, Equal<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* NotEqual(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::not_equal_to<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun notEqual[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotEqual<cmajor::symbols::BoolValue>, NotEqual<cmajor::symbols::SByteValue>, NotEqual<cmajor::symbols::ByteValue>, NotEqual<cmajor::symbols::ShortValue>, NotEqual<cmajor::symbols::UShortValue>, NotEqual<cmajor::symbols::IntValue>, NotEqual<cmajor::symbols::UIntValue>,
    NotEqual<cmajor::symbols::LongValue>, NotEqual<cmajor::symbols::ULongValue>, NotEqual<cmajor::symbols::FloatValue>, NotEqual<cmajor::symbols::DoubleValue>, NotEqual<cmajor::symbols::CharValue>, NotEqual<cmajor::symbols::WCharValue>, NotEqual<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, NotEqual<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Less(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::less<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun less[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Less<cmajor::symbols::SByteValue>, Less<cmajor::symbols::ByteValue>, Less<cmajor::symbols::ShortValue>, Less<cmajor::symbols::UShortValue>, Less<cmajor::symbols::IntValue>, Less<cmajor::symbols::UIntValue>,
    Less<cmajor::symbols::LongValue>, Less<cmajor::symbols::ULongValue>, Less<cmajor::symbols::FloatValue>, Less<cmajor::symbols::DoubleValue>, Less<cmajor::symbols::CharValue>, Less<cmajor::symbols::WCharValue>, Less<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, Less<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Greater(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::greater<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun greater[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Greater<cmajor::symbols::SByteValue>, Greater<cmajor::symbols::ByteValue>, Greater<cmajor::symbols::ShortValue>, Greater<cmajor::symbols::UShortValue>, Greater<cmajor::symbols::IntValue>, Greater<cmajor::symbols::UIntValue>,
    Greater<cmajor::symbols::LongValue>, Greater<cmajor::symbols::ULongValue>, Greater<cmajor::symbols::FloatValue>, Greater<cmajor::symbols::DoubleValue>, Greater<cmajor::symbols::CharValue>, Greater<cmajor::symbols::WCharValue>, Greater<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, Greater<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* LessEqual(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::less_equal<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun lessEqual[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, LessEqual<cmajor::symbols::SByteValue>, LessEqual<cmajor::symbols::ByteValue>, LessEqual<cmajor::symbols::ShortValue>, LessEqual<cmajor::symbols::UShortValue>, LessEqual<cmajor::symbols::IntValue>, LessEqual<cmajor::symbols::UIntValue>,
    LessEqual<cmajor::symbols::LongValue>, LessEqual<cmajor::symbols::ULongValue>, LessEqual<cmajor::symbols::FloatValue>, LessEqual<cmajor::symbols::DoubleValue>, LessEqual<cmajor::symbols::CharValue>, LessEqual<cmajor::symbols::WCharValue>, LessEqual<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, LessEqual<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* GreaterEqual(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryPredEvaluate<ValueT>(left, right, std::greater_equal<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun greaterEqual[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, GreaterEqual<cmajor::symbols::SByteValue>, GreaterEqual<cmajor::symbols::ByteValue>, GreaterEqual<cmajor::symbols::ShortValue>, GreaterEqual<cmajor::symbols::UShortValue>, GreaterEqual<cmajor::symbols::IntValue>, GreaterEqual<cmajor::symbols::UIntValue>,
    GreaterEqual<cmajor::symbols::LongValue>, GreaterEqual<cmajor::symbols::ULongValue>, GreaterEqual<cmajor::symbols::FloatValue>, GreaterEqual<cmajor::symbols::DoubleValue>, GreaterEqual<cmajor::symbols::CharValue>, GreaterEqual<cmajor::symbols::WCharValue>, GreaterEqual<cmajor::symbols::UCharValue>,
    NotSupported, NotSupported, NotSupported, NotSupported, GreaterEqual<cmajor::symbols::PointerValue>, NotSupported, NotSupported
};

template<typename T>
struct shiftLeftFun
{
    T operator()(const T& left, const T& right) const
    {
        return left << right;
    }
};

template<typename ValueT>
cmajor::symbols::Value* ShiftLeft(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, shiftLeftFun<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun shiftLeft[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, ShiftLeft<cmajor::symbols::SByteValue>, ShiftLeft<cmajor::symbols::ByteValue>, ShiftLeft<cmajor::symbols::ShortValue>, ShiftLeft<cmajor::symbols::UShortValue>, ShiftLeft<cmajor::symbols::IntValue>, ShiftLeft<cmajor::symbols::UIntValue>,
    ShiftLeft<cmajor::symbols::LongValue>, ShiftLeft<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename T>
struct shiftRightFun
{
    T operator()(const T& left, const T& right) const
    {
        return left >> right;
    }
};

template<typename ValueT>
cmajor::symbols::Value* ShiftRight(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, shiftRightFun<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun shiftRight[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, ShiftRight<cmajor::symbols::SByteValue>, ShiftRight<cmajor::symbols::ByteValue>, ShiftRight<cmajor::symbols::ShortValue>, ShiftRight<cmajor::symbols::UShortValue>, ShiftRight<cmajor::symbols::IntValue>, ShiftRight<cmajor::symbols::UIntValue>,
    ShiftRight<cmajor::symbols::LongValue>, ShiftRight<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Add(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::plus<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun add[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Add<cmajor::symbols::SByteValue>, Add<cmajor::symbols::ByteValue>, Add<cmajor::symbols::ShortValue>, Add<cmajor::symbols::UShortValue>, Add<cmajor::symbols::IntValue>, Add<cmajor::symbols::UIntValue>,
    Add<cmajor::symbols::LongValue>, Add<cmajor::symbols::ULongValue>, Add<cmajor::symbols::FloatValue>, Add<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Sub(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::minus<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun sub[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Sub<cmajor::symbols::SByteValue>, Sub<cmajor::symbols::ByteValue>, Sub<cmajor::symbols::ShortValue>, Sub<cmajor::symbols::UShortValue>, Sub<cmajor::symbols::IntValue>, Sub<cmajor::symbols::UIntValue>,
    Sub<cmajor::symbols::LongValue>, Sub<cmajor::symbols::ULongValue>, Sub<cmajor::symbols::FloatValue>, Sub<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Mul(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::multiplies<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun mul[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Mul<cmajor::symbols::SByteValue>, Mul<cmajor::symbols::ByteValue>, Mul<cmajor::symbols::ShortValue>, Mul<cmajor::symbols::UShortValue>, Mul<cmajor::symbols::IntValue>, Mul<cmajor::symbols::UIntValue>,
    Mul<cmajor::symbols::LongValue>, Mul<cmajor::symbols::ULongValue>, Mul<cmajor::symbols::FloatValue>, Mul<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Div(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::divides<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun div[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Div<cmajor::symbols::SByteValue>, Div<cmajor::symbols::ByteValue>, Div<cmajor::symbols::ShortValue>, Div<cmajor::symbols::UShortValue>, Div<cmajor::symbols::IntValue>, Div<cmajor::symbols::UIntValue>,
    Div<cmajor::symbols::LongValue>, Div<cmajor::symbols::ULongValue>, Div<cmajor::symbols::FloatValue>, Div<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Rem(cmajor::symbols::Value* left, cmajor::symbols::Value* right, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return BinaryEvaluate<ValueT>(left, right, std::modulus<typename ValueT::OperandType>(), sourcePos, moduleId);
}

BinaryOperatorFun rem[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Rem<cmajor::symbols::SByteValue>, Rem<cmajor::symbols::ByteValue>, Rem<cmajor::symbols::ShortValue>, Rem<cmajor::symbols::UShortValue>, Rem<cmajor::symbols::IntValue>, Rem<cmajor::symbols::UIntValue>,
    Rem<cmajor::symbols::LongValue>, Rem<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* Not(cmajor::symbols::Value* subject, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return UnaryEvaluate<ValueT>(subject, std::logical_not<typename ValueT::OperandType>(), sourcePos, moduleId);
}

UnaryOperatorFun logicalNot[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, Not<cmajor::symbols::BoolValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename T>
struct Identity
{
    const T& operator()(const T& value) const
    {
        return value;
    }
};

template<typename ValueT>
cmajor::symbols::Value* UnaryPlus(cmajor::symbols::Value* subject, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return UnaryEvaluate<ValueT>(subject, Identity<typename ValueT::OperandType>(), sourcePos, moduleId);
}

UnaryOperatorFun unaryPlus[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, UnaryPlus<cmajor::symbols::SByteValue>, UnaryPlus<cmajor::symbols::ByteValue>, UnaryPlus<cmajor::symbols::ShortValue>, UnaryPlus<cmajor::symbols::UShortValue>, UnaryPlus<cmajor::symbols::IntValue>, UnaryPlus<cmajor::symbols::UIntValue>,
    UnaryPlus<cmajor::symbols::LongValue>, UnaryPlus<cmajor::symbols::ULongValue>, UnaryPlus<cmajor::symbols::FloatValue>, UnaryPlus<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template<typename ValueT>
cmajor::symbols::Value* UnaryMinus(cmajor::symbols::Value* subject, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return UnaryEvaluate<ValueT>(subject, std::negate<typename ValueT::OperandType>(), sourcePos, moduleId);
}

UnaryOperatorFun unaryMinus[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, UnaryMinus<cmajor::symbols::SByteValue>, UnaryMinus<cmajor::symbols::ByteValue>, UnaryMinus<cmajor::symbols::ShortValue>, UnaryMinus<cmajor::symbols::UShortValue>, UnaryMinus<cmajor::symbols::IntValue>, NotSupported,
    UnaryMinus<cmajor::symbols::LongValue>, NotSupported, UnaryMinus<cmajor::symbols::FloatValue>, UnaryMinus<cmajor::symbols::DoubleValue>, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

template <typename T>
struct BitNot
{
    T operator()(const T& x) const
    {
        return ~x;
    }
};

template<typename ValueT>
cmajor::symbols::Value* Complement(cmajor::symbols::Value* subject, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, bool dontThrow)
{
    return UnaryEvaluate<ValueT>(subject, BitNot<typename ValueT::OperandType>(), sourcePos, moduleId);
}

UnaryOperatorFun complement[uint8_t(cmajor::symbols::ValueType::maxValue)] =
{
    NotSupported, NotSupported, Complement<cmajor::symbols::SByteValue>, Complement<cmajor::symbols::ByteValue>, Complement<cmajor::symbols::ShortValue>, Complement<cmajor::symbols::UShortValue>, Complement<cmajor::symbols::IntValue>, Complement<cmajor::symbols::UIntValue>,
    Complement<cmajor::symbols::LongValue>, Complement<cmajor::symbols::ULongValue>, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported,
    NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported, NotSupported
};

enum class Operator
{
    add, sub, comparison, other
};

class Evaluator : public cmajor::ast::Visitor
{
public:
    Evaluator(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_, cmajor::symbols::TypeSymbol* targetType_, cmajor::symbols::ValueType targetValueType_, bool cast_, bool dontThrow_, BoundFunction* currentFunction_, const soul::ast::SourcePos& span_,
        const util::uuid& moduleId_);
    bool Error() const { return error; }
    std::unique_ptr<cmajor::symbols::Value> GetValue();

    void Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode) override;
    void Visit(cmajor::ast::AliasNode& aliasNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::ClassNode& classNode) override;
    void Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode) override;
    void Visit(cmajor::ast::ConstructorNode& constructorNode) override;
    void Visit(cmajor::ast::DestructorNode& destructorNode) override;
    void Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode) override;
    void Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode) override;
    void Visit(cmajor::ast::MemberVariableNode& memberVariableNode) override;
    void Visit(cmajor::ast::InterfaceNode& interfaceNode) override;
    void Visit(cmajor::ast::DelegateNode& delegateNode) override;
    void Visit(cmajor::ast::ClassDelegateNode& classDelegateNode) override;

    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::ReturnStatementNode& returnStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::BreakStatementNode& breakStatementNode) override;
    void Visit(cmajor::ast::ContinueStatementNode& continueStatementNode) override;
    void Visit(cmajor::ast::GotoStatementNode& gotoStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode) override;
    void Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode) override;
    void Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode) override;
    void Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode) override;
    void Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode) override;
    void Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode) override;
    void Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode) override;
    void Visit(cmajor::ast::ThrowStatementNode& throwStatementNode) override;
    void Visit(cmajor::ast::TryStatementNode& tryStatementNode) override;
    void Visit(cmajor::ast::CatchNode& catchNode) override;
    void Visit(cmajor::ast::AssertStatementNode& assertStatementNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode) override;
    void Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode) override;
    void Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode) override;

    void Visit(cmajor::ast::BoolNode& boolNode) override;
    void Visit(cmajor::ast::SByteNode& sbyteNode) override;
    void Visit(cmajor::ast::ByteNode& byteNode) override;
    void Visit(cmajor::ast::ShortNode& shortNode) override;
    void Visit(cmajor::ast::UShortNode& ushortNode) override;
    void Visit(cmajor::ast::IntNode& intNode) override;
    void Visit(cmajor::ast::UIntNode& uintNode) override;
    void Visit(cmajor::ast::LongNode& longNode) override;
    void Visit(cmajor::ast::ULongNode& ulongNode) override;
    void Visit(cmajor::ast::FloatNode& floatNode) override;
    void Visit(cmajor::ast::DoubleNode& doubleNode) override;
    void Visit(cmajor::ast::CharNode& charNode) override;
    void Visit(cmajor::ast::WCharNode& wcharNode) override;
    void Visit(cmajor::ast::UCharNode& ucharNode) override;
    void Visit(cmajor::ast::VoidNode& voidNode) override;

    void Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode) override;
    void Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode) override;
    void Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode) override;
    void Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode) override;
    void Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode) override;
    void Visit(cmajor::ast::IntLiteralNode& intLiteralNode) override;
    void Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode) override;
    void Visit(cmajor::ast::LongLiteralNode& longLiteralNode) override;
    void Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode) override;
    void Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode) override;
    void Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode) override;
    void Visit(cmajor::ast::CharLiteralNode& charLiteralNode) override;
    void Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode) override;
    void Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode) override;
    void Visit(cmajor::ast::StringLiteralNode& stringLiteralNode) override;
    void Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode) override;
    void Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode) override;
    void Visit(cmajor::ast::NullLiteralNode& nullLiteralNode) override;
    void Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode) override;
    void Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode) override;

    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;

    void Visit(cmajor::ast::DotNode& dotNode) override;
    void Visit(cmajor::ast::ArrowNode& arrowNode) override;
    void Visit(cmajor::ast::EquivalenceNode& equivalenceNode) override;
    void Visit(cmajor::ast::ImplicationNode& implicationNode) override;
    void Visit(cmajor::ast::DisjunctionNode& disjunctionNode) override;
    void Visit(cmajor::ast::ConjunctionNode& conjunctionNode) override;
    void Visit(cmajor::ast::BitOrNode& bitOrNode) override;
    void Visit(cmajor::ast::BitXorNode& bitXorNode) override;
    void Visit(cmajor::ast::BitAndNode& bitAndNode) override;
    void Visit(cmajor::ast::EqualNode& equalNode) override;
    void Visit(cmajor::ast::NotEqualNode& notEqualNode) override;
    void Visit(cmajor::ast::LessNode& lessNode) override;
    void Visit(cmajor::ast::GreaterNode& greaterNode) override;
    void Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode) override;
    void Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode) override;
    void Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode) override;
    void Visit(cmajor::ast::ShiftRightNode& shiftRightNode) override;
    void Visit(cmajor::ast::AddNode& addNode) override;
    void Visit(cmajor::ast::SubNode& subNode) override;
    void Visit(cmajor::ast::MulNode& mulNode) override;
    void Visit(cmajor::ast::DivNode& divNode) override;
    void Visit(cmajor::ast::RemNode& remNode) override;
    void Visit(cmajor::ast::NotNode& notNode) override;
    void Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode) override;
    void Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode) override;
    void Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode) override;
    void Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode) override;
    void Visit(cmajor::ast::DerefNode& derefNode) override;
    void Visit(cmajor::ast::AddrOfNode& addrOfNode) override;
    void Visit(cmajor::ast::ComplementNode& complementNode) override;
    void Visit(cmajor::ast::IsNode& isNode) override;
    void Visit(cmajor::ast::AsNode& asNode) override;
    void Visit(cmajor::ast::IndexingNode& indexingNode) override;
    void Visit(cmajor::ast::InvokeNode& invokeNode) override;
    void Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode) override;
    void Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode) override;
    void Visit(cmajor::ast::SizeOfNode& sizeOfNode) override;
    void Visit(cmajor::ast::TypeNameNode& typeNameNode) override;
    void Visit(cmajor::ast::TypeIdNode& typeIdNode) override;
    void Visit(cmajor::ast::CastNode& castNode) override;
    void Visit(cmajor::ast::ConstructNode& constructNode) override;
    void Visit(cmajor::ast::NewNode& newNode) override;
    void Visit(cmajor::ast::ThisNode& thisNode) override;
    void Visit(cmajor::ast::BaseNode& baseNode) override;
    void Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode) override;
private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable* symbolTable;
    cmajor::symbols::Module* module;
    cmajor::symbols::ContainerScope* containerScope;
    cmajor::symbols::ContainerScope* qualifiedScope;
    BoundFunction* currentFunction;
    cmajor::symbols::DeclarationBlock* currentDeclarationBlock;
    cmajor::symbols::FileScope* currentFileScope;
    cmajor::symbols::ClassTypeSymbol* currentClassType;
    bool cast;
    bool dontThrow;
    bool error;
    bool returned;
    bool broke;
    bool continued;
    bool lvalue;
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    std::unique_ptr<cmajor::symbols::Value> value;
    cmajor::symbols::TypeSymbol* targetType;
    cmajor::symbols::ValueType targetValueType;
    VariableValueSymbol* targetValueSymbol;
    std::vector<std::unique_ptr<cmajor::symbols::Value>> argumentValues;
    std::unique_ptr<cmajor::symbols::Value> structureReferenceValue;
    std::vector<cmajor::symbols::TypeSymbol*> templateTypeArguments;
    void EvaluateBinOp(cmajor::ast::BinaryNode& node, BinaryOperatorFun* fun);
    void EvaluateBinOp(cmajor::ast::BinaryNode& node, BinaryOperatorFun* fun, Operator op);
    void EvaluateAdditivePointerOp(const soul::ast::SourcePos& sourcePos, Operator op, const std::unique_ptr<cmajor::symbols::Value>& left, const std::unique_ptr<cmajor::symbols::Value>& right);
    void EvaluateUnaryOp(cmajor::ast::UnaryNode& node, UnaryOperatorFun* fun);
    void EvaluateSymbol(cmajor::symbols::Symbol* symbol, const soul::ast::SourcePos& sourcePos);
    void EvaluateConstantSymbol(cmajor::symbols::ConstantSymbol* constantSymbol, const soul::ast::SourcePos& sourcePos);
    void EvaluateEnumConstantSymbol(cmajor::symbols::EnumConstantSymbol* enumConstantSymbol, const soul::ast::SourcePos& sourcePos);
};

Evaluator::Evaluator(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_, cmajor::symbols::TypeSymbol* targetType_, cmajor::symbols::ValueType targetValueType_, bool cast_, bool dontThrow_, BoundFunction* currentFunction_,
    const soul::ast::SourcePos& span_, const util::uuid& moduleId_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(&boundCompileUnit.GetSymbolTable()), module(&boundCompileUnit.GetModule()),
    containerScope(containerScope_), qualifiedScope(nullptr), cast(cast_), dontThrow(dontThrow_), error(false),
    returned(false), broke(false), continued(false), lvalue(false), currentFunction(currentFunction_), currentDeclarationBlock(nullptr), currentFileScope(nullptr), currentClassType(nullptr),
    sourcePos(span_), moduleId(moduleId_), value(), targetType(targetType_), targetValueType(targetValueType_), targetValueSymbol(nullptr)
{
}

void Evaluator::EvaluateBinOp(cmajor::ast::BinaryNode& node, BinaryOperatorFun* fun)
{
    EvaluateBinOp(node, fun, Operator::other);
}

void Evaluator::EvaluateBinOp(cmajor::ast::BinaryNode& node, BinaryOperatorFun* fun, Operator op)
{
    node.Left()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, node.GetSourcePos(), node.ModuleId());
        }
    }
    std::unique_ptr<cmajor::symbols::Value> left(value.release());
    node.Right()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, node.GetSourcePos(), node.ModuleId());
        }
    }
    std::unique_ptr<cmajor::symbols::Value> right(value.release());
    if ((op == Operator::add || op == Operator::sub) && (left->GetValueType() == cmajor::symbols::ValueType::pointerValue || right->GetValueType() == cmajor::symbols::ValueType::pointerValue))
    {
        EvaluateAdditivePointerOp(node.GetSourcePos(), op, left, right);
        return;
    }
    if (op == Operator::comparison && left->GetValueType() == cmajor::symbols::ValueType::pointerValue && right->GetValueType() == cmajor::symbols::ValueType::pointerValue)
    {
        cmajor::symbols::PointerValue* leftPtr = static_cast<cmajor::symbols::PointerValue*>(left.get());
        cmajor::symbols::PointerValue* rightPtr = static_cast<cmajor::symbols::PointerValue*>(right.get());
        if (leftPtr->GetValue() != nullptr && rightPtr->GetValue() != nullptr && leftPtr->PointeeType() != rightPtr->PointeeType())
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("incompatible pointer types for comparison", node.GetSourcePos(), node.ModuleId());
            }
        }
    }
    cmajor::symbols::ValueType leftType = left->GetValueType();
    cmajor::symbols::ValueType rightType = right->GetValueType();
    cmajor::symbols::ValueType commonType = CommonType(leftType, rightType);
    cmajor::symbols::ValueType operationType = commonType;
    if (targetValueType > operationType)
    {
        operationType = targetValueType;
    }
    cmajor::symbols::TypeSymbol* type = GetTypeFor(operationType, symbolTable);
    if (!type)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("conversion from " + ValueTypeStr(leftType) + " to " + ValueTypeStr(operationType) + " is not valid", sourcePos, moduleId);
        }
    }
    std::unique_ptr<cmajor::symbols::Value> leftConverted(left->As(type, cast, node.GetSourcePos(), node.ModuleId(), dontThrow));
    std::unique_ptr<cmajor::symbols::Value> rightConverted(right->As(type, cast, node.GetSourcePos(), node.ModuleId(), dontThrow));
    if (dontThrow)
    {
        if (!leftConverted || !rightConverted)
        {
            error = true;
            return;
        }
    }
    BinaryOperatorFun operation = fun[uint8_t(operationType)];
    value.reset(operation(leftConverted.get(), rightConverted.get(), node.GetSourcePos(), node.ModuleId(), dontThrow));
}

void Evaluator::EvaluateAdditivePointerOp(const soul::ast::SourcePos& sourcePos, Operator op, const std::unique_ptr<cmajor::symbols::Value>& left, const std::unique_ptr<cmajor::symbols::Value>& right)
{
    if (op == Operator::add)
    {
        if (left->GetValueType() == cmajor::symbols::ValueType::pointerValue)
        {
            std::unique_ptr<cmajor::symbols::Value> rightConverted(right->As(symbolTable->GetTypeByName(U"long"), cast, sourcePos, moduleId, dontThrow));
            if (dontThrow)
            {
                if (!rightConverted)
                {
                    error = true;
                    return;
                }
            }
            int64_t offset = static_cast<cmajor::symbols::LongValue*>(rightConverted.get())->GetValue();
            cmajor::symbols::PointerValue* leftPointerValue = static_cast<cmajor::symbols::PointerValue*>(left.get());
            value.reset(leftPointerValue->Add(offset));
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
                }
            }
        }
        else if (right->GetValueType() == cmajor::symbols::ValueType::pointerValue)
        {
            std::unique_ptr<cmajor::symbols::Value> leftConverted(right->As(symbolTable->GetTypeByName(U"long"), cast, sourcePos, moduleId, dontThrow));
            if (dontThrow)
            {
                if (!leftConverted)
                {
                    error = true;
                    return;
                }
            }
            int64_t offset = static_cast<cmajor::symbols::LongValue*>(leftConverted.get())->GetValue();
            cmajor::symbols::PointerValue* rightPointerValue = static_cast<cmajor::symbols::PointerValue*>(right.get());
            value.reset(rightPointerValue->Add(offset));
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
                }
            }
        }
        else
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
            }
        }
    }
    else if (op == Operator::sub)
    {
        if (left->GetValueType() == cmajor::symbols::ValueType::pointerValue && right->GetValueType() != cmajor::symbols::ValueType::pointerValue)
        {
            std::unique_ptr<cmajor::symbols::Value> rightConverted(right->As(symbolTable->GetTypeByName(U"long"), cast, sourcePos, moduleId, dontThrow));
            if (dontThrow)
            {
                if (!rightConverted)
                {
                    error = true;
                    return;
                }
            }
            int64_t offset = static_cast<cmajor::symbols::LongValue*>(rightConverted.get())->GetValue();
            cmajor::symbols::PointerValue* leftPointerValue = static_cast<cmajor::symbols::PointerValue*>(left.get());
            value.reset(leftPointerValue->Sub(offset));
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
                }
            }
        }
        else if (left->GetValueType() == cmajor::symbols::ValueType::pointerValue && right->GetValueType() == cmajor::symbols::ValueType::pointerValue)
        {
            cmajor::symbols::PointerValue* leftPointerValue = static_cast<cmajor::symbols::PointerValue*>(left.get());
            cmajor::symbols::PointerValue* rightPointerValue = static_cast<cmajor::symbols::PointerValue*>(right.get());
            if (leftPointerValue->PointeeType() != rightPointerValue->PointeeType())
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("incompatible pointer operands", sourcePos, moduleId);
                }
            }
            value.reset(leftPointerValue->Sub(rightPointerValue->GetValue()));
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
                }
            }
        }
        else
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("invalid pointer operands", sourcePos, moduleId);
            }
        }
    }
}

void Evaluator::EvaluateUnaryOp(cmajor::ast::UnaryNode& node, UnaryOperatorFun* fun)
{
    node.Subject()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, node.GetSourcePos(), node.ModuleId());
        }
    }
    std::unique_ptr<cmajor::symbols::Value> subject(value.release());
    cmajor::symbols::ValueType subjectType = subject->GetValueType();
    cmajor::symbols::ValueType operationType = subjectType;
    if (targetValueType > operationType)
    {
        operationType = targetValueType;
    }
    cmajor::symbols::TypeSymbol* type = GetTypeFor(operationType, symbolTable);
    if (!type)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("conversion from " + ValueTypeStr(subjectType) + " to " + ValueTypeStr(operationType) + " is not valid", sourcePos, moduleId);
        }
    }
    std::unique_ptr<cmajor::symbols::Value> subjectConverted(subject->As(type, cast, node.GetSourcePos(), node.ModuleId(), dontThrow));
    if (dontThrow)
    {
        if (!subjectConverted)
        {
            error = true;
            return;
        }
    }
    UnaryOperatorFun operation = fun[uint8_t(operationType)];
    value.reset(operation(subjectConverted.get(), node.GetSourcePos(), node.ModuleId(), dontThrow));
}

std::unique_ptr<cmajor::symbols::Value> Evaluator::GetValue()
{
    if (error)
    {
        return std::unique_ptr<cmajor::symbols::Value>();
    }
    return std::move(value);
}

void Evaluator::Visit(cmajor::ast::FunctionNode& functionNode)
{
    bool fileScopeAdded = false;
    cmajor::symbols::Symbol* symbol = symbolTable->GetSymbol(&functionNode);
    if (symbol->IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
        int n = functionSymbol->UsingNodes().Count();
        if (n > 0)
        {
            cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
            cmajor::symbols::FileScope* prevFileScope = currentFileScope;
            currentFileScope = fileScope;
            boundCompileUnit.AddFileScope(fileScope);
            fileScopeAdded = true;
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* usingNode = functionSymbol->UsingNodes()[i];
                usingNode->Accept(*this);
            }
            currentFileScope = prevFileScope;
        }
    }
    bool prevReturned = returned;
    cmajor::symbols::DeclarationBlock* prevDeclarationBlock = currentDeclarationBlock;
    cmajor::symbols::DeclarationBlock declarationBlock(sourcePos, moduleId, U"functionBlock");
    currentDeclarationBlock = &declarationBlock;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = symbol->GetContainerScope();
    declarationBlock.GetContainerScope()->SetParentScope(containerScope);
    containerScope = declarationBlock.GetContainerScope();
    int nt = functionNode.TemplateParameters().Count();
    if (nt != templateTypeArguments.size())
    {
        if (dontThrow)
        {
            containerScope = prevContainerScope;
            currentDeclarationBlock = prevDeclarationBlock;
            returned = prevReturned;
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("wrong number of function template type arguments", sourcePos, moduleId);
        }
    }
    for (int i = 0; i < nt; ++i)
    {
        cmajor::ast::TemplateParameterNode* templateParameterNode = functionNode.TemplateParameters()[i];
        cmajor::symbols::TypeSymbol* templateTypeArgument = templateTypeArguments[i];
        cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameter = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, templateParameterNode->Id()->Str());
        boundTemplateParameter->SetType(templateTypeArgument);
        declarationBlock.AddMember(boundTemplateParameter);
    }
    int n = functionNode.Parameters().Count();
    if (n != argumentValues.size())
    {
        if (dontThrow)
        {
            containerScope = prevContainerScope;
            currentDeclarationBlock = prevDeclarationBlock;
            returned = prevReturned;
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("wrong number of function arguments", sourcePos, moduleId);
        }
    }
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<cmajor::symbols::Value> argumentValue = std::move(argumentValues[i]);
        cmajor::symbols::TypeSymbol* argumentType = argumentValue->GetType(symbolTable);
        cmajor::ast::ParameterNode* parameterNode = functionNode.Parameters()[i];
        VariableValueSymbol* variableValueSymbol = new VariableValueSymbol(parameterNode->GetSourcePos(), parameterNode->ModuleId(), parameterNode->Id()->Str(), std::move(argumentValue));
        variableValueSymbol->SetType(argumentType);
        declarationBlock.AddMember(variableValueSymbol);
    }
    functionNode.Body()->Accept(*this);
    containerScope = prevContainerScope;
    currentDeclarationBlock = prevDeclarationBlock;
    returned = prevReturned;
    if (fileScopeAdded)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
}

void Evaluator::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    bool fileScopeAdded = false;
    cmajor::symbols::Symbol* symbol = symbolTable->GetSymbol(&constructorNode);
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol->Parent());
    cmajor::symbols::ClassTypeSymbol* prevClassType = currentClassType;
    currentClassType = classType;
    if (symbol->IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
        int n = functionSymbol->UsingNodes().Count();
        if (n > 0)
        {
            cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
            cmajor::symbols::FileScope* prevFileScope = currentFileScope;
            currentFileScope = fileScope;
            boundCompileUnit.AddFileScope(fileScope);
            fileScopeAdded = true;
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* usingNode = functionSymbol->UsingNodes()[i];
                usingNode->Accept(*this);
            }
            currentFileScope = prevFileScope;
        }
    }
    bool prevReturned = returned;
    cmajor::symbols::DeclarationBlock* prevDeclarationBlock = currentDeclarationBlock;
    cmajor::symbols::DeclarationBlock declarationBlock(sourcePos, moduleId, U"constructorBlock");
    currentDeclarationBlock = &declarationBlock;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = symbol->GetContainerScope();
    declarationBlock.GetContainerScope()->SetParentScope(containerScope);
    containerScope = declarationBlock.GetContainerScope();
    int n = constructorNode.Parameters().Count();
    if (n != argumentValues.size())
    {
        if (dontThrow)
        {
            containerScope = prevContainerScope;
            currentDeclarationBlock = prevDeclarationBlock;
            returned = prevReturned;
            currentClassType = prevClassType;
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("wrong number of constructor arguments", sourcePos, moduleId);
        }
    }
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<cmajor::symbols::Value> argumentValue = std::move(argumentValues[i]);
        cmajor::symbols::TypeSymbol* argumentType = argumentValue->GetType(symbolTable);
        cmajor::ast::ParameterNode* parameterNode = constructorNode.Parameters()[i];
        VariableValueSymbol* variableValueSymbol = new VariableValueSymbol(parameterNode->GetSourcePos(), parameterNode->ModuleId(), parameterNode->Id()->Str(), std::move(argumentValue));
        variableValueSymbol->SetType(argumentType);
        declarationBlock.AddMember(variableValueSymbol);
    }
    std::unordered_map<std::u32string, cmajor::ast::MemberInitializerNode*> memberInitializerMap;
    int ni = constructorNode.Initializers().Count();
    for (int i = 0; i < ni; ++i)
    {
        cmajor::ast::InitializerNode* initializer = constructorNode.Initializers()[i];
        if (initializer->GetNodeType() == cmajor::ast::NodeType::thisInitializerNode || initializer->GetNodeType() == cmajor::ast::NodeType::baseInitializerNode)
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                currentClassType = prevClassType;
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("this and base initializers not supported for a constexpr constructor", constructorNode.GetSourcePos(), constructorNode.ModuleId());
            }
        }
        else
        {
            cmajor::ast::MemberInitializerNode* memberInitializer = static_cast<cmajor::ast::MemberInitializerNode*>(initializer);
            std::u32string memberName = memberInitializer->MemberId()->Str();
            auto it = memberInitializerMap.find(memberName);
            if (it != memberInitializerMap.cend())
            {
                if (dontThrow)
                {
                    containerScope = prevContainerScope;
                    currentDeclarationBlock = prevDeclarationBlock;
                    returned = prevReturned;
                    currentClassType = prevClassType;
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("already has initializer for member variable '" + util::ToUtf8(memberName) + "'", initializer->GetSourcePos(), 
                        initializer->ModuleId());
                }
            }
            memberInitializerMap[memberName] = memberInitializer;
        }
    }
    std::vector<std::unique_ptr<cmajor::symbols::Value>> memberValues;
    int nm = classType->MemberVariables().size();
    for (int i = 0; i < nm; ++i)
    {
        value.reset();
        cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
        std::vector<std::unique_ptr<cmajor::symbols::Value>> initializerArgumentValues;
        auto it = memberInitializerMap.find(memberVariableSymbol->Name());
        if (it != memberInitializerMap.cend())
        {
            cmajor::ast::MemberInitializerNode* memberInitializer = it->second;
            int na = memberInitializer->Arguments().Count();
            for (int i = 0; i < na; ++i)
            {
                cmajor::ast::Node* argumentNode = memberInitializer->Arguments()[i];
                argumentNode->Accept(*this);
                if (error) return;
                if (!value)
                {
                    if (dontThrow)
                    {
                        containerScope = prevContainerScope;
                        currentDeclarationBlock = prevDeclarationBlock;
                        returned = prevReturned;
                        currentClassType = prevClassType;
                        error = true;
                        return;
                    }
                    else
                    {
                        ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
                    }
                }
                initializerArgumentValues.push_back(std::move(value));
            }
        }
        std::vector<FunctionScopeLookup> lookups;
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> initializerArguments = ValuesToLiterals(initializerArgumentValues, symbolTable, error);
        if (error)
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                currentClassType = prevClassType;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
            }
        }
        initializerArguments.insert(initializerArguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, memberVariableSymbol->GetType()->AddPointer(sourcePos, moduleId))));
        OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
        if (dontThrow)
        {
            flags = flags | OverloadResolutionFlags::dontThrow;
        }
        std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
        std::unique_ptr<cmajor::symbols::Exception> exception;
        std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, initializerArguments, boundCompileUnit, currentFunction,
            constructorNode.GetSourcePos(), constructorNode.ModuleId(), flags, templateArgumentTypes, exception);
        if (!constructorCall)
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                currentClassType = prevClassType;
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
            }
        }
        argumentValues = ArgumentsToValues(constructorCall->Arguments(), error, true, boundCompileUnit);
        if (error)
        {
            if (dontThrow)
            {
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
            }
        }
        cmajor::symbols::FunctionSymbol* constructorSymbol = constructorCall->GetFunctionSymbol();
        if (constructorSymbol->IsCompileTimePrimitiveFunction())
        {
            value = constructorSymbol->ConstructValue(argumentValues, sourcePos, moduleId, nullptr);
            if (!value)
            {
                if (dontThrow)
                {
                    containerScope = prevContainerScope;
                    currentDeclarationBlock = prevDeclarationBlock;
                    returned = prevReturned;
                    currentClassType = prevClassType;
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
                }
            }
        }
        else if (constructorSymbol->IsConstExpr())
        {
            cmajor::ast::FunctionNode* ctorNode = boundCompileUnit.GetFunctionNodeFor(constructorSymbol);
            ctorNode->Accept(*this);
            if (!value)
            {
                if (dontThrow)
                {
                    containerScope = prevContainerScope;
                    currentDeclarationBlock = prevDeclarationBlock;
                    returned = prevReturned;
                    currentClassType = prevClassType;
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, ctorNode->GetSourcePos(), ctorNode->ModuleId());
                }
            }
        }
        else
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                currentClassType = prevClassType;
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructorNode.GetSourcePos(), constructorNode.ModuleId());
            }
        }
        memberValues.push_back(std::move(value));
    }
    constructorNode.Body()->Accept(*this);
    value.reset(new cmajor::symbols::StructuredValue(sourcePos, moduleId, classType, std::move(memberValues)));
    containerScope = prevContainerScope;
    currentDeclarationBlock = prevDeclarationBlock;
    returned = prevReturned;
    currentClassType = prevClassType;
    if (fileScopeAdded)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
}

void Evaluator::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    bool fileScopeAdded = false;
    cmajor::symbols::Symbol* symbol = symbolTable->GetSymbol(&memberFunctionNode);
    if (symbol->IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
        int n = functionSymbol->UsingNodes().Count();
        if (n > 0)
        {
            cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
            cmajor::symbols::FileScope* prevFileScope = currentFileScope;
            currentFileScope = fileScope;
            boundCompileUnit.AddFileScope(fileScope);
            fileScopeAdded = true;
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* usingNode = functionSymbol->UsingNodes()[i];
                usingNode->Accept(*this);
            }
            currentFileScope = prevFileScope;
        }
    }
    bool prevReturned = returned;
    cmajor::symbols::DeclarationBlock* prevDeclarationBlock = currentDeclarationBlock;
    cmajor::symbols::DeclarationBlock declarationBlock(sourcePos, moduleId, U"functionBlock");
    currentDeclarationBlock = &declarationBlock;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = symbol->GetContainerScope();
    declarationBlock.GetContainerScope()->SetParentScope(containerScope);
    containerScope = declarationBlock.GetContainerScope();
    int n = memberFunctionNode.Parameters().Count();
    if (n != argumentValues.size())
    {
        if (dontThrow)
        {
            containerScope = prevContainerScope;
            currentDeclarationBlock = prevDeclarationBlock;
            returned = prevReturned;
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("wrong number of function arguments", memberFunctionNode.GetSourcePos(), memberFunctionNode.ModuleId());
        }
    }
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<cmajor::symbols::Value> argumentValue = std::move(argumentValues[i]);
        cmajor::symbols::TypeSymbol* argumentType = argumentValue->GetType(symbolTable);
        cmajor::ast::ParameterNode* parameterNode = memberFunctionNode.Parameters()[i];
        VariableValueSymbol* variableValueSymbol = new VariableValueSymbol(parameterNode->GetSourcePos(), parameterNode->ModuleId(), parameterNode->Id()->Str(), std::move(argumentValue));
        variableValueSymbol->SetType(argumentType);
        declarationBlock.AddMember(variableValueSymbol);
    }
    if (currentClassType && structureReferenceValue)
    {
        cmajor::symbols::StructuredValue* structuredValue = nullptr;
        if (structureReferenceValue->IsStructuredReferenceValue())
        {
            structuredValue = static_cast<StructuredReferenceValue*>(structureReferenceValue.get())->GetStructuredValue();
        }
        else
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("structured reference value expected", memberFunctionNode.GetSourcePos(), memberFunctionNode.ModuleId());
            }
        }
        int n = currentClassType->MemberVariables().size();
        if (n != structuredValue->Members().size())
        {
            if (dontThrow)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                returned = prevReturned;
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("wrong number of structured value members", memberFunctionNode.GetSourcePos(), memberFunctionNode.ModuleId());
            }
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = currentClassType->MemberVariables()[i];
            cmajor::symbols::Value* memberValue = structuredValue->Members()[i].get();
            cmajor::symbols::ConstantSymbol* constantSymbol = new cmajor::symbols::ConstantSymbol(sourcePos, moduleId, memberVariableSymbol->Name());
            constantSymbol->SetModule(module);
            constantSymbol->SetType(memberVariableSymbol->GetType());
            if (memberValue->GetValueType() == cmajor::symbols::ValueType::arrayValue)
            {
                constantSymbol->SetValue(new ArrayReferenceValue(static_cast<cmajor::symbols::ArrayValue*>(memberValue)));
            }
            else if (memberValue->GetValueType() == cmajor::symbols::ValueType::structuredValue)
            {
                constantSymbol->SetValue(new StructuredReferenceValue(static_cast<cmajor::symbols::StructuredValue*>(memberValue)));
            }
            else
            {
                constantSymbol->SetValue(memberValue->Clone());
            }
            declarationBlock.AddMember(constantSymbol);
        }
    }
    memberFunctionNode.Body()->Accept(*this);
    containerScope = prevContainerScope;
    currentDeclarationBlock = prevDeclarationBlock;
    returned = prevReturned;
    if (fileScopeAdded)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
}

void Evaluator::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conversionFunctionNode.GetSourcePos(), conversionFunctionNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    if (currentFileScope)
    {
        currentFileScope->InstallNamespaceImport(containerScope, &namespaceImportNode);
    }
}

void Evaluator::Visit(cmajor::ast::AliasNode& aliasNode)
{
    if (currentFileScope)
    {
        // currentFileScope->InstallAlias(containerScope, &aliasNode); TODO
    }
}

void Evaluator::Visit(cmajor::ast::ClassNode& classNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, classNode.GetSourcePos(), classNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, staticConstructorNode.GetSourcePos(), staticConstructorNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, destructorNode.GetSourcePos(), destructorNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    memberVariableNode.Id()->Accept(*this);
}

void Evaluator::Visit(cmajor::ast::InterfaceNode& interfaceNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, interfaceNode.GetSourcePos(), interfaceNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DelegateNode& delegateNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, delegateNode.GetSourcePos(), delegateNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ClassDelegateNode& classDelegateNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, classDelegateNode.GetSourcePos(), classDelegateNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    cmajor::symbols::DeclarationBlock* prevDeclarationBlock = currentDeclarationBlock;
    cmajor::symbols::DeclarationBlock declarationBlock(sourcePos, moduleId, U"block");
    currentDeclarationBlock = &declarationBlock;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    declarationBlock.GetContainerScope()->SetParentScope(containerScope);
    containerScope = declarationBlock.GetContainerScope();
    int n = compoundStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = compoundStatementNode.Statements()[i];
        statementNode->Accept(*this);
        if (error || returned || broke || continued)
        {
            currentDeclarationBlock = prevDeclarationBlock;
            containerScope = prevContainerScope;
            return;
        }
    }
    containerScope = prevContainerScope;
    currentDeclarationBlock = prevDeclarationBlock;
}

void Evaluator::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Stmt()->Accept(*this);
}

void Evaluator::Visit(cmajor::ast::ReturnStatementNode& returnStatementNode)
{
    if (returnStatementNode.Expression())
    {
        returnStatementNode.Expression()->Accept(*this);
        if (error) return;
    }
    returned = true;
}

void Evaluator::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    ifStatementNode.Condition()->Accept(*this);
    if (error) return;
    if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
    {
        cmajor::symbols::BoolValue* condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
        if (condition->GetValue())
        {
            ifStatementNode.ThenS()->Accept(*this);
        }
        else if (ifStatementNode.ElseS())
        {
            ifStatementNode.ElseS()->Accept(*this);
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            throw cmajor::symbols::Exception("Boolean expression expected", ifStatementNode.GetSourcePos(), ifStatementNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    bool prevBroke = broke;
    bool prevContinued = continued;
    whileStatementNode.Condition()->Accept(*this);
    if (error)
    {
        broke = prevBroke;
        continued = prevContinued;
        return;
    }
    if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
    {
        cmajor::symbols::BoolValue* condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
        while (condition->GetValue())
        {
            whileStatementNode.Statement()->Accept(*this);
            if (error || returned)
            {
                broke = prevBroke;
                continued = prevContinued;
                return;
            }
            if (broke)
            {
                break;
            }
            if (continued)
            {
                continued = false;
            }
            whileStatementNode.Condition()->Accept(*this);
            if (error)
            {
                broke = prevBroke;
                continued = prevContinued;
                return;
            }
            if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
            {
                condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
            }
            else
            {
                if (dontThrow)
                {
                    broke = prevBroke;
                    continued = prevContinued;
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("Boolean expression expected", whileStatementNode.GetSourcePos(), whileStatementNode.ModuleId());
                }
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            throw cmajor::symbols::Exception("Boolean expression expected", whileStatementNode.GetSourcePos(), whileStatementNode.ModuleId());
        }
    }
    broke = prevBroke;
    continued = prevContinued;
}

void Evaluator::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    bool prevBroke = broke;
    bool prevContinued = continued;
    bool loop = true;
    while (loop)
    {
        doStatementNode.Statement()->Accept(*this);
        if (error || returned)
        {
            broke = prevBroke;
            continued = prevContinued;
            return;
        }
        if (broke)
        {
            break;
        }
        if (continued)
        {
            continued = false;
        }
        doStatementNode.Condition()->Accept(*this);
        if (error)
        {
            broke = prevBroke;
            continued = prevContinued;
            return;
        }
        if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
        {
            cmajor::symbols::BoolValue* condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
            loop = condition->GetValue();
        }
        else
        {
            if (dontThrow)
            {
                broke = prevBroke;
                continued = prevContinued;
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("Boolean expression expected", doStatementNode.GetSourcePos(), doStatementNode.ModuleId());
            }
        }
    }
    broke = prevBroke;
    continued = prevContinued;
}

void Evaluator::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    bool prevBroke = broke;
    bool prevContinued = continued;
    cmajor::symbols::DeclarationBlock* prevDeclarationBlock = currentDeclarationBlock;
    cmajor::symbols::DeclarationBlock declarationBlock(sourcePos, moduleId, U"forBlock");
    currentDeclarationBlock = &declarationBlock;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    declarationBlock.GetContainerScope()->SetParentScope(containerScope);
    containerScope = declarationBlock.GetContainerScope();
    forStatementNode.InitS()->Accept(*this);
    if (error || returned)
    {
        containerScope = prevContainerScope;
        currentDeclarationBlock = prevDeclarationBlock;
        broke = prevBroke;
        continued = prevContinued;
        return;
    }
    forStatementNode.Condition()->Accept(*this);
    if (error)
    {
        containerScope = prevContainerScope;
        currentDeclarationBlock = prevDeclarationBlock;
        broke = prevBroke;
        continued = prevContinued;
        return;
    }
    if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
    {
        cmajor::symbols::BoolValue* condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
        bool loop = condition->GetValue();
        while (loop)
        {
            forStatementNode.ActionS()->Accept(*this);
            if (error || returned)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                broke = prevBroke;
                continued = prevContinued;
                return;
            }
            if (broke)
            {
                break;
            }
            if (continued)
            {
                continued = false;
            }
            forStatementNode.LoopS()->Accept(*this);
            if (error)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                broke = prevBroke;
                continued = prevContinued;
                return;
            }
            forStatementNode.Condition()->Accept(*this);
            if (error)
            {
                containerScope = prevContainerScope;
                currentDeclarationBlock = prevDeclarationBlock;
                broke = prevBroke;
                continued = prevContinued;
                return;
            }
            if (value && value->GetValueType() == cmajor::symbols::ValueType::boolValue)
            {
                cmajor::symbols::BoolValue* condition = static_cast<cmajor::symbols::BoolValue*>(value.get());
                loop = condition->GetValue();
            }
            else
            {
                if (dontThrow)
                {
                    containerScope = prevContainerScope;
                    currentDeclarationBlock = prevDeclarationBlock;
                    broke = prevBroke;
                    continued = prevContinued;
                    error = true;
                    return;
                }
                else
                {
                    throw cmajor::symbols::Exception("Boolean expression expected", forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
                }
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            containerScope = prevContainerScope;
            currentDeclarationBlock = prevDeclarationBlock;
            broke = prevBroke;
            continued = prevContinued;
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("Boolean expression expected", forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
        }
    }
    containerScope = prevContainerScope;
    currentDeclarationBlock = prevDeclarationBlock;
    broke = prevBroke;
    continued = prevContinued;
}

void Evaluator::Visit(cmajor::ast::BreakStatementNode& breakStatementNode)
{
    broke = true;
}

void Evaluator::Visit(cmajor::ast::ContinueStatementNode& continueStatementNode)
{
    continued = true;
}

void Evaluator::Visit(cmajor::ast::GotoStatementNode& gotoStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, gotoStatementNode.GetSourcePos(), gotoStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    if (!currentDeclarationBlock)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("internal error: current declaration block not set", constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
    }
    cmajor::symbols::TypeSymbol* type = ResolveType(constructionStatementNode.TypeExpr(), boundCompileUnit, containerScope);
    std::vector<std::unique_ptr<cmajor::symbols::Value>> values;
    int n = constructionStatementNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* argumentNode = constructionStatementNode.Arguments()[i];
        argumentNode->Accept(*this);
        if (error) return;
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
            }
        }
        values.push_back(std::move(value));
    }
    std::vector<std::unique_ptr<BoundExpression>> arguments = ValuesToLiterals(values, symbolTable, error);
    if (error)
    {
        if (dontThrow)
        {
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
    }
    arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, type->AddPointer(sourcePos, moduleId))));
    std::vector<FunctionScopeLookup> scopeLookups;
    scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::unique_ptr<cmajor::symbols::Exception> exception;
    OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
    if (dontThrow)
    {
        flags = flags | OverloadResolutionFlags::dontThrow;
    }
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, scopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        flags, templateArgumentTypes, exception);
    if (!constructorCall)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
    }
    argumentValues = ArgumentsToValues(constructorCall->Arguments(), error, true, boundCompileUnit);
    if (error)
    {
        if (dontThrow)
        {
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
    }
    cmajor::symbols::FunctionSymbol* constructorSymbol = constructorCall->GetFunctionSymbol();
    if (constructorSymbol->IsCompileTimePrimitiveFunction())
    {
        value = constructorSymbol->ConstructValue(argumentValues, sourcePos, moduleId, nullptr);
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
            }
        }
    }
    else if (constructorSymbol->IsConstExpr())
    {
        cmajor::ast::FunctionNode* ctorNode = boundCompileUnit.GetFunctionNodeFor(constructorSymbol);
        ctorNode->Accept(*this);
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, ctorNode->GetSourcePos(), ctorNode->ModuleId());
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
    }
    VariableValueSymbol* variableValue = new VariableValueSymbol(sourcePos, moduleId, constructionStatementNode.Id()->Str(), std::move(value));
    variableValue->SetType(type);
    currentDeclarationBlock->AddMember(variableValue);
}

void Evaluator::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, deleteStatementNode.GetSourcePos(), deleteStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode)
{
    bool prevLvalue = lvalue;
    lvalue = true;
    VariableValueSymbol* prevTargetValueSymbol = targetValueSymbol;
    assignmentStatementNode.TargetExpr()->Accept(*this);
    VariableValueSymbol* target = targetValueSymbol;
    targetValueSymbol = prevTargetValueSymbol;
    lvalue = prevLvalue;
    assignmentStatementNode.SourceExpr()->Accept(*this);
    std::vector<std::unique_ptr<cmajor::symbols::Value>> values;
    values.push_back(std::move(value));
    std::vector<std::unique_ptr<BoundExpression>> arguments = ValuesToLiterals(values, symbolTable, error);
    if (error)
    {
        if (dontThrow)
        {
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        }
    }
    arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, target->GetType()->AddPointer(sourcePos, moduleId))));
    std::vector<FunctionScopeLookup> scopeLookups;
    scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::unique_ptr<cmajor::symbols::Exception> exception;
    OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
    if (dontThrow)
    {
        flags = flags | OverloadResolutionFlags::dontThrow;
    }
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, scopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        flags, templateArgumentTypes, exception);
    if (!assignmentCall)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        }
    }
    argumentValues = ArgumentsToValues(assignmentCall->Arguments(), error, true, boundCompileUnit);
    if (error)
    {
        if (dontThrow)
        {
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        }
    }
    target->SetValue(argumentValues.front().release());
}

void Evaluator::Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode)
{
    expressionStatementNode.Expression()->Accept(*this);
}

void Evaluator::Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode)
{
}

void Evaluator::Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode)
{
    // todo
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, rangeForStatementNode.GetSourcePos(), rangeForStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    // todo
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, switchStatementNode.GetSourcePos(), switchStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    // todo
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, caseStatementNode.GetSourcePos(), caseStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    // todo
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, defaultStatementNode.GetSourcePos(), defaultStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, gotoCaseStatementNode.GetSourcePos(), gotoCaseStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, gotoDefaultStatementNode.GetSourcePos(), gotoDefaultStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ThrowStatementNode& throwStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, throwStatementNode.GetSourcePos(), throwStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, tryStatementNode.GetSourcePos(), tryStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::CatchNode& catchNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, catchNode.GetSourcePos(), catchNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::AssertStatementNode& assertStatementNode)
{
    assertStatementNode.AssertExpr()->Accept(*this);
    if (error) return;
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId());
        }
    }
    if (value->GetValueType() == cmajor::symbols::ValueType::boolValue)
    {
        cmajor::symbols::BoolValue* boolValue = static_cast<cmajor::symbols::BoolValue*>(value.get());
        if (!boolValue->GetValue())
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("assertion '" + assertStatementNode.AssertExpr()->ToString() + "' failed", sourcePos, moduleId, assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId());
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("assertion expression is not a Boolean-valued expression", sourcePos, moduleId, assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationPartNode.GetSourcePos(), conditionalCompilationPartNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationDisjunctionNode.GetSourcePos(), conditionalCompilationDisjunctionNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationConjunctionNode.GetSourcePos(), conditionalCompilationConjunctionNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationNotNode.GetSourcePos(), conditionalCompilationNotNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationPrimaryNode.GetSourcePos(), conditionalCompilationPrimaryNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    if (dontThrow)
    {
        error = true;
        return;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, conditionalCompilationStatementNode.GetSourcePos(), conditionalCompilationStatementNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::BoolNode& boolNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, boolNode.GetSourcePos(), boolNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, sbyteNode.GetSourcePos(), sbyteNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ByteNode& byteNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, byteNode.GetSourcePos(), byteNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ShortNode& shortNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, shortNode.GetSourcePos(), shortNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::UShortNode& ushortNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, ushortNode.GetSourcePos(), ushortNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::IntNode& intNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, intNode.GetSourcePos(), intNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::UIntNode& uintNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, uintNode.GetSourcePos(), uintNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::LongNode& longNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, longNode.GetSourcePos(), longNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ULongNode& ulongNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, ulongNode.GetSourcePos(), ulongNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::FloatNode& floatNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, floatNode.GetSourcePos(), floatNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, doubleNode.GetSourcePos(), doubleNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::CharNode& charNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, charNode.GetSourcePos(), charNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::WCharNode& wcharNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, wcharNode.GetSourcePos(), wcharNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::UCharNode& ucharNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, ucharNode.GetSourcePos(), ucharNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::VoidNode& voidNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, voidNode.GetSourcePos(), voidNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode)
{
    value.reset(new cmajor::symbols::BoolValue(booleanLiteralNode.GetSourcePos(), booleanLiteralNode.ModuleId(), booleanLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode)
{
    value.reset(new cmajor::symbols::SByteValue(sbyteLiteralNode.GetSourcePos(), sbyteLiteralNode.ModuleId(), sbyteLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode)
{
    value.reset(new cmajor::symbols::ByteValue(byteLiteralNode.GetSourcePos(), byteLiteralNode.ModuleId(), byteLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode)
{
    value.reset(new cmajor::symbols::ShortValue(shortLiteralNode.GetSourcePos(), shortLiteralNode.ModuleId(), shortLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode)
{
    value.reset(new cmajor::symbols::UShortValue(ushortLiteralNode.GetSourcePos(), ushortLiteralNode.ModuleId(), ushortLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::IntLiteralNode& intLiteralNode)
{
    value.reset(new cmajor::symbols::IntValue(intLiteralNode.GetSourcePos(), intLiteralNode.ModuleId(), intLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode)
{
    value.reset(new cmajor::symbols::UIntValue(uintLiteralNode.GetSourcePos(), uintLiteralNode.ModuleId(), uintLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::LongLiteralNode& longLiteralNode)
{
    value.reset(new cmajor::symbols::LongValue(longLiteralNode.GetSourcePos(), longLiteralNode.ModuleId(), longLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode)
{
    value.reset(new cmajor::symbols::ULongValue(ulongLiteralNode.GetSourcePos(), ulongLiteralNode.ModuleId(), ulongLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode)
{
    value.reset(new cmajor::symbols::FloatValue(floatLiteralNode.GetSourcePos(), floatLiteralNode.ModuleId(), floatLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode)
{
    value.reset(new cmajor::symbols::DoubleValue(doubleLiteralNode.GetSourcePos(), doubleLiteralNode.ModuleId(), doubleLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::CharLiteralNode& charLiteralNode)
{
    value.reset(new cmajor::symbols::CharValue(charLiteralNode.GetSourcePos(), charLiteralNode.ModuleId(), charLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode)
{
    value.reset(new cmajor::symbols::WCharValue(wcharLiteralNode.GetSourcePos(), wcharLiteralNode.ModuleId(), wcharLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode)
{
    value.reset(new cmajor::symbols::UCharValue(ucharLiteralNode.GetSourcePos(), ucharLiteralNode.ModuleId(), ucharLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::StringLiteralNode& stringLiteralNode)
{
    value.reset(new cmajor::symbols::StringValue(stringLiteralNode.GetSourcePos(), stringLiteralNode.ModuleId(), boundCompileUnit.Install(stringLiteralNode.Value()), stringLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode)
{
    value.reset(new cmajor::symbols::WStringValue(wstringLiteralNode.GetSourcePos(), wstringLiteralNode.ModuleId(), boundCompileUnit.Install(wstringLiteralNode.Value()), wstringLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode)
{
    value.reset(new cmajor::symbols::UStringValue(ustringLiteralNode.GetSourcePos(), ustringLiteralNode.ModuleId(), boundCompileUnit.Install(ustringLiteralNode.Value()), ustringLiteralNode.Value()));
}

void Evaluator::Visit(cmajor::ast::NullLiteralNode& nullLiteralNode)
{
    value.reset(new cmajor::symbols::NullValue(nullLiteralNode.GetSourcePos(), nullLiteralNode.ModuleId(), symbolTable->GetTypeByName(U"@nullptr_type")));
}

void Evaluator::Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode)
{
    if (targetValueType != cmajor::symbols::ValueType::arrayValue)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("array type expected", sourcePos, moduleId);
        }
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(targetType);
    cmajor::symbols::TypeSymbol* elementType = arrayType->ElementType();
    std::vector<std::unique_ptr<cmajor::symbols::Value>> elementValues;
    int n = arrayLiteralNode.Values().Count();
    if (arrayType->Size() != -1 && arrayType->Size() != n)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("invalid length for array literal of type '" + util::ToUtf8(arrayType->FullName()) + "'", arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId());
        }
    }
    for (int i = 0; i < n; ++i)
    {
        value = Evaluate(arrayLiteralNode.Values()[i], elementType, containerScope, boundCompileUnit, dontThrow, currentFunction, arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId());
        if (error)
        {
            if (dontThrow)
            {
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId());
            }
        }
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId());
            }
        }
        elementValues.push_back(std::move(value));
    }
    if (arrayType->Size() == -1)
    {
        arrayType = symbolTable->MakeArrayType(arrayType->ElementType(), n, arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId());
    }
    value.reset(new cmajor::symbols::ArrayValue(arrayLiteralNode.GetSourcePos(), arrayLiteralNode.ModuleId(), arrayType, std::move(elementValues)));
}

void Evaluator::Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode)
{
    if (targetValueType != cmajor::symbols::ValueType::structuredValue)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("class type expected", sourcePos, moduleId);
        }
    }
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(targetType);
    if (!currentFunction)
    {
        if (classType->IsProject() && !classType->IsBound())
        {
            cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(classType);
            if (node)
            {
                TypeBinder typeBinder(boundCompileUnit);
                typeBinder.SetContainerScope(containerScope);
                node->Accept(typeBinder);
            }
        }
    }
    if (classType->IsLiteralClassType())
    {
        std::vector<std::unique_ptr<cmajor::symbols::Value>> memberValues;
        int n = structuredLiteralNode.Members().Count();
        if (classType->MemberVariables().size() != n)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("wrong number of members variables for class literal of type '" + util::ToUtf8(classType->FullName()) + "'", structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            }
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TypeSymbol* memberType = classType->MemberVariables()[i]->GetType();
            value = Evaluate(structuredLiteralNode.Members()[i], memberType, containerScope, boundCompileUnit, dontThrow, currentFunction, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            if (error)
            {
                if (dontThrow)
                {
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
                }
            }
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
                }
            }
            memberValues.push_back(std::move(value));
        }
        std::vector<std::unique_ptr<BoundExpression>> arguments = ValuesToLiterals(memberValues, symbolTable, error);
        if (error)
        {
            if (dontThrow)
            {
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            }
        }
        arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, classType->AddPointer(sourcePos, moduleId))));
        std::vector<FunctionScopeLookup> scopeLookups;
        scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, classType->ClassOrNsScope()));
        scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        scopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::unique_ptr<cmajor::symbols::Exception> exception;
        OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
        if (dontThrow)
        {
            flags = flags | OverloadResolutionFlags::dontThrow;
        }
        std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
        std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, scopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
            flags, templateArgumentTypes, exception);
        if (!constructorCall)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            }
        }
        argumentValues = ArgumentsToValues(constructorCall->Arguments(), error, true, boundCompileUnit);
        if (error)
        {
            if (dontThrow)
            {
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            }
        }
        cmajor::symbols::FunctionSymbol* constructorSymbol = constructorCall->GetFunctionSymbol();
        if (constructorSymbol->IsConstExpr())
        {
            cmajor::ast::FunctionNode* constructorNode = boundCompileUnit.GetFunctionNodeFor(constructorSymbol);
            constructorNode->Accept(*this);
        }
        else
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("class '" + util::ToUtf8(classType->FullName()) + "' is not a literal class ", structuredLiteralNode.GetSourcePos(), structuredLiteralNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    std::u32string name = identifierNode.Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol) break;
        }
    }
    if (symbol)
    {
        qualifiedScope = nullptr;
        if (name.find('.') != std::u32string::npos)
        {
            qualifiedScope = symbol->Parent()->GetContainerScope();
        }
        EvaluateSymbol(symbol, identifierNode.GetSourcePos());
        if (error)
        {
            return;
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found", identifierNode.GetSourcePos(), identifierNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    templateIdNode.Primary()->Accept(*this);
    if (error) return;
    if (value && value->IsFunctionGroupValue())
    {
        FunctionGroupValue* functionGroupValue = static_cast<FunctionGroupValue*>(value.get());
        cmajor::symbols::FunctionGroupSymbol* functionGroup = functionGroupValue->FunctionGroup();
        std::vector<cmajor::symbols::TypeSymbol*> templateTypeArguments;
        int n = templateIdNode.TemplateArguments().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* templateArgumentNode = templateIdNode.TemplateArguments()[i];
            cmajor::symbols::TypeSymbol* templateTypeArgument = ResolveType(templateArgumentNode, boundCompileUnit, containerScope);
            templateTypeArguments.push_back(templateTypeArgument);
        }
        functionGroupValue->SetTemplateTypeArguments(std::move(templateTypeArguments));
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
        }
    }
}

void Evaluator::EvaluateSymbol(cmajor::symbols::Symbol* symbol, const soul::ast::SourcePos& sourcePos)
{
    if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::constantSymbol)
    {
        cmajor::symbols::ConstantSymbol* constantSymbol = static_cast<cmajor::symbols::ConstantSymbol*>(symbol);
        EvaluateConstantSymbol(constantSymbol, sourcePos);
    }
    else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::enumConstantSymbol)
    {
        cmajor::symbols::EnumConstantSymbol* enumConstantSymbol = static_cast<cmajor::symbols::EnumConstantSymbol*>(symbol);
        EvaluateEnumConstantSymbol(enumConstantSymbol, sourcePos);
    }
    else if (symbol->IsContainerSymbol())
    {
        cmajor::symbols::ContainerSymbol* containerSymbol = static_cast<cmajor::symbols::ContainerSymbol*>(symbol);
        value.reset(new ScopedValue(sourcePos, moduleId, containerSymbol));
    }
    else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupSymbol)
    {
        cmajor::symbols::FunctionGroupSymbol* functionGroup = static_cast<cmajor::symbols::FunctionGroupSymbol*>(symbol);
        value.reset(new FunctionGroupValue(functionGroup, qualifiedScope));
    }
    else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::variableValueSymbol)
    {
        VariableValueSymbol* variableValueSymbol = static_cast<VariableValueSymbol*>(symbol);
        if (lvalue)
        {
            targetValueSymbol = variableValueSymbol;
        }
        else
        {
            value.reset(variableValueSymbol->GetValue()->Clone());
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId);
        }
    }
}

void Evaluator::EvaluateConstantSymbol(cmajor::symbols::ConstantSymbol* constantSymbol, const soul::ast::SourcePos& sourcePos)
{
    if (constantSymbol->Evaluating())
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        throw cmajor::symbols::Exception("cyclic depenency detected", sourcePos, moduleId);
    }
    cmajor::symbols::Value* constantValue = constantSymbol->GetValue();
    if (constantValue)
    {
        switch (constantValue->GetValueType())
        {
        case cmajor::symbols::ValueType::arrayValue:
        {
            value.reset(new ArrayReferenceValue(static_cast<cmajor::symbols::ArrayValue*>(constantValue)));
            break;
        }
        case cmajor::symbols::ValueType::structuredValue:
        {
            value.reset(new StructuredReferenceValue(static_cast<cmajor::symbols::StructuredValue*>(constantValue)));
            break;
        }
        case cmajor::symbols::ValueType::stringValue: case cmajor::symbols::ValueType::wstringValue: case cmajor::symbols::ValueType::ustringValue:
        {
            value.reset(new StringReferenceValue(constantValue));
            break;
        }
        default:
        {
            value.reset(constantValue->Clone());
            break;
        }
        }
    }
    else
    {
        cmajor::ast::Node* node = symbolTable->GetNodeNoThrow(constantSymbol);
        if (!node)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            throw cmajor::symbols::Exception("node for constant symbol '" + util::ToUtf8(constantSymbol->FullName()) + "' not found from symbol table", sourcePos, moduleId);
        }
        /// Assert(node->GetNodeType() == cmajor::ast::NodeType::constantNode, "constant node expected"); TODO
        cmajor::ast::ConstantNode* constantNode = static_cast<cmajor::ast::ConstantNode*>(node);
        constantSymbol->SetEvaluating();
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(containerScope);
        constantNode->Accept(typeBinder);
        constantSymbol->ResetEvaluating();
        cmajor::symbols::Value* constantValue = constantSymbol->GetValue();
        // Assert(constantValue, "constant value expected"); TODO
        value.reset(constantValue->Clone());
    }
}

void Evaluator::EvaluateEnumConstantSymbol(cmajor::symbols::EnumConstantSymbol* enumConstantSymbol, const soul::ast::SourcePos& sourcePos)
{
    if (enumConstantSymbol->Evaluating())
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        throw cmajor::symbols::Exception("cyclic depenency detected", sourcePos, moduleId);
    }
    cmajor::symbols::Value* enumConstantValue = enumConstantSymbol->GetValue();
    if (enumConstantValue)
    {
        value.reset(enumConstantValue->Clone());
    }
    else
    {
        cmajor::symbols::Symbol* symbol = enumConstantSymbol->Parent();
        // Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol, "enum type symbol expected"); TODO
        cmajor::symbols::EnumTypeSymbol* enumTypeSymbol = static_cast<cmajor::symbols::EnumTypeSymbol*>(symbol);
        cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNode(enumTypeSymbol);
        // Assert(node->GetNodeType() == cmajor::ast::NodeType::enumTypeNode, "enum type node expected"); TODO
        cmajor::ast::EnumTypeNode* enumTypeNode = static_cast<cmajor::ast::EnumTypeNode*>(node);
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(containerScope);
        enumTypeNode->Accept(typeBinder);
        enumConstantSymbol->ResetEvaluating();
        cmajor::symbols::Value* enumConstantValue = enumConstantSymbol->GetValue();
        // Assert(enumConstantValue, "enum constant value expected"); TODO
        value.reset(enumConstantValue->Clone());
    }
}

void Evaluator::Visit(cmajor::ast::DotNode& dotNode)
{
    dotNode.Subject()->Accept(*this);
    if (error)
    {
        return;
    }
    if (value)
    {
        if (value->IsArrayReferenceValue())
        {
            cmajor::symbols::TypeSymbol* type = static_cast<ArrayReferenceValue*>(value.get())->GetArrayValue()->GetType(symbolTable);
            ScopedValue* scopedValue = new ScopedValue(sourcePos, moduleId, type);
            scopedValue->SetType(type);
            value.reset(scopedValue);
        }
        else if (value->IsStructuredReferenceValue())
        {
            cmajor::symbols::TypeSymbol* type = static_cast<StructuredReferenceValue*>(value.get())->GetStructuredValue()->GetType(symbolTable);
            ScopedValue* scopedValue = new ScopedValue(sourcePos, moduleId, type);
            scopedValue->SetType(type->AddPointer(sourcePos, moduleId));
            scopedValue->SetSubject(value.release());
            value.reset(scopedValue);
        }
        else if (value->IsStringReferenceValue())
        {
            cmajor::symbols::TypeSymbol* type = symbolTable->GetTypeByName(U"@string_functions");
            ScopedValue* scopedValue = new ScopedValue(sourcePos, moduleId, type);
            scopedValue->SetType(type);
            scopedValue->SetSubject(value.release());
            value.reset(scopedValue);
        }
        else if (value->GetValueType() == cmajor::symbols::ValueType::structuredValue)
        {
            cmajor::symbols::TypeSymbol* type = static_cast<cmajor::symbols::StructuredValue*>(value.get())->GetType(symbolTable);
            ScopedValue* scopedValue = new ScopedValue(sourcePos, moduleId, type);
            scopedValue->SetType(type);
            value.reset(scopedValue);
        }
    }
    if (value && value->IsScopedValue())
    {
        ScopedValue* scopedValue = static_cast<ScopedValue*>(value.get());
        cmajor::symbols::ContainerSymbol* containerSymbol = scopedValue->GetContainerSymbol();
        if (containerSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
        {
            cmajor::symbols::ClassGroupTypeSymbol* classGroupTypeSymbol = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(containerSymbol);
            containerSymbol = classGroupTypeSymbol->GetClass(0);
        }
        cmajor::symbols::ContainerScope* scope = containerSymbol->GetContainerScope();
        qualifiedScope = scope;
        std::u32string memberName = dotNode.MemberId()->Str();
        cmajor::symbols::Symbol* symbol = scope->Lookup(memberName);
        if (symbol)
        {
            std::unique_ptr<cmajor::symbols::Value> receiver;
            cmajor::symbols::TypeSymbol* type = scopedValue->GetType(symbolTable);
            if (type && (type->IsArrayType() || type->BaseType()->IsClassTypeSymbol() || type->IsStringFunctionContainer()))
            {
                receiver = std::move(value);
            }
            EvaluateSymbol(symbol, dotNode.GetSourcePos());
            if (error) return;
            if (receiver && value->IsFunctionGroupValue())
            {
                FunctionGroupValue* functionGroupValue = static_cast<FunctionGroupValue*>(value.get());
                functionGroupValue->SetReceiver(std::move(receiver));
            }
        }
        else
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(containerSymbol->FullName()) + "' does not have member '" + util::ToUtf8(memberName) + "'", 
                    dotNode.GetSourcePos(), dotNode.ModuleId());
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("expression '" + dotNode.Subject()->ToString() + "' must denote a namespace, class type or enumerated type", dotNode.Subject()->GetSourcePos(), dotNode.Subject()->ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::ArrowNode& arrowNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, arrowNode.GetSourcePos(), arrowNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::EquivalenceNode& equivalenceNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, equivalenceNode.GetSourcePos(), equivalenceNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ImplicationNode& implicationNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, implicationNode.GetSourcePos(), implicationNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::DisjunctionNode& disjunctionNode)
{
    EvaluateBinOp(disjunctionNode, disjunction);
}

void Evaluator::Visit(cmajor::ast::ConjunctionNode& conjunctionNode)
{
    EvaluateBinOp(conjunctionNode, conjunction);
}

void Evaluator::Visit(cmajor::ast::BitOrNode& bitOrNode)
{
    EvaluateBinOp(bitOrNode, bitOr);
}

void Evaluator::Visit(cmajor::ast::BitXorNode& bitXorNode)
{
    EvaluateBinOp(bitXorNode, bitXor);
}

void Evaluator::Visit(cmajor::ast::BitAndNode& bitAndNode)
{
    EvaluateBinOp(bitAndNode, bitAnd);
}

void Evaluator::Visit(cmajor::ast::EqualNode& equalNode)
{
    EvaluateBinOp(equalNode, equal, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::NotEqualNode& notEqualNode)
{
    EvaluateBinOp(notEqualNode, notEqual, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::LessNode& lessNode)
{
    EvaluateBinOp(lessNode, less, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::GreaterNode& greaterNode)
{
    EvaluateBinOp(greaterNode, greater, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode)
{
    EvaluateBinOp(lessOrEqualNode, lessEqual, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode)
{
    EvaluateBinOp(greaterOrEqualNode, greaterEqual, Operator::comparison);
}

void Evaluator::Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode)
{
    EvaluateBinOp(shiftLeftNode, shiftLeft);
}

void Evaluator::Visit(cmajor::ast::ShiftRightNode& shiftRightNode)
{
    EvaluateBinOp(shiftRightNode, shiftRight);
}

void Evaluator::Visit(cmajor::ast::AddNode& addNode)
{
    EvaluateBinOp(addNode, add, Operator::add);
}

void Evaluator::Visit(cmajor::ast::SubNode& subNode)
{
    EvaluateBinOp(subNode, sub, Operator::sub);
}

void Evaluator::Visit(cmajor::ast::MulNode& mulNode)
{
    EvaluateBinOp(mulNode, mul);
}

void Evaluator::Visit(cmajor::ast::DivNode& divNode)
{
    EvaluateBinOp(divNode, div);
}

void Evaluator::Visit(cmajor::ast::RemNode& remNode)
{
    EvaluateBinOp(remNode, rem);
}

void Evaluator::Visit(cmajor::ast::NotNode& notNode)
{
    EvaluateUnaryOp(notNode, logicalNot);
}

void Evaluator::Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode)
{
    EvaluateUnaryOp(unaryPlusNode, unaryPlus);
}

void Evaluator::Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode)
{
    EvaluateUnaryOp(unaryMinusNode, unaryMinus);
}

void Evaluator::Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode)
{
    prefixIncrementNode.Subject()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId());
        }
    }
    bool unsignedType = value->GetType(symbolTable)->IsUnsignedType();
    cmajor::ast::CloneContext cloneContext;
    if (unsignedType)
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::AddNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::ByteLiteralNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    else
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::AddNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::SByteLiteralNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    prefixIncrementNode.Subject()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode)
{
    prefixDecrementNode.Subject()->Accept(*this);
    if (error)
    {
        return;
    }
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId());
        }
    }
    bool unsignedType = value->GetType(symbolTable)->IsUnsignedType();
    cmajor::ast::CloneContext cloneContext;
    if (unsignedType)
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::SubNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::ByteLiteralNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    else
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::SubNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::SByteLiteralNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    prefixDecrementNode.Subject()->Accept(*this);
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::DerefNode& derefNode)
{
    derefNode.Subject()->Accept(*this);
    if (value && value->GetValueType() == cmajor::symbols::ValueType::pointerValue)
    {
        cmajor::symbols::PointerValue* pointerValue = static_cast<cmajor::symbols::PointerValue*>(value.get());
        value.reset(pointerValue->Deref());
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
            }
            else
            {
                throw cmajor::symbols::Exception("unsupported pointer value", derefNode.GetSourcePos(), derefNode.ModuleId());
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            throw cmajor::symbols::Exception("pointer value expected", derefNode.GetSourcePos(), derefNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::AddrOfNode& addrOfNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, addrOfNode.GetSourcePos(), addrOfNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ComplementNode& complementNode)
{
    EvaluateUnaryOp(complementNode, complement);
}

void Evaluator::Visit(cmajor::ast::IsNode& isNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, isNode.GetSourcePos(), isNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::AsNode& asNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, asNode.GetSourcePos(), asNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::IndexingNode& indexingNode)
{
    indexingNode.Subject()->Accept(*this);
    if (value && value->IsArrayReferenceValue())
    {
        cmajor::symbols::ArrayValue* arrayValue = static_cast<ArrayReferenceValue*>(value.get())->GetArrayValue();
        value = Evaluate(indexingNode.Index(), symbolTable->GetTypeByName(U"long"), containerScope, boundCompileUnit, dontThrow, currentFunction, indexingNode.GetSourcePos(), indexingNode.ModuleId());
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, indexingNode.GetSourcePos(), indexingNode.ModuleId());
            }
        }
        cmajor::symbols::LongValue* indexValue = static_cast<cmajor::symbols::LongValue*>(value.get());
        int64_t index = indexValue->GetValue();
        if (index < 0 || index >= int64_t(arrayValue->Elements().size()))
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("array index out of range", indexingNode.GetSourcePos(), indexingNode.ModuleId());
            }
        }
        cmajor::symbols::Value* elementValue = arrayValue->Elements()[index].get();
        switch (elementValue->GetValueType())
        {
        case cmajor::symbols::ValueType::arrayValue:
        {
            value.reset(new ArrayReferenceValue(static_cast<cmajor::symbols::ArrayValue*>(elementValue)));
            break;
        }
        case cmajor::symbols::ValueType::stringValue: case cmajor::symbols::ValueType::wstringValue: case cmajor::symbols::ValueType::ustringValue:
        {
            value.reset(new StringReferenceValue(elementValue));
            break;
        }
        default:
        {
            value = std::unique_ptr<cmajor::symbols::Value>(elementValue->Clone());
            break;
        }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, indexingNode.GetSourcePos(), indexingNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::InvokeNode& invokeNode)
{
    if (error) return;
    std::vector<std::unique_ptr<cmajor::symbols::Value>> values;
    int n = invokeNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* arg = invokeNode.Arguments()[i];
        arg->Accept(*this);
        if (error) return;
        if (!value)
        {
            if (dontThrow)
            {
                error = true;
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
            }
        }
        values.push_back(std::move(value));
    }
    invokeNode.Subject()->Accept(*this);
    if (error) return;
    if (value && value->IsFunctionGroupValue())
    {
        FunctionGroupValue* functionGroupValue = static_cast<FunctionGroupValue*>(value.get());
        cmajor::symbols::FunctionGroupSymbol* functionGroup = functionGroupValue->FunctionGroup();
        std::vector<FunctionScopeLookup> functionScopeLookups;
        if (functionGroupValue->QualifiedScope())
        {
            FunctionScopeLookup qualifiedScopeLookup(cmajor::symbols::ScopeLookup::this_and_base, functionGroupValue->QualifiedScope());
            functionScopeLookups.push_back(qualifiedScopeLookup);
        }
        else
        {
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        }
        std::vector<std::unique_ptr<BoundExpression>> arguments = ValuesToLiterals(values, symbolTable, error);
        if (error)
        {
            if (dontThrow)
            {
                return;
            }
            else
            {
                ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
            }
        }
        if (functionGroupValue->Receiver() && functionGroupValue->Receiver()->IsScopedValue())
        {
            cmajor::symbols::TypeSymbol* type = static_cast<ScopedValue*>(functionGroupValue->Receiver())->GetType(symbolTable);
            if (type)
            {
                arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, type)));
            }
        }
        templateTypeArguments = std::move(functionGroupValue->TemplateTypeArguments());
        std::unique_ptr<cmajor::symbols::Exception> exception;
        OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
        flags = flags | OverloadResolutionFlags::dontThrow;
        std::unique_ptr<BoundFunctionCall> functionCall = ResolveOverload(functionGroup->Name(), containerScope, functionScopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
            flags, templateTypeArguments, exception);
        bool memberFunctionCall = false;
        if (!functionCall)
        {
            if (currentClassType)
            {
                arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, currentClassType->AddPointer(sourcePos, moduleId))));
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, currentClassType->GetContainerScope()));
                OverloadResolutionFlags flags = OverloadResolutionFlags::dontInstantiate;
                if (dontThrow)
                {
                    flags = flags | OverloadResolutionFlags::dontThrow;
                }
                std::unique_ptr<cmajor::symbols::Exception> exception;
                functionCall = ResolveOverload(functionGroup->Name(), containerScope, functionScopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
                    flags, templateTypeArguments, exception);
                if (functionCall)
                {
                    memberFunctionCall = true;
                }
            }
            if (!functionCall)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                }
            }
        }
        cmajor::symbols::FunctionSymbol* functionSymbol = functionCall->GetFunctionSymbol();
        if (functionSymbol->IsCompileTimePrimitiveFunction())
        {
            cmajor::symbols::Value* receiver = functionGroupValue->Receiver();
            bool skipFirst = receiver != nullptr;;
            argumentValues = ArgumentsToValues(functionCall->Arguments(), error, skipFirst, boundCompileUnit);
            if (error)
            {
                if (dontThrow)
                {
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                }
            }
            value = functionSymbol->ConstructValue(argumentValues, invokeNode.GetSourcePos(), invokeNode.ModuleId(), receiver);
            if (!value)
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                }
            }
        }
        else if (functionSymbol->IsConstExpr())
        {
            cmajor::ast::FunctionNode* functionNode = boundCompileUnit.GetFunctionNodeFor(functionSymbol);
            CheckFunctionReturnPaths(functionSymbol, *functionNode, containerScope, boundCompileUnit);
            bool skipFirst = memberFunctionCall || functionGroupValue->Receiver();
            argumentValues = ArgumentsToValues(functionCall->Arguments(), error, skipFirst, boundCompileUnit);
            if (error)
            {
                if (dontThrow)
                {
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                }
            }
            cmajor::symbols::ClassTypeSymbol* prevClassType = currentClassType;
            if (functionGroupValue->Receiver() && functionGroupValue->Receiver()->IsScopedValue())
            {
                ScopedValue* receiver = static_cast<ScopedValue*>(functionGroupValue->Receiver());
                if (receiver->GetSubject() && receiver->GetSubject()->GetType(symbolTable)->IsClassTypeSymbol())
                {
                    currentClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(receiver->GetSubject()->GetType(symbolTable));
                    structureReferenceValue = std::unique_ptr<cmajor::symbols::Value>(receiver->GetSubject()->Clone());
                }
            }
            functionNode->Accept(*this);
            currentClassType = prevClassType;
        }
        else
        {
            cmajor::symbols::IntrinsicFunction* intrinsic = functionSymbol->GetIntrinsic();
            if (intrinsic)
            {
                argumentValues = ArgumentsToValues(functionCall->Arguments(), error, boundCompileUnit);
                if (error)
                {
                    if (dontThrow)
                    {
                        return;
                    }
                    else
                    {
                        ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                    }
                }
                value = intrinsic->Evaluate(argumentValues, templateTypeArguments, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                if (!value)
                {
                    if (dontThrow)
                    {
                        error = true;
                        return;
                    }
                    else
                    {
                        ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                    }
                }
            }
            else
            {
                if (dontThrow)
                {
                    error = true;
                    return;
                }
                else
                {
                    ThrowCannotEvaluateStatically(sourcePos, moduleId, invokeNode.GetSourcePos(), invokeNode.ModuleId());
                }
            }
        }
    }
    else
    {
        if (dontThrow)
        {
            error = true;
        }
        else
        {
            throw cmajor::symbols::Exception("function group expected", invokeNode.GetSourcePos(), invokeNode.ModuleId());
        }
    }
}

void Evaluator::Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode)
{
    postfixIncrementNode.Subject()->Accept(*this);
    if (error) return;
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId());
        }
    }
    bool unsignedType = value->GetType(symbolTable)->IsUnsignedType();
    std::unique_ptr<cmajor::symbols::Value> result = std::move(value);
    cmajor::ast::CloneContext cloneContext;
    if (unsignedType)
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::AddNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext), new cmajor::ast::ByteLiteralNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    else
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::AddNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext), new cmajor::ast::SByteLiteralNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    value = std::move(result);
}

void Evaluator::Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode)
{
    postfixDecrementNode.Subject()->Accept(*this);
    if (error) return;
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId());
        }
    }
    bool unsignedType = value->GetType(symbolTable)->IsUnsignedType();
    std::unique_ptr<cmajor::symbols::Value> result = std::move(value);
    cmajor::ast::CloneContext cloneContext;
    if (unsignedType)
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::SubNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::ByteLiteralNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    else
    {
        cmajor::ast::AssignmentStatementNode assignmentStatementNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
            new cmajor::ast::SubNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                new cmajor::ast::SByteLiteralNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), 1)));
        assignmentStatementNode.Accept(*this);
    }
    value = std::move(result);
}

void Evaluator::Visit(cmajor::ast::SizeOfNode& sizeOfNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, sizeOfNode.GetSourcePos(), sizeOfNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::TypeNameNode& typeNameNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, typeNameNode.GetSourcePos(), typeNameNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::TypeIdNode& typeIdNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, typeIdNode.GetSourcePos(), typeIdNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::CastNode& castNode)
{
    cmajor::symbols::TypeSymbol* type = ResolveType(castNode.TargetTypeExpr(), boundCompileUnit, containerScope);
    bool prevCast = cast;
    cast = true;
    castNode.SourceExpr()->Accept(*this);
    if (error) return;
    if (!value)
    {
        if (dontThrow)
        {
            error = true;
            return;
        }
        else
        {
            ThrowCannotEvaluateStatically(sourcePos, moduleId, castNode.GetSourcePos(), castNode.ModuleId());
        }
    }
    value.reset(value->As(type, true, castNode.GetSourcePos(), castNode.ModuleId(), dontThrow));
    cast = prevCast;
}

void Evaluator::Visit(cmajor::ast::ConstructNode& constructNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, constructNode.GetSourcePos(), constructNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::NewNode& newNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, newNode.GetSourcePos(), newNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ThisNode& thisNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, thisNode.GetSourcePos(), thisNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::BaseNode& baseNode)
{
    if (dontThrow)
    {
        error = true;
    }
    else
    {
        ThrowCannotEvaluateStatically(sourcePos, moduleId, baseNode.GetSourcePos(), baseNode.ModuleId());
    }
}

void Evaluator::Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode)
{
    parenthesizedExpressionNode.Subject()->Accept(*this);
}

std::unique_ptr<cmajor::symbols::Value> Evaluate(cmajor::ast::Node* node, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit, bool dontThrow, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos,
    const util::uuid& moduleId)
{
    cmajor::symbols::ValueType targetValueType = targetType->GetValueType();
    Evaluator evaluator(boundCompileUnit, containerScope, targetType, targetValueType, false, dontThrow, currentFunction, sourcePos, moduleId);
    node->Accept(evaluator);
    if (evaluator.Error())
    {
        return std::unique_ptr<cmajor::symbols::Value>();
    }
    else
    {
        std::unique_ptr<cmajor::symbols::Value> value = evaluator.GetValue();
        if (value && value->IsComplete())
        {
            if (!TypesEqual(targetType->PlainType(sourcePos, moduleId), value->GetType(&boundCompileUnit.GetSymbolTable())))
            {
                if (targetType->IsArrayType() && static_cast<cmajor::symbols::ArrayTypeSymbol*>(targetType)->Size() == -1)
                {
                    return std::move(value);
                }
                if (value->IsStringReferenceValue())
                {
                    return std::move(value);
                }
                value.reset(value->As(targetType->PlainType(sourcePos, moduleId), false, node->GetSourcePos(), node->ModuleId(), dontThrow));
            }
            return std::move(value);
        }
        else
        {
            if (dontThrow)
            {
                return std::unique_ptr<cmajor::symbols::Value>();
            }
            else
            {
                throw cmajor::symbols::Exception("value not complete", node->GetSourcePos(), node->ModuleId());
            }
        }
    }
}

} // namespace cmajor::binder