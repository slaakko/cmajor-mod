// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>


module cmajor.binder.bound.expression;

import cmajor.binder.type.resolver;
import cmajor.binder.bound.node.visitor;
import util;

namespace cmajor::binder {

BoundExpression::BoundExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundNodeType boundNodeType_, cmajor::symbols::TypeSymbol* type_) :
    BoundNode(sourcePos_, moduleId_, boundNodeType_), type(type_), flags(BoundExpressionFlags::none)
{
}

void BoundExpression::AddTemporaryDestructorCall(std::unique_ptr<BoundFunctionCall>&& destructorCall)
{
    temporaryDestructorCalls.push_back(std::move(destructorCall));
}

void BoundExpression::MoveTemporaryDestructorCallsTo(BoundExpression& expression)
{
    for (std::unique_ptr<BoundFunctionCall>& destructorCall : temporaryDestructorCalls)
    {
        expression.AddTemporaryDestructorCall(std::move(destructorCall));
    }
    temporaryDestructorCalls.clear();
}

void BoundExpression::DestroyTemporaries(cmajor::ir::Emitter& emitter)
{
    for (const std::unique_ptr<BoundFunctionCall>& destructorCall : temporaryDestructorCalls)
    {
        destructorCall->Load(emitter, cmajor::ir::OperationFlags::none);
    }
}

BoundParameter::BoundParameter(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::ParameterSymbol* parameterSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundParameter, parameterSymbol_->GetType()), parameterSymbol(parameterSymbol_)
{
}

BoundExpression* BoundParameter::Clone()
{
    return new BoundParameter(GetSourcePos(), ModuleId(), parameterSymbol);
}

void BoundParameter::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = parameterSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Push(parameterSymbol->IrObject(emitter));
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* value = emitter.CreateLoad(type->IrType(emitter), parameterSymbol->IrObject(emitter));
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 0; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            value = emitter.CreateLoad(type->IrType(emitter), value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), parameterSymbol->IrObject(emitter)));
    }
    DestroyTemporaries(emitter);
}

void BoundParameter::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = parameterSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    void* value = emitter.Stack().Pop();
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a parameter", GetSourcePos(), ModuleId());
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* ptr = emitter.CreateLoad(type->IrType(emitter), parameterSymbol->IrObject(emitter));
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 1; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
        }
        emitter.CreateStore(value, ptr);
    }
    else
    {
        emitter.CreateStore(value, parameterSymbol->IrObject(emitter));
    }
    DestroyTemporaries(emitter);
}

void BoundParameter::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundLocalVariable::BoundLocalVariable(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::LocalVariableSymbol* localVariableSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundLocalVariable, localVariableSymbol_->GetType()), localVariableSymbol(localVariableSymbol_)
{
}

BoundExpression* BoundLocalVariable::Clone()
{
    return new BoundLocalVariable(GetSourcePos(), ModuleId(), localVariableSymbol);
}

void BoundLocalVariable::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = localVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Push(localVariableSymbol->IrObject(emitter));
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* value = emitter.CreateLoad(type->IrType(emitter), localVariableSymbol->IrObject(emitter));
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 0; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            value = emitter.CreateLoad(type->IrType(emitter), value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), localVariableSymbol->IrObject(emitter)));
    }
    DestroyTemporaries(emitter);
}

void BoundLocalVariable::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = localVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    void* value = emitter.Stack().Pop();
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot store to address of a local variable", GetSourcePos(), ModuleId());
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* ptr = emitter.CreateLoad(type->IrType(emitter), localVariableSymbol->IrObject(emitter));
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 1; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
        }
        emitter.CreateStore(value, ptr);
    }
    else
    {
        emitter.CreateStore(value, localVariableSymbol->IrObject(emitter));
    }
    DestroyTemporaries(emitter);
}

void BoundLocalVariable::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundMemberVariable::BoundMemberVariable(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_,cmajor::symbols::MemberVariableSymbol* memberVariableSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundMemberVariable, memberVariableSymbol_->GetType()),
    memberVariableSymbol(memberVariableSymbol_), staticInitNeeded(false)
{
}

BoundExpression* BoundMemberVariable::Clone()
{
    BoundMemberVariable* clone = new BoundMemberVariable(GetSourcePos(), ModuleId(), memberVariableSymbol);
    if (classPtr)
    {
        clone->classPtr.reset(classPtr->Clone());
    }
    if (staticInitNeeded)
    {
        clone->staticInitNeeded = true;
    }
    return clone;
}

void BoundMemberVariable::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = memberVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    Assert(memberVariableSymbol->LayoutIndex() != -1, "layout index of the member variable not set"); 
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableSymbol->Parent());
    if (memberVariableSymbol->IsStatic())
    {
        if (staticInitNeeded)
        {
            if (classType->StaticConstructor())
            {
                BoundFunctionCall staticConstructorCall(classType->StaticConstructor()->GetSourcePos(), classType->StaticConstructor()->SourceModuleId(), classType->StaticConstructor());
                staticConstructorCall.Load(emitter, cmajor::ir::OperationFlags::none);
            }
        }
        emitter.Stack().Push(classType->StaticObject(emitter, false));
    }
    else
    {
        if (!classPtr)
        {
            throw cmajor::symbols::Exception("class pointer of the member variable not set", GetSourcePos(), ModuleId());
        }
        classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* ptr = emitter.Stack().Pop();
    void* memberVariablePtr = emitter.GetMemberVariablePtr(classType->IrType(emitter), ptr, memberVariableSymbol->LayoutIndex());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Push(memberVariablePtr);
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* value = emitter.CreateLoad(type->IrType(emitter), memberVariablePtr);
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 0; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            value = emitter.CreateLoad(type->IrType(emitter), value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), memberVariablePtr));
    }
    DestroyTemporaries(emitter);
}

void BoundMemberVariable::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableSymbol->Parent());
    cmajor::symbols::TypeSymbol* type = memberVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    Assert(memberVariableSymbol->LayoutIndex() != -1, "layout index of the member variable not set"); 
    void* value = emitter.Stack().Pop();
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot store to the address of a member variable", GetSourcePos(), ModuleId());
    }
    else
    {
        if (memberVariableSymbol->IsStatic())
        {
            if (staticInitNeeded)
            {
                if (classType->StaticConstructor())
                {
                    BoundFunctionCall staticConstructorCall(classType->StaticConstructor()->GetSourcePos(), classType->StaticConstructor()->SourceModuleId(), classType->StaticConstructor());
                    staticConstructorCall.Load(emitter, cmajor::ir::OperationFlags::none);
                }
            }
            emitter.Stack().Push(classType->StaticObject(emitter, false));
        }
        else
        {
            classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
        }
        void* ptr = emitter.Stack().Pop();
        void* memberVariablePtr = emitter.GetMemberVariablePtr(classType->IrType(emitter), ptr, memberVariableSymbol->LayoutIndex());
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            void* ptr = emitter.CreateLoad(type->IrType(emitter), memberVariablePtr);
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 1; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
            }
            emitter.CreateStore(value, ptr);
        }
        else
        {
            emitter.CreateStore(value, memberVariablePtr);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundMemberVariable::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundMemberVariable::SetClassPtr(std::unique_ptr<BoundExpression>&& classPtr_)
{
    classPtr = std::move(classPtr_);
}

BoundConstant::BoundConstant(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::ConstantSymbol* constantSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundConstant, constantSymbol_->GetType()), constantSymbol(constantSymbol_)
{
}

BoundExpression* BoundConstant::Clone()
{
    return new BoundConstant(GetSourcePos(), ModuleId(), constantSymbol);
}

void BoundConstant::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if (constantSymbol->GetValue()->GetValueType() == cmajor::symbols::ValueType::arrayValue && (flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Push(constantSymbol->ArrayIrObject(emitter, false));
    }
    else
    {
        if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
        {
            throw cmajor::symbols::Exception("cannot take address of a constant", GetSourcePos(), ModuleId());
        }
        else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            throw cmajor::symbols::Exception("cannot dereference a constant", GetSourcePos(), ModuleId());
        }
        else
        {
            emitter.Stack().Push(constantSymbol->GetValue()->IrValue(emitter));
        }
    }
    DestroyTemporaries(emitter);
}

void BoundConstant::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a constant", GetSourcePos(), ModuleId());
}

void BoundConstant::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundEnumConstant::BoundEnumConstant(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::EnumConstantSymbol* enumConstantSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundEnumConstant, enumConstantSymbol_->GetType()), enumConstantSymbol(enumConstantSymbol_)
{
}

BoundExpression* BoundEnumConstant::Clone()
{
    return new BoundEnumConstant(GetSourcePos(), ModuleId(), enumConstantSymbol);
}

void BoundEnumConstant::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of an enumeration constant", GetSourcePos(), ModuleId());
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot dereference an enumeration constant", GetSourcePos(), ModuleId());
    }
    else
    {
        emitter.Stack().Push(enumConstantSymbol->GetValue()->IrValue(emitter));
    }
    DestroyTemporaries(emitter);
}

void BoundEnumConstant::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to an enumeration constant", GetSourcePos(), ModuleId());
}

void BoundEnumConstant::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundLiteral::BoundLiteral(std::unique_ptr<cmajor::symbols::Value>&& value_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(value_->GetSourcePos(), value_->ModuleId(), BoundNodeType::boundLiteral, type_), value(std::move(value_))
{
}

BoundExpression* BoundLiteral::Clone()
{
    std::unique_ptr<cmajor::symbols::Value> clonedValue;
    clonedValue.reset(value->Clone());
    return new BoundLiteral(std::move(clonedValue), GetType());
}

void BoundLiteral::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a literal", GetSourcePos(), ModuleId());
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot dereference a literal", GetSourcePos(), ModuleId());
    }
    else
    {
        emitter.Stack().Push(value->IrValue(emitter));
    }
    DestroyTemporaries(emitter);
}

void BoundLiteral::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a literal", GetSourcePos(), ModuleId());
}

void BoundLiteral::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

std::unique_ptr<cmajor::symbols::Value> BoundLiteral::ToValue(BoundCompileUnit& boundCompileUnit) const
{
    return std::unique_ptr<cmajor::symbols::Value>(value->Clone());
}

BoundGlobalVariable::BoundGlobalVariable(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundGlobalVariable, globalVariableSymbol_->GetType()), globalVariableSymbol(globalVariableSymbol_)
{
}

BoundExpression* BoundGlobalVariable::Clone()
{
    return new BoundGlobalVariable(GetSourcePos(), ModuleId(), globalVariableSymbol);
}

void BoundGlobalVariable::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = globalVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    void* globalVariablePtr = globalVariableSymbol->IrObject(emitter);
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Push(globalVariablePtr);
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        void* value = emitter.CreateLoad(type->IrType(emitter), globalVariablePtr);
        uint8_t n = GetDerefCount(flags);
        for (uint8_t i = 0; i < n; ++i)
        {
            type = type->RemovePointer(GetSourcePos(), ModuleId());
            value = emitter.CreateLoad(type->IrType(emitter), value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), globalVariablePtr));
    }
    DestroyTemporaries(emitter);
}

void BoundGlobalVariable::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    cmajor::symbols::TypeSymbol* type = globalVariableSymbol->GetType();
    emitter.SetCurrentDebugLocation(GetSourcePos());
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot store to the address of a global variable", GetSourcePos(), ModuleId());
    }
    else
    {
        void* value = emitter.Stack().Pop();
        void* ptr = globalVariableSymbol->IrObject(emitter);
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            void* loadedPtr = emitter.CreateLoad(type->IrType(emitter), ptr);
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 1; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                loadedPtr = emitter.CreateLoad(type->IrType(emitter), loadedPtr);
            }
            emitter.CreateStore(value, loadedPtr);
        }
        else
        {
            emitter.CreateStore(value, ptr);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundGlobalVariable::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundTemporary::BoundTemporary(std::unique_ptr<BoundExpression>&& rvalueExpr_, std::unique_ptr<BoundLocalVariable>&& backingStore_) :
    BoundExpression(rvalueExpr_->GetSourcePos(), rvalueExpr_->ModuleId(), BoundNodeType::boundTemporary, rvalueExpr_->GetType()), rvalueExpr(std::move(rvalueExpr_)), backingStore(std::move(backingStore_))
{
    rvalueExpr->MoveTemporaryDestructorCallsTo(*this);
    if (backingStore)
    {
        backingStore->MoveTemporaryDestructorCallsTo(*this);
    }
}

BoundExpression* BoundTemporary::Clone()
{
    std::unique_ptr<BoundExpression> clonedRvalueExpr;
    clonedRvalueExpr.reset(rvalueExpr->Clone());
    std::unique_ptr<BoundLocalVariable> clonedBackingStore;
    if (backingStore)
    {
        clonedBackingStore.reset(static_cast<BoundLocalVariable*>(backingStore->Clone()));
    }
    return new BoundTemporary(std::move(clonedRvalueExpr), std::move(clonedBackingStore));
}

void BoundTemporary::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    rvalueExpr->Load(emitter, cmajor::ir::OperationFlags::none);
    if (!backingStore)
    {
        throw cmajor::symbols::Exception("backing store of temporary not set", GetSourcePos(), ModuleId());
    }
    backingStore->Store(emitter, cmajor::ir::OperationFlags::none);
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        backingStore->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        backingStore->Load(emitter, SetDerefCount(cmajor::ir::OperationFlags::deref, GetDerefCount(flags) + 1));
    }
    else
    {
        backingStore->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    DestroyTemporaries(emitter);
}

void BoundTemporary::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a temporary", GetSourcePos(), ModuleId());
}

void BoundTemporary::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

std::unique_ptr<cmajor::symbols::Value> BoundTemporary::ToValue(BoundCompileUnit& boundCompileUnit) const
{
    return rvalueExpr->ToValue(boundCompileUnit);
}

bool BoundTemporary::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (rvalueExpr->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundSizeOfExpression::BoundSizeOfExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* pointerType_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundSizeOfExpression, type_), pointerType(pointerType_)
{
}

BoundExpression* BoundSizeOfExpression::Clone()
{
    return new BoundSizeOfExpression(GetSourcePos(), ModuleId(), GetType(), pointerType);
}

void BoundSizeOfExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a sizeof expression", GetSourcePos(), ModuleId());
    }
    else if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot dereference a sizeof expression", GetSourcePos(), ModuleId());
    }
    else
    {
        emitter.Stack().Push(emitter.SizeOf(pointerType->IrType(emitter)));
    }
    DestroyTemporaries(emitter);
}

void BoundSizeOfExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a sizeof expression", GetSourcePos(), ModuleId());
}

void BoundSizeOfExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundAddressOfExpression::BoundAddressOfExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(subject_->GetSourcePos(), subject_->ModuleId(), BoundNodeType::boundAddressOfExpression, type_), subject(std::move(subject_))
{
    subject->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundAddressOfExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedSubject;
    clonedSubject.reset(subject->Clone());
    return new BoundAddressOfExpression(std::move(clonedSubject), GetType());
}

void BoundAddressOfExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (subject->GetBoundNodeType() != BoundNodeType::boundDereferenceExpression)
    {
        subject->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        BoundDereferenceExpression* derefExpr = static_cast<BoundDereferenceExpression*>(subject.get());
        derefExpr->Subject()->Load(emitter, flags);
    }
    DestroyTemporaries(emitter);
}

void BoundAddressOfExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (subject->GetBoundNodeType() != BoundNodeType::boundDereferenceExpression)
    {
        subject->Store(emitter, flags);
    }
    else
    {
        BoundDereferenceExpression* derefExpr = static_cast<BoundDereferenceExpression*>(subject.get());
        derefExpr->Subject()->Store(emitter, flags);
    }
    DestroyTemporaries(emitter);
}

void BoundAddressOfExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

std::unique_ptr<cmajor::symbols::Value> BoundAddressOfExpression::ToValue(BoundCompileUnit& boundCompileUnit) const
{
    return subject->ToValue(boundCompileUnit);
}

bool BoundAddressOfExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (subject->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundDereferenceExpression::BoundDereferenceExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(subject_->GetSourcePos(), subject_->ModuleId(), BoundNodeType::boundDereferenceExpression, type_), subject(std::move(subject_))
{
    subject->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundDereferenceExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedSubject;
    clonedSubject.reset(subject->Clone());
    return new BoundDereferenceExpression(std::move(clonedSubject), GetType());
}

void BoundDereferenceExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (subject->GetBoundNodeType() != BoundNodeType::boundAddressOfExpression)
    {
        if (GetDerefCount(flags) == 0 && (flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
        {
            subject->Load(emitter, cmajor::ir::OperationFlags::none);
        }
        else
        {
            subject->Load(emitter, SetDerefCount(cmajor::ir::OperationFlags::deref, GetDerefCount(flags) + 1));
        }
    }
    else
    {
        BoundAddressOfExpression* addressOfExpr = static_cast<BoundAddressOfExpression*>(subject.get());
        addressOfExpr->Subject()->Load(emitter, flags);
    }
    DestroyTemporaries(emitter);
}

void BoundDereferenceExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (subject->GetBoundNodeType() != BoundNodeType::boundAddressOfExpression)
    {
        subject->Store(emitter, SetDerefCount(cmajor::ir::OperationFlags::deref | (flags & cmajor::ir::OperationFlags::functionCallFlags), GetDerefCount(flags) + 1));
    }
    else
    {
        BoundAddressOfExpression* addressOfExpr = static_cast<BoundAddressOfExpression*>(subject.get());
        addressOfExpr->Subject()->Store(emitter, flags | (flags & cmajor::ir::OperationFlags::functionCallFlags));
    }
    DestroyTemporaries(emitter);
}

void BoundDereferenceExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundDereferenceExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (subject->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundReferenceToPointerExpression::BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(subject_->GetSourcePos(), subject_->ModuleId(), BoundNodeType::boundReferenceToPointerExpression, type_), subject(std::move(subject_))
{
    subject->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundReferenceToPointerExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedSubject;
    clonedSubject.reset(subject->Clone());
    return new BoundReferenceToPointerExpression(std::move(clonedSubject), GetType());
}

void BoundReferenceToPointerExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    subject->Load(emitter, flags);
    DestroyTemporaries(emitter);
}

void BoundReferenceToPointerExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    subject->Store(emitter, flags);
    DestroyTemporaries(emitter);
}

void BoundReferenceToPointerExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundReferenceToPointerExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (subject->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundFunctionCall::BoundFunctionCall(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::FunctionSymbol* functionSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundFunctionCall, functionSymbol_->ReturnType()), functionSymbol(functionSymbol_)
{
}

BoundExpression* BoundFunctionCall::Clone()
{
    BoundFunctionCall* clone = new BoundFunctionCall(GetSourcePos(), ModuleId(), functionSymbol);
    for (std::unique_ptr<BoundExpression>& argument : arguments)
    {
        clone->AddArgument(std::unique_ptr<BoundExpression>(argument->Clone()));
    }
    for (const auto& p : temporaries)
    {
        clone->AddTemporary(std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(p->Clone())));
    }
    return clone;
}

void BoundFunctionCall::AddArgument(std::unique_ptr<BoundExpression>&& argument)
{
    argument->MoveTemporaryDestructorCallsTo(*this);
    arguments.push_back(std::move(argument));
}

void BoundFunctionCall::SetArguments(std::vector<std::unique_ptr<BoundExpression>>&& arguments_)
{
    arguments = std::move(arguments_);
}

void BoundFunctionCall::AddTemporary(std::unique_ptr<BoundLocalVariable>&& temporary)
{
    temporaries.push_back(std::move(temporary));
}

bool BoundFunctionCall::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    for (const std::unique_ptr<BoundExpression>& arg : arguments)
    {
        if (arg->ContainsExceptionCapture())
        {
            return true;
        }
    }
    return false;
}

std::vector<std::unique_ptr<cmajor::ir::GenObject>> BoundFunctionCall::ReleaseTemporaries()
{
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temps;
    for (std::unique_ptr<BoundLocalVariable>& temp : temporaries)
    {
        temps.push_back(std::move(temp));
    }
    return temps;
}

void BoundFunctionCall::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        if (functionSymbol->IsArrayElementAccess())
        {
            std::vector<cmajor::ir::GenObject*> genObjects;
            for (const std::unique_ptr<BoundExpression>& argument : arguments)
            {
                genObjects.push_back(argument.get());
                genObjects.back()->SetType(argument->GetType());
            }
            functionSymbol->GenerateCall(emitter, genObjects, flags, GetSourcePos(), ModuleId());
        }
        else
        {
            throw cmajor::symbols::Exception("cannot take address of a function call", GetSourcePos(), ModuleId());
        }
    }
    else
    {
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        for (const std::unique_ptr<BoundLocalVariable>& temporary : temporaries)
        {
            genObjects.push_back(temporary.get());
            genObjects.back()->SetType(temporary->GetType());
        }
        cmajor::ir::OperationFlags callFlags = flags & cmajor::ir::OperationFlags::functionCallFlags;
        if (GetFlag(BoundExpressionFlags::virtualCall))
        {
            Assert(!arguments.empty(), "nonempty argument list expected"); 
            genObjects[0]->SetType(arguments[0]->GetType());
            callFlags = callFlags | cmajor::ir::OperationFlags::virtualCall;
        }
        if (!functionSymbol->DontThrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        if (functionSymbol->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol && functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(functionSymbol->Parent());
            cmajor::symbols::MemberFunctionSymbol* interfaceMemberFunction = static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol);
            interfaceType->GenerateCall(emitter, genObjects, callFlags, interfaceMemberFunction, GetSourcePos(), ModuleId());
        }
        else
        {
            functionSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
        }
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            cmajor::symbols::TypeSymbol* type = functionSymbol->ReturnType();
            void* value = emitter.Stack().Pop();
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 0; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                value = emitter.CreateLoad(type->IrType(emitter), value);
            }
            emitter.Stack().Push(value);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundFunctionCall::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a function call", GetSourcePos(), ModuleId());
    }
    else
    {
        void* value = emitter.Stack().Pop();
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        cmajor::ir::OperationFlags callFlags = cmajor::ir::OperationFlags::none;
        if (GetFlag(BoundExpressionFlags::virtualCall))
        {
            callFlags = callFlags | cmajor::ir::OperationFlags::virtualCall;
        }
        if (!functionSymbol->DontThrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        if (functionSymbol->IsArrayElementAccess())
        {
            functionSymbol->GenerateCall(emitter, genObjects, callFlags | cmajor::ir::OperationFlags::addr, GetSourcePos(), ModuleId());
            void* ptr = emitter.Stack().Pop();
            emitter.CreateStore(value, ptr);
        }
        else
        {
            functionSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
            cmajor::symbols::TypeSymbol* type = functionSymbol->ReturnType();
            void* ptr = emitter.Stack().Pop();
            if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
            {
                emitter.SaveObjectPointer(ptr);
            }
            if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none || GetFlag(BoundExpressionFlags::deref))
            {
                uint8_t n = GetDerefCount(flags);
                for (uint8_t i = 1; i < n; ++i)
                {
                    type = type->RemovePointer(GetSourcePos(), ModuleId());
                    ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
                }
                emitter.CreateStore(value, ptr);
            }
            else
            {
                emitter.CreateStore(emitter.CreateLoad(type->IrType(emitter), value), ptr);
            }
        }
    }
    DestroyTemporaries(emitter);
}

void BoundFunctionCall::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundFunctionCall::HasValue() const
{
    return functionSymbol->ReturnType() && functionSymbol->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol;
}

bool BoundFunctionCall::IsLvalueExpression() const
{
    if (functionSymbol->IsArrayElementAccess()) return true;
    cmajor::symbols::TypeSymbol* returnType = functionSymbol->ReturnType();
    if (returnType && returnType->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol)
    {
        return !returnType->IsConstType() && returnType->IsLvalueReferenceType();
    }
    return false;
}

BoundDelegateCall::BoundDelegateCall(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::DelegateTypeSymbol* delegateType_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundDelegateCall, delegateType_->ReturnType()), delegateTypeSymbol(delegateType_), arguments()
{
}

BoundExpression* BoundDelegateCall::Clone()
{
    return new BoundDelegateCall(GetSourcePos(), ModuleId(), delegateTypeSymbol);
}

void BoundDelegateCall::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a delegate call", GetSourcePos(), ModuleId());
    }
    else
    {
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        cmajor::ir::OperationFlags callFlags = flags & cmajor::ir::OperationFlags::functionCallFlags;
        if (!delegateTypeSymbol->IsNothrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        delegateTypeSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
        cmajor::symbols::TypeSymbol* type = delegateTypeSymbol->ReturnType();
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            void* value = emitter.Stack().Pop();
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 0; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                value = emitter.CreateLoad(type->IrType(emitter), value);
            }
            emitter.Stack().Push(value);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundDelegateCall::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a delegate call", GetSourcePos(), ModuleId());
    }
    else
    {
        void* value = emitter.Stack().Pop();
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        cmajor::ir::OperationFlags callFlags = cmajor::ir::OperationFlags::none;
        if (GetFlag(BoundExpressionFlags::virtualCall))
        {
            callFlags = callFlags | cmajor::ir::OperationFlags::virtualCall;
        }
        if (!delegateTypeSymbol->IsNothrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        delegateTypeSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
        cmajor::symbols::TypeSymbol* type = delegateTypeSymbol->ReturnType();
        void* ptr = emitter.Stack().Pop();
        if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
        {
            emitter.SaveObjectPointer(ptr);
        }
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none || GetFlag(BoundExpressionFlags::deref))
        {
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 1; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
            }
            emitter.CreateStore(value, ptr);
        }
        else
        {

            emitter.CreateStore(emitter.CreateLoad(type->IrType(emitter), value), ptr);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundDelegateCall::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundDelegateCall::HasValue() const
{
    return delegateTypeSymbol->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol;
}

bool BoundDelegateCall::IsLvalueExpression() const
{
    cmajor::symbols::TypeSymbol* returnType = delegateTypeSymbol->ReturnType();
    if (returnType->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol)
    {
        return !returnType->IsConstType() && returnType->IsLvalueReferenceType();
    }
    return false;
}

void BoundDelegateCall::AddArgument(std::unique_ptr<BoundExpression>&& argument)
{
    arguments.push_back(std::move(argument));
}

bool BoundDelegateCall::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    for (const std::unique_ptr<BoundExpression>& arg : arguments)
    {
        if (arg->ContainsExceptionCapture())
        {
            return true;
        }
    }
    return false;
}

BoundClassDelegateCall::BoundClassDelegateCall(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundClassDelegateCall, classDelegateType_->ReturnType()), classDelegateTypeSymbol(classDelegateType_), arguments()
{
}

BoundExpression* BoundClassDelegateCall::Clone()
{
    return new BoundClassDelegateCall(GetSourcePos(), ModuleId(), classDelegateTypeSymbol);
}

void BoundClassDelegateCall::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a class delegate call", GetSourcePos(), ModuleId());
    }
    else
    {
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        cmajor::ir::OperationFlags callFlags = flags & cmajor::ir::OperationFlags::functionCallFlags;
        if (!classDelegateTypeSymbol->IsNothrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        classDelegateTypeSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
        cmajor::symbols::TypeSymbol* type = classDelegateTypeSymbol->ReturnType();
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none)
        {
            void* value = emitter.Stack().Pop();
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 0; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                value = emitter.CreateLoad(type->IrType(emitter), value);
            }
            emitter.Stack().Push(value);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundClassDelegateCall::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a clas delegate call", GetSourcePos(), ModuleId());
    }
    else
    {
        void* value = emitter.Stack().Pop();
        std::vector<cmajor::ir::GenObject*> genObjects;
        for (const std::unique_ptr<BoundExpression>& argument : arguments)
        {
            genObjects.push_back(argument.get());
            genObjects.back()->SetType(argument->GetType());
        }
        cmajor::ir::OperationFlags callFlags = cmajor::ir::OperationFlags::none;
        if (GetFlag(BoundExpressionFlags::virtualCall))
        {
            callFlags = callFlags | cmajor::ir::OperationFlags::virtualCall;
        }
        if (!classDelegateTypeSymbol->IsNothrow())
        {
            emitter.SetLineNumber(GetSourcePos().line);
        }
        classDelegateTypeSymbol->GenerateCall(emitter, genObjects, callFlags, GetSourcePos(), ModuleId());
        cmajor::symbols::TypeSymbol* type = classDelegateTypeSymbol->ReturnType();
        void* ptr = emitter.Stack().Pop();
        if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
        {
            emitter.SaveObjectPointer(ptr);
        }
        if ((flags & cmajor::ir::OperationFlags::deref) != cmajor::ir::OperationFlags::none || GetFlag(BoundExpressionFlags::deref))
        {
            uint8_t n = GetDerefCount(flags);
            for (uint8_t i = 1; i < n; ++i)
            {
                type = type->RemovePointer(GetSourcePos(), ModuleId());
                ptr = emitter.CreateLoad(type->IrType(emitter), ptr);
            }
            emitter.CreateStore(value, ptr);
        }
        else
        {
            emitter.CreateStore(emitter.CreateLoad(type->IrType(emitter), value), ptr);
        }
    }
    DestroyTemporaries(emitter);
}

void BoundClassDelegateCall::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundClassDelegateCall::HasValue() const
{
    return classDelegateTypeSymbol->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol;
}

bool BoundClassDelegateCall::IsLvalueExpression() const
{
    cmajor::symbols::TypeSymbol* returnType = classDelegateTypeSymbol->ReturnType();
    if (returnType->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol)
    {
        return !returnType->IsConstType() && returnType->IsLvalueReferenceType();
    }
    return false;
}

void BoundClassDelegateCall::AddArgument(std::unique_ptr<BoundExpression>&& argument)
{
    arguments.push_back(std::move(argument));
}

bool BoundClassDelegateCall::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    for (const std::unique_ptr<BoundExpression>& arg : arguments)
    {
        if (arg->ContainsExceptionCapture())
        {
            return true;
        }
    }
    return false;
}

BoundConstructExpression::BoundConstructExpression(std::unique_ptr<BoundExpression>&& constructorCall_, cmajor::symbols::TypeSymbol* resultType_) :
    BoundExpression(constructorCall_->GetSourcePos(), constructorCall_->ModuleId(), BoundNodeType::boundConstructExpression, resultType_), constructorCall(std::move(constructorCall_))
{
    constructorCall->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundConstructExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedConstructorCall;
    clonedConstructorCall.reset(constructorCall->Clone());
    return new BoundConstructExpression(std::move(clonedConstructorCall), GetType());
}

void BoundConstructExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    void* prevObjectPointer = emitter.GetObjectPointer();
    emitter.SetObjectPointer(nullptr);
    if ((flags & cmajor::ir::OperationFlags::addr) != cmajor::ir::OperationFlags::none)
    {
        throw cmajor::symbols::Exception("cannot take address of a construct expression", GetSourcePos(), ModuleId());
    }
    else
    {
        constructorCall->Load(emitter, cmajor::ir::OperationFlags::leaveFirstArg);
        void* objectPointer = emitter.GetObjectPointer();
        if (!objectPointer)
        {
            throw cmajor::symbols::Exception("do not have object pointer", GetSourcePos(), ModuleId());
        }
        else
        {
            emitter.Stack().Push(objectPointer);
        }
    }
    DestroyTemporaries(emitter);
    emitter.SetObjectPointer(prevObjectPointer);
}

void BoundConstructExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to construct expression", GetSourcePos(), ModuleId());
}

void BoundConstructExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConstructExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return constructorCall->ContainsExceptionCapture();
}

BoundConstructAndReturnTemporaryExpression::BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>&& constructorCall_, std::unique_ptr<BoundExpression>&& boundTemporary_) :
    BoundExpression(constructorCall_->GetSourcePos(), constructorCall_->ModuleId(), BoundNodeType::boundConstructAndReturnTemporary, boundTemporary_->GetType()), constructorCall(std::move(constructorCall_)),
    boundTemporary(std::move(boundTemporary_))
{
    constructorCall->MoveTemporaryDestructorCallsTo(*this);
    boundTemporary->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundConstructAndReturnTemporaryExpression::Clone()
{
    return new BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>(constructorCall->Clone()), std::unique_ptr<BoundExpression>(boundTemporary->Clone()));
}

void BoundConstructAndReturnTemporaryExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    constructorCall->Load(emitter, cmajor::ir::OperationFlags::none);
    if (boundTemporary->GetType()->IsClassTypeSymbol() || boundTemporary->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
    {
        flags = flags | cmajor::ir::OperationFlags::addr;
    }
    boundTemporary->Load(emitter, flags);
    DestroyTemporaries(emitter);
}

void BoundConstructAndReturnTemporaryExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to construct and return temporary expression", GetSourcePos(), ModuleId());
}

void BoundConstructAndReturnTemporaryExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConstructAndReturnTemporaryExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (constructorCall->ContainsExceptionCapture())
    {
        return true;
    }
    if (boundTemporary->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundClassOrClassDelegateConversionResult::BoundClassOrClassDelegateConversionResult(std::unique_ptr<BoundExpression>&& conversionResult_, std::unique_ptr<BoundFunctionCall>&& conversionFunctionCall_) :
    BoundExpression(conversionResult_->GetSourcePos(), conversionResult_->ModuleId(), BoundNodeType::boundClassOrClassDelegateConversionResult, conversionResult_->GetType()),
    conversionResult(std::move(conversionResult_)), conversionFunctionCall(std::move(conversionFunctionCall_))
{
}

BoundExpression* BoundClassOrClassDelegateConversionResult::Clone()
{
    return new BoundClassOrClassDelegateConversionResult(std::unique_ptr<BoundExpression>(conversionResult->Clone()),
        std::unique_ptr<BoundFunctionCall>(static_cast<BoundFunctionCall*>(conversionFunctionCall->Clone())));
}

void BoundClassOrClassDelegateConversionResult::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    conversionFunctionCall->Load(emitter, cmajor::ir::OperationFlags::none);
    conversionResult->Load(emitter, flags);
}

void BoundClassOrClassDelegateConversionResult::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to class conversion result", GetSourcePos(), ModuleId());
}

void BoundClassOrClassDelegateConversionResult::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundClassOrClassDelegateConversionResult::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (conversionResult->ContainsExceptionCapture())
    {
        return true;
    }
    if (conversionFunctionCall->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundConversion::BoundConversion(std::unique_ptr<BoundExpression>&& sourceExpr_, cmajor::symbols::FunctionSymbol* conversionFun_) :
    BoundExpression(sourceExpr_->GetSourcePos(), sourceExpr_->ModuleId(), BoundNodeType::boundConversion, conversionFun_->ConversionTargetType()), sourceExpr(std::move(sourceExpr_)), conversionFun(conversionFun_)
{
    sourceExpr->MoveTemporaryDestructorCallsTo(*this);
}

void BoundConversion::AddTemporary(std::unique_ptr<BoundLocalVariable>&& temporary)
{
    temporaries.push_back(std::move(temporary));
}

BoundExpression* BoundConversion::Clone()
{
    std::unique_ptr<BoundExpression> clonedSourceExpr;
    clonedSourceExpr.reset(sourceExpr->Clone());
    BoundConversion* clone = new BoundConversion(std::move(clonedSourceExpr), conversionFun);
    for (const auto& p : temporaries)
    {
        clone->AddTemporary(std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(p->Clone())));
    }
    return clone;
}

void BoundConversion::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    sourceExpr->Load(emitter, flags);
    std::vector<cmajor::ir::GenObject*> genObjects;
    for (const std::unique_ptr<BoundLocalVariable>& temporary : temporaries)
    {
        genObjects.push_back(temporary.get());
        genObjects.back()->SetType(temporary->GetType());
    }
    conversionFun->GenerateCall(emitter, genObjects, cmajor::ir::OperationFlags::none, GetSourcePos(), ModuleId());
    DestroyTemporaries(emitter);
}

void BoundConversion::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a conversion", GetSourcePos(), ModuleId());
}

bool BoundConversion::IsLvalueExpression() const
{
    if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol) return true;
    if (conversionFun->IsClassToInterfaceTypeConversion()) return true;
    if (conversionFun->IsMemberFunctionToClassDelegateConversion()) return true;
    return false;
}

void BoundConversion::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

std::unique_ptr<cmajor::symbols::Value> BoundConversion::ToValue(BoundCompileUnit& boundCompileUnit) const
{
    std::unique_ptr<cmajor::symbols::Value> sourceValue = sourceExpr->ToValue(boundCompileUnit);
    if (sourceValue)
    {
        return conversionFun->ConvertValue(sourceValue);
    }
    return std::unique_ptr<cmajor::symbols::Value>();
}

bool BoundConversion::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return sourceExpr->ContainsExceptionCapture();
}

BoundIsExpression::BoundIsExpression(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::ClassTypeSymbol* rightClassType_, cmajor::symbols::TypeSymbol* boolType_,
    std::unique_ptr<BoundLocalVariable>&& leftClassIdVar_, std::unique_ptr<BoundLocalVariable>&& rightClassIdVar_) :
    BoundExpression(expr_->GetSourcePos(), expr_->ModuleId(), BoundNodeType::boundIsExpression, boolType_), expr(std::move(expr_)), rightClassType(rightClassType_),
    leftClassIdVar(std::move(leftClassIdVar_)), rightClassIdVar(std::move(rightClassIdVar_))
{
}

BoundExpression* BoundIsExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedExpr;
    clonedExpr.reset(expr->Clone());
    return new BoundIsExpression(std::move(clonedExpr), rightClassType, GetType(),
        std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(leftClassIdVar->Clone())),
        std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(rightClassIdVar->Clone())));
}

void BoundIsExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        expr->Load(emitter, cmajor::ir::OperationFlags::none);
        void* thisPtr = emitter.Stack().Pop();
        cmajor::symbols::TypeSymbol* exprType = static_cast<cmajor::symbols::TypeSymbol*>(expr->GetType());
        Assert(exprType->IsPointerType(), "pointer type expected"); 
        cmajor::symbols::TypeSymbol* leftType = exprType->RemovePointer(GetSourcePos(), ModuleId());
        Assert(leftType->IsClassTypeSymbol(), "class type expected"); 
        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftType);
        cmajor::symbols::ClassTypeSymbol* leftVmtPtrHolderClass = leftClassType->VmtPtrHolderClass();
        if (leftClassType != leftVmtPtrHolderClass)
        {
            thisPtr = emitter.CreateBitCast(thisPtr, leftVmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
        }
        void* vmtPtr = emitter.GetVmtPtr(leftVmtPtrHolderClass->IrType(emitter), thisPtr, leftVmtPtrHolderClass->VmtPtrIndex(), leftClassType->VmtPtrType(emitter));
        void* leftClassIdPtr = emitter.GetClassIdPtr(vmtPtr, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* rightClassTypeVmtObject = rightClassType->VmtObject(emitter, false);
        void* rightClassIdPtr = emitter.GetClassIdPtr(rightClassTypeVmtObject, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* retType = emitter.GetIrTypeForBool();
        std::vector<void*> paramTypes;
        paramTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        paramTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        void* dynamicInitAndCompareFnType = emitter.GetIrTypeForFunction(retType, paramTypes);
        void* dynamicInitAndCompareFn = emitter.GetOrInsertFunction("DynamicInitVmtsAndCompare", dynamicInitAndCompareFnType, true);
        std::vector<void*> args;
        args.push_back(emitter.CreateBitCast(leftClassIdPtr, emitter.GetIrTypeForVoidPtrType()));
        args.push_back(emitter.CreateBitCast(rightClassIdPtr, emitter.GetIrTypeForVoidPtrType()));
        emitter.Stack().Push(emitter.CreateCall(dynamicInitAndCompareFnType, dynamicInitAndCompareFn, args));
        DestroyTemporaries(emitter);
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        expr->Load(emitter, cmajor::ir::OperationFlags::none);
        void* thisPtr = emitter.Stack().Pop();
        cmajor::symbols::TypeSymbol* exprType = static_cast<cmajor::symbols::TypeSymbol*>(expr->GetType());
        Assert(exprType->IsPointerType(), "pointer type expected"); 
        cmajor::symbols::TypeSymbol* leftType = exprType->RemovePointer(GetSourcePos(), ModuleId());
        Assert(leftType->IsClassTypeSymbol(), "class type expected"); 
        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftType);
        cmajor::symbols::ClassTypeSymbol* leftVmtPtrHolderClass = leftClassType->VmtPtrHolderClass();
        if (leftClassType != leftVmtPtrHolderClass)
        {
            thisPtr = emitter.CreateBitCast(thisPtr, leftVmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
        }
        void* vmtPtr = emitter.GetVmtPtr(leftVmtPtrHolderClass->IrType(emitter), thisPtr, leftVmtPtrHolderClass->VmtPtrIndex(), leftClassType->VmtPtrType(emitter));
        void* leftClassIdPtr = emitter.GetClassIdPtr(vmtPtr, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* leftClassId = emitter.CreatePtrToInt(emitter.CreateLoad(emitter.GetIrTypeForULong(), leftClassIdPtr), emitter.GetIrTypeForULong());
        void* rightClassTypeVmtObject = rightClassType->VmtObject(emitter, false);
        void* rightClassIdPtr = emitter.GetClassIdPtr(rightClassTypeVmtObject, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* rightClassId = emitter.CreatePtrToInt(emitter.CreateLoad(emitter.GetIrTypeForULong(), rightClassIdPtr), emitter.GetIrTypeForULong());
        void* remainder = emitter.CreateURem(leftClassId, rightClassId);
        void* remainderIsZero = emitter.CreateICmpEQ(remainder, emitter.CreateDefaultIrValueForULong());
        emitter.Stack().Push(remainderIsZero);
        DestroyTemporaries(emitter);
    }
}

void BoundIsExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a 'is' expression", GetSourcePos(), ModuleId());
}

void BoundIsExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundIsExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return expr->ContainsExceptionCapture();
}

BoundAsExpression::BoundAsExpression(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::ClassTypeSymbol* rightClassType_, std::unique_ptr<BoundLocalVariable>&& variable_,
    std::unique_ptr<BoundLocalVariable>&& leftClassIdVar_, std::unique_ptr<BoundLocalVariable>&& rightClassIdVar_) :
    BoundExpression(expr_->GetSourcePos(), expr_->ModuleId(), BoundNodeType::boundAsExpression, rightClassType_->AddPointer(expr_->GetSourcePos(), expr_->ModuleId())),
    expr(std::move(expr_)), rightClassType(rightClassType_), variable(std::move(variable_)),
    leftClassIdVar(std::move(leftClassIdVar_)), rightClassIdVar(std::move(rightClassIdVar_))
{
}

BoundExpression* BoundAsExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedExpr;
    clonedExpr.reset(expr->Clone());
    std::unique_ptr<BoundLocalVariable> clonedVariable;
    clonedVariable.reset(static_cast<BoundLocalVariable*>(variable->Clone()));
    return new BoundAsExpression(std::move(clonedExpr), rightClassType, std::move(clonedVariable),
        std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(leftClassIdVar->Clone())),
        std::unique_ptr<BoundLocalVariable>(static_cast<BoundLocalVariable*>(rightClassIdVar->Clone())));
}

void BoundAsExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        expr->Load(emitter, cmajor::ir::OperationFlags::none);
        void* thisPtr = emitter.Stack().Pop();
        cmajor::symbols::TypeSymbol* exprType = static_cast<cmajor::symbols::TypeSymbol*>(expr->GetType());
        Assert(exprType->IsPointerType(), "pointer type expected"); 
        cmajor::symbols::TypeSymbol* leftType = exprType->RemovePointer(GetSourcePos(), ModuleId());
        Assert(leftType->IsClassTypeSymbol(), "class type expected"); 
        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftType);
        cmajor::symbols::ClassTypeSymbol* leftVmtPtrHolderClass = leftClassType->VmtPtrHolderClass();
        if (leftClassType != leftVmtPtrHolderClass)
        {
            thisPtr = emitter.CreateBitCast(thisPtr, leftVmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
        }
        void* vmtPtr = emitter.GetVmtPtr(leftVmtPtrHolderClass->IrType(emitter), thisPtr, leftVmtPtrHolderClass->VmtPtrIndex(), leftClassType->VmtPtrType(emitter));
        void* leftClassIdPtr = emitter.GetClassIdPtr(vmtPtr, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* rightClassTypeVmtObject = rightClassType->VmtObject(emitter, false);
        void* rightClassIdPtr = emitter.GetClassIdPtr(rightClassTypeVmtObject, cmajor::symbols::GetClassIdVmtIndexOffset());

        void* retType = emitter.GetIrTypeForBool();
        std::vector<void*> paramTypes;
        paramTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        paramTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        void* dynamicInitAndCompareFnType = emitter.GetIrTypeForFunction(retType, paramTypes);
        void* dynamicInitAndCompareFn = emitter.GetOrInsertFunction("DynamicInitVmtsAndCompare", dynamicInitAndCompareFnType, true);
        std::vector<void*> args;
        args.push_back(emitter.CreateBitCast(leftClassIdPtr, emitter.GetIrTypeForVoidPtrType()));
        args.push_back(emitter.CreateBitCast(rightClassIdPtr, emitter.GetIrTypeForVoidPtrType()));
        emitter.Stack().Push(emitter.CreateCall(dynamicInitAndCompareFnType, dynamicInitAndCompareFn, args));
        void* remainderIsZero = emitter.Stack().Pop();
        void* trueBlock = emitter.CreateBasicBlock("true");
        void* falseBlock = emitter.CreateBasicBlock("false");
        void* continueBlock = emitter.CreateBasicBlock("continue");
        emitter.CreateCondBr(remainderIsZero, trueBlock, falseBlock);
        emitter.SetCurrentBasicBlock(trueBlock);
        emitter.Stack().Push(emitter.CreateBitCast(thisPtr, rightClassType->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter)));
        variable->Store(emitter, cmajor::ir::OperationFlags::none);
        emitter.CreateBr(continueBlock);
        emitter.SetCurrentBasicBlock(falseBlock);
        emitter.Stack().Push(emitter.CreateDefaultIrValueForPtrType(rightClassType->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter)));
        variable->Store(emitter, cmajor::ir::OperationFlags::none);
        emitter.CreateBr(continueBlock);
        emitter.SetCurrentBasicBlock(continueBlock);
        variable->Load(emitter, cmajor::ir::OperationFlags::none);
        DestroyTemporaries(emitter);
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        expr->Load(emitter, cmajor::ir::OperationFlags::none);
        void* thisPtr = emitter.Stack().Pop();
        cmajor::symbols::TypeSymbol* exprType = static_cast<cmajor::symbols::TypeSymbol*>(expr->GetType());
        Assert(exprType->IsPointerType(), "pointer type expected"); 
        cmajor::symbols::TypeSymbol* leftType = exprType->RemovePointer(GetSourcePos(), ModuleId());
        Assert(leftType->IsClassTypeSymbol(), "class type expected"); 
        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftType);
        cmajor::symbols::ClassTypeSymbol* leftVmtPtrHolderClass = leftClassType->VmtPtrHolderClass();
        if (leftClassType != leftVmtPtrHolderClass)
        {
            thisPtr = emitter.CreateBitCast(thisPtr, leftVmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
        }
        void* vmtPtr = emitter.GetVmtPtr(leftVmtPtrHolderClass->IrType(emitter), thisPtr, leftVmtPtrHolderClass->VmtPtrIndex(), leftClassType->VmtPtrType(emitter));
        void* leftClassIdPtr = emitter.GetClassIdPtr(vmtPtr, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* leftClassId = emitter.CreatePtrToInt(emitter.CreateLoad(emitter.GetIrTypeForULong(), leftClassIdPtr), emitter.GetIrTypeForULong());
        void* rightClassTypeVmtObject = rightClassType->VmtObject(emitter, false);
        void* rightClassIdPtr = emitter.GetClassIdPtr(rightClassTypeVmtObject, cmajor::symbols::GetClassIdVmtIndexOffset());
        void* rightClassId = emitter.CreatePtrToInt(emitter.CreateLoad(emitter.GetIrTypeForULong(), rightClassIdPtr), emitter.GetIrTypeForULong());
        void* remainder = emitter.CreateURem(leftClassId, rightClassId);
        void* remainderIsZero = emitter.CreateICmpEQ(remainder, emitter.CreateDefaultIrValueForULong());
        void* trueBlock = emitter.CreateBasicBlock("true");
        void* falseBlock = emitter.CreateBasicBlock("false");
        void* continueBlock = emitter.CreateBasicBlock("continue");
        emitter.CreateCondBr(remainderIsZero, trueBlock, falseBlock);
        emitter.SetCurrentBasicBlock(trueBlock);
        emitter.Stack().Push(emitter.CreateBitCast(thisPtr, rightClassType->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter)));
        variable->Store(emitter, cmajor::ir::OperationFlags::none);
        emitter.CreateBr(continueBlock);
        emitter.SetCurrentBasicBlock(falseBlock);
        emitter.Stack().Push(emitter.CreateDefaultIrValueForPtrType(rightClassType->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter)));
        variable->Store(emitter, cmajor::ir::OperationFlags::none);
        emitter.CreateBr(continueBlock);
        emitter.SetCurrentBasicBlock(continueBlock);
        variable->Load(emitter, cmajor::ir::OperationFlags::none);
        DestroyTemporaries(emitter);
    }
}

void BoundAsExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to an 'as' expression", GetSourcePos(), ModuleId());
}

void BoundAsExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundAsExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return expr->ContainsExceptionCapture();
}

BoundTypeNameExpression::BoundTypeNameExpression(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::TypeSymbol* constCharPtrType_) :
    BoundExpression(classPtr_->GetSourcePos(), classPtr_->ModuleId(), BoundNodeType::boundTypeNameExpression, constCharPtrType_), classPtr(std::move(classPtr_))
{
    classPtr->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundTypeNameExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedClassPtr;
    clonedClassPtr.reset(classPtr->Clone());
    return new BoundTypeNameExpression(std::move(clonedClassPtr), GetType());
}

void BoundTypeNameExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thisPtr = emitter.Stack().Pop();
    cmajor::symbols::TypeSymbol* classPtrType = static_cast<cmajor::symbols::TypeSymbol*>(classPtr->GetType());
    Assert(classPtrType->IsPointerType(), "pointer type expected"); 
    cmajor::symbols::TypeSymbol* type = classPtrType->BaseType();
    Assert(type->IsClassTypeSymbol(), "class type expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
    cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
    if (classType != vmtPtrHolderClass)
    {
        thisPtr = emitter.CreateBitCast(thisPtr, vmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
    }
    void* vmtPtr = emitter.GetVmtPtr(vmtPtrHolderClass->IrType(emitter), thisPtr, vmtPtrHolderClass->VmtPtrIndex(), classType->VmtPtrType(emitter));
    void* className = emitter.GetClassName(vmtPtr, cmajor::symbols::GetClassNameVmtIndexOffset());
    emitter.Stack().Push(className);
    DestroyTemporaries(emitter);
}

void BoundTypeNameExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to typename expression", GetSourcePos(), ModuleId());
}

void BoundTypeNameExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundTypeNameExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return classPtr->ContainsExceptionCapture();
}

BoundTypeIdExpression::BoundTypeIdExpression(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::TypeSymbol* ulongType_) :
    BoundExpression(classPtr_->GetSourcePos(), classPtr_->ModuleId(), BoundNodeType::boundTypeIdExpression, ulongType_), classPtr(std::move(classPtr_))
{
    classPtr->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundTypeIdExpression::Clone()
{
    std::unique_ptr<BoundExpression> clonedClassPtr;
    clonedClassPtr.reset(classPtr->Clone());
    return new BoundTypeIdExpression(std::move(clonedClassPtr), GetType());
}

void BoundTypeIdExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thisPtr = emitter.Stack().Pop();
    cmajor::symbols::TypeSymbol* classPtrType = static_cast<cmajor::symbols::TypeSymbol*>(classPtr->GetType());
    Assert(classPtrType->IsPointerType(), "pointer type expected");
    cmajor::symbols::TypeSymbol* type = classPtrType->BaseType();
    Assert(type->IsClassTypeSymbol(), "class type expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
    cmajor::symbols::ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
    if (classType != vmtPtrHolderClass)
    {
        thisPtr = emitter.CreateBitCast(thisPtr, vmtPtrHolderClass->AddPointer(GetSourcePos(), ModuleId())->IrType(emitter));
    }
    void* vmtPtr = emitter.GetVmtPtr(vmtPtrHolderClass->IrType(emitter), thisPtr, vmtPtrHolderClass->VmtPtrIndex(), classType->VmtPtrType(emitter));
    void* classIdPtr = emitter.GetClassIdPtr(vmtPtr, cmajor::symbols::GetClassIdVmtIndexOffset());
    void* classId = emitter.CreatePtrToInt(emitter.CreateLoad(emitter.GetIrTypeForULong(), classIdPtr), emitter.GetIrTypeForULong());
    emitter.Stack().Push(classId);
    DestroyTemporaries(emitter);
}

void BoundTypeIdExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to typeid expression", GetSourcePos(), ModuleId());
}

void BoundTypeIdExpression::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundTypeIdExpression::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return classPtr->ContainsExceptionCapture();
}

BoundBitCast::BoundBitCast(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(expr_->GetSourcePos(), expr_->ModuleId(), BoundNodeType::boundBitCast, type_), expr(std::move(expr_))
{
    expr->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundBitCast::Clone()
{
    return new BoundBitCast(std::unique_ptr<BoundExpression>(expr->Clone()), GetType());
}

void BoundBitCast::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    expr->Load(emitter, cmajor::ir::OperationFlags::none);
    void* value = emitter.Stack().Pop();
    void* casted = emitter.CreateBitCast(value, GetType()->IrType(emitter));
    emitter.Stack().Push(casted);
    DestroyTemporaries(emitter);
}

void BoundBitCast::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to bit cast", GetSourcePos(), ModuleId());
}

void BoundBitCast::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundBitCast::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    return expr->ContainsExceptionCapture();
}

BoundFunctionPtr::BoundFunctionPtr(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::FunctionSymbol* function_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundFunctionPtr, type_), function(function_)
{
}

BoundExpression* BoundFunctionPtr::Clone()
{
    return new BoundFunctionPtr(GetSourcePos(), ModuleId(), function, GetType());
}

void BoundFunctionPtr::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    void* irObject = emitter.GetOrInsertFunction(util::ToUtf8(function->MangledName()), function->IrType(emitter), function->DontThrow());
    emitter.Stack().Push(irObject);
    DestroyTemporaries(emitter);
}

void BoundFunctionPtr::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to function ptr expression", GetSourcePos(), ModuleId());
}

void BoundFunctionPtr::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundDisjunction::BoundDisjunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, std::unique_ptr<BoundExpression>&& left_, std::unique_ptr<BoundExpression>&& right_, cmajor::symbols::TypeSymbol* boolType_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundDisjunction, boolType_), left(std::move(left_)), right(std::move(right_))
{
}

BoundExpression* BoundDisjunction::Clone()
{
    return new BoundDisjunction(GetSourcePos(), ModuleId(), std::unique_ptr<BoundExpression>(left->Clone()), std::unique_ptr<BoundExpression>(right->Clone()), GetType());
}

void BoundDisjunction::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    temporary->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* temp = emitter.Stack().Pop();
    left->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftValue = emitter.Stack().Pop();
    void* trueBlock = emitter.CreateBasicBlock("true");
    void* rightBlock = emitter.CreateBasicBlock("right");
    void* falseBlock = emitter.CreateBasicBlock("false");
    void* nextBlock = emitter.CreateBasicBlock("next");
    emitter.CreateCondBr(leftValue, trueBlock, rightBlock);
    emitter.SetCurrentBasicBlock(rightBlock);
    right->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightValue = emitter.Stack().Pop();
    emitter.CreateCondBr(rightValue, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    emitter.CreateStore(emitter.CreateIrValueForBool(true), temp);
    emitter.CreateBr(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    emitter.CreateStore(emitter.CreateIrValueForBool(false), temp);
    emitter.CreateBr(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    void* value = emitter.CreateLoad(emitter.GetIrTypeForBool(), temp);
    emitter.Stack().Push(value);
    DestroyTemporaries(emitter);
}

void BoundDisjunction::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to disjunction", GetSourcePos(), ModuleId());
}

void BoundDisjunction::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundDisjunction::SetTemporary(BoundLocalVariable* temporary_)
{
    temporary.reset(temporary_);
}

bool BoundDisjunction::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (left->ContainsExceptionCapture())
    {
        return true;
    }
    if (right->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundConjunction::BoundConjunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, std::unique_ptr<BoundExpression>&& left_, std::unique_ptr<BoundExpression>&& right_, cmajor::symbols::TypeSymbol* boolType_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundConjunction, boolType_), left(std::move(left_)), right(std::move(right_))
{
}

BoundExpression* BoundConjunction::Clone()
{
    return new BoundConjunction(GetSourcePos(), ModuleId(), std::unique_ptr<BoundExpression>(left->Clone()), std::unique_ptr<BoundExpression>(right->Clone()), GetType());
}

void BoundConjunction::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    temporary->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* temp = emitter.Stack().Pop();
    left->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftValue = emitter.Stack().Pop();
    void* trueBlock = emitter.CreateBasicBlock("true");
    void* rightBlock = emitter.CreateBasicBlock("right");
    void* falseBlock = emitter.CreateBasicBlock("false");
    void* nextBlock = emitter.CreateBasicBlock("next");
    emitter.CreateCondBr(leftValue, rightBlock, falseBlock);
    emitter.SetCurrentBasicBlock(rightBlock);
    right->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightValue = emitter.Stack().Pop();
    emitter.CreateCondBr(rightValue, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    emitter.CreateStore(emitter.CreateIrValueForBool(true), temp);
    emitter.CreateBr(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    emitter.CreateStore(emitter.CreateIrValueForBool(false), temp);
    emitter.CreateBr(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    void* value = emitter.CreateLoad(emitter.GetIrTypeForBool(), temp);
    emitter.Stack().Push(value);
    DestroyTemporaries(emitter);
}

void BoundConjunction::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to conjunction", GetSourcePos(), ModuleId());
}

void BoundConjunction::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundConjunction::SetTemporary(BoundLocalVariable* temporary_)
{
    temporary.reset(temporary_);
}

bool BoundConjunction::ContainsExceptionCapture() const
{
    if (BoundExpression::ContainsExceptionCapture())
    {
        return true;
    }
    if (left->ContainsExceptionCapture())
    {
        return true;
    }
    if (right->ContainsExceptionCapture())
    {
        return true;
    }
    return false;
}

BoundTypeExpression::BoundTypeExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::TypeSymbol* type_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundTypeExpression, type_)
{
}

BoundExpression* BoundTypeExpression::Clone()
{
    return new BoundTypeExpression(GetSourcePos(), ModuleId(), GetType());
}

void BoundTypeExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from a type", GetSourcePos(), ModuleId());
}

void BoundTypeExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a type", GetSourcePos(), ModuleId());
}

void BoundTypeExpression::Accept(BoundNodeVisitor& visitor)
{
    throw cmajor::symbols::Exception("cannot visit a type", GetSourcePos(), ModuleId());
}

cmajor::symbols::TypeSymbol* CreateNamespaceTypeSymbol(cmajor::symbols::NamespaceSymbol* ns)
{
    cmajor::symbols::TypeSymbol* nsTypeSymbol = new NamespaceTypeSymbol(ns);
    ns->GetModule()->GetSymbolTable().SetTypeIdFor(nsTypeSymbol);
    return nsTypeSymbol;
}

BoundNamespaceExpression::BoundNamespaceExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::NamespaceSymbol* ns_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundNamespaceExpression, CreateNamespaceTypeSymbol(ns_)), ns(ns_)
{
    nsType.reset(GetType());
}

BoundExpression* BoundNamespaceExpression::Clone()
{
    return new BoundNamespaceExpression(GetSourcePos(), ModuleId(), ns);
}

void BoundNamespaceExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from a namespace", GetSourcePos(), ModuleId());
}

void BoundNamespaceExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a namespace", GetSourcePos(), ModuleId());
}

void BoundNamespaceExpression::Accept(BoundNodeVisitor& visitor)
{
    throw cmajor::symbols::Exception("cannot visit a namespace", GetSourcePos(), ModuleId());
}

cmajor::symbols::TypeSymbol* CreateFunctionGroupTypeSymbol(cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol, void* boundFunctionGroupExpression)
{
    cmajor::symbols::TypeSymbol* functionGroupTypeSymbol = new cmajor::symbols::FunctionGroupTypeSymbol(functionGroupSymbol, boundFunctionGroupExpression);
    functionGroupTypeSymbol->SetModule(functionGroupSymbol->GetModule());
    functionGroupSymbol->GetModule()->GetSymbolTable().SetTypeIdFor(functionGroupTypeSymbol);
    return functionGroupTypeSymbol;
}

BoundFunctionGroupExpression::BoundFunctionGroupExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundFunctionGroupExpression, CreateFunctionGroupTypeSymbol(functionGroupSymbol_, this)),
    functionGroupSymbol(functionGroupSymbol_), scopeQualified(false), qualifiedScope(nullptr)
{
    functionGroupType.reset(GetType());
}

BoundExpression* BoundFunctionGroupExpression::Clone()
{
    BoundFunctionGroupExpression* clone = new BoundFunctionGroupExpression(GetSourcePos(), ModuleId(), functionGroupSymbol);
    if (classPtr)
    {
        clone->classPtr.reset(classPtr->Clone());
    }
    clone->scopeQualified = scopeQualified;
    clone->qualifiedScope = qualifiedScope;
    return clone;
}

void BoundFunctionGroupExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (classPtr)
    {
        classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    else
    {
        emitter.Stack().Push(nullptr);
    }
}

void BoundFunctionGroupExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a function group", GetSourcePos(), ModuleId());
}

void BoundFunctionGroupExpression::Accept(BoundNodeVisitor& visitor)
{
    throw cmajor::symbols::Exception("cannot visit a function group", GetSourcePos(), ModuleId());
}

void BoundFunctionGroupExpression::SetClassPtr(std::unique_ptr<BoundExpression>&& classPtr_)
{
    classPtr = std::move(classPtr_);
}

void BoundFunctionGroupExpression::SetTemplateArgumentTypes(const std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes_)
{
    templateArgumentTypes = templateArgumentTypes_;
}

cmajor::symbols::TypeSymbol* CreateMemberExpressionTypeSymbol(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId_, const std::u32string& name, void* boundMemberExpression)
{
    cmajor::symbols::TypeSymbol* memberExpressionTypeSymbol = new cmajor::symbols::MemberExpressionTypeSymbol(sourcePos, moduleId_, name, boundMemberExpression);
    cmajor::symbols::Module* module = cmajor::symbols::GetRootModuleForCurrentThread();
    memberExpressionTypeSymbol->SetModule(module);
    module->GetSymbolTable().SetTypeIdFor(memberExpressionTypeSymbol);
    return memberExpressionTypeSymbol;
}

BoundMemberExpression::BoundMemberExpression(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, std::unique_ptr<BoundExpression>&& classPtr_, std::unique_ptr<BoundExpression>&& member_) :
    BoundExpression(sourcePos_, moduleId_, BoundNodeType::boundMemberExpression, CreateMemberExpressionTypeSymbol(sourcePos_, member_->ModuleId(), member_->GetType()->Name(), this)), classPtr(std::move(classPtr_)), member(std::move(member_))
{
    memberExpressionType.reset(GetType());
    classPtr->MoveTemporaryDestructorCallsTo(*this);
}

BoundExpression* BoundMemberExpression::Clone()
{
    return new BoundMemberExpression(GetSourcePos(), ModuleId(), std::unique_ptr<BoundExpression>(classPtr->Clone()), std::unique_ptr<BoundExpression>(member->Clone()));
}

void BoundMemberExpression::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    if (classPtr)
    {
        classPtr->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    else
    {
        emitter.Stack().Push(nullptr);
    }
}

void BoundMemberExpression::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to a member expression", GetSourcePos(), ModuleId());
}

void BoundMemberExpression::Accept(BoundNodeVisitor& visitor)
{
    throw cmajor::symbols::Exception("cannot visit a member expression", GetSourcePos(), ModuleId());
}

} // namespace cmajor::binder
