// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.array.type.symbol;

import cmajor.ast.reader;
import cmajor.ast.writer;
import soul.ast.span;
import cmajor.symbols.context;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.exception;
import cmajor.symbols.value;
import cmajor.symbols.modules;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import util;
import std.core;

namespace cmajor::symbols {

ArrayTypeSymbol::ArrayTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::arrayTypeSymbol, span_, name_), elementType(nullptr), size(-1)
{
}

ArrayTypeSymbol::ArrayTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_, TypeSymbol* elementType_, int64_t size_) :
    TypeSymbol(SymbolType::arrayTypeSymbol, span_, name_), elementType(elementType_), size(size_)
{
}

void ArrayTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    const util::uuid& elementTypeId = elementType->TypeId();
    writer.GetBinaryStreamWriter().Write(elementTypeId);
    writer.GetBinaryStreamWriter().Write(size);
}

void ArrayTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    util::uuid elementTypeId;
    reader.GetBinaryStreamReader().ReadUuid(elementTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, elementTypeId, 0);
    size = reader.GetBinaryStreamReader().ReadLong();
}

void ArrayTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 0)
    {
        elementType = typeSymbol;
    }
    else
    {
        throw Exception("internal error: invalid array emplace type index " + std::to_string(index), GetFullSpan());
    }
}

void* ArrayTypeSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    if (size == -1)
    {
        throw Exception("array '" + util::ToUtf8(FullName()) + "' size not defined", GetFullSpan());
    }
    void* localIrType = emitter.GetIrTypeByTypeId(TypeId());
    if (!localIrType)
    {
        localIrType = emitter.GetIrTypeForArrayType(elementType->IrType(emitter, context), size);
        emitter.SetIrTypeByTypeId(TypeId(), localIrType);
    }
    return localIrType;
}

void* ArrayTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context)
{
    if (size == -1)
    {
        throw Exception("array '" + util::ToUtf8(FullName()) + "' size not defined", GetFullSpan());
    }
    void* irType = IrType(emitter, context);
    std::vector<void*> arrayOfDefaults;
    for (int64_t i = 0; i < size; ++i)
    {
        arrayOfDefaults.push_back(elementType->CreateDefaultIrValue(emitter, context));
    }
    return emitter.CreateIrValueForConstantArray(irType, arrayOfDefaults, std::string());
}

void* ArrayTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    // todo...
    std::vector<void*> elements;
    return emitter.CreateDITypeForArray(elementType->GetDIType(emitter, context), elements);
}

ValueType ArrayTypeSymbol::GetValueType() const
{
    return ValueType::arrayValue;
}

Value* ArrayTypeSymbol::MakeValue() const
{
    std::vector<std::unique_ptr<Value>> elementValues;
    return new ArrayValue(GetSpan(), const_cast<TypeSymbol*>(static_cast<const TypeSymbol*>(this)), std::move(elementValues));
}

void ArrayTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!elementType)
    {
        throw SymbolCheckException("array type has no element type", GetFullSpan());
    }
}

ArrayLengthFunction::ArrayLengthFunction(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::arrayLengthFunctionSymbol, span_, name_), arrayType(nullptr)
{
}

ArrayLengthFunction::ArrayLengthFunction(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(SymbolType::arrayLengthFunctionSymbol, arrayType_->GetSpan(), U"Length"), arrayType(arrayType_)
{
    SetGroupName(U"Length");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    TypeSymbol* longType = context->RootModule()->GetSymbolTable().GetTypeByName(U"long");
    SetReturnType(longType);
    ComputeName(context);
}

void ArrayLengthFunction::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(arrayType->TypeId());
}

void ArrayLengthFunction::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ArrayLengthFunction::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::arrayTypeSymbol, "array type expected");
        arrayType = static_cast<ArrayTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ArrayLengthFunction::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "array length needs one object");
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    emitter.Stack().Push(size);
}

std::unique_ptr<Value> ArrayLengthFunction::ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, 
    const soul::ast::Span& span, Value* receiver) const
{
    return std::unique_ptr<Value>(new LongValue(span, arrayType->Size()));
}

void ArrayLengthFunction::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array length function has no array type", GetFullSpan());
    }
}

ArrayBeginFunction::ArrayBeginFunction(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::arrayBeginFunctionSymbol, span_, name_), arrayType(nullptr)
{
}

ArrayBeginFunction::ArrayBeginFunction(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(SymbolType::arrayBeginFunctionSymbol, arrayType_->GetSpan(), U"@arrayBegin"), arrayType(arrayType_)
{
    SetGroupName(U"Begin");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    TypeSymbol* returnType = arrayType->ElementType()->AddPointer(context);
    SetReturnType(returnType);
    ComputeName(context);
}

void ArrayBeginFunction::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(arrayType->TypeId());
}

void ArrayBeginFunction::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ArrayBeginFunction::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::arrayTypeSymbol, "array type expected");
        arrayType = static_cast<ArrayTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ArrayBeginFunction::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "array begin needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* arrayPtr = emitter.Stack().Pop();
    void* type = arrayType->IrType(emitter, context);
    void* beginPtr = emitter.GetArrayBeginAddress(type, arrayPtr);
    emitter.Stack().Push(beginPtr);
}

void ArrayBeginFunction::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array begin function has no array type", GetFullSpan());
    }
}

ArrayEndFunction::ArrayEndFunction(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::arrayEndFunctionSymbol, span_, name_), arrayType(nullptr)
{
}

ArrayEndFunction::ArrayEndFunction(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(SymbolType::arrayEndFunctionSymbol, arrayType_->GetSpan(), U"@arrayEnd"), arrayType(arrayType_)
{
    SetGroupName(U"End");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    TypeSymbol* returnType = arrayType->ElementType()->AddPointer(context);
    SetReturnType(returnType);
    ComputeName(context);
}

void ArrayEndFunction::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(arrayType->TypeId());
}

void ArrayEndFunction::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ArrayEndFunction::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::arrayTypeSymbol, "array type expected");
        arrayType = static_cast<ArrayTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ArrayEndFunction::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "array end needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* arrayIrType = arrayType->IrType(emitter, context);
    void* arrayPtr = emitter.Stack().Pop();
    void* endPtr = emitter.GetArrayEndAddress(arrayIrType, arrayPtr, arrayType->Size());
    emitter.Stack().Push(endPtr);
}

void ArrayEndFunction::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array end function has no array type", GetFullSpan());
    }
}

ArrayCBeginFunction::ArrayCBeginFunction(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::arrayCBeginFunctionSymbol, span_, name_), arrayType(nullptr)
{
}

ArrayCBeginFunction::ArrayCBeginFunction(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(SymbolType::arrayCBeginFunctionSymbol, arrayType_->GetSpan(), U"@arrayCBegin"), arrayType(arrayType_)
{
    SetGroupName(U"CBegin");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    TypeSymbol* returnType = arrayType->ElementType()->AddConst(context)->AddPointer(context);
    SetReturnType(returnType);
    ComputeName(context);
}

void ArrayCBeginFunction::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(arrayType->TypeId());
}

void ArrayCBeginFunction::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ArrayCBeginFunction::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::arrayTypeSymbol, "array type expected");
        arrayType = static_cast<ArrayTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ArrayCBeginFunction::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "array cbegin needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* arrayPtr = emitter.Stack().Pop();
    void* type = arrayType->IrType(emitter, context);
    void* beginPtr = emitter.GetArrayBeginAddress(type, arrayPtr);
    emitter.Stack().Push(beginPtr);
}

void ArrayCBeginFunction::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array cbegin function has no array type", GetFullSpan());
    }
}

ArrayCEndFunction::ArrayCEndFunction(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::arrayCEndFunctionSymbol, span_, name_), arrayType(nullptr)
{
}

ArrayCEndFunction::ArrayCEndFunction(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(SymbolType::arrayCEndFunctionSymbol, arrayType_->GetSpan(), U"@arrayCEnd"), arrayType(arrayType_)
{
    SetGroupName(U"CEnd");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    TypeSymbol* returnType = arrayType->ElementType()->AddConst(context)->AddPointer(context);
    SetReturnType(returnType);
    ComputeName(context);
}

void ArrayCEndFunction::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(arrayType->TypeId());
}

void ArrayCEndFunction::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ArrayCEndFunction::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::arrayTypeSymbol, "array type expected");
        arrayType = static_cast<ArrayTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ArrayCEndFunction::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "array cend needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* arrayIrType = arrayType->IrType(emitter, context);
    void* arrayPtr = emitter.Stack().Pop();
    void* endPtr = emitter.GetArrayEndAddress(arrayIrType, arrayPtr, arrayType->Size());
    emitter.Stack().Push(endPtr);
}

void ArrayCEndFunction::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array cend function has no array type", GetFullSpan());
    }
}

ArrayTypeDefaultConstructor::ArrayTypeDefaultConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeDefaultConstructor_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayDefaultCtor"), arrayType(arrayType_), elementTypeDefaultConstructor(elementTypeDefaultConstructor_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(arrayType_->GetSpan(), U"this");
    thisParam->SetType(arrayType->AddPointer(context));
    AddMember(thisParam, context);
    ComputeName(context);
}

std::vector<LocalVariableSymbol*> ArrayTypeDefaultConstructor::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(GetModule()->GetSymbolTable().GetTypeByName(U"long"), GetSpan(), context));
    return temporaries;
}

void ArrayTypeDefaultConstructor::SetTemporariesForElementTypeDefaultCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries)
{
    temporariesForElementTypeDefaultCtor = std::move(temporaries);
}

void ArrayTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    Assert(genObjects.size() == 2, "array type default constructor needs two objects: one array type object and one loop variable temporary");
    emitter.Stack().Push(emitter.CreateIrValueForLong(0));
    cmajor::ir::GenObject* loopVar = genObjects[1];
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    void* loop = emitter.CreateBasicBlock("loop");
    void* init = emitter.CreateBasicBlock("init");
    void* next = emitter.CreateBasicBlock("next");
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(loop);
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index = emitter.Stack().Pop();
    void* less = emitter.CreateICmpULT(index, size);
    emitter.CreateCondBr(less, init, next);
    emitter.SetCurrentBasicBlock(init);
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index2 = emitter.Stack().Pop();
    void* elementType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elementType, index2);
    cmajor::ir::NativeValue elementPtrValue(elementPtr);
    std::vector<cmajor::ir::GenObject*> elementGenObjects;
    elementGenObjects.push_back(&elementPtrValue);
    for (const std::unique_ptr<cmajor::ir::GenObject>& temp : temporariesForElementTypeDefaultCtor)
    {
        elementGenObjects.push_back(temp.get());
    }
    elementTypeDefaultConstructor->GenerateCall(emitter, elementGenObjects, cmajor::ir::OperationFlags::none, context);
    void* nextI = emitter.CreateAdd(index2, emitter.CreateIrValueForLong(1));
    emitter.Stack().Push(nextI);
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(next);
}

void ArrayTypeDefaultConstructor::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array default constructor has no array type", GetFullSpan());
    }
    if (!elementTypeDefaultConstructor)
    {
        throw SymbolCheckException("array default constructor has no element type default constructor", GetFullSpan());
    }
}

ArrayTypeCopyConstructor::ArrayTypeCopyConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeCopyConstructor_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayCopyCtor"), arrayType(arrayType_), elementTypeCopyConstructor(elementTypeCopyConstructor_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(arrayType->GetSpan(), U"this");
    thisParam->SetType(arrayType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(arrayType->GetSpan(), U"that");
    thatParam->SetType(arrayType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

std::vector<LocalVariableSymbol*> ArrayTypeCopyConstructor::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(GetModule()->GetSymbolTable().GetTypeByName(U"long"), GetSpan(), context));
    return temporaries;
}

void ArrayTypeCopyConstructor::SetTemporariesForElementTypeCopyCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries)
{
    temporariesForElementTypeCopyConstructor = std::move(temporaries);
}

void ArrayTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 3, "copy constructor needs three objects: two array type objects and one loop variable temporary");
    emitter.Stack().Push(emitter.CreateIrValueForLong(0));
    cmajor::ir::GenObject* loopVar = genObjects[2];
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    void* loop = emitter.CreateBasicBlock("loop");
    void* init = emitter.CreateBasicBlock("init");
    void* next = emitter.CreateBasicBlock("next");
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(loop);
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index = emitter.Stack().Pop();
    void* less = emitter.CreateICmpULT(index, size);
    emitter.CreateCondBr(less, init, next);
    emitter.SetCurrentBasicBlock(init);
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* sourcePtr = emitter.Stack().Pop();
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index2 = emitter.Stack().Pop();
    void* elemType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elemType, index2);
    cmajor::ir::NativeValue elementPtrValue(elementPtr);
    std::vector<cmajor::ir::GenObject*> elementGenObjects;
    elementGenObjects.push_back(&elementPtrValue);
    void* sourceElementPtr = emitter.CreateArrayIndexAddress(type, sourcePtr, elemType, index2);
    void* sourceElementValue = sourceElementPtr;
    TypeSymbol* elementType = arrayType->ElementType();
    if (elementType->IsBasicTypeSymbol() || elementType->IsPointerType() || elementType->GetSymbolType() == SymbolType::delegateTypeSymbol)
    {
        sourceElementValue = emitter.CreateLoad(elementType->IrType(emitter, context), sourceElementPtr); // TODO
    }
    cmajor::ir::NativeValue sourceValue(sourceElementValue);
    elementGenObjects.push_back(&sourceValue);
    for (const std::unique_ptr<cmajor::ir::GenObject>& temp : temporariesForElementTypeCopyConstructor)
    {
        elementGenObjects.push_back(temp.get());
    }
    elementTypeCopyConstructor->GenerateCall(emitter, elementGenObjects, cmajor::ir::OperationFlags::none, context);
    void* nextI = emitter.CreateAdd(index2, emitter.CreateIrValueForLong(1));
    emitter.Stack().Push(nextI);
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(next);
}

void ArrayTypeCopyConstructor::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array copy constructor has no array type", GetFullSpan());
    }
    if (!elementTypeCopyConstructor)
    {
        throw SymbolCheckException("array copy constructor has no element type copy constructor", GetFullSpan());
    }
}

ArrayTypeMoveConstructor::ArrayTypeMoveConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeMoveConstructor_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayMoveCtor"), arrayType(arrayType_), elementTypeMoveConstructor(elementTypeMoveConstructor_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(arrayType->GetSpan(), U"this");
    thisParam->SetType(arrayType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(arrayType->GetSpan(), U"that");
    thatParam->SetType(arrayType->AddRvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

std::vector<LocalVariableSymbol*> ArrayTypeMoveConstructor::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(GetModule()->GetSymbolTable().GetTypeByName(U"long"), GetSpan(), context));
    return temporaries;
}

void ArrayTypeMoveConstructor::SetTemporariesForElementTypeMoveCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries)
{
    temporariesForElementTypeMoveConstructor = std::move(temporaries);
}

void ArrayTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 3, "move constructor needs three objects: two array type objects and one loop variable temporary");
    emitter.Stack().Push(emitter.CreateIrValueForLong(0));
    cmajor::ir::GenObject* loopVar = genObjects[2];
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    void* loop = emitter.CreateBasicBlock("loop");
    void* init = emitter.CreateBasicBlock("init");
    void* next = emitter.CreateBasicBlock("next");
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(loop);
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index = emitter.Stack().Pop();
    void* less = emitter.CreateICmpULT(index, size);
    emitter.CreateCondBr(less, init, next);
    emitter.SetCurrentBasicBlock(init);
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* sourcePtr = emitter.Stack().Pop();
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index2 = emitter.Stack().Pop();
    void* elemType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elemType, index2);
    cmajor::ir::NativeValue elementPtrValue(elementPtr);
    std::vector<cmajor::ir::GenObject*> elementGenObjects;
    elementGenObjects.push_back(&elementPtrValue);
    void* sourceElementPtr = emitter.CreateArrayIndexAddress(type, sourcePtr, elemType, index2);
    cmajor::ir::NativeValue sourcePtrValue(sourceElementPtr);
    elementGenObjects.push_back(&sourcePtrValue);
    for (const std::unique_ptr<cmajor::ir::GenObject>& temp : temporariesForElementTypeMoveConstructor)
    {
        elementGenObjects.push_back(temp.get());
    }
    elementTypeMoveConstructor->GenerateCall(emitter, elementGenObjects, cmajor::ir::OperationFlags::none, context);
    void* nextI = emitter.CreateAdd(index2, emitter.CreateIrValueForLong(1));
    emitter.Stack().Push(nextI);
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(next);
}

void ArrayTypeMoveConstructor::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array move constructor has no array type", GetFullSpan());
    }
    if (!elementTypeMoveConstructor)
    {
        throw SymbolCheckException("array move constructor has no element type move constructor", GetFullSpan());
    }
}

ArrayTypeCopyAssignment::ArrayTypeCopyAssignment(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeCopyAssignment_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayCopyAssignment"), arrayType(arrayType_), elementTypeCopyAssignment(elementTypeCopyAssignment_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(arrayType->GetSpan(), U"this");
    thisParam->SetType(arrayType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(arrayType->GetSpan(), U"that");
    thatParam->SetType(arrayType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    TypeSymbol* voidType = context->RootModule()->GetSymbolTable().GetTypeByName(U"void");
    SetReturnType(voidType);
    ComputeName(context);
}

std::vector<LocalVariableSymbol*> ArrayTypeCopyAssignment::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(GetModule()->GetSymbolTable().GetTypeByName(U"long"), GetSpan(), context));
    return temporaries;
}

void ArrayTypeCopyAssignment::SetTemporariesForElementTypeCopyAssignment(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries)
{
    temporariesForElementTypeCopyAssignment = std::move(temporaries);
}

void ArrayTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 3, "copy assignment needs three objects: two array type objects and one loop variable temporary");
    emitter.Stack().Push(emitter.CreateIrValueForLong(0));
    cmajor::ir::GenObject* loopVar = genObjects[2];
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    void* loop = emitter.CreateBasicBlock("loop");
    void* init = emitter.CreateBasicBlock("init");
    void* next = emitter.CreateBasicBlock("next");
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(loop);
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index = emitter.Stack().Pop();
    void* less = emitter.CreateICmpULT(index, size);
    emitter.CreateCondBr(less, init, next);
    emitter.SetCurrentBasicBlock(init);
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* sourcePtr = emitter.Stack().Pop();
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index2 = emitter.Stack().Pop();
    void* elemType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elemType, index2);
    cmajor::ir::NativeValue elementPtrValue(elementPtr);
    std::vector<cmajor::ir::GenObject*> elementGenObjects;
    elementGenObjects.push_back(&elementPtrValue);
    void* sourceElementPtr = emitter.CreateArrayIndexAddress(type, sourcePtr, elemType, index2);
    void* sourceElementValue = sourceElementPtr;
    TypeSymbol* elementType = arrayType->ElementType();
    if (elementType->IsBasicTypeSymbol() || elementType->IsPointerType() || elementType->GetSymbolType() == SymbolType::delegateTypeSymbol)
    {
        sourceElementValue = emitter.CreateLoad(elementType->IrType(emitter, context), sourceElementPtr); // TODO
    }
    cmajor::ir::NativeValue sourceValue(sourceElementValue);
    elementGenObjects.push_back(&sourceValue);
    for (const std::unique_ptr<cmajor::ir::GenObject>& temp : temporariesForElementTypeCopyAssignment)
    {
        elementGenObjects.push_back(temp.get());
    }
    elementTypeCopyAssignment->GenerateCall(emitter, elementGenObjects, cmajor::ir::OperationFlags::none, context);
    void* nextI = emitter.CreateAdd(index2, emitter.CreateIrValueForLong(1));
    emitter.Stack().Push(nextI);
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(next);
}

void ArrayTypeCopyAssignment::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array copy assignment has no array type", GetFullSpan());
    }
    if (!elementTypeCopyAssignment)
    {
        throw SymbolCheckException("array copy assignmet has no element type copy assignment", GetFullSpan());
    }
}

ArrayTypeMoveAssignment::ArrayTypeMoveAssignment(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeMoveAssignment_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayMoveAssignment"), arrayType(arrayType_), elementTypeMoveAssignment(elementTypeMoveAssignment_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(arrayType->GetSpan(), U"this");
    thisParam->SetType(arrayType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(arrayType->GetSpan(), U"that");
    thatParam->SetType(arrayType->AddRvalueReference(context));
    AddMember(thatParam, context);
    TypeSymbol* voidType = context->RootModule()->GetSymbolTable().GetTypeByName(U"void");
    SetReturnType(voidType);
    ComputeName(context);
}

std::vector<LocalVariableSymbol*> ArrayTypeMoveAssignment::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(GetModule()->GetSymbolTable().GetTypeByName(U"long"), GetSpan(), context));
    return temporaries;
}

void ArrayTypeMoveAssignment::SetTemporariesForElementTypeMoveAssignment(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries)
{
    temporariesForElementTypeMoveAssignment = std::move(temporaries);
}

void ArrayTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 3, "move assignment needs three objects: two array type objects and one loop variable temporary");
    emitter.Stack().Push(emitter.CreateIrValueForLong(0));
    cmajor::ir::GenObject* loopVar = genObjects[2];
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    void* size = emitter.CreateIrValueForLong(arrayType->Size());
    void* loop = emitter.CreateBasicBlock("loop");
    void* init = emitter.CreateBasicBlock("init");
    void* next = emitter.CreateBasicBlock("next");
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(loop);
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index = emitter.Stack().Pop();
    void* less = emitter.CreateICmpULT(index, size);
    emitter.CreateCondBr(less, init, next);
    emitter.SetCurrentBasicBlock(init);
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptrPtr = emitter.Stack().Pop();
    void* ptr = emitter.CreateLoad(arrayType->AddPointer(context)->IrType(emitter, context), ptrPtr); // TODO
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* sourcePtr = emitter.Stack().Pop();
    loopVar->Load(emitter, cmajor::ir::OperationFlags::none);
    void* index2 = emitter.Stack().Pop();
    void* elemType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elemType, index2);
    cmajor::ir::NativeValue elementPtrValue(elementPtr);
    std::vector<cmajor::ir::GenObject*> elementGenObjects;
    elementGenObjects.push_back(&elementPtrValue);
    void* sourceElementPtr = emitter.CreateArrayIndexAddress(type, sourcePtr, elemType, index2);
    TypeSymbol* elementType = arrayType->ElementType();
    cmajor::ir::NativeValue sourcePtrValue(sourceElementPtr);
    elementGenObjects.push_back(&sourcePtrValue);
    for (const std::unique_ptr<cmajor::ir::GenObject>& temp : temporariesForElementTypeMoveAssignment)
    {
        elementGenObjects.push_back(temp.get());
    }
    elementTypeMoveAssignment->GenerateCall(emitter, elementGenObjects, cmajor::ir::OperationFlags::none, context);
    void* nextI = emitter.CreateAdd(index2, emitter.CreateIrValueForLong(1));
    emitter.Stack().Push(nextI);
    loopVar->Store(emitter, cmajor::ir::OperationFlags::none);
    emitter.CreateBr(loop);
    emitter.SetCurrentBasicBlock(next);
}

void ArrayTypeMoveAssignment::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array move assignment has no array type", GetFullSpan());
    }
    if (!elementTypeMoveAssignment)
    {
        throw SymbolCheckException("array move assignmet has no element type move assignment", GetFullSpan());
    }
}

ArrayTypeElementAccess::ArrayTypeElementAccess(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(arrayType_->GetSpan(), U"@arrayElementAccess"), arrayType(arrayType_)
{
    SetGroupName(U"operator[]");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* arrayParam = new ParameterSymbol(arrayType->GetSpan(), U"array");
    arrayParam->SetType(arrayType);
    AddMember(arrayParam, context);
    ParameterSymbol* indexParam = new ParameterSymbol(arrayType->GetSpan(), U"index");
    indexParam->SetType(context->RootModule()->GetSymbolTable().GetTypeByName(U"long"));
    AddMember(indexParam, context);
    TypeSymbol* returnType = arrayType->ElementType();
    if (!returnType->IsBasicTypeSymbol() && !returnType->IsPointerType() && returnType->GetSymbolType() != SymbolType::delegateTypeSymbol)
    {
        returnType = returnType->AddLvalueReference(context);
    }
    SetReturnType(returnType);
    ComputeName(context);
}

void ArrayTypeElementAccess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 2, "element access needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* indexValue = emitter.Stack().Pop();
    void* elemType = arrayType->ElementType()->IrType(emitter, context);
    void* type = arrayType->IrType(emitter, context);
    void* elementPtr = emitter.CreateArrayIndexAddress(type, ptr, elemType, indexValue);
    TypeSymbol* elementType = arrayType->ElementType();
    if ((flags & cmajor::ir::OperationFlags::addr) == cmajor::ir::OperationFlags::none && (elementType->IsBasicTypeSymbol() || 
        elementType->IsPointerType() || elementType->GetSymbolType() == SymbolType::delegateTypeSymbol))
    {
        void* elementValue = emitter.CreateLoad(elemType, elementPtr); // TODO
        emitter.Stack().Push(elementValue);
    }
    else
    {
        emitter.Stack().Push(elementPtr);
    }
}

void ArrayTypeElementAccess::Check()
{
    FunctionSymbol::Check();
    if (!arrayType)
    {
        throw SymbolCheckException("array type element access has no array type", GetFullSpan());
    }
}

} // namespace cmajor::symbols
