// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.meta;

import cmajor.symbols.exception;
import cmajor.symbols.function.symbol;
import cmajor.symbols.symbol.table;
import cmajor.symbols.templates;
import cmajor.symbols.value;
import cmajor.symbols.array.type.symbol;
import util;

namespace cmajor::symbols {

IntrinsicFunction::IntrinsicFunction(const soul::ast::Span& span_) : span(span_)
{
}

IntrinsicFunction::~IntrinsicFunction()
{
}

std::unique_ptr<Value> IntrinsicFunction::Evaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, 
    cmajor::ast::Node* node)
{
    if (arguments.size() != Arity())
    {
        throw Exception("wrong number of parameters for intrinsic " + std::string(GroupName()), node->GetFullSpan());
    }
    if (templateArguments.size() != NumberOfTypeParameters())
    {
        throw Exception("wrong number of template type arguments for intrinsic " + std::string(GroupName()), node->GetFullSpan());
    }
    return DoEvaluate(arguments, templateArguments, node);
}

FunctionSymbol* CreateIntrinsic(IntrinsicFunction* intrinsic, SymbolTable& symbolTable, ContainerSymbol* parent, Context* context)
{
    FunctionSymbol* fun = new FunctionSymbol(intrinsic->GetSpan(), util::ToUtf32(intrinsic->GroupName()));
    fun->SetModule(symbolTable.GetModule());
    fun->SetGroupName(util::ToUtf32(intrinsic->GroupName()));
    fun->SetIntrinsic(intrinsic);
    fun->SetAccess(SymbolAccess::public_);
    fun->SetReturnType(intrinsic->ReturnType(symbolTable));
    fun->SetParent(parent);
    int n = intrinsic->NumberOfTypeParameters();
    for (int i = 0; i < n; ++i)
    {
        std::u32string p = U"T";
        if (i > 0)
        {
            p.append(util::ToUtf32(std::to_string(i)));
        }
        TemplateParameterSymbol* s = new TemplateParameterSymbol(soul::ast::Span(), p);
        symbolTable.SetTypeIdFor(s);
        s->SetModule(symbolTable.GetModule());
        fun->AddMember(s, context);
    }
    fun->ComputeName(context);
    return fun;
}

class TypePredicate : public IntrinsicFunction
{
public:
    TypePredicate(const soul::ast::Span& span_);
    int Arity() const override { return 0; }
    int NumberOfTypeParameters() const override { return 1; }
    TypeSymbol* ReturnType(SymbolTable& symbolTable) const override { return symbolTable.GetTypeByName(U"bool"); }
};

TypePredicate::TypePredicate(const soul::ast::Span& span_) : IntrinsicFunction(span_)
{
}

class IsIntegralTypePredicate : public TypePredicate
{
public:
    IsIntegralTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsIntegralType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsIntegralType()));
    }
};

IsIntegralTypePredicate::IsIntegralTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsSignedTypePredicate : public TypePredicate
{
public:
    IsSignedTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsSignedType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsIntegralType() && !type->IsUnsignedType()));
    }
};

IsSignedTypePredicate::IsSignedTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsUnsignedTypePredicate : public TypePredicate
{
public:
    IsUnsignedTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsUnsignedType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsUnsignedType()));
    }
};

IsUnsignedTypePredicate::IsUnsignedTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsFloatingPointTypePredicate : public TypePredicate
{
public:
    IsFloatingPointTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsFloatingPointType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsFloatingPointType()));
    }
};

IsFloatingPointTypePredicate::IsFloatingPointTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsBasicTypePredicate : public TypePredicate
{
public:
    IsBasicTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsBasicType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsBasicTypeSymbol()));
    }
};

IsBasicTypePredicate::IsBasicTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsBoolTypePredicate : public TypePredicate
{
public:
    IsBoolTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsBoolType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::boolTypeSymbol));
    }
};

IsBoolTypePredicate::IsBoolTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsSByteTypePredicate : public TypePredicate
{
public:
    IsSByteTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsSByteType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::sbyteTypeSymbol));
    }
};

IsSByteTypePredicate::IsSByteTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsByteTypePredicate : public TypePredicate
{
public:
    IsByteTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsByteType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::byteTypeSymbol));
    }
};

IsByteTypePredicate::IsByteTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsShortTypePredicate : public TypePredicate
{
public:
    IsShortTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsShortType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::shortTypeSymbol));
    }
};

IsShortTypePredicate::IsShortTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsUShortTypePredicate : public TypePredicate
{
public:
    IsUShortTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsUShortType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::ushortTypeSymbol));
    }
};

IsUShortTypePredicate::IsUShortTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsIntTypePredicate : public TypePredicate
{
public:
    IsIntTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsIntType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::intTypeSymbol));
    }
};

IsIntTypePredicate::IsIntTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsUIntTypePredicate : public TypePredicate
{
public:
    IsUIntTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsUIntType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::uintTypeSymbol));
    }
};

IsUIntTypePredicate::IsUIntTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsLongTypePredicate : public TypePredicate
{
public:
    IsLongTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsLongType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::longTypeSymbol));
    }
};

IsLongTypePredicate::IsLongTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsULongTypePredicate : public TypePredicate
{
public:
    IsULongTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsULongType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::ulongTypeSymbol));
    }
};

IsULongTypePredicate::IsULongTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsFloatTypePredicate : public TypePredicate
{
public:
    IsFloatTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsFloatType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::floatTypeSymbol));
    }
};

IsFloatTypePredicate::IsFloatTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsDoubleTypePredicate : public TypePredicate
{
public:
    IsDoubleTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsDoubleType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::doubleTypeSymbol));
    }
};

IsDoubleTypePredicate::IsDoubleTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsCharTypePredicate : public TypePredicate
{
public:
    IsCharTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsCharType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::charTypeSymbol));
    }
};

IsCharTypePredicate::IsCharTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsWCharTypePredicate : public TypePredicate
{
public:
    IsWCharTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsWCharType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::wcharTypeSymbol));
    }
};

IsWCharTypePredicate::IsWCharTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsUCharTypePredicate : public TypePredicate
{
public:
    IsUCharTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsUCharType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::ucharTypeSymbol));
    }
};

IsUCharTypePredicate::IsUCharTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsVoidTypePredicate : public TypePredicate
{
public:
    IsVoidTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsVoidType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::voidTypeSymbol));
    }
};

IsVoidTypePredicate::IsVoidTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsClassTypePredicate : public TypePredicate
{
public:
    IsClassTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsClassType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsClassTypeSymbol()));
    }
};

IsClassTypePredicate::IsClassTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsPolymorphicTypePredicate : public TypePredicate
{
public:
    IsPolymorphicTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsPolymorphicType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsPolymorphicType()));
    }
};

IsPolymorphicTypePredicate::IsPolymorphicTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsInterfaceTypePredicate : public TypePredicate
{
public:
    IsInterfaceTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsInterfaceType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->GetSymbolType() == SymbolType::interfaceTypeSymbol));
    }
};

IsInterfaceTypePredicate::IsInterfaceTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsDelegateTypePredicate : public TypePredicate
{
public:
    IsDelegateTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsDelegateType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsDelegateType()));
    }
};

IsDelegateTypePredicate::IsDelegateTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsClassDelegateTypePredicate : public TypePredicate
{
public:
    IsClassDelegateTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsClassDelegateType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsClassDelegateType()));
    }
};

IsClassDelegateTypePredicate::IsClassDelegateTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsEnumeratedTypePredicate : public TypePredicate
{
public:
    IsEnumeratedTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsEnumeratedType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsEnumeratedType()));
    }
};

IsEnumeratedTypePredicate::IsEnumeratedTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsConstTypePredicate : public TypePredicate
{
public:
    IsConstTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsConstType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsConstType()));
    }
};

IsConstTypePredicate::IsConstTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsReferenceTypePredicate : public TypePredicate
{
public:
    IsReferenceTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsReferenceType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsReferenceType()));
    }
};

IsReferenceTypePredicate::IsReferenceTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsLvalueReferenceTypePredicate : public TypePredicate
{
public:
    IsLvalueReferenceTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsLvalueReferenceType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsLvalueReferenceType()));
    }
};

IsLvalueReferenceTypePredicate::IsLvalueReferenceTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsRvalueReferenceTypePredicate : public TypePredicate
{
public:
    IsRvalueReferenceTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsRvalueReferenceType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsRvalueReferenceType()));
    }
};

IsRvalueReferenceTypePredicate::IsRvalueReferenceTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsArrayTypePredicate : public TypePredicate
{
public:
    IsArrayTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsArrayType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsArrayType()));
    }
};

IsArrayTypePredicate::IsArrayTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsPointerTypePredicate : public TypePredicate
{
public:
    IsPointerTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsPointerType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsPointerType()));
    }
};

IsPointerTypePredicate::IsPointerTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class IsGenericPtrTypePredicate : public TypePredicate
{
public:
    IsGenericPtrTypePredicate(const soul::ast::Span& span_);
    const char* GroupName() const override { return "IsGenericPtrType"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new BoolValue(node->GetSpan(), type->IsVoidPtrType()));
    }
};

IsGenericPtrTypePredicate::IsGenericPtrTypePredicate(const soul::ast::Span& span_) : TypePredicate(span_)
{
}

class PointerCountIntrinsicFunction : public IntrinsicFunction
{
public:
    PointerCountIntrinsicFunction(const soul::ast::Span& span_);
    int Arity() const override { return 0; }
    int NumberOfTypeParameters() const override { return 1; }
    TypeSymbol* ReturnType(SymbolTable& symbolTable) const override { return symbolTable.GetTypeByName(U"int"); }
    const char* GroupName() const override { return "PointerCount"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        return std::unique_ptr<Value>(new IntValue(node->GetSpan(), type->PointerCount()));
    }
};

PointerCountIntrinsicFunction::PointerCountIntrinsicFunction(const soul::ast::Span& span_) : IntrinsicFunction(span_)
{
}

class ArrayLengthIntrinsicFunction : public IntrinsicFunction
{
public:
    ArrayLengthIntrinsicFunction(const soul::ast::Span& span_);
    int Arity() const override { return 0; }
    int NumberOfTypeParameters() const override { return 1; }
    TypeSymbol* ReturnType(SymbolTable& symbolTable) const override { return symbolTable.GetTypeByName(U"long"); }
    const char* GroupName() const override { return "ArrayLength"; }
    std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) override
    {
        TypeSymbol* type = templateArguments.front();
        if (type->IsArrayType())
        {
            ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(type);
            return std::unique_ptr<Value>(new LongValue(node->GetSpan(), arrayType->Size()));
        }
        return std::unique_ptr<Value>(new LongValue(node->GetSpan(), 0));
    }
};

ArrayLengthIntrinsicFunction::ArrayLengthIntrinsicFunction(const soul::ast::Span& span_) : IntrinsicFunction(span_)
{
}

void MetaInit(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, Context* context)
{
    symbolTable.BeginNamespace(U"System.Meta", rootSpan, symbolTable.GlobalNs().ModuleId(), symbolTable.GlobalNs().FileIndex(), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsIntegralTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsSignedTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsUnsignedTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsFloatingPointTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsBasicTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsBoolTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsSByteTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsByteTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsShortTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsUShortTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsIntTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsUIntTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsLongTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsULongTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsFloatTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsDoubleTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsCharTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsWCharTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsUCharTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsVoidTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsClassTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsPolymorphicTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsInterfaceTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsDelegateTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsClassDelegateTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsEnumeratedTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsConstTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsReferenceTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsLvalueReferenceTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsRvalueReferenceTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsArrayTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsPointerTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new IsGenericPtrTypePredicate(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new PointerCountIntrinsicFunction(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.Container()->AddMember(CreateIntrinsic(new ArrayLengthIntrinsicFunction(rootSpan), symbolTable, symbolTable.Container(), context), context);
    symbolTable.EndNamespace();
}

} // namespace cmajor::symbols
