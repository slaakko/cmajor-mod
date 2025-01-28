// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.backend.emitter;

import cmajor.masm.ir;

namespace cmajor::masm::backend {

MasmEmitter::MasmEmitter(cmajor::ir::EmittingContext* emittingContext_) :
    cmajor::ir::Emitter(), emittingContext(emittingContext_), emittingDelegate(nullptr), context(nullptr), compileUnit(nullptr), currentFunction(nullptr), objectPointer(nullptr),
    substituteLineNumber(false), currentLineNumber(0)
{
    SetStack(&stack);
}

void MasmEmitter::SetEmittingDelegate(cmajor::ir::EmittingDelegate* emittingDelegate_)
{
    emittingDelegate = emittingDelegate_;
}

void* MasmEmitter::GetIrTypeForBool()
{
    return context->GetBoolType();
}

void* MasmEmitter::GetIrTypeForSByte()
{
    return context->GetSByteType();
}

void* MasmEmitter::GetIrTypeForByte()
{
    return context->GetByteType();
}

void* MasmEmitter::GetIrTypeForShort()
{
    return context->GetShortType();
}

void* MasmEmitter::GetIrTypeForUShort()
{
    return context->GetUShortType();
}

void* MasmEmitter::GetIrTypeForInt()
{
    return context->GetIntType();
}

void* MasmEmitter::GetIrTypeForUInt()
{
    return context->GetUIntType();
}

void* MasmEmitter::GetIrTypeForLong()
{
    return context->GetLongType();
}

void* MasmEmitter::GetIrTypeForULong()
{
    return context->GetULongType();
}

void* MasmEmitter::GetIrTypeForFloat()
{
    return context->GetFloatType();
}

void* MasmEmitter::GetIrTypeForDouble()
{
    return context->GetDoubleType();
}

void* MasmEmitter::GetIrTypeForChar()
{
    return context->GetByteType();
}

void* MasmEmitter::GetIrTypeForWChar()
{
    return context->GetUShortType();
}

void* MasmEmitter::GetIrTypeForUChar()
{
    return context->GetUIntType();
}

void* MasmEmitter::GetIrTypeForVoid()
{
    return context->GetVoidType();
}

void* MasmEmitter::GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::masm::ir::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::masm::ir::Type*>(paramType));
    }
    return context->GetFunctionType(static_cast<cmajor::masm::ir::Type*>(retType), parameterTypes);
}

void* MasmEmitter::GetIrTypeForVariableParamFunction(void* retType)
{
    return nullptr;
}

void* MasmEmitter::GetIrTypeByTypeId(const util::uuid& typeId)
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

void MasmEmitter::SetIrTypeByTypeId(const util::uuid& typeId, void* irType)
{
    irTypeTypeIdMap[typeId] = static_cast<cmajor::masm::ir::Type*>(irType);
}

void* MasmEmitter::GetIrTypeForArrayType(void* elementType, int64_t size)
{
    return context->GetArrayType(static_cast<cmajor::masm::ir::Type*>(elementType), size);
}

void* MasmEmitter::GetIrTypeForClassType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::masm::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::masm::ir::Type*>(elementType));
    }
    return context->GetStructureType(memberTypes);
}

void* MasmEmitter::CreateFwdIrTypeForClassType()
{
    return context->CreateStructureType();
}

void MasmEmitter::SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::masm::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::masm::ir::Type*>(elementType));
    }
    cmajor::masm::ir::StructureType* structureType = static_cast<cmajor::masm::ir::StructureType*>(forwardDeclaredType);
    structureType->SetMemberTypes(memberTypes);
}

void* MasmEmitter::GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::masm::ir::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::masm::ir::Type*>(paramType));
    }
    return context->GetPtrType(context->GetFunctionType(static_cast<cmajor::masm::ir::Type*>(retType), parameterTypes));
}

void* MasmEmitter::GetIrTypeForVoidPtrType()
{
    return context->GetPtrType(context->GetVoidType());
}

void* MasmEmitter::GetIrTypeForStructType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::masm::ir::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::masm::ir::Type*>(elementType));
    }
    return context->GetStructureType(memberTypes);
}

void* MasmEmitter::GetIrTypeForPtrType(void* baseIrType)
{
    return context->GetPtrType(static_cast<cmajor::masm::ir::Type*>(baseIrType));
}

std::string MasmEmitter::GetIrTypeName(void* irType)
{
    return std::string();
}

std::string MasmEmitter::MakeVmtVariableName(const std::string& vmtObjectName)
{
    return std::string();
}

void* MasmEmitter::CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults)
{
    std::vector<cmajor::masm::ir::Value*> arrayOfConstants;
    for (void* constant : arrayOfDefaults)
    {
        arrayOfConstants.push_back(static_cast<cmajor::masm::ir::Value*>(constant));
    }
    cmajor::masm::ir::Type* arrayType = static_cast<cmajor::masm::ir::Type*>(arrayIrType);
    return context->GetArrayValue(arrayType, arrayOfConstants, std::string());
}

void* MasmEmitter::CreateDefaultIrValueForBool()
{
    return context->GetDefaultBoolValue();
}

void* MasmEmitter::CreateTrue()
{
    return context->GetBoolValue(true);
}

void* MasmEmitter::CreateFalse()
{
    return context->GetBoolValue(false);
}

void* MasmEmitter::CreateDefaultIrValueForSByte()
{
    return context->GetDefaultSByteValue();
}

void* MasmEmitter::CreateDefaultIrValueForByte()
{
    return context->GetDefaultByteValue();
}

void* MasmEmitter::CreateDefaultIrValueForShort()
{
    return context->GetDefaultShortValue();
}

void* MasmEmitter::CreateDefaultIrValueForUShort()
{
    return context->GetDefaultUShortValue();
}

void* MasmEmitter::CreateDefaultIrValueForInt()
{
    return context->GetDefaultIntValue();
}

void* MasmEmitter::CreateDefaultIrValueForUInt()
{
    return context->GetDefaultUIntValue();
}

void* MasmEmitter::CreateDefaultIrValueForLong()
{
    return context->GetDefaultLongValue();
}

void* MasmEmitter::CreateDefaultIrValueForULong()
{
    return context->GetDefaultULongValue();
}

void* MasmEmitter::CreateDefaultIrValueForFloat()
{
    return context->GetDefaultFloatValue();
}

void* MasmEmitter::CreateDefaultIrValueForDouble()
{
    return context->GetDefaultDoubleValue();
}

void* MasmEmitter::CreateDefaultIrValueForChar()
{
    return context->GetDefaultByteValue();
}

void* MasmEmitter::CreateDefaultIrValueForWChar()
{
    return context->GetDefaultUShortValue();
}

void* MasmEmitter::CreateDefaultIrValueForUChar()
{
    return context->GetDefaultUIntValue();
}

void* MasmEmitter::CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers)
{
    std::vector<cmajor::masm::ir::Value*> arrayOfDefaults;
    for (void* constant : defaultMembers)
    {
        arrayOfDefaults.push_back(static_cast<cmajor::masm::ir::Value*>(constant));
    }
    return context->GetStructureValue(static_cast<cmajor::masm::ir::StructureType*>(irType), arrayOfDefaults);
}

void* MasmEmitter::CreateDefaultIrValueForDelegateType(void* irType)
{
    return context->GetNullValue(static_cast<cmajor::masm::ir::PtrType*>(irType));
}

void* MasmEmitter::CreateDefaultIrValueForVoidPtrType()
{
    return context->GetNullValue(static_cast<cmajor::masm::ir::PtrType*>(context->GetPtrType(context->GetVoidType())));
}

void* MasmEmitter::CreateDefaultIrValueForDerivedType(void* irType)
{
    return static_cast<cmajor::masm::ir::Type*>(irType)->DefaultValue();
}

void* MasmEmitter::CreateDefaultIrValueForPtrType(void* irType)
{
    return context->GetNullValue(static_cast<cmajor::masm::ir::PtrType*>(irType));
}

void* MasmEmitter::CreateIrValueForBool(bool value)
{
    return context->GetBoolValue(value);
}

void* MasmEmitter::CreateIrValueForSByte(int8_t value)
{
    return context->GetSByteValue(value);
}

void* MasmEmitter::CreateIrValueForByte(uint8_t value)
{
    return context->GetByteValue(value);
}

void* MasmEmitter::CreateIrValueForShort(int16_t value)
{
    return context->GetShortValue(value);
}

void* MasmEmitter::CreateIrValueForUShort(uint16_t value)
{
    return context->GetUShortValue(value);
}

void* MasmEmitter::CreateIrValueForInt(int32_t value)
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

void* MasmEmitter::CreateIrValueForUInt(uint32_t value)
{
    return context->GetUIntValue(value);
}

void* MasmEmitter::CreateIrValueForLong(int64_t value)
{
    return context->GetLongValue(value);
}

void* MasmEmitter::CreateIrValueForULong(uint64_t value)
{
    return context->GetULongValue(value);
}

void* MasmEmitter::CreateIrValueForFloat(float value)
{
    return context->GetFloatValue(value);
}

void* MasmEmitter::CreateIrValueForDouble(double value)
{
    return context->GetDoubleValue(value);
}

void* MasmEmitter::CreateIrValueForChar(uint8_t value)
{
    return context->GetByteValue(value);
}

void* MasmEmitter::CreateIrValueForWChar(uint16_t value)
{
    return context->GetUShortValue(value);
}

void* MasmEmitter::CreateIrValueForUChar(uint32_t value)
{
    return context->GetUIntValue(value);
}

void* MasmEmitter::CreateIrValueForWString(void* type, void* wstringConstant)
{
    return static_cast<cmajor::masm::ir::Value*>(wstringConstant);
}

void* MasmEmitter::CreateIrValueForUString(void* type, void* ustringConstant)
{
    return static_cast<cmajor::masm::ir::Value*>(ustringConstant);
}

void* MasmEmitter::CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elementConstants, const std::string& prefix)
{
    std::vector<cmajor::masm::ir::Value*> elements;
    for (void* elementConstant : elementConstants)
    {
        elements.push_back(static_cast<cmajor::masm::ir::Value*>(elementConstant));
    }
    return context->GetArrayValue(static_cast<cmajor::masm::ir::ArrayType*>(arrayIrType), elements, prefix);
}

void* MasmEmitter::CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants)
{
    std::vector<cmajor::masm::ir::Value*> memberConstants;
    for (void* elementConstant : elementConstants)
    {
        memberConstants.push_back(static_cast<cmajor::masm::ir::Value*>(elementConstant));
    }
    return context->GetStructureValue(static_cast<cmajor::masm::ir::StructureType*>(structIrType), memberConstants);
}

void* MasmEmitter::CreateIrValueForUuid(void* type, void* uuidConstant)
{
    cmajor::masm::ir::Value* arg = context->CreatePtrOffset(static_cast<cmajor::masm::ir::Value*>(uuidConstant), context->GetLongValue(0));
    return context->CreateBitCast(arg, context->GetPtrType(context->GetVoidType()));
}

void* MasmEmitter::GetConversionValue(void* type, void* from)
{
    return context->GetConversionValue(static_cast<cmajor::masm::ir::Type*>(type), static_cast<cmajor::masm::ir::Value*>(from));
}

void* MasmEmitter::CreateGlobalStringPtr(const std::string& stringValue)
{
    return context->CreateGlobalStringPtr(stringValue);
}

void* MasmEmitter::CreateGlobalWStringPtr(const std::u16string& stringValue)
{
    return nullptr;
}

void* MasmEmitter::CreateGlobalUStringPtr(const std::u32string& stringValue)
{
    return nullptr;
}

void* MasmEmitter::GetGlobalStringPtr(int stringId)
{
    return emittingDelegate->GetGlobalStringPtr(stringId);
}

void* MasmEmitter::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalWStringConstant(stringId, arrayType);
}

void* MasmEmitter::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalUStringConstant(stringId, arrayType);
}

void* MasmEmitter::GetGlobalUuidConstant(int uuidId)
{
    return emittingDelegate->GetGlobalUuidConstant(uuidId);
}

void* MasmEmitter::CreateDITypeForBool()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForSByte()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForByte()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForShort()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForUShort()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForInt()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForUInt()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForLong()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForULong()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForFloat()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForDouble()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForChar()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForWChar()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForUChar()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForVoid()
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements)
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForEnumConstant(const std::string& name, int64_t value)
{
    return nullptr;
}

void* MasmEmitter::CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType)
{
    return nullptr;
}

void* MasmEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName,
    const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

uint64_t MasmEmitter::GetOffsetInBits(void* classIrType, int layoutIndex)
{
    return uint64_t();
}

void* MasmEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan, 
    const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    return nullptr;
}

void MasmEmitter::MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId)
{
}

void* MasmEmitter::GetDITypeByTypeId(const util::uuid& typeId) const
{
    return nullptr;
}

void MasmEmitter::SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName)
{
}

void* MasmEmitter::GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId)
{
    return nullptr;
}

void MasmEmitter::SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType)
{
}

void* MasmEmitter::CreateDIMemberType(void* scope, const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType)
{
    return nullptr;
}

void* MasmEmitter::CreateConstDIType(void* diType)
{
    return nullptr;
}

void* MasmEmitter::CreateLValueRefDIType(void* diType)
{
    return nullptr;
}

void* MasmEmitter::CreateRValueRefDIType(void* diType)
{
    return nullptr;
}

void* MasmEmitter::CreatePointerDIType(void* diType)
{
    return nullptr;
}

void* MasmEmitter::CreateUnspecifiedDIType(const std::string& name)
{
    return nullptr;
}

void MasmEmitter::MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className)
{
}

uint64_t MasmEmitter::GetSizeInBits(void* irType)
{
    return 0;
}

uint64_t MasmEmitter::GetAlignmentInBits(void* irType)
{
    return 0;
}

void MasmEmitter::SetCurrentDebugLocation(const soul::ast::LineColLen& lineColLen)
{
}

void MasmEmitter::SetCurrentDebugLocation(const soul::ast::Span& span)
{
}

void* MasmEmitter::GetArrayBeginAddress(void* arrayType, void* arrayPtr)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(arrayPtr), context->GetLongValue(0));
}

void* MasmEmitter::GetArrayEndAddress(void* arrayType, void* arrayPtr, uint64_t size)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(arrayPtr), context->GetLongValue(size));
}

void* MasmEmitter::CreateBasicBlock(const std::string& name)
{
    return currentFunction->CreateBasicBlock();
}

void* MasmEmitter::CreateIncludeBasicBlockInstruction(void* basicBlock)
{
    return nullptr;
}

void MasmEmitter::PushParentBlock()
{
}

void MasmEmitter::PopParentBlock()
{
}

void MasmEmitter::SetHandlerBlock(void* tryBlock, void* catchBlock)
{
}

void MasmEmitter::SetCleanupBlock(void* cleanupBlock)
{
}

int MasmEmitter::GetBasicBlockId(void* basicBlock)
{
    return static_cast<cmajor::masm::ir::BasicBlock*>(basicBlock)->Id();
}

void MasmEmitter::CreateBr(void* targetBasicBlock)
{
    context->CreateJump(static_cast<cmajor::masm::ir::BasicBlock*>(targetBasicBlock));
}

void* MasmEmitter::CurrentBasicBlock() const
{
    return context->GetCurrentBasicBlock();
}

void MasmEmitter::SetCurrentBasicBlock(void* basicBlock)
{
    context->SetCurrentBasicBlock(static_cast<cmajor::masm::ir::BasicBlock*>(basicBlock));
}

void MasmEmitter::CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock)
{
    context->CreateBranch(static_cast<cmajor::masm::ir::Value*>(cond), static_cast<cmajor::masm::ir::BasicBlock*>(trueBasicBlock), 
        static_cast<cmajor::masm::ir::BasicBlock*>(falseBasicBlock));
}

void* MasmEmitter::CreateArrayIndexAddress(void* arrayType, void* arrayPtr, void* elementType, void* index)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(arrayPtr), static_cast<cmajor::masm::ir::Value*>(index));
}

void MasmEmitter::CreateStore(void* value, void* ptr)
{
    context->CreateStore(static_cast<cmajor::masm::ir::Value*>(value), static_cast<cmajor::masm::ir::Value*>(ptr));
}

void* MasmEmitter::CreateLoad(void* type, void* ptr)
{
    return context->CreateLoad(static_cast<cmajor::masm::ir::Value*>(ptr));
}

void* MasmEmitter::CreateAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateUDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateSDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateURem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateSRem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateAnd(void* left, void* right)
{
    return context->CreateAnd(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateOr(void* left, void* right)
{
    return context->CreateOr(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateXor(void* left, void* right)
{
    return context->CreateXor(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateShl(void* left, void* right)
{
    return context->CreateShl(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateAShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateLShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateICmpEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFCmpOEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateICmpULT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateICmpSLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateFCmpOLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

void* MasmEmitter::CreateSExt(void* operand, void* destinationType)
{
    return context->CreateSignExtend(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateZExt(void* operand, void* destinationType)
{
    return context->CreateZeroExtend(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateFPExt(void* operand, void* destinationType)
{
    return context->CreateFloatingPointExtend(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateTrunc(void* operand, void* destinationType)
{
    return context->CreateTruncate(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateFPTrunc(void* operand, void* destinationType)
{
    return context->CreateTruncate(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateBitCast(void* operand, void* destinationType)
{
    return context->CreateBitCast(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateUIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateSIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateFPToUI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateFPToSI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::masm::ir::Value*>(operand), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateIntToPtr(void* intValue, void* destinationType)
{
    return context->CreateIntToPtr(static_cast<cmajor::masm::ir::Value*>(intValue), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreatePtrToInt(void* ptrValue, void* destinationType)
{
    return context->CreatePtrToInt(static_cast<cmajor::masm::ir::Value*>(ptrValue), static_cast<cmajor::masm::ir::Type*>(destinationType));
}

void* MasmEmitter::CreateNot(void* value)
{
    return context->CreateNot(static_cast<cmajor::masm::ir::Value*>(value));
}

void* MasmEmitter::CreateNeg(void* value)
{
    return context->CreateNeg(static_cast<cmajor::masm::ir::Value*>(value));
}

void* MasmEmitter::CreateFNeg(void* value)
{
    return context->CreateNeg(static_cast<cmajor::masm::ir::Value*>(value));
}

void* MasmEmitter::CreateNop()
{
    return context->CreateNop();
}

void* MasmEmitter::CreateSave()
{
    return nullptr;
}

void* MasmEmitter::CreateBeginTry()
{
    return nullptr;
}

void* MasmEmitter::CreateEndTry(void* nextDest)
{
    return nullptr;
}

void* MasmEmitter::CreateBeginCatch()
{
    return nullptr;
}

void* MasmEmitter::CreateEndCatch(void* nextDest)
{
    return nullptr;
}

std::string MasmEmitter::GetVmtObjectName(void* symbol) const
{
    return std::string();
}

void MasmEmitter::SetVmtObjectName(void* symbol, const std::string& vmtObjectName)
{
}

std::string MasmEmitter::GetImtArrayObjectName(void* symbol) const
{
    return std::string();
}

void MasmEmitter::SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName)
{
}

void* MasmEmitter::GetVmtObjectType(void* symbol) const
{
    return nullptr;
}

void MasmEmitter::SetVmtObjectType(void* symbol, void* vmtObjectType)
{
}

void* MasmEmitter::GetStaticObjectType(void* symbol) const
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

void MasmEmitter::SetStaticObjectType(void* symbol, void* type)
{
    staticTypeMap[symbol] = static_cast<cmajor::masm::ir::StructureType*>(type);
}

std::string MasmEmitter::GetStaticObjectName(void* symbol) const
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

void MasmEmitter::SetStaticObjectName(void* symbol, const std::string& staticObjectName)
{
    staticObjectNameMap[symbol] = staticObjectName;
}

void* MasmEmitter::GetOrInsertGlobal(const std::string& name, void* type)
{
    return context->GetOrInsertGlobal(name, static_cast<cmajor::masm::ir::Type*>(type));
}

void* MasmEmitter::GetOrInsertAnyComdat(const std::string& name, void* global)
{
    //static_cast<cmajor::systemx::ir::GlobalVariable*>(global)->SetLinkOnce();
    return nullptr;
}

void* MasmEmitter::GetOrInsertAnyFunctionComdat(const std::string& name, void* function)
{
    //static_cast<cmajor::systemx::ir::Function*>(function)->SetLinkOnce();
    return nullptr;
}

void* MasmEmitter::GetOrInsertFunction(const std::string& name, void* type, bool nothrow)
{
    return compileUnit->GetOrInsertFunction(name, static_cast<cmajor::masm::ir::FunctionType*>(type));
}

void MasmEmitter::SetInitializer(void* global, void* initializer)
{
    cmajor::masm::ir::GlobalVariable* globalVar = static_cast<cmajor::masm::ir::GlobalVariable*>(global);
    globalVar->SetInitializer(static_cast<cmajor::masm::ir::Value*>(initializer));
}

void MasmEmitter::SetPrivateLinkage(void* global)
{
}

bool MasmEmitter::IsVmtObjectCreated(void* symbol) const
{
    return vmtObjectCreatedSet.find(symbol) != vmtObjectCreatedSet.cend();
}

void MasmEmitter::SetVmtObjectCreated(void* symbol)
{
    vmtObjectCreatedSet.insert(symbol);
}

bool MasmEmitter::IsStaticObjectCreated(void* symbol) const
{
    return staticObjectCreatedSet.find(symbol) != staticObjectCreatedSet.cend();
}

void MasmEmitter::SetStaticObjectCreated(void* symbol)
{
    staticObjectCreatedSet.insert(symbol);
}

void* MasmEmitter::HandlerBlock()
{
    // return emittingDelegate->HandlerBlock();
    return nullptr;
}

void* MasmEmitter::CleanupBlock()
{
    //return emittingDelegate->CleanupBlock();
    return nullptr;
}

bool MasmEmitter::NewCleanupNeeded()
{
    //return emittingDelegate->NewCleanupNeeded();
    return false;
}

void MasmEmitter::CreateCleanup()
{
    //return emittingDelegate->CreateCleanup();
}

cmajor::ir::Pad* MasmEmitter::CurrentPad()
{
    return nullptr;
}

void* MasmEmitter::CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* MasmEmitter::CreateCleanupPad(const std::vector<void*>& args)
{
    return nullptr;
}

void* MasmEmitter::CreateCleanupRet(void* cleanupPad, void* unwindTarget)
{
    return nullptr;
}

void* MasmEmitter::CreateCatchRet(void* catchPad, void* returnTarget)
{
    return nullptr;
}

void* MasmEmitter::CreateCatchSwitch(void* unwindBlock)
{
    return nullptr;
}

void* MasmEmitter::CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock)
{
    return nullptr;
}

void MasmEmitter::AddHandlerToCatchSwitch(void* catchSwitch, void* target)
{
}

void* MasmEmitter::CreateCatchPad(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* MasmEmitter::CreateClassDIType(void* classPtr)
{
    return nullptr;
}

void* MasmEmitter::CreateCall(void* functionType, void* callee, const std::vector<void*>& args)
{
    for (void* arg : args)
    {
        cmajor::masm::ir::Value* argument = static_cast<cmajor::masm::ir::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::masm::ir::Value* calleeValue = static_cast<cmajor::masm::ir::Value*>(callee);
    return context->CreateCall(calleeValue);
}

void* MasmEmitter::CreateCallInst(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen)
{
    for (void* arg : args)
    {
        cmajor::masm::ir::Value* argument = static_cast<cmajor::masm::ir::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::masm::ir::Value* calleeValue = static_cast<cmajor::masm::ir::Value*>(callee);
    return context->CreateCall(calleeValue);
}

void* MasmEmitter::CreateCallInstToBasicBlock(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, 
    void* basicBlock, const soul::ast::LineColLen& lineColLen)
{
    void* prevBasicBlock = context->GetCurrentBasicBlock();
    SetCurrentBasicBlock(basicBlock);
    for (void* arg : args)
    {
        cmajor::masm::ir::Value* argument = static_cast<cmajor::masm::ir::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::masm::ir::Value* calleeValue = static_cast<cmajor::masm::ir::Value*>(callee);
    cmajor::masm::ir::Instruction* callInst = context->CreateCall(calleeValue);
    SetCurrentBasicBlock(prevBasicBlock);
    return callInst;
}

void* MasmEmitter::CreateInvoke(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args)
{
/*
    void* cleanupBlock = CleanupBlock();
    if (unwindBlock == cleanupBlock)
    {
        void* nop1 = CreateNop();
        void* beginCleanup = CreateMDStruct();
        AddMDItem(beginCleanup, "nodeType", CreateMDLong(cmajor::systemx::ir::beginCleanupNodeType));
        AddMDItem(beginCleanup, "cleanupBlockId", CreateMDBasicBlockRef(cleanupBlock));
        if (emittingDelegate->InTryBlock())
        {
            AddMDItem(beginCleanup, "tryBlockId", CreateMDLong(emittingDelegate->CurrentTryBlockId()));
        }
        int beginCleanupId = GetMDStructId(beginCleanup);
        void* beginCleanupMdRef = CreateMDStructRef(beginCleanupId);
        SetMetadataRef(nop1, beginCleanupMdRef);
    }
    void* call = CreateCall(functionType, callee, args);
    if (unwindBlock == cleanupBlock)
    {
        void* nop2 = CreateNop();
        void* endCleanup = CreateMDStruct();
        AddMDItem(endCleanup, "nodeType", CreateMDLong(cmajor::systemx::ir::endCleanupNodeType));
        AddMDItem(endCleanup, "cleanupBlockId", CreateMDBasicBlockRef(cleanupBlock));
        int endCleanupId = GetMDStructId(endCleanup);
        void* endCleanupMdRef = CreateMDStructRef(endCleanupId);
        SetMetadataRef(nop2, endCleanupMdRef);
    }
    return call;
*/
    return nullptr;
}

void* MasmEmitter::CreateInvokeInst(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, const std::vector<void*>& bundles,
    const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

void* MasmEmitter::DIBuilder()
{
    return nullptr;
}

void MasmEmitter::SetCurrentDIBuilder(void* diBuilder_)
{
}

void* MasmEmitter::GetObjectFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(classDelegatePtr), context->GetLongValue(0));
}

void* MasmEmitter::GetDelegateFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(classDelegatePtr), context->GetLongValue(1));
}

void* MasmEmitter::GetObjectFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::masm::ir::Value* addr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(interfaceTypePtr), context->GetLongValue(0));
    return context->CreateLoad(addr);
}

void* MasmEmitter::GetObjectPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(interfaceTypePtr), context->GetLongValue(0));
}

void* MasmEmitter::GetImtPtrPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(interfaceTypePtr), context->GetLongValue(1));
}

void* MasmEmitter::GetImtPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::masm::ir::Value* interfacePtrAddr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(interfaceTypePtr), context->GetLongValue(1));
    cmajor::masm::ir::Value* interfacePtr = context->CreateLoad(interfacePtrAddr);
    return context->CreateBitCast(interfacePtr, context->GetPtrType(context->GetPtrType(context->GetVoidType())));
}

void* MasmEmitter::GetInterfaceMethod(void* interfaceType, void* imtPtr, int32_t methodIndex, void* interfaceMethodType)
{
    cmajor::masm::ir::Value* methodPtrPtr = context->CreatePtrOffset(static_cast<cmajor::masm::ir::Value*>(imtPtr), context->GetLongValue(methodIndex));
    cmajor::masm::ir::Value* methodPtr = context->CreateLoad(methodPtrPtr);
    cmajor::masm::ir::Value* callee = context->CreateBitCast(methodPtr, context->GetPtrType(static_cast<cmajor::masm::ir::Type*>(interfaceMethodType)));
    return callee;
}

void* MasmEmitter::GetImtsArrayPtrFromVmt(void* vmtPtr, void* vmtArrayType, int32_t imtsVmtIndexOffset)
{
    cmajor::masm::ir::Value* imtsArrayPtrAddr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(vmtPtr), context->GetLongValue(imtsVmtIndexOffset));
    cmajor::masm::ir::Value* imtsArrayPtr = context->CreateLoad(imtsArrayPtrAddr);
    return imtsArrayPtr;
}

void* MasmEmitter::GetImtPtrFromImtsPtr(void* imtsPtr, int32_t interfaceIndex, int32_t interfaceCount)
{
    cmajor::masm::ir::Value* imtsArrayPtr = context->CreateBitCast(static_cast<cmajor::masm::ir::Value*>(imtsPtr), 
        context->GetPtrType(context->GetArrayType(context->GetPtrType(context->GetVoidType()), static_cast<uint64_t>(interfaceCount))));
    cmajor::masm::ir::Value* imtPtrAddr = context->CreateElemAddr(imtsArrayPtr, context->GetLongValue(interfaceIndex));
    cmajor::masm::ir::Value* imtPtr = context->CreateLoad(imtPtrAddr);
    return imtPtr;
}

void* MasmEmitter::GetFunctionIrType(void* functionSymbol) const
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

void MasmEmitter::SetFunctionIrType(void* symbol, void* irType)
{
    functionIrTypeMap[symbol] = static_cast<cmajor::masm::ir::FunctionType*>(irType);
}

void* MasmEmitter::GetVmtPtr(void* classType, void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType)
{
    cmajor::masm::ir::Value* vmtPtrPtr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(thisPtr), context->GetLongValue(vmtPtrIndex));
    cmajor::masm::ir::Value* vmtPtr = context->CreateLoad(vmtPtrPtr);
    return context->CreateBitCast(vmtPtr, static_cast<cmajor::masm::ir::Type*>(vmtPtrType));
}

void* MasmEmitter::GetMethodPtr(void* vmtType, void* vmtPtr, int32_t vmtIndex)
{
    cmajor::masm::ir::Value* funPtrPtr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(vmtPtr), context->GetLongValue(vmtIndex));
    return context->CreateLoad(funPtrPtr);
}

/*
void* MasmEmitter::GetImtArray(void* vmtType, void* vmtObjectPtr, int32_t imtsVmtIndexOffset)
{
    cmajor::masm::ir::Value* imtsArrayPtrPtr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(vmtObjectPtr), context->GetLongValue(imtsVmtIndexOffset));
    cmajor::masm::ir::Value* imtsArrayPtr = context->CreateBitCast(imtsArrayPtrPtr, context->GetPtrType(context->GetPtrType(context->GetVoidType())));
    return context->CreateLoad(imtsArrayPtr);
}

void* MasmEmitter::GetImt(void* imtArrayType, void* imtArray, int32_t interfaceIndex)
{
    cmajor::masm::ir::Value* imtArrayPtr = context->CreatePtrOffset(static_cast<cmajor::masm::ir::Value*>(imtArray), context->GetLongValue(interfaceIndex));
    return context->CreateLoad(imtArrayPtr);
}
*/

void* MasmEmitter::GetIrObject(void* symbol) const
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

void MasmEmitter::SetIrObject(void* symbol, void* irObject)
{
    irObjectMap[symbol] = static_cast<cmajor::masm::ir::Value*>(irObject);
}

void* MasmEmitter::GetMemberVariablePtr(void* classType, void* classPtr, int32_t memberVariableLayoutIndex)
{
    cmajor::masm::ir::Value* clsPtr = static_cast<cmajor::masm::ir::Value*>(classPtr);
    return context->CreateElemAddr(clsPtr, context->GetLongValue(memberVariableLayoutIndex));
}

void* MasmEmitter::SizeOf(void* elementType, void* ptrType)
{
    cmajor::masm::ir::Value* nullPtr = context->GetNullValue(static_cast<cmajor::masm::ir::PtrType*>(ptrType));
    cmajor::masm::ir::Value* one = context->CreatePtrOffset(nullPtr, context->GetLongValue(1));
    cmajor::masm::ir::Value* size = context->CreatePtrToInt(one, context->GetLongType());
    return size;
}

void MasmEmitter::SetLineNumber(int32_t lineNumber)
{
}

void MasmEmitter::SaveObjectPointer(void* objectPointer_)
{
    if (objectPointer == nullptr)
    {
        objectPointer = static_cast<cmajor::masm::ir::Value*>(objectPointer_);
    }
}

void MasmEmitter::SetObjectPointer(void* objectPointer_)
{
    objectPointer = static_cast<cmajor::masm::ir::Value*>(objectPointer_);
}

void* MasmEmitter::GetObjectPointer()
{
    return objectPointer;
}

void* MasmEmitter::GetClassIdPtr(void* vmtArrayType, void* vmtPtr, int32_t classIdVmtIndexOffset)
{
    cmajor::masm::ir::Value* classIdPtr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(vmtPtr), context->GetLongValue(0));
    return classIdPtr;
}

void* MasmEmitter::GetClassName(void* vmtArrayType, void* vmtPtr, int32_t classNameVmtIndexOffset)
{
    cmajor::masm::ir::Value* classNamePtrPtr = context->CreateElemAddr(static_cast<cmajor::masm::ir::Value*>(vmtPtr), context->GetLongValue(classNameVmtIndexOffset));
    cmajor::masm::ir::Value* classNamePtr = context->CreateLoad(classNamePtrPtr);
    cmajor::masm::ir::Value* classNameCharPtr = context->CreateBitCast(classNamePtr, context->GetPtrType(context->GetPtrType(context->GetByteType())));
    cmajor::masm::ir::Value* className = context->CreateLoad(classNameCharPtr);
    return className;
}

void* MasmEmitter::ComputeAddress(void* type, void* ptr, void* index)
{
    return context->CreatePtrOffset(static_cast<cmajor::masm::ir::Value*>(ptr), static_cast<cmajor::masm::ir::Value*>(index));
}

void* MasmEmitter::CreatePtrDiff(void* elementType, void* left, void* right)
{
    return context->CreatePtrDiff(static_cast<cmajor::masm::ir::Value*>(left), static_cast<cmajor::masm::ir::Value*>(right));
}

uint32_t MasmEmitter::GetPrivateFlag()
{
    return uint32_t();
}

uint32_t MasmEmitter::GetProtectedFlag()
{
    return uint32_t();
}

uint32_t MasmEmitter::GetPublicFlag()
{
    return uint32_t();
}

uint32_t MasmEmitter::GetNoFlags()
{
    return uint32_t();
}

void* MasmEmitter::CreateModule(const std::string& moduleName)
{
    return new cmajor::masm::ir::CompileUnit(moduleName);
}

void MasmEmitter::DestroyModule(void* module)
{
    delete static_cast<cmajor::masm::ir::CompileUnit*>(module);
}

void MasmEmitter::SetModule(void* module_)
{
    compileUnit = static_cast<cmajor::masm::ir::CompileUnit*>(module_);
    context = compileUnit->GetContext();
}

void MasmEmitter::SetTargetTriple(const std::string& targetTriple)
{
}

void MasmEmitter::SetDataLayout(void* dataLayout_)
{
}

void MasmEmitter::SetSourceFileName(const std::string& sourceFileName)
{
}

void MasmEmitter::SetDICompileUnit(void* diCompileUnit_)
{
}

void MasmEmitter::SetDIFile(void* diFile_)
{
}

void MasmEmitter::ResetCurrentDebugLocation()
{
}

void MasmEmitter::StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized)
{
}

void MasmEmitter::FinalizeDebugInfo()
{
}

void MasmEmitter::EndDebugInfo()
{
}

void MasmEmitter::Compile(const std::string& objectFilePath)
{
}

void MasmEmitter::VerifyModule()
{
}

void* MasmEmitter::CreateDebugInfoForNamespace(void* scope, const std::string& name)
{
    return nullptr;
}

void* MasmEmitter::GetDebugInfoForFile(const soul::ast::FullSpan& fullSpan)
{
    return nullptr;
}

void MasmEmitter::PushScope(void* scope)
{
}

void MasmEmitter::PopScope()
{
}

void* MasmEmitter::CurrentScope()
{
    return nullptr;
}

uint64_t MasmEmitter::GetClassTypeSizeInBits(void* classIrType)
{
    return uint64_t();
}

uint64_t MasmEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    return uint64_t();
}

void MasmEmitter::AddInlineFunctionAttribute(void* function)
{
    cmajor::masm::ir::Function* fn = static_cast<cmajor::masm::ir::Function*>(function);
    fn->SetInline();
}

void MasmEmitter::SetFunctionLinkage(void* function, bool setInline)
{
}

void MasmEmitter::SetFunctionLinkageToLinkOnceODRLinkage(void* function)
{
    //static_cast<cmajor::systemx::ir::Function*>(function)->SetLinkOnce();
}

void MasmEmitter::SetFunctionCallConventionToStdCall(void* function)
{
}

void MasmEmitter::SetFunction(void* function_, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId)
{
    currentFunction = static_cast<cmajor::masm::ir::Function*>(function_);
}

void MasmEmitter::SetFunctionName(const std::string& functionName)
{
}

void MasmEmitter::SetFunctionComment(void* function, const std::string& functionComment)
{
    cmajor::masm::ir::Function* fn = static_cast<cmajor::masm::ir::Function*>(function);
    fn->SetComment(functionComment);
}

void MasmEmitter::BeginScope()
{
}

void MasmEmitter::EndScope()
{
}

int16_t MasmEmitter::GetCurrentScopeId() const
{
    return 0;
}

void MasmEmitter::SetCurrentScopeId(int16_t scopeId)
{
}

int32_t MasmEmitter::AddControlFlowGraphNode()
{
    return -1;
}

void MasmEmitter::SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId)
{
}

void MasmEmitter::AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId)
{
}

void MasmEmitter::AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject)
{
}

void MasmEmitter::BeginInstructionFlag(int16_t flag)
{
}

void MasmEmitter::EndInstructionFlag(int16_t flag)
{
}

void MasmEmitter::SetInPrologue(bool inPrologue_)
{
}

void* MasmEmitter::CreateSubroutineType(const std::vector<void*>& elementTypes)
{
    return nullptr;
}

unsigned MasmEmitter::GetPureVirtualVirtuality()
{
    return unsigned();
}

unsigned MasmEmitter::GetVirtualVirtuality()
{
    return unsigned();
}

unsigned MasmEmitter::GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit)
{
    return unsigned();
}

void* MasmEmitter::CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags)
{
    return nullptr;
}

void* MasmEmitter::CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* subroutineType, unsigned flags)
{
    return nullptr;
}

void MasmEmitter::SetDISubprogram(void* function, void* subprogram)
{
}

void* MasmEmitter::CreateAlloca(void* irType)
{
    return context->CreateLocal(static_cast<cmajor::masm::ir::Type*>(irType));
}

void* MasmEmitter::CreateDIParameterVariable(const std::string& name, int index, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* irType, void* allocaInst)
{
    return nullptr;
}

void* MasmEmitter::CreateDIAutoVariable(const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, void* irType, void* allocaInst)
{
    return nullptr;
}

void* MasmEmitter::GetFunctionArgument(void* function, int argumentIndex)
{
    return static_cast<cmajor::masm::ir::Function*>(function)->GetParam(argumentIndex);
}

void MasmEmitter::SetDebugLoc(void* callInst)
{
}

void* MasmEmitter::CreateRet(void* value)
{
    return context->CreateRet(static_cast<cmajor::masm::ir::Value*>(value));
}

void* MasmEmitter::CreateRetVoid()
{
    return context->CreateRet(nullptr);
}

void MasmEmitter::SetPersonalityFunction(void* function, void* personalityFunction)
{
}

void MasmEmitter::AddNoUnwindAttribute(void* function)
{
}

void MasmEmitter::AddUWTableAttribute(void* function)
{
}

void* MasmEmitter::CreateLexicalBlock(const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

void* MasmEmitter::CreateSwitch(void* condition, void* defaultDest, unsigned numCases)
{
    return context->CreateSwitch(static_cast<cmajor::masm::ir::Value*>(condition), static_cast<cmajor::masm::ir::BasicBlock*>(defaultDest));
}

void MasmEmitter::AddCase(void* switchInst, void* caseValue, void* caseDest)
{
    cmajor::masm::ir::SwitchInstruction* inst = static_cast<cmajor::masm::ir::SwitchInstruction*>(switchInst);
    inst->AddCase(static_cast<cmajor::masm::ir::Value*>(caseValue), static_cast<cmajor::masm::ir::BasicBlock*>(caseDest));
}

void* MasmEmitter::GenerateTrap(const std::vector<void*>& args)
{
/*
    std::vector<cmajor::masm::ir::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<cmajor::systemx::ir::Value*>(arg));
    }
    return context->CreateTrap(arguments);
*/
    return nullptr;
}

void MasmEmitter::SetCompileUnitId(const std::string& compileUnitId)
{
    context->SetCompileUnitId(compileUnitId);
}

const std::string& MasmEmitter::CompileUnitId() const
{
    return context->CompileUnitId();
}

void* MasmEmitter::GetClsIdValue(const std::string& typeId)
{
    return context->GetClsIdValue(typeId);
}

void* MasmEmitter::CreateMDBool(bool value)
{
    return context->CreateMetadataBool(value);
}

void* MasmEmitter::CreateMDLong(int64_t value)
{
    return context->CreateMetadataLong(value);
}

void* MasmEmitter::CreateMDString(const std::string& value)
{
    return context->CreateMetadataString(value, false);
}

void* MasmEmitter::CreateMDStructRef(int id)
{
    return context->CreateMetadataRef(soul::ast::Span(), id);
}

int MasmEmitter::GetMDStructId(void* mdStruct)
{
    return static_cast<cmajor::masm::ir::MetadataStruct*>(mdStruct)->Id();
}

void* MasmEmitter::CreateMDStruct()
{
    return context->CreateMetadataStruct();
}

void* MasmEmitter::CreateMDBasicBlockRef(void* bb)
{
    //return context->CreateMDBasicBlockRef(bb);
    return nullptr;
}

void MasmEmitter::AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem)
{
    context->AddMetadataStructItem(static_cast<cmajor::masm::ir::MetadataStruct*>(mdStruct), fieldName, static_cast<cmajor::masm::ir::MetadataItem*>(mdItem));
}

void MasmEmitter::SetFunctionMdId(void* function, int mdId)
{
    static_cast<cmajor::masm::ir::Function*>(function)->SetMdId(mdId);
}

void* MasmEmitter::GetMDStructRefForSourceFile(const std::string& sourceFileName)
{
    //return context->GetMDStructRefForSourceFile(sourceFileName);
    return nullptr;
}

void MasmEmitter::SetMetadataRef(void* inst, void* mdStructRef)
{
    //context->SetMetadataRef(static_cast<cmajor::masm::ir::Instruction*>(inst), static_cast<cmajor::systemx::ir::MDStructRef*>(mdStructRef));
}

void MasmEmitter::FinalizeFunction(void* function, bool hasCleanup)
{
    static_cast<cmajor::masm::ir::Function*>(function)->Finalize();
}

int MasmEmitter::Install(const std::string& str)
{
    return emittingDelegate->Install(str);
}

int MasmEmitter::Install(const std::u16string& str)
{
    return emittingDelegate->Install(str);
}

int MasmEmitter::Install(const std::u32string& str)
{
    return emittingDelegate->Install(str);
}

void* MasmEmitter::CreateLandingPad(void* lpType)
{
    return nullptr;
}

void MasmEmitter::SetLandindPadAsCleanup(void* landingPad)
{
}

void MasmEmitter::MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock)
{
}

void MasmEmitter::AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId)
{
}

void* MasmEmitter::CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* MasmEmitter::CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* MasmEmitter::CreateUndefValue(void* type)
{
    return nullptr;
}

void MasmEmitter::CreateResume(void* exception)
{
}

void MasmEmitter::DebugPrintDebugInfo(const std::string& filePath)
{
}

void MasmEmitter::BeginSubstituteLineNumber(int32_t lineNumber)
{
    substituteLineNumber = true;
    currentLineNumber = lineNumber;
}

void MasmEmitter::EndSubstituteLineNumber()
{
    substituteLineNumber = false;
}

void MasmEmitter::SetCurrentSourcePos(int32_t lineNumber, int16_t scol, int16_t ecol)
{
    context->SetCurrentLineNumber(lineNumber);
}

void MasmEmitter::SetSpan(const soul::ast::Span& span)
{
    emittingDelegate->SetSpan(span);
}

void MasmEmitter::PrintModule()
{
}

} // namespace cmajor::masm::backend
