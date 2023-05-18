// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

module cmajor.llvm.emitter;

import cmajor.llvm.value.stack;

namespace cmajor::llvm {

using ArgVector = ::llvm::SmallVector<::llvm::Value*, 4>;

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
};

LLvmEmitterImpl::LLvmEmitterImpl(cmajor::ir::EmittingContext* emittingContext_) : 
    emittingContext(emittingContext_), 
    stack(),
    context(static_cast<::llvm::LLVMContext*>(emittingContext->NativeContext())),
    builder(*context),
    module(nullptr),
    dataLayout(nullptr),
    diCompileUnit(nullptr),
    diFile(nullptr)
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

} // cmajor::llvm
