// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.llvm.codegen;

import util;

namespace cmajor::llvm {

struct NativeModule
{
    NativeModule(cmajor::ir::Emitter* emitter_, const std::string& moduleFilePath_) : emitter(emitter_)
    {
        module = emitter->CreateModule(moduleFilePath_);
        emitter->SetModule(module);
    }
    ~NativeModule()
    {
        emitter->DestroyModule(module);
    }
    cmajor::ir::Emitter* emitter;
    void* module;
};

LLVMCodeGenerator::LLVMCodeGenerator(cmajor::ir::Emitter* emitter_) : symbolTable(nullptr), symbolsModule(nullptr), emitter(emitter_), module(nullptr), debugInfo(false),
    compileUnit(nullptr), currentClass(nullptr), currentFunction(nullptr), classStack(), utf8stringMap(), utf16stringMap(), utf32stringMap(), uuidMap(),
    trueBlock(nullptr), falseBlock(nullptr), breakTarget(nullptr), continueTarget(nullptr), handlerBlock(nullptr), cleanupBlock(nullptr), entryBasicBlock(nullptr),
    newCleanupNeeded(false), currentPad(nullptr), prevLineNumber(0), destructorCallGenerated(false), lastInstructionWasRet(false), basicBlockOpen(false),
    lastAlloca(nullptr), compoundLevel(0), compileUnitId(), sequenceSecond(nullptr), currentBlock(nullptr), cleanups(), pads(), labeledStatementMap(),
    function(nullptr), blockDestructionMap(), blocks(), genJumpingBoolCode(false), breakTargetBlock(nullptr), continueTargetBlock(nullptr), defaultDest(nullptr),
    currentCaseMap(), compileUnitFunctions(), fileIndex(-1), inSetLineOrEntryCode(false), generateLineNumbers(false)
{
    emitter->SetEmittingDelegate(this);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    fileIndex = boundCompileUnit.FileIndex();
    symbolTable = &boundCompileUnit.GetSymbolTable();
    symbolsModule = &boundCompileUnit.GetModule();
    compileUnitId = boundCompileUnit.Id();
    if (!symbolsModule->IsCore())
    {
        symbolsModule->AddCompileUnitId(compileUnitId);
    }
    emitter->SetCompileUnitId(compileUnitId);
    generateLineNumbers = false;
    NativeModule nativeModule(emitter, util::Path::GetFileName(boundCompileUnit.GetCompileUnitNode()->FilePath()));
    module = nativeModule.module;
    emitter->SetTargetTriple(emitter->EmittingContext()->TargetTripleStr());
    emitter->SetDataLayout(emitter->EmittingContext()->DataLayout());
    emitter->SetSourceFileName(util::Path::GetFileName(boundCompileUnit.GetCompileUnitNode()->FilePath()));
    emitter->ResetCurrentDebugLocation();
    debugInfo = false;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo) && boundCompileUnit.GetCompileUnitNode() && 
        !boundCompileUnit.GetCompileUnitNode()->IsSynthesizedUnit() && 
        !compileUnitId.starts_with("Attribute_") && 
        !compileUnitId.starts_with("type_symbol_") &&
        !compileUnitId.starts_with("interface_type_symbol_"))
    {
        emitter->StartDebugInfo(boundCompileUnit.GetCompileUnitNode()->FilePath(), cmajor::symbols::GetCompilerVersion(), 
            cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release));
        debugInfo = true;
    }
    compileUnit = &boundCompileUnit;
    cmajor::binder::ConstantArrayRepository& constantArrayRepository = boundCompileUnit.GetConstantArrayRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantArrayRepository.ConstantArrays())
    {
        constantSymbol->ArrayIrObject(*emitter, true);
    }
    cmajor::binder::ConstantStructureRepository& constantStructureRepository = boundCompileUnit.GetConstantStructureRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantStructureRepository.ConstantStructures())
    {
        constantSymbol->StructureIrObject(*emitter, true);
    }
    int n = boundCompileUnit.BoundNodes().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* boundNode = boundCompileUnit.BoundNodes()[i].get();
        boundNode->Accept(*this);
    }
    if (debugInfo)
    {
        emitter->FinalizeDebugInfo();
    }
    if (debugInfo)
    {
        emitter->EndDebugInfo();
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::print))
    {
        emitter->PrintModule();
    }
    emitter->VerifyModule();
    emitter->Compile(boundCompileUnit.ObjectFilePath());
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundNamespace& boundNamespace)
{
    int numComponents = 0;
    if (debugInfo)
    {
        if (!boundNamespace.GetNamespaceNode().Id()->Str().empty())
        {
            std::vector<std::u32string> components = util::Split(boundNamespace.GetNamespaceNode().Id()->Str(), '.');
            numComponents = components.size();
            for (int i = 0; i < numComponents; ++i)
            {
                void* ns = emitter->CreateDebugInfoForNamespace(emitter->CurrentScope(), util::ToUtf8(components[i]));
                emitter->PushScope(ns);
            }
        }
        else
        {
            emitter->PushScope(emitter->GetDebugInfoForFile(boundNamespace.GetFullSpan()));
        }
    }
    int n = boundNamespace.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* member = boundNamespace.Members()[i].get();
        member->Accept(*this);
    }
    if (debugInfo)
    {
        if (!boundNamespace.GetNamespaceNode().Id()->Str().empty())
        {
            for (int i = 0; i < numComponents; ++i)
            {
                emitter->PopScope();
            }
        }
        else
        {
            emitter->PopScope();
        }
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundClass& boundClass)
{
    classStack.push(currentClass);
    currentClass = &boundClass;
    bool prevDebugInfo = debugInfo;
    void* prevDIBuilder = emitter->DIBuilder();
    if (!boundClass.ContainsSourceFunctions())
    {
        emitter->ResetCurrentDebugLocation();
        debugInfo = false;
        emitter->SetCurrentDIBuilder(nullptr);
    }
    if (debugInfo)
    {
        emitter->MapClassPtr(currentClass->GetClassTypeSymbol()->TypeId(), currentClass->GetClassTypeSymbol(), util::ToUtf8(currentClass->GetClassTypeSymbol()->FullName()));
        void* diType = emitter->GetDITypeByTypeId(currentClass->GetClassTypeSymbol()->TypeId());
        if (diType)
        {
            emitter->PushScope(diType);
        }
        else
        {
            void* baseClassDIType = nullptr;
            if (currentClass->GetClassTypeSymbol()->BaseClass())
            {
                baseClassDIType = currentClass->GetClassTypeSymbol()->BaseClass()->GetDIType(*emitter);
            }
            void* vtableHolderClassDIType = nullptr;
            if (currentClass->GetClassTypeSymbol()->IsPolymorphic() && currentClass->GetClassTypeSymbol()->VmtPtrHolderClass())
            {
                vtableHolderClassDIType = currentClass->GetClassTypeSymbol()->VmtPtrHolderClass()->CreateDIForwardDeclaration(*emitter);
                emitter->MapFwdDeclaration(vtableHolderClassDIType, currentClass->GetClassTypeSymbol()->VmtPtrHolderClass()->TypeId());
            }
            void* classIrType = currentClass->GetClassTypeSymbol()->IrType(*emitter);
            soul::ast::FullSpan classFullSpan = currentClass->GetClassTypeSymbol()->GetFullSpan();
            soul::ast::LineColLen classLineColLen = cmajor::symbols::GetLineColLen(classFullSpan);
            if (currentClass->GetClassTypeSymbol()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(
                    currentClass->GetClassTypeSymbol());
                classFullSpan = specialization->GetClassTemplate()->GetFullSpan();
                classLineColLen = cmajor::symbols::GetLineColLen(classFullSpan);
            }
            void* forwardDeclaration = emitter->CreateIrDIForwardDeclaration(classIrType, util::ToUtf8(currentClass->GetClassTypeSymbol()->Name()), 
                util::ToUtf8(currentClass->GetClassTypeSymbol()->MangledName()), classFullSpan, classLineColLen);
            emitter->SetDITypeByTypeId(currentClass->GetClassTypeSymbol()->TypeId(), forwardDeclaration, util::ToUtf8(currentClass->GetClassTypeSymbol()->FullName()));
            std::vector<void*> memberVariableElements;
            for (cmajor::symbols::MemberVariableSymbol* memberVariable : currentClass->GetClassTypeSymbol()->MemberVariables())
            {
                int memberVariableLayoutIndex = memberVariable->LayoutIndex();
                uint64_t offsetInBits = emitter->GetOffsetInBits(classIrType, memberVariableLayoutIndex);
                memberVariableElements.push_back(memberVariable->GetDIMemberType(*emitter, offsetInBits));
            }
            void* clsDIType = emitter->CreateDITypeForClassType(classIrType, memberVariableElements, classFullSpan, classLineColLen,
                util::ToUtf8(currentClass->GetClassTypeSymbol()->Name()), vtableHolderClassDIType,
                util::ToUtf8(currentClass->GetClassTypeSymbol()->MangledName()), baseClassDIType);
            emitter->MapFwdDeclaration(forwardDeclaration, currentClass->GetClassTypeSymbol()->TypeId());
            emitter->SetDITypeByTypeId(currentClass->GetClassTypeSymbol()->TypeId(), clsDIType, util::ToUtf8(currentClass->GetClassTypeSymbol()->FullName()));
            emitter->PushScope(clsDIType);
        }
    }
    int n = boundClass.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* boundNode = boundClass.Members()[i].get();
        boundNode->Accept(*this);
    }
    if (debugInfo)
    {
        emitter->PopScope();
    }
    currentClass = classStack.top();
    classStack.pop();
    debugInfo = prevDebugInfo;
    emitter->SetCurrentDIBuilder(prevDIBuilder);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundEnumTypeDefinition& boundEnumTypeDefinition)
{
    if (debugInfo)
    {
        cmajor::symbols::EnumTypeSymbol* enumTypeSymbol = boundEnumTypeDefinition.GetEnumTypeSymbol();
        uint64_t sizeInBits = enumTypeSymbol->SizeInBits(*emitter);
        uint32_t alignInBits = enumTypeSymbol->AlignmentInBits(*emitter);
        std::vector<void*> elements;
        std::vector<cmajor::symbols::EnumConstantSymbol*> enumConstants = enumTypeSymbol->GetEnumConstants();
        for (cmajor::symbols::EnumConstantSymbol* enumConstant : enumConstants)
        {
            int64_t value = 0;
            if (enumTypeSymbol->UnderlyingType()->IsUnsignedType())
            {
                cmajor::symbols::Value* val = enumConstant->GetValue()->As(symbolTable->GetTypeByName(U"ulong"), false, nullptr, true);
                if (val)
                {
                    cmajor::symbols::ULongValue* ulongValue = static_cast<cmajor::symbols::ULongValue*>(val);
                    value = static_cast<int64_t>(ulongValue->GetValue());
                }
            }
            else
            {
                cmajor::symbols::Value* val = enumConstant->GetValue()->As(symbolTable->GetTypeByName(U"long"), false, nullptr, true);
                if (val)
                {
                    cmajor::symbols::LongValue* longValue = static_cast<cmajor::symbols::LongValue*>(val);
                    value = longValue->GetValue();
                }
            }
            elements.push_back(emitter->CreateDITypeForEnumConstant(util::ToUtf8(enumConstant->Name()), value));
        }
        soul::ast::FullSpan fullSpan = enumTypeSymbol->GetFullSpan();
        soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
        void* enumTypeDI = emitter->CreateDITypeForEnumType(util::ToUtf8(enumTypeSymbol->Name()), util::ToUtf8(enumTypeSymbol->MangledName()), 
            fullSpan, lineColLen, elements, sizeInBits, alignInBits, enumTypeSymbol->UnderlyingType()->GetDIType(*emitter));
        emitter->SetDITypeByTypeId(enumTypeSymbol->TypeId(), enumTypeDI, util::ToUtf8(enumTypeSymbol->FullName()));
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundFunction& boundFunction)
{
    if (!boundFunction.Body()) return;
    currentFunction = &boundFunction;
    handlerBlock = nullptr;
    cleanupBlock = nullptr;
    newCleanupNeeded = false;
    currentPad = nullptr;
    prevLineNumber = 0;
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    lastAlloca = nullptr;
    compoundLevel = 0;
    cleanups.clear();
    pads.clear();
    labeledStatementMap.clear();
    cmajor::symbols::FunctionSymbol* functionSymbol = boundFunction.GetFunctionSymbol();
    if (compileUnit->CodeGenerated(functionSymbol)) return;
    compileUnit->SetCodeGenerated(functionSymbol);
    void* functionType = functionSymbol->IrType(*emitter);
    function = emitter->GetOrInsertFunction(util::ToUtf8(functionSymbol->MangledName()), functionType, functionSymbol->DontThrow());
    bool setInline = false;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && functionSymbol->IsInline())
    {
        emitter->AddInlineFunctionAttribute(function);
        functionSymbol->SetLinkOnceOdrLinkage();
    }
    else if (functionSymbol->IsGeneratedFunction())
    {
        emitter->AddInlineFunctionAttribute(function);
        functionSymbol->SetLinkOnceOdrLinkage();
    }
    if (functionSymbol->HasLinkOnceOdrLinkage())
    {
        setInline = true;
        void* comdat = emitter->GetOrInsertAnyFunctionComdat(util::ToUtf8(functionSymbol->MangledName()), function);
    }
    if (functionSymbol->GetFlag(cmajor::symbols::FunctionSymbolFlags::winapi))
    {
        emitter->SetFunctionCallConventionToStdCall(function);
    }
    emitter->SetFunctionLinkage(function, setInline);
    int32_t fileIndex = -1;
    util::uuid functionId;
    if (functionSymbol->HasSource())
    {
        fileIndex = functionSymbol->FileIndex();
        functionId = functionSymbol->FunctionId();
    }
    emitter->SetFunction(function, -1, util::nil_uuid(), util::nil_uuid());
    bool hasSource = functionSymbol->HasSource();
    bool prevDebugInfo = debugInfo;
    void* prevDIBuilder = emitter->DIBuilder();
    if (!hasSource)
    {
        emitter->ResetCurrentDebugLocation();
        debugInfo = false;
        emitter->SetCurrentDIBuilder(nullptr);
        fullSpan = soul::ast::FullSpan();
        generateLineNumbers = false;
    }
    if (debugInfo)
    {
        fullSpan = functionSymbol->GetFullSpan();
        generateLineNumbers = true;
        emitter->SetInPrologue(true);
        emitter->SetCurrentDebugLocation(soul::ast::LineColLen());
        std::vector<void*> elementTypes;
        if (functionSymbol->ReturnType())
        {
            elementTypes.push_back(functionSymbol->ReturnType()->GetDIType(*emitter)); // 0'th entry is return type
        }
        else
        {
            elementTypes.push_back(symbolTable->GetTypeByName(U"void")->GetDIType(*emitter));
        }
        for (cmajor::symbols::ParameterSymbol* parameter : functionSymbol->Parameters())
        {
            cmajor::symbols::TypeSymbol* parameterType = parameter->GetType();
            elementTypes.push_back(parameterType->GetDIType(*emitter));
        }
        void* subroutineType = emitter->CreateSubroutineType(elementTypes);
        void* subprogram = nullptr;
        if (functionSymbol->IsConstructorDestructorOrNonstaticMemberFunction())
        {
            unsigned virtuality = 0;
            if (functionSymbol->IsVirtualAbstractOrOverride())
            {
                if (functionSymbol->IsAbstract())
                {
                    virtuality = emitter->GetPureVirtualVirtuality();
                }
                else
                {
                    virtuality = emitter->GetVirtualVirtuality();
                }
            }
            void* vtableHolder = nullptr;
            cmajor::symbols::Symbol* parent = functionSymbol->Parent();
            if (parent && parent->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* cls = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
                if (cls->IsPolymorphic() && cls->VmtPtrHolderClass())
                {
                    vtableHolder = cls->VmtPtrHolderClass()->GetDIType(*emitter);
                }
            }
            unsigned flags = emitter->GetFunctionFlags(functionSymbol->IsStatic(), cmajor::symbols::AccessFlag(*emitter, functionSymbol->Access()), functionSymbol->IsExplicit());
            unsigned vtableIndex = 0;
            if (functionSymbol->VmtIndex() != -1)
            {
                vtableIndex = functionSymbol->VmtIndex();
            }
            soul::ast::FullSpan fullSpan = functionSymbol->GetFullSpan();
            soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
            subprogram = emitter->CreateDIMethod(util::ToUtf8(functionSymbol->Name()), util::ToUtf8(functionSymbol->MangledName()), fullSpan, lineColLen, 
                subroutineType, virtuality, vtableIndex, vtableHolder, flags);
        }
        else
        {
            unsigned flags = cmajor::symbols::AccessFlag(*emitter, functionSymbol->Access());
            soul::ast::FullSpan fullSpan = functionSymbol->GetFullSpan();
            soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
            subprogram = emitter->CreateDIFunction(util::ToUtf8(functionSymbol->GroupName()), util::ToUtf8(functionSymbol->MangledName()), fullSpan, lineColLen,
                subroutineType, flags);
        }
        emitter->SetDISubprogram(function, subprogram);
        emitter->PushScope(subprogram);
    }
    void* entryBlock = emitter->CreateBasicBlock("entry");
    entryBasicBlock = entryBlock;
    emitter->SetCurrentBasicBlock(entryBlock);
    if (currentClass && !currentClass->IsInlineFunctionContainer())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = currentClass->GetClassTypeSymbol();
        if (!emitter->IsVmtObjectCreated(classTypeSymbol))
        {
            if (classTypeSymbol->GetCompileUnit() == compileUnit->GetCompileUnitNode() || 
                classTypeSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                classTypeSymbol->VmtObject(*emitter, true);
            }
        }
        if (!emitter->IsStaticObjectCreated(classTypeSymbol))
        {
            classTypeSymbol->StaticObject(*emitter, true);
        }
    }
    int np = functionSymbol->Parameters().size();
    for (int i = 0; i < np; ++i)
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->Parameters()[i];
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter));
        emitter->SetIrObject(parameter, allocaInst);
        lastAlloca = allocaInst;
        if (debugInfo)
        {
            soul::ast::FullSpan fullSpan = parameter->GetFullSpan();
            soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
            void* paramVar = emitter->CreateDIParameterVariable(util::ToUtf8(parameter->Name()), i + 1, fullSpan, lineColLen, 
                parameter->GetType()->GetDIType(*emitter), allocaInst);
        }
    }
    if (functionSymbol->ReturnParam())
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->ReturnParam();
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter));
        emitter->SetIrObject(parameter, allocaInst);
        lastAlloca = allocaInst;
    }
    int nlv = functionSymbol->LocalVariables().size();
    for (int i = 0; i < nlv; ++i)
    {
        cmajor::symbols::LocalVariableSymbol* localVariable = functionSymbol->LocalVariables()[i];
        void* allocaInst = emitter->CreateAlloca(localVariable->GetType()->IrType(*emitter));
        emitter->SetIrObject(localVariable, allocaInst);
        lastAlloca = allocaInst;
        if (debugInfo && localVariable->GetSpan().IsValid())
        {
            soul::ast::FullSpan fullSpan = localVariable->GetFullSpan();
            soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
            void* localVar = emitter->CreateDIAutoVariable(util::ToUtf8(localVariable->Name()), fullSpan, lineColLen,
                localVariable->GetType()->GetDIType(*emitter), allocaInst);
        }
    }
    for (int i = 0; i < np; ++i)
    {
        void* arg = emitter->GetFunctionArgument(function, i);
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->Parameters()[i];
        if (parameter->GetType()->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = classType->CopyConstructor();
            if (!copyConstructor)
            {
                copyConstructor = compileUnit->GetCopyConstructorFor(classType->TypeId());
            }
            void* copyCtorType = copyConstructor->IrType(*emitter);
            void* callee = emitter->GetOrInsertFunction(util::ToUtf8(copyConstructor->MangledName()), copyCtorType, copyConstructor->DontThrow());
            std::vector<void*> args;
            args.push_back(parameter->IrObject(*emitter));
            args.push_back(arg);
            emitter->CreateCall(copyCtorType, callee, args);
        }
        else if (parameter->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
        {
            cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = classDelegateType->CopyConstructor();
            if (!copyConstructor)
            {
                throw std::runtime_error("internal error: class delegate type has no copy constructor");
            }
            std::vector<cmajor::symbols::GenObject*> copyCtorArgs;
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter));
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::symbols::OperationFlags::none);
        }
        else if (parameter->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = interfaceType->CopyConstructor();
            if (!copyConstructor)
            {
                copyConstructor = compileUnit->GetCopyConstructorFor(interfaceType->TypeId());
            }
            std::vector<cmajor::symbols::GenObject*> copyCtorArgs;
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter));
            paramValue.SetType(interfaceType->AddPointer());
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            argumentValue.SetType(interfaceType->AddPointer());
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::symbols::OperationFlags::none);
        }
        else
        {
            emitter->CreateStore(arg, parameter->IrObject(*emitter));
        }
    }
    if (functionSymbol->ReturnParam())
    {
        void* arg = emitter->GetFunctionArgument(function, np);
        emitter->CreateStore(arg, functionSymbol->ReturnParam()->IrObject(*emitter));
    }
    for (cmajor::binder::BoundStatement* labeledStatement : boundFunction.LabeledStatements())
    {
        void* target = emitter->CreateBasicBlock(util::ToUtf8(labeledStatement->Label()));
        labeledStatementMap[labeledStatement] = target;
    }
    cmajor::binder::BoundCompoundStatement* body = boundFunction.Body();
    if (debugInfo)
    {
        emitter->SetInPrologue(false);
    }
    body->Accept(*this);
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (!lastStatement || lastStatement->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundReturnStatement || 
        lastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement &&
        destructorCallGenerated)
    {
        if (functionSymbol->ReturnType() && functionSymbol->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol && 
            !functionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            void* defaultValue = functionSymbol->ReturnType()->CreateDefaultIrValue(*emitter);
            emitter->CreateRet(defaultValue);
            lastInstructionWasRet = true;
        }
        else
        {
            emitter->CreateRetVoid();
            lastInstructionWasRet = true;
        }
    }
    if (functionSymbol->HasTry() || !cleanups.empty())
    {
        void* personalityFunction = GetPersonalityFunction();
        emitter->SetPersonalityFunction(function, personalityFunction);
    }
    if (functionSymbol->DontThrow() && !functionSymbol->HasTry() && cleanups.empty())
    {
        emitter->AddNoUnwindAttribute(function);
    }
    else
    {
        // emitter->AddUWTableAttribute(function); TODO
    }
    GenerateCodeForCleanups();
    if (debugInfo)
    {
        emitter->PopScope();
    }
    debugInfo = prevDebugInfo;
    emitter->SetCurrentDIBuilder(prevDIBuilder);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSequenceStatement);
    cmajor::binder::BoundStatement* prevSequence = sequenceSecond;
    sequenceSecond = boundSequenceStatement.Second();
    boundSequenceStatement.First()->Accept(*this);
    sequenceSecond = prevSequence;
    if (!boundSequenceStatement.Second()->Generated())
    {
        boundSequenceStatement.Second()->Accept(*this);
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement)
{
    ++compoundLevel;
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCompoundStatement);
    cmajor::binder::BoundCompoundStatement* prevBlock = currentBlock;
    currentBlock = &boundCompoundStatement;
    blockDestructionMap[currentBlock] = std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>();
    blocks.push_back(currentBlock);
    if (!prevBlock && debugInfo)
    {
        GenerateEnterFunctionCode(*currentFunction);
    }
    if (generateLineNumbers)
    {
        SetSpan(boundCompoundStatement.GetSpan());
        soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
        if (compoundLevel > 0)
        {
            void* block = emitter->CreateLexicalBlock(fullSpan, lineColLen);
        }
        emitter->SetCurrentDebugLocation(lineColLen);
    }
    int n = boundCompoundStatement.Statements().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundStatement* boundStatement = boundCompoundStatement.Statements()[i].get();
        boundStatement->Accept(*this);
    }
    ExitBlocks(prevBlock);
    blocks.pop_back();
    currentBlock = prevBlock;
    --compoundLevel;
    if (debugInfo && compoundLevel > 0)
    {
        emitter->PopScope();
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundIfStatement& boundIfStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundIfStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundIfStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    trueBlock = emitter->CreateBasicBlock("true");
    void* nextBlock = emitter->CreateBasicBlock("next");
    if (boundIfStatement.ElseS())
    {
        falseBlock = emitter->CreateBasicBlock("false");
    }
    else
    {
        falseBlock = nextBlock;
    }
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    boundIfStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundIfStatement.ThenS()->Accept(*this);
    emitter->CreateBr(nextBlock);
    if (boundIfStatement.ElseS())
    {
        emitter->SetCurrentBasicBlock(falseBlock);
        boundIfStatement.ElseS()->Accept(*this);
        emitter->CreateBr(nextBlock);
    }
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundWhileStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundWhileStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    trueBlock = emitter->CreateBasicBlock("true");
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    void* condBlock = emitter->CreateBasicBlock("cond");
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    continueTarget = condBlock;
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    boundWhileStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundWhileStatement.Statement()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundDoStatement& boundDoStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundDoStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDoStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    void* doBlock = emitter->CreateBasicBlock("do");
    void* condBlock = emitter->CreateBasicBlock("cond");
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    trueBlock = doBlock;
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    continueTarget = condBlock;
    emitter->CreateBr(doBlock);
    emitter->SetCurrentBasicBlock(doBlock);
    boundDoStatement.Statement()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    boundDoStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundForStatement& boundForStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundForStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundForStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    boundForStatement.InitS()->Accept(*this);
    void* condBlock = emitter->CreateBasicBlock("cond");
    void* actionBlock = emitter->CreateBasicBlock("action");
    void* loopBlock = emitter->CreateBasicBlock("loop");
    trueBlock = actionBlock;
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    continueTarget = loopBlock;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    boundForStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(actionBlock);
    boundForStatement.ActionS()->Accept(*this);
    emitter->CreateBr(loopBlock);
    emitter->SetCurrentBasicBlock(loopBlock);
    boundForStatement.LoopS()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundSwitchStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSwitchStatement);
    void* prevBreakTarget = breakTarget;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    breakTargetBlock = currentBlock;
    boundSwitchStatement.Condition()->Accept(*this);
    void* condition = emitter->Stack().Pop();
    void* prevDefaultDest = defaultDest;
    void* next = nullptr;
    if (boundSwitchStatement.DefaultStatement())
    {
        defaultDest = emitter->CreateBasicBlock("default");
        next = emitter->CreateBasicBlock("next");
    }
    else
    {
        defaultDest = emitter->CreateBasicBlock("next");
        next = defaultDest;
    }
    breakTarget = next;
    unsigned n = boundSwitchStatement.CaseStatements().size();
    void* switchInst = emitter->CreateSwitch(condition, defaultDest, n);
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* prevCaseMap = currentCaseMap;
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash> caseMap;
    currentCaseMap = &caseMap;
    for (unsigned i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCaseStatement>& caseS = boundSwitchStatement.CaseStatements()[i];
        void* caseDest = emitter->CreateBasicBlock("case" + std::to_string(i));
        for (const std::unique_ptr<cmajor::symbols::Value>& caseValue : caseS->CaseValues())
        {
            cmajor::symbols::IntegralValue integralCaseValue(caseValue.get());
            caseMap[integralCaseValue] = caseDest;
            emitter->AddCase(switchInst, caseValue->IrValue(*emitter), caseDest);
        }
    }
    for (unsigned i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCaseStatement>& caseS = boundSwitchStatement.CaseStatements()[i];
        caseS->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    if (boundSwitchStatement.DefaultStatement())
    {
        boundSwitchStatement.DefaultStatement()->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    emitter->SetCurrentBasicBlock(next);
    basicBlockOpen = true;
    currentCaseMap = prevCaseMap;
    defaultDest = prevDefaultDest;
    breakTargetBlock = prevBreakTargetBlock;
    breakTarget = prevBreakTarget;
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCaseStatement);
    if (!boundCaseStatement.CaseValues().empty())
    {
        cmajor::symbols::IntegralValue integralCaseValue(boundCaseStatement.CaseValues().front().get());
        auto it = currentCaseMap->find(integralCaseValue);
        if (it != currentCaseMap->cend())
        {
            void* caseDest = it->second;
            emitter->SetCurrentBasicBlock(caseDest);
            if (generateLineNumbers)
            {
                SetSpan(boundCaseStatement.GetSpan());
            }
            if (boundCaseStatement.CompoundStatement())
            {
                boundCaseStatement.CompoundStatement()->Accept(*this);
            }
        }
        else
        {
            throw cmajor::symbols::Exception("case not found", boundCaseStatement.GetFullSpan());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no cases", boundCaseStatement.GetFullSpan());
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDefaultStatement);
    if (defaultDest)
    {
        emitter->SetCurrentBasicBlock(defaultDest);
        if (generateLineNumbers)
        {
            SetSpan(boundDefaultStatement.GetSpan());
        }
        if (boundDefaultStatement.CompoundStatement())
        {
            boundDefaultStatement.CompoundStatement()->Accept(*this);
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundDefaultStatement.GetFullSpan());
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundConstructionStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundConstructionStatement);
    boundConstructionStatement.ConstructorCall()->Accept(*this);
    if (!boundConstructionStatement.ConstructorCall()->GetFunctionSymbol()->IsBasicTypeOperation())
    {
        int n = boundConstructionStatement.ConstructorCall()->Arguments().size();
        if (n > 0)
        {
            const std::unique_ptr<cmajor::binder::BoundExpression>& firstArgument = boundConstructionStatement.ConstructorCall()->Arguments()[0];
            cmajor::symbols::TypeSymbol* firstArgumentBaseType = firstArgument->GetType()->BaseType();
            if (firstArgumentBaseType->IsClassTypeSymbol())
            {
                std::lock_guard<std::recursive_mutex> lock(compileUnit->GetModule().Lock());
                if (firstArgument->GetType()->IsPointerType() && firstArgument->GetType()->RemovePointer()->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstArgumentBaseType);
                    if (classType->Destructor())
                    {
                        newCleanupNeeded = true;
                        std::unique_ptr<cmajor::binder::BoundExpression> classPtrArgument(firstArgument->Clone());
                        std::unique_ptr<cmajor::binder::BoundFunctionCall> destructorCall(new cmajor::binder::BoundFunctionCall(
                            currentBlock->GetSpan(), classType->Destructor()));
                        destructorCall->AddArgument(std::move(classPtrArgument));
                        Assert(currentBlock, "current block not set");
                        auto it = blockDestructionMap.find(currentBlock);
                        if (it != blockDestructionMap.cend())
                        {
                            std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>& destructorCallVec = it->second;
                            destructorCallVec.push_back(std::move(destructorCall));
                        }
                        else
                        {
                            Assert(false, "block destruction not found");
                        }
                    }
                }
            }
        }
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundAssignmentStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundAssignmentStatement);
    boundAssignmentStatement.AssignmentCall()->Accept(*this);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundExpressionStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundExpressionStatement);
    boundExpressionStatement.Expression()->Accept(*this);
    if (boundExpressionStatement.Expression()->HasValue())
    {
        emitter->Stack().Pop();
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundInitializationStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundInitializationStatement);
    boundInitializationStatement.InitializationExpression()->Accept(*this);
    if (boundInitializationStatement.InitializationExpression()->HasValue())
    {
        emitter->Stack().Pop();
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundEmptyStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundEmptyStatement);
    void* retType = emitter->GetIrTypeForVoid();
    std::vector<void*> paramTypes;
    void* doNothingFunType = emitter->GetIrTypeForFunction(retType, paramTypes);
    void* doNothingFun = emitter->GetOrInsertFunction("llvm.donothing", doNothingFunType, true);
    std::vector<void*> args;
    std::vector<void*> bundles;
    if (currentPad != nullptr)
    {
        bundles.push_back(currentPad->value);
    }
    if (currentPad == nullptr)
    {
        emitter->CreateCall(doNothingFunType, doNothingFun, args);
    }
    else
    {
        soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(fullSpan);
        void* callInst = emitter->CreateCallInst(doNothingFunType, doNothingFun, args, bundles, lineColLen);
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSetVmtPtrStatement);
    cmajor::binder::BoundExpression* classPtr = boundSetVmtPtrStatement.ClassPtr();
    cmajor::symbols::TypeSymbol* type = classPtr->GetType()->BaseType();
    Assert(type->IsClassTypeSymbol(), "class type expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
    int32_t vmtPtrIndex = classType->VmtPtrIndex();
    Assert(vmtPtrIndex != -1, "invalid vmt ptr index");
    classPtr->Accept(*this);
    void* classPtrValue = emitter->Stack().Pop();
    void* ptr = emitter->GetMemberVariablePtr(classType->IrType(*emitter), classPtrValue, vmtPtrIndex);
    void* vmtPtr = emitter->CreateBitCast(boundSetVmtPtrStatement.ClassType()->VmtObject(*emitter, false), emitter->GetIrTypeForVoidPtrType());
    emitter->CreateStore(vmtPtr, ptr);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement) 
{
    if (generateLineNumbers)
    {
        SetSpan(boundThrowStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundThrowStatement);
    boundThrowStatement.ThrowCallExpr()->Accept(*this);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundParameter& boundParameter)
{
    boundParameter.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundLocalVariable& boundLocalVariable)
{
    boundLocalVariable.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundMemberVariable& boundMemberVariable)
{
    boundMemberVariable.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConstant& boundConstant)
{
    boundConstant.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundEnumConstant& boundEnumConstant)
{
    boundEnumConstant.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundLiteral& boundLiteral)
{
    boundLiteral.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundTemporary& boundTemporary)
{
    boundTemporary.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundSizeOfExpression& boundSizeOfExpression)
{
    boundSizeOfExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundAddressOfExpression& boundAddressOfExpression)
{
    boundAddressOfExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundDereferenceExpression& boundDereferenceExpression)
{
    boundDereferenceExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundReferenceToPointerExpression& boundReferenceToPointerExpression)
{
    boundReferenceToPointerExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundFunctionCall& boundFunctionCall)
{
    boundFunctionCall.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundDelegateCall& boundDelegateCall)
{
    boundDelegateCall.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundClassDelegateCall& boundClassDelegateCall)
{
    boundClassDelegateCall.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConversion& boundConversion)
{
    boundConversion.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConstructExpression& boundConstructExpression)
{
    boundConstructExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConstructAndReturnTemporaryExpression& boundConstructAndReturnTemporaryExpression)
{
    boundConstructAndReturnTemporaryExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundClassOrClassDelegateConversionResult& boundClassOrClassDelegateConversionResult)
{
    boundClassOrClassDelegateConversionResult.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundIsExpression& boundIsExpression)
{
    boundIsExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
    GenJumpingBoolCode();
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundAsExpression& boundAsExpression)
{
    boundAsExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundTypeNameExpression& boundTypeNameExpression)
{
    boundTypeNameExpression.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundBitCast& boundBitCast)
{
    boundBitCast.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundFunctionPtr& boundFunctionPtr)
{
    boundFunctionPtr.Load(*emitter, cmajor::symbols::OperationFlags::none);
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundDisjunction& boundDisjunction)
{
    if (genJumpingBoolCode)
    {
        Assert(trueBlock, "true block not set");
        Assert(falseBlock, "false block not set");
        void* rightBlock = emitter->CreateBasicBlock("right");
        void* prevFalseBlock = falseBlock;
        falseBlock = rightBlock;
        boundDisjunction.Left()->Accept(*this);
        emitter->SetCurrentBasicBlock(rightBlock);
        falseBlock = prevFalseBlock;
        boundDisjunction.Right()->Accept(*this);
        boundDisjunction.DestroyTemporaries(*emitter);
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundConjunction& boundConjunction)
{
    if (genJumpingBoolCode)
    {
        Assert(trueBlock, "true block not set");
        Assert(falseBlock, "false block not set");
        void* rightBlock = emitter->CreateBasicBlock("right");
        void* prevTrueBlock = trueBlock;
        trueBlock = rightBlock;
        boundConjunction.Left()->Accept(*this);
        trueBlock = prevTrueBlock;
        emitter->SetCurrentBasicBlock(rightBlock);
        boundConjunction.Right()->Accept(*this);
        boundConjunction.DestroyTemporaries(*emitter);
    }
}

void LLVMCodeGenerator::Visit(cmajor::binder::BoundGlobalVariable& boundGlobalVariable)
{
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = boundGlobalVariable.GetGlobalVariableSymbol();
    globalVariableSymbol->CreateIrObject(*emitter);
}

void* LLVMCodeGenerator::GetGlobalStringPtr(int stringId)
{
    auto it = utf8stringMap.find(stringId);
    if (it != utf8stringMap.cend())
    {
        return it->second;
    }
    else
    {
        void* stringValue = emitter->CreateGlobalStringPtr(compileUnit->GetUtf8String(stringId));
        utf8stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* LLVMCodeGenerator::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    auto it = utf16stringMap.find(stringId);
    if (it != utf16stringMap.cend())
    {
        arrayType = it->second.second;
        return it->second.first;
    }
    else
    {
        const std::u16string& str = compileUnit->GetUtf16String(stringId);
        uint64_t length = str.length();
        std::vector<void*> wcharConstants;
        for (char16_t c : str)
        {
            wcharConstants.push_back(emitter->CreateIrValueForUShort(static_cast<uint16_t>(c)));
        }
        wcharConstants.push_back(emitter->CreateIrValueForUShort(static_cast<uint16_t>(0)));
        arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForUShort(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("wstring" + std::to_string(stringId), arrayType);
        void* stringGlobal = stringObject;
        emitter->SetPrivateLinkage(stringGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, wcharConstants, std::string());
        emitter->SetInitializer(stringGlobal, constant);
        void* stringValue = stringGlobal;
        void* arrType = arrayType;
        utf16stringMap[stringId] = std::make_pair(stringValue, arrType);
        return stringValue;
    }
}

void* LLVMCodeGenerator::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    auto it = utf32stringMap.find(stringId);
    if (it != utf32stringMap.cend())
    {
        arrayType = it->second.second;
        return it->second.first;
    }
    else
    {
        const std::u32string& str = compileUnit->GetUtf32String(stringId);
        uint64_t length = str.length();
        std::vector<void*> ucharConstants;
        for (char32_t c : str)
        {
            ucharConstants.push_back(emitter->CreateIrValueForUInt(static_cast<uint32_t>(c)));
        }
        ucharConstants.push_back(emitter->CreateIrValueForUInt(static_cast<uint32_t>(0)));
        arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForUInt(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("ustring" + std::to_string(stringId), arrayType);
        void* stringGlobal = stringObject;
        emitter->SetPrivateLinkage(stringGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, ucharConstants, std::string());
        emitter->SetInitializer(stringGlobal, constant);
        void* stringValue = stringGlobal;
        void* arrType = arrayType;
        utf32stringMap[stringId] = std::make_pair(stringValue, arrType);
        return stringValue;
    }
}

void* LLVMCodeGenerator::GetGlobalUuidConstant(int uuidId)
{
    auto it = uuidMap.find(uuidId);
    if (it != uuidMap.cend())
    {
        return it->second;
    }
    else
    {
        const util::uuid& uuid = compileUnit->GetUuid(uuidId);
        uint64_t length = uuid.static_size();
        std::vector<void*> byteConstants;
        for (util::uuid::value_type x : uuid)
        {
            byteConstants.push_back(emitter->CreateIrValueForByte(static_cast<int8_t>(x)));
        }
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForByte(), length);
        void* uuidObject = emitter->GetOrInsertGlobal("uuid" + std::to_string(uuidId), arrayType);
        void* uuidGlobal = uuidObject;
        emitter->SetPrivateLinkage(uuidGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, byteConstants, std::string());
        emitter->SetInitializer(uuidGlobal, constant);
        void* uuidValue = uuidGlobal;
        uuidMap[uuidId] = uuidValue;
        return uuidValue;
    }
}

void LLVMCodeGenerator::SetSpan(const soul::ast::Span& span)
{
    if (!span.IsValid()) return;
    if (inSetLineOrEntryCode) return;
    fullSpan.span = span;
    SetLineNumber(cmajor::symbols::GetLineNumber(fullSpan));
}

void LLVMCodeGenerator::SetLineNumber(int32_t lineNumber)
{
    if (prevLineNumber == lineNumber) return;
    prevLineNumber = lineNumber;
    cmajor::binder::BoundStatement* setLineNumberStatement = currentFunction->GetLineCode();
    if (setLineNumberStatement)
    {
        bool prevGenJumpingBoolCode = genJumpingBoolCode;
        genJumpingBoolCode = false;
        emitter->BeginSubstituteLineNumber(lineNumber);
        bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
        inSetLineOrEntryCode = true;
        setLineNumberStatement->Accept(*this);
        inSetLineOrEntryCode = prevSetLineOrEntryCode;
        emitter->EndSubstituteLineNumber();
        genJumpingBoolCode = prevGenJumpingBoolCode;
    }
}

void* LLVMCodeGenerator::HandlerBlock()
{ 
    return handlerBlock;
}

void* LLVMCodeGenerator::CleanupBlock()
{ 
    return cleanupBlock;
}

bool LLVMCodeGenerator::NewCleanupNeeded()
{ 
    return newCleanupNeeded;
}

bool LLVMCodeGenerator::InTryBlock() const
{ 
    return false; 
}

int LLVMCodeGenerator::CurrentTryBlockId() const
{ 
    return 0; 

}
void LLVMCodeGenerator::CreateCleanup()
{ 
}

std::string LLVMCodeGenerator::GetSourceFilePath(int fileIndex, const util::uuid& moduleId)
{ 
    return cmajor::symbols::GetSourceFilePath(fileIndex, moduleId);
}

cmajor::ir::Pad* LLVMCodeGenerator::CurrentPad()
{
    return currentPad;
}

void* LLVMCodeGenerator::CreateClassDIType(void* classPtr)
{
    cmajor::symbols::ClassTypeSymbol* cls = static_cast<cmajor::symbols::ClassTypeSymbol*>(classPtr);
    return cls->CreateDIType(*emitter);
}

int LLVMCodeGenerator::Install(const std::string& str)
{
    return compileUnit->Install(str);
}

int LLVMCodeGenerator::Install(const std::u16string& str)
{
    return compileUnit->Install(str);
}

int LLVMCodeGenerator::Install(const std::u32string& str)
{
    return compileUnit->Install(str);
}

void LLVMCodeGenerator::GenJumpingBoolCode()
{
    if (!genJumpingBoolCode) return;
    Assert(trueBlock, "true block not set");
    Assert(falseBlock, "false block not set");
    void* cond = emitter->Stack().Pop();
    if (sequenceSecond)
    {
        genJumpingBoolCode = false;
        sequenceSecond->SetGenerated();
        sequenceSecond->Accept(*this);
        genJumpingBoolCode = true;
    }
    emitter->CreateCondBr(cond, trueBlock, falseBlock);
}

void LLVMCodeGenerator::ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock)
{
    bool createBasicBlock = false;
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!currentFunction->Body()->Statements().empty())
    {
        lastStatement = currentFunction->Body()->Statements().back().get();
    }
    cmajor::binder::BoundStatement* currentBlockLastStatement = nullptr;
    if (currentBlock && !currentBlock->Statements().empty())
    {
        currentBlockLastStatement = currentBlock->Statements().back().get();
    }
    if (lastStatement && currentBlockLastStatement && lastStatement == currentBlockLastStatement && 
        currentBlockLastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement)
    {
        createBasicBlock = true;
    }
    int n = blocks.size();
    for (int i = n - 1; i >= 0; --i)
    {
        cmajor::binder::BoundCompoundStatement* block = blocks[i];
        if (block == targetBlock)
        {
            break;
        }
        auto it = blockDestructionMap.find(block);
        if (it != blockDestructionMap.cend())
        {
            std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>& destructorCallVec = it->second;
            int nd = destructorCallVec.size();
            for (int i = nd - 1; i >= 0; --i)
            {
                std::unique_ptr<cmajor::binder::BoundFunctionCall>& destructorCall = destructorCallVec[i];
                if (destructorCall)
                {
                    if (createBasicBlock)
                    {
                        void* nextBlock = emitter->CreateBasicBlock("next");
                        if (!lastInstructionWasRet)
                        {
                            emitter->CreateBr(nextBlock);
                        }
                        emitter->SetCurrentBasicBlock(nextBlock);
                        createBasicBlock = false;
                    }
                    bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
                    inSetLineOrEntryCode = true;
                    destructorCall->Accept(*this);
                    inSetLineOrEntryCode = prevSetLineOrEntryCode;
                    destructorCallGenerated = true;
                    newCleanupNeeded = true;
                }
            }
        }
    }
}

void LLVMCodeGenerator::SetTarget(cmajor::binder::BoundStatement* labeledStatement)
{
    if (labeledStatement->Label().empty()) return;
    auto it = labeledStatementMap.find(labeledStatement);
    if (it != labeledStatementMap.cend())
    {
        void* target = it->second;
        emitter->CreateBr(target);
        emitter->SetCurrentBasicBlock(target);
    }
    else
    {
        throw cmajor::symbols::Exception("target for labeled statement not found", labeledStatement->GetFullSpan());
    }
}

void LLVMCodeGenerator::GenerateEnterFunctionCode(cmajor::binder::BoundFunction& boundFunction)
{
    const std::vector<std::unique_ptr<cmajor::binder::BoundStatement>>& enterCode = boundFunction.EnterCode();
    if (enterCode.empty()) return;
    bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
    inSetLineOrEntryCode = true;
    cmajor::symbols::LocalVariableSymbol* traceEntryVar = boundFunction.GetFunctionSymbol()->TraceEntryVar();
    void* traceEntryAlloca = emitter->CreateAlloca(traceEntryVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(traceEntryVar, traceEntryAlloca);
    cmajor::symbols::LocalVariableSymbol* traceGuardVar = boundFunction.GetFunctionSymbol()->TraceGuardVar();
    void* traceGuardAlloca = emitter->CreateAlloca(traceGuardVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(traceGuardVar, traceGuardAlloca);
    lastAlloca = traceGuardAlloca;
    for (const auto& statement : enterCode)
    {
        statement->Accept(*this);
    }
    inSetLineOrEntryCode = prevSetLineOrEntryCode;
}

} // namespace cmajor::llvm
