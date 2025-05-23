// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.delegate.symbol;

import soul.ast.span;
import cmajor.ir.emitter;
import cmajor.symbols.exception;
import cmajor.symbols.type.symbol;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.global.flags;
import cmajor.symbols.classes;
import cmajor.symbols.modules;
import util;
import std.core;

namespace cmajor::symbols {

DelegateTypeSymbol::DelegateTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::delegateTypeSymbol, span_, name_), returnType(), parameters()
{
}

void DelegateTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    const util::uuid& returnTypeId = returnType->TypeId();
    writer.GetBinaryStreamWriter().Write(returnTypeId);
    bool hasReturnParam = returnParam != nullptr;
    writer.GetBinaryStreamWriter().Write(hasReturnParam);
    if (hasReturnParam)
    {
        writer.Write(returnParam.get());
    }
}

void DelegateTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    util::uuid returnTypeId;
    reader.GetBinaryStreamReader().ReadUuid(returnTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, returnTypeId, 0);
    bool hasReturnParam = reader.GetBinaryStreamReader().ReadBool();
    if (hasReturnParam)
    {
        returnParam.reset(reader.ReadParameterSymbol(this));
    }
}

void DelegateTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    Assert(index == 0, "invalid emplace type index");
    returnType = typeSymbol;
}

void DelegateTypeSymbol::AddMember(Symbol* member, Context* context)
{
    TypeSymbol::AddMember(member, context);
    if (member->GetSymbolType() == SymbolType::parameterSymbol)
    {
        parameters.push_back(static_cast<ParameterSymbol*>(member));
    }
}

std::string DelegateTypeSymbol::Syntax(Context* context)
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("delegate ");
    syntax.append(util::ToUtf8(ReturnType()->DocName(context)));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName(context)));
    syntax.append(1, '(');
    bool first = true;
    for (ParameterSymbol* param : parameters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            syntax.append(", ");
        }
        syntax.append(util::ToUtf8(param->GetType()->DocName(context)));
        syntax.append(1, ' ');
        syntax.append(util::ToUtf8(param->DocName(context)));
    }
    syntax.append(");");
    return syntax;
}

std::u32string DelegateTypeSymbol::Id() const
{
    return MangledName();
}

void DelegateTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddDelegate(this);
    }
}

void DelegateTypeSymbol::Dump(util::CodeFormatter& formatter, Context* context)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
}

void* DelegateTypeSymbol::IrFunctionType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* retType = emitter.GetIrTypeForVoid();
    if (!returnType->IsVoidType() && !ReturnsClassInterfaceOrClassDelegateByValue())
    {
        retType = returnType->IrType(emitter, context);
    }
    std::vector<void*> paramTypes;
    int np = parameters.size();
    for (int i = 0; i < np; ++i)
    {
        ParameterSymbol* parameter = parameters[i];
        paramTypes.push_back(parameter->GetType()->IrType(emitter, context));
    }
    if (returnParam)
    {
        paramTypes.push_back(returnParam->GetType()->IrType(emitter, context));
    }
    return emitter.GetIrTypeForFunction(retType, paramTypes);
}

void* DelegateTypeSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localIrType = emitter.GetIrTypeByTypeId(TypeId());
    if (!localIrType)
    {
        void* retType = emitter.GetIrTypeForVoid();
        if (!returnType->IsVoidType() && !ReturnsClassInterfaceOrClassDelegateByValue())
        {
            retType = returnType->IrType(emitter, context);
        }
        std::vector<void*> paramTypes;
        int np = parameters.size();
        for (int i = 0; i < np; ++i)
        {
            ParameterSymbol* parameter = parameters[i];
            paramTypes.push_back(parameter->GetType()->IrType(emitter, context));
        }
        if (returnParam)
        {
            paramTypes.push_back(returnParam->GetType()->IrType(emitter, context));
        }
        localIrType = emitter.GetIrTypeForDelegateType(retType, paramTypes);
        emitter.SetIrTypeByTypeId(TypeId(), localIrType);
    }
    return localIrType;
}

void* DelegateTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDefaultIrValueForDelegateType(IrType(emitter, context));
}

void DelegateTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("delegate cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("delegate cannot be unit_test", GetFullSpan());
    }
}

bool DelegateTypeSymbol::ReturnsClassInterfaceOrClassDelegateByValue() const
{
    return returnType->IsClassTypeSymbol() || returnType->GetSymbolType() == SymbolType::classDelegateTypeSymbol || returnType->GetSymbolType() == SymbolType::interfaceTypeSymbol;
}

void DelegateTypeSymbol::SetReturnParam(ParameterSymbol* returnParam_)
{
    returnParam.reset(returnParam_);
}

void DelegateTypeSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    void* callee = nullptr;
    int na = genObjects.size();
    for (int i = 0; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        genObject->Load(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
        if (i == 0)
        {
            callee = emitter.Stack().Pop();
        }
    }
    std::vector<void*> args;
    int n = parameters.size();
    if (ReturnsClassInterfaceOrClassDelegateByValue())
    {
        ++n;
    }
    args.resize(n);
    for (int i = 0; i < n; ++i)
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
    if (returnType->GetSymbolType() != SymbolType::voidTypeSymbol && !ReturnsClassInterfaceOrClassDelegateByValue())
    {
        if (IsNothrow() || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                void* functionType = IrFunctionType(emitter, context);
                emitter.Stack().Push(emitter.CreateCall(functionType, callee, args));
            }
            else
            {
                void* functionType = IrFunctionType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                void* callInst = emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen);
                emitter.Stack().Push(callInst);
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
                void* functionType = IrFunctionType(emitter, context);
                emitter.Stack().Push(emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args));
            }
            else
            {
                void* functionType = IrFunctionType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                void* invokeInst = emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen);
                emitter.Stack().Push(invokeInst);
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
                void* functionType = IrFunctionType(emitter, context);
                emitter.CreateCall(functionType, callee, args);
            }
            else
            {
                void* functionType = IrFunctionType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen);
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
                void* functionType = IrFunctionType(emitter, context);
                emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args);
            }
            else
            {
                void* functionType = IrFunctionType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen);
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
}

void DelegateTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!returnType)
    {
        throw SymbolCheckException("delegate type symbol has no return type", GetFullSpan());
    }
}

std::string DelegateTypeSymbol::GetSymbolHelp() const
{
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(ReturnType()->FullName())).append(" ").append(util::ToUtf8(FullName())).append("(");
    bool first = true;
    for (ParameterSymbol* param : parameters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            help.append(", ");
        }
        help.append(util::ToUtf8(param->GetType()->FullName())).append(" ").append(util::ToUtf8(param->Name()));
    }
    help.append(")");
    return help;
}

DelegateTypeDefaultConstructor::DelegateTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeDefaultConstructor, span_, name_), delegateType(nullptr)
{
}

DelegateTypeDefaultConstructor::DelegateTypeDefaultConstructor(DelegateTypeSymbol* delegateType_, Context* context) :
    FunctionSymbol(SymbolType::delegateTypeDefaultConstructor, delegateType_->GetSpan(), U"@constructor"), delegateType(delegateType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(delegateType->GetSpan(), U"this");
    thisParam->SetType(delegateType->AddPointer(context));
    AddMember(thisParam, context);
    ComputeName(context);
}

void DelegateTypeDefaultConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(delegateType->TypeId());
}

void DelegateTypeDefaultConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void DelegateTypeDefaultConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::delegateTypeSymbol, "delegate type symbol expected");
        delegateType = static_cast<DelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void DelegateTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    Assert(genObjects.size() == 1, "default constructor needs one object");
    emitter.Stack().Push(delegateType->CreateDefaultIrValue(emitter, context));
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

void DelegateTypeDefaultConstructor::Check()
{
    FunctionSymbol::Check();
    if (!delegateType)
    {
        throw SymbolCheckException("delegate type default constructor has no delegate type", GetFullSpan());
    }
}

DelegateTypeCopyConstructor::DelegateTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeCopyConstructor, span_, name_)
{
}

DelegateTypeCopyConstructor::DelegateTypeCopyConstructor(DelegateTypeSymbol* delegateType, Context* context) : FunctionSymbol(SymbolType::delegateTypeCopyConstructor,
    delegateType->GetSpan(), U"@constructor")
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(delegateType->GetSpan(), U"this");
    thisParam->SetType(delegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(delegateType->GetSpan(), U"that");
    thatParam->SetType(delegateType);
    AddMember(thatParam, context);
    ComputeName(context);
}

void DelegateTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
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

DelegateTypeMoveConstructor::DelegateTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeMoveConstructor, span_, name_), delegateType(nullptr)
{
}

DelegateTypeMoveConstructor::DelegateTypeMoveConstructor(DelegateTypeSymbol* delegateType_, Context* context) :
    FunctionSymbol(SymbolType::delegateTypeMoveConstructor, delegateType_->GetSpan(), U"@constructor"), delegateType(delegateType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(delegateType->GetSpan(), U"this");
    thisParam->SetType(delegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(delegateType->GetSpan(), U"that");
    thatParam->SetType(delegateType->AddRvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void DelegateTypeMoveConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(delegateType->TypeId());
}

void DelegateTypeMoveConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void DelegateTypeMoveConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::delegateTypeSymbol, "delegate type symbol expected");
        delegateType = static_cast<DelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void DelegateTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    Assert(genObjects.size() == 2, "move constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        emitter.Stack().Dup();
        void* ptr = emitter.Stack().Pop();
        emitter.SaveObjectPointer(ptr);
    }
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(delegateType->IrType(emitter, context), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

DelegateTypeCopyAssignment::DelegateTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeCopyAssignment, span_, name_)
{
}

DelegateTypeCopyAssignment::DelegateTypeCopyAssignment(DelegateTypeSymbol* delegateType, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::delegateTypeCopyAssignment, delegateType->GetSpan(), U"operator=")
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(delegateType->GetSpan(), U"this");
    thisParam->SetType(delegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(delegateType->GetSpan(), U"that");
    thatParam->SetType(delegateType);
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void DelegateTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

DelegateTypeMoveAssignment::DelegateTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeMoveAssignment, span_, name_), delegateType(nullptr)
{
}

DelegateTypeMoveAssignment::DelegateTypeMoveAssignment(DelegateTypeSymbol* delegateType_, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::delegateTypeMoveAssignment, delegateType_->GetSpan(), U"operator="), delegateType(delegateType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(delegateType->GetSpan(), U"this");
    thisParam->SetType(delegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(delegateType->GetSpan(), U"that");
    thatParam->SetType(delegateType->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void DelegateTypeMoveAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(delegateType->TypeId());
}

void DelegateTypeMoveAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void DelegateTypeMoveAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::delegateTypeSymbol, "delegate type symbol expected");
        delegateType = static_cast<DelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void DelegateTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 2, "move assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(delegateType->IrType(emitter, context), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

DelegateTypeReturn::DelegateTypeReturn(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeReturn, span_, name_)
{
}

DelegateTypeReturn::DelegateTypeReturn(DelegateTypeSymbol* delegateType, Context* context) :
    FunctionSymbol(SymbolType::delegateTypeReturn, delegateType->GetSpan(), U"@return")
{
    SetGroupName(U"@return");
    ParameterSymbol* valueParam = new ParameterSymbol(delegateType->GetSpan(), U"value");
    valueParam->SetType(delegateType);
    AddMember(valueParam, context);
    SetReturnType(delegateType);
    ComputeName(context);
}

void DelegateTypeReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

DelegateTypeEquality::DelegateTypeEquality(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeEquality, span_, name_)
{
}

DelegateTypeEquality::DelegateTypeEquality(DelegateTypeSymbol* delegateType, TypeSymbol* boolType, Context* context)
    : FunctionSymbol(SymbolType::delegateTypeEquality, delegateType->GetSpan(), U"operator==")
{
    SetGroupName(U"operator==");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(delegateType->GetSpan(), U"left");
    leftParam->SetType(delegateType);
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(delegateType->GetSpan(), U"right");
    rightParam->SetType(delegateType);
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void DelegateTypeEquality::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 2, "operator== needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateICmpEQ(left, right));
}

DelegateTypeLess::DelegateTypeLess(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::delegateTypeLess, span_, name_)
{
}

DelegateTypeLess::DelegateTypeLess(DelegateTypeSymbol* delegateType, TypeSymbol* boolType, Context* context)
    : FunctionSymbol(SymbolType::delegateTypeLess, delegateType->GetSpan(), U"operator<")
{
    SetGroupName(U"operator<");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(delegateType->GetSpan(), U"left");
    leftParam->SetType(delegateType);
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(delegateType->GetSpan(), U"right");
    rightParam->SetType(delegateType);
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void DelegateTypeLess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(genObjects.size() == 2, "operator< needs two objects");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* left = emitter.Stack().Pop();
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateICmpULT(left, right));
}

FunctionToDelegateConversion::FunctionToDelegateConversion(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::functionToDelegateSymbol, span_, name_), sourceType(nullptr), targetType(nullptr), function(nullptr)
{
}

FunctionToDelegateConversion::FunctionToDelegateConversion(TypeSymbol* sourceType_, TypeSymbol* targetType_, FunctionSymbol* function_, Context* context) :
    FunctionSymbol(SymbolType::functionToDelegateSymbol, function_->GetSpan(), U"@conversion"), sourceType(sourceType_), targetType(targetType_), function(function_)
{
    SetConversion();
    SetConversionSourceType(sourceType->PlainType(context));
    SetConversionTargetType(targetType->PlainType(context));
}

void FunctionToDelegateConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    emitter.Stack().Pop();
    if (GetBackEnd() == BackEnd::systemx)
    {
        emitter.Stack().Push(emitter.MakeSymbolValue(function->IrType(emitter, context), util::ToUtf8(function->MangledName())));
    }
    else
    {
        emitter.Stack().Push(emitter.GetOrInsertFunction(util::ToUtf8(function->MangledName()), function->IrType(emitter, context), function->DontThrow()));
    }
}

void FunctionToDelegateConversion::Check()
{
    FunctionSymbol::Check();
    if (!sourceType)
    {
        throw SymbolCheckException("function to delegate conversion has no source type", GetFullSpan());
    }
    if (!targetType)
    {
        throw SymbolCheckException("function to delegate conversion has no target type", GetFullSpan());
    }
    if (!function)
    {
        throw SymbolCheckException("function to delegate conversion has no function", GetFullSpan());
    }
}

DelegateToVoidPtrConversion::DelegateToVoidPtrConversion(TypeSymbol* delegateType_, TypeSymbol* voidPtrType_, Context* context) :
    FunctionSymbol(delegateType_->GetSpan(), U"dlg2voidptr"), delegateType(delegateType_), voidPtrType(voidPtrType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(SymbolAccess::public_);
    SetConversionSourceType(delegateType->PlainType(context));
    SetConversionTargetType(voidPtrType->PlainType(context));
}

void DelegateToVoidPtrConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateBitCast(value, voidPtrType->IrType(emitter, context)));
}

VoidPtrToDelegateConversion::VoidPtrToDelegateConversion(TypeSymbol* voidPtrType_, TypeSymbol* delegateType_, TypeSymbol* ulongType_, Context* context) :
    FunctionSymbol(delegateType_->GetSpan(), U"voidptr2dlg"), voidPtrType(voidPtrType_), delegateType(delegateType_), ulongType(ulongType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(SymbolAccess::public_);
    SetConversionSourceType(delegateType->PlainType(context));
    SetConversionTargetType(voidPtrType->PlainType(context));
}

void VoidPtrToDelegateConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    void* value = emitter.Stack().Pop();
    void* ulongValue = emitter.CreatePtrToInt(value, ulongType->IrType(emitter, context));
    emitter.Stack().Push(emitter.CreateIntToPtr(ulongValue, delegateType->IrType(emitter, context)));
}

ClassDelegateTypeSymbol::ClassDelegateTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::classDelegateTypeSymbol, span_, name_), returnType(nullptr), parameters(), delegateType(nullptr), objectDelegatePairType(nullptr),
    copyConstructor(nullptr)
{
}

void ClassDelegateTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    const util::uuid& returnTypeId = returnType->TypeId();
    writer.GetBinaryStreamWriter().Write(returnTypeId);
}

void ClassDelegateTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    util::uuid returnTypeId;
    reader.GetBinaryStreamReader().ReadUuid(returnTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, returnTypeId, -1);
}

void ClassDelegateTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == -1)
    {
        returnType = typeSymbol;
    }
    else
    {
        TypeSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeSymbol::AddMember(Symbol* member, Context* context)
{
    TypeSymbol::AddMember(member, context);
    if (member->GetSymbolType() == SymbolType::parameterSymbol)
    {
        parameters.push_back(static_cast<ParameterSymbol*>(member));
    }
    else if (member->GetSymbolType() == SymbolType::delegateTypeSymbol)
    {
        delegateType = static_cast<DelegateTypeSymbol*>(member);
    }
    else if (member->GetSymbolType() == SymbolType::classTypeSymbol)
    {
        objectDelegatePairType = static_cast<ClassTypeSymbol*>(member);
    }
    else if (member->IsFunctionSymbol())
    {
        FunctionSymbol* functionSymbol = static_cast<FunctionSymbol*>(member);
        if (functionSymbol->IsClassDelegateCopyConstructor())
        {
            copyConstructor = functionSymbol;
        }
    }
}

std::string ClassDelegateTypeSymbol::Syntax(Context* context)
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("class delegate ");
    syntax.append(util::ToUtf8(ReturnType()->DocName(context)));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName(context)));
    syntax.append(1, '(');
    bool first = true;
    for (ParameterSymbol* param : parameters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            syntax.append(", ");
        }
        syntax.append(util::ToUtf8(param->GetType()->DocName(context)));
        syntax.append(1, ' ');
        syntax.append(util::ToUtf8(param->DocName(context)));
    }
    syntax.append(");");
    return syntax;
}

std::string ClassDelegateTypeSymbol::GetSymbolHelp() const
{
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(ReturnType()->FullName())).append(" ").append(util::ToUtf8(FullName())).append("(");
    bool first = true;
    for (ParameterSymbol* param : parameters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            help.append(", ");
        }
        help.append(util::ToUtf8(param->GetType()->FullName())).append(" ").append(util::ToUtf8(param->Name()));
    }
    help.append(")");
    return help;
}

std::u32string ClassDelegateTypeSymbol::Id() const
{
    return MangledName();
}

void ClassDelegateTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddClassDelegate(this);
    }
}

void ClassDelegateTypeSymbol::Dump(util::CodeFormatter& formatter, Context* context)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
}

void* ClassDelegateTypeSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localIrType = emitter.GetIrTypeByTypeId(TypeId());
    if (!localIrType)
    {
        std::vector<void*> elementTypes;
        elementTypes.push_back(emitter.GetIrTypeForVoidPtrType());
        elementTypes.push_back(delegateType->IrType(emitter, context));
        localIrType = emitter.GetIrTypeForStructType(elementTypes);
        emitter.SetIrTypeByTypeId(TypeId(), localIrType);
    }
    return localIrType;
}

void* ClassDelegateTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context)
{
    std::vector<void*> constants;
    constants.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
    constants.push_back(delegateType->CreateDefaultIrValue(emitter, context));
    return emitter.CreateDefaultIrValueForStruct(IrType(emitter, context), constants);
}

bool ClassDelegateTypeSymbol::ReturnsClassInterfaceOrClassDelegateByValue() const
{
    return returnType->IsClassTypeSymbol() || returnType->GetSymbolType() == SymbolType::classDelegateTypeSymbol || returnType->GetSymbolType() == SymbolType::interfaceTypeSymbol;
}

void ClassDelegateTypeSymbol::SetReturnParam(ParameterSymbol* returnParam_)
{
    returnParam.reset(returnParam_);
}

void ClassDelegateTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("class delegate cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class delegate cannot be unit_test", GetFullSpan());
    }
}

void ClassDelegateTypeSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    Assert(!genObjects.empty(), "gen objects is empty");
    genObjects[0]->Load(emitter, flags);
    void* classDelegatePtr = emitter.Stack().Pop();
    void* dlgType = delegateType->IrType(emitter, context);
    void* delegatePtr = emitter.GetDelegateFromClassDelegate(IrType(emitter, context), classDelegatePtr);
    void* callee = emitter.CreateLoad(dlgType, delegatePtr); // TODO
    cmajor::ir::NativeValue calleeValue(callee);
    void* objectPtr = emitter.GetObjectFromClassDelegate(IrType(emitter, context), classDelegatePtr);
    void* object = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), objectPtr); // TODO
    cmajor::ir::NativeValue objectValue(object);
    std::vector<cmajor::ir::GenObject*> classDelegateCallObjects;
    classDelegateCallObjects.push_back(&calleeValue);
    classDelegateCallObjects.push_back(&objectValue);
    int na = genObjects.size();
    for (int i = 1; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        classDelegateCallObjects.push_back(genObject);
    }
    delegateType->GenerateCall(emitter, classDelegateCallObjects, flags, context);
}

void ClassDelegateTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!returnType)
    {
        throw SymbolCheckException("class delegate type symbol has no return type", GetFullSpan());
    }
    if (!delegateType)
    {
        throw SymbolCheckException("class delegate type symbol has no delegate type", GetFullSpan());
    }
    if (!objectDelegatePairType)
    {
        throw SymbolCheckException("class delegate type symbol has no object delegate pair type", GetFullSpan());
    }
    if (!copyConstructor)
    {
        throw SymbolCheckException("class delegate type symbol has no copy constructor", GetFullSpan());
    }
}

ClassDelegateTypeDefaultConstructor::ClassDelegateTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeDefaultConstructor, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeDefaultConstructor::ClassDelegateTypeDefaultConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeDefaultConstructor, classDelegateType_->GetSpan(), U"@constructor"), 
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(classDelegateType->GetSpan(), U"this");
    thisParam->SetType(classDelegateType->AddPointer(context));
    AddMember(thisParam, context);
    ComputeName(context);
}

void ClassDelegateTypeDefaultConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeDefaultConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeDefaultConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    Assert(genObjects.size() == 1, "default constructor needs one object");
    void* objectValue = emitter.CreateDefaultIrValueForVoidPtrType();
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* ptr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), ptr);
    emitter.CreateStore(objectValue, objectPtr);
    void* delegateValue = classDelegateType->DelegateType()->CreateDefaultIrValue(emitter, context);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* delegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), ptr);
    emitter.CreateStore(delegateValue, delegatePtr);
}

void ClassDelegateTypeDefaultConstructor::Check()
{
    FunctionSymbol::Check();
    if (!classDelegateType)
    {
        throw SymbolCheckException("class delegate type default constructor has no class delegate type", GetFullSpan());
    }
}

ClassDelegateTypeCopyConstructor::ClassDelegateTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeCopyConstructor, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeCopyConstructor::ClassDelegateTypeCopyConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeCopyConstructor, classDelegateType_->GetSpan(), U"@constructor"), 
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(classDelegateType->GetSpan(), U"this");
    thisParam->SetType(classDelegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(classDelegateType->GetSpan(), U"that");
    thatParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void ClassDelegateTypeCopyConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeCopyConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeCopyConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* thatObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* objectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    cmajor::ir::OperationFlags loadFlags = cmajor::ir::OperationFlags::none;
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        loadFlags = loadFlags | cmajor::ir::OperationFlags::leaveFirstArg;
    }
    genObjects[0]->Load(emitter, loadFlags);
    void* thisPtr = emitter.Stack().Pop();
    void* thisObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(objectValue, thisObjectPtr);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* thatDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* delegateValue = emitter.CreateLoad(dlgType, thatDelegatePtr); // TODO
    void* thisDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(delegateValue, thisDelegatePtr);
}

ClassDelegateTypeMoveConstructor::ClassDelegateTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeMoveConstructor, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeMoveConstructor::ClassDelegateTypeMoveConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeCopyConstructor, classDelegateType_->GetSpan(), U"@constructor"),
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(classDelegateType->GetSpan(), U"this");
    thisParam->SetType(classDelegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(classDelegateType->GetSpan(), U"that");
    thatParam->SetType(classDelegateType->AddRvalueReference(context));
    AddMember(thatParam, context);
    ComputeName(context);
}

void ClassDelegateTypeMoveConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeMoveConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeMoveConstructor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* thatObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* objectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    cmajor::ir::OperationFlags loadFlags = cmajor::ir::OperationFlags::none;
    if ((flags & cmajor::ir::OperationFlags::leaveFirstArg) != cmajor::ir::OperationFlags::none)
    {
        loadFlags = loadFlags | cmajor::ir::OperationFlags::leaveFirstArg;
    }
    genObjects[0]->Load(emitter, loadFlags);
    void* thisPtr = emitter.Stack().Pop();
    void* thisObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(objectValue, thisObjectPtr);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* thatDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* delegateValue = emitter.CreateLoad(dlgType, thatDelegatePtr); // TODO
    void* thisDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(delegateValue, thisDelegatePtr);
}

ClassDelegateTypeCopyAssignment::ClassDelegateTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeCopyAssignment, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeCopyAssignment::ClassDelegateTypeCopyAssignment(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeCopyAssignment, classDelegateType_->GetSpan(), U"operator="), 
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(classDelegateType->GetSpan(), U"this");
    thisParam->SetType(classDelegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(classDelegateType->GetSpan(), U"that");
    thatParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void ClassDelegateTypeCopyAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeCopyAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeCopyAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* thatObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* objectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thisPtr = emitter.Stack().Pop();
    void* thisObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(objectValue, thisObjectPtr);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* thatDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* delegateValue = emitter.CreateLoad(dlgType, thatDelegatePtr); // TODO
    void* thisDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(delegateValue, thisDelegatePtr);
}

ClassDelegateTypeMoveAssignment::ClassDelegateTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeMoveAssignment, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeMoveAssignment::ClassDelegateTypeMoveAssignment(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* voidType, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeMoveAssignment, classDelegateType_->GetSpan(), U"operator="),
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(classDelegateType->GetSpan(), U"this");
    thisParam->SetType(classDelegateType->AddPointer(context));
    AddMember(thisParam, context);
    ParameterSymbol* thatParam = new ParameterSymbol(classDelegateType->GetSpan(), U"that");
    thatParam->SetType(classDelegateType->AddRvalueReference(context));
    AddMember(thatParam, context);
    SetReturnType(voidType);
    ComputeName(context);
}

void ClassDelegateTypeMoveAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeMoveAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeMoveAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thatPtr = emitter.Stack().Pop();
    void* thatObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* objectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), thatObjectPtr); // TODO
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* thisPtr = emitter.Stack().Pop();
    void* thisObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(objectValue, thisObjectPtr);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* thatDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thatPtr);
    void* delegateValue = emitter.CreateLoad(dlgType, thatDelegatePtr); // TODO
    void* thisDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), thisPtr);
    emitter.CreateStore(delegateValue, thisDelegatePtr);
}

ClassDelegateTypeEquality::ClassDelegateTypeEquality(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeEquality, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeEquality::ClassDelegateTypeEquality(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* boolType, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeEquality, classDelegateType_->GetSpan(), U"operator=="),
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"operator==");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(classDelegateType->GetSpan(), U"left");
    leftParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(classDelegateType->GetSpan(), U"right");
    rightParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void ClassDelegateTypeEquality::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeEquality::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeEquality::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeEquality::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftPtr = emitter.Stack().Pop();
    void* leftObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), leftPtr);
    void* leftObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), leftObjectPtr); // TODO
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightPtr = emitter.Stack().Pop();
    void* rightObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), rightPtr);
    void* rightObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), rightObjectPtr); // TODO
    void* objectsEqual = emitter.CreateICmpEQ(leftObjectValue, rightObjectValue);
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* leftDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), leftPtr);
    void* leftDelegateValue = emitter.CreateLoad(dlgType, leftDelegatePtr); // TODO
    void* rightDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), rightPtr);
    void* rightDelegateValue = emitter.CreateLoad(dlgType, rightDelegatePtr); // TODO
    void* delegatesEqual = emitter.CreateICmpEQ(leftDelegateValue, rightDelegateValue);
    void* equal = emitter.CreateAnd(objectsEqual, delegatesEqual);
    emitter.Stack().Push(equal);
}

ClassDelegateTypeLess::ClassDelegateTypeLess(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::classDelegateTypeLess, span_, name_), classDelegateType(nullptr)
{
}

ClassDelegateTypeLess::ClassDelegateTypeLess(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* boolType, Context* context) :
    FunctionSymbol(SymbolType::classDelegateTypeLess, classDelegateType_->GetSpan(), U"operator<"),
    classDelegateType(classDelegateType_)
{
    SetGroupName(U"operator<");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(classDelegateType->GetSpan(), U"left");
    leftParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(leftParam, context);
    ParameterSymbol* rightParam = new ParameterSymbol(classDelegateType->GetSpan(), U"right");
    rightParam->SetType(classDelegateType->AddConst(context)->AddLvalueReference(context));
    AddMember(rightParam, context);
    SetReturnType(boolType);
    ComputeName(context);
}

void ClassDelegateTypeLess::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(classDelegateType->TypeId());
}

void ClassDelegateTypeLess::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void ClassDelegateTypeLess::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
        classDelegateType = static_cast<ClassDelegateTypeSymbol*>(typeSymbol);
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void ClassDelegateTypeLess::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    void* retVal = emitter.CreateAlloca(emitter.GetIrTypeForBool());
    void* retBlock = emitter.CreateBasicBlock("ret");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* leftPtr = emitter.Stack().Pop();
    void* leftObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), leftPtr);
    void* leftObjectValue = emitter.CreateLoad(emitter.GetIrTypeForVoidPtrType(), leftObjectPtr); // TODO
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rightPtr = emitter.Stack().Pop();
    void* rightObjectPtr = emitter.GetObjectFromClassDelegate(classDelegateType->IrType(emitter, context), rightPtr);
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
    void* dlgType = classDelegateType->DelegateType()->IrType(emitter, context);
    void* leftDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), leftPtr);
    void* leftDelegateValue = emitter.CreateLoad(dlgType, leftDelegatePtr); // TODO
    void* rightDelegatePtr = emitter.GetDelegateFromClassDelegate(classDelegateType->IrType(emitter, context), rightPtr);
    void* rightDelegateValue = emitter.CreateLoad(dlgType, rightDelegatePtr); // TODO
    void* leftDelegateLessThanRightDelegate = emitter.CreateICmpULT(leftDelegateValue, rightDelegateValue);
    emitter.CreateStore(leftDelegateLessThanRightDelegate, retVal);
    emitter.CreateBr(retBlock);
    emitter.SetCurrentBasicBlock(retBlock);
    void* rv = emitter.CreateLoad(emitter.GetIrTypeForBool(), retVal);
    emitter.Stack().Push(rv);
}

MemberFunctionToClassDelegateConversion::MemberFunctionToClassDelegateConversion(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::memberFunctionToClassDelegateSymbol, span_, name_), sourceType(nullptr), targetType(nullptr), function(nullptr)
{
}

MemberFunctionToClassDelegateConversion::MemberFunctionToClassDelegateConversion(const soul::ast::Span& span_, TypeSymbol* sourceType_, 
    ClassDelegateTypeSymbol* targetType_, FunctionSymbol* function_, Context* context) :
    FunctionSymbol(SymbolType::memberFunctionToClassDelegateSymbol, span_, U"@conversion"), sourceType(sourceType_), targetType(targetType_), function(function_)
{
    SetConversion();
    SetConversionSourceType(sourceType->PlainType(context));
    SetConversionTargetType(targetType->PlainType(context));
}

std::vector<LocalVariableSymbol*> MemberFunctionToClassDelegateConversion::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context, bool add)
{
    std::vector<LocalVariableSymbol*> temporaries;
    LocalVariableSymbol* objectDelegatePairVariable = currentFunction->CreateTemporary(targetType->ObjectDelegatePairType(), GetSpan(), context, add);
    temporaries.push_back(objectDelegatePairVariable);
    return temporaries;
}

void MemberFunctionToClassDelegateConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    Context* context)
{
    Assert(genObjects.size() == 1, "MemberFunctionToClassDelegateConversion needs one temporary object");
    void* objectValue = emitter.Stack().Pop();
    if (!objectValue)
    {
        throw Exception("cannot construct class delegate because expression has no this pointer", GetFullSpan());
    }
    void* objectValueAsVoidPtr = emitter.CreateBitCast(objectValue, emitter.GetIrTypeForVoidPtrType());
    void* memFunPtrValue = nullptr;
    if (GetBackEnd() == BackEnd::systemx)
    {
        memFunPtrValue = emitter.MakeSymbolValue(function->IrType(emitter, context), util::ToUtf8(function->MangledName()));
    }
    else
    {
        memFunPtrValue = emitter.GetOrInsertFunction(util::ToUtf8(function->MangledName()), function->IrType(emitter, context), function->DontThrow());
    }
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::addr);
    void* ptr = emitter.Stack().Pop();
    void* objectPtr = emitter.GetObjectFromClassDelegate(targetType->IrType(emitter, context), ptr);
    emitter.CreateStore(objectValueAsVoidPtr, objectPtr);
    void* dlgType = targetType->DelegateType()->IrType(emitter, context);
    void* delegatePtr = emitter.GetDelegateFromClassDelegate(targetType->IrType(emitter, context), ptr);
    void* delegateValue = emitter.CreateBitCast(memFunPtrValue, targetType->DelegateType()->IrType(emitter, context));
    emitter.CreateStore(delegateValue, delegatePtr);
    emitter.Stack().Push(ptr);
}

void MemberFunctionToClassDelegateConversion::Check()
{
    FunctionSymbol::Check();
    if (!sourceType)
    {
        throw SymbolCheckException("member function to class delegate conversion has no source type", GetFullSpan());
    }
    if (!targetType)
    {
        throw SymbolCheckException("member function to class delegate conversion has no target type", GetFullSpan());
    }
    if (!function)
    {
        throw SymbolCheckException("member function to class delegate conversion has no function", GetFullSpan());
    }
}

} // namespace cmajor::symbols
