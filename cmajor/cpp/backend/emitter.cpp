// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.cpp.backend.emitter;

import cmajor.debug;

namespace cmajor::cpp::backend {

CppEmitter::CppEmitter(cmajor::ir::EmittingContext* emittingContext_) :
    emittingContext(emittingContext_), emittingDelegate(nullptr), context(nullptr), compileUnit(nullptr), currentFunction(nullptr),
    objectPointer(nullptr), substituteLineNumber(false), currentLineNumber(-1), boundCompileUnit(nullptr)
{
    SetStack(&stack);
}

void CppEmitter::SetEmittingDelegate(cmajor::ir::EmittingDelegate* emittingDelegate_)
{
    emittingDelegate = emittingDelegate_;
}

void* CppEmitter::GetIrTypeForBool()
{
    return context->GetBoolType();
}

void* CppEmitter::GetIrTypeForSByte()
{
    return context->GetSByteType();
}

void* CppEmitter::GetIrTypeForByte()
{
    return context->GetByteType();
}

void* CppEmitter::GetIrTypeForShort()
{
    return context->GetShortType();
}

void* CppEmitter::GetIrTypeForUShort()
{
    return context->GetUShortType();
}

void* CppEmitter::GetIrTypeForInt()
{
    return context->GetIntType();
}

void* CppEmitter::GetIrTypeForUInt()
{
    return context->GetUIntType();
}

void* CppEmitter::GetIrTypeForLong()
{
    return context->GetLongType();
}

void* CppEmitter::GetIrTypeForULong()
{
    return context->GetULongType();
}

void* CppEmitter::GetIrTypeForFloat()
{
    return context->GetFloatType();
}

void* CppEmitter::GetIrTypeForDouble()
{
    return context->GetDoubleType();
}

void* CppEmitter::GetIrTypeForChar()
{
    return context->GetCharType();
}

void* CppEmitter::GetIrTypeForWChar()
{
    return context->GetWCharType();
}

void* CppEmitter::GetIrTypeForUChar()
{
    return context->GetUCharType();
}

void* CppEmitter::GetIrTypeForVoid()
{
    return context->GetVoidType();
}

void* CppEmitter::GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::cpp::ir::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::cpp::ir::Type*>(paramType));
    }
    return context->GetFunctionType(static_cast<cmajor::cpp::ir::Type*>(retType), parameterTypes);
}

void* CppEmitter::GetIrTypeForVariableParamFunction(void* retType)
{
    return nullptr;
}

void* CppEmitter::GetIrTypeByTypeId(const util::uuid& typeId)
{
    auto it = irTypeTypeIdMap.find(typeId);
    if (it != irTypeTypeIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CppEmitter::SetIrTypeByTypeId(const util::uuid& typeId, void* irType)
{
    irTypeTypeIdMap[typeId] = static_cast<cmajor::cpp::ir::Type*>(irType);
}

void* CppEmitter::GetIrTypeForArrayType(void* elementType, int64_t size)
{
    return context->GetArrayType(static_cast<cmajor::cpp::ir::Type*>(elementType), size);
}

void* CppEmitter::GetIrTypeForClassType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::cpp::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::cpp::ir::Type*>(elementType));
    }
    return context->GetStructureType(memberTypes);
}

void* CppEmitter::CreateFwdIrTypeForClassType()
{
    return context->CreateStructureType();
}

void CppEmitter::SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::cpp::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::cpp::ir::Type*>(elementType));
    }
    cmajor::cpp::ir::StructureType* structureType = static_cast<cmajor::cpp::ir::StructureType*>(forwardDeclaredType);
    structureType->SetMemberTypes(memberTypes);
}

void* CppEmitter::GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::cpp::ir::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::cpp::ir::Type*>(paramType));
    }
    return context->GetPtrType(context->GetFunctionType(static_cast<cmajor::cpp::ir::Type*>(retType), parameterTypes));
}

void* CppEmitter::GetIrTypeForVoidPtrType()
{
    return context->GetPtrType(context->GetVoidType());
}

void* CppEmitter::GetIrTypeForStructType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::cpp::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::cpp::ir::Type*>(elementType));
    }
    return context->GetStructureType(memberTypes);
}

void* CppEmitter::GetIrTypeForPtrType(void* baseIrType)
{
    return context->GetPtrType(static_cast<cmajor::cpp::ir::Type*>(baseIrType));
}

std::string CppEmitter::GetIrTypeName(void* irType)
{
    return static_cast<cmajor::cpp::ir::Type*>(irType)->Name();
}

std::string CppEmitter::MakeVmtVariableName(const std::string& vmtObjectName)
{
    return "__global_" + vmtObjectName;
}

void* CppEmitter::CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults)
{
    std::vector<cmajor::cpp::ir::Value*> arrayOfConstants;
    for (void* constant : arrayOfDefaults)
    {
        arrayOfConstants.push_back(static_cast<cmajor::cpp::ir::Value*>(constant));
    }
    cmajor::cpp::ir::Type* arrayType = static_cast<cmajor::cpp::ir::Type*>(arrayIrType);
    return context->GetArrayValue(arrayType, arrayOfConstants, std::string());
}

void* CppEmitter::CreateDefaultIrValueForBool()
{
    return context->GetDefaultBoolValue();
}

void* CppEmitter::CreateDefaultIrValueForSByte()
{
    return context->GetDefaultSByteValue();
}

void* CppEmitter::CreateDefaultIrValueForByte()
{
    return context->GetDefaultByteValue();
}

void* CppEmitter::CreateDefaultIrValueForShort()
{
    return context->GetDefaultShortValue();
}

void* CppEmitter::CreateDefaultIrValueForUShort()
{
    return context->GetDefaultUShortValue();
}

void* CppEmitter::CreateDefaultIrValueForInt()
{
    return context->GetDefaultIntValue();
}

void* CppEmitter::CreateDefaultIrValueForUInt()
{
    return context->GetDefaultUIntValue();
}

void* CppEmitter::CreateDefaultIrValueForLong()
{
    return context->GetDefaultLongValue();
}

void* CppEmitter::CreateDefaultIrValueForULong()
{
    return context->GetDefaultULongValue();
}

void* CppEmitter::CreateDefaultIrValueForFloat()
{
    return context->GetDefaultFloatValue();
}

void* CppEmitter::CreateDefaultIrValueForDouble()
{
    return context->GetDefaultDoubleValue();
}

void* CppEmitter::CreateDefaultIrValueForChar()
{
    return context->GetDefaultByteValue();
}

void* CppEmitter::CreateDefaultIrValueForWChar()
{
    return context->GetDefaultWCharValue();
}

void* CppEmitter::CreateDefaultIrValueForUChar()
{
    return context->GetDefaultUCharValue();
}

void* CppEmitter::CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers)
{
    std::vector<cmajor::cpp::ir::Value*> arrayOfDefaults;
    for (void* constant : defaultMembers)
    {
        arrayOfDefaults.push_back(static_cast<cmajor::cpp::ir::Value*>(constant));
    }
    return context->GetStructureValue(static_cast<cmajor::cpp::ir::StructureType*>(irType), arrayOfDefaults);
}

void* CppEmitter::CreateDefaultIrValueForDelegateType(void* irType)
{
    return context->GetNullValue(static_cast<cmajor::cpp::ir::PtrType*>(irType));
}

void* CppEmitter::CreateDefaultIrValueForVoidPtrType()
{
    return context->GetNullValue(static_cast<cmajor::cpp::ir::PtrType*>(context->GetPtrType(context->GetVoidType())));
}

void* CppEmitter::CreateDefaultIrValueForDerivedType(void* irType)
{
    return static_cast<cmajor::cpp::ir::Type*>(irType)->DefaultValue();
}

void* CppEmitter::CreateDefaultIrValueForPtrType(void* irType)
{
    return context->GetNullValue(static_cast<cmajor::cpp::ir::PtrType*>(irType));
}

void* CppEmitter::CreateIrValueForBool(bool value)
{
    return context->GetBoolValue(value);
}

void* CppEmitter::CreateIrValueForSByte(int8_t value)
{
    return context->GetSByteValue(value);
}

void* CppEmitter::CreateIrValueForByte(uint8_t value)
{
    return context->GetByteValue(value);
}

void* CppEmitter::CreateIrValueForShort(int16_t value)
{
    return context->GetShortValue(value);
}

void* CppEmitter::CreateIrValueForUShort(uint16_t value)
{
    return context->GetUShortValue(value);
}

void* CppEmitter::CreateIrValueForInt(int32_t value)
{
    if (substituteLineNumber)
    {
        return context->GetIntValue(currentLineNumber);
    }
    else
    {
        return context->GetIntValue(value);
    }
}

void* CppEmitter::CreateIrValueForUInt(uint32_t value)
{
    return context->GetUIntValue(value);
}

void* CppEmitter::CreateIrValueForLong(int64_t value)
{
    return context->GetLongValue(value);
}

void* CppEmitter::CreateIrValueForULong(uint64_t value)
{
    return context->GetULongValue(value);
}

void* CppEmitter::CreateIrValueForFloat(float value)
{
    return context->GetFloatValue(value);
}

void* CppEmitter::CreateIrValueForDouble(double value)
{
    return context->GetDoubleValue(value);
}

void* CppEmitter::CreateIrValueForChar(uint8_t value)
{
    return context->GetByteValue(value);
}

void* CppEmitter::CreateIrValueForWChar(uint16_t value)
{
    return context->GetWCharValue(value);
}

void* CppEmitter::CreateIrValueForUChar(uint32_t value)
{
    return context->GetUCharValue(value);
}

void* CppEmitter::CreateIrValueForWString(void* type, void* wstringConstant)
{
    return static_cast<cmajor::cpp::ir::Value*>(wstringConstant);
}

void* CppEmitter::CreateIrValueForUString(void* type, void* ustringConstant)
{
    return static_cast<cmajor::cpp::ir::Value*>(ustringConstant);
}

void* CppEmitter::CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elementConstants, const std::string& prefix)
{
    std::vector<cmajor::cpp::ir::Value*> elements;
    for (void* elementConstant : elementConstants)
    {
        elements.push_back(static_cast<cmajor::cpp::ir::ConstantValue*>(elementConstant));
    }
    return context->GetArrayValue(static_cast<cmajor::cpp::ir::ArrayType*>(arrayIrType), elements, prefix);
}

void* CppEmitter::CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants)
{
    std::vector<cmajor::cpp::ir::Value*> memberConstants;
    for (void* elementConstant : elementConstants)
    {
        memberConstants.push_back(static_cast<cmajor::cpp::ir::Value*>(elementConstant));
    }
    return context->GetStructureValue(static_cast<cmajor::cpp::ir::StructureType*>(structIrType), memberConstants);
}

void* CppEmitter::CreateIrValueForUuid(void* type, void* uuidConstant)
{
    cmajor::cpp::ir::Value* arg = context->CreatePtrOffset(static_cast<cmajor::cpp::ir::Value*>(uuidConstant), context->GetLongValue(0));
    return context->CreateBitCast(arg, context->GetPtrType(context->GetVoidType()));
}

void* CppEmitter::GetConversionValue(void* type, void* from)
{
    return context->GetConversionValue(static_cast<cmajor::cpp::ir::Type*>(type), static_cast<cmajor::cpp::ir::ConstantValue*>(from));
}

void* CppEmitter::CreateGlobalStringPtr(const std::string& stringValue)
{
    return context->CreateGlobalStringPtr(stringValue);
}

void* CppEmitter::CreateGlobalWStringPtr(const std::u16string& stringValue)
{
    return context->CreateGlobalWStringPtr(stringValue);
}

void* CppEmitter::CreateGlobalUStringPtr(const std::u32string& stringValue)
{
    return context->CreateGlobalUStringPtr(stringValue);
}

void* CppEmitter::GetGlobalStringPtr(int stringId)
{
    return emittingDelegate->GetGlobalStringPtr(stringId);
}

void* CppEmitter::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalWStringConstant(stringId, arrayType);
}

void* CppEmitter::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalUStringConstant(stringId, arrayType);
}

void* CppEmitter::GetGlobalUuidConstant(int uuidId)
{
    return emittingDelegate->GetGlobalUuidConstant(uuidId);
}

void* CppEmitter::CreateDITypeForBool()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForSByte()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForByte()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForShort()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForUShort()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForInt()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForUInt()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForLong()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForULong()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForFloat()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForDouble()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForChar()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForWChar()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForUChar()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForVoid()
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements)
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForEnumConstant(const std::string& name, int64_t value)
{
    return nullptr;
}

void* CppEmitter::CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
    const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType)
{
    return nullptr;
}

void* CppEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName, 
    const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

uint64_t CppEmitter::GetOffsetInBits(void* classIrType, int layoutIndex)
{
    return uint64_t();
}

void* CppEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan, 
    const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    return nullptr;
}

void CppEmitter::MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId)
{
}

void* CppEmitter::GetDITypeByTypeId(const util::uuid& typeId) const
{
    return nullptr;
}

void CppEmitter::SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName)
{
}

void* CppEmitter::GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId)
{
    return nullptr;
}

void CppEmitter::SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType)
{
}

void* CppEmitter::CreateDIMemberType(void* scope, const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType)
{
    return nullptr;
}

void* CppEmitter::CreateConstDIType(void* diType)
{
    return nullptr;
}

void* CppEmitter::CreateLValueRefDIType(void* diType)
{
    return nullptr;
}

void* CppEmitter::CreateRValueRefDIType(void* diType)
{
    return nullptr;
}

void* CppEmitter::CreatePointerDIType(void* diType)
{
    return nullptr;
}

void* CppEmitter::CreateUnspecifiedDIType(const std::string& name)
{
    return nullptr;
}

void CppEmitter::MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className)
{
}

uint64_t CppEmitter::GetSizeInBits(void* irType)
{
    return 0;
}

uint64_t CppEmitter::GetAlignmentInBits(void* irType)
{
    return 0;
}

void CppEmitter::SetCurrentDebugLocation(const soul::ast::LineColLen& lineColLen)
{
}

void CppEmitter::SetCurrentDebugLocation(const soul::ast::Span& span)
{
}

void* CppEmitter::GetArrayBeginAddress(void* arrayType, void* arrayPtr)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(arrayPtr), context->GetLongValue(0));
}

void* CppEmitter::GetArrayEndAddress(void* arrayType, void* arrayPtr, uint64_t size)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(arrayPtr), context->GetLongValue(size));
}

void* CppEmitter::CreateBasicBlock(const std::string& name)
{
    if (name == "cleanup")
    {
        return currentFunction->CreateCleanupBasicBlock(*context);
    }
    else
    {
        return currentFunction->CreateBasicBlock(name, *context);
    }
}

void* CppEmitter::CreateIncludeBasicBlockInstruction(void* basicBlock)
{
    return context->CreateIncludeBasicBlockInstruction(static_cast<cmajor::cpp::ir::BasicBlock*>(basicBlock));
}

void CppEmitter::PushParentBlock()
{
    context->PushParent();
}

void CppEmitter::PopParentBlock()
{
    context->PopParent();
}

void CppEmitter::SetHandlerBlock(void* tryBlock, void* catchBlock)
{
    context->SetHandlerBlock(static_cast<cmajor::cpp::ir::BasicBlock*>(tryBlock), static_cast<cmajor::cpp::ir::BasicBlock*>(catchBlock));
}

void CppEmitter::SetCleanupBlock(void* cleanupBlock)
{
    context->SetCleanupBlock(static_cast<cmajor::cpp::ir::BasicBlock*>(cleanupBlock));
}

int CppEmitter::GetBasicBlockId(void* basicBlock)
{
    return static_cast<cmajor::cpp::ir::BasicBlock*>(basicBlock)->Id();
}

void CppEmitter::CreateBr(void* targetBasicBlock)
{
    context->CreateJump(static_cast<cmajor::cpp::ir::BasicBlock*>(targetBasicBlock));
}

void* CppEmitter::CurrentBasicBlock() const
{
    return context->GetCurrentBasicBlock();
}

void CppEmitter::SetCurrentBasicBlock(void* basicBlock)
{
    context->SetCurrentBasicBlock(static_cast<cmajor::cpp::ir::BasicBlock*>(basicBlock));
}

void CppEmitter::CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock)
{
    context->CreateBranch(static_cast<cmajor::cpp::ir::Value*>(cond), static_cast<cmajor::cpp::ir::BasicBlock*>(trueBasicBlock), static_cast<cmajor::cpp::ir::BasicBlock*>(falseBasicBlock));
}

void* CppEmitter::CreateArrayIndexAddress(void* arrayType, void* arrayPtr, void* elementType, void* index)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(arrayPtr), static_cast<cmajor::cpp::ir::Value*>(index));
}

void CppEmitter::CreateStore(void* value, void* ptr)
{
    context->CreateStore(static_cast<cmajor::cpp::ir::Value*>(value), static_cast<cmajor::cpp::ir::Value*>(ptr));
}

void* CppEmitter::CreateLoad(void* type, void* ptr)
{
    return context->CreateLoad(static_cast<cmajor::cpp::ir::Value*>(ptr));
}

void* CppEmitter::CreateAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateUDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateSDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateURem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateSRem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateAnd(void* left, void* right)
{
    return context->CreateAnd(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateOr(void* left, void* right)
{
    return context->CreateOr(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateXor(void* left, void* right)
{
    return context->CreateXor(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateShl(void* left, void* right)
{
    return context->CreateShl(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateAShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateLShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateICmpEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFCmpOEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateICmpULT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateICmpSLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateFCmpOLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

void* CppEmitter::CreateSExt(void* operand, void* destinationType)
{
    return context->CreateSignExtend(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateZExt(void* operand, void* destinationType)
{
    return context->CreateZeroExtend(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateFPExt(void* operand, void* destinationType)
{
    return operand;
}

void* CppEmitter::CreateTrunc(void* operand, void* destinationType)
{
    return context->CreateTruncate(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateFPTrunc(void* operand, void* destinationType)
{
    return context->CreateBitCast(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateBitCast(void* operand, void* destinationType)
{
    return context->CreateBitCast(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateUIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateSIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateFPToUI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateFPToSI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::cpp::ir::Value*>(operand), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateIntToPtr(void* intValue, void* destinationType)
{
    return context->CreateIntToPtr(static_cast<cmajor::cpp::ir::Value*>(intValue), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreatePtrToInt(void* ptrValue, void* destinationType)
{
    return context->CreatePtrToInt(static_cast<cmajor::cpp::ir::Value*>(ptrValue), static_cast<cmajor::cpp::ir::Type*>(destinationType));
}

void* CppEmitter::CreateNot(void* value)
{
    return context->CreateNot(static_cast<cmajor::cpp::ir::Value*>(value));
}

void* CppEmitter::CreateNeg(void* value)
{
    return context->CreateNeg(static_cast<cmajor::cpp::ir::Value*>(value));
}

void* CppEmitter::CreateFNeg(void* value)
{
    cmajor::cpp::ir::Value* val = static_cast<cmajor::cpp::ir::Value*>(value);
    if (val->GetType(*context)->Id() == cmajor::cpp::ir::doubleTypeId)
    {
        cmajor::cpp::ir::ConstantValue* minusOne = context->GetDoubleValue(-1.0);
        return context->CreateMul(minusOne, val);
    }
    else if (val->GetType(*context)->Id() == cmajor::cpp::ir::floatTypeId)
    {
        cmajor::cpp::ir::ConstantValue* minusOne = context->GetFloatValue(-1.0f);
        return context->CreateMul(minusOne, val);
    }
    else
    {
        throw std::runtime_error("invalid FNeg operand type");
    }
}

void* CppEmitter::CreateNop()
{
    return context->CreateNop();
}

void* CppEmitter::CreateSave()
{
    return nullptr;
}

void* CppEmitter::CreateBeginTry()
{
    return context->CreateBeginTry();
}

void* CppEmitter::CreateEndTry(void* nextDest)
{
    return context->CreateEndTry(static_cast<cmajor::cpp::ir::BasicBlock*>(nextDest));
}

void* CppEmitter::CreateBeginCatch()
{
    return context->CreateBeginCatch();
}

void* CppEmitter::CreateEndCatch(void* nextDest)
{
    return context->CreateEndCatch(static_cast<cmajor::cpp::ir::BasicBlock*>(nextDest));
}

std::string CppEmitter::GetVmtObjectName(void* symbol) const
{
    auto it = vmtObjectNameMap.find(symbol);
    if (it != vmtObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void CppEmitter::SetVmtObjectName(void* symbol, const std::string& vmtObjectName)
{
    vmtObjectNameMap[symbol] = vmtObjectName;
}

std::string CppEmitter::GetImtArrayObjectName(void* symbol) const
{
    auto it = imtArrayObjectNameMap.find(symbol);
    if (it != imtArrayObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void CppEmitter::SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName)
{
    imtArrayObjectNameMap[symbol] = imtArrayObjectName;
}

void* CppEmitter::GetVmtObjectType(void* symbol) const
{
    auto it = vmtObjectTypeMap.find(symbol);
    if (it != vmtObjectTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CppEmitter::SetVmtObjectType(void* symbol, void* vmtObjectType)
{
    vmtObjectTypeMap[symbol] = static_cast<cmajor::cpp::ir::ArrayType*>(vmtObjectType);
}

void* CppEmitter::GetStaticObjectType(void* symbol) const
{
    auto it = staticTypeMap.find(symbol);
    if (it != staticTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CppEmitter::SetStaticObjectType(void* symbol, void* type)
{
    staticTypeMap[symbol] = static_cast<cmajor::cpp::ir::StructureType*>(type);
}

std::string CppEmitter::GetStaticObjectName(void* symbol) const
{
    auto it = staticObjectNameMap.find(symbol);
    if (it != staticObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void CppEmitter::SetStaticObjectName(void* symbol, const std::string& staticObjectName)
{
    staticObjectNameMap[symbol] = staticObjectName;
}

void* CppEmitter::GetOrInsertGlobal(const std::string& name, void* type)
{
    return context->GetOrInsertGlobal(name, static_cast<cmajor::cpp::ir::Type*>(type));
}

void* CppEmitter::GetOrInsertAnyComdat(const std::string& name, void* global)
{
    static_cast<cmajor::cpp::ir::GlobalVariable*>(global)->SetLinkOnce();
    return nullptr;
}

void* CppEmitter::GetOrInsertAnyFunctionComdat(const std::string& name, void* function)
{
    return nullptr;
}

void* CppEmitter::GetOrInsertFunction(const std::string& name, void* type, bool nothrow)
{
    cmajor::cpp::ir::Function* function = compileUnit->GetOrInsertFunction(name, static_cast<cmajor::cpp::ir::FunctionType*>(type));
    if (nothrow)
    {
        function->SetNothrow();
    }
    return function;
}


void CppEmitter::SetInitializer(void* global, void* initializer)
{
    cmajor::cpp::ir::GlobalVariable* globalVar = static_cast<cmajor::cpp::ir::GlobalVariable*>(global);
    globalVar->SetInitializer(static_cast<cmajor::cpp::ir::ConstantValue*>(initializer));
}

void CppEmitter::SetPrivateLinkage(void* global)
{
}

bool CppEmitter::IsVmtObjectCreated(void* symbol) const
{
    return vmtObjectCreatedSet.find(symbol) != vmtObjectCreatedSet.cend();
}

void CppEmitter::SetVmtObjectCreated(void* symbol)
{
    vmtObjectCreatedSet.insert(symbol);
}

bool CppEmitter::IsStaticObjectCreated(void* symbol) const
{
    return staticObjectCreatedSet.find(symbol) != staticObjectCreatedSet.cend();
}

void CppEmitter::SetStaticObjectCreated(void* symbol)
{
    staticObjectCreatedSet.insert(symbol);
}

void* CppEmitter::HandlerBlock()
{
    return emittingDelegate->HandlerBlock();
}

void* CppEmitter::CleanupBlock()
{
    return emittingDelegate->CleanupBlock();
}

bool CppEmitter::NewCleanupNeeded()
{
    return false;
}

void CppEmitter::CreateCleanup()
{
    return emittingDelegate->CreateCleanup();
}

cmajor::ir::Pad* CppEmitter::CurrentPad()
{
    return nullptr;
}

void* CppEmitter::CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* CppEmitter::CreateCleanupPad(const std::vector<void*>& args)
{
    return nullptr;
}

void* CppEmitter::CreateCleanupRet(void* cleanupPad, void* unwindTarget)
{
    return nullptr;
}

void* CppEmitter::CreateCatchRet(void* catchPad, void* returnTarget)
{
    return nullptr;
}

void* CppEmitter::CreateCatchSwitch(void* unwindBlock)
{
    return nullptr;
}

void* CppEmitter::CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock)
{
    return nullptr;
}

void CppEmitter::AddHandlerToCatchSwitch(void* catchSwitch, void* target)
{
}

void* CppEmitter::CreateCatchPad(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* CppEmitter::CreateClassDIType(void* classPtr)
{
    return nullptr;
}

void* CppEmitter::CreateCall(void* functionType, void* callee, const std::vector<void*>& args)
{
    std::vector<cmajor::cpp::ir::Value*> argInsts;
    for (void* arg : args)
    {
        cmajor::cpp::ir::Value* argument = static_cast<cmajor::cpp::ir::Value*>(arg);
        argInsts.push_back(context->CreateArg(argument));
    }
    cmajor::cpp::ir::Value* calleeValue = static_cast<cmajor::cpp::ir::Value*>(callee);
    return context->CreateCall(calleeValue, argInsts);
}

void* CppEmitter::CreateCallInst(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen)
{
    std::vector<cmajor::cpp::ir::Value*> argInsts;
    for (void* arg : args)
    {
        cmajor::cpp::ir::Value* argument = static_cast<cmajor::cpp::ir::Value*>(arg);
        argInsts.push_back(context->CreateArg(argument));
    }
    cmajor::cpp::ir::Value* calleeValue = static_cast<cmajor::cpp::ir::Value*>(callee);
    return context->CreateCall(calleeValue, argInsts);
}

void* CppEmitter::CreateCallInstToBasicBlock(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, void* basicBlock, 
    const soul::ast::LineColLen& lineColLen)
{
    std::vector<cmajor::cpp::ir::Value*> argInsts;
    void* prevBasicBlock = context->GetCurrentBasicBlock();
    SetCurrentBasicBlock(basicBlock);
    for (void* arg : args)
    {
        cmajor::cpp::ir::Value* argument = static_cast<cmajor::cpp::ir::Value*>(arg);
        argInsts.push_back(context->CreateArg(argument));
    }
    cmajor::cpp::ir::Value* calleeValue = static_cast<cmajor::cpp::ir::Value*>(callee);
    cmajor::cpp::ir::Instruction* callInst = context->CreateCall(calleeValue, argInsts);
    SetCurrentBasicBlock(prevBasicBlock);
    return callInst;
}

void* CppEmitter::CreateInvoke(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args)
{
    std::vector<cmajor::cpp::ir::Value*> argInsts;
    for (void* arg : args)
    {
        cmajor::cpp::ir::Value* argument = static_cast<cmajor::cpp::ir::Value*>(arg);
        argInsts.push_back(context->CreateArg(argument));
    }
    cmajor::cpp::ir::Value* calleeValue = static_cast<cmajor::cpp::ir::Value*>(callee);
    cmajor::cpp::ir::BasicBlock* normalBlockNext = static_cast<cmajor::cpp::ir::BasicBlock*>(normalBlock);
    cmajor::cpp::ir::BasicBlock* unwindBlockNext = static_cast<cmajor::cpp::ir::BasicBlock*>(unwindBlock);
    return context->CreateInvoke(calleeValue, argInsts, normalBlockNext, unwindBlockNext);
}

void* CppEmitter::CreateInvokeInst(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, 
    const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen)
{
    return CreateInvoke(functionType, callee, normalBlock, unwindBlock, args);
}

void* CppEmitter::DIBuilder()
{
    return nullptr;
}

void CppEmitter::SetCurrentDIBuilder(void* diBuilder_)
{
}

void* CppEmitter::GetObjectFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(classDelegatePtr), context->GetLongValue(0));
}

void* CppEmitter::GetDelegateFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(classDelegatePtr), context->GetLongValue(1));
}

void* CppEmitter::GetObjectFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::cpp::ir::Value* addr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(interfaceTypePtr), context->GetLongValue(0));
    return context->CreateLoad(addr);
}

void* CppEmitter::GetObjectPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(interfaceTypePtr), context->GetLongValue(0));
}

void* CppEmitter::GetImtPtrPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(interfaceTypePtr), context->GetLongValue(1));
}

void* CppEmitter::GetImtPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::cpp::ir::Value* interfacePtrPtr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(interfaceTypePtr), context->GetLongValue(1));
    cmajor::cpp::ir::Value* interfacePtr = context->CreateLoad(interfacePtrPtr);
    return context->CreateBitCast(interfacePtr, context->GetPtrType(context->GetPtrType(context->GetVoidType())));
}

void* CppEmitter::GetInterfaceMethod(void* interfaceType, void* imtPtr, int32_t methodIndex, void* interfaceMethodType)
{
    cmajor::cpp::ir::Value* methodPtrPtr = context->CreatePtrOffset(static_cast<cmajor::cpp::ir::Value*>(imtPtr), context->GetLongValue(methodIndex));
    cmajor::cpp::ir::Value* methodPtr = context->CreateLoad(methodPtrPtr);
    cmajor::cpp::ir::Value* callee = context->CreateBitCast(methodPtr, context->GetPtrType(static_cast<cmajor::cpp::ir::Type*>(interfaceMethodType)));
    return callee;
}

void* CppEmitter::GetFunctionIrType(void* functionSymbol) const
{
    auto it = functionIrTypeMap.find(functionSymbol);
    if (it != functionIrTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CppEmitter::SetFunctionIrType(void* symbol, void* irType)
{
    functionIrTypeMap[symbol] = static_cast<cmajor::cpp::ir::FunctionType*>(irType);
}

void* CppEmitter::GetVmtPtr(void* classType, void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType)
{
    cmajor::cpp::ir::Value* vmtPtrPtr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(thisPtr), context->GetLongValue(vmtPtrIndex));
    cmajor::cpp::ir::Value* vmtPtr = context->CreateLoad(vmtPtrPtr);
    return context->CreateBitCast(vmtPtr, static_cast<cmajor::cpp::ir::Type*>(vmtPtrType));
}

void* CppEmitter::GetMethodPtr(void* vmtType, void* vmtPtr, int32_t vmtIndex)
{
    cmajor::cpp::ir::Value* funPtrPtr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(vmtPtr), context->GetLongValue(vmtIndex));
    return context->CreateLoad(funPtrPtr);
}

void* CppEmitter::GetImtArray(void* vmtType, void* vmtObjectPtr, int32_t imtsVmtIndexOffset)
{
    cmajor::cpp::ir::Value* imtsArrayPtrPtr = context->CreateBitCast(context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(vmtObjectPtr), context->GetLongValue(imtsVmtIndexOffset)),
        context->GetPtrType(context->GetPtrType(context->GetPtrType(context->GetVoidType()))));
    cmajor::cpp::ir::Value* imtsArrayPtr = context->CreateLoad(imtsArrayPtrPtr);
    return imtsArrayPtr;
}

void* CppEmitter::GetImt(void* imtArrayType, void* imtArray, int32_t interfaceIndex)
{
    cmajor::cpp::ir::Value* imtArrayPtr = context->CreatePtrOffset(static_cast<cmajor::cpp::ir::Value*>(imtArray), context->GetLongValue(interfaceIndex));
    return context->CreateLoad(imtArrayPtr);
}

void* CppEmitter::GetIrObject(void* symbol) const
{
    auto it = irObjectMap.find(symbol);
    if (it != irObjectMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("emitter: IR object not found");
    }
}

void CppEmitter::SetIrObject(void* symbol, void* irObject)
{
    irObjectMap[symbol] = static_cast<cmajor::cpp::ir::Value*>(irObject);
}

void* CppEmitter::GetMemberVariablePtr(void* classType, void* classPtr, int32_t memberVariableLayoutIndex)
{
    cmajor::cpp::ir::Value* clsPtr = static_cast<cmajor::cpp::ir::Value*>(classPtr);
    return context->CreateElemAddr(clsPtr, context->GetLongValue(memberVariableLayoutIndex));
}

void* CppEmitter::SizeOf(void* elementType, void* ptrType)
{
    cmajor::cpp::ir::Value* nullPtr = context->GetNullValue(static_cast<cmajor::cpp::ir::PtrType*>(ptrType));
    cmajor::cpp::ir::Value* one = context->CreatePtrOffset(nullPtr, context->GetLongValue(1));
    cmajor::cpp::ir::Value* size = context->CreatePtrToInt(one, context->GetLongType());
    return size;
}

void CppEmitter::SetLineNumber(int32_t lineNumber)
{
}

void CppEmitter::SaveObjectPointer(void* objectPointer_)
{
    if (objectPointer == nullptr)
    {
        objectPointer = static_cast<cmajor::cpp::ir::Value*>(objectPointer_);
    }
}

void CppEmitter::SetObjectPointer(void* objectPointer_)
{
    objectPointer = static_cast<cmajor::cpp::ir::Value*>(objectPointer_);
}

void* CppEmitter::GetObjectPointer()
{
    return objectPointer;
}

void* CppEmitter::GetClassIdPtr(void* vmtArrayType, void* vmtPtr, int32_t classIdVmtIndexOffset)
{
    cmajor::cpp::ir::Value* classIdPtr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(vmtPtr), context->GetLongValue(0));
    return classIdPtr;
}

void* CppEmitter::GetClassName(void* vmtArrayType, void* vmtPtr, int32_t classNameVmtIndexOffset)
{
    cmajor::cpp::ir::Value* classNamePtrPtr = context->CreateElemAddr(static_cast<cmajor::cpp::ir::Value*>(vmtPtr), context->GetLongValue(classNameVmtIndexOffset));
    cmajor::cpp::ir::Value* classNamePtr = context->CreateLoad(classNamePtrPtr);
    cmajor::cpp::ir::Value* className = context->CreateBitCast(classNamePtr, context->GetPtrType(context->GetCharType()));
    return className;
}

void* CppEmitter::ComputeAddress(void* type, void* ptr, void* index)
{
    return context->CreatePtrOffset(static_cast<cmajor::cpp::ir::Value*>(ptr), static_cast<cmajor::cpp::ir::Value*>(index));
}

void* CppEmitter::CreatePtrDiff(void* elementType, void* left, void* right)
{
    return context->CreatePtrDiff(static_cast<cmajor::cpp::ir::Value*>(left), static_cast<cmajor::cpp::ir::Value*>(right));
}

uint32_t CppEmitter::GetPrivateFlag()
{
    return uint32_t();
}

uint32_t CppEmitter::GetProtectedFlag()
{
    return uint32_t();
}

uint32_t CppEmitter::GetPublicFlag()
{
    return uint32_t();
}

uint32_t CppEmitter::GetNoFlags()
{
    return uint32_t();
}

void* CppEmitter::CreateModule(const std::string& moduleName)
{
    return new cmajor::cpp::ir::CompileUnit(moduleName);
}

void CppEmitter::DestroyModule(void* module)
{
    delete static_cast<cmajor::cpp::ir::CompileUnit*>(module);
}

void CppEmitter::SetModule(void* module_)
{
    compileUnit = static_cast<cmajor::cpp::ir::CompileUnit*>(module_);
    context = compileUnit->GetContext();
}

void CppEmitter::SetTargetTriple(const std::string& targetTriple)
{
}

void CppEmitter::SetDataLayout(void* dataLayout_)
{
}

void CppEmitter::SetSourceFileName(const std::string& sourceFileName)
{
}

void CppEmitter::SetDICompileUnit(void* diCompileUnit_)
{
}

void CppEmitter::SetDIFile(void* diFile_)
{
}

void CppEmitter::ResetCurrentDebugLocation()
{
}

void CppEmitter::StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized)
{
}

void CppEmitter::FinalizeDebugInfo()
{
}

void CppEmitter::EndDebugInfo()
{
}

void CppEmitter::Compile(const std::string& objectFilePath)
{
}

void CppEmitter::VerifyModule()
{
}

void* CppEmitter::CreateDebugInfoForNamespace(void* scope, const std::string& name)
{
    return nullptr;
}

void* CppEmitter::GetDebugInfoForFile(const soul::ast::FullSpan& fullSpan)
{
    return nullptr;
}

void CppEmitter::PushScope(void* scope)
{
}

void CppEmitter::PopScope()
{
}

void* CppEmitter::CurrentScope()
{
    return nullptr;
}

uint64_t CppEmitter::GetClassTypeSizeInBits(void* classIrType)
{
    return uint64_t();
}

uint64_t CppEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    return uint64_t();
}

void CppEmitter::AddInlineFunctionAttribute(void* function)
{
}

void CppEmitter::SetFunctionLinkage(void* function, bool setInline)
{
}

void CppEmitter::SetFunctionLinkageToLinkOnceODRLinkage(void* function)
{
    static_cast<cmajor::cpp::ir::Function*>(function)->SetLinkOnce();
}

void CppEmitter::SetFunctionCallConventionToStdCall(void* function)
{
}

void CppEmitter::SetFunction(void* function_, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId)
{
    currentFunction = static_cast<cmajor::cpp::ir::Function*>(function_);
    currentFunction->SetFileIndex(fileIndex);
    currentFunction->SetSourceModuleId(sourceModuleId);
    currentFunction->SetFunctionId(functionId);
    context->SetCurrentFunction(currentFunction);
}

void CppEmitter::SetFunctionName(const std::string& functionName)
{
    currentFunction->SetFullName(functionName);
}

void CppEmitter::SetFunctionComment(void* function, const std::string& functionComment)
{
}

void CppEmitter::BeginScope()
{
    context->BeginScope();
}

void CppEmitter::EndScope()
{
    context->EndScope();
}

int16_t CppEmitter::GetCurrentScopeId() const
{
    return context->CurrentScopeId();
}

void CppEmitter::SetCurrentScopeId(int16_t scopeId)
{
    context->SetCurrentScopeId(scopeId);
}

int32_t CppEmitter::AddControlFlowGraphNode()
{
    return context->AddControlFlowGraphNode();
}

void CppEmitter::SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId)
{
    context->SetCurrentFlowGraphNodeId(controlFlowGraphNodeId);
}

void CppEmitter::AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId)
{
    context->AddControlFlowGraphEdge(startNodeId, endNodeId);
}

void CppEmitter::AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject)
{
    context->AddLocalVariable(localVariableName, typeId, static_cast<cmajor::cpp::ir::LocalInstruction*>(irObject));
}

void CppEmitter::BeginInstructionFlag(int16_t flag)
{
    context->BeginInstructionFlag(flag);
}

void CppEmitter::EndInstructionFlag(int16_t flag)
{
    context->EndInstructionFlag(flag);
}

void CppEmitter::SetInPrologue(bool inPrologue_)
{
}

void* CppEmitter::CreateSubroutineType(const std::vector<void*>& elementTypes)
{
    return nullptr;
}

unsigned CppEmitter::GetPureVirtualVirtuality()
{
    return unsigned();
}

unsigned CppEmitter::GetVirtualVirtuality()
{
    return unsigned();
}

unsigned CppEmitter::GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit)
{
    return unsigned();
}

void* CppEmitter::CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags)
{
    return nullptr;
}

void* CppEmitter::CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
    void* subroutineType, unsigned flags)
{
    return nullptr;
}

void CppEmitter::SetDISubprogram(void* function, void* subprogram)
{
}

void* CppEmitter::CreateAlloca(void* irType)
{
    return context->CreateLocal(static_cast<cmajor::cpp::ir::Type*>(irType));
}

void* CppEmitter::CreateDIParameterVariable(const std::string& name, int index, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* irType, void* allocaInst)
{
    return nullptr;
}

void* CppEmitter::CreateDIAutoVariable(const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, void* irType, void* allocaInst)
{
    return nullptr;
}

void* CppEmitter::GetFunctionArgument(void* function, int argumentIndex)
{
    return static_cast<cmajor::cpp::ir::Function*>(function)->GetParam(argumentIndex);
}

void CppEmitter::SetDebugLoc(void* callInst)
{
}

void* CppEmitter::CreateRet(void* value)
{
    return context->CreateRet(static_cast<cmajor::cpp::ir::Value*>(value));
}

void* CppEmitter::CreateRetVoid()
{
    return context->CreateRet(nullptr);
}

void CppEmitter::SetPersonalityFunction(void* function, void* personalityFunction)
{
}

void CppEmitter::AddNoUnwindAttribute(void* function)
{
}

void CppEmitter::AddUWTableAttribute(void* function)
{
}

void* CppEmitter::CreateLexicalBlock(const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

void* CppEmitter::CreateSwitch(void* condition, void* defaultDest, unsigned numCases)
{
    return context->CreateSwitch(static_cast<cmajor::cpp::ir::Value*>(condition), static_cast<cmajor::cpp::ir::BasicBlock*>(defaultDest));
}

void CppEmitter::AddCase(void* switchInst, void* caseValue, void* caseDest)
{
    cmajor::cpp::ir::SwitchInstruction* inst = static_cast<cmajor::cpp::ir::SwitchInstruction*>(switchInst);
    inst->AddCase(static_cast<cmajor::cpp::ir::Value*>(caseValue), static_cast<cmajor::cpp::ir::BasicBlock*>(caseDest));
}

void* CppEmitter::GenerateTrap(const std::vector<void*>& args)
{
    return nullptr;
}

void CppEmitter::SetCompileUnitId(const std::string& compileUnitId)
{
    context->SetCompileUnitId(compileUnitId);
}

void* CppEmitter::GetClsIdValue(const std::string& typeId)
{
    return nullptr;
}

void* CppEmitter::CreateMDBool(bool value)
{
    return nullptr;
}

void* CppEmitter::CreateMDLong(int64_t value)
{
    return nullptr;
}

void* CppEmitter::CreateMDString(const std::string& value)
{
    return nullptr;
}

void* CppEmitter::CreateMDStructRef(int id)
{
    return nullptr;
}

int CppEmitter::GetMDStructId(void* mdStruct)
{
    return 0;
}

void* CppEmitter::CreateMDStruct()
{
    return nullptr;
}

void* CppEmitter::CreateMDBasicBlockRef(void* bb)
{
    return nullptr;
}

void CppEmitter::AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem)
{
}

void CppEmitter::SetFunctionMdId(void* function, int mdId)
{
}

void CppEmitter::SetCurrentSourcePos(int32_t line, int16_t scol, int16_t ecol)
{
    context->SetCurrentSourceSpan(cmajor::debug::SourceSpan(line, scol, ecol));
}

void* CppEmitter::GetMDStructRefForSourceFile(const std::string& sourceFileName)
{
    return nullptr;
}

void CppEmitter::SetMetadataRef(void* inst, void* mdStructRef)
{
}

void CppEmitter::FinalizeFunction(void* function, bool hasCleanup)
{
    static_cast<cmajor::cpp::ir::Function*>(function)->Finalize(hasCleanup);
}

int CppEmitter::Install(const std::string& str)
{
    return emittingDelegate->Install(str);
}

int CppEmitter::Install(const std::u16string& str)
{
    return emittingDelegate->Install(str);
}

int CppEmitter::Install(const std::u32string& str)
{
    return emittingDelegate->Install(str);
}

void* CppEmitter::CreateLandingPad(void* lpType)
{
    return nullptr;
}

void CppEmitter::SetLandindPadAsCleanup(void* landingPad)
{
}

void CppEmitter::MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock)
{
}

void CppEmitter::AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId)
{
}

void* CppEmitter::CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* CppEmitter::CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* CppEmitter::CreateUndefValue(void* type)
{
    return nullptr;
}

void CppEmitter::DebugPrintDebugInfo(const std::string& filePath)
{
}

void CppEmitter::CreateResume(void* exception)
{
    context->CreateResume();
}

void CppEmitter::BeginSubstituteLineNumber(int32_t lineNumber)
{
    substituteLineNumber = true;
    currentLineNumber = lineNumber;
}

void CppEmitter::EndSubstituteLineNumber()
{
    substituteLineNumber = false;
}

void CppEmitter::PrintModule()
{
}

} // namespace cmajor::cpp::backend
