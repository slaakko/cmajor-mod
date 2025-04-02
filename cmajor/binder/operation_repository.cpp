// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.operation.repository;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.bound_class;
import cmajor.binder.bound.function;
import cmajor.binder.bound.statement;
import cmajor.binder.expression.binder;
import cmajor.binder.type.binder;
import cmajor.binder.overload.resolution;
import cmajor.binder.type.resolver;
import cmajor.binder.bound.expression;
import util;

namespace cmajor::binder {

class PointerDefaultCtor : public cmajor::symbols::FunctionSymbol
{
public:
    PointerDefaultCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerDefaultCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
    void* nullValue;
};

PointerDefaultCtor::PointerDefaultCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(span_, U"@constructor"), type(type_), nullValue(nullptr)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    ComputeName(context);
}

void PointerDefaultCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 1, "default constructor needs one object"); 
    if (!nullValue)
    {
        nullValue = emitter.CreateDefaultIrValueForPtrType(type->IrType(emitter, context));
    }
    emitter.Stack().Push(nullValue);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerDefaultConstructorOperation : public Operation
{
public:
    PointerDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerDefaultConstructorOperation::PointerDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void PointerDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
    if (!function)
    {
        function = new PointerDefaultCtor(pointerType, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[pointerType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerCopyCtor : public cmajor::symbols::FunctionSymbol
{
public:
    PointerCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerCopyCtor::PointerCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    ComputeName(context);
}

void PointerCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Dup();
        void* ptr = emitter.Stack().Pop();
        emitter.SaveObjectPointer(ptr);
    }
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerCopyConstructorOperation : public Operation
{
public:
    PointerCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerCopyConstructorOperation::PointerCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void PointerCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(GetContext())) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerCopyCtor(pointerType, node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[pointerType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class PointerMoveCtor : public cmajor::symbols::FunctionSymbol
{
public:
    PointerMoveCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMoveCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerMoveCtor::PointerMoveCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type->AddRvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void PointerMoveCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "move constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter, context), rvalueRefValue)); // TODO
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Dup();
        void* ptr = emitter.Stack().Pop();
        emitter.SaveObjectPointer(ptr);
    }
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerMoveConstructorOperation : public Operation
{
public:
    PointerMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMoveConstructorOperation::PointerMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void PointerMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    if (TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(GetContext())) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerMoveCtor(pointerType, node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[pointerType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class PointerCopyAssignment : public cmajor::symbols::FunctionSymbol
{
public:
    PointerCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerCopyAssignment::PointerCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, 
    cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    ComputeName(context);
}

void PointerCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerCopyAssignmentOperation : public Operation
{
public:
    PointerCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerCopyAssignmentOperation::PointerCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void PointerCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, 
    cmajor::ast::Node* node, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(GetContext())) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerCopyAssignment(pointerType, GetSymbolTable()->GetTypeByName(U"void"), node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[pointerType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class PointerMoveAssignment : public cmajor::symbols::FunctionSymbol
{
public:
    PointerMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMoveAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerMoveAssignment::PointerMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, 
    const soul::ast::Span& span_, cmajor::symbols::Context* context) : cmajor::symbols::FunctionSymbol(span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    ComputeName(context);
}

void PointerMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter, context), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerMoveAssignmentOperation : public Operation
{
public:
    PointerMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMoveAssignmentOperation::PointerMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void PointerMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    if (TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(GetContext())) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerMoveAssignment(pointerType, GetSymbolTable()->GetTypeByName(U"void"), node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[pointerType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class PointerReturn : public cmajor::symbols::FunctionSymbol
{
public:
    PointerReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerReturn::PointerReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(span_, U"value");
    valueParam->SetType(type);
    AddMember(valueParam, context);
    SetReturnType(type);
    ComputeName(context);
}

void PointerReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Dup();
        void* ptr = emitter.Stack().Pop();
        emitter.SaveObjectPointer(ptr);
    }
}

class PointerReturnOperation : public Operation
{
public:
    PointerReturnOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerReturnOperation::PointerReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void PointerReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (!type->IsPointerType()) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[type];
    if (!function)
    {
        function = new PointerReturn(type, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[type] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerPlusOffset : public cmajor::symbols::FunctionSymbol
{
public:
    PointerPlusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerPlusOffset"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerPlusOffset::PointerPlusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::Span& span_, 
    cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator+"), pointerType(pointerType_)
{
    SetGroupName(U"operator+");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(pointerType);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(longType_);
    AddMember(rightParam, context);
    SetReturnType(pointerType);
    ComputeName(context);
}

void PointerPlusOffset::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator+ needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(context)->IrType(emitter, context), left, right));
}

class PointerPlusOffsetOperation : public Operation
{
public:
    PointerPlusOffsetOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerPlusOffsetOperation::PointerPlusOffsetOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator+", 2, boundCompileUnit_)
{
}

void PointerPlusOffsetOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(GetContext())->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, argumentMatch, node))
        {
            return;
        }
    }
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerPlusOffset(leftType, GetSymbolTable()->GetTypeByName(U"long"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[leftType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class OffsetPlusPointer : public cmajor::symbols::FunctionSymbol
{
public:
    OffsetPlusPointer(cmajor::symbols::TypeSymbol* longType_, cmajor::symbols::TypeSymbol* pointerType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "OffsetPlusPointer"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

OffsetPlusPointer::OffsetPlusPointer(cmajor::symbols::TypeSymbol* longType_, cmajor::symbols::TypeSymbol* pointerType_, const soul::ast::Span& span_, 
    cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator+"), pointerType(pointerType_)
{
    SetGroupName(U"operator+");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(longType_);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(pointerType);
    AddMember(rightParam, context);
    SetReturnType(pointerType);
    ComputeName(context);
}

void OffsetPlusPointer::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator+ needs two objects"); 
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(context)->IrType(emitter, context), right, left));
}

class OffsetPlusPointerOperation : public Operation
{
public:
    OffsetPlusPointerOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

OffsetPlusPointerOperation::OffsetPlusPointerOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator+", 2, boundCompileUnit_)
{
}

void OffsetPlusPointerOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->PlainType(GetContext())->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(leftType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, argumentMatch, node))
        {
            return;
        }
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* longType = GetSymbolTable()->GetTypeByName(U"long");
    if (rightType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(rightType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[rightType];
    if (!function)
    {
        function = new OffsetPlusPointer(longType, rightType, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[rightType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerMinusOffset : public cmajor::symbols::FunctionSymbol
{
public:
    PointerMinusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMinusOffset"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerMinusOffset::PointerMinusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, 
    const soul::ast::Span& span_, cmajor::symbols::Context* context) : cmajor::symbols::FunctionSymbol(span_, U"operator-"), pointerType(pointerType_)
{
    SetGroupName(U"operator-");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(longType_);
    AddMember(rightParam, context);
    SetReturnType(pointerType_);
    ComputeName(context);
}

void PointerMinusOffset::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator- needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    void* offset = emitter.CreateNeg(right);
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(context)->IrType(emitter, context), left, offset));
}

class PointerMinusOffsetOperation : public Operation
{
public:
    PointerMinusOffsetOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMinusOffsetOperation::PointerMinusOffsetOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator-", 2, boundCompileUnit_)
{
}

void PointerMinusOffsetOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(GetContext())->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, argumentMatch, node))
        {
            return;
        }
    }
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerMinusOffset(leftType, GetSymbolTable()->GetTypeByName(U"long"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[leftType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerMinusPointer : public cmajor::symbols::FunctionSymbol
{
public:
    PointerMinusPointer(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMinusPointer"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerMinusPointer::PointerMinusPointer(cmajor::symbols::TypeSymbol* pointerType_, 
    cmajor::symbols::TypeSymbol* longType_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator-"), pointerType(pointerType_)
{
    SetGroupName(U"operator-");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam, context);
    SetReturnType(longType_);
    ComputeName(context);
}

void PointerMinusPointer::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator- needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreatePtrDiff(pointerType->RemovePointer(context)->IrType(emitter, context), left, right));
}

class PointerMinusPointerOperation : public Operation
{
public:
    PointerMinusPointerOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMinusPointerOperation::PointerMinusPointerOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator-", 2, boundCompileUnit_)
{
}

void PointerMinusPointerOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(GetContext());
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerMinusPointer(leftType, GetSymbolTable()->GetTypeByName(U"long"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[leftType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerEqual : public cmajor::symbols::FunctionSymbol
{
public:
    PointerEqual(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerEqual"; }
};

PointerEqual::PointerEqual(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator==")
{
    SetGroupName(U"operator==");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam, context);
    SetReturnType(boolType_);
    ComputeName(context);
}

void PointerEqual::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator== needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateICmpEQ(left, right));
}

class PointerEqualOperation : public Operation
{
public:
    PointerEqualOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerEqualOperation::PointerEqualOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator==", 2, boundCompileUnit_)
{
}

void PointerEqualOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    rightType = rightType->PlainType(GetContext());
    if (!rightType->IsPointerType()) return;
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerEqual(leftType, GetSymbolTable()->GetTypeByName(U"bool"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[leftType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerLess : public cmajor::symbols::FunctionSymbol
{
public:
    PointerLess(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerLess"; }
};

PointerLess::PointerLess(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator<")
{
    SetGroupName(U"operator<");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(span_, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam, context);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(span_, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam, context);
    SetReturnType(boolType_);
    ComputeName(context);
}

void PointerLess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "operator< needs two objects"); 
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateICmpULT(left, right));
}

class PointerLessOperation : public Operation
{
public:
    PointerLessOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerLessOperation::PointerLessOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator<", 2, boundCompileUnit_)
{
}

void PointerLessOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(GetContext());
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(GetContext());
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerLess(leftType, GetSymbolTable()->GetTypeByName(U"bool"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[leftType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class PointerArrow : public cmajor::symbols::FunctionSymbol
{
public:
    PointerArrow(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerArrow"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerArrow::PointerArrow(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"operator->"), type(type_)
{
    SetGroupName(U"operator->");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* operandParam = new cmajor::symbols::ParameterSymbol(span_, U"operand");
    operandParam->SetType(type->AddPointer(context));
    AddMember(operandParam, context);
    SetReturnType(type);
    ComputeName(context);
}

void PointerArrow::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context) 
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class PointerArrowOperation : public Operation
{
public:
    PointerArrowOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerArrowOperation::PointerArrowOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator->", 1, boundCompileUnit_)
{
}

void PointerArrowOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(GetContext());
    cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
    if (!function)
    {
        function = new PointerArrow(pointerType, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[pointerType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class LvalueRefefenceCopyCtor : public cmajor::symbols::FunctionSymbol
{
public:
    LvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueRefefenceCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueRefefenceCopyCtor::LvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    ComputeName(context);
}

void LvalueRefefenceCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "reference copy constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceCopyConstructorOperation : public Operation
{
public:
    LvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceCopyConstructorOperation::LvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void LvalueReferenceCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(GetContext());
    cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
    if (!function)
    {
        function = new LvalueRefefenceCopyCtor(lvalueRefType, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[lvalueRefType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class LvalueReferenceCopyAssignment : public cmajor::symbols::FunctionSymbol
{
public:
    LvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsLvalueReferenceCopyAssignment() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceCopyAssignment::LvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_,
    cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    ComputeName(context);
}

void LvalueReferenceCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceCopyAssignmentOperation : public Operation
{
public:
    LvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceCopyAssignmentOperation::LvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void LvalueReferenceCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(GetContext());
    if (lvalueRefType->PlainType(GetContext())->IsClassTypeSymbol()) return;
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType()->RemoveConst(GetContext()), lvalueRefType->PlainType(GetContext())->AddRvalueReference(GetContext())) && 
        !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
        if (!function)
        {
            function = new LvalueReferenceCopyAssignment(lvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[lvalueRefType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class LvalueReferenceMoveAssignment : public cmajor::symbols::FunctionSymbol
{
public:
    LvalueReferenceMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceMoveAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceMoveAssignment::LvalueReferenceMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_,
    cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type->RemoveReference(context)->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    ComputeName(context);
}

void LvalueReferenceMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->PlainType(context)->IrType(emitter, context), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceMoveAssignmentOperation : public Operation
{
public:
    LvalueReferenceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceMoveAssignmentOperation::LvalueReferenceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void LvalueReferenceMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(GetContext());
    if (lvalueRefType->PlainType(GetContext())->IsClassTypeSymbol()) return;
    if (lvalueRefType->PlainType(GetContext())->IsArrayType()) return;
    if (TypesEqual(arguments[1]->GetType()->RemoveConst(GetContext()), lvalueRefType->PlainType(GetContext())->AddRvalueReference(GetContext())) || 
        arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
        if (!function)
        {
            function = new LvalueReferenceMoveAssignment(lvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), node->GetSpan(), GetContext());
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[lvalueRefType] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class LvalueReferenceReturn : public cmajor::symbols::FunctionSymbol
{
public:
    LvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceReturn::LvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(span_, U"value");
    valueParam->SetType(type);
    AddMember(valueParam, context);
    SetReturnType(type);
    ComputeName(context);
}

void LvalueReferenceReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceReturnOperation : public Operation
{
public:
    LvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceReturnOperation::LvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void LvalueReferenceReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (!type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[type];
    if (!function)
    {
        function = new LvalueReferenceReturn(type, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[type] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class RvalueRefefenceCopyCtor : public cmajor::symbols::FunctionSymbol
{
public:
    RvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueRefefenceCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueRefefenceCopyCtor::RvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) : 
    cmajor::symbols::FunctionSymbol(span_, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    ComputeName(context);
}

void RvalueRefefenceCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "reference copy constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class RvalueReferenceCopyConstructorOperation : public Operation
{
public:
    RvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceCopyConstructorOperation::RvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void RvalueReferenceCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsRvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* rvalueRefType = type->RemovePointer(GetContext());
    if (rvalueRefType->PlainType(GetContext())->IsClassTypeSymbol()) return;
    cmajor::symbols::FunctionSymbol* function = functionMap[rvalueRefType];
    if (!function)
    {
        function = new RvalueRefefenceCopyCtor(rvalueRefType, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[rvalueRefType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class RvalueReferenceCopyAssignment : public cmajor::symbols::FunctionSymbol
{
public:
    RvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueReferenceCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueReferenceCopyAssignment::RvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::Span& span_,
    cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    ComputeName(context);
}

void RvalueReferenceCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->PlainType(context)->IrType(emitter, context), rvalueRefValue));  // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class RvalueReferenceCopyAssignmentOperation : public Operation
{
public:
    RvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceCopyAssignmentOperation::RvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void RvalueReferenceCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsRvalueReferenceType()) return;
    cmajor::symbols::TypeSymbol* rvalueRefType = type->RemovePointer(GetContext());
    if (rvalueRefType->PlainType(GetContext())->IsClassTypeSymbol()) return;
    if (rvalueRefType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(rvalueRefType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[rvalueRefType];
    if (!function)
    {
        function = new RvalueReferenceCopyAssignment(rvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[rvalueRefType] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class RvalueReferenceReturn : public cmajor::symbols::FunctionSymbol
{
public:
    RvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
        cmajor::symbols::Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueReferenceReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueReferenceReturn::RvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::Span& span_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(span_, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(span_, U"value");
    valueParam->SetType(type);
    AddMember(valueParam, context);
    SetReturnType(type);
    ComputeName(context);
}

void RvalueReferenceReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags,
    cmajor::symbols::Context* context)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class RvalueReferenceReturnOperation : public Operation
{
public:
    RvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceReturnOperation::RvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void RvalueReferenceReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (!type->IsRvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[type];
    if (!function)
    {
        function = new RvalueReferenceReturn(type, node->GetSpan(), GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[type] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class ArrayDefaultConstructorOperation : public Operation
{
public:
    ArrayDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayDefaultConstructorOperation::ArrayDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void ArrayDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(GetContext())->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(GetContext()));
    cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
    if (!function)
    {
        std::vector<FunctionScopeLookup> elementLookups;
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
            arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> elementArguments;
        elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddPointer(GetContext()))));
        std::unique_ptr<BoundFunctionCall> elementDefaultConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), 
            currentFunction, node);
        cmajor::symbols::FunctionSymbol* elementTypeDefaultConstructor = elementDefaultConstructor->GetFunctionSymbol();
        cmajor::symbols::ArrayTypeDefaultConstructor* arrayTypeDefaultConstructor = new cmajor::symbols::ArrayTypeDefaultConstructor(arrayType, elementTypeDefaultConstructor,
            GetContext());
        arrayTypeDefaultConstructor->SetTemporariesForElementTypeDefaultCtor(elementDefaultConstructor->ReleaseTemporaries());
        function = arrayTypeDefaultConstructor;
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[arrayType->TypeId()] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class ArrayCopyConstructorOperation : public Operation
{
public:
    ArrayCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayCopyConstructorOperation::ArrayCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ArrayCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(GetContext())->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(GetContext()));
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(GetContext())) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        TypesEqual(arguments[1]->GetType()->PlainType(GetContext()), arrayType))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddPointer(GetContext()))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(),
                arrayType->ElementType()->AddConst(GetContext())->AddLvalueReference(GetContext()))));
            std::unique_ptr<BoundFunctionCall> elementCopyConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, 
                GetBoundCompileUnit(), currentFunction, node);
            cmajor::symbols::FunctionSymbol* elementTypeCopyConstructor = elementCopyConstructor->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeCopyConstructor* arrayTypeCopyConstructor = new cmajor::symbols::ArrayTypeCopyConstructor(arrayType, elementTypeCopyConstructor,
                GetContext());
            arrayTypeCopyConstructor->SetTemporariesForElementTypeCopyCtor(elementCopyConstructor->ReleaseTemporaries());
            function = arrayTypeCopyConstructor;
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[arrayType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class ArrayMoveConstructorOperation : public Operation
{
public:
    ArrayMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayMoveConstructorOperation::ArrayMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ArrayMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(GetContext())->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(GetContext()));
    if (TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(GetContext())) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddPointer(GetContext()))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(),
                arrayType->ElementType()->AddRvalueReference(GetContext()))));
            elementArguments.back()->SetFlag(BoundExpressionFlags::bindToRvalueReference);
            std::unique_ptr<BoundFunctionCall> elementMoveConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, 
                GetBoundCompileUnit(), currentFunction, node);
            cmajor::symbols::FunctionSymbol* elementTypeMoveConstructor = elementMoveConstructor->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeMoveConstructor* arrayTypeMoveConstructor = new cmajor::symbols::ArrayTypeMoveConstructor(arrayType, elementTypeMoveConstructor,
                GetContext());
            arrayTypeMoveConstructor->SetTemporariesForElementTypeMoveCtor(elementMoveConstructor->ReleaseTemporaries());
            function = arrayTypeMoveConstructor;
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[arrayType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class ArrayCopyAssignmentOperation : public Operation
{
public:
    ArrayCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayCopyAssignmentOperation::ArrayCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ArrayCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(GetContext())->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(GetContext()));
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none && TypesEqual(arguments[1]->GetType()->PlainType(GetContext()), arrayType) ||
        !TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(GetContext())) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        TypesEqual(arguments[1]->GetType()->PlainType(GetContext()), arrayType))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddPointer(GetContext()))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(),
                arrayType->ElementType()->AddConst(GetContext())->AddLvalueReference(GetContext()))));
            std::unique_ptr<BoundFunctionCall> elementCopyAssignment = ResolveOverload(U"operator=", containerScope, elementLookups, elementArguments, 
                GetBoundCompileUnit(), currentFunction, node);
            cmajor::symbols::FunctionSymbol* elementTypeCopyAssignment = elementCopyAssignment->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeCopyAssignment* arrayTypeCopyAssignment = new cmajor::symbols::ArrayTypeCopyAssignment(arrayType, elementTypeCopyAssignment, 
                GetContext());
            arrayTypeCopyAssignment->SetTemporariesForElementTypeCopyAssignment(elementCopyAssignment->ReleaseTemporaries());
            function = arrayTypeCopyAssignment;
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[arrayType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class ArrayMoveAssignmentOperation : public Operation
{
public:
    ArrayMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayMoveAssignmentOperation::ArrayMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ArrayMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(GetContext())->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(GetContext()));
    if (TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(GetContext())) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddPointer(GetContext()))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(node->GetSpan(), arrayType->ElementType()->AddRvalueReference(GetContext()))));
            elementArguments.back()->SetFlag(BoundExpressionFlags::bindToRvalueReference);
            std::unique_ptr<BoundFunctionCall> elementMoveAssignment = ResolveOverload(U"operator=", containerScope, elementLookups, elementArguments, 
                GetBoundCompileUnit(), currentFunction, node);
            cmajor::symbols::FunctionSymbol* elementTypeMoveAssignment = elementMoveAssignment->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeMoveAssignment* arrayTypeMoveAssignment = new cmajor::symbols::ArrayTypeMoveAssignment(arrayType, elementTypeMoveAssignment,
                GetContext());
            arrayTypeMoveAssignment->SetTemporariesForElementTypeMoveAssignment(elementMoveAssignment->ReleaseTemporaries());
            function = arrayTypeMoveAssignment;
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[arrayType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class ArrayElementAccessOperation : public Operation
{
public:
    ArrayElementAccessOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayElementAccessOperation::ArrayElementAccessOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator[]", 2, boundCompileUnit_)
{
}

void ArrayElementAccessOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->PlainType(GetContext())->IsArrayType()) return;
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(leftType->PlainType(GetContext()));
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(GetContext())->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, argumentMatch, node))
        {
            return;
        }
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
    if (!function)
    {
        function = new cmajor::symbols::ArrayTypeElementAccess(arrayType, GetContext());
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[arrayType->TypeId()] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class ClassDefaultConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassDefaultConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    bool CanInline() const override { return canInline; }
    void SetCanInline(bool canInline_) { canInline = canInline_; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassDefaultConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
    bool canInline;
};

ClassDefaultConstructor::ClassDefaultConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSpan(), U"@constructor"), classType(classType_), canInline(true)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"this");
    thisParam->SetType(classType->AddPointer(boundCompileUnit->GetContext()));
    AddMember(thisParam, boundCompileUnit->GetContext());
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (boundCompileUnit)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
            if (compileUnitNode)
            {
                SetCompileUnitId(compileUnitNode->Id());
            }
        }
    }
    ComputeName(boundCompileUnit->GetContext());
}

class ClassDefaultConstructorOperation : public Operation
{
public:
    ClassDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
    bool GenerateImplementation(ClassDefaultConstructor* defaultConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassDefaultConstructorOperation::ClassDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void ClassDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    cmajor::symbols::TypeSymbol* baseType = type->RemovePointer(GetContext())->PlainType(GetContext());
    if (type->PointerCount() != 1 || !baseType->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot create an instance of a static class", node->GetFullSpan(), classType->GetFullSpan()));
        return;
    }
    if (classType->DefaultConstructor())
    {
        viableFunctions.Insert(classType->DefaultConstructor());
        return;
    }
    if (classType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        GetBoundCompileUnit().FinalizeBinding(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(classType));
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[classType->TypeId()];
    if (!function)
    {
        std::unique_ptr<ClassDefaultConstructor> defaultConstructor(new ClassDefaultConstructor(classType, &GetBoundCompileUnit()));
        function = defaultConstructor.get();
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(classType);
        function->SetLinkOnceOdrLinkage();
        defaultConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
        defaultConstructor->SetModule(&GetBoundCompileUnit().GetModule());
        if ((flags & CollectFlags::dontInstantiate) == CollectFlags::none)
        {
            functionMap[classType->TypeId()] = function;
            if (GenerateImplementation(defaultConstructor.get(), containerScope, currentFunction, exception, node))
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(defaultConstructor.release()));
            }
            else
            {
                functionMap[classType->TypeId()] = nullptr;
                return;
            }
        }
        else
        {
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(defaultConstructor.release()));
        }
    }
    viableFunctions.Insert(function);
}

bool ClassDefaultConstructorOperation::GenerateImplementation(ClassDefaultConstructor* defaultConstructor, cmajor::symbols::ContainerScope* containerScope, 
    BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = defaultConstructor->ClassType();
    cmajor::symbols::Context* context = GetContext();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), defaultConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                new BoundFunctionCall(node->GetSpan(), classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
            defaultConstructor->SetCanInline(false);
        }
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseConstructorCallLookups;
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseConstructorCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = defaultConstructor->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, 
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
            defaultConstructor->SetCanInline(false);
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = defaultConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(context), 
                    containerScope, currentFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
            defaultConstructor->SetCanInline(false);
        }
        int n = classType->MemberVariables().size();
        if (n > maxGeneratedInlineFunctionMembers)
        {
            defaultConstructor->SetCanInline(false);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), defaultConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, 
                memberConstructorCallArguments, GetBoundCompileUnit(), boundFunction.get(), node);
            if (!memberConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->MoveTemporaryDestructorCallsTo(*memberConstructorCall);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(memberConstructorCall))));
        }
        GetBoundCompileUnit().AddBoundNode(std::move(boundFunction));
        if (nothrow)
        {
            defaultConstructor->SetNothrow();
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create default constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references));
        return false;
    }
    return  true;
}

class ClassCopyConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassCopyConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    bool CanInline() const override { return canInline; }
    void SetCanInline(bool canInline_) { canInline = canInline_; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassCopyConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
    bool canInline;
};

ClassCopyConstructor::ClassCopyConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSpan(), U"@constructor"), classType(classType_), canInline(true)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::Context* context = boundCompileUnit->GetContext();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"this");
    thisParam->SetType(classType->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"that");
    thatParam->SetType(classType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (boundCompileUnit)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
            if (compileUnitNode)
            {
                SetCompileUnitId(compileUnitNode->Id());
            }
        }
    }
    ComputeName(context);
}

class ClassCopyConstructorOperation : public Operation
{
public:
    ClassCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
    bool GenerateImplementation(ClassCopyConstructor* copyConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node);
    void AddFunction(std::unique_ptr<cmajor::symbols::FunctionSymbol>&& function) { functions.push_back(std::move(function)); }
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassCopyConstructorOperation::ClassCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ClassCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    cmajor::symbols::TypeSymbol* baseType = type->RemovePointer(GetContext())->PlainType(GetContext());
    if (type->PointerCount() != 1 || !baseType->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot copy an instance of a static class", node->GetFullSpan(), classType->GetFullSpan()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(GetContext());
    bool typesEqual = TypesEqual(rightType, classType);
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool conversionFunctionExists = false;
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, argumentMatch, node);
        if (conversion && conversion->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
        {
            conversionFunctionExists = true;
            if (conversion->ReturnsClassInterfaceOrClassDelegateByValue())
            {
                bindToRvalueRef = true; // in this case class move constructor will match
            }
        }
    }
    if (typesEqual ||
        (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
            !TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(GetContext())) && !bindToRvalueRef) && (typesEqual || conversionFunctionExists)))
    {
        if (classType->CopyConstructor())
        {
            if (!classType->CopyConstructor()->IsSuppressed())
            {
                viableFunctions.Insert(classType->CopyConstructor());
            }
            return;
        }
        if (GetBoundCompileUnit().HasCopyConstructorFor(classType->TypeId()))
        {
            viableFunctions.Insert(GetBoundCompileUnit().GetCopyConstructorFor(classType->TypeId()));
            return;
        }
        if (classType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            GetBoundCompileUnit().FinalizeBinding(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(classType));
        }
        cmajor::symbols::FunctionSymbol* function = functionMap[classType->TypeId()];
        if (!function)
        {
            std::unique_ptr<ClassCopyConstructor> copyConstructor(new ClassCopyConstructor(classType, &GetBoundCompileUnit()));
            function = copyConstructor.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            copyConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            copyConstructor->SetModule(&GetBoundCompileUnit().GetModule());
            if ((flags & CollectFlags::dontInstantiate) == CollectFlags::none)
            {
                functionMap[classType->TypeId()] = function;
                if (GenerateImplementation(copyConstructor.get(), containerScope, currentFunction, exception, node))
                {
                    GetBoundCompileUnit().AddCopyConstructorToMap(classType->TypeId(), copyConstructor.get());
                    functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copyConstructor.release()));
                }
                else
                {
                    functionMap[classType->TypeId()] = nullptr;
                    return;
                }
            }
            else
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copyConstructor.release()));
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassCopyConstructorOperation::GenerateImplementation(ClassCopyConstructor* copyConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = copyConstructor->ClassType();
    cmajor::symbols::Context* context = GetContext();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), copyConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                new BoundFunctionCall(node->GetSpan(), classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
            copyConstructor->SetCanInline(false);
        }
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseConstructorCallLookups;
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseConstructorCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = copyConstructor->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context),
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = copyConstructor->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), 
                classType->BaseClass()->AddConst(context)->AddLvalueReference(context), containerScope, currentFunction, argumentMatch, node);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* thatArgumentConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, 
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
            copyConstructor->SetCanInline(false);
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = copyConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(context), 
                    containerScope, currentFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
            copyConstructor->SetCanInline(false);
        }
        int n = classType->MemberVariables().size();
        if (n > maxGeneratedInlineFunctionMembers)
        {
            copyConstructor->SetCanInline(false);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), copyConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
            cmajor::symbols::ParameterSymbol* thatParam = copyConstructor->Parameters()[1];
            BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)),
                    thatParam->GetType()->BaseType()->AddPointer(context))));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, memberConstructorCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!memberConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->MoveTemporaryDestructorCallsTo(*memberConstructorCall);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(memberConstructorCall))));
        }
        GetBoundCompileUnit().AddBoundNode(std::move(boundFunction));
        if (nothrow)
        {
            copyConstructor->SetNothrow();
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create copy constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references));
        return false;
    }
    return true;
}

class ClassMoveConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassMoveConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    bool CanInline() const override { return canInline; }
    void SetCanInline(bool canInline_) { canInline = canInline_; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassMoveConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
    bool canInline;
};

ClassMoveConstructor::ClassMoveConstructor(cmajor::symbols::ClassTypeSymbol* classType_, BoundCompileUnit* boundCompileUnit) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSpan(), U"@constructor"), classType(classType_), canInline(true)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::Context* context = boundCompileUnit->GetContext();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"this");
    thisParam->SetType(classType->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"that");
    thatParam->SetType(classType->AddRvalueReference(context));
    AddMember(thatParam, context);
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (boundCompileUnit)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
            if (compileUnitNode)
            {
                SetCompileUnitId(compileUnitNode->Id());
            }
        }
    }
    ComputeName(context);
}

class ClassMoveConstructorOperation : public Operation
{
public:
    ClassMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
    bool GenerateImplementation(ClassMoveConstructor* moveConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassMoveConstructorOperation::ClassMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ClassMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    cmajor::symbols::TypeSymbol* baseType = type->RemovePointer(GetContext())->PlainType(GetContext());
    if (type->PointerCount() != 1 || !baseType->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot move an instance of a static class", node->GetFullSpan(), classType->GetFullSpan()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(GetContext());
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool typesEqual = TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(GetContext()));
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, argumentMatch, node);
        if (conversion && conversion->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
        {
            if (conversion->ReturnsClassInterfaceOrClassDelegateByValue())
            {
                bindToRvalueRef = true;
            }
        }
        else if (!TypesEqual(rightType, classType))
        {
            return; // reject conversion
        }
    }
    if (typesEqual || bindToRvalueRef)
    {
        if (classType->MoveConstructor())
        {
            viableFunctions.Insert(classType->MoveConstructor());
            return;
        }
        if (classType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            GetBoundCompileUnit().FinalizeBinding(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(classType));
        }
        cmajor::symbols::FunctionSymbol* function = functionMap[classType->TypeId()];
        if (!function)
        {
            std::unique_ptr<ClassMoveConstructor> moveConstructor(new ClassMoveConstructor(classType, &GetBoundCompileUnit()));
            function = moveConstructor.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            moveConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            moveConstructor->SetModule(&GetBoundCompileUnit().GetModule());
            if ((flags & CollectFlags::dontInstantiate) == CollectFlags::none)
            {
                functionMap[classType->TypeId()] = function;
                if (GenerateImplementation(moveConstructor.get(), containerScope, currentFunction, exception, node))
                {
                    functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveConstructor.release()));
                }
                else
                {
                    functionMap[classType->TypeId()] = nullptr;
                    return;
                }
            }
            else
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveConstructor.release()));
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassMoveConstructorOperation::GenerateImplementation(ClassMoveConstructor* moveConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = moveConstructor->ClassType();
    cmajor::symbols::Context* context = GetContext();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), moveConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                new BoundFunctionCall(node->GetSpan(), classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
            moveConstructor->SetCanInline(false);
        }
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseConstructorCallLookups;
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseConstructorCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = moveConstructor->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            std::unique_ptr<BoundExpression> baseClassPointerConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)),
                thisToBaseConversion));
            baseConstructorCallArguments.push_back(std::move(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = moveConstructor->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), 
                classType->BaseClass()->AddRvalueReference(context), containerScope, currentFunction, argumentMatch, node);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            std::unique_ptr<BoundExpression> thatArgumentConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)),
                thatToBaseConversion));
            baseConstructorCallArguments.push_back(std::move(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, 
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
            moveConstructor->SetCanInline(false);
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = moveConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(context), 
                    containerScope, currentFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
            moveConstructor->SetCanInline(false);
        }
        int n = classType->MemberVariables().size();
        if (n > maxGeneratedInlineFunctionMembers)
        {
            moveConstructor->SetCanInline(false);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), moveConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
            cmajor::symbols::ParameterSymbol* thatParam = moveConstructor->Parameters()[1];
            std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(node->GetSpan(), memberVariableSymbol));
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)),
                    thatParam->GetType()->BaseType()->AddPointer(context))));
            std::vector<FunctionScopeLookup> rvalueLookups;
            rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            std::vector<std::unique_ptr<BoundExpression>> rvalueArguments;
            rvalueArguments.push_back(std::move(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> rvalueMemberCall = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, GetBoundCompileUnit(),
                boundFunction.get(), node);
            memberConstructorCallArguments.push_back(std::move(rvalueMemberCall));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, memberConstructorCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!memberConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->MoveTemporaryDestructorCallsTo(*memberConstructorCall);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(memberConstructorCall))));
        }
        GetBoundCompileUnit().AddBoundNode(std::move(boundFunction));
        if (nothrow)
        {
            moveConstructor->SetNothrow();
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create move constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references));
        return false;
    }
    return true;
}

class ClassCopyAssignment : public cmajor::symbols::MemberFunctionSymbol
{
public:
    ClassCopyAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_, BoundCompileUnit* boundCompileUnit);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    bool CanInline() const override { return canInline; }
    void SetCanInline(bool canInline_) { canInline = canInline_; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassCopyAssignment"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
    bool canInline; 
};

ClassCopyAssignment::ClassCopyAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_, BoundCompileUnit* boundCompileUnit) :
    cmajor::symbols::MemberFunctionSymbol(classType_->GetSpan(), U"operator="), classType(classType_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::Context* context = boundCompileUnit->GetContext();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"this");
    thisParam->SetType(classType->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"that");
    thatParam->SetType(classType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (boundCompileUnit)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
            if (compileUnitNode)
            {
                SetCompileUnitId(compileUnitNode->Id());
            }
        }
    }
    ComputeName(context);
}

class ClassCopyAssignmentOperation : public Operation
{
public:
    ClassCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
    bool GenerateImplementation(ClassCopyAssignment* copyAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassCopyAssignmentOperation::ClassCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ClassCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    cmajor::symbols::TypeSymbol* baseType = type->RemovePointer(GetContext())->PlainType(GetContext());
    if (type->PointerCount() != 1 || !baseType->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot assign an instance of a static class", node->GetFullSpan(), classType->GetFullSpan()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(GetContext());
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool conversionFunctionExists = false;
    bool typesEqual = TypesEqual(rightType, classType);
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, argumentMatch, node);
        if (conversion && conversion->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
        {
            conversionFunctionExists = true;
            if (conversion->ReturnsClassInterfaceOrClassDelegateByValue())
            {
                bindToRvalueRef = true; // in this case class move constructor will match
            }
        }
    }
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(GetContext())) && !bindToRvalueRef) && (TypesEqual(rightType, classType) || conversionFunctionExists))
    {
        if (classType->CopyAssignment())
        {
            viableFunctions.Insert(classType->CopyAssignment());
            return;
        }
        if (classType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            GetBoundCompileUnit().FinalizeBinding(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(classType));
        }
        cmajor::symbols::FunctionSymbol* function = functionMap[classType->TypeId()];
        if (!function)
        {
            std::unique_ptr<ClassCopyAssignment> copyAssignment(new ClassCopyAssignment(classType, GetBoundCompileUnit().GetSymbolTable().GetTypeByName(U"void"),
                &GetBoundCompileUnit()));
            function = copyAssignment.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            copyAssignment->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            copyAssignment->SetModule(&GetBoundCompileUnit().GetModule());
            if ((flags & CollectFlags::dontInstantiate) == CollectFlags::none)
            {
                functionMap[classType->TypeId()] = function;
                if (GenerateImplementation(copyAssignment.get(), containerScope, currentFunction, exception, node))
                {
                    functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copyAssignment.release())); // todo
                }
                else
                {
                    functionMap[classType->TypeId()] = nullptr;
                    return;
                }
            }
            else
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copyAssignment.release())); // todo
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassCopyAssignmentOperation::GenerateImplementation(ClassCopyAssignment* copyAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = copyAssignment->ClassType();
    cmajor::symbols::Context* context = GetContext();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), copyAssignment));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseAssignmentCallLookups;
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseAssignmentCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = copyAssignment->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(thisParam->GetSpan(), thisParam)), thisToBaseConversion);
            baseAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = copyAssignment->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), 
                classType->BaseClass()->AddConst(context)->AddLvalueReference(context), containerScope, currentFunction, argumentMatch, node);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* thatArgumentConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion);
            baseAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseAssignmentCall = ResolveOverload(U"operator=", containerScope, baseAssignmentCallLookups, baseAssignmentCallArguments, 
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!baseAssignmentCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseAssignmentCall))));
            copyAssignment->SetCanInline(false);
        }
        int n = classType->MemberVariables().size();
        if (n > maxGeneratedInlineFunctionMembers)
        {
            copyAssignment->SetCanInline(false);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberAssignmentCallLookups;
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(GetContext())));
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberAssignmentCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), copyAssignment->GetThisParam())));
            memberAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
            cmajor::symbols::ParameterSymbol* thatParam = copyAssignment->Parameters()[1];
            BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)),
                    thatParam->GetType()->BaseType()->AddPointer(context))));
            memberAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> memberAssignmentCall = ResolveOverload(U"operator=", containerScope, memberAssignmentCallLookups, memberAssignmentCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!memberAssignmentCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->MoveTemporaryDestructorCallsTo(*memberAssignmentCall);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(memberAssignmentCall))));
        }
        GetBoundCompileUnit().AddBoundNode(std::move(boundFunction));
        if (nothrow)
        {
            copyAssignment->SetNothrow();
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create copy assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references));
        return false;
    }
    return true;
}

class ClassMoveAssignment : public cmajor::symbols::MemberFunctionSymbol
{
public:
    ClassMoveAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_, BoundCompileUnit* boundCompileUnit);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    bool CanInline() const override { return canInline; }
    void SetCanInline(bool canInline_) { canInline = canInline_; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassMoveAssignment"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
    bool canInline;
};

ClassMoveAssignment::ClassMoveAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_, BoundCompileUnit* boundCompileUnit) :
    cmajor::symbols::MemberFunctionSymbol(classType_->GetSpan(), U"operator="), classType(classType_), canInline(true)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::Context* context = boundCompileUnit->GetContext();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"this");
    thisParam->SetType(classType->AddPointer(context));
    AddMember(thisParam, context);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType->GetSpan(), U"that");
    thatParam->SetType(classType->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType_);
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (boundCompileUnit)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
            if (compileUnitNode)
            {
                SetCompileUnitId(compileUnitNode->Id());
            }
        }
    }
    ComputeName(context);
}

class ClassMoveAssignmentOperation : public Operation
{
public:
    ClassMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags) override;
    bool GenerateImplementation(ClassMoveAssignment* moveAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassMoveAssignmentOperation::ClassMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ClassMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, 
    cmajor::ast::Node* node, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    cmajor::symbols::TypeSymbol* baseType = type->RemovePointer(GetContext())->PlainType(GetContext());
    if (type->PointerCount() != 1 || !baseType->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot assign an instance of a static class", node->GetFullSpan(), classType->GetFullSpan()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(GetContext());
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool typesEqual = TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(GetContext()));
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, argumentMatch, node);
        if (conversion && conversion->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
        {
            if (conversion->ReturnsClassInterfaceOrClassDelegateByValue())
            {
                bindToRvalueRef = true;
            }
        }
    }
    if (typesEqual || bindToRvalueRef)
    {
        if (classType->MoveAssignment())
        {
            viableFunctions.Insert(classType->MoveAssignment());
            return;
        }
        if (classType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            GetBoundCompileUnit().FinalizeBinding(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(classType));
        }
        cmajor::symbols::FunctionSymbol* function = functionMap[classType->TypeId()];
        if (!function)
        {
            std::unique_ptr<ClassMoveAssignment> moveAssignment(new ClassMoveAssignment(classType, GetBoundCompileUnit().GetSymbolTable().GetTypeByName(U"void"),
                &GetBoundCompileUnit()));
            function = moveAssignment.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            moveAssignment->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            moveAssignment->SetModule(&GetBoundCompileUnit().GetModule());
            if ((flags & CollectFlags::dontInstantiate) == CollectFlags::none)
            {
                functionMap[classType->TypeId()] = function;
                if (GenerateImplementation(moveAssignment.get(), containerScope, currentFunction, exception, node))
                {
                    functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveAssignment.release()));
                }
                else
                {
                    functionMap[classType->TypeId()] = nullptr;
                    return;
                }
            }
            else
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveAssignment.release()));
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassMoveAssignmentOperation::GenerateImplementation(ClassMoveAssignment* moveAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = moveAssignment->ClassType();
    cmajor::symbols::Context* context = GetContext();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), moveAssignment));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseAssignmentCallLookups;
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseAssignmentCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = moveAssignment->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            std::unique_ptr<BoundExpression> baseClassPointerConversion(new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion));
            baseAssignmentCallArguments.push_back(std::move(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = moveAssignment->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), 
                classType->BaseClass()->AddRvalueReference(context), containerScope, currentFunction, argumentMatch, node);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            std::unique_ptr<BoundExpression> thatArgumentConversion(new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion));
            baseAssignmentCallArguments.push_back(std::move(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseAssignmentCall = ResolveOverload(U"operator=", containerScope, baseAssignmentCallLookups, baseAssignmentCallArguments, 
                GetBoundCompileUnit(), boundFunction.get(), node);
            if (!baseAssignmentCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseAssignmentCall))));
            moveAssignment->SetCanInline(false);
        }
        int n = classType->MemberVariables().size();
        if (n > maxGeneratedInlineFunctionMembers)
        {
            moveAssignment->SetCanInline(false);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::unique_ptr<BoundMemberVariable> boundMemberVariable(new BoundMemberVariable(node->GetSpan(), memberVariableSymbol));
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), moveAssignment->GetThisParam())));
            cmajor::symbols::ParameterSymbol* thatParam = moveAssignment->Parameters()[1];
            std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(node->GetSpan(), memberVariableSymbol));
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thatParam)),
                    thatParam->GetType()->BaseType()->AddPointer(context))));
            std::vector<FunctionScopeLookup> swapLookups;
            swapLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            swapLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            std::vector<std::unique_ptr<BoundExpression>> swapArguments;
            swapArguments.push_back(std::move(boundMemberVariable));
            swapArguments.push_back(std::move(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> swapMemberCall = ResolveOverload(U"System.Swap", containerScope, swapLookups, swapArguments, GetBoundCompileUnit(), 
                boundFunction.get(), node);
            if (!swapMemberCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(swapMemberCall))));
        }
        GetBoundCompileUnit().AddBoundNode(std::move(boundFunction));
        if (nothrow)
        {
            moveAssignment->SetNothrow();
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create move assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references));
        return false;
    }
    return true;
}

void GenerateDestructorImplementation(BoundClass* boundClass, cmajor::symbols::DestructorSymbol* destructorSymbol, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    cmajor::symbols::ClassTypeSymbol* classType = boundClass->GetClassTypeSymbol();
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    try
    {
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, destructorSymbol));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(node->GetSpan())));
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = destructorSymbol->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), 
                    vmtPtrHolderClass->AddPointer(context), containerScope, currentFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int n = classType->MemberVariables().size();
        for (int i = n - 1; i >= 0; --i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            if (memberVariableSymbol->GetType()->HasNontrivialDestructor())
            {
                std::vector<FunctionScopeLookup> memberDestructorCallLookups;
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                    memberVariableSymbol->GetType()->BaseType()->ClassInterfaceOrNsScope(context)));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> memberDestructorCallArguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), destructorSymbol->GetThisParam())));
                memberDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                std::unique_ptr<BoundFunctionCall> memberDestructorCall = ResolveOverload(
                    U"@destructor", containerScope, memberDestructorCallLookups, memberDestructorCallArguments,
                    boundCompileUnit, boundFunction.get(), node);
                boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(memberDestructorCall), node->GetSpan())));
            }
        }
        if (classType->BaseClass() && classType->BaseClass()->HasNontrivialDestructor())
        {
            std::vector<FunctionScopeLookup> baseDestructorCallLookups;
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseDestructorCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = destructorSymbol->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, currentFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)),
                thisToBaseConversion);
            baseDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseDestructorCall = ResolveOverload(U"@destructor", containerScope, baseDestructorCallLookups, baseDestructorCallArguments, 
                boundCompileUnit, boundFunction.get(), node);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(baseDestructorCall), node->GetSpan())));
        }
        boundClass->AddMember(std::move(boundFunction));
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("cannot create destructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), node->GetFullSpan(), references);
    }
}

BoundExpression* MakeExitEntryPtr(BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, cmajor::ast::Node* node)
{
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(U"System.ExitEntry", cmajor::symbols::ScopeLookup::this_and_base_and_parent, boundCompileUnit.GetContext());
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
            {
                cmajor::symbols::ClassGroupTypeSymbol* classGroupSymbol = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(symbol);
                symbol = classGroupSymbol->GetClass(0);
            }
            std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().Lock());
            cmajor::symbols::TypeSymbol* exitEntryType = static_cast<cmajor::symbols::TypeSymbol*>(symbol);
            cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(boundCompileUnit.GetSymbolTable(), boundCompileUnit.GetContext());
            cmajor::ast::GlobalVariableNode globalVariableNode(node->GetSpan(), cmajor::ast::Specifiers::private_, new cmajor::ast::DotNode(node->GetSpan(),
                new cmajor::ast::IdentifierNode(node->GetSpan(), U"System"),
                new cmajor::ast::IdentifierNode(node->GetSpan(), U"ExitEntry")),
                new cmajor::ast::IdentifierNode(node->GetSpan(), U"exit@entry@" +
                    util::ToUtf32(std::to_string(boundCompileUnit.GetNextExitEntryIndex()))), nullptr, boundCompileUnit.GetCompileUnitNode());
            globalVariableNode.Accept(symbolCreatorVisitor);
            TypeBinder typeBinder(boundCompileUnit);
            typeBinder.SetContainerScope(containerScope);
            globalVariableNode.Accept(typeBinder);
            BoundGlobalVariable* exitEntryGlobalVariable = static_cast<BoundGlobalVariable*>(typeBinder.GetBoundGlobalVariable()->Clone());
            return new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(exitEntryGlobalVariable), exitEntryType->AddPointer(boundCompileUnit.GetContext()));
        }
        else
        {
            throw cmajor::symbols::Exception("System.ExitEntry expected to denote a type", node->GetFullSpan());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("System.ExitEntry symbol not found", node->GetFullSpan());
    }
}

void GenerateStaticClassInitialization(cmajor::symbols::StaticConstructorSymbol* staticConstructorSymbol, cmajor::ast::StaticConstructorNode* staticConstructorNode, 
    BoundCompileUnit& boundCompileUnit, BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction, cmajor::symbols::ContainerScope* containerScope, 
    StatementBinder* statementBinder, cmajor::ast::Node* node)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::Symbol* parent = staticConstructorSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    try
    {
        Assert(classType->InitializedVar(), "initialized variable expected");
        std::unique_ptr<BoundIfStatement> ifStatement(new BoundIfStatement(node->GetSpan(), std::unique_ptr<BoundExpression>(
            new BoundMemberVariable(node->GetSpan(), classType->InitializedVar())),
            std::unique_ptr<BoundStatement>(new BoundReturnStatement(std::unique_ptr<BoundFunctionCall>(nullptr), node->GetSpan())), std::unique_ptr<BoundStatement>(nullptr)));
        boundCompoundStatement->AddStatement(std::move(ifStatement));
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || 
            cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp || 
            cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm ||
            cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
        {
            cmajor::ast::IdentifierNode staticInitCriticalSection(node->GetSpan(), U"System.Runtime.StaticInitCriticalSection");
            cmajor::symbols::TypeSymbol* staticInitCriticalSectionClassType = ResolveType(&staticInitCriticalSection, boundCompileUnit, containerScope);
            std::vector<FunctionScopeLookup> constructorLookups;
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, staticInitCriticalSectionClassType->ClassInterfaceOrNsScope(context)));
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> constructorArguments;
            bool immutable = staticConstructorSymbol->GetModule()->IsImmutable();
            cmajor::symbols::LocalVariableSymbol* temporary = staticConstructorSymbol->CreateTemporary(
                staticInitCriticalSectionClassType, node->GetSpan(), context, !immutable);
            if (immutable)
            {
                boundFunction->AddTemporary(temporary);
            }
            constructorArguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
                new BoundLocalVariable(node->GetSpan(), temporary)), staticInitCriticalSectionClassType->AddPointer(context))));
            std::unique_ptr<BoundConstructionStatement> constructionStatement(new BoundConstructionStatement(
                ResolveOverload(U"@constructor", containerScope, constructorLookups, constructorArguments, boundCompileUnit, boundFunction, node), node->GetSpan()));
            boundCompoundStatement->AddStatement(std::move(constructionStatement));
            std::unique_ptr<BoundIfStatement> ifStatement2(new BoundIfStatement(node->GetSpan(), std::unique_ptr<BoundExpression>(
                new BoundMemberVariable(node->GetSpan(), classType->InitializedVar())),
                std::unique_ptr<BoundStatement>(new BoundReturnStatement(std::unique_ptr<BoundFunctionCall>(nullptr), node->GetSpan())), std::unique_ptr<BoundStatement>(nullptr)));
            boundCompoundStatement->AddStatement(std::move(ifStatement2));
        }
        std::vector<FunctionScopeLookup> assignmentLookups;
        assignmentLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        assignmentLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> assignmentArguments;
        assignmentArguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundMemberVariable(node->GetSpan(), 
            classType->InitializedVar())), classType->InitializedVar()->GetType()->AddPointer(context))));
        assignmentArguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::BoolValue(node->GetSpan(), true)),
            boundCompileUnit.GetSymbolTable().GetTypeByName(U"bool"))));
        std::unique_ptr<BoundAssignmentStatement> assignmentStatement(new BoundAssignmentStatement(
            ResolveOverload(U"operator=", containerScope, assignmentLookups, assignmentArguments, boundCompileUnit, boundFunction, node), node->GetSpan()));
        boundCompoundStatement->AddStatement(std::move(assignmentStatement));
        cmajor::symbols::ClassTypeSymbol* baseClass = classType->BaseClass();
        if (baseClass)
        {
            if (baseClass->StaticConstructor())
            {
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                    new BoundFunctionCall(node->GetSpan(), baseClass->StaticConstructor())))));
            }
        }
        std::unordered_map<std::u32string, cmajor::ast::MemberInitializerNode*> memberInitializerMap;
        int n = staticConstructorNode->Initializers().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::InitializerNode* initializer = staticConstructorNode->Initializers()[i];
            if (initializer->GetNodeType() == cmajor::ast::NodeType::thisInitializerNode)
            {
                throw cmajor::symbols::Exception("static constructor cannot have 'this' initializers", node->GetFullSpan(), initializer->GetFullSpan());
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::baseInitializerNode)
            {
                throw cmajor::symbols::Exception("static constructor cannot have 'base' initializers", node->GetFullSpan(), initializer->GetFullSpan());
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::memberInitializerNode)
            {
                cmajor::ast::MemberInitializerNode* memberInitializer = static_cast<cmajor::ast::MemberInitializerNode*>(initializer);
                std::u32string memberName = memberInitializer->MemberId()->Str();
                auto it = memberInitializerMap.find(memberName);
                if (it != memberInitializerMap.cend())
                {
                    throw cmajor::symbols::Exception("already has initializer for member variable '" + util::ToUtf8(memberName) + "'", node->GetFullSpan(), initializer->GetFullSpan());
                }
                memberInitializerMap[memberName] = memberInitializer;
            }
        }
        int nm = classType->StaticMemberVariables().size();
        for (int i = 0; i < nm; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->StaticMemberVariables()[i];
            auto it = memberInitializerMap.find(memberVariableSymbol->Name());
            if (it != memberInitializerMap.cend())
            {
                cmajor::ast::MemberInitializerNode* memberInitializer = it->second;
                memberInitializerMap.erase(memberInitializer->MemberId()->Str());
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable),
                    boundMemberVariable->GetType()->AddPointer(context)));
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable2;
                cmajor::symbols::ClassTypeSymbol* memberVariableClassTypeWithDestructor = nullptr;
                if (memberVariableSymbol->GetType()->HasNontrivialDestructor())
                {
                    addrOfBoundMemberVariable2.reset(addrOfBoundMemberVariable->Clone());
                    memberVariableClassTypeWithDestructor = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableSymbol->GetType()->BaseType());
                }
                arguments.push_back(std::move(addrOfBoundMemberVariable));
                int n = memberInitializer->Arguments().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::Node* argumentNode = memberInitializer->Arguments()[i];
                    std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(argument));
                }
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                if (memberVariableClassTypeWithDestructor)
                {
                    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* enqueueDestructorFunction = U"RtEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, enqueueArguments, 
                            boundCompileUnit, boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                    {
                        std::vector<FunctionScopeLookup> atExitLookups;
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> atExitArguments;
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(MakeExitEntryPtr(boundCompileUnit, containerScope, node)));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* atExitFunction = U"at_exit";
                        std::unique_ptr<BoundFunctionCall> atExitCall = ResolveOverload(atExitFunction, containerScope, atExitLookups, atExitArguments, boundCompileUnit,
                            boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(atExitCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin ||
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp ||
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* enqueueDestructorFunction = U"RtmEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, enqueueArguments,
                            boundCompileUnit, boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                }
            }
            else
            {
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context)));
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable2;
                cmajor::symbols::ClassTypeSymbol* memberVariableClassTypeWithDestructor = nullptr;
                if (memberVariableSymbol->GetType()->HasNontrivialDestructor())
                {
                    addrOfBoundMemberVariable2.reset(addrOfBoundMemberVariable->Clone());
                    memberVariableClassTypeWithDestructor = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableSymbol->GetType()->BaseType());
                }
                arguments.push_back(std::move(addrOfBoundMemberVariable));
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                if (memberVariableClassTypeWithDestructor)
                {
                    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* enqueueDestructorFunction = U"RtEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, 
                            enqueueArguments, boundCompileUnit, boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                    {
                        std::vector<FunctionScopeLookup> atExitLookups;
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> atExitArguments;
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(MakeExitEntryPtr(boundCompileUnit, containerScope, node)));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* atExitFunction = U"at_exit";
                        std::unique_ptr<BoundFunctionCall> atExitCall = ResolveOverload(atExitFunction, containerScope, atExitLookups, atExitArguments, boundCompileUnit,
                            boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(atExitCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin ||
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp ||
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(node->GetSpan(),
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(context))));
                        const char32_t* enqueueDestructorFunction = U"RtmEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, enqueueArguments,
                            boundCompileUnit, boundFunction, node);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                }
            }
        }
        if (!memberInitializerMap.empty())
        {
            cmajor::ast::MemberInitializerNode* initializer = memberInitializerMap.begin()->second;
            throw cmajor::symbols::Exception("no static member variable found for initializer named '" + util::ToUtf8(initializer->MemberId()->Str()) + "'",
                node->GetFullSpan(), initializer->GetFullSpan());

        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate static initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(),
            staticConstructorNode->GetFullSpan(), references);
    }
}

void GenerateClassInitialization(cmajor::symbols::ConstructorSymbol* constructorSymbol, cmajor::ast::ConstructorNode* constructorNode, BoundCompoundStatement* boundCompoundStatement, 
    BoundFunction* boundFunction, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool generateDefault, 
    cmajor::ast::Node* node)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::Symbol* parent = constructorSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected"); 
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    if (generateDefault)
    {
        if (classType->IsStatic())
        {
            throw cmajor::symbols::Exception("cannot create default initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: class is static", 
                node->GetFullSpan());
        }
    }
    try
    {
        if (classType->StaticConstructor())
        {
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                new BoundFunctionCall(node->GetSpan(), classType->StaticConstructor())))));
        }
        cmajor::symbols::ParameterSymbol* thisParam = constructorSymbol->GetThisParam();
        Assert(thisParam, "this parameter expected");
        cmajor::ast::ThisInitializerNode* thisInitializer = nullptr;
        cmajor::ast::BaseInitializerNode* baseInitializer = nullptr;
        std::unordered_map<std::u32string, cmajor::ast::MemberInitializerNode*> memberInitializerMap;
        int ni = constructorNode->Initializers().Count();
        for (int i = 0; i < ni; ++i)
        {
            cmajor::ast::InitializerNode* initializer = constructorNode->Initializers()[i];
            if (initializer->GetNodeType() == cmajor::ast::NodeType::thisInitializerNode)
            {
                if (thisInitializer)
                {
                    throw cmajor::symbols::Exception("already has 'this' initializer", node->GetFullSpan(), initializer->GetFullSpan());
                }
                else if (baseInitializer)
                {
                    throw cmajor::symbols::Exception("cannot have both 'this' and 'base' initializer", node->GetFullSpan(), initializer->GetFullSpan());
                }
                thisInitializer = static_cast<cmajor::ast::ThisInitializerNode*>(initializer);
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::baseInitializerNode)
            {
                if (baseInitializer)
                {
                    throw cmajor::symbols::Exception("already has 'base' initializer", node->GetFullSpan(), initializer->GetFullSpan());
                }
                else if (thisInitializer)
                {
                    throw cmajor::symbols::Exception("cannot have both 'this' and 'base' initializer", node->GetFullSpan(), initializer->GetFullSpan());
                }
                baseInitializer = static_cast<cmajor::ast::BaseInitializerNode*>(initializer);
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::memberInitializerNode)
            {
                cmajor::ast::MemberInitializerNode* memberInitializer = static_cast<cmajor::ast::MemberInitializerNode*>(initializer);
                std::u32string memberName = memberInitializer->MemberId()->Str();
                auto it = memberInitializerMap.find(memberName);
                if (it != memberInitializerMap.cend())
                {
                    throw cmajor::symbols::Exception("already has initializer for member variable '" + util::ToUtf8(memberName) + "'", node->GetFullSpan(), initializer->GetFullSpan());
                }
                memberInitializerMap[memberName] = memberInitializer;
            }
        }
        if (thisInitializer)
        {
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->GetContainerScope()));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
            int n = thisInitializer->Arguments().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* argumentNode = thisInitializer->Arguments()[i];
                std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                arguments.push_back(std::move(argument));
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, 
                boundFunction, node, OverloadResolutionFlags::none, templateArgumentTypes, exception);
            boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
        }
        else if (baseInitializer)
        {
            if (!classType->BaseClass())
            {
                throw cmajor::symbols::Exception("class '" + util::ToUtf8(classType->FullName()) + "' does not have a base class", node->GetFullSpan(), 
                    baseInitializer->GetFullSpan());
            }
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, boundFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
            arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            int n = baseInitializer->Arguments().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* argumentNode = baseInitializer->Arguments()[i];
                std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                arguments.push_back(std::move(argument));
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, 
                node, OverloadResolutionFlags::none, templateArgumentTypes, exception);
            boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
        }
        else if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, boundFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
            arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            bool copyConstructor = constructorSymbol->IsCopyConstructor(context);
            if (copyConstructor)
            {
                cmajor::symbols::ParameterSymbol* thatParam = constructorSymbol->Parameters()[1];
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddConst(context)->AddLvalueReference(context), containerScope, boundFunction, argumentMatch, node);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                    new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, 
                node, OverloadResolutionFlags::none, templateArgumentTypes, exception);
            boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
        }
        if (classType->IsPolymorphic() && !thisInitializer)
        {
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(context), 
                    containerScope, boundFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int nm = classType->MemberVariables().size();
        for (int i = 0; i < nm; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            auto it = memberInitializerMap.find(memberVariableSymbol->Name());
            if (it != memberInitializerMap.cend())
            {
                cmajor::ast::MemberInitializerNode* memberInitializer = it->second;
                memberInitializerMap.erase(memberInitializer->MemberId()->Str());
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                arguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                int n = memberInitializer->Arguments().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::Node* argumentNode = memberInitializer->Arguments()[i];
                    std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(argument));
                }
                OverloadResolutionFlags flags = OverloadResolutionFlags::none;
                std::unique_ptr<cmajor::symbols::Exception> exception;
                std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, 
                    node, flags, templateArgumentTypes, exception);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
            }
            else if (!thisInitializer)
            {
                if (constructorSymbol->IsCopyConstructor(context))
                {
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                    cmajor::ast::CloneContext cloneContext;
                    cmajor::ast::DotNode thatMemberVarNode(node->GetSpan(), constructorNode->Parameters()[0]->Clone(cloneContext),
                        new cmajor::ast::IdentifierNode(node->GetSpan(), memberVariableSymbol->Name()));
                    std::unique_ptr<BoundExpression> thatMemberVarArgument = BindExpression(&thatMemberVarNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(thatMemberVarArgument));
                    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                    boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                }
                else if (constructorSymbol->IsMoveConstructor(context))
                {
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                    cmajor::symbols::ParameterSymbol* thatParam = constructorSymbol->Parameters()[1];
                    std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(node->GetSpan(), memberVariableSymbol));
                    thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                        new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(
                            new BoundParameter(node->GetSpan(), thatParam)), thatParam->GetType()->BaseType()->AddPointer(context))));
                    std::vector<FunctionScopeLookup> rvalueLookups;
                    rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    std::vector<std::unique_ptr<BoundExpression>> rvalueArguments;
                    rvalueArguments.push_back(std::move(thatBoundMemberVariable));
                    std::unique_ptr<BoundFunctionCall> rvalueMemberCall = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, boundCompileUnit,
                        boundFunction, node);
                    arguments.push_back(std::move(rvalueMemberCall));
                    std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                        node);
                    boundFunction->MoveTemporaryDestructorCallsTo(*memberConstructorCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(memberConstructorCall))));
                }
                else
                {
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                    std::unique_ptr<cmajor::symbols::Exception> exception;
                    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
                    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, 
                        node, OverloadResolutionFlags::none, templateArgumentTypes, exception);
                    boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                }
            }
        }
        if (!memberInitializerMap.empty())
        {
            cmajor::ast::MemberInitializerNode* initializer = memberInitializerMap.begin()->second;
            throw cmajor::symbols::Exception("no member variable found for initializer named '" + util::ToUtf8(initializer->MemberId()->Str()) + "'",
                node->GetFullSpan(), initializer->GetFullSpan());
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references);
    }
}

void GenerateClassAssignment(cmajor::symbols::MemberFunctionSymbol* assignmentFunctionSymbol, cmajor::ast::MemberFunctionNode* assignmentNode, 
    BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, 
    StatementBinder* statementBinder, bool generateDefault, cmajor::ast::Node* node)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::Symbol* parent = assignmentFunctionSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    if (generateDefault)
    {
        if (classType->IsStatic())
        {
            throw cmajor::symbols::Exception("cannot create default assigment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: class is static", 
                node->GetFullSpan(), classType->GetFullSpan());
        }
    }
    try
    {
        cmajor::symbols::ParameterSymbol* thisParam = assignmentFunctionSymbol->GetThisParam();
        Assert(thisParam, "this parameter expected");
        if (assignmentFunctionSymbol->IsCopyAssignment(context))
        {
            if (classType->BaseClass())
            {
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                    containerScope, boundFunction, argumentMatch, node);
                if (!thisToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                    new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
                cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddConst(context)->AddLvalueReference(context), containerScope, boundFunction, argumentMatch, node);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                    new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
                std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                boundFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(assignmentCall))));
            }
            if (generateDefault)
            {
                int n = classType->MemberVariables().size();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                    cmajor::ast::CloneContext cloneContext;
                    cmajor::ast::DotNode thatMemberVarNode(node->GetSpan(), assignmentNode->Parameters()[0]->Clone(cloneContext),
                        new cmajor::ast::IdentifierNode(node->GetSpan(), memberVariableSymbol->Name()));
                    std::unique_ptr<BoundExpression> thatMemberVarArgument = BindExpression(&thatMemberVarNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(thatMemberVarArgument));
                    std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                    boundFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(assignmentCall))));
                }
            }
        }
        else if (assignmentFunctionSymbol->IsMoveAssignment(context))
        {
            if (classType->BaseClass())
            {
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                    containerScope, boundFunction, argumentMatch, node);
                if (!thisToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                    new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
                cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddRvalueReference(context), containerScope, boundFunction, argumentMatch, node);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                    new BoundParameter(node->GetSpan(), thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
                std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                boundFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(assignmentCall))));
            }
            if (generateDefault)
            {
                int n = classType->MemberVariables().size();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(boundMemberVariable));
                    BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                    cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                    cmajor::symbols::TypeSymbol* thatPtrType = thatParam->GetType()->RemoveReference(context)->AddPointer(context);
                    thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(
                        new BoundParameter(node->GetSpan(), thatParam)), thatPtrType)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
                    std::unique_ptr<BoundFunctionCall> swapCall = ResolveOverload(U"System.Swap", containerScope, lookups, arguments, boundCompileUnit, boundFunction, node);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(swapCall))));
                }
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references);
    }
}

void GenerateClassTermination(cmajor::symbols::DestructorSymbol* destructorSymbol, cmajor::ast::DestructorNode* destructorNode, BoundCompoundStatement* boundCompoundStatement, 
    BoundFunction* boundFunction, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, cmajor::ast::Node* node)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::Symbol* parent = destructorSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    try
    {
        cmajor::symbols::ParameterSymbol* thisParam = destructorSymbol->GetThisParam();
        Assert(thisParam, "this parameter expected");
        if (classType->IsPolymorphic())
        {
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(node->GetSpan(), thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(context), containerScope, 
                    boundFunction, argumentMatch, node);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), thisParam)), thisToHolderConversion);
            }
            boundCompoundStatement->InsertStatementToFront(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int n = classType->MemberVariables().size();
        for (int i = n - 1; i >= 0; --i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            if (memberVariableSymbol->GetType()->HasNontrivialDestructor())
            {
                std::vector<FunctionScopeLookup> memberDestructorCallLookups;
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, 
                    memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope(context)));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> memberDestructorCallArguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(node->GetSpan(), memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(node->GetSpan(), destructorSymbol->GetThisParam())));
                memberDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(context))));
                std::unique_ptr<BoundFunctionCall> memberDestructorCall = ResolveOverload(U"@destructor", containerScope, memberDestructorCallLookups, memberDestructorCallArguments,
                    boundCompileUnit, boundFunction, node);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(memberDestructorCall), node->GetSpan())));
            }
        }
        if (classType->BaseClass() && classType->BaseClass()->HasNontrivialDestructor())
        {
            std::vector<FunctionScopeLookup> baseDestructorCallLookups;
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseDestructorCallArguments;
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(context), 
                containerScope, boundFunction, argumentMatch, node);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", node->GetFullSpan(), classType->GetFullSpan());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(
                new BoundParameter(node->GetSpan(), thisParam)), thisToBaseConversion);
            baseDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseDestructorCall = ResolveOverload(U"@destructor", containerScope, baseDestructorCallLookups, baseDestructorCallArguments, 
                boundCompileUnit, boundFunction, node);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(baseDestructorCall), node->GetSpan())));
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate termination for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references);
    }
}

Operation::Operation(const std::u32string& groupName_, int arity_, BoundCompileUnit& boundCompileUnit_) : 
    groupName(groupName_), arity(arity_), boundCompileUnit(boundCompileUnit_), context(boundCompileUnit.GetContext())
{
}

Operation::~Operation()
{
}

cmajor::symbols::SymbolTable* Operation::GetSymbolTable()
{
    return &boundCompileUnit.GetSymbolTable();
}

BoundCompileUnit& Operation::GetBoundCompileUnit()
{
    return boundCompileUnit;
}

void ArityOperation::Add(Operation* operation)
{
    operations.push_back(operation);
}

void ArityOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    for (Operation* operation : operations)
    {
        operation->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, node, flags);
    }
}

void OperationGroup::Add(Operation* operation)
{
    int arity = operation->Arity();
    if (arity >= arityOperations.size())
    {
        arityOperations.resize(arity + 1);
    }
    ArityOperation* arityOperation = arityOperations[arity].get();
    if (!arityOperation)
    {
        arityOperation = new ArityOperation();
        arityOperations[arity].reset(arityOperation);
    }
    arityOperation->Add(operation);
}

void OperationGroup::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, 
    CollectFlags flags)
{
    int arity = arguments.size();
    if (arity < arityOperations.size())
    {
        ArityOperation* arityOperation = arityOperations[arity].get();
        if (arityOperation)
        {
            arityOperation->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, node, flags);
        }
    }
}

OperationRepository::OperationRepository(BoundCompileUnit& boundCompileUnit_) : boundCompileUnit(boundCompileUnit_), copyConstructorOperation(nullptr)
{
    Add(new LvalueReferenceCopyConstructorOperation(boundCompileUnit));
    Add(new LvalueReferenceCopyAssignmentOperation(boundCompileUnit));
    Add(new LvalueReferenceMoveAssignmentOperation(boundCompileUnit));
    Add(new LvalueReferenceReturnOperation(boundCompileUnit));
    Add(new RvalueReferenceCopyConstructorOperation(boundCompileUnit));
    Add(new RvalueReferenceCopyAssignmentOperation(boundCompileUnit));
    Add(new RvalueReferenceReturnOperation(boundCompileUnit));
    Add(new PointerDefaultConstructorOperation(boundCompileUnit));
    Add(new PointerCopyConstructorOperation(boundCompileUnit));
    Add(new PointerMoveConstructorOperation(boundCompileUnit));
    Add(new PointerCopyAssignmentOperation(boundCompileUnit));
    Add(new PointerMoveAssignmentOperation(boundCompileUnit));
    Add(new PointerReturnOperation(boundCompileUnit));
    Add(new PointerPlusOffsetOperation(boundCompileUnit));
    Add(new OffsetPlusPointerOperation(boundCompileUnit));
    Add(new PointerMinusOffsetOperation(boundCompileUnit));
    Add(new PointerMinusPointerOperation(boundCompileUnit));
    Add(new PointerEqualOperation(boundCompileUnit));
    Add(new PointerLessOperation(boundCompileUnit));
    Add(new PointerArrowOperation(boundCompileUnit));
    Add(new ClassDefaultConstructorOperation(boundCompileUnit));
    copyConstructorOperation = new ClassCopyConstructorOperation(boundCompileUnit);
    Add(copyConstructorOperation);
    Add(new ClassMoveConstructorOperation(boundCompileUnit));
    Add(new ClassCopyAssignmentOperation(boundCompileUnit));
    Add(new ClassMoveAssignmentOperation(boundCompileUnit));
    Add(new ArrayDefaultConstructorOperation(boundCompileUnit));
    Add(new ArrayCopyConstructorOperation(boundCompileUnit));
    Add(new ArrayMoveConstructorOperation(boundCompileUnit));
    Add(new ArrayCopyAssignmentOperation(boundCompileUnit));
    Add(new ArrayMoveAssignmentOperation(boundCompileUnit));
    Add(new ArrayElementAccessOperation(boundCompileUnit));
}

void OperationRepository::Add(Operation* operation)
{
    OperationGroup* group = nullptr;
    auto it = operationGroupMap.find(operation->GroupName());
    if (it != operationGroupMap.cend())
    {
        group = it->second;
    }
    else
    {
        group = new OperationGroup();
        operationGroupMap.insert(std::make_pair(operation->GroupName(), group));
        operationGroups.push_back(std::unique_ptr<OperationGroup>(group));
    }
    group->Add(operation);
    operations.push_back(std::unique_ptr<Operation>(operation));
}

void OperationRepository::CollectViableFunctions(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, 
    const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, 
    std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags)
{
    auto it = operationGroupMap.find(groupName);
    if (it != operationGroupMap.cend())
    {
        OperationGroup* operationGroup = it->second;
        operationGroup->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, node, flags);
    }
}

void OperationRepository::GenerateCopyConstructorFor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
    BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    if (boundCompileUnit.HasCopyConstructorFor(classTypeSymbol->TypeId())) return;
    std::unique_ptr<ClassCopyConstructor> copyConstructor(new ClassCopyConstructor(classTypeSymbol, currentFunction->GetBoundCompileUnit()));
    copyConstructor->SetCompileUnit(boundCompileUnit.GetCompileUnitNode());
    copyConstructor->SetModule(&boundCompileUnit.GetModule());
    ClassCopyConstructorOperation* copyConstructorOp = static_cast<ClassCopyConstructorOperation*>(copyConstructorOperation);
    std::unique_ptr<cmajor::symbols::Exception> exception;
    if (copyConstructorOp->GenerateImplementation(copyConstructor.get(), containerScope, currentFunction, exception, node))
    {
        copyConstructor->SetModule(&boundCompileUnit.GetModule());
        copyConstructor->SetParent(classTypeSymbol);
        copyConstructor->SetLinkOnceOdrLinkage();
        boundCompileUnit.AddCopyConstructorFor(classTypeSymbol->TypeId(), std::move(copyConstructor));
    }
    else
    {
        if (exception)
        {
            throw* exception;
        }
        else
        {
            throw cmajor::symbols::Exception("could not generate copy constructor for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "'", node->GetFullSpan());
        }
    }
}

void OperationRepository::GenerateCopyConstructorFor(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
    cmajor::symbols::Context* context, cmajor::ast::Node* node)
{
    if (boundCompileUnit.HasCopyConstructorFor(interfaceTypeSymbol->TypeId())) return;
    std::unique_ptr<cmajor::symbols::InterfaceTypeCopyConstructor> copyConstructor(new cmajor::symbols::InterfaceTypeCopyConstructor(interfaceTypeSymbol, context));
    boundCompileUnit.GetSymbolTable().SetFunctionIdFor(copyConstructor.get());
    copyConstructor->SetCompileUnit(boundCompileUnit.GetCompileUnitNode());
    copyConstructor->SetModule(&boundCompileUnit.GetModule());
    copyConstructor->SetParent(interfaceTypeSymbol);
    boundCompileUnit.AddCopyConstructorFor(interfaceTypeSymbol->TypeId(), std::move(copyConstructor));
}

} // namespace cmajor::binder
