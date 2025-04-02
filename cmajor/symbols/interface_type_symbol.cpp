// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.interfaces;

import cmajor.symbols.context;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.exception;
import cmajor.symbols.global.flags;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.modules;
import cmajor.ir.emitter;

namespace cmajor::symbols {

InterfaceTypeSymbol::InterfaceTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::interfaceTypeSymbol, span_, name_), 
    defaultConstructor(nullptr), copyConstructor(nullptr), moveConstructor(nullptr), copyAssignment(nullptr), moveAssignment(nullptr)
{
}

void InterfaceTypeSymbol::AddMember(Symbol* member, Context* context)
{
    TypeSymbol::AddMember(member, context);
    switch (member->GetSymbolType())
    {
        case SymbolType::interfaceTypeDefaultCtor:
        {
            defaultConstructor = static_cast<InterfaceTypeDefaultConstructor*>(member);
            break;
        }
        case SymbolType::interfaceTypeCopyCtor:
        {
            copyConstructor = static_cast<InterfaceTypeCopyConstructor*>(member);
            break;
        }
        case SymbolType::interfaceTypeMoveCtor:
        {
            moveConstructor = static_cast<InterfaceTypeMoveConstructor*>(member);
            break;
        }
        case SymbolType::interfaceTypeCopyAssignment:
        {
            copyAssignment = static_cast<InterfaceTypeCopyAssignment*>(member);
            break;
        }
        case SymbolType::interfaceTypeMoveAssignment:
        {
            moveAssignment = static_cast<InterfaceTypeMoveAssignment*>(member);
            break;
        }
        case SymbolType::memberFunctionSymbol:
        {
            MemberFunctionSymbol* memberFunction = static_cast<MemberFunctionSymbol*>(member);
            memberFunction->SetImtIndex(memberFunctions.size());
            memberFunctions.push_back(memberFunction);
            break;
        }
    }
}

void InterfaceTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddInterface(this);
    }
}

void InterfaceTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be nothrow", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be throw", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("interface cannot be unit_test", GetFullSpan());
    }
}

void* InterfaceTypeSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localIrType = emitter.GetIrTypeByTypeId(TypeId());
    if (!localIrType)
    {
        std::vector<void*> elemTypes;
        elemTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        elemTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        localIrType = emitter.GetIrTypeForStructType(elemTypes);
        emitter.SetIrTypeByTypeId(TypeId(), localIrType);
    }
    return localIrType;
}

void* InterfaceTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context)
{
    void* irType = IrType(emitter, context);
    std::vector<void*> arrayOfDefaults;
    arrayOfDefaults.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
    arrayOfDefaults.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
    return emitter.CreateDefaultIrValueForStruct(irType, arrayOfDefaults);
}

void InterfaceTypeSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    MemberFunctionSymbol* interfaceMemberFunction, Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectFromInterface(IrType(emitter, context), interfaceTypePtr);
    void* imtPtr = emitter.GetImtPtrFromInterface(IrType(emitter, context), interfaceTypePtr);
    void* callee = emitter.GetInterfaceMethod(IrType(emitter, context), imtPtr, interfaceMemberFunction->ImtIndex(), interfaceMemberFunction->IrType(emitter, context));
    int na = genObjects.size();
    for (int i = 1; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        genObject->Load(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
    }
    std::vector<void*> args;
    int n = interfaceMemberFunction->Parameters().size();
    if (interfaceMemberFunction->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        ++n;
    }
    args.resize(n);
    args[0] = objectPtr;
    for (int i = 0; i < n - 1; ++i)
    {
        void* arg = emitter.Stack().Pop();
        args[n - i - 1] = arg;
    }
    void* handlerBlock = emitter.HandlerBlock();
    void* cleanupBlock = emitter.CleanupBlock();
    bool newCleanupNeeded = emitter.NewCleanupNeeded();
    cmajor::ir::Pad* currentPad = emitter.CurrentPad();
    std::vector<void*> bundles;
    if (currentPad != nullptr)
    {
        bundles.push_back(currentPad->value);
    }
    if (interfaceMemberFunction->ReturnType()->GetSymbolType() != SymbolType::voidTypeSymbol && !interfaceMemberFunction->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        if (IsNothrow() || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateCall(functionType, callee, args));
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                soul::ast::FullSpan fullSpan = interfaceMemberFunction->GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen));
            }
        }
        else
        {
            void* nextBlock = nullptr;
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                nextBlock = emitter.CreateBasicBlock("next");
            }
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block");
            }
            if (currentPad == nullptr)
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args));
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                soul::ast::FullSpan fullSpan = interfaceMemberFunction->GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen));
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
    else
    {
        if (IsNothrow() || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                emitter.CreateCall(functionType, callee, args);
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                soul::ast::FullSpan fullSpan = interfaceMemberFunction->GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen);
            }
        }
        else
        {
            void* nextBlock = emitter.CreateBasicBlock("next");
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block"); 
            }
            if (currentPad == nullptr)
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args);
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter, context);
                soul::ast::FullSpan fullSpan = interfaceMemberFunction->GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen);
            }
            emitter.SetCurrentBasicBlock(nextBlock);
        }
    }
}

void InterfaceTypeSymbol::Check()
{
    TypeSymbol::Check();
    for (MemberFunctionSymbol* memberFunction : memberFunctions)
    {
        if (!memberFunction)
        {
            throw SymbolCheckException("interface type symbol contains null member function", GetFullSpan());
        }
    }
}

InterfaceTypeDefaultConstructor::InterfaceTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeDefaultCtor, span_, U"@interfaceDefaultCtor"), interfaceType(nullptr)
{
}

InterfaceTypeDefaultConstructor::InterfaceTypeDefaultConstructor(InterfaceTypeSymbol* interfaceType_, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeDefaultCtor, interfaceType_->GetSpan(), U"@constructor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    ComputeName(context);
}

void InterfaceTypeDefaultConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeDefaultConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeDefaultConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected"); 
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    emitter.CreateStore(emitter.CreateDefaultIrValueForVoidPtrType(), objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    emitter.CreateStore(emitter.CreateDefaultIrValueForVoidPtrType(), interfacePtrPtr);
}

InterfaceTypeCopyConstructor::InterfaceTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyCtor, span_, U"@interfaceCopyCtor"), interfaceType(nullptr)
{
}

InterfaceTypeCopyConstructor::InterfaceTypeCopyConstructor(InterfaceTypeSymbol* interfaceType_, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeCopyCtor, interfaceType_->GetSpan(), U"@interfaceCopyCtor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void InterfaceTypeCopyConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeCopyConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeCopyConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected"); 
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeMoveConstructor::InterfaceTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveCtor, span_, U"@interfaceMoveCtor"), interfaceType(nullptr)
{
}

InterfaceTypeMoveConstructor::InterfaceTypeMoveConstructor(InterfaceTypeSymbol* interfaceType_, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeMoveCtor, interfaceType_->GetSpan(), U"@interfaceMoveCtor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst(context)->AddRvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void InterfaceTypeMoveConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeMoveConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeMoveConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected");
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeCopyAssignment::InterfaceTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyAssignment, span_, U"@interfaceCopyAssignment"), interfaceType(nullptr)
{
}

InterfaceTypeCopyAssignment::InterfaceTypeCopyAssignment(InterfaceTypeSymbol* interfaceType_, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeCopyAssignment, interfaceType_->GetSpan(), U"@interfaceCopyAssignment"), interfaceType(interfaceType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void InterfaceTypeCopyAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeCopyAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeCopyAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected");
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeMoveAssignment::InterfaceTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveAssignment, span_, U"@interfaceMoveAssignment"), interfaceType(nullptr)
{
}

InterfaceTypeMoveAssignment::InterfaceTypeMoveAssignment(InterfaceTypeSymbol* interfaceType_, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeMoveAssignment, interfaceType_->GetSpan(), U"@interfaceMoveAssignment"), 
    interfaceType(interfaceType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void InterfaceTypeMoveAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeMoveAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeMoveAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected");
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeEqual::InterfaceTypeEqual(const soul::ast::Span& span_, const std::u32string& name_) : 
    FunctionSymbol(SymbolType::interfaceTypeEqual, span_, U"@interfaceEqual"), interfaceType(nullptr)
{
}

InterfaceTypeEqual::InterfaceTypeEqual(InterfaceTypeSymbol* interfaceType_, TypeSymbol* boolType, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeEqual, interfaceType_->GetSpan(), U"@interfaceEqual"),
    interfaceType(interfaceType_)
{
    SetGroupName(U"operator==");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(interfaceType->GetSpan(), U"left");
    leftParam->SetType(interfaceType->AddConst(context)->AddLvalueReference(context));
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(interfaceType->GetSpan(), U"right");
    rightParam->SetType(interfaceType->AddConst(context)->AddLvalueReference(context));
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void InterfaceTypeEqual::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeEqual::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeEqual::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected");
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeEqual::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftPtr = emitter.Stack().Pop();
    void* leftObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), leftPtr);
    void* leftObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), leftObjectPtr); // TODO
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightPtr = emitter.Stack().Pop();
    void* rightObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), rightPtr);
    void* rightObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), rightObjectPtr); // TODO
    void* objectsEqual = emitter.CreateICmpEQ(leftObjectValue, rightObjectValue);
    void* interfaceIrType = interfaceType->IrType(emitter, context);
    void* leftInterfacePtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), leftPtr);
    void* leftInterfaceValue = emitter.CreateLoad(interfaceIrType, leftInterfacePtr); // TODO
    void* rightInterfacePtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), rightPtr);
    void* rightIntefaceValue = emitter.CreateLoad(interfaceIrType, rightInterfacePtr); // TODO
    void* interfacesEqual = emitter.CreateICmpEQ(leftInterfaceValue, rightIntefaceValue);
    void* equal = emitter.CreateAnd(objectsEqual, interfacesEqual);
    emitter.Stack().Push(equal);
}

InterfaceTypeLess::InterfaceTypeLess(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeLess, span_, U"@interfaceTypeLess"), interfaceType(nullptr)
{
}

InterfaceTypeLess::InterfaceTypeLess(InterfaceTypeSymbol* interfaceType_, TypeSymbol* boolType, Context* context) :
    FunctionSymbol(SymbolType::interfaceTypeLess, interfaceType_->GetSpan(), U"@interfaceTypeLess"),
    interfaceType(interfaceType_)
{
    SetGroupName(U"operator<");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(interfaceType->GetSpan(), U"left");
    leftParam->SetType(interfaceType->AddConst(context)->AddLvalueReference(context));
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(interfaceType->GetSpan(), U"right");
    rightParam->SetType(interfaceType->AddConst(context)->AddLvalueReference(context));
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void InterfaceTypeLess::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void InterfaceTypeLess::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void InterfaceTypeLess::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected");
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void InterfaceTypeLess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    void* retVal = emitter.CreateAlloca(emitter.GetIrTypeForBool());
    void* retBlock = emitter.CreateBasicBlock("ret");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftPtr = emitter.Stack().Pop();
    void* leftObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), leftPtr);
    void* leftObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), leftObjectPtr); // TODO
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightPtr = emitter.Stack().Pop();
    void* rightObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), rightPtr);
    void* rightObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), rightObjectPtr); // TODO
    void* leftObjectLessThanRightObject = emitter.CreateICmpULT(leftObjectValue, rightObjectValue);
    void* leftLessBlock = emitter.CreateBasicBlock("leftLess");
    void* leftNotLessBlock = emitter.CreateBasicBlock("leftNotLess");
    emitter.CreateCondBr(leftObjectLessThanRightObject, leftLessBlock, leftNotLessBlock);
    emitter.SetCurrentBasicBlock(leftLessBlock);
    emitter.CreateStore(emitter.CreateTrue(), retVal);
    emitter.CreateBr(retBlock);
    emitter.SetCurrentBasicBlock(leftNotLessBlock);
    void* rightObjectLessThanLeftObject = emitter.CreateICmpULT(rightObjectValue, leftObjectValue);
    void* rightLessBlock = emitter.CreateBasicBlock("rightLess");
    void* rightNotLessBlock = emitter.CreateBasicBlock("rightNotLess");
    emitter.CreateCondBr(rightObjectLessThanLeftObject, rightLessBlock, rightNotLessBlock);
    emitter.SetCurrentBasicBlock(rightLessBlock);
    emitter.CreateStore(emitter.CreateFalse(), retVal);
    emitter.CreateBr(retBlock);
    emitter.SetCurrentBasicBlock(rightNotLessBlock);
    void* interfaceIrType = interfaceType->IrType(emitter, context);
    void* leftInterfacePtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), leftPtr);
    void* leftInterfaceValue = emitter.CreateLoad(interfaceIrType, leftInterfacePtr); // TODO
    void* rightInterfacePtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter, context), rightPtr);
    void* rightIntefaceValue = emitter.CreateLoad(interfaceIrType, rightInterfacePtr); // TODO
    void* interfaceLess = emitter.CreateICmpULT(leftInterfaceValue, rightIntefaceValue);
    emitter.CreateStore(interfaceLess, retVal);
    emitter.CreateBr(retBlock);
    emitter.SetCurrentBasicBlock(retBlock);
    void* rv = emitter.CreateLoad(emitter.GetIrTypeForBool(), retVal);
    emitter.Stack().Push(rv);
}

ClassToInterfaceConversion::ClassToInterfaceConversion(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classToInterfaceConversion, span_, U"@classToInterfaceConversion"), sourceClassType(nullptr), targetInterfaceType(nullptr), interfaceIndex(0)
{
}

ClassToInterfaceConversion::ClassToInterfaceConversion(ClassTypeSymbol* sourceClassType_, InterfaceTypeSymbol* targetInterfaceType_, int32_t interfaceIndex_,
    const soul::ast::Span& span_, Context* context) :
    FunctionSymbol(SymbolType::classToInterfaceConversion, span_, U"@classToInterfaceConversion"), sourceClassType(sourceClassType_),
    targetInterfaceType(targetInterfaceType_), interfaceIndex(interfaceIndex_)
{
    SetConversion();
    SetConversionSourceType(sourceClassType->PlainType(context));
    SetConversionTargetType(targetInterfaceType->PlainType(context));
}

void ClassToInterfaceConversion::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(sourceClassType->TypeId());
    writer.GetBinaryStreamWriter().Write(targetInterfaceType->TypeId());
    writer.GetBinaryStreamWriter().Write(interfaceIndex);
}

void ClassToInterfaceConversion::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 2);
}

void ClassToInterfaceConversion::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classTypeSymbol, "class type expected"); 
        sourceClassType = static_cast<ClassTypeSymbol*>(typeSymbol);
    }
    else if (index == 2)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected"); 
        targetInterfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

std::vector<LocalVariableSymbol*> ClassToInterfaceConversion::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context, bool add)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(targetInterfaceType, GetSpan(), context, add));
    return temporaries;
}

void ClassToInterfaceConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    void* classPtr = emitter.Stack().Pop();
    void* classPtrAsVoidPtr = emitter.CreateBitCast(classPtr, emitter.GetIrTypeForVoidPtrType());
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* temporaryInterfaceObjectVar = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(targetInterfaceType->IrType(emitter, context), temporaryInterfaceObjectVar);
    emitter.CreateStore(classPtrAsVoidPtr, objectPtr);
    ClassTypeSymbol* vmtPtrHolderClass = sourceClassType->VmtPtrHolderClass();
    if (sourceClassType != vmtPtrHolderClass)
    {
        classPtr = emitter.CreateBitCast(classPtr, vmtPtrHolderClass->AddPointer(context)->IrType(emitter, context));
    }
    void* vmtPtr = emitter.GetVmtPtr(vmtPtrHolderClass->IrType(emitter, context), classPtr, vmtPtrHolderClass->VmtPtrIndex(), sourceClassType->VmtPtrType(emitter));
    void* imtsPtr = emitter.GetImtsArrayPtrFromVmt(vmtPtr, sourceClassType->VmtArrayType(emitter), GetImtsVmtIndexOffset());
    void* imtPtr = emitter.GetImtPtrFromImtsPtr(imtsPtr, interfaceIndex, sourceClassType->ImplementedInterfaces().size());
    void* imtPtrPtr = emitter.GetImtPtrPtrFromInterface(targetInterfaceType->IrType(emitter, context), temporaryInterfaceObjectVar);
    emitter.CreateStore(imtPtr, imtPtrPtr);
    emitter.Stack().Push(temporaryInterfaceObjectVar);
    /*
    void* classPtr = emitter.Stack().Pop();
    void* classPtrAsVoidPtr = emitter.CreateBitCast(classPtr, emitter.GetIrTypeForVoidPtrType());
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* temporaryInterfaceObjectVar = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(targetInterfaceType->IrType(emitter), temporaryInterfaceObjectVar);
    emitter.CreateStore(classPtrAsVoidPtr, objectPtr);
    ClassTypeSymbol* vmtHolder = sourceClassType->VmtPtrHolderClass();
    void* vmtObjectPtr = vmtHolder->VmtObject(emitter, true);
    void* imtArray = emitter.GetImtArray(vmtHolder->IrType(emitter), vmtObjectPtr, GetImtsVmtIndexOffset());
    void* imtArrayType = emitter.GetIrTypeForArrayType(emitter.GetIrTypeForVoidPtrType(), sourceClassType->ImplementedInterfaces().size());
    void* imt = emitter.GetImt(imtArrayType, imtArray, interfaceIndex);
    void* imtPtr = emitter.GetImtPtrPtrFromInterface(targetInterfaceType->IrType(emitter), temporaryInterfaceObjectVar);
    emitter.CreateStore(imt, imtPtr);
    emitter.Stack().Push(temporaryInterfaceObjectVar);
*/
}

void ClassToInterfaceConversion::Check()
{
    FunctionSymbol::Check();
    if (!sourceClassType)
    {
        throw SymbolCheckException("class to interface conversion has no source class type", GetFullSpan());
    }
    if (!targetInterfaceType)
    {
        throw SymbolCheckException("class to interface conversion has no target interface type", GetFullSpan());
    }
}

GetObjectPtrFromInterface::GetObjectPtrFromInterface(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::getObjectPtrFromInterfaceSymbol, span_, name_), interfaceType(nullptr)
{
}

GetObjectPtrFromInterface::GetObjectPtrFromInterface(InterfaceTypeSymbol* interfaceType_, Context* context) :
    FunctionSymbol(SymbolType::getObjectPtrFromInterfaceSymbol, interfaceType_->GetSpan(), U"GetObjectPtrFromInterface"), interfaceType(interfaceType_)
{
    SetGroupName(U"GetObjectPtrFromInterface");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(soul::ast::Span(), U"this");
    thisParam->SetType(interfaceType_->AddPointer(context));
    AddMember(thisParam, context);
    TypeSymbol* voidPtrType = context->RootModule()->GetSymbolTable().GetTypeByName(U"void")->AddPointer(context);
    SetReturnType(voidPtrType);
    ComputeName(context);
}

void GetObjectPtrFromInterface::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(interfaceType->TypeId());
}

void GetObjectPtrFromInterface::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void GetObjectPtrFromInterface::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type expected"); 
        interfaceType = static_cast<InterfaceTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void GetObjectPtrFromInterface::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    TypeSymbol* type = static_cast<TypeSymbol*>(genObjects[0]->GetType());
    if (type->GetSymbolType() == SymbolType::interfaceTypeSymbol)
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    }
    else
    {
        genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    }
    void* interfaceTypePtr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter, context), interfaceTypePtr);
    void* object = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), objectPtr); // TODO
    emitter.Stack().Push(object);
}

} // namespace cmajor::symbols
