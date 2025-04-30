// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.backend.emitter;

namespace cmajor::systemx::backend {

SystemXEmitter::SystemXEmitter(cmajor::ir::EmittingContext* emittingContext_) :
    cmajor::ir::Emitter(), emittingContext(emittingContext_), emittingDelegate(nullptr), context(new cmajor::systemx::intermediate::Context()), currentFunction(nullptr),
    objectPointer(nullptr)
{
    SetStack(&stack);
}

void SystemXEmitter::SetEmittingDelegate(cmajor::ir::EmittingDelegate* emittingDelegate_)
{
    emittingDelegate = emittingDelegate_;
}

void* SystemXEmitter::GetCompileUnit()
{
    return &context->GetCompileUnit();
}

void SystemXEmitter::SetFilePath(const std::string& filePath)
{
    context->SetFilePath(filePath);
}

void SystemXEmitter::SetCompileUnitMetadataRef(void* metadataRef)
{
    context->GetCompileUnit().SetMetadataRef(static_cast<cmajor::systemx::intermediate::MetadataRef*>(metadataRef));
}

void* SystemXEmitter::GetIrTypeForBool()
{
    return context->GetTypes().GetBoolType();
}

void* SystemXEmitter::GetIrTypeForSByte()
{
    return context->GetTypes().GetSByteType();
}

void* SystemXEmitter::GetIrTypeForByte()
{
    return context->GetTypes().GetByteType();
}

void* SystemXEmitter::GetIrTypeForShort()
{
    return context->GetTypes().GetShortType();
}

void* SystemXEmitter::GetIrTypeForUShort()
{
    return context->GetTypes().GetUShortType();
}

void* SystemXEmitter::GetIrTypeForInt()
{
    return context->GetTypes().GetIntType();
}

void* SystemXEmitter::GetIrTypeForUInt()
{
    return context->GetTypes().GetUIntType();
}

void* SystemXEmitter::GetIrTypeForLong()
{
    return context->GetTypes().GetLongType();
}

void* SystemXEmitter::GetIrTypeForULong()
{
    return context->GetTypes().GetULongType();
}

void* SystemXEmitter::GetIrTypeForFloat()
{
    return context->GetTypes().GetFloatType();
}

void* SystemXEmitter::GetIrTypeForDouble()
{
    return context->GetTypes().GetDoubleType();
}

void* SystemXEmitter::GetIrTypeForChar()
{
    return context->GetTypes().GetByteType();
}

void* SystemXEmitter::GetIrTypeForWChar()
{
    return context->GetTypes().GetUShortType();
}

void* SystemXEmitter::GetIrTypeForUChar()
{
    return context->GetTypes().GetUIntType();
}

void* SystemXEmitter::GetIrTypeForVoid()
{
    return context->GetTypes().GetVoidType();
}

void* SystemXEmitter::GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::systemx::intermediate::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::systemx::intermediate::Type*>(paramType));
    }
    return context->GetTypes().GetFunctionType(static_cast<cmajor::systemx::intermediate::Type*>(retType), parameterTypes);
}

void* SystemXEmitter::GetIrTypeForVariableParamFunction(void* retType)
{
    return nullptr;
}

void* SystemXEmitter::GetIrTypeByTypeId(const util::uuid& typeId)
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

void SystemXEmitter::SetIrTypeByTypeId(const util::uuid& typeId, void* irType)
{
    irTypeTypeIdMap[typeId] = static_cast<cmajor::systemx::intermediate::Type*>(irType);
}

void* SystemXEmitter::GetIrTypeForArrayType(void* elementType, int64_t size)
{
    return context->GetTypes().GetArrayType(size, static_cast<cmajor::systemx::intermediate::Type*>(elementType));
}

void* SystemXEmitter::GetIrTypeForClassType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::systemx::intermediate::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::systemx::intermediate::Type*>(elementType));
    }
    return context->GetTypes().GetStructureType(memberTypes);
}

void* SystemXEmitter::CreateFwdIrTypeForClassType()
{
    return context->GetTypes().CreateStructureType();
}

void SystemXEmitter::SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::systemx::intermediate::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::systemx::intermediate::Type*>(elementType));
    }
    cmajor::systemx::intermediate::StructureType* structureType = static_cast<cmajor::systemx::intermediate::StructureType*>(forwardDeclaredType);
    structureType->SetMemberTypes(memberTypes);
}

void* SystemXEmitter::GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes)
{
    std::vector<cmajor::systemx::intermediate::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<cmajor::systemx::intermediate::Type*>(paramType));
    }
    return context->GetTypes().GetPointerType(context->GetTypes().GetFunctionType(static_cast<cmajor::systemx::intermediate::Type*>(retType), parameterTypes));
}

void* SystemXEmitter::GetIrTypeForVoidPtrType()
{
    return context->GetTypes().GetPointerType(context->GetTypes().GetVoidType());
}

void* SystemXEmitter::GetIrTypeForStructType(const std::vector<void*>& elementTypes)
{
    std::vector<cmajor::systemx::intermediate::Type*> memberTypes;
    for (void* elementType : elementTypes)
    {
        memberTypes.push_back(static_cast<cmajor::systemx::intermediate::Type*>(elementType));
    }
    return context->GetTypes().GetStructureType(memberTypes);
}

void* SystemXEmitter::GetIrTypeForPtrType(void* baseIrType)
{
    return context->GetTypes().GetPointerType(static_cast<cmajor::systemx::intermediate::Type*>(baseIrType));
}

std::string SystemXEmitter::GetIrTypeName(void* irType)
{
    return std::string();
}

std::string SystemXEmitter::MakeVmtVariableName(const std::string& vmtObjectName)
{
    return std::string();
}

void* SystemXEmitter::CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults)
{
    std::vector<cmajor::systemx::intermediate::ConstantValue*> arrayOfConstants;
    for (void* constant : arrayOfDefaults)
    {
        arrayOfConstants.push_back(static_cast<cmajor::systemx::intermediate::ConstantValue*>(constant));
    }
    cmajor::systemx::intermediate::Value* arrayValue = context->GetData().MakeArrayValue(soul::ast::SourcePos(), arrayOfConstants, std::string());
    arrayValue->SetType(static_cast<cmajor::systemx::intermediate::Type*>(arrayIrType));
    return arrayValue;
}

void* SystemXEmitter::CreateDefaultIrValueForBool()
{
    return context->GetFalseValue();
}

void* SystemXEmitter::CreateTrue()
{
    return context->GetTrueValue();
}

void* SystemXEmitter::CreateFalse()
{
    return context->GetFalseValue();
}

void* SystemXEmitter::CreateDefaultIrValueForSByte()
{
    return context->GetSByteValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForByte()
{
    return context->GetByteValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForShort()
{
    return context->GetShortValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForUShort()
{
    return context->GetUShortValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForInt()
{
    return context->GetIntValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForUInt()
{
    return context->GetUIntValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForLong()
{
    return context->GetLongValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForULong()
{
    return context->GetULongValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForFloat()
{
    return context->GetFloatValue(0.0f);
}

void* SystemXEmitter::CreateDefaultIrValueForDouble()
{
    return context->GetDoubleValue(0.0);
}

void* SystemXEmitter::CreateDefaultIrValueForChar()
{
    return context->GetByteValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForWChar()
{
    return context->GetUShortValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForUChar()
{
    return context->GetUIntValue(0);
}

void* SystemXEmitter::CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers)
{
    std::vector<cmajor::systemx::intermediate::ConstantValue*> arrayOfDefaults;
    for (void* constant : defaultMembers)
    {
        arrayOfDefaults.push_back(static_cast<cmajor::systemx::intermediate::ConstantValue*>(constant));
    }
    cmajor::systemx::intermediate::Value* structureValue = context->MakeStructureValue(soul::ast::SourcePos(), arrayOfDefaults);
    structureValue->SetType(static_cast<cmajor::systemx::intermediate::Type*>(irType));
    return structureValue;
}

void* SystemXEmitter::CreateDefaultIrValueForDelegateType(void* irType)
{
    return context->GetNullValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::PointerType*>(irType));
}

void* SystemXEmitter::CreateDefaultIrValueForVoidPtrType()
{
    return context->GetNullValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::PointerType*>(context->GetTypes().GetPointerType(
        context->GetTypes().GetVoidType())));
}

void* SystemXEmitter::CreateDefaultIrValueForDerivedType(void* irType)
{
    return static_cast<cmajor::systemx::intermediate::Type*>(irType)->DefaultValue();
}

void* SystemXEmitter::CreateDefaultIrValueForPtrType(void* irType)
{
    return context->GetNullValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::PointerType*>(irType));
}

void* SystemXEmitter::CreateIrValueForBool(bool value)
{
    return value ? context->GetTrueValue() : context->GetFalseValue();
}

void* SystemXEmitter::CreateIrValueForSByte(int8_t value)
{
    return context->GetSByteValue(value);
}

void* SystemXEmitter::CreateIrValueForByte(uint8_t value)
{
    return context->GetByteValue(value);
}

void* SystemXEmitter::CreateIrValueForShort(int16_t value)
{
    return context->GetShortValue(value);
}

void* SystemXEmitter::CreateIrValueForUShort(uint16_t value)
{
    return context->GetUShortValue(value);
}

void* SystemXEmitter::CreateIrValueForInt(int32_t value)
{
    return context->GetIntValue(value);
}

void* SystemXEmitter::CreateIrValueForUInt(uint32_t value)
{
    return context->GetUIntValue(value);
}

void* SystemXEmitter::CreateIrValueForLong(int64_t value)
{
    return context->GetLongValue(value);
}

void* SystemXEmitter::CreateIrValueForULong(uint64_t value)
{
    return context->GetULongValue(value);
}

void* SystemXEmitter::CreateIrValueForFloat(float value)
{
    return context->GetFloatValue(value);
}

void* SystemXEmitter::CreateIrValueForDouble(double value)
{
    return context->GetDoubleValue(value);
}

void* SystemXEmitter::CreateIrValueForChar(uint8_t value)
{
    return context->GetByteValue(value);
}

void* SystemXEmitter::CreateIrValueForWChar(uint16_t value)
{
    return context->GetUShortValue(value);
}

void* SystemXEmitter::CreateIrValueForUChar(uint32_t value)
{
    return context->GetUIntValue(value);
}

void* SystemXEmitter::CreateIrValueForWString(void* type, void* wstringConstant)
{
    return static_cast<cmajor::systemx::intermediate::Value*>(wstringConstant);
}

void* SystemXEmitter::CreateIrValueForUString(void* type, void* ustringConstant)
{
    return static_cast<cmajor::systemx::intermediate::Value*>(ustringConstant);
}

void* SystemXEmitter::CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elementConstants, const std::string& prefix)
{
    std::vector<cmajor::systemx::intermediate::ConstantValue*> elements;
    for (void* elementConstant : elementConstants)
    {
        elements.push_back(static_cast<cmajor::systemx::intermediate::ConstantValue*>(elementConstant));
    }
    cmajor::systemx::intermediate::Value* arrayValue = context->MakeArrayValue(soul::ast::SourcePos(), elements, prefix);
    arrayValue->SetType(static_cast<cmajor::systemx::intermediate::Type*>(arrayIrType));
    return arrayValue;
}

void* SystemXEmitter::CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants)
{
    std::vector<cmajor::systemx::intermediate::ConstantValue*> memberConstants;
    for (void* elementConstant : elementConstants)
    {
        memberConstants.push_back(static_cast<cmajor::systemx::intermediate::ConstantValue*>(elementConstant));
    }
    cmajor::systemx::intermediate::Value* structureValue = context->MakeStructureValue(soul::ast::SourcePos(), memberConstants);
    structureValue->SetType(static_cast<cmajor::systemx::intermediate::Type*>(structIrType));
    return structureValue;
}

void* SystemXEmitter::CreateIrValueForUuid(void* type, void* uuidConstant)
{
    cmajor::systemx::intermediate::Value* arg = context->CreatePtrOffset(static_cast<cmajor::systemx::intermediate::Value*>(uuidConstant), context->GetLongValue(0));
    return context->CreateBitCast(arg, context->GetTypes().GetPointerType(context->GetTypes().GetVoidType()));
}

void* SystemXEmitter::GetConversionValue(void* type, void* from)
{
    return context->GetData().MakeConversionValue(soul::ast::SourcePos(), 
        static_cast<cmajor::systemx::intermediate::Type*>(type), static_cast<cmajor::systemx::intermediate::ConstantValue*>(from));
}

void* SystemXEmitter::CreateGlobalStringPtr(const std::string& stringValue)
{
    return context->GetData().AddGlobalStringPtr(stringValue);
}

void* SystemXEmitter::CreateGlobalWStringPtr(const std::u16string& stringValue)
{
    return nullptr;
}

void* SystemXEmitter::CreateGlobalUStringPtr(const std::u32string& stringValue)
{
    return nullptr;
}

void* SystemXEmitter::GetGlobalStringPtr(int stringId)
{
    return emittingDelegate->GetGlobalStringPtr(stringId);
}

void* SystemXEmitter::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalWStringConstant(stringId, arrayType);
}

void* SystemXEmitter::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalUStringConstant(stringId, arrayType);
}

void* SystemXEmitter::GetGlobalUuidConstant(int uuidId)
{
    return emittingDelegate->GetGlobalUuidConstant(uuidId);
}

void* SystemXEmitter::CreateDITypeForBool()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForSByte()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForByte()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForShort()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForUShort()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForInt()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForUInt()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForLong()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForULong()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForFloat()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForDouble()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForChar()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForWChar()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForUChar()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForVoid()
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements)
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForEnumConstant(const std::string& name, int64_t value)
{
    return nullptr;
}

void* SystemXEmitter::CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, 
    const soul::ast::LineColLen& lineColLen, const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType)
{
    return nullptr;
}

void* SystemXEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName, 
    const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

uint64_t SystemXEmitter::GetOffsetInBits(void* classIrType, int layoutIndex)
{
    return uint64_t();
}

void* SystemXEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan, 
    const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    return nullptr;
}

void SystemXEmitter::MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId)
{
}

void* SystemXEmitter::GetDITypeByTypeId(const util::uuid& typeId) const
{
    return nullptr;
}

void SystemXEmitter::SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName)
{
}

void* SystemXEmitter::GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId)
{
    return nullptr;
}

void SystemXEmitter::SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType)
{
}

void* SystemXEmitter::CreateDIMemberType(void* scope, const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType)
{
    return nullptr;
}

void* SystemXEmitter::CreateConstDIType(void* diType)
{
    return nullptr;
}

void* SystemXEmitter::CreateLValueRefDIType(void* diType)
{
    return nullptr;
}

void* SystemXEmitter::CreateRValueRefDIType(void* diType)
{
    return nullptr;
}

void* SystemXEmitter::CreatePointerDIType(void* diType)
{
    return nullptr;
}

void* SystemXEmitter::CreateUnspecifiedDIType(const std::string& name)
{
    return nullptr;
}

void SystemXEmitter::MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className)
{
}

uint64_t SystemXEmitter::GetSizeInBits(void* irType)
{
    return 0;
}

uint64_t SystemXEmitter::GetAlignmentInBits(void* irType)
{
    return 0;
}

void SystemXEmitter::SetCurrentDebugLocation(const soul::ast::LineColLen& lineColLen)
{
}

void SystemXEmitter::SetCurrentDebugLocation(const soul::ast::Span& span)
{
}

void* SystemXEmitter::GetArrayBeginAddress(void* arrayType, void* arrayPtr)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(arrayPtr), context->GetLongValue(0));
}

void* SystemXEmitter::GetArrayEndAddress(void* arrayType, void* arrayPtr, uint64_t size)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(arrayPtr), context->GetLongValue(size));
}

void* SystemXEmitter::CreateBasicBlock(const std::string& name)
{
    if (name == "cleanup")
    {
        return currentFunction->CreateCleanupBasicBlock();
    }
    else
    {
        return currentFunction->CreateBasicBlock();
    }
}

void* SystemXEmitter::CreateIncludeBasicBlockInstruction(void* basicBlock)
{
    return nullptr;
}

void SystemXEmitter::PushParentBlock()
{
}

void SystemXEmitter::PopParentBlock()
{
}

void SystemXEmitter::SetHandlerBlock(void* tryBlock, void* catchBlock)
{
}

void SystemXEmitter::SetCleanupBlock(void* cleanupBlock)
{
}

int SystemXEmitter::GetBasicBlockId(void* basicBlock)
{
    return static_cast<cmajor::systemx::intermediate::BasicBlock*>(basicBlock)->Id();
}

void SystemXEmitter::CreateBr(void* targetBasicBlock)
{
    context->CreateJump(static_cast<cmajor::systemx::intermediate::BasicBlock*>(targetBasicBlock));
}

void* SystemXEmitter::CurrentBasicBlock() const
{
    return context->GetCurrentBasicBlock();
}

void SystemXEmitter::SetCurrentBasicBlock(void* basicBlock)
{
    context->SetCurrentBasicBlock(static_cast<cmajor::systemx::intermediate::BasicBlock*>(basicBlock));
}

void SystemXEmitter::CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock)
{
    context->CreateBranch(static_cast<cmajor::systemx::intermediate::Value*>(cond), static_cast<cmajor::systemx::intermediate::BasicBlock*>(trueBasicBlock), 
        static_cast<cmajor::systemx::intermediate::BasicBlock*>(falseBasicBlock));
}

void* SystemXEmitter::CreateArrayIndexAddress(void* arrayType, void* arrayPtr, void* elementType, void* index)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(arrayPtr), static_cast<cmajor::systemx::intermediate::Value*>(index));
}

void SystemXEmitter::CreateStore(void* value, void* ptr)
{
    context->CreateStore(static_cast<cmajor::systemx::intermediate::Value*>(value), static_cast<cmajor::systemx::intermediate::Value*>(ptr));
}

void* SystemXEmitter::CreateLoad(void* type, void* ptr)
{
    return context->CreateLoad(static_cast<cmajor::systemx::intermediate::Value*>(ptr));
}

void* SystemXEmitter::CreateAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFAdd(void* left, void* right)
{
    return context->CreateAdd(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFSub(void* left, void* right)
{
    return context->CreateSub(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFMul(void* left, void* right)
{
    return context->CreateMul(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateUDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateSDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFDiv(void* left, void* right)
{
    return context->CreateDiv(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateURem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateSRem(void* left, void* right)
{
    return context->CreateMod(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateAnd(void* left, void* right)
{
    return context->CreateAnd(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateOr(void* left, void* right)
{
    return context->CreateOr(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateXor(void* left, void* right)
{
    return context->CreateXor(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateShl(void* left, void* right)
{
    return context->CreateShl(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateAShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateLShr(void* left, void* right)
{
    return context->CreateShr(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateICmpEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFCmpOEQ(void* left, void* right)
{
    return context->CreateEqual(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateICmpULT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateICmpSLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateFCmpOLT(void* left, void* right)
{
    return context->CreateLess(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

void* SystemXEmitter::CreateSExt(void* operand, void* destinationType)
{
    return context->CreateSignExtend(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateZExt(void* operand, void* destinationType)
{
    return context->CreateZeroExtend(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateFPExt(void* operand, void* destinationType)
{
    return operand;
}

void* SystemXEmitter::CreateTrunc(void* operand, void* destinationType)
{
    return context->CreateTruncate(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateFPTrunc(void* operand, void* destinationType)
{
    return context->CreateTruncate(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateBitCast(void* operand, void* destinationType)
{
    return context->CreateBitCast(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateUIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateSIToFP(void* operand, void* destinationType)
{
    return context->CreateIntToFloat(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateFPToUI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateFPToSI(void* operand, void* destinationType)
{
    return context->CreateFloatToInt(static_cast<cmajor::systemx::intermediate::Value*>(operand), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateIntToPtr(void* intValue, void* destinationType)
{
    return context->CreateIntToPtr(static_cast<cmajor::systemx::intermediate::Value*>(intValue), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreatePtrToInt(void* ptrValue, void* destinationType)
{
    return context->CreatePtrToInt(static_cast<cmajor::systemx::intermediate::Value*>(ptrValue), static_cast<cmajor::systemx::intermediate::Type*>(destinationType));
}

void* SystemXEmitter::CreateNot(void* value)
{
    return context->CreateNot(static_cast<cmajor::systemx::intermediate::Value*>(value));
}

void* SystemXEmitter::CreateNeg(void* value)
{
    return context->CreateNeg(static_cast<cmajor::systemx::intermediate::Value*>(value));
}

void* SystemXEmitter::CreateFNeg(void* value)
{
    cmajor::systemx::intermediate::Value* val = static_cast<cmajor::systemx::intermediate::Value*>(value);
    if (val->GetType()->Id() == cmajor::systemx::intermediate::doubleTypeId)
    {
        cmajor::systemx::intermediate::ConstantValue* minusOne = context->GetDoubleValue(-1.0);
        return context->CreateMul(minusOne, val);
    }
    else if (val->GetType()->Id() == cmajor::systemx::intermediate::floatTypeId)
    {
        cmajor::systemx::intermediate::ConstantValue* minusOne = context->GetFloatValue(-1.0f);
        return context->CreateMul(minusOne, val);
    }
    else
    {
        throw std::runtime_error("invalid FNeg operand type");
    }
}

void* SystemXEmitter::CreateNop()
{
    return context->CreateNop();
}

void* SystemXEmitter::CreateSave()
{
    return nullptr;
}

void* SystemXEmitter::CreateBeginTry()
{
    return nullptr;
}

void* SystemXEmitter::CreateEndTry(void* nextDest)
{
    return nullptr;
}

void* SystemXEmitter::CreateBeginCatch()
{
    return nullptr;
}

void* SystemXEmitter::CreateEndCatch(void* nextDest)
{
    return nullptr;
}

std::string SystemXEmitter::GetVmtObjectName(void* symbol) const
{
    return std::string();
}

void SystemXEmitter::SetVmtObjectName(void* symbol, const std::string& vmtObjectName)
{
}

std::string SystemXEmitter::GetImtArrayObjectName(void* symbol) const
{
    return std::string();
}

void SystemXEmitter::SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName)
{
}

void* SystemXEmitter::GetVmtObjectType(void* symbol) const
{
    return nullptr;
}

void SystemXEmitter::SetVmtObjectType(void* symbol, void* vmtObjectType)
{
}

void* SystemXEmitter::GetStaticObjectType(void* symbol) const
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

void SystemXEmitter::SetStaticObjectType(void* symbol, void* type)
{
    staticTypeMap[symbol] = static_cast<cmajor::systemx::intermediate::StructureType*>(type);
}

std::string SystemXEmitter::GetStaticObjectName(void* symbol) const
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

void SystemXEmitter::SetStaticObjectName(void* symbol, const std::string& staticObjectName)
{
    staticObjectNameMap[symbol] = staticObjectName;
}

void* SystemXEmitter::GetOrInsertGlobal(const std::string& name, void* type)
{
    return context->GetOrInsertGlobal(name, static_cast<cmajor::systemx::intermediate::Type*>(type));
}

void* SystemXEmitter::GetOrInsertAnyComdat(const std::string& name, void* global)
{
    static_cast<cmajor::systemx::intermediate::GlobalVariable*>(global)->SetOnce();
    return nullptr;
}

void* SystemXEmitter::GetOrInsertAnyFunctionComdat(const std::string& name, void* function)
{
    static_cast<cmajor::systemx::intermediate::Function*>(function)->SetOnce();
    return nullptr;
}

void* SystemXEmitter::GetOrInsertFunction(const std::string& name, void* type, bool nothrow)
{
    return context->GetOrInsertFunction(name, context->GetTypes().GetPointerType(static_cast<cmajor::systemx::intermediate::Type*>(type)));
}

void* SystemXEmitter::MakeSymbolValue(void* type, const std::string& name)
{
    return context->MakeSymbolValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::Type*>(type), name);
}

void SystemXEmitter::SetInitializer(void* global, void* initializer)
{
    cmajor::systemx::intermediate::GlobalVariable* globalVar = static_cast<cmajor::systemx::intermediate::GlobalVariable*>(global);
    globalVar->SetInitializer(static_cast<cmajor::systemx::intermediate::ConstantValue*>(initializer));
}

void SystemXEmitter::SetPrivateLinkage(void* global)
{
}

bool SystemXEmitter::IsVmtObjectCreated(void* symbol) const
{
    return vmtObjectCreatedSet.find(symbol) != vmtObjectCreatedSet.cend();
}

void SystemXEmitter::SetVmtObjectCreated(void* symbol)
{
    vmtObjectCreatedSet.insert(symbol);
}

bool SystemXEmitter::IsStaticObjectCreated(void* symbol) const
{
    return staticObjectCreatedSet.find(symbol) != staticObjectCreatedSet.cend();
}

void SystemXEmitter::SetStaticObjectCreated(void* symbol)
{
    staticObjectCreatedSet.insert(symbol);
}

void* SystemXEmitter::HandlerBlock()
{
    return emittingDelegate->HandlerBlock();
}

void* SystemXEmitter::CleanupBlock()
{
    return emittingDelegate->CleanupBlock();
}

bool SystemXEmitter::NewCleanupNeeded()
{
    return emittingDelegate->NewCleanupNeeded();
}

void SystemXEmitter::CreateCleanup()
{
    return emittingDelegate->CreateCleanup();
}

cmajor::ir::Pad* SystemXEmitter::CurrentPad()
{
    return nullptr;
}

void* SystemXEmitter::CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* SystemXEmitter::CreateCleanupPad(const std::vector<void*>& args)
{
    return nullptr;
}

void* SystemXEmitter::CreateCleanupRet(void* cleanupPad, void* unwindTarget)
{
    return nullptr;
}

void* SystemXEmitter::CreateCatchRet(void* catchPad, void* returnTarget)
{
    return nullptr;
}

void* SystemXEmitter::CreateCatchSwitch(void* unwindBlock)
{
    return nullptr;
}

void* SystemXEmitter::CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock)
{
    return nullptr;
}

void SystemXEmitter::AddHandlerToCatchSwitch(void* catchSwitch, void* target)
{
}

void* SystemXEmitter::CreateCatchPad(void* parentPad, const std::vector<void*>& args)
{
    return nullptr;
}

void* SystemXEmitter::CreateClassDIType(void* classPtr)
{
    return nullptr;
}

void* SystemXEmitter::CreateCall(void* functionType, void* callee, const std::vector<void*>& args)
{
    for (void* arg : args)
    {
        cmajor::systemx::intermediate::Value* argument = static_cast<cmajor::systemx::intermediate::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::systemx::intermediate::Value* calleeValue = static_cast<cmajor::systemx::intermediate::Value*>(callee);
    if (!calleeValue->IsInstruction())
    {
        calleeValue = context->MakeSymbolValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::Type*>(functionType),
            calleeValue->ToString());
    }
    return context->CreateCall(calleeValue);
}

void* SystemXEmitter::CreateCallInst(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen)
{
    for (void* arg : args)
    {
        cmajor::systemx::intermediate::Value* argument = static_cast<cmajor::systemx::intermediate::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::systemx::intermediate::Value* calleeValue = static_cast<cmajor::systemx::intermediate::Value*>(callee);
    if (!calleeValue->IsInstruction())
    {
        calleeValue = context->MakeSymbolValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::Type*>(functionType),
            calleeValue->ToString());
    }
    return context->CreateCall(calleeValue);
}

void* SystemXEmitter::CreateCallInstToBasicBlock(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, void* basicBlock, 
    const soul::ast::LineColLen& lineColLen)
{
    void* prevBasicBlock = context->GetCurrentBasicBlock();
    SetCurrentBasicBlock(basicBlock);
    for (void* arg : args)
    {
        cmajor::systemx::intermediate::Value* argument = static_cast<cmajor::systemx::intermediate::Value*>(arg);
        context->CreateArg(argument);
    }
    cmajor::systemx::intermediate::Value* calleeValue = static_cast<cmajor::systemx::intermediate::Value*>(callee);
    if (!calleeValue->IsInstruction())
    {
        calleeValue = context->MakeSymbolValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::Type*>(functionType),
            calleeValue->ToString());
    }
    cmajor::systemx::intermediate::Instruction* callInst = context->CreateCall(calleeValue);
    SetCurrentBasicBlock(prevBasicBlock);
    return callInst;
}

void* SystemXEmitter::CreateInvoke(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args)
{
    void* cleanupBlock = CleanupBlock();
    if (unwindBlock == cleanupBlock)
    {
        void* nop1 = CreateNop();
        void* beginCleanup = CreateMDStruct();
        AddMDItem(beginCleanup, "nodeType", CreateMDLong(cmajor::systemx::intermediate::beginCleanupNodeType));
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
        AddMDItem(endCleanup, "nodeType", CreateMDLong(cmajor::systemx::intermediate::endCleanupNodeType));
        AddMDItem(endCleanup, "cleanupBlockId", CreateMDBasicBlockRef(cleanupBlock));
        int endCleanupId = GetMDStructId(endCleanup);
        void* endCleanupMdRef = CreateMDStructRef(endCleanupId);
        SetMetadataRef(nop2, endCleanupMdRef);
    }
    return call;
}

void* SystemXEmitter::CreateInvokeInst(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, const std::vector<void*>& bundles,
    const soul::ast::LineColLen& lineColLen)
{
    return CreateInvoke(functionType, callee, normalBlock, unwindBlock, args);
}

void* SystemXEmitter::DIBuilder()
{
    return nullptr;
}

void SystemXEmitter::SetCurrentDIBuilder(void* diBuilder_)
{
}

void* SystemXEmitter::GetObjectFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(classDelegatePtr), context->GetLongValue(0));
}

void* SystemXEmitter::GetDelegateFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(classDelegatePtr), context->GetLongValue(1));
}

void* SystemXEmitter::GetObjectFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::systemx::intermediate::Value* addr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(interfaceTypePtr), context->GetLongValue(0));
    return context->CreateLoad(addr);
}

void* SystemXEmitter::GetObjectPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(interfaceTypePtr), context->GetLongValue(0));
}

void* SystemXEmitter::GetImtPtrPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    return context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(interfaceTypePtr), context->GetLongValue(1));
}

void* SystemXEmitter::GetImtPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    cmajor::systemx::intermediate::Value* interfacePtrAddr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(interfaceTypePtr), 
        context->GetLongValue(1));
    cmajor::systemx::intermediate::Value* interfacePtr = context->CreateLoad(interfacePtrAddr);
    return context->CreateBitCast(interfacePtr, context->GetTypes().GetPointerType(context->GetTypes().GetVoidType()));
}

void* SystemXEmitter::GetInterfaceMethod(void* interfaceType, void* imtPtr, int32_t methodIndex, void* interfaceMethodType)
{
    cmajor::systemx::intermediate::Value* methodPtrPtr = context->CreatePtrOffset(static_cast<cmajor::systemx::intermediate::Value*>(imtPtr), 
        context->GetLongValue(methodIndex));
    cmajor::systemx::intermediate::Value* methodPtr = context->CreateLoad(methodPtrPtr);
    cmajor::systemx::intermediate::Value* callee = context->CreateBitCast(methodPtr, context->GetTypes().GetPointerType(
        static_cast<cmajor::systemx::intermediate::Type*>(interfaceMethodType)));
    return callee;
}

void* SystemXEmitter::GetImtsArrayPtrFromVmt(void* vmtPtr, void* vmtArrayType, int32_t imtsVmtIndexOffset)
{
    cmajor::systemx::intermediate::Value* imtsArrayPtrAddr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(vmtPtr), 
        context->GetLongValue(imtsVmtIndexOffset));
    cmajor::systemx::intermediate::Value* imtsArrayPtr = context->CreateLoad(imtsArrayPtrAddr);
    return imtsArrayPtr;
}

void* SystemXEmitter::GetImtPtrFromImtsPtr(void* imtsPtr, int32_t interfaceIndex, int32_t interfaceCount)
{
    cmajor::systemx::intermediate::Value* imtsArrayPtr = context->CreateBitCast(static_cast<cmajor::systemx::intermediate::Value*>(imtsPtr),
        context->GetTypes().GetPointerType(context->GetTypes().GetArrayType(static_cast<uint64_t>(interfaceCount), context->GetTypes().GetPointerType(
            context->GetTypes().GetVoidType()))));
    cmajor::systemx::intermediate::Value* imtPtrAddr = context->CreateElemAddr(imtsArrayPtr, context->GetLongValue(interfaceIndex));
    cmajor::systemx::intermediate::Value* imtPtr = context->CreateLoad(imtPtrAddr);
    return imtPtr;
}

void* SystemXEmitter::GetFunctionIrType(void* functionSymbol) const
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

void SystemXEmitter::SetFunctionIrType(void* symbol, void* irType)
{
    functionIrTypeMap[symbol] = static_cast<cmajor::systemx::intermediate::FunctionType*>(irType);
}

void* SystemXEmitter::GetVmtPtr(void* classType, void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType)
{
    cmajor::systemx::intermediate::Value* vmtPtrPtr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(thisPtr), context->GetLongValue(vmtPtrIndex));
    cmajor::systemx::intermediate::Value* vmtPtr = context->CreateLoad(vmtPtrPtr);
    return context->CreateBitCast(vmtPtr, static_cast<cmajor::systemx::intermediate::Type*>(vmtPtrType));
}

void* SystemXEmitter::GetMethodPtr(void* vmtType, void* vmtPtr, int32_t vmtIndex)
{
    cmajor::systemx::intermediate::Value* funPtrPtr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(vmtPtr), context->GetLongValue(vmtIndex));
    return context->CreateLoad(funPtrPtr);
}

void* SystemXEmitter::GetIrObject(void* symbol) const
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

void SystemXEmitter::SetIrObject(void* symbol, void* irObject)
{
    irObjectMap[symbol] = static_cast<cmajor::systemx::intermediate::Value*>(irObject);
}

void* SystemXEmitter::GetMemberVariablePtr(void* classType, void* classPtr, int32_t memberVariableLayoutIndex)
{
    cmajor::systemx::intermediate::Value* clsPtr = static_cast<cmajor::systemx::intermediate::Value*>(classPtr);
    return context->CreateElemAddr(clsPtr, context->GetLongValue(memberVariableLayoutIndex));
}

void* SystemXEmitter::SizeOf(void* elementType, void* ptrType)
{
    cmajor::systemx::intermediate::Value* nullPtr = context->GetNullValue(soul::ast::SourcePos(), static_cast<cmajor::systemx::intermediate::PointerType*>(ptrType));
    cmajor::systemx::intermediate::Value* one = context->CreatePtrOffset(nullPtr, context->GetLongValue(1));
    cmajor::systemx::intermediate::Value* size = context->CreatePtrToInt(one, context->GetTypes().GetLongType());
    return size;
}

void SystemXEmitter::SetLineNumber(int32_t lineNumber)
{
}

void SystemXEmitter::SaveObjectPointer(void* objectPointer_)
{
    if (objectPointer == nullptr)
    {
        objectPointer = static_cast<cmajor::systemx::intermediate::Value*>(objectPointer_);
    }
}

void SystemXEmitter::SetObjectPointer(void* objectPointer_)
{
    objectPointer = static_cast<cmajor::systemx::intermediate::Value*>(objectPointer_);
}

void* SystemXEmitter::GetObjectPointer()
{
    return objectPointer;
}

void* SystemXEmitter::GetClassIdPtr(void* vmtArrayType, void* vmtPtr, int32_t classIdVmtIndexOffset)
{
    cmajor::systemx::intermediate::Value* classIdPtr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(vmtPtr), context->GetLongValue(0));
    return classIdPtr;
}

void* SystemXEmitter::GetClassName(void* vmtArrayType, void* vmtPtr, int32_t classNameVmtIndexOffset)
{
    cmajor::systemx::intermediate::Value* classNamePtrPtr = context->CreateElemAddr(static_cast<cmajor::systemx::intermediate::Value*>(vmtPtr), 
        context->GetLongValue(classNameVmtIndexOffset));
    cmajor::systemx::intermediate::Value* classNamePtr = context->CreateLoad(classNamePtrPtr);
    cmajor::systemx::intermediate::Value* classNameCharPtr = context->CreateBitCast(classNamePtr, context->GetTypes().GetPointerType(
        context->GetTypes().GetPointerType(context->GetTypes().GetByteType())));
    cmajor::systemx::intermediate::Value* className = context->CreateLoad(classNameCharPtr);
    return className;
}

void* SystemXEmitter::ComputeAddress(void* type, void* ptr, void* index)
{
    return context->CreatePtrOffset(static_cast<cmajor::systemx::intermediate::Value*>(ptr), static_cast<cmajor::systemx::intermediate::Value*>(index));
}

void* SystemXEmitter::CreatePtrDiff(void* elementType, void* left, void* right)
{
    return context->CreatePtrDiff(static_cast<cmajor::systemx::intermediate::Value*>(left), static_cast<cmajor::systemx::intermediate::Value*>(right));
}

uint32_t SystemXEmitter::GetPrivateFlag()
{
    return uint32_t();
}

uint32_t SystemXEmitter::GetProtectedFlag()
{
    return uint32_t();
}

uint32_t SystemXEmitter::GetPublicFlag()
{
    return uint32_t();
}

uint32_t SystemXEmitter::GetNoFlags()
{
    return uint32_t();
}

void* SystemXEmitter::CreateModule(const std::string& moduleName)
{
    return nullptr;
}

void SystemXEmitter::DestroyModule(void* module)
{
}
 
void SystemXEmitter::SetModule(void* module_)
{
}

void SystemXEmitter::SetTargetTriple(const std::string& targetTriple)
{
}

void SystemXEmitter::SetDataLayout(void* dataLayout_)
{
}

void SystemXEmitter::SetSourceFileName(const std::string& sourceFileName)
{
}

void SystemXEmitter::SetDICompileUnit(void* diCompileUnit_)
{
}

void SystemXEmitter::SetDIFile(void* diFile_)
{
}

void SystemXEmitter::ResetCurrentDebugLocation()
{
}

void SystemXEmitter::StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized)
{
}

void SystemXEmitter::FinalizeDebugInfo()
{
}

void SystemXEmitter::EndDebugInfo()
{
}

void SystemXEmitter::Compile(const std::string& objectFilePath)
{
}

void SystemXEmitter::VerifyModule()
{
}

void* SystemXEmitter::CreateDebugInfoForNamespace(void* scope, const std::string& name)
{
    return nullptr;
}

void* SystemXEmitter::GetDebugInfoForFile(const soul::ast::FullSpan& fullSpan)
{
    return nullptr;
}

void SystemXEmitter::PushScope(void* scope)
{
}

void SystemXEmitter::PopScope()
{
}

void* SystemXEmitter::CurrentScope()
{
    return nullptr;
}

uint64_t SystemXEmitter::GetClassTypeSizeInBits(void* classIrType)
{
    return uint64_t();
}

uint64_t SystemXEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    return uint64_t();
}

void SystemXEmitter::AddInlineFunctionAttribute(void* function)
{
}

void SystemXEmitter::SetFunctionLinkage(void* function, bool setInline)
{
}

void SystemXEmitter::SetFunctionLinkageToLinkOnceODRLinkage(void* function)
{
    static_cast<cmajor::systemx::intermediate::Function*>(function)->SetOnce();
}

void SystemXEmitter::SetFunctionCallConventionToStdCall(void* function)
{
}

void SystemXEmitter::SetFunction(void* function_, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId)
{
    currentFunction = static_cast<cmajor::systemx::intermediate::Function*>(function_);
    context->SetCurrentFunction(currentFunction);
}

void SystemXEmitter::SetFunctionName(const std::string& functionName)
{
}

void SystemXEmitter::SetFunctionComment(void* function, const std::string& functionComment)
{
}

void SystemXEmitter::BeginScope()
{
}

void SystemXEmitter::EndScope()
{
}

int16_t SystemXEmitter::GetCurrentScopeId() const
{
    return 0;
}

void SystemXEmitter::SetCurrentScopeId(int16_t scopeId)
{
}

int32_t SystemXEmitter::AddControlFlowGraphNode()
{
    return -1;
}

void SystemXEmitter::SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId)
{
}

void SystemXEmitter::AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId)
{
}

void SystemXEmitter::AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject)
{
}

void SystemXEmitter::BeginInstructionFlag(int16_t flag)
{
}

void SystemXEmitter::EndInstructionFlag(int16_t flag)
{
}

void SystemXEmitter::SetInPrologue(bool inPrologue_)
{
}

void* SystemXEmitter::CreateSubroutineType(const std::vector<void*>& elementTypes)
{
    return nullptr;
}

unsigned SystemXEmitter::GetPureVirtualVirtuality()
{
    return unsigned();
}

unsigned SystemXEmitter::GetVirtualVirtuality()
{
    return unsigned();
}

unsigned SystemXEmitter::GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit)
{
    return unsigned();
}

void* SystemXEmitter::CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags)
{
    return nullptr;
}

void* SystemXEmitter::CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* subroutineType, unsigned flags)
{
    return nullptr;
}

void SystemXEmitter::SetDISubprogram(void* function, void* subprogram)
{
}

void* SystemXEmitter::CreateAlloca(void* irType)
{
    return context->CreateLocal(static_cast<cmajor::systemx::intermediate::Type*>(irType));
}

void* SystemXEmitter::CreateDIParameterVariable(const std::string& name, int index, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, 
    void* irType, void* allocaInst)
{
    return nullptr;
}

void* SystemXEmitter::CreateDIAutoVariable(const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, void* irType, void* allocaInst)
{
    return nullptr;
}

void* SystemXEmitter::GetFunctionArgument(void* function, int argumentIndex)
{
    return static_cast<cmajor::systemx::intermediate::Function*>(function)->GetParam(argumentIndex);
}

void SystemXEmitter::SetDebugLoc(void* callInst)
{
}

void* SystemXEmitter::CreateRet(void* value)
{
    return context->CreateRet(static_cast<cmajor::systemx::intermediate::Value*>(value));
}

void* SystemXEmitter::CreateRetVoid()
{
    return context->CreateRet(nullptr);
}

void SystemXEmitter::SetPersonalityFunction(void* function, void* personalityFunction)
{
}

void SystemXEmitter::AddNoUnwindAttribute(void* function)
{
}

void SystemXEmitter::AddUWTableAttribute(void* function)
{
}

void* SystemXEmitter::CreateLexicalBlock(const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    return nullptr;
}

void* SystemXEmitter::CreateSwitch(void* condition, void* defaultDest, unsigned numCases)
{
    return context->CreateSwitch(static_cast<cmajor::systemx::intermediate::Value*>(condition), static_cast<cmajor::systemx::intermediate::BasicBlock*>(defaultDest));
}

void SystemXEmitter::AddCase(void* switchInst, void* caseValue, void* caseDest)
{
    cmajor::systemx::intermediate::SwitchInstruction* inst = static_cast<cmajor::systemx::intermediate::SwitchInstruction*>(switchInst);
    inst->AddCase(static_cast<cmajor::systemx::intermediate::Value*>(caseValue), static_cast<cmajor::systemx::intermediate::BasicBlock*>(caseDest));
}

void* SystemXEmitter::GenerateTrap(const std::vector<void*>& args)
{
    std::vector<cmajor::systemx::intermediate::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<cmajor::systemx::intermediate::Value*>(arg));
    }
    return context->CreateTrap(arguments);
}

void SystemXEmitter::SetCompileUnitId(const std::string& compileUnitId)
{
    context->GetCompileUnit().SetInfo(compileUnitId, nullptr);
}

const std::string& SystemXEmitter::CompileUnitId() const
{
    return context->GetCompileUnit().Id();
}

void* SystemXEmitter::GetClsIdValue(const std::string& typeId)
{
    return context->MakeClsIdValue(soul::ast::SourcePos(), context->GetTypes().GetPointerType(context->GetTypes().GetVoidType()), typeId, false);
}

void* SystemXEmitter::CreateMDBool(bool value)
{
    return context->GetMetadata().CreateMetadataBool(value);
}

void* SystemXEmitter::CreateMDLong(int64_t value)
{
    return context->GetMetadata().CreateMetadataLong(value);
}

void* SystemXEmitter::CreateMDString(const std::string& value)
{
    return context->GetMetadata().CreateMetadataString(value, false);
}

void* SystemXEmitter::CreateMDStructRef(int id)
{
    return context->GetMetadata().CreateMetadataRef(soul::ast::SourcePos(), id);
}

int SystemXEmitter::GetMDStructId(void* mdStruct)
{
    return static_cast<cmajor::systemx::intermediate::MetadataStruct*>(mdStruct)->Id();
}

void* SystemXEmitter::CreateMDStruct()
{
    return context->GetMetadata().CreateMetadataStruct();
}

void* SystemXEmitter::CreateMDArray()
{
    return context->GetMetadata().CreateMetadataArray();
}

void* SystemXEmitter::CreateMDBasicBlockRef(void* bb)
{
    return context->GetMetadata().CreateMetadataLong(static_cast<cmajor::systemx::intermediate::BasicBlock*>(bb)->Id());
}

void SystemXEmitter::AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem)
{
    static_cast<cmajor::systemx::intermediate::MetadataStruct*>(mdStruct)->AddItem(fieldName, static_cast<cmajor::systemx::intermediate::MetadataItem*>(mdItem));
}

void SystemXEmitter::AddMDArrayItem(void* mdArray, void* mdItem)
{
    static_cast<cmajor::systemx::intermediate::MetadataArray*>(mdArray)->AddItem(static_cast<cmajor::systemx::intermediate::MetadataItem*>(mdItem));
}

void SystemXEmitter::SetFunctionMdId(void* function, int mdId)
{
    cmajor::systemx::intermediate::MetadataRef* metadataRef = context->GetMetadata().CreateMetadataRef(soul::ast::SourcePos(), mdId);
    static_cast<cmajor::systemx::intermediate::Function*>(function)->SetMetadataRef(metadataRef);
}

void* SystemXEmitter::GetMDStructRefForSourceFile(const std::string& sourceFileName)
{
    return context->GetMetadataRefForSourceFile(sourceFileName);
}

void SystemXEmitter::SetMetadataRef(void* inst, void* mdStructRef)
{
    static_cast<cmajor::systemx::intermediate::Instruction*>(inst)->SetMetadataRef(static_cast<cmajor::systemx::intermediate::MetadataRef*>(mdStructRef));
}

void SystemXEmitter::SetMetadataRefForStructType(void* structType, void* mdRef)
{
    cmajor::systemx::intermediate::StructureType* structureType = static_cast<cmajor::systemx::intermediate::StructureType*>(structType);
    cmajor::systemx::intermediate::MetadataRef* metadataRef = static_cast<cmajor::systemx::intermediate::MetadataRef*>(mdRef);
    structureType->SetMetadataRef(metadataRef);
}

void* SystemXEmitter::GetMetadataRefForStructType(void* structType) const
{
    cmajor::systemx::intermediate::StructureType* structureType = static_cast<cmajor::systemx::intermediate::StructureType*>(structType);
    return structureType->GetMetadataRef();
}

int SystemXEmitter::GetTypeId(void* type) const
{
    cmajor::systemx::intermediate::Type* tp = static_cast<cmajor::systemx::intermediate::Type*>(type);
    return tp->Id();
}

void SystemXEmitter::FinalizeFunction(void* function, bool hasCleanup)
{
    static_cast<cmajor::systemx::intermediate::Function*>(function)->Finalize();
}

int SystemXEmitter::Install(const std::string& str)
{
    return emittingDelegate->Install(str);
}

int SystemXEmitter::Install(const std::u16string& str)
{
    return emittingDelegate->Install(str);
}

int SystemXEmitter::Install(const std::u32string& str)
{
    return emittingDelegate->Install(str);
}

void* SystemXEmitter::CreateLandingPad(void* lpType)
{
    return nullptr;
}

void SystemXEmitter::SetLandindPadAsCleanup(void* landingPad)
{
}

void SystemXEmitter::MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock)
{
}

void SystemXEmitter::AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId)
{
}

void* SystemXEmitter::CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* SystemXEmitter::CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces)
{
    return nullptr;
}

void* SystemXEmitter::CreateUndefValue(void* type)
{
    return nullptr;
}

void SystemXEmitter::CreateResume(void* exception)
{
}

void SystemXEmitter::DebugPrintDebugInfo(const std::string& filePath)
{
}

void SystemXEmitter::BeginSubstituteLineNumber(int32_t lineNumber)
{
}

void SystemXEmitter::EndSubstituteLineNumber()
{
}

void SystemXEmitter::SetCurrentSourcePos(int32_t lineNumber, int16_t scol, int16_t ecol)
{
}

void SystemXEmitter::SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen)
{
    context->SetCurrentLineColLen(lineColLen);
}

int32_t SystemXEmitter::GetLineColLenIndex(const soul::ast::LineColLen& lineColLen) const
{
    return context->GetLineColLenIndex(lineColLen);
}

void SystemXEmitter::PrintModule()
{
}

void SystemXEmitter::SetCurrentFunctionMain()
{
    currentFunction->SetMain();
}

} // namespace cmajor::systemx::backend
