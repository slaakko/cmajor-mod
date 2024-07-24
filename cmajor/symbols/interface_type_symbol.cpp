// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.interfaces;

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
    TypeSymbol(SymbolType::interfaceTypeSymbol, span_, name_), copyConstructor(nullptr)
{
}

void InterfaceTypeSymbol::AddMember(Symbol* member)
{
    TypeSymbol::AddMember(member);
    if (member->GetSymbolType() == SymbolType::memberFunctionSymbol)
    {
        MemberFunctionSymbol* memberFunction = static_cast<MemberFunctionSymbol*>(member);
        memberFunction->SetImtIndex(memberFunctions.size());
        memberFunctions.push_back(memberFunction);
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

void* InterfaceTypeSymbol::IrType(cmajor::ir::Emitter& emitter)
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

void* InterfaceTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter)
{
    void* irType = IrType(emitter);
    std::vector<void*> arrayOfDefaults;
    arrayOfDefaults.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
    arrayOfDefaults.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
    return emitter.CreateDefaultIrValueForStruct(irType, arrayOfDefaults);
}

void InterfaceTypeSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    MemberFunctionSymbol* interfaceMemberFunction)
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
    void* objectPtr = emitter.GetObjectFromInterface(IrType(emitter), interfaceTypePtr);
    void* imtPtr = emitter.GetImtPtrFromInterface(IrType(emitter), interfaceTypePtr);
    void* callee = emitter.GetInterfaceMethod(IrType(emitter), imtPtr, interfaceMemberFunction->ImtIndex(), interfaceMemberFunction->IrType(emitter));
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
                void* functionType = interfaceMemberFunction->IrType(emitter);
                emitter.Stack().Push(emitter.CreateCall(functionType, callee, args));
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter);
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
                void* functionType = interfaceMemberFunction->IrType(emitter);
                emitter.Stack().Push(emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args));
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter);
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
                void* functionType = interfaceMemberFunction->IrType(emitter);
                emitter.CreateCall(functionType, callee, args);
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter);
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
                void* functionType = interfaceMemberFunction->IrType(emitter);
                emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args);
            }
            else
            {
                void* functionType = interfaceMemberFunction->IrType(emitter);
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

InterfaceTypeDefaultConstructor::InterfaceTypeDefaultConstructor(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::interfaceTypeDefaultCtor, interfaceType_->GetSpan(), U"@constructor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    ComputeName();
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

void InterfaceTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    emitter.CreateStore(emitter.CreateDefaultIrValueForVoidPtrType(), objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    emitter.CreateStore(emitter.CreateDefaultIrValueForVoidPtrType(), interfacePtrPtr);
}

InterfaceTypeCopyConstructor::InterfaceTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyCtor, span_, U"@interfaceCopyCtor"), interfaceType(nullptr)
{
}

InterfaceTypeCopyConstructor::InterfaceTypeCopyConstructor(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyCtor, interfaceType_->GetSpan(), U"@interfaceCopyCtor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst()->AddLvalueReference());
    AddMember(thatParam);
    ComputeName();
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

void InterfaceTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeMoveConstructor::InterfaceTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveCtor, span_, U"@interfaceMoveCtor"), interfaceType(nullptr)
{
}

InterfaceTypeMoveConstructor::InterfaceTypeMoveConstructor(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveCtor, interfaceType_->GetSpan(), U"@interfaceMoveCtor"), interfaceType(interfaceType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst()->AddRvalueReference());
    AddMember(thatParam);
    ComputeName();
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

void InterfaceTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeCopyAssignment::InterfaceTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyAssignment, span_, U"@interfaceCopyAssignment"), interfaceType(nullptr)
{
}

InterfaceTypeCopyAssignment::InterfaceTypeCopyAssignment(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::interfaceTypeCopyAssignment, interfaceType_->GetSpan(), U"@interfaceCopyAssignment"), interfaceType(interfaceType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddConst()->AddLvalueReference());
    AddMember(thatParam);
    TypeSymbol* voidType = GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"void");
    SetReturnType(voidType);
    ComputeName();
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

void InterfaceTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

InterfaceTypeMoveAssignment::InterfaceTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveAssignment, span_, U"@interfaceMoveAssignment"), interfaceType(nullptr)
{
}

InterfaceTypeMoveAssignment::InterfaceTypeMoveAssignment(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::interfaceTypeMoveAssignment, interfaceType_->GetSpan(), U"@interfaceMoveAssignment"), interfaceType(interfaceType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(interfaceType_->GetSpan(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(interfaceType_->GetSpan(), U"that");
    thatParam->SetType(interfaceType_->AddRvalueReference());
    AddMember(thatParam);
    TypeSymbol* voidType = GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"void");
    SetReturnType(voidType);
    ComputeName();
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

void InterfaceTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatObjectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatObject = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    emitter.CreateStore(thatObject, objectPtr);

    void* interfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* thatInterfacePtrPtr = emitter.GetImtPtrPtrFromInterface(interfaceType->IrType(emitter), thatPtr);
    void* thatInterfacePtr = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatInterfacePtrPtr); // TODO
    emitter.CreateStore(thatInterfacePtr, interfacePtrPtr);
}

ClassToInterfaceConversion::ClassToInterfaceConversion(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classToInterfaceConversion, span_, U"@classToInterfaceConversion"), sourceClassType(nullptr), targetInterfaceType(nullptr), interfaceIndex(0)
{
}

ClassToInterfaceConversion::ClassToInterfaceConversion(ClassTypeSymbol* sourceClassType_, InterfaceTypeSymbol* targetInterfaceType_, int32_t interfaceIndex_,
    const soul::ast::Span& span_) :
    FunctionSymbol(SymbolType::classToInterfaceConversion, span_, U"@classToInterfaceConversion"), sourceClassType(sourceClassType_),
    targetInterfaceType(targetInterfaceType_), interfaceIndex(interfaceIndex_)
{
    SetConversion();
    SetConversionSourceType(sourceClassType->PlainType());
    SetConversionTargetType(targetInterfaceType->PlainType());
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

std::vector<LocalVariableSymbol*> ClassToInterfaceConversion::CreateTemporariesTo(FunctionSymbol* currentFunction)
{
    std::vector<LocalVariableSymbol*> temporaries;
    temporaries.push_back(currentFunction->CreateTemporary(targetInterfaceType, GetSpan()));
    return temporaries;
}

void ClassToInterfaceConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
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

GetObjectPtrFromInterface::GetObjectPtrFromInterface(InterfaceTypeSymbol* interfaceType_) :
    FunctionSymbol(SymbolType::getObjectPtrFromInterfaceSymbol, interfaceType_->GetSpan(), U"GetObjectPtrFromInterface"), interfaceType(interfaceType_)
{
    SetGroupName(U"GetObjectPtrFromInterface");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(soul::ast::Span(), U"this");
    thisParam->SetType(interfaceType_->AddPointer());
    AddMember(thisParam);
    TypeSymbol* voidPtrType = GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"void")->AddPointer();
    SetReturnType(voidPtrType);
    ComputeName();
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

void GetObjectPtrFromInterface::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
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
    void* objectPtr = emitter.GetObjectPtrFromInterface(interfaceType->IrType(emitter), interfaceTypePtr);
    void* object = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), objectPtr); // TODO
    emitter.Stack().Push(object);
}

} // namespace cmajor::symbols
