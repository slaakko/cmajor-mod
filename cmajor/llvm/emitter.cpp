// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Type.h>

module cmajor.llvm.emitter;

import cmajor.llvm.value.stack;
import util;

namespace cmajor::llvm {

using ArgVector = ::llvm::SmallVector<::llvm::Value*, 4>;

struct UuidIntPairHash
{
    size_t operator()(const std::pair<util::uuid, int32_t>& p) const
    {
        return std::hash<int32_t>()(p.second) ^ util::HashValue(p.first);
    }
};

struct LLvmEmitterImpl
{
    LLvmEmitterImpl(cmajor::ir::EmittingContext* emittingContext_);
    cmajor::ir::EmittingContext* emittingContext;
    ValueStack stack;
    ::llvm::LLVMContext& Context() const { return *context; }
    ::llvm::LLVMContext* context;
    ::llvm::IRBuilder<> builder;
    ::llvm::Module* module;
    ::llvm::DataLayout* dataLayout;
    ::llvm::DICompileUnit* diCompileUnit;
    ::llvm::DIFile* diFile;
    std::unique_ptr<::llvm::DIBuilder> diBuilder;
    ::llvm::DIBuilder* currentDIBuilder;
    std::unordered_map<util::uuid, ::llvm::Type*, util::UuidHash> irTypeTypeIdMap;
    std::unordered_map<::llvm::DIType*, util::uuid> fwdDeclarationMap;
    std::unordered_map<util::uuid, ::llvm::DIType*, util::UuidHash> diTypeTypeIdMap;
    std::unordered_map<::llvm::DIType*, std::string> diTypeNameMap;
    std::unordered_map<std::pair<util::uuid, int32_t>, ::llvm::DIDerivedType*, UuidIntPairHash> diMemberTypeMap;
    std::unordered_map<util::uuid, void*, util::UuidHash> classPtrMap;
    std::unordered_map<void*, std::string> classNameMap;
    bool inPrologue;
    std::vector<::llvm::DIScope*> scopes;
    ::llvm::DebugLoc currentDebugLocation;
    ::llvm::DIScope* CurrentScope()
    {
        ::llvm::DIScope* currentScope = diCompileUnit;
        if (!scopes.empty())
        {
            currentScope = scopes.back();
        }
        return currentScope;
    }
    ::llvm::Function* function;
};

LLvmEmitterImpl::LLvmEmitterImpl(cmajor::ir::EmittingContext* emittingContext_) : 
    emittingContext(emittingContext_), 
    stack(),
    context(static_cast<::llvm::LLVMContext*>(emittingContext->NativeContext())),
    builder(*context),
    module(nullptr),
    dataLayout(nullptr),
    diCompileUnit(nullptr),
    diFile(nullptr),
    currentDIBuilder(nullptr),
    inPrologue(false),
    function(nullptr)
{
}

LLvmEmitter::LLvmEmitter(cmajor::ir::EmittingContext* emittingContext_) : impl(new LLvmEmitterImpl(emittingContext_)), emittingDelegate(nullptr)
{
    SetStack(&impl->stack);
}

void LLvmEmitter::SetEmittingDelegate(cmajor::ir::EmittingDelegate* emittingDelegate_)
{
    emittingDelegate = emittingDelegate_;
}

void* LLvmEmitter::GetIrTypeForBool()
{
    return ::llvm::Type::getInt1Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForSByte()
{
    return ::llvm::Type::getInt8Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForByte()
{
    return ::llvm::Type::getInt8Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForShort()
{
    return ::llvm::Type::getInt16Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForUShort()
{
    return ::llvm::Type::getInt16Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForInt()
{
    return ::llvm::Type::getInt32Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForUInt()
{
    return ::llvm::Type::getInt32Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForLong()
{
    return ::llvm::Type::getInt64Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForULong()
{
    return ::llvm::Type::getInt64Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForFloat()
{
    return ::llvm::Type::getFloatTy(impl->Context());
}

void* LLvmEmitter::GetIrTypeForDouble()
{
    return ::llvm::Type::getDoubleTy(impl->Context());
}

void* LLvmEmitter::GetIrTypeForChar()
{
    return ::llvm::Type::getInt8Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForWChar()
{
    return ::llvm::Type::getInt16Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForUChar()
{
    return ::llvm::Type::getInt32Ty(impl->Context());
}

void* LLvmEmitter::GetIrTypeForVoid()
{
    return ::llvm::Type::getVoidTy(impl->Context());
}

void* LLvmEmitter::GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes)
{
    ::llvm::Type* returnType = static_cast<::llvm::Type*>(retType);
    std::vector<::llvm::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<::llvm::Type*>(paramType));
    }
    return ::llvm::FunctionType::get(returnType, parameterTypes, false);
}

void* LLvmEmitter::GetIrTypeForVariableParamFunction(void* retType)
{
    ::llvm::Type* returnType = static_cast<::llvm::Type*>(retType);
    return ::llvm::FunctionType::get(returnType, true);
}

void* LLvmEmitter::GetIrTypeForArrayType(void* elementType, int64_t size)
{
    ::llvm::Type* elementIrType = static_cast<::llvm::Type*>(elementType);
    return ::llvm::ArrayType::get(elementIrType, size);
}

void* LLvmEmitter::GetIrTypeForClassType(const std::vector<void*>& elementTypes)
{
    std::vector<::llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<::llvm::Type*>(elementType));
    }
    return ::llvm::StructType::get(impl->Context(), elementIrTypes);
}

void* LLvmEmitter::GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes)
{
    ::llvm::Type* returnType = static_cast<::llvm::Type*>(retType);
    std::vector<::llvm::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<::llvm::Type*>(paramType));
    }
    return ::llvm::PointerType::get(::llvm::FunctionType::get(returnType, parameterTypes, false), 0);
}

void* LLvmEmitter::GetIrTypeForVoidPtrType()
{
    return ::llvm::Type::getInt8PtrTy(impl->Context());
}

void* LLvmEmitter::GetIrTypeForStructType(const std::vector<void*>& elementTypes)
{
    std::vector<::llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<::llvm::Type*>(elementType));
    }
    return ::llvm::StructType::get(impl->Context(), elementIrTypes);
}

void* LLvmEmitter::CreateDefaultIrValueForBool()
{
    return impl->builder.getInt1(false);
}

void* LLvmEmitter::CreateDefaultIrValueForSByte()
{
    return impl->builder.getInt8(0);
}

void* LLvmEmitter::CreateDefaultIrValueForByte()
{
    return impl->builder.getInt8(0);
}

void* LLvmEmitter::CreateDefaultIrValueForShort()
{
    return impl->builder.getInt16(0);
}

void* LLvmEmitter::CreateDefaultIrValueForUShort()
{
    return impl->builder.getInt16(0);
}

void* LLvmEmitter::CreateDefaultIrValueForInt()
{
    return impl->builder.getInt32(0);
}

void* LLvmEmitter::CreateDefaultIrValueForUInt()
{
    return impl->builder.getInt32(0);
}

void* LLvmEmitter::CreateDefaultIrValueForLong()
{
    return impl->builder.getInt64(0);
}

void* LLvmEmitter::CreateDefaultIrValueForULong()
{
    return impl->builder.getInt64(0);
}

void* LLvmEmitter::CreateDefaultIrValueForFloat()
{
    return ::llvm::ConstantFP::get(::llvm::Type::getFloatTy(impl->Context()), 0.0);
}

void* LLvmEmitter::CreateDefaultIrValueForDouble()
{
    return ::llvm::ConstantFP::get(::llvm::Type::getDoubleTy(impl->Context()), 0.0);
}

void* LLvmEmitter::CreateDefaultIrValueForChar()
{
    return impl->builder.getInt8(0);
}

void* LLvmEmitter::CreateDefaultIrValueForWChar()
{
    return impl->builder.getInt16(0);
}

void* LLvmEmitter::CreateDefaultIrValueForUChar()
{
    return impl->builder.getInt32(0);
}

void* LLvmEmitter::CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults)
{
    std::vector<::llvm::Constant*> arrayOfConstants;
    for (void* constant : arrayOfDefaults)
    {
        arrayOfConstants.push_back(static_cast<::llvm::Constant*>(constant));
    }
    ::llvm::Type* arrayType = static_cast<::llvm::Type*>(arrayIrType);
    return ::llvm::ConstantArray::get(::llvm::cast<::llvm::ArrayType>(arrayType), arrayOfConstants);
}

void* LLvmEmitter::CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers)
{
    std::vector<::llvm::Constant*> arrayOfDefaults;
    for (void* constant : defaultMembers)
    {
        arrayOfDefaults.push_back(static_cast<::llvm::Constant*>(constant));
    }
    return ::llvm::ConstantStruct::get(::llvm::cast <::llvm::StructType >(static_cast<::llvm::Type*>(irType)), arrayOfDefaults);
}

void* LLvmEmitter::CreateDefaultIrValueForDelegateType(void* irType)
{
    return ::llvm::Constant::getNullValue(static_cast<::llvm::Type*>(irType));
}

void* LLvmEmitter::CreateDefaultIrValueForVoidPtrType()
{
    return ::llvm::Constant::getNullValue(impl->builder.getInt8PtrTy());
}

void* LLvmEmitter::CreateDefaultIrValueForDerivedType(void* irType)
{
    return ::llvm::Constant::getNullValue(static_cast<::llvm::Type*>(irType));
}

void* LLvmEmitter::CreateDefaultIrValueForPtrType(void* irType)
{
    return ::llvm::Constant::getNullValue(static_cast<::llvm::Type*>(irType));
}

void* LLvmEmitter::CreateIrValueForBool(bool value)
{
    return impl->builder.getInt1(value);
}

void* LLvmEmitter::CreateIrValueForSByte(int8_t value)
{
    return impl->builder.getInt8(static_cast<uint8_t>(value));
}

void* LLvmEmitter::CreateIrValueForByte(uint8_t value)
{
    return impl->builder.getInt8(value);
}

void* LLvmEmitter::CreateIrValueForShort(int16_t value)
{
    return impl->builder.getInt16(static_cast<uint16_t>(value));
}

void* LLvmEmitter::CreateIrValueForUShort(uint16_t value)
{
    return impl->builder.getInt16(value);
}

void* LLvmEmitter::CreateIrValueForInt(int32_t value)
{
    return impl->builder.getInt32(static_cast<uint32_t>(value));
}

void* LLvmEmitter::CreateIrValueForUInt(uint32_t value)
{
    return impl->builder.getInt32(value);
}

void* LLvmEmitter::CreateIrValueForLong(int64_t value)
{
    return impl->builder.getInt64(static_cast<uint64_t>(value));
}

void* LLvmEmitter::CreateIrValueForULong(uint64_t value)
{
    return impl->builder.getInt64(value);
}

void* LLvmEmitter::CreateIrValueForFloat(float value)
{
    return ::llvm::ConstantFP::get(impl->builder.getFloatTy(), value);
}

void* LLvmEmitter::CreateIrValueForDouble(double value)
{
    return ::llvm::ConstantFP::get(impl->builder.getDoubleTy(), value);
}

void* LLvmEmitter::CreateIrValueForChar(uint8_t value)
{
    return impl->builder.getInt8(value);
}

void* LLvmEmitter::CreateIrValueForWChar(uint16_t value)
{
    return impl->builder.getInt16(value);
}

void* LLvmEmitter::CreateIrValueForUChar(uint32_t value)
{
    return impl->builder.getInt32(value);
}

void* LLvmEmitter::CreateIrValueForWString(void* wstringConstant) 
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(0));
    return impl->builder.CreateGEP(static_cast<::llvm::Value*>(wstringConstant), indeces);
}

void* LLvmEmitter::CreateIrValueForUString(void* ustringConstant)
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(0));
    return impl->builder.CreateGEP(static_cast<::llvm::Value*>(ustringConstant), indeces);
}

void* LLvmEmitter::CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elements, const std::string& prefix)
{
    std::vector<::llvm::Constant*> elementConstants;
    for (void* elementConstant : elements)
    {
        elementConstants.push_back(static_cast<::llvm::Constant*>(elementConstant));
    }
    return ::llvm::ConstantArray::get(::llvm::cast<::llvm::ArrayType>(static_cast<::llvm::Type*>(arrayIrType)), elementConstants);
}

void* LLvmEmitter::CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants)
{
    std::vector<::llvm::Constant*> memberConstants;
    for (void* elementConstant : elementConstants)
    {
        memberConstants.push_back(static_cast<::llvm::Constant*>(elementConstant));
    }
    return ::llvm::ConstantStruct::get(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(structIrType)), memberConstants);
}

void* LLvmEmitter::CreateIrValueForUuid(void* uuidConstant)
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(0));
    return impl->builder.CreateBitCast(impl->builder.CreateGEP(static_cast<::llvm::Value*>(uuidConstant), indeces), impl->builder.getInt8PtrTy());
}

void LLvmEmitter::CreateBr(void* targetBasicBlock)
{
    impl->builder.CreateBr(static_cast<::llvm::BasicBlock*>(targetBasicBlock));
}

void LLvmEmitter::CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock)
{
    impl->builder.CreateCondBr(static_cast<::llvm::Value*>(cond), static_cast<::llvm::BasicBlock*>(trueBasicBlock), static_cast<::llvm::BasicBlock*>(falseBasicBlock));
}

void* LLvmEmitter::CreateArrayIndexAddress(void* arrayPtr, void* index)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(impl->builder.getInt64(0));
    elementIndeces.push_back(static_cast<::llvm::Value*>(index));
    return impl->builder.CreateGEP(static_cast<::llvm::Value*>(arrayPtr), elementIndeces);
}

void LLvmEmitter::CreateStore(void* value, void* ptr)
{
    impl->builder.CreateStore(static_cast<::llvm::Value*>(value), static_cast<::llvm::Value*>(ptr));
}

void* LLvmEmitter::CreateLoad(void* ptr)
{
    return impl->builder.CreateLoad(static_cast<::llvm::Value*>(ptr));
}

void* LLvmEmitter::CreateAdd(void* left, void* right)
{
    return impl->builder.CreateAdd(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateFAdd(void* left, void* right)
{
    return impl->builder.CreateFAdd(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateSub(void* left, void* right)
{
    return impl->builder.CreateSub(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateFSub(void* left, void* right)
{
    return impl->builder.CreateFSub(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateMul(void* left, void* right)
{
    return impl->builder.CreateMul(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateFMul(void* left, void* right)
{
    return impl->builder.CreateFMul(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateUDiv(void* left, void* right)
{
    return impl->builder.CreateUDiv(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateSDiv(void* left, void* right)
{
    return impl->builder.CreateSDiv(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateFDiv(void* left, void* right)
{
    return impl->builder.CreateFDiv(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateURem(void* left, void* right)
{
    return impl->builder.CreateURem(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateSRem(void* left, void* right)
{
    return impl->builder.CreateSRem(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateAnd(void* left, void* right)
{
    return impl->builder.CreateAnd(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateOr(void* left, void* right)
{
    return impl->builder.CreateOr(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateXor(void* left, void* right)
{
    return impl->builder.CreateXor(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateShl(void* left, void* right)
{
    return impl->builder.CreateShl(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateAShr(void* left, void* right)
{
    return impl->builder.CreateAShr(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateLShr(void* left, void* right)
{
    return impl->builder.CreateLShr(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateICmpEQ(void* left, void* right)
{
    return impl->builder.CreateICmpEQ(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateFCmpOEQ(void* left, void* right)
{
    return impl->builder.CreateFCmpOEQ(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

void* LLvmEmitter::CreateICmpULT(void* leftValue, void* rightValue)
{
    return impl->builder.CreateICmpULT(static_cast<::llvm::Value*>(leftValue), static_cast<::llvm::Value*>(rightValue));
}

void* LLvmEmitter::CreateICmpSLT(void* leftValue, void* rightValue)
{
    return impl->builder.CreateICmpSLT(static_cast<::llvm::Value*>(leftValue), static_cast<::llvm::Value*>(rightValue));
}

void* LLvmEmitter::CreateFCmpOLT(void* leftValue, void* rightValue)
{
    return impl->builder.CreateFCmpOLT(static_cast<::llvm::Value*>(leftValue), static_cast<::llvm::Value*>(rightValue));
}

void* LLvmEmitter::CreateSExt(void* operand, void* destinationType)
{
    return impl->builder.CreateSExt(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateZExt(void* operand, void* destinationType) 
{
    return impl->builder.CreateZExt(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateFPExt(void* operand, void* destinationType)
{
    return impl->builder.CreateFPExt(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateTrunc(void* operand, void* destinationType)
{
    return impl->builder.CreateTrunc(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateFPTrunc(void* operand, void* destinationType)
{
    return impl->builder.CreateFPTrunc(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateBitCast(void* operand, void* destinationType)
{
    return impl->builder.CreateBitCast(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateUIToFP(void* operand, void* destinationType)
{
    return impl->builder.CreateUIToFP(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateSIToFP(void* operand, void* destinationType)
{
    return impl->builder.CreateSIToFP(static_cast<::llvm::Value* > (operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateFPToUI(void* operand, void* destinationType)
{
    return impl->builder.CreateFPToUI(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateFPToSI(void* operand, void* destinationType)
{
    return impl->builder.CreateFPToSI(static_cast<::llvm::Value*>(operand), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateIntToPtr(void* intValue, void* destinationType) 
{
    return impl->builder.CreateIntToPtr(static_cast<::llvm::Value*>(intValue), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreatePtrToInt(void* ptrValue, void* destinationType)
{
    return impl->builder.CreatePtrToInt(static_cast<::llvm::Value*>(ptrValue), static_cast<::llvm::Type*>(destinationType));
}

void* LLvmEmitter::CreateNot(void* value)
{
    return impl->builder.CreateNot(static_cast<::llvm::Value*>(value));
}

void* LLvmEmitter::CreateNeg(void* value)
{
    return impl->builder.CreateNeg(static_cast<::llvm::Value*>(value));
}

void* LLvmEmitter::CreateFNeg(void* value)
{
    return impl->builder.CreateFNeg(static_cast<::llvm::Value*>(value));
}

void* LLvmEmitter::CreateNop()
{
    return nullptr;
}

void* LLvmEmitter::CreateSave()
{
    return nullptr;
}

void* LLvmEmitter::GetConversionValue(void* type, void* from)
{
    return nullptr;
}

void* LLvmEmitter::CreateFwdIrTypeForClassType()
{
    return ::llvm::StructType::create(impl->Context());
}

void LLvmEmitter::SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes)
{
    ::llvm::StructType* structType = static_cast<::llvm::StructType*>(forwardDeclaredType);
    std::vector<::llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<::llvm::Type*>(elementType));
    }
    structType->setBody(elementIrTypes);
}

void* LLvmEmitter::GetIrTypeByTypeId(const util::uuid& typeId)
{
    auto it = impl->irTypeTypeIdMap.find(typeId);
    if (it != impl->irTypeTypeIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLvmEmitter::SetIrTypeByTypeId(const util::uuid& typeId, void* irType)
{
    impl->irTypeTypeIdMap[typeId] = static_cast<::llvm::Type*>(irType);
}

void* LLvmEmitter::GetIrTypeForPtrType(void* baseIrType)
{
    return ::llvm::PointerType::get(static_cast<::llvm::Type*>(baseIrType), 0);
}

std::string LLvmEmitter::GetIrTypeName(void* irType)
{
    return std::string();
}

std::string LLvmEmitter::MakeVmtVariableName(const std::string& vmtObjectName)
{
    return std::string();
}

void* LLvmEmitter::CreateDITypeForBool()
{
    return impl->diBuilder->createBasicType("bool", 8, ::llvm::dwarf::DW_ATE_boolean);
}

void* LLvmEmitter::CreateDITypeForSByte()
{
    return impl->diBuilder->createBasicType("sbyte", 8, ::llvm::dwarf::DW_ATE_signed);
}

void* LLvmEmitter::CreateDITypeForByte()
{
    return impl->diBuilder->createBasicType("byte", 8, ::llvm::dwarf::DW_ATE_unsigned);
}

void* LLvmEmitter::CreateDITypeForShort()
{
    return impl->diBuilder->createBasicType("short", 16, ::llvm::dwarf::DW_ATE_signed);
}

void* LLvmEmitter::CreateDITypeForUShort() 
{
    return impl->diBuilder->createBasicType("ushort", 16, ::llvm::dwarf::DW_ATE_unsigned);
}

void* LLvmEmitter::CreateDITypeForInt()
{
    return impl->diBuilder->createBasicType("int", 32, ::llvm::dwarf::DW_ATE_signed);
}

void* LLvmEmitter::CreateDITypeForUInt()
{
    return impl->diBuilder->createBasicType("uint", 32, ::llvm::dwarf::DW_ATE_unsigned);
}

void* LLvmEmitter::CreateDITypeForLong()
{
    return impl->diBuilder->createBasicType("long", 64, ::llvm::dwarf::DW_ATE_signed);
}

void* LLvmEmitter::CreateDITypeForULong()
{
    return impl->diBuilder->createBasicType("ulong", 64, ::llvm::dwarf::DW_ATE_unsigned);
}

void* LLvmEmitter::CreateDITypeForFloat()
{
    return impl->diBuilder->createBasicType("float", 32, ::llvm::dwarf::DW_ATE_float);
}

void* LLvmEmitter::CreateDITypeForDouble()
{
    return impl->diBuilder->createBasicType("double", 64, ::llvm::dwarf::DW_ATE_float);
}

void* LLvmEmitter::CreateDITypeForChar()
{
    return impl->diBuilder->createBasicType("char", 8, ::llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLvmEmitter::CreateDITypeForWChar()
{
    return impl->diBuilder->createBasicType("wchar", 16, ::llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLvmEmitter::CreateDITypeForUChar()
{
    return impl->diBuilder->createBasicType("uchar", 32, ::llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLvmEmitter::CreateDITypeForVoid()
{
    return impl->diBuilder->createUnspecifiedType("void");
}

void* LLvmEmitter::CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements)
{
    // todo...
    std::vector<::llvm::Metadata*> metadataElements;
    return impl->diBuilder->createArrayType(elements.size(), 8, static_cast<::llvm::DIType*>(elementDIType), impl->diBuilder->getOrCreateArray(metadataElements));
}

#if (LLVM_VERSION_MAJOR >= 10)

void* LLvmEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName, const soul::ast::SourcePos& sourcePos, 
    const util::uuid& moduleId)
{
    uint64_t sizeInBits = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)))->getSizeInBits();
    uint64_t alignInBits = 8 * impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)))->getAlignment().value();
    uint64_t offsetInBits = 0; 
    return impl->diBuilder->createReplaceableCompositeType(::llvm::dwarf::DW_TAG_class_type, name, nullptr, 
        static_cast<::llvm::DIFile*>(GetDebugInfoForFile(sourcePos, moduleId)), sourcePos.line,
        0, sizeInBits, alignInBits, ::llvm::DINode::DIFlags::FlagZero, mangledName);
}

#else

void* LLvmEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName, const soul::ast::SourcePos& sourcePos,
    const util::uuid& moduleId)
{
    uint64_t sizeInBits = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)))->getSizeInBits();
    uint32_t alignInBits = 8 * impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)))->getAlignment();
    uint64_t offsetInBits = 0; // todo?
    return impl->diBuilder->createReplaceableCompositeType(::llvm::dwarf::DW_TAG_class_type, name, nullptr, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)), sourcePos.line,
        0, sizeInBits, alignInBits, ::llvm::DINode::DIFlags::FlagZero, mangledName);
}

#endif

void* LLvmEmitter::GetDebugInfoForFile(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (sourcePos.file == -1)
    {
        return impl->diFile;
    }
    std::string sourceFilePath = GetSourceFilePath(sourcePos, moduleId);
    if (sourceFilePath.empty())
    {
        return impl->diFile;
    }
    ::llvm::DIFile* file = impl->diBuilder->createFile(util::Path::GetFileName(sourceFilePath), util::Path::GetDirectoryName(sourceFilePath));
    return file;
}

uint64_t LLvmEmitter::GetOffsetInBits(void* classIrType, int layoutIndex)
{
    const ::llvm::StructLayout* structLayout = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(classIrType)));
    uint64_t offsetInBits = structLayout->getElementOffsetInBits(layoutIndex);
    return offsetInBits;
}

#if (LLVM_VERSION_MAJOR >= 10)

void* LLvmEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::SourcePos& classSourcePos, const util::uuid& moduleId, 
    const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    std::vector<::llvm::Metadata*> elements;
    const ::llvm::StructLayout* structLayout = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)));
    for (void* element : memberVariableElements)
    {
        elements.push_back(static_cast<::llvm::Metadata*>(element));
    }
    ::llvm::MDNode* templateParams = nullptr;
    uint64_t sizeInBits = structLayout->getSizeInBits();
    uint64_t alignInBits = 8 * structLayout->getAlignment().value();
    uint64_t offsetInBits = 0; // todo?
    ::llvm::DINode::DIFlags flags = ::llvm::DINode::DIFlags::FlagZero;
    return impl->diBuilder->createClassType(static_cast<::llvm::DIScope*>(CurrentScope()), name, static_cast<::llvm::DIFile*>(GetDebugInfoForFile(classSourcePos, moduleId)), 
        classSourcePos.line, sizeInBits, alignInBits, offsetInBits,
        flags, static_cast<::llvm::DIType*>(baseClassDIType), impl->diBuilder->getOrCreateArray(elements), static_cast<::llvm::DIType*>(vtableHolderClass), templateParams, mangledName);
}

#else

void* LLvmEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::SourcePos& classSourcePos, const util::uuid& moduleId,
    const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    std::vector<::llvm::Metadata*> elements;
    const ::llvm::StructLayout* structLayout = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::Type*>(irType)));
    for (void* element : memberVariableElements)
    {
        elements.push_back(static_cast<::llvm::Metadata*>(element));
    }
    ::llvm::MDNode* templateParams = nullptr;
    uint64_t sizeInBits = structLayout->getSizeInBits();
    uint32_t alignInBits = 8 * structLayout->getAlignment();
    uint64_t offsetInBits = 0; // todo?
    ::llvm::DINode::DIFlags flags = ::llvm::DINode::DIFlags::FlagZero;
    return impl->diBuilder->createClassType(static_cast<::llvm::DIScope*>(CurrentScope()), name, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(classSourcePos, moduleId)), classSourcePos.line, sizeInBits, alignInBits, offsetInBits,
        flags, static_cast<::llvm::DIType*>(baseClassDIType), impl->diBuilder->getOrCreateArray(elements), static_cast<::llvm::DIType*>(vtableHolderClass), templateParams, mangledName);
}


#endif

void* LLvmEmitter::CreateDITypeForEnumConstant(const std::string& name, int64_t value)
{
    return impl->diBuilder->createEnumerator(name, value);
}

void* LLvmEmitter::CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType)
{
    std::vector<::llvm::Metadata*> elements;
    for (void* element : enumConstantElements)
    {
        elements.push_back(static_cast<::llvm::Metadata*>(element));
    }
    return impl->diBuilder->createEnumerationType(nullptr, name, static_cast<::llvm::DIFile*>(GetDebugInfoForFile(sourcePos, moduleId)), sourcePos.line, sizeInBits, alignInBits,
        impl->diBuilder->getOrCreateArray(elements), static_cast<::llvm::DIType*>(underlyingDIType), mangledName);
}

void LLvmEmitter::MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId)
{
    impl->fwdDeclarationMap[static_cast<::llvm::DIType*>(fwdDeclaration)] = typeId;
}

void* LLvmEmitter::GetDITypeByTypeId(const util::uuid& typeId) const
{
    auto it = impl->diTypeTypeIdMap.find(typeId);
    if (it != impl->diTypeTypeIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLvmEmitter::SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName)
{
    impl->diTypeTypeIdMap[typeId] = static_cast<::llvm::DIType*>(diType);
    impl->diTypeNameMap[static_cast<::llvm::DIType*>(diType)] = typeName;
}

void* LLvmEmitter::GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId)
{
    auto it = impl->diMemberTypeMap.find(memberVariableId);
    if (it != impl->diMemberTypeMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

void LLvmEmitter::SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType)
{
    impl->diMemberTypeMap[memberVariableId] = static_cast<::llvm::DIDerivedType*>(diType);
}

void* LLvmEmitter::CreateDIMemberType(void* scope, const std::string& name, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType)
{
    ::llvm::DINode::DIFlags flags = ::llvm::DINode::DIFlags::FlagZero;
    return impl->diBuilder->createMemberType(static_cast<::llvm::DIType*>(scope), name, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)), sourcePos.line, sizeInBits, alignInBits, offsetInBits, flags,
        static_cast<::llvm::DIType*>(diType));
}

void* LLvmEmitter::CreateConstDIType(void* diType)
{
    return impl->diBuilder->createQualifiedType(::llvm::dwarf::DW_TAG_const_type, static_cast<::llvm::DIType*>(diType));
}

void* LLvmEmitter::CreateLValueRefDIType(void* diType)
{
    return impl->diBuilder->createReferenceType(::llvm::dwarf::DW_TAG_reference_type, static_cast<::llvm::DIType*>(diType));
}

void* LLvmEmitter::CreateRValueRefDIType(void* diType)
{
    return impl->diBuilder->createReferenceType(::llvm::dwarf::DW_TAG_rvalue_reference_type, static_cast<::llvm::DIType*>(diType));
}

void* LLvmEmitter::CreatePointerDIType(void* diType)
{
    return impl->diBuilder->createPointerType(static_cast<::llvm::DIType*>(diType), 64);
}

void* LLvmEmitter::CreateUnspecifiedDIType(const std::string& name) 
{
    return impl->diBuilder->createUnspecifiedType(name);
}

void LLvmEmitter::MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className)
{
    if (impl->classPtrMap.find(typeId) == impl->classPtrMap.cend())
    {
        impl->classPtrMap[typeId] = classPtr;
        impl->classNameMap[classPtr] = className;
    }
}

uint64_t LLvmEmitter::GetSizeInBits(void* irType)
{
    return impl->dataLayout->getTypeSizeInBits(static_cast<::llvm::Type*>(irType));
}

uint64_t LLvmEmitter::GetAlignmentInBits(void* irType)
{
    return 8 * impl->dataLayout->getABITypeAlignment(static_cast<::llvm::Type*>(irType));
}

#if (LLVM_VERSION_MAJOR < 12)

::llvm::DebugLoc GetDebugLocation(LLvmEmitterImpl* impl, const soul::ast::SourcePos& sourcePos)
{
    if (!impl->diCompileUnit || !sourcePos.IsValid() || !impl->currentDIBuilder) return ::llvm::DebugLoc();
    return ::llvm::DebugLoc::get(sourcePos.line, sourcePos.col, static_cast<::llvm::DIScope*>(CurrentScope()));
}

#else

::llvm::DebugLoc GetDebugLocation(LLvmEmitterImpl* impl, const soul::ast::SourcePos& sourcePos)
{
    if (!impl->diCompileUnit || !sourcePos.IsValid() || !impl->currentDIBuilder) return ::llvm::DebugLoc();
    return ::llvm::DILocation::get(impl->Context(), sourcePos.line, sourcePos.col, impl->CurrentScope());
}

#endif

void LLvmEmitter::PushScope(void* scope)
{
    impl->scopes.push_back(static_cast<::llvm::DIScope*>(scope));
}

void LLvmEmitter::PopScope()
{
    impl->scopes.pop_back();
}

void* LLvmEmitter::CurrentScope()
{
    return impl->CurrentScope();
}

void LLvmEmitter::SetCurrentDebugLocation(const soul::ast::SourcePos& sourcePos)
{
    if (!impl->diCompileUnit || !impl->currentDIBuilder) return;
    if (impl->inPrologue || !sourcePos.IsValid())
    {
        impl->currentDebugLocation = ::llvm::DebugLoc();
        impl->builder.SetCurrentDebugLocation(impl->currentDebugLocation);
    }
    else
    {
        impl->currentDebugLocation = GetDebugLocation(impl.get(), sourcePos);
        impl->builder.SetCurrentDebugLocation(impl->currentDebugLocation);
    }
}

void* LLvmEmitter::GetArrayBeginAddress(void* arrayPtr)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(impl->builder.getInt64(0));
    elementIndeces.push_back(impl->builder.getInt64(0));
    ::llvm::Value* beginPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(arrayPtr), elementIndeces);
    return beginPtr;
}

void* LLvmEmitter::GetArrayEndAddress(void* arrayPtr, uint64_t size)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(impl->builder.getInt64(0));
    elementIndeces.push_back(impl->builder.getInt64(size));
    ::llvm::Value* endPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(arrayPtr), elementIndeces);
    return endPtr;
}

void* LLvmEmitter::CreateBasicBlock(const std::string& name)
{
    return ::llvm::BasicBlock::Create(impl->Context(), name, impl->function);
}

void* LLvmEmitter::CreateIncludeBasicBlockInstruction(void* basicBlock)
{
    return nullptr;
}

void LLvmEmitter::PushParentBlock()
{
}

void LLvmEmitter::PopParentBlock()
{
}

void LLvmEmitter::SetHandlerBlock(void* tryBlock, void* catchBlock)
{
}

void LLvmEmitter::SetCleanupBlock(void* cleanupBlock)
{
}

int LLvmEmitter::GetBasicBlockId(void* basicBlock)
{
    return -1;
}

void LLvmEmitter::SetInPrologue(bool inPrologue_) 
{ 
    impl->inPrologue = inPrologue_; 
}

} // cmajor::llvm
