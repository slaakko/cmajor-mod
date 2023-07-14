// =================================
// Copyright (c) 2023 Seppo Laakko
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
    PointerDefaultCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerDefaultCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
    void* nullValue;
};

PointerDefaultCtor::PointerDefaultCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@constructor"), type(type_), nullValue(nullptr)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    ComputeName();
}

void PointerDefaultCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 1, "default constructor needs one object"); 
    if (!nullValue)
    {
        nullValue = emitter.CreateDefaultIrValueForPtrType(type->IrType(emitter));
    }
    emitter.Stack().Push(nullValue);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerDefaultConstructorOperation : public Operation
{
public:
    PointerDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerDefaultConstructorOperation::PointerDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void PointerDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
    if (!function)
    {
        function = new PointerDefaultCtor(pointerType, sourcePos, moduleId);
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
    PointerCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerCopyCtor::PointerCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    ComputeName();
}

void PointerCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerCopyConstructorOperation::PointerCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void PointerCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerCopyCtor(pointerType, sourcePos, moduleId);
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
    PointerMoveCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMoveCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerMoveCtor::PointerMoveCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type->AddRvalueReference(sourcePos, moduleId));
    AddMember(thatParam);
    ComputeName();
}

void PointerMoveCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "move constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), rvalueRefValue)); // TODO
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMoveConstructorOperation::PointerMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void PointerMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
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
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    if (TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerMoveCtor(pointerType, sourcePos, moduleId);
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
    PointerCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerCopyAssignment::PointerCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

void PointerCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerCopyAssignmentOperation::PointerCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void PointerCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->IsReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerCopyAssignment(pointerType, GetSymbolTable()->GetTypeByName(U"void"), sourcePos, moduleId);
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
    PointerMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMoveAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerMoveAssignment::PointerMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type->AddRvalueReference(sourcePos, moduleId));
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

void PointerMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class PointerMoveAssignmentOperation : public Operation
{
public:
    PointerMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMoveAssignmentOperation::PointerMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void PointerMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
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
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    if (TypesEqual(arguments[1]->GetType(), pointerType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
        if (!function)
        {
            function = new PointerMoveAssignment(pointerType, GetSymbolTable()->GetTypeByName(U"void"), sourcePos, moduleId);
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
    PointerReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerReturn::PointerReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"value");
    valueParam->SetType(type);
    AddMember(valueParam);
    SetReturnType(type);
    ComputeName();
}

void PointerReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerReturnOperation::PointerReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void PointerReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
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
        function = new PointerReturn(type, sourcePos, moduleId);
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
    PointerPlusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerPlusOffset"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerPlusOffset::PointerPlusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::SourcePos& sourcePos,
    const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator+"), pointerType(pointerType_)
{
    SetGroupName(U"operator+");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(pointerType);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(longType_);
    AddMember(rightParam);
    SetReturnType(pointerType);
    ComputeName();
}

void PointerPlusOffset::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "operator+ needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(sourcePos, moduleId)->IrType(emitter), left, right));
}

class PointerPlusOffsetOperation : public Operation
{
public:
    PointerPlusOffsetOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerPlusOffsetOperation::PointerPlusOffsetOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator+", 2, boundCompileUnit_)
{
}

void PointerPlusOffsetOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(sourcePos, moduleId)->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, sourcePos, moduleId, argumentMatch))
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
        function = new PointerPlusOffset(leftType, GetSymbolTable()->GetTypeByName(U"long"), sourcePos, moduleId);
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
    OffsetPlusPointer(cmajor::symbols::TypeSymbol* longType_, cmajor::symbols::TypeSymbol* pointerType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "OffsetPlusPointer"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

OffsetPlusPointer::OffsetPlusPointer(cmajor::symbols::TypeSymbol* longType_, cmajor::symbols::TypeSymbol* pointerType_, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator+"), pointerType(pointerType_)
{
    SetGroupName(U"operator+");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(longType_);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(pointerType);
    AddMember(rightParam);
    SetReturnType(pointerType);
    ComputeName();
}

void OffsetPlusPointer::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "operator+ needs two objects"); 
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(sourcePos, moduleId)->IrType(emitter), right, left));
}

class OffsetPlusPointerOperation : public Operation
{
public:
    OffsetPlusPointerOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

OffsetPlusPointerOperation::OffsetPlusPointerOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator+", 2, boundCompileUnit_)
{
}

void OffsetPlusPointerOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->PlainType(sourcePos, moduleId)->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(leftType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, sourcePos, moduleId, argumentMatch))
        {
            return;
        }
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* longType = GetSymbolTable()->GetTypeByName(U"long");
    if (rightType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(rightType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[rightType];
    if (!function)
    {
        function = new OffsetPlusPointer(longType, rightType, sourcePos, moduleId);
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
    PointerMinusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMinusOffset"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerMinusOffset::PointerMinusOffset(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator-"), pointerType(pointerType_)
{
    SetGroupName(U"operator-");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(longType_);
    AddMember(rightParam);
    SetReturnType(pointerType_);
    ComputeName();
}

void PointerMinusOffset::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "operator- needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    void* offset = emitter.CreateNeg(right);
    emitter.Stack().Push(emitter.ComputeAddress(pointerType->RemovePointer(sourcePos, moduleId)->IrType(emitter), left, offset));
}

class PointerMinusOffsetOperation : public Operation
{
public:
    PointerMinusOffsetOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMinusOffsetOperation::PointerMinusOffsetOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator-", 2, boundCompileUnit_)
{
}

void PointerMinusOffsetOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(sourcePos, moduleId)->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, sourcePos, moduleId, argumentMatch))
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
        function = new PointerMinusOffset(leftType, GetSymbolTable()->GetTypeByName(U"long"), sourcePos, moduleId);
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
    PointerMinusPointer(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* longType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerMinusPointer"; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

PointerMinusPointer::PointerMinusPointer(cmajor::symbols::TypeSymbol* pointerType_, 
    cmajor::symbols::TypeSymbol* longType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : 
    cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator-"), pointerType(pointerType_)
{
    SetGroupName(U"operator-");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam);
    SetReturnType(longType_);
    ComputeName();
}

void PointerMinusPointer::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "operator- needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreatePtrDiff(pointerType->RemovePointer(sourcePos, moduleId)->IrType(emitter), left, right));
}

class PointerMinusPointerOperation : public Operation
{
public:
    PointerMinusPointerOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerMinusPointerOperation::PointerMinusPointerOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator-", 2, boundCompileUnit_)
{
}

void PointerMinusPointerOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(sourcePos, moduleId);
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerMinusPointer(leftType, GetSymbolTable()->GetTypeByName(U"long"), sourcePos, moduleId);
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
    PointerEqual(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerEqual"; }
};

PointerEqual::PointerEqual(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator==")
{
    SetGroupName(U"operator==");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam);
    SetReturnType(boolType_);
    ComputeName();
}

void PointerEqual::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerEqualOperation::PointerEqualOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator==", 2, boundCompileUnit_)
{
}

void PointerEqualOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    rightType = rightType->PlainType(sourcePos, moduleId);
    if (!rightType->IsPointerType()) return;
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerEqual(leftType, GetSymbolTable()->GetTypeByName(U"bool"), sourcePos, moduleId);
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
    PointerLess(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerLess"; }
};

PointerLess::PointerLess(cmajor::symbols::TypeSymbol* pointerType_, cmajor::symbols::TypeSymbol* boolType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator<")
{
    SetGroupName(U"operator<");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* leftParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"left");
    leftParam->SetType(pointerType_);
    AddMember(leftParam);
    cmajor::symbols::ParameterSymbol* rightParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"right");
    rightParam->SetType(pointerType_);
    AddMember(rightParam);
    SetReturnType(boolType_);
    ComputeName();
}

void PointerLess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerLessOperation::PointerLessOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator<", 2, boundCompileUnit_)
{
}

void PointerLessOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->IsPointerType()) return;
    leftType = leftType->PlainType(sourcePos, moduleId);
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->IsPointerType()) return;
    rightType = rightType->PlainType(sourcePos, moduleId);
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[leftType];
    if (!function)
    {
        function = new PointerLess(leftType, GetSymbolTable()->GetTypeByName(U"bool"), sourcePos, moduleId);
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
    PointerArrow(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "PointerArrow"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

PointerArrow::PointerArrow(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator->"), type(type_)
{
    SetGroupName(U"operator->");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* operandParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"operand");
    operandParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(operandParam);
    SetReturnType(type);
    ComputeName();
}

void PointerArrow::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class PointerArrowOperation : public Operation
{
public:
    PointerArrowOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

PointerArrowOperation::PointerArrowOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator->", 1, boundCompileUnit_)
{
}

void PointerArrowOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() <= 1) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* pointerType = type->RemovePointer(sourcePos, moduleId);
    cmajor::symbols::FunctionSymbol* function = functionMap[pointerType];
    if (!function)
    {
        function = new PointerArrow(pointerType, sourcePos, moduleId);
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
    LvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueRefefenceCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueRefefenceCopyCtor::LvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    ComputeName();
}

void LvalueRefefenceCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceCopyConstructorOperation::LvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void LvalueReferenceCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(sourcePos, moduleId);
    cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
    if (!function)
    {
        function = new LvalueRefefenceCopyCtor(lvalueRefType, sourcePos, moduleId);
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
    LvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsLvalueReferenceCopyAssignment() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceCopyAssignment::LvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) :
    cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

void LvalueReferenceCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceCopyAssignmentOperation::LvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void LvalueReferenceCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(sourcePos, moduleId);
    if (lvalueRefType->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType()->RemoveConst(sourcePos, moduleId), lvalueRefType->PlainType(sourcePos, moduleId)->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
        if (!function)
        {
            function = new LvalueReferenceCopyAssignment(lvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), sourcePos, moduleId);
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
    LvalueReferenceMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceMoveAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceMoveAssignment::LvalueReferenceMoveAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) :
    cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type->RemoveReference(sourcePos, moduleId)->AddRvalueReference(sourcePos, moduleId));
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

void LvalueReferenceMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->PlainType(sourcePos, moduleId)->IrType(emitter), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceMoveAssignmentOperation : public Operation
{
public:
    LvalueReferenceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceMoveAssignmentOperation::LvalueReferenceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void LvalueReferenceMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsLvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* lvalueRefType = type->RemovePointer(sourcePos, moduleId);
    if (lvalueRefType->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    if (lvalueRefType->PlainType(sourcePos, moduleId)->IsArrayType()) return;
    if (TypesEqual(arguments[1]->GetType()->RemoveConst(sourcePos, moduleId), lvalueRefType->PlainType(sourcePos, moduleId)->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[lvalueRefType];
        if (!function)
        {
            function = new LvalueReferenceMoveAssignment(lvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), sourcePos, moduleId);
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
    LvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "LvalueReferenceReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

LvalueReferenceReturn::LvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"value");
    valueParam->SetType(type);
    AddMember(valueParam);
    SetReturnType(type);
    ComputeName();
}

void LvalueReferenceReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class LvalueReferenceReturnOperation : public Operation
{
public:
    LvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

LvalueReferenceReturnOperation::LvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void LvalueReferenceReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
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
        function = new LvalueReferenceReturn(type, sourcePos, moduleId);
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
    RvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueRefefenceCopyCtor"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueRefefenceCopyCtor::RvalueRefefenceCopyCtor(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) : cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    ComputeName();
}

void RvalueRefefenceCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceCopyConstructorOperation::RvalueReferenceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void RvalueReferenceCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsRvalueReferenceType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::TypeSymbol* rvalueRefType = type->RemovePointer(sourcePos, moduleId);
    if (rvalueRefType->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::FunctionSymbol* function = functionMap[rvalueRefType];
    if (!function)
    {
        function = new RvalueRefefenceCopyCtor(rvalueRefType, sourcePos, moduleId);
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
    RvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueReferenceCopyAssignment"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueReferenceCopyAssignment::RvalueReferenceCopyAssignment(cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* voidType_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) :
    cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"this");
    thisParam->SetType(type->AddPointer(sourcePos, moduleId));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

void RvalueReferenceCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->PlainType(sourcePos, moduleId)->IrType(emitter), rvalueRefValue));  // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

class RvalueReferenceCopyAssignmentOperation : public Operation
{
public:
    RvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceCopyAssignmentOperation::RvalueReferenceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void RvalueReferenceCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() < 1 || !type->IsRvalueReferenceType()) return;
    cmajor::symbols::TypeSymbol* rvalueRefType = type->RemovePointer(sourcePos, moduleId);
    if (rvalueRefType->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    if (rvalueRefType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(rvalueRefType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[rvalueRefType];
    if (!function)
    {
        function = new RvalueReferenceCopyAssignment(rvalueRefType, GetSymbolTable()->GetTypeByName(U"void"), sourcePos, moduleId);
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
    RvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "RvalueReferenceReturn"; }
private:
    cmajor::symbols::TypeSymbol* type;
};

RvalueReferenceReturn::RvalueReferenceReturn(cmajor::symbols::TypeSymbol* type_, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) :
    cmajor::symbols::FunctionSymbol(sourcePos, moduleId, U"@return"), type(type_)
{
    SetGroupName(U"@return");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    cmajor::symbols::ParameterSymbol* valueParam = new cmajor::symbols::ParameterSymbol(sourcePos, moduleId, U"value");
    valueParam->SetType(type);
    AddMember(valueParam);
    SetReturnType(type);
    ComputeName();
}

void RvalueReferenceReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

class RvalueReferenceReturnOperation : public Operation
{
public:
    RvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<cmajor::symbols::TypeSymbol*, cmajor::symbols::FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

RvalueReferenceReturnOperation::RvalueReferenceReturnOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@return", 1, boundCompileUnit_)
{
}

void RvalueReferenceReturnOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
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
        function = new RvalueReferenceReturn(type, sourcePos, moduleId);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayDefaultConstructorOperation::ArrayDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void ArrayDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
    if (!function)
    {
        std::vector<FunctionScopeLookup> elementLookups;
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
        elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> elementArguments;
        elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddPointer(sourcePos, moduleId))));
        std::unique_ptr<BoundFunctionCall> elementDefaultConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), currentFunction,
            sourcePos, moduleId);
        cmajor::symbols::FunctionSymbol* elementTypeDefaultConstructor = elementDefaultConstructor->GetFunctionSymbol();
        cmajor::symbols::ArrayTypeDefaultConstructor* arrayTypeDefaultConstructor = new cmajor::symbols::ArrayTypeDefaultConstructor(arrayType, elementTypeDefaultConstructor);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayCopyConstructorOperation::ArrayCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ArrayCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        TypesEqual(arguments[1]->GetType()->PlainType(sourcePos, moduleId), arrayType))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddPointer(sourcePos, moduleId))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId))));
            std::unique_ptr<BoundFunctionCall> elementCopyConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), currentFunction,
                sourcePos, moduleId);
            cmajor::symbols::FunctionSymbol* elementTypeCopyConstructor = elementCopyConstructor->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeCopyConstructor* arrayTypeCopyConstructor = new cmajor::symbols::ArrayTypeCopyConstructor(arrayType, elementTypeCopyConstructor);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayMoveConstructorOperation::ArrayMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ArrayMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddPointer(sourcePos, moduleId))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddRvalueReference(sourcePos, moduleId))));
            elementArguments.back()->SetFlag(BoundExpressionFlags::bindToRvalueReference);
            std::unique_ptr<BoundFunctionCall> elementMoveConstructor = ResolveOverload(U"@constructor", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), currentFunction,
                sourcePos, moduleId);
            cmajor::symbols::FunctionSymbol* elementTypeMoveConstructor = elementMoveConstructor->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeMoveConstructor* arrayTypeMoveConstructor = new cmajor::symbols::ArrayTypeMoveConstructor(arrayType, elementTypeMoveConstructor);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayCopyAssignmentOperation::ArrayCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ArrayCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none && TypesEqual(arguments[1]->GetType()->PlainType(sourcePos, moduleId), arrayType) ||
        !TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        TypesEqual(arguments[1]->GetType()->PlainType(sourcePos, moduleId), arrayType))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddPointer(sourcePos, moduleId))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId))));
            std::unique_ptr<BoundFunctionCall> elementCopyAssignment = ResolveOverload(U"operator=", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), currentFunction,
                sourcePos, moduleId);
            cmajor::symbols::FunctionSymbol* elementTypeCopyAssignment = elementCopyAssignment->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeCopyAssignment* arrayTypeCopyAssignment = new cmajor::symbols::ArrayTypeCopyAssignment(arrayType, elementTypeCopyAssignment);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayMoveAssignmentOperation::ArrayMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ArrayMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->IsArrayType()) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (TypesEqual(arguments[1]->GetType(), arrayType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
        if (!function)
        {
            std::vector<FunctionScopeLookup> elementLookups;
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, arrayType->ElementType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            elementLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> elementArguments;
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddPointer(sourcePos, moduleId))));
            elementArguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, arrayType->ElementType()->AddRvalueReference(sourcePos, moduleId))));
            elementArguments.back()->SetFlag(BoundExpressionFlags::bindToRvalueReference);
            std::unique_ptr<BoundFunctionCall> elementMoveAssignment = ResolveOverload(U"operator=", containerScope, elementLookups, elementArguments, GetBoundCompileUnit(), currentFunction,
                sourcePos, moduleId);
            cmajor::symbols::FunctionSymbol* elementTypeMoveAssignment = elementMoveAssignment->GetFunctionSymbol();
            cmajor::symbols::ArrayTypeMoveAssignment* arrayTypeMoveAssignment = new cmajor::symbols::ArrayTypeMoveAssignment(arrayType, elementTypeMoveAssignment);
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
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ArrayElementAccessOperation::ArrayElementAccessOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator[]", 2, boundCompileUnit_)
{
}

void ArrayElementAccessOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* leftType = arguments[0]->GetType();
    if (!leftType->PlainType(sourcePos, moduleId)->IsArrayType()) return;
    if (leftType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(leftType->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(leftType->PlainType(sourcePos, moduleId));
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType();
    if (!rightType->PlainType(sourcePos, moduleId)->IsIntegralType())
    {
        ArgumentMatch argumentMatch;
        if (!GetBoundCompileUnit().GetConversion(rightType, GetSymbolTable()->GetTypeByName(U"long"), containerScope, currentFunction, sourcePos, moduleId, argumentMatch))
        {
            return;
        }
    }
    cmajor::symbols::FunctionSymbol* function = functionMap[arrayType->TypeId()];
    if (!function)
    {
        function = new cmajor::symbols::ArrayTypeElementAccess(arrayType);
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[arrayType->TypeId()] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class InterfaceDefaultConstructorOperation : public Operation
{
public:
    InterfaceDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

InterfaceDefaultConstructorOperation::InterfaceDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void InterfaceDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || type->RemovePointer(sourcePos, moduleId)->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    cmajor::symbols::FunctionSymbol* function = functionMap[interfaceType->TypeId()];
    if (!function)
    {
        function = new cmajor::symbols::InterfaceTypeDefaultConstructor(interfaceType);
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(&GetSymbolTable()->GlobalNs());
        functionMap[interfaceType->TypeId()] = function;
        functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
    }
    viableFunctions.Insert(function);
}

class InterfaceCopyConstructorOperation : public Operation
{
public:
    InterfaceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

InterfaceCopyConstructorOperation::InterfaceCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void InterfaceCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || type->RemovePointer(sourcePos, moduleId)->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));

    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), interfaceType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        (TypesEqual(arguments[1]->GetType()->PlainType(sourcePos, moduleId), interfaceType) || arguments[1]->GetType()->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()))
    {
        if (GetBoundCompileUnit().HasCopyConstructorFor(interfaceType->TypeId()))
        {
            viableFunctions.Insert(GetBoundCompileUnit().GetCopyConstructorFor(interfaceType->TypeId()));
            return;
        }
        cmajor::symbols::FunctionSymbol* function = functionMap[interfaceType->TypeId()];
        if (!function)
        {
            function = new cmajor::symbols::InterfaceTypeCopyConstructor(interfaceType);
            GetBoundCompileUnit().AddCopyConstructorToMap(interfaceType->TypeId(), function);
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[interfaceType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class InterfaceMoveConstructorOperation : public Operation
{
public:
    InterfaceMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

InterfaceMoveConstructorOperation::InterfaceMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void InterfaceMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || type->RemovePointer(sourcePos, moduleId)->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (TypesEqual(arguments[1]->GetType(), interfaceType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[interfaceType->TypeId()];
        if (!function)
        {
            function = new cmajor::symbols::InterfaceTypeMoveConstructor(interfaceType);
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[interfaceType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class InterfaceCopyAssignmentOperation : public Operation
{
public:
    InterfaceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

InterfaceCopyAssignmentOperation::InterfaceCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void InterfaceCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || type->RemovePointer(sourcePos, moduleId)->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (((flags & CollectFlags::noRvalueRef) != CollectFlags::none ||
        !TypesEqual(arguments[1]->GetType(), interfaceType->AddRvalueReference(sourcePos, moduleId)) && !arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference)) &&
        TypesEqual(arguments[1]->GetType()->PlainType(sourcePos, moduleId), interfaceType))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[interfaceType->TypeId()];
        if (!function)
        {
            function = new cmajor::symbols::InterfaceTypeCopyAssignment(interfaceType);
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[interfaceType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class InterfaceMoveAssignmentOperation : public Operation
{
public:
    InterfaceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

InterfaceMoveAssignmentOperation::InterfaceMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void InterfaceMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || type->RemovePointer(sourcePos, moduleId)->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol) return;
    if (type->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type->BaseType());
        GetBoundCompileUnit().FinalizeBinding(specialization);
    }
    cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->RemovePointer(sourcePos, moduleId));
    if (TypesEqual(arguments[1]->GetType(), interfaceType->AddRvalueReference(sourcePos, moduleId)) || arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference))
    {
        cmajor::symbols::FunctionSymbol* function = functionMap[interfaceType->TypeId()];
        if (!function)
        {
            function = new cmajor::symbols::InterfaceTypeMoveAssignment(interfaceType);
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(&GetSymbolTable()->GlobalNs());
            functionMap[interfaceType->TypeId()] = function;
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(function));
        }
        viableFunctions.Insert(function);
    }
}

class ClassDefaultConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassDefaultConstructor(cmajor::symbols::ClassTypeSymbol* classType_);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassDefaultConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
};

ClassDefaultConstructor::ClassDefaultConstructor(cmajor::symbols::ClassTypeSymbol* classType_) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"@constructor"), classType(classType_)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"this");
    thisParam->SetType(classType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ComputeName();
}

class ClassDefaultConstructorOperation : public Operation
{
public:
    ClassDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
    bool GenerateImplementation(ClassDefaultConstructor* defaultConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception,
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassDefaultConstructorOperation::ClassDefaultConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 1, boundCompileUnit_)
{
}

void ClassDefaultConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot create an instance of a static class", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId()));
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
        std::unique_ptr<ClassDefaultConstructor> defaultConstructor(new ClassDefaultConstructor(classType));
        function = defaultConstructor.get();
        function->SetModule(&GetBoundCompileUnit().GetModule());
        function->SetParent(classType);
        function->SetLinkOnceOdrLinkage();
        functionMap[classType->TypeId()] = function;
        defaultConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
        defaultConstructor->SetModule(&GetBoundCompileUnit().GetModule());
        if (GenerateImplementation(defaultConstructor.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
        {
            functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(defaultConstructor.release()));
        }
        else
        {
            functionMap[classType->TypeId()] = nullptr;
            return;
        }
    }
    viableFunctions.Insert(function);
}

bool ClassDefaultConstructorOperation::GenerateImplementation(ClassDefaultConstructor* defaultConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = defaultConstructor->ClassType();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), defaultConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                new BoundFunctionCall(sourcePos, moduleId, classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, GetBoundCompileUnit(),
                boundFunction.get(), sourcePos, moduleId);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = defaultConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int n = classType->MemberVariables().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, defaultConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, memberConstructorCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), sourcePos, moduleId);
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
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create default constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references));
        return false;
    }
    return  true;
}

class ClassCopyConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassCopyConstructor(cmajor::symbols::ClassTypeSymbol* classType_);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassCopyConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
};

ClassCopyConstructor::ClassCopyConstructor(cmajor::symbols::ClassTypeSymbol* classType_) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"@constructor"), classType(classType_)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"this");
    thisParam->SetType(classType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"that");
    thatParam->SetType(classType->AddConst(soul::ast::SourcePos(), util::nil_uuid())->AddLvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    ComputeName();
}

class ClassCopyConstructorOperation : public Operation
{
public:
    ClassCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
    bool GenerateImplementation(ClassCopyConstructor* copyConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception,
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    void AddFunction(std::unique_ptr<cmajor::symbols::FunctionSymbol>&& function) { functions.push_back(std::move(function)); }
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassCopyConstructorOperation::ClassCopyConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ClassCopyConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot copy an instance of a static class", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(sourcePos, moduleId);
    bool typesEqual = TypesEqual(rightType, classType);
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool conversionFunctionExists = false;
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
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
            !TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(sourcePos, moduleId)) && !bindToRvalueRef) && (typesEqual || conversionFunctionExists)))
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
            std::unique_ptr<ClassCopyConstructor> copyConstructor(new ClassCopyConstructor(classType));
            function = copyConstructor.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            functionMap[classType->TypeId()] = function;
            copyConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            copyConstructor->SetModule(&GetBoundCompileUnit().GetModule());
            if (GenerateImplementation(copyConstructor.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
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
        viableFunctions.Insert(function);
    }
}

bool ClassCopyConstructorOperation::GenerateImplementation(ClassCopyConstructor* copyConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = copyConstructor->ClassType();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), copyConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(new BoundFunctionCall(sourcePos, moduleId,
                classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = copyConstructor->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), classType->BaseClass()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId), containerScope,
                currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* thatArgumentConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion);
            baseConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, GetBoundCompileUnit(),
                boundFunction.get(), sourcePos, moduleId);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = copyConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int n = classType->MemberVariables().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, copyConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
            cmajor::symbols::ParameterSymbol* thatParam = copyConstructor->Parameters()[1];
            BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId))));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, memberConstructorCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), sourcePos, moduleId);
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
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create copy constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references));
        return false;
    }
    return true;
}

class ClassMoveConstructor : public cmajor::symbols::ConstructorSymbol
{
public:
    ClassMoveConstructor(cmajor::symbols::ClassTypeSymbol* classType_);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassMoveConstructor"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
};

ClassMoveConstructor::ClassMoveConstructor(cmajor::symbols::ClassTypeSymbol* classType_) :
    cmajor::symbols::ConstructorSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"@constructor"), classType(classType_)
{
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"this");
    thisParam->SetType(classType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"that");
    thatParam->SetType(classType->AddRvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    ComputeName();
}

class ClassMoveConstructorOperation : public Operation
{
public:
    ClassMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
    bool GenerateImplementation(ClassMoveConstructor* moveConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos,
        const util::uuid& moduleId);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassMoveConstructorOperation::ClassMoveConstructorOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"@constructor", 2, boundCompileUnit_)
{
}

void ClassMoveConstructorOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot move an instance of a static class", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(sourcePos, moduleId);
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool typesEqual = TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(sourcePos, moduleId));
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
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
            std::unique_ptr<ClassMoveConstructor> moveConstructor(new ClassMoveConstructor(classType));
            function = moveConstructor.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            functionMap[classType->TypeId()] = function;
            moveConstructor->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            moveConstructor->SetModule(&GetBoundCompileUnit().GetModule());
            if (GenerateImplementation(moveConstructor.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveConstructor.release()));
            }
            else
            {
                functionMap[classType->TypeId()] = nullptr;
                return;
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassMoveConstructorOperation::GenerateImplementation(ClassMoveConstructor* moveConstructor, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = moveConstructor->ClassType();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), moveConstructor));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->StaticConstructor())
        {
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(new BoundFunctionCall(sourcePos, moduleId,
                classType->StaticConstructor())))));
            if (!classType->StaticConstructor()->DontThrow()) nothrow = false;
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            std::unique_ptr<BoundExpression> baseClassPointerConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion));
            baseConstructorCallArguments.push_back(std::move(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = moveConstructor->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), classType->BaseClass()->AddRvalueReference(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            std::unique_ptr<BoundExpression> thatArgumentConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion));
            baseConstructorCallArguments.push_back(std::move(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseConstructorCall = ResolveOverload(U"@constructor", containerScope, baseConstructorCallLookups, baseConstructorCallArguments, GetBoundCompileUnit(),
                boundFunction.get(), sourcePos, moduleId);
            if (!baseConstructorCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseConstructorCall))));
        }
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = moveConstructor->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
            }
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundSetVmtPtrStatement(std::unique_ptr<BoundExpression>(classPtr), classType)));
        }
        int n = classType->MemberVariables().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberConstructorCallLookups;
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            memberConstructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberConstructorCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, moveConstructor->GetThisParam())));
            memberConstructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
            cmajor::symbols::ParameterSymbol* thatParam = moveConstructor->Parameters()[1];
            std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol));
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId))));
            std::vector<FunctionScopeLookup> rvalueLookups;
            rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            std::vector<std::unique_ptr<BoundExpression>> rvalueArguments;
            rvalueArguments.push_back(std::move(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> rvalueMemberCall = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, GetBoundCompileUnit(), boundFunction.get(), sourcePos,
                moduleId);
            memberConstructorCallArguments.push_back(std::move(rvalueMemberCall));
            std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, memberConstructorCallLookups, memberConstructorCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), sourcePos, moduleId);
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
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create move constructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references));
        return false;
    }
    return true;
}

class ClassCopyAssignment : public cmajor::symbols::MemberFunctionSymbol
{
public:
    ClassCopyAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassCopyAssignment"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
};

ClassCopyAssignment::ClassCopyAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_) :
    cmajor::symbols::MemberFunctionSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"operator="), classType(classType_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"this");
    thisParam->SetType(classType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"that");
    thatParam->SetType(classType->AddConst(soul::ast::SourcePos(), util::nil_uuid())->AddLvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

class ClassCopyAssignmentOperation : public Operation
{
public:
    ClassCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
    bool GenerateImplementation(ClassCopyAssignment* copyAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos,
        const util::uuid& moduleId);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassCopyAssignmentOperation::ClassCopyAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ClassCopyAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot assign an instance of a static class", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(sourcePos, moduleId);
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool conversionFunctionExists = false;
    bool typesEqual = TypesEqual(rightType, classType);
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
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
        !TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(sourcePos, moduleId)) && !bindToRvalueRef) && (TypesEqual(rightType, classType) || conversionFunctionExists))
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
            std::unique_ptr<ClassCopyAssignment> copyAssignment(new ClassCopyAssignment(classType, GetBoundCompileUnit().GetSymbolTable().GetTypeByName(U"void")));
            function = copyAssignment.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            functionMap[classType->TypeId()] = function;
            copyAssignment->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            copyAssignment->SetModule(&GetBoundCompileUnit().GetModule());
            if (GenerateImplementation(copyAssignment.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copyAssignment.release())); // todo
            }
            else
            {
                functionMap[classType->TypeId()] = nullptr;
                return;
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassCopyAssignmentOperation::GenerateImplementation(ClassCopyAssignment* copyAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = copyAssignment->ClassType();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), copyAssignment));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseAssignmentCallLookups;
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseAssignmentCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = copyAssignment->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            baseAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = copyAssignment->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), classType->BaseClass()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId), containerScope,
                currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* thatArgumentConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion);
            baseAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseAssignmentCall = ResolveOverload(U"operator=", containerScope, baseAssignmentCallLookups, baseAssignmentCallArguments, GetBoundCompileUnit(),
                boundFunction.get(), sourcePos, moduleId);
            if (!baseAssignmentCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseAssignmentCall))));
        }
        int n = classType->MemberVariables().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::vector<FunctionScopeLookup> memberAssignmentCallLookups;
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            memberAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> memberAssignmentCallArguments;
            BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, copyAssignment->GetThisParam())));
            memberAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(
                new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
            cmajor::symbols::ParameterSymbol* thatParam = copyAssignment->Parameters()[1];
            BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId))));
            memberAssignmentCallArguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> memberAssignmentCall = ResolveOverload(U"operator=", containerScope, memberAssignmentCallLookups, memberAssignmentCallArguments,
                GetBoundCompileUnit(), boundFunction.get(), sourcePos, moduleId);
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
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create copy assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references));
        return false;
    }
    return true;
}

class ClassMoveAssignment : public cmajor::symbols::MemberFunctionSymbol
{
public:
    ClassMoveAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_);
    cmajor::symbols::SymbolAccess DeclaredAccess() const override { return cmajor::symbols::SymbolAccess::public_; }
    bool IsGeneratedFunction() const override { return true; }
    cmajor::symbols::ClassTypeSymbol* ClassType() { return classType; }
    const char* ClassName() const override { return "ClassMoveAssignment"; }
private:
    cmajor::symbols::ClassTypeSymbol* classType;
};

ClassMoveAssignment::ClassMoveAssignment(cmajor::symbols::ClassTypeSymbol* classType_, cmajor::symbols::TypeSymbol* voidType_) :
    cmajor::symbols::MemberFunctionSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"operator="), classType(classType_)
{
    SetGroupName(U"operator=");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetParent(classType);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"this");
    thisParam->SetType(classType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    cmajor::symbols::ParameterSymbol* thatParam = new cmajor::symbols::ParameterSymbol(classType_->GetSourcePos(), classType_->SourceModuleId(), U"that");
    thatParam->SetType(classType->AddRvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
}

class ClassMoveAssignmentOperation : public Operation
{
public:
    ClassMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags) override;
    bool GenerateImplementation(ClassMoveAssignment* moveAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos,
        const util::uuid& moduleId);
private:
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> functions;
};

ClassMoveAssignmentOperation::ClassMoveAssignmentOperation(BoundCompileUnit& boundCompileUnit_) : Operation(U"operator=", 2, boundCompileUnit_)
{
}

void ClassMoveAssignmentOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    if ((flags & CollectFlags::noRvalueRef) != CollectFlags::none) return;
    cmajor::symbols::TypeSymbol* type = arguments[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(sourcePos, moduleId)->PlainType(sourcePos, moduleId)->IsClassTypeSymbol()) return;
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
    if (classType->IsStatic())
    {
        exception.reset(new cmajor::symbols::Exception("cannot assign an instance of a static class", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId()));
        return;
    }
    cmajor::symbols::TypeSymbol* rightType = arguments[1]->GetType()->PlainType(sourcePos, moduleId);
    bool bindToRvalueRef = arguments[1]->GetFlag(BoundExpressionFlags::bindToRvalueReference);
    bool typesEqual = TypesEqual(arguments[1]->GetType(), classType->AddRvalueReference(sourcePos, moduleId));
    if (!typesEqual)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = GetBoundCompileUnit().GetConversion(rightType, classType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
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
            std::unique_ptr<ClassMoveAssignment> moveAssignment(new ClassMoveAssignment(classType, GetBoundCompileUnit().GetSymbolTable().GetTypeByName(U"void")));
            function = moveAssignment.get();
            function->SetModule(&GetBoundCompileUnit().GetModule());
            function->SetParent(classType);
            function->SetLinkOnceOdrLinkage();
            functionMap[classType->TypeId()] = function;
            moveAssignment->SetCompileUnit(GetBoundCompileUnit().GetCompileUnitNode());
            moveAssignment->SetModule(&GetBoundCompileUnit().GetModule());
            if (GenerateImplementation(moveAssignment.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
            {
                functions.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(moveAssignment.release()));
            }
            else
            {
                functionMap[classType->TypeId()] = nullptr;
                return;
            }
        }
        viableFunctions.Insert(function);
    }
}

bool ClassMoveAssignmentOperation::GenerateImplementation(ClassMoveAssignment* moveAssignment, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = moveAssignment->ClassType();
    try
    {
        bool nothrow = true;
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&GetBoundCompileUnit(), moveAssignment));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->BaseClass())
        {
            std::vector<FunctionScopeLookup> baseAssignmentCallLookups;
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            baseAssignmentCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> baseAssignmentCallArguments;
            cmajor::symbols::ParameterSymbol* thisParam = moveAssignment->Parameters()[0];
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = GetBoundCompileUnit().GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            std::unique_ptr<BoundExpression> baseClassPointerConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion));
            baseAssignmentCallArguments.push_back(std::move(baseClassPointerConversion));
            cmajor::symbols::ParameterSymbol* thatParam = moveAssignment->Parameters()[1];
            cmajor::symbols::FunctionSymbol* thatToBaseConversion = GetBoundCompileUnit().GetConversion(thatParam->GetType(), classType->BaseClass()->AddRvalueReference(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thatToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            std::unique_ptr<BoundExpression> thatArgumentConversion(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion));
            baseAssignmentCallArguments.push_back(std::move(thatArgumentConversion));
            std::unique_ptr<BoundFunctionCall> baseAssignmentCall = ResolveOverload(U"operator=", containerScope, baseAssignmentCallLookups, baseAssignmentCallArguments, GetBoundCompileUnit(),
                boundFunction.get(), sourcePos, moduleId);
            if (!baseAssignmentCall->GetFunctionSymbol()->DontThrow()) nothrow = false;
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(baseAssignmentCall))));
        }
        int n = classType->MemberVariables().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = classType->MemberVariables()[i];
            std::unique_ptr<BoundMemberVariable> boundMemberVariable(new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol));
            boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, moveAssignment->GetThisParam())));
            cmajor::symbols::ParameterSymbol* thatParam = moveAssignment->Parameters()[1];
            std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol));
            thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId))));
            std::vector<FunctionScopeLookup> swapLookups;
            swapLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            swapLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            std::vector<std::unique_ptr<BoundExpression>> swapArguments;
            swapArguments.push_back(std::move(boundMemberVariable));
            swapArguments.push_back(std::move(thatBoundMemberVariable));
            std::unique_ptr<BoundFunctionCall> swapMemberCall = ResolveOverload(U"System.Swap", containerScope, swapLookups, swapArguments, GetBoundCompileUnit(), boundFunction.get(), sourcePos, moduleId);
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
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        exception.reset(new cmajor::symbols::Exception("cannot create move assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references));
        return false;
    }
    return true;
}

void GenerateDestructorImplementation(BoundClass* boundClass, cmajor::symbols::DestructorSymbol* destructorSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::ClassTypeSymbol* classType = boundClass->GetClassTypeSymbol();
    try
    {
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, destructorSymbol));
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(new BoundCompoundStatement(sourcePos, moduleId)));
        if (classType->IsPolymorphic())
        {
            cmajor::symbols::ParameterSymbol* thisParam = destructorSymbol->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
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
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceOrNsScope()));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> memberDestructorCallArguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, destructorSymbol->GetThisParam())));
                memberDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                std::unique_ptr<BoundFunctionCall> memberDestructorCall = ResolveOverload(U"@destructor", containerScope, memberDestructorCallLookups, memberDestructorCallArguments,
                    boundCompileUnit, boundFunction.get(), sourcePos, moduleId);
                boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(memberDestructorCall), sourcePos, moduleId)));
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", sourcePos, moduleId, classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            baseDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseDestructorCall = ResolveOverload(U"@destructor", containerScope, baseDestructorCallLookups, baseDestructorCallArguments, boundCompileUnit,
                boundFunction.get(), sourcePos, moduleId);
            boundFunction->Body()->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(baseDestructorCall), sourcePos, moduleId)));
        }
        boundClass->AddMember(std::move(boundFunction));
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("cannot create destructor for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), sourcePos, moduleId, references);
    }
}

BoundExpression* MakeExitEntryPtr(BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(U"System.ExitEntry", cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
            {
                cmajor::symbols::ClassGroupTypeSymbol* classGroupSymbol = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(symbol);
                symbol = classGroupSymbol->GetClass(0);
            }
            cmajor::symbols::TypeSymbol* exitEntryType = static_cast<cmajor::symbols::TypeSymbol*>(symbol);
            cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(boundCompileUnit.GetSymbolTable());
            cmajor::ast::GlobalVariableNode globalVariableNode(sourcePos, moduleId, cmajor::ast::Specifiers::private_, new cmajor::ast::DotNode(sourcePos, moduleId, 
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System"),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ExitEntry")),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"exit@entry@" + 
                    util::ToUtf32(std::to_string(boundCompileUnit.GetNextExitEntryIndex()))), nullptr, boundCompileUnit.GetCompileUnitNode());
            globalVariableNode.Accept(symbolCreatorVisitor);
            TypeBinder typeBinder(boundCompileUnit);
            typeBinder.SetContainerScope(containerScope);
            globalVariableNode.Accept(typeBinder);
            BoundGlobalVariable* exitEntryGlobalVariable = static_cast<BoundGlobalVariable*>(typeBinder.GetBoundGlobalVariable()->Clone());
            return new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(exitEntryGlobalVariable), exitEntryType->AddPointer(sourcePos, moduleId));
        }
        else
        {
            throw cmajor::symbols::Exception("System.ExitEntry expected to denote a type", sourcePos, moduleId);
        }
    }
    else
    {
        throw cmajor::symbols::Exception("System.ExitEntry symbol not found", sourcePos, moduleId);
    }
}

void GenerateStaticClassInitialization(cmajor::symbols::StaticConstructorSymbol* staticConstructorSymbol, cmajor::ast::StaticConstructorNode* staticConstructorNode, BoundCompileUnit& boundCompileUnit,
    BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::Symbol* parent = staticConstructorSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    try
    {
        Assert(classType->InitializedVar(), "initialized variable expected");
        std::unique_ptr<BoundIfStatement> ifStatement(new BoundIfStatement(sourcePos, moduleId, std::unique_ptr<BoundExpression>(new BoundMemberVariable(sourcePos, moduleId, classType->InitializedVar())),
            std::unique_ptr<BoundStatement>(new BoundReturnStatement(std::unique_ptr<BoundFunctionCall>(nullptr), sourcePos, moduleId)), std::unique_ptr<BoundStatement>(nullptr)));
        boundCompoundStatement->AddStatement(std::move(ifStatement));
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
        {
            cmajor::ast::IdentifierNode staticInitCriticalSection(sourcePos, moduleId, U"System.Runtime.StaticInitCriticalSection");
            cmajor::symbols::TypeSymbol* staticInitCriticalSectionClassType = ResolveType(&staticInitCriticalSection, boundCompileUnit, containerScope);
            std::vector<FunctionScopeLookup> constructorLookups;
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, staticInitCriticalSectionClassType->ClassInterfaceOrNsScope()));
            constructorLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> constructorArguments;
            constructorArguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
                new BoundLocalVariable(sourcePos, moduleId, staticConstructorSymbol->CreateTemporary(staticInitCriticalSectionClassType, sourcePos, moduleId))),
                staticInitCriticalSectionClassType->AddPointer(sourcePos, moduleId))));
            constructorArguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UuidValue(sourcePos, moduleId,
                boundCompileUnit.Install(classType->TypeId()))), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
            std::unique_ptr<BoundConstructionStatement> constructionStatement(new BoundConstructionStatement(
                ResolveOverload(U"@constructor", containerScope, constructorLookups, constructorArguments, boundCompileUnit, boundFunction, sourcePos, moduleId), sourcePos, moduleId));
            boundCompoundStatement->AddStatement(std::move(constructionStatement));
            std::unique_ptr<BoundIfStatement> ifStatement2(new BoundIfStatement(sourcePos, moduleId, std::unique_ptr<BoundExpression>(new BoundMemberVariable(sourcePos, moduleId, classType->InitializedVar())),
                std::unique_ptr<BoundStatement>(new BoundReturnStatement(std::unique_ptr<BoundFunctionCall>(nullptr), sourcePos, moduleId)), std::unique_ptr<BoundStatement>(nullptr)));
            boundCompoundStatement->AddStatement(std::move(ifStatement2));
        }
        std::vector<FunctionScopeLookup> assignmentLookups;
        assignmentLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        assignmentLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> assignmentArguments;
        assignmentArguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundMemberVariable(sourcePos, moduleId, classType->InitializedVar())),
            classType->InitializedVar()->GetType()->AddPointer(sourcePos, moduleId))));
        assignmentArguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::BoolValue(sourcePos, moduleId, true)),
            boundCompileUnit.GetSymbolTable().GetTypeByName(U"bool"))));
        std::unique_ptr<BoundAssignmentStatement> assignmentStatement(new BoundAssignmentStatement(
            ResolveOverload(U"operator=", containerScope, assignmentLookups, assignmentArguments, boundCompileUnit, boundFunction, sourcePos, moduleId), sourcePos, moduleId));
        boundCompoundStatement->AddStatement(std::move(assignmentStatement));
        cmajor::symbols::ClassTypeSymbol* baseClass = classType->BaseClass();
        if (baseClass)
        {
            if (baseClass->StaticConstructor())
            {
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                    new BoundFunctionCall(sourcePos, moduleId, baseClass->StaticConstructor())))));
            }
        }
        std::unordered_map<std::u32string, cmajor::ast::MemberInitializerNode*> memberInitializerMap;
        int n = staticConstructorNode->Initializers().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::InitializerNode* initializer = staticConstructorNode->Initializers()[i];
            if (initializer->GetNodeType() == cmajor::ast::NodeType::thisInitializerNode)
            {
                throw cmajor::symbols::Exception("static constructor cannot have 'this' initializers", initializer->GetSourcePos(), initializer->ModuleId());
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::baseInitializerNode)
            {
                throw cmajor::symbols::Exception("static constructor cannot have 'base' initializers", initializer->GetSourcePos(), initializer->ModuleId());
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::memberInitializerNode)
            {
                cmajor::ast::MemberInitializerNode* memberInitializer = static_cast<cmajor::ast::MemberInitializerNode*>(initializer);
                std::u32string memberName = memberInitializer->MemberId()->Str();
                auto it = memberInitializerMap.find(memberName);
                if (it != memberInitializerMap.cend())
                {
                    throw cmajor::symbols::Exception("already has initializer for member variable '" + util::ToUtf8(memberName) + "'", initializer->GetSourcePos(), initializer->ModuleId());
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
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable),
                    boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId)));
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
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                    sourcePos, moduleId);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                if (memberVariableClassTypeWithDestructor)
                {
                    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(sourcePos, moduleId,
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        const char32_t* enqueueDestructorFunction = U"RtEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, enqueueArguments, boundCompileUnit,
                            boundFunction, sourcePos, moduleId);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                    {
                        std::vector<FunctionScopeLookup> atExitLookups;
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> atExitArguments;
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(MakeExitEntryPtr(boundCompileUnit, containerScope, sourcePos, moduleId)));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(sourcePos, moduleId,
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        const char32_t* atExitFunction = U"at_exit";
                        std::unique_ptr<BoundFunctionCall> atExitCall = ResolveOverload(atExitFunction, containerScope, atExitLookups, atExitArguments, boundCompileUnit,
                            boundFunction, sourcePos, moduleId);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(atExitCall))));
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
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId)));
                std::unique_ptr<BoundExpression> addrOfBoundMemberVariable2;
                cmajor::symbols::ClassTypeSymbol* memberVariableClassTypeWithDestructor = nullptr;
                if (memberVariableSymbol->GetType()->HasNontrivialDestructor())
                {
                    addrOfBoundMemberVariable2.reset(addrOfBoundMemberVariable->Clone());
                    memberVariableClassTypeWithDestructor = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableSymbol->GetType()->BaseType());
                }
                arguments.push_back(std::move(addrOfBoundMemberVariable));
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                    sourcePos, moduleId);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                if (memberVariableClassTypeWithDestructor)
                {
                    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                    {
                        std::vector<FunctionScopeLookup> enqueueLookups;
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        enqueueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> enqueueArguments;
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(sourcePos, moduleId,
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        enqueueArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        const char32_t* enqueueDestructorFunction = U"RtEnqueueDestruction";
                        std::unique_ptr<BoundFunctionCall> enqueueDestructorCall = ResolveOverload(enqueueDestructorFunction, containerScope, enqueueLookups, enqueueArguments, boundCompileUnit,
                            boundFunction, sourcePos, moduleId);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(enqueueDestructorCall))));
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                    {
                        std::vector<FunctionScopeLookup> atExitLookups;
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                        atExitLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                        std::vector<std::unique_ptr<BoundExpression>> atExitArguments;
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(MakeExitEntryPtr(boundCompileUnit, containerScope, sourcePos, moduleId)));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::unique_ptr<BoundExpression>(new BoundFunctionPtr(sourcePos, moduleId,
                            memberVariableClassTypeWithDestructor->Destructor(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        atExitArguments.push_back(std::unique_ptr<BoundExpression>(new BoundBitCast(std::move(addrOfBoundMemberVariable2),
                            boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId))));
                        const char32_t* atExitFunction = U"at_exit";
                        std::unique_ptr<BoundFunctionCall> atExitCall = ResolveOverload(atExitFunction, containerScope, atExitLookups, atExitArguments, boundCompileUnit,
                            boundFunction, sourcePos, moduleId);
                        boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(atExitCall))));
                    }
                }
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate static initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(),
            staticConstructorNode->GetSourcePos(), staticConstructorNode->ModuleId(), references);
    }
}

void GenerateClassInitialization(cmajor::symbols::ConstructorSymbol* constructorSymbol, cmajor::ast::ConstructorNode* constructorNode, BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction,
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool generateDefault, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::Symbol* parent = constructorSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected"); 
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    if (generateDefault)
    {
        if (classType->IsStatic())
        {
            throw cmajor::symbols::Exception("cannot create default initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: class is static", sourcePos, moduleId);
        }
    }
    try
    {
        if (classType->StaticConstructor())
        {
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(new BoundFunctionCall(sourcePos, moduleId,
                classType->StaticConstructor())))));
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
                    throw cmajor::symbols::Exception("already has 'this' initializer", initializer->GetSourcePos(), initializer->ModuleId());
                }
                else if (baseInitializer)
                {
                    throw cmajor::symbols::Exception("cannot have both 'this' and 'base' initializer", initializer->GetSourcePos(), initializer->ModuleId());
                }
                thisInitializer = static_cast<cmajor::ast::ThisInitializerNode*>(initializer);
            }
            else if (initializer->GetNodeType() == cmajor::ast::NodeType::baseInitializerNode)
            {
                if (baseInitializer)
                {
                    throw cmajor::symbols::Exception("already has 'base' initializer", initializer->GetSourcePos(), initializer->ModuleId());
                }
                else if (thisInitializer)
                {
                    throw cmajor::symbols::Exception("cannot have both 'this' and 'base' initializer", initializer->GetSourcePos(), initializer->ModuleId());
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
                    throw cmajor::symbols::Exception("already has initializer for member variable '" + util::ToUtf8(memberName) + "'", initializer->GetSourcePos(), initializer->ModuleId());
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
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
            int n = thisInitializer->Arguments().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* argumentNode = thisInitializer->Arguments()[i];
                std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                arguments.push_back(std::move(argument));
            }
            OverloadResolutionFlags flags = OverloadResolutionFlags::none;
            if (!constructorSymbol->IsMoveConstructor())
            {
                flags = flags | OverloadResolutionFlags::noRvalueRef;
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId,
                flags, templateArgumentTypes, exception);
            boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
        }
        else if (baseInitializer)
        {
            if (!classType->BaseClass())
            {
                throw cmajor::symbols::Exception("class '" + util::ToUtf8(classType->FullName()) + "' does not have a base class", constructorNode->GetSourcePos(), constructorNode->ModuleId(),
                    classType->GetSourcePos(), classType->SourceModuleId());
            }
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", constructorNode->GetSourcePos(), constructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            int n = baseInitializer->Arguments().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* argumentNode = baseInitializer->Arguments()[i];
                std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                arguments.push_back(std::move(argument));
            }
            OverloadResolutionFlags flags = OverloadResolutionFlags::none;
            if (!constructorSymbol->IsMoveConstructor())
            {
                flags = flags | OverloadResolutionFlags::noRvalueRef;
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId,
                flags, templateArgumentTypes, exception);
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", constructorNode->GetSourcePos(), constructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            bool copyConstructor = constructorSymbol->IsCopyConstructor();
            if (copyConstructor)
            {
                cmajor::symbols::ParameterSymbol* thatParam = constructorSymbol->Parameters()[1];
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId), containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", constructorNode->GetSourcePos(), constructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
            }
            OverloadResolutionFlags flags = OverloadResolutionFlags::none;
            if (!constructorSymbol->IsMoveConstructor())
            {
                flags = flags | OverloadResolutionFlags::noRvalueRef;
            }
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId,
                flags, templateArgumentTypes, exception);
            boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
        }
        if (classType->IsPolymorphic() && !thisInitializer)
        {
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                    sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", constructorNode->GetSourcePos(), constructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
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
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                arguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
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
                std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId,
                    flags, templateArgumentTypes, exception);
                boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
            }
            else if (!thisInitializer)
            {
                if (constructorSymbol->IsCopyConstructor())
                {
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                    cmajor::ast::CloneContext cloneContext;
                    cmajor::ast::DotNode thatMemberVarNode(sourcePos, moduleId, constructorNode->Parameters()[0]->Clone(cloneContext),
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, memberVariableSymbol->Name()));
                    std::unique_ptr<BoundExpression> thatMemberVarArgument = BindExpression(&thatMemberVarNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(thatMemberVarArgument));
                    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                        sourcePos, moduleId);
                    boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                }
                else if (constructorSymbol->IsMoveConstructor())
                {
                    std::vector<FunctionScopeLookup> lookups;
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->GetContainerScope()));
                    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    std::vector<std::unique_ptr<BoundExpression>> arguments;
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                    cmajor::symbols::ParameterSymbol* thatParam = constructorSymbol->Parameters()[1];
                    std::unique_ptr<BoundMemberVariable> thatBoundMemberVariable(new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol));
                    thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(
                        new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(
                            new BoundParameter(sourcePos, moduleId, thatParam)), thatParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId))));
                    std::vector<FunctionScopeLookup> rvalueLookups;
                    rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                    rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                    std::vector<std::unique_ptr<BoundExpression>> rvalueArguments;
                    rvalueArguments.push_back(std::move(thatBoundMemberVariable));
                    std::unique_ptr<BoundFunctionCall> rvalueMemberCall = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, boundCompileUnit, boundFunction,
                        sourcePos, moduleId);
                    arguments.push_back(std::move(rvalueMemberCall));
                    std::unique_ptr<BoundFunctionCall> memberConstructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                        sourcePos, moduleId);
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
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                    OverloadResolutionFlags flags = OverloadResolutionFlags::none;
                    if (!constructorSymbol->IsMoveConstructor())
                    {
                        flags = flags | OverloadResolutionFlags::noRvalueRef;
                    }
                    std::unique_ptr<cmajor::symbols::Exception> exception;
                    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
                    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId,
                        flags, templateArgumentTypes, exception);
                    boundFunction->MoveTemporaryDestructorCallsTo(*constructorCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(constructorCall))));
                }
            }
        }
        if (!memberInitializerMap.empty())
        {
            cmajor::ast::MemberInitializerNode* initializer = memberInitializerMap.begin()->second;
            throw cmajor::symbols::Exception("no member variable found for initializer named '" + util::ToUtf8(initializer->MemberId()->Str()) + "'", initializer->GetSourcePos(), initializer->ModuleId(),
                classType->GetSourcePos(), classType->SourceModuleId());
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate initialization for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), constructorNode->GetSourcePos(), constructorNode->ModuleId(),
            references);
    }
}

void GenerateClassAssignment(cmajor::symbols::MemberFunctionSymbol* assignmentFunctionSymbol, cmajor::ast::MemberFunctionNode* assignmentNode, BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction,
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool generateDefault, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    cmajor::symbols::Symbol* parent = assignmentFunctionSymbol->Parent();
    Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    if (generateDefault)
    {
        if (classType->IsStatic())
        {
            throw cmajor::symbols::Exception("cannot create default assigment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: class is static", sourcePos, moduleId);
        }
    }
    try
    {
        cmajor::symbols::ParameterSymbol* thisParam = assignmentFunctionSymbol->GetThisParam();
        Assert(thisParam, "this parameter expected");
        if (assignmentFunctionSymbol->IsCopyAssignment())
        {
            if (classType->BaseClass())
            {
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                    sourcePos, moduleId, argumentMatch);
                if (!thisToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", assignmentNode->GetSourcePos(), assignmentNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
                cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId), containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", assignmentNode->GetSourcePos(), assignmentNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
                std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                    sourcePos, moduleId);
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
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(
                        new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                    cmajor::ast::CloneContext cloneContext;
                    cmajor::ast::DotNode thatMemberVarNode(sourcePos, moduleId, assignmentNode->Parameters()[0]->Clone(cloneContext),
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, memberVariableSymbol->Name()));
                    std::unique_ptr<BoundExpression> thatMemberVarArgument = BindExpression(&thatMemberVarNode, boundCompileUnit, boundFunction, containerScope, statementBinder);
                    arguments.push_back(std::move(thatMemberVarArgument));
                    std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                        sourcePos, moduleId);
                    boundFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(assignmentCall))));
                }
            }
        }
        else if (assignmentFunctionSymbol->IsMoveAssignment())
        {
            if (classType->BaseClass())
            {
                std::vector<FunctionScopeLookup> lookups;
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->BaseClass()->GetContainerScope()));
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> arguments;
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope,
                    boundFunction, sourcePos, moduleId, argumentMatch);
                if (!thisToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", assignmentNode->GetSourcePos(), assignmentNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
                cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                cmajor::symbols::FunctionSymbol* thatToBaseConversion = boundCompileUnit.GetConversion(thatParam->GetType(),
                    classType->BaseClass()->AddRvalueReference(sourcePos, moduleId), containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                if (!thatToBaseConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", assignmentNode->GetSourcePos(), assignmentNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                BoundExpression* baseClassReferenceConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thatParam)), thatToBaseConversion);
                arguments.push_back(std::unique_ptr<BoundExpression>(baseClassReferenceConversion));
                std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, lookups, arguments, boundCompileUnit, boundFunction,
                    sourcePos, moduleId);
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
                    BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(boundMemberVariable));
                    BoundMemberVariable* thatBoundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                    cmajor::symbols::ParameterSymbol* thatParam = assignmentFunctionSymbol->Parameters()[1];
                    cmajor::symbols::TypeSymbol* thatPtrType = thatParam->GetType()->RemoveReference(sourcePos, moduleId)->AddPointer(sourcePos, moduleId);
                    thatBoundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(
                        new BoundParameter(sourcePos, moduleId, thatParam)), thatPtrType)));
                    arguments.push_back(std::unique_ptr<BoundExpression>(thatBoundMemberVariable));
                    std::unique_ptr<BoundFunctionCall> swapCall = ResolveOverload(U"System.Swap", containerScope, lookups, arguments, boundCompileUnit, boundFunction, sourcePos, moduleId);
                    boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::move(swapCall))));
                }
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate assignment for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), assignmentNode->GetSourcePos(), assignmentNode->ModuleId(), references);
    }
}

void GenerateClassTermination(cmajor::symbols::DestructorSymbol* destructorSymbol, cmajor::ast::DestructorNode* destructorNode, BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction,
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
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
            cmajor::symbols::ParameterSymbol* thisParam = destructorSymbol->Parameters()[0];
            BoundExpression* classPtr = nullptr;
            cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
            if (vmtPtrHolderClass == classType)
            {
                classPtr = new BoundParameter(sourcePos, moduleId, thisParam);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisToHolderConversion = boundCompileUnit.GetConversion(thisParam->GetType(), vmtPtrHolderClass->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                    sourcePos, moduleId, argumentMatch);
                if (!thisToHolderConversion)
                {
                    throw cmajor::symbols::Exception("base class conversion not found", destructorNode->GetSourcePos(), destructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
                }
                classPtr = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToHolderConversion);
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
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, memberVariableSymbol->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
                memberDestructorCallLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> memberDestructorCallArguments;
                BoundMemberVariable* boundMemberVariable = new BoundMemberVariable(sourcePos, moduleId, memberVariableSymbol);
                boundMemberVariable->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, destructorSymbol->GetThisParam())));
                memberDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(
                    new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(boundMemberVariable), boundMemberVariable->GetType()->AddPointer(sourcePos, moduleId))));
                std::unique_ptr<BoundFunctionCall> memberDestructorCall = ResolveOverload(U"@destructor", containerScope, memberDestructorCallLookups, memberDestructorCallArguments,
                    boundCompileUnit, boundFunction, sourcePos, moduleId);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(memberDestructorCall), sourcePos, moduleId)));
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
            cmajor::symbols::FunctionSymbol* thisToBaseConversion = boundCompileUnit.GetConversion(thisParam->GetType(), classType->BaseClass()->AddPointer(sourcePos, moduleId), containerScope, boundFunction,
                sourcePos, moduleId, argumentMatch);
            if (!thisToBaseConversion)
            {
                throw cmajor::symbols::Exception("base class conversion not found", destructorNode->GetSourcePos(), destructorNode->ModuleId(), classType->GetSourcePos(), classType->SourceModuleId());
            }
            BoundExpression* baseClassPointerConversion = new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(sourcePos, moduleId, thisParam)), thisToBaseConversion);
            baseDestructorCallArguments.push_back(std::unique_ptr<BoundExpression>(baseClassPointerConversion));
            std::unique_ptr<BoundFunctionCall> baseDestructorCall = ResolveOverload(U"@destructor", containerScope, baseDestructorCallLookups, baseDestructorCallArguments, boundCompileUnit,
                boundFunction, sourcePos, moduleId);
            boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundExpressionStatement(std::move(baseDestructorCall), sourcePos, moduleId)));
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not generate termination for class '" + util::ToUtf8(classType->FullName()) + "'. Reason: " + ex.Message(), destructorNode->GetSourcePos(), destructorNode->ModuleId(), references);
    }
}

Operation::Operation(const std::u32string& groupName_, int arity_, BoundCompileUnit& boundCompileUnit_) : groupName(groupName_), arity(arity_), boundCompileUnit(boundCompileUnit_)
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

void ArityOperation::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    for (Operation* operation : operations)
    {
        operation->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, sourcePos, moduleId, flags);
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

void OperationGroup::CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
    cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    int arity = arguments.size();
    if (arity < arityOperations.size())
    {
        ArityOperation* arityOperation = arityOperations[arity].get();
        if (arityOperation)
        {
            arityOperation->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, sourcePos, moduleId, flags);
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
    Add(new InterfaceDefaultConstructorOperation(boundCompileUnit));
    Add(new InterfaceCopyConstructorOperation(boundCompileUnit));
    Add(new InterfaceMoveConstructorOperation(boundCompileUnit));
    Add(new InterfaceCopyAssignmentOperation(boundCompileUnit));
    Add(new InterfaceMoveAssignmentOperation(boundCompileUnit));
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

void OperationRepository::CollectViableFunctions(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments,
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags)
{
    auto it = operationGroupMap.find(groupName);
    if (it != operationGroupMap.cend())
    {
        OperationGroup* operationGroup = it->second;
        operationGroup->CollectViableFunctions(containerScope, arguments, currentFunction, viableFunctions, exception, sourcePos, moduleId, flags);
    }
}

void OperationRepository::GenerateCopyConstructorFor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (boundCompileUnit.HasCopyConstructorFor(classTypeSymbol->TypeId())) return;
    std::unique_ptr<ClassCopyConstructor> copyConstructor(new ClassCopyConstructor(classTypeSymbol));
    copyConstructor->SetCompileUnit(boundCompileUnit.GetCompileUnitNode());
    copyConstructor->SetModule(&boundCompileUnit.GetModule());
    ClassCopyConstructorOperation* copyConstructorOp = static_cast<ClassCopyConstructorOperation*>(copyConstructorOperation);
    std::unique_ptr<cmajor::symbols::Exception> exception;
    if (copyConstructorOp->GenerateImplementation(copyConstructor.get(), containerScope, currentFunction, exception, sourcePos, moduleId))
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
            throw cmajor::symbols::Exception("could not generate copy constructor for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "'", sourcePos, moduleId);
        }
    }
}

void OperationRepository::GenerateCopyConstructorFor(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::symbols::ContainerScope* containerScope, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (boundCompileUnit.HasCopyConstructorFor(interfaceTypeSymbol->TypeId())) return;
    std::unique_ptr<cmajor::symbols::InterfaceTypeCopyConstructor> copyConstructor(new cmajor::symbols::InterfaceTypeCopyConstructor(interfaceTypeSymbol));
    boundCompileUnit.GetSymbolTable().SetFunctionIdFor(copyConstructor.get());
    copyConstructor->SetCompileUnit(boundCompileUnit.GetCompileUnitNode());
    copyConstructor->SetModule(&boundCompileUnit.GetModule());
    copyConstructor->SetParent(interfaceTypeSymbol);
    boundCompileUnit.AddCopyConstructorFor(interfaceTypeSymbol->TypeId(), std::move(copyConstructor));
}

} // namespace cmajor::binder
