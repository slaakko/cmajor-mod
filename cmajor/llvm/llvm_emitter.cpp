// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#pragma warning(disable : 4996)
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/IRPrinter/IRPrintingPasses.h>
#include <llvm/IR/DebugInfoMetadata.h>
#pragma warning(default : 4996)

module cmajor.llvm.emitter;

import cmajor.symbols;

namespace cmllvm {

LLVMEmitter::LLVMEmitter(cmajor::ir::EmittingContext* emittingContext_) :
    emittingContext(emittingContext_),
    stack(),
    context(),
    builder(context),
    module(nullptr),
    dataLayout(nullptr),
    diCompileUnit(nullptr),
    diFile(nullptr),
    currentDIBuilder(nullptr),
    function(nullptr),
    currentBasicBlock(nullptr),
    objectPointer(nullptr),
    inPrologue(false),
    currentLineNumber(-1),
    substituteLineNumber(false),
    emittingDelegate(nullptr)
{
    SetStack(&stack);
}

cmajor::ir::EmittingContext* LLVMEmitter::EmittingContext() const
{
    return emittingContext;
}

void LLVMEmitter::SetEmittingDelegate(cmajor::ir::EmittingDelegate* emittingDelegate_)
{
    emittingDelegate = emittingDelegate_;
}

void* LLVMEmitter::GetIrTypeForBool()
{
    return llvm::Type::getInt1Ty(context);
}

void* LLVMEmitter::GetIrTypeForSByte()
{
    return llvm::Type::getInt8Ty(context);
}

void* LLVMEmitter::GetIrTypeForByte()
{
    return llvm::Type::getInt8Ty(context);
}

void* LLVMEmitter::GetIrTypeForShort()
{
    return llvm::Type::getInt16Ty(context);
}

void* LLVMEmitter::GetIrTypeForUShort()
{
    return llvm::Type::getInt16Ty(context);
}

void* LLVMEmitter::GetIrTypeForInt()
{
    return llvm::Type::getInt32Ty(context);
}

void* LLVMEmitter::GetIrTypeForUInt()
{
    return llvm::Type::getInt32Ty(context);
}

void* LLVMEmitter::GetIrTypeForLong()
{
    return llvm::Type::getInt64Ty(context);
}

void* LLVMEmitter::GetIrTypeForULong()
{
    return llvm::Type::getInt64Ty(context);
}

void* LLVMEmitter::GetIrTypeForFloat()
{
    return llvm::Type::getFloatTy(context);
}

void* LLVMEmitter::GetIrTypeForDouble()
{
    return llvm::Type::getDoubleTy(context);
}

void* LLVMEmitter::GetIrTypeForChar()
{
    return llvm::Type::getInt8Ty(context);
}

void* LLVMEmitter::GetIrTypeForWChar()
{
    return llvm::Type::getInt16Ty(context);
}

void* LLVMEmitter::GetIrTypeForUChar()
{
    return llvm::Type::getInt32Ty(context);
}

void* LLVMEmitter::GetIrTypeForVoid()
{
    return llvm::Type::getVoidTy(context);
}

void* LLVMEmitter::GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes)
{
    llvm::Type* returnType = static_cast<llvm::Type*>(retType);
    std::vector<llvm::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<llvm::Type*>(paramType));
    }
    return llvm::FunctionType::get(returnType, parameterTypes, false);
}

void* LLVMEmitter::GetIrTypeForVariableParamFunction(void* retType)
{
    llvm::Type* returnType = static_cast<llvm::Type*>(retType);
    return llvm::FunctionType::get(returnType, true);
}

void* LLVMEmitter::GetIrTypeForArrayType(void* elementType, int64_t size)
{
    llvm::Type* elementIrType = static_cast<llvm::Type*>(elementType);
    return llvm::ArrayType::get(elementIrType, size);
}

void* LLVMEmitter::GetIrTypeForClassType(const std::vector<void*>& elementTypes)
{
    std::vector<llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<llvm::Type*>(elementType));
    }
    return llvm::StructType::get(context, elementIrTypes);
}

void* LLVMEmitter::GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes)
{
    llvm::Type* returnType = static_cast<llvm::Type*>(retType);
    std::vector<llvm::Type*> parameterTypes;
    for (void* paramType : paramTypes)
    {
        parameterTypes.push_back(static_cast<llvm::Type*>(paramType));
    }
    return llvm::PointerType::get(llvm::FunctionType::get(returnType, parameterTypes, false), 0);
}

void* LLVMEmitter::GetIrTypeForVoidPtrType()
{
    return llvm::PointerType::getUnqual(context); // new return opaque pointer type 
}

void* LLVMEmitter::GetIrTypeForStructType(const std::vector<void*>& elementTypes)
{
    std::vector<llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<llvm::Type*>(elementType));
    }
    return llvm::StructType::get(context, elementIrTypes);
}

void* LLVMEmitter::CreateDefaultIrValueForBool()
{
    return builder.getInt1(false);
}

void* LLVMEmitter::CreateDefaultIrValueForSByte()
{
    return builder.getInt8(0);
}

void* LLVMEmitter::CreateDefaultIrValueForByte()
{
    return builder.getInt8(0);
}

void* LLVMEmitter::CreateDefaultIrValueForShort()
{
    return builder.getInt16(0);
}

void* LLVMEmitter::CreateDefaultIrValueForUShort()
{
    return builder.getInt16(0);
}

void* LLVMEmitter::CreateDefaultIrValueForInt()
{
    return builder.getInt32(0);
}

void* LLVMEmitter::CreateDefaultIrValueForUInt()
{
    return builder.getInt32(0);
}

void* LLVMEmitter::CreateDefaultIrValueForLong()
{
    return builder.getInt64(0);
}

void* LLVMEmitter::CreateDefaultIrValueForULong()
{
    return builder.getInt64(0);
}

void* LLVMEmitter::CreateDefaultIrValueForFloat()
{
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(context), 0.0);
}

void* LLVMEmitter::CreateDefaultIrValueForDouble()
{
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0);
}

void* LLVMEmitter::CreateDefaultIrValueForChar()
{
    return builder.getInt8(0);
}

void* LLVMEmitter::CreateDefaultIrValueForWChar()
{
    return builder.getInt16(0);
}

void* LLVMEmitter::CreateDefaultIrValueForUChar()
{
    return builder.getInt32(0);
}

void* LLVMEmitter::CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults)
{
    std::vector<llvm::Constant*> arrayOfConstants;
    for (void* constant : arrayOfDefaults)
    {
        arrayOfConstants.push_back(static_cast<llvm::Constant*>(constant));
    }
    llvm::Type* arrayType = static_cast<llvm::Type*>(arrayIrType);
    return llvm::ConstantArray::get(llvm::cast<llvm::ArrayType>(arrayType), arrayOfConstants);
}

void* LLVMEmitter::CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers)
{
    std::vector<llvm::Constant*> arrayOfDefaults;
    for (void* constant : defaultMembers)
    {
        arrayOfDefaults.push_back(static_cast<llvm::Constant*>(constant));
    }
    return llvm::ConstantStruct::get(llvm::cast <llvm::StructType >(static_cast<llvm::Type*>(irType)), arrayOfDefaults);
}

void* LLVMEmitter::CreateDefaultIrValueForDelegateType(void* irType)
{
    return llvm::Constant::getNullValue(static_cast<llvm::Type*>(irType));
}

void* LLVMEmitter::CreateDefaultIrValueForVoidPtrType()
{
    return llvm::Constant::getNullValue(builder.getPtrTy()); // // getInt8PtrTy -> getPtrTy
}

void* LLVMEmitter::CreateDefaultIrValueForDerivedType(void* irType)
{
    return llvm::Constant::getNullValue(static_cast<llvm::Type*>(irType));
}

void* LLVMEmitter::CreateDefaultIrValueForPtrType(void* irType)
{
    return llvm::Constant::getNullValue(static_cast<llvm::Type*>(irType));
}

void* LLVMEmitter::CreateIrValueForBool(bool value)
{
    return builder.getInt1(value);
}

void* LLVMEmitter::CreateIrValueForSByte(int8_t value)
{
    return builder.getInt8(static_cast<uint8_t>(value));
}

void* LLVMEmitter::CreateIrValueForByte(uint8_t value)
{
    return builder.getInt8(value);
}

void* LLVMEmitter::CreateIrValueForShort(int16_t value)
{
    return builder.getInt16(static_cast<uint16_t>(value));
}

void* LLVMEmitter::CreateIrValueForUShort(uint16_t value)
{
    return builder.getInt16(value);
}

void* LLVMEmitter::CreateIrValueForInt(int32_t value)
{
    if (substituteLineNumber)
    {
        return builder.getInt32(static_cast<uint32_t>(currentLineNumber));
    }
    else
    {
        return builder.getInt32(static_cast<uint32_t>(value));
    }
}

void* LLVMEmitter::CreateIrValueForUInt(uint32_t value)
{
    return builder.getInt32(value);
}

void* LLVMEmitter::CreateIrValueForLong(int64_t value)
{
    return builder.getInt64(static_cast<uint64_t>(value));
}

void* LLVMEmitter::CreateIrValueForULong(uint64_t value)
{
    return builder.getInt64(value);
}

void* LLVMEmitter::CreateIrValueForFloat(float value)
{
    return llvm::ConstantFP::get(builder.getFloatTy(), value);
}

void* LLVMEmitter::CreateIrValueForDouble(double value)
{
    return llvm::ConstantFP::get(builder.getDoubleTy(), value);
}

void* LLVMEmitter::CreateIrValueForChar(uint8_t value)
{
    return builder.getInt8(value);
}

void* LLVMEmitter::CreateIrValueForWChar(uint16_t value)
{
    return builder.getInt16(value);
}

void* LLVMEmitter::CreateIrValueForUChar(uint32_t value)
{
    return builder.getInt32(value);
}

void* LLVMEmitter::CreateIrValueForWString(void* type, void* wstringConstant)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateGEP(static_cast<llvm::Value*>(wstringConstant), indeces);
#else
    llvm::Value* value = static_cast<llvm::Value*>(wstringConstant);
    return builder.CreateGEP(static_cast<llvm::Type*>(type), value, indeces);
#endif 
}

void* LLVMEmitter::CreateIrValueForUString(void* type, void* ustringConstant)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateGEP(static_cast<llvm::Value*>(ustringConstant), indeces);
#else
    llvm::Value* value = static_cast<llvm::Value*>(ustringConstant);
    return builder.CreateGEP(static_cast<llvm::Type*>(type), value, indeces);
#endif 
}

void* LLVMEmitter::CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elements, const std::string& prefix)
{
    std::vector<llvm::Constant*> elementConstants;
    for (void* elementConstant : elements)
    {
        elementConstants.push_back(static_cast<llvm::Constant*>(elementConstant));
    }
    return llvm::ConstantArray::get(llvm::cast<llvm::ArrayType>(static_cast<llvm::Type*>(arrayIrType)), elementConstants);
}

void* LLVMEmitter::CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants)
{
    std::vector<llvm::Constant*> memberConstants;
    for (void* elementConstant : elementConstants)
    {
        memberConstants.push_back(static_cast<llvm::Constant*>(elementConstant));
    }
    return llvm::ConstantStruct::get(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(structIrType)), memberConstants);
}

void* LLVMEmitter::CreateIrValueForUuid(void* type, void* uuidConstant)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateBitCast(builder.CreateGEP(static_cast<llvm::Value*>(uuidConstant), indeces), builder.getPtrTy()); // getInt8PtrTy -> getPtrTy
#else
    llvm::Value* value = static_cast<llvm::Value*>(uuidConstant);
    llvm::Value* gep = builder.CreateGEP(static_cast<llvm::Type*>(type), value, indeces);
    return builder.CreateBitCast(gep, builder.getPtrTy()); 
#endif
}

void LLVMEmitter::CreateBr(void* targetBasicBlock)
{
    builder.CreateBr(static_cast<llvm::BasicBlock*>(targetBasicBlock));
}

void LLVMEmitter::CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock)
{
    builder.CreateCondBr(static_cast<llvm::Value*>(cond), static_cast<llvm::BasicBlock*>(trueBasicBlock), static_cast<llvm::BasicBlock*>(falseBasicBlock));
}

void* LLVMEmitter::CreateArrayIndexAddress(void* arrayType, void* arrayPtr, void* elementType, void* index)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(builder.getInt64(0));
    elementIndeces.push_back(static_cast<llvm::Value*>(index));
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateGEP(static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#else
    return builder.CreateGEP(static_cast<llvm::Type*>(arrayType), static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#endif
}

void LLVMEmitter::CreateStore(void* value, void* ptr)
{
    builder.CreateStore(static_cast<llvm::Value*>(value), static_cast<llvm::Value*>(ptr));
}

void* LLVMEmitter::CreateLoad(void* type, void* ptr)
{
    llvm::Type* t = static_cast<llvm::Type*>(type);
    return builder.CreateLoad(t, static_cast<llvm::Value*>(ptr));
}

void* LLVMEmitter::CreateAdd(void* left, void* right)
{
    return builder.CreateAdd(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateFAdd(void* left, void* right)
{
    return builder.CreateFAdd(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateSub(void* left, void* right)
{
    return builder.CreateSub(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateFSub(void* left, void* right)
{
    return builder.CreateFSub(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateMul(void* left, void* right)
{
    return builder.CreateMul(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateFMul(void* left, void* right)
{
    return builder.CreateFMul(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateUDiv(void* left, void* right)
{
    return builder.CreateUDiv(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateSDiv(void* left, void* right)
{
    return builder.CreateSDiv(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateFDiv(void* left, void* right)
{
    return builder.CreateFDiv(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateURem(void* left, void* right)
{
    return builder.CreateURem(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateSRem(void* left, void* right)
{
    return builder.CreateSRem(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateAnd(void* left, void* right)
{
    return builder.CreateAnd(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateOr(void* left, void* right)
{
    return builder.CreateOr(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateXor(void* left, void* right)
{
    return builder.CreateXor(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateShl(void* left, void* right)
{
    return builder.CreateShl(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateAShr(void* left, void* right)
{
    return builder.CreateAShr(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateLShr(void* left, void* right)
{
    return builder.CreateLShr(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateICmpEQ(void* left, void* right)
{
    return builder.CreateICmpEQ(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateFCmpOEQ(void* left, void* right)
{
    return builder.CreateFCmpOEQ(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
}

void* LLVMEmitter::CreateICmpULT(void* leftValue, void* rightValue)
{
    return builder.CreateICmpULT(static_cast<llvm::Value*>(leftValue), static_cast<llvm::Value*>(rightValue));
}

void* LLVMEmitter::CreateICmpSLT(void* leftValue, void* rightValue)
{
    return builder.CreateICmpSLT(static_cast<llvm::Value*>(leftValue), static_cast<llvm::Value*>(rightValue));
}

void* LLVMEmitter::CreateFCmpOLT(void* leftValue, void* rightValue)
{
    return builder.CreateFCmpOLT(static_cast<llvm::Value*>(leftValue), static_cast<llvm::Value*>(rightValue));
}

void* LLVMEmitter::CreateSExt(void* operand, void* destinationType)
{
    return builder.CreateSExt(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateZExt(void* operand, void* destinationType)
{
    return builder.CreateZExt(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateFPExt(void* operand, void* destinationType)
{
    return builder.CreateFPExt(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateTrunc(void* operand, void* destinationType)
{
    return builder.CreateTrunc(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateFPTrunc(void* operand, void* destinationType)
{
    return builder.CreateFPTrunc(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateBitCast(void* operand, void* destinationType)
{
    return builder.CreateBitCast(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateUIToFP(void* operand, void* destinationType)
{
    return builder.CreateUIToFP(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateSIToFP(void* operand, void* destinationType)
{
    return builder.CreateSIToFP(static_cast<llvm::Value*> (operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateFPToUI(void* operand, void* destinationType)
{
    return builder.CreateFPToUI(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateFPToSI(void* operand, void* destinationType)
{
    return builder.CreateFPToSI(static_cast<llvm::Value*>(operand), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateIntToPtr(void* intValue, void* destinationType)
{
    return builder.CreateIntToPtr(static_cast<llvm::Value*>(intValue), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreatePtrToInt(void* ptrValue, void* destinationType)
{
    return builder.CreatePtrToInt(static_cast<llvm::Value*>(ptrValue), static_cast<llvm::Type*>(destinationType));
}

void* LLVMEmitter::CreateNot(void* value)
{
    return builder.CreateNot(static_cast<llvm::Value*>(value));
}

void* LLVMEmitter::CreateNeg(void* value)
{
    return builder.CreateNeg(static_cast<llvm::Value*>(value));
}

void* LLVMEmitter::CreateFNeg(void* value)
{
    return builder.CreateFNeg(static_cast<llvm::Value*>(value));
}

void* LLVMEmitter::CreateNop()
{
    return nullptr;
}

void* LLVMEmitter::CreateSave()
{
    return nullptr;
}

void* LLVMEmitter::GetConversionValue(void* type, void* from)
{
    return nullptr;
}

void* LLVMEmitter::CreateFwdIrTypeForClassType()
{
    return llvm::StructType::create(context);
}

void LLVMEmitter::SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes)
{
    llvm::StructType* structType = static_cast<llvm::StructType*>(forwardDeclaredType);
    std::vector<llvm::Type*> elementIrTypes;
    for (void* elementType : elementTypes)
    {
        elementIrTypes.push_back(static_cast<llvm::Type*>(elementType));
    }
    structType->setBody(elementIrTypes);
}

void* LLVMEmitter::GetIrTypeByTypeId(const util::uuid& typeId)
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

void LLVMEmitter::SetIrTypeByTypeId(const util::uuid& typeId, void* irType)
{
    irTypeTypeIdMap[typeId] = static_cast<llvm::Type*>(irType);
}

void* LLVMEmitter::GetIrTypeForPtrType(void* baseIrType)
{
    return llvm::PointerType::get(static_cast<llvm::Type*>(baseIrType), 0);
}

std::string LLVMEmitter::GetIrTypeName(void* irType)
{
    return std::string();
}

std::string LLVMEmitter::MakeVmtVariableName(const std::string& vmtObjectName)
{
    return std::string();
}

void* LLVMEmitter::CreateDITypeForBool()
{
    return diBuilder->createBasicType("bool", 8, llvm::dwarf::DW_ATE_boolean);
}

void* LLVMEmitter::CreateDITypeForSByte()
{
    return diBuilder->createBasicType("sbyte", 8, llvm::dwarf::DW_ATE_signed);
}

void* LLVMEmitter::CreateDITypeForByte()
{
    return diBuilder->createBasicType("byte", 8, llvm::dwarf::DW_ATE_unsigned);
}

void* LLVMEmitter::CreateDITypeForShort()
{
    return diBuilder->createBasicType("short", 16, llvm::dwarf::DW_ATE_signed);
}

void* LLVMEmitter::CreateDITypeForUShort()
{
    return diBuilder->createBasicType("ushort", 16, llvm::dwarf::DW_ATE_unsigned);
}

void* LLVMEmitter::CreateDITypeForInt()
{
    return diBuilder->createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
}

void* LLVMEmitter::CreateDITypeForUInt()
{
    return diBuilder->createBasicType("uint", 32, llvm::dwarf::DW_ATE_unsigned);
}

void* LLVMEmitter::CreateDITypeForLong()
{
    return diBuilder->createBasicType("long", 64, llvm::dwarf::DW_ATE_signed);
}

void* LLVMEmitter::CreateDITypeForULong()
{
    return diBuilder->createBasicType("ulong", 64, llvm::dwarf::DW_ATE_unsigned);
}

void* LLVMEmitter::CreateDITypeForFloat()
{
    return diBuilder->createBasicType("float", 32, llvm::dwarf::DW_ATE_float);
}

void* LLVMEmitter::CreateDITypeForDouble()
{
    return diBuilder->createBasicType("double", 64, llvm::dwarf::DW_ATE_float);
}

void* LLVMEmitter::CreateDITypeForChar()
{
    return diBuilder->createBasicType("char", 8, llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLVMEmitter::CreateDITypeForWChar()
{
    return diBuilder->createBasicType("wchar", 16, llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLVMEmitter::CreateDITypeForUChar()
{
    return diBuilder->createBasicType("uchar", 32, llvm::dwarf::DW_ATE_unsigned_char);
}

void* LLVMEmitter::CreateDITypeForVoid()
{
    return diBuilder->createUnspecifiedType("void");
}

void* LLVMEmitter::CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements)
{
    // todo...
    std::vector<llvm::Metadata*> metadataElements;
    return diBuilder->createArrayType(elements.size(), 8, static_cast<llvm::DIType*>(elementDIType), diBuilder->getOrCreateArray(metadataElements));
}

#if (LLVM_VERSION_MAJOR >= 10)

void* LLVMEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName, 
    const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    uint64_t sizeInBits = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)))->getSizeInBits();
    uint64_t alignInBits = 8 * dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)))->getAlignment().value();
    uint64_t offsetInBits = 0;
    return diBuilder->createReplaceableCompositeType(llvm::dwarf::DW_TAG_class_type, name, nullptr,
        static_cast<llvm::DIFile*>(GetDebugInfoForFile(fullSpan)), lineColLen.line,
        0, sizeInBits, alignInBits, llvm::DINode::DIFlags::FlagZero, mangledName);
}

#else

void* LLVMEmitter::CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName)
{
    uint64_t sizeInBits = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)))->getSizeInBits();
    uint32_t alignInBits = 8 * dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)))->getAlignment();
    uint64_t offsetInBits = 0; // todo?
    return diBuilder->createReplaceableCompositeType(llvm::dwarf::DW_TAG_class_type, name, nullptr, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)), sourcePos.line,
        0, sizeInBits, alignInBits, llvm::DINode::DIFlags::FlagZero, mangledName);
}

#endif

void* LLVMEmitter::GetDebugInfoForFile(const soul::ast::FullSpan& fullSpan)
{
    std::string sourceFilePath = GetSourceFilePath(fullSpan.fileIndex, fullSpan.moduleId);
    if (sourceFilePath.empty())
    {
        return diFile;
    }
    llvm::DIFile* file = diBuilder->createFile(util::Path::GetFileName(sourceFilePath), util::Path::GetDirectoryName(sourceFilePath));
    return file;
}

uint64_t LLVMEmitter::GetOffsetInBits(void* classIrType, int layoutIndex)
{
    const llvm::StructLayout* structLayout = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(classIrType)));
    uint64_t offsetInBits = structLayout->getElementOffsetInBits(layoutIndex);
    return offsetInBits;
}

#if (LLVM_VERSION_MAJOR >= 10)

void* LLVMEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan,
    const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    std::vector<llvm::Metadata*> elements;
    const llvm::StructLayout* structLayout = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)));
    for (void* element : memberVariableElements)
    {
        elements.push_back(static_cast<llvm::Metadata*>(element));
    }
    llvm::MDNode* templateParams = nullptr;
    uint64_t sizeInBits = structLayout->getSizeInBits();
    uint64_t alignInBits = 8 * structLayout->getAlignment().value();
    uint64_t offsetInBits = 0; // todo?
    llvm::DINode::DIFlags flags = llvm::DINode::DIFlags::FlagZero;
    return diBuilder->createClassType(static_cast<llvm::DIScope*>(CurrentScope()), name, static_cast<llvm::DIFile*>(GetDebugInfoForFile(fullSpan)),
        lineColLen.line, sizeInBits, alignInBits, offsetInBits,
        flags, static_cast<llvm::DIType*>(baseClassDIType), diBuilder->getOrCreateArray(elements), static_cast<llvm::DIType*>(vtableHolderClass), templateParams, mangledName);
    return nullptr;
}

#else

void* LLVMEmitter::CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan,
    const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType)
{
    std::vector<llvm::Metadata*> elements;
    const llvm::StructLayout* structLayout = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::Type*>(irType)));
    for (void* element : memberVariableElements)
    {
        elements.push_back(static_cast<llvm::Metadata*>(element));
    }
    llvm::MDNode* templateParams = nullptr;
    uint64_t sizeInBits = structLayout->getSizeInBits();
    uint32_t alignInBits = 8 * structLayout->getAlignment();
    uint64_t offsetInBits = 0; // todo?
    llvm::DINode::DIFlags flags = llvm::DINode::DIFlags::FlagZero;
    return diBuilder->createClassType(static_cast<llvm::DIScope*>(CurrentScope()), name, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(classSourcePos, moduleId)), classSourcePos.line, sizeInBits, alignInBits, offsetInBits,
        flags, static_cast<llvm::DIType*>(baseClassDIType), diBuilder->getOrCreateArray(elements), static_cast<llvm::DIType*>(vtableHolderClass), templateParams, mangledName);
}


#endif

void* LLVMEmitter::CreateDITypeForEnumConstant(const std::string& name, int64_t value)
{
    return diBuilder->createEnumerator(name, value);
}

void* LLVMEmitter::CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan,
    const soul::ast::LineColLen& lineColLen, const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType)
{
    std::vector<llvm::Metadata*> elements;
    for (void* element : enumConstantElements)
    {
        elements.push_back(static_cast<llvm::Metadata*>(element));
    }
    return diBuilder->createEnumerationType(nullptr, name, static_cast<llvm::DIFile*>(GetDebugInfoForFile(fullSpan)), lineColLen.line, sizeInBits, alignInBits,
        diBuilder->getOrCreateArray(elements), static_cast<llvm::DIType*>(underlyingDIType), mangledName);
}

void LLVMEmitter::MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId)
{
    fwdDeclarationMap[static_cast<llvm::DIType*>(fwdDeclaration)] = typeId;
}

void* LLVMEmitter::GetDITypeByTypeId(const util::uuid& typeId) const
{
    auto it = diTypeTypeIdMap.find(typeId);
    if (it != diTypeTypeIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLVMEmitter::SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName)
{
    diTypeTypeIdMap[typeId] = static_cast<llvm::DIType*>(diType);
    diTypeNameMap[static_cast<llvm::DIType*>(diType)] = typeName;
    //std::cout << diType << " : " << typeName << "\n";
}

void* LLVMEmitter::GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId)
{
    auto it = diMemberTypeMap.find(memberVariableId);
    if (it != diMemberTypeMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

void LLVMEmitter::SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType)
{
    diMemberTypeMap[memberVariableId] = static_cast<llvm::DIDerivedType*>(diType);
}

void* LLVMEmitter::CreateDIMemberType(void* scope, const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
    uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType)
{
    llvm::DINode::DIFlags flags = llvm::DINode::DIFlags::FlagZero;
    return diBuilder->createMemberType(static_cast<llvm::DIType*>(scope), name, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(fullSpan)), lineColLen.line, sizeInBits, alignInBits, offsetInBits, flags,
        static_cast<llvm::DIType*>(diType));
}

void* LLVMEmitter::CreateConstDIType(void* diType)
{
    return diBuilder->createQualifiedType(llvm::dwarf::DW_TAG_const_type, static_cast<llvm::DIType*>(diType));
}

void* LLVMEmitter::CreateLValueRefDIType(void* diType)
{
    return diBuilder->createReferenceType(llvm::dwarf::DW_TAG_reference_type, static_cast<llvm::DIType*>(diType));
}

void* LLVMEmitter::CreateRValueRefDIType(void* diType)
{
    return diBuilder->createReferenceType(llvm::dwarf::DW_TAG_rvalue_reference_type, static_cast<llvm::DIType*>(diType));
}

void* LLVMEmitter::CreatePointerDIType(void* diType)
{
    return diBuilder->createPointerType(static_cast<llvm::DIType*>(diType), 64);
}

void* LLVMEmitter::CreateUnspecifiedDIType(const std::string& name)
{
    return diBuilder->createUnspecifiedType(name);
}

void LLVMEmitter::MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className)
{
    if (classPtrMap.find(typeId) == classPtrMap.cend())
    {
        classPtrMap[typeId] = classPtr;
        classNameMap[classPtr] = className;
    }
}

uint64_t LLVMEmitter::GetSizeInBits(void* irType)
{
    return dataLayout->getTypeSizeInBits(static_cast<llvm::Type*>(irType));
}

uint64_t LLVMEmitter::GetAlignmentInBits(void* irType)
{
    return 8 * dataLayout->getABITypeAlignment(static_cast<llvm::Type*>(irType));
}

#if (LLVM_VERSION_MAJOR < 12)

llvm::DebugLoc GetDebugLocation(LLVMEmitter* emitter, const soul::ast::LineColLen& lineColLen)
{
    if (!emitter->diCompileUnit || !sourcePos.IsValid() || !emitter->currentDIBuilder) return llvm::DebugLoc();
    return llvm::DebugLoc::get(lineColLen.line, lineColLen.col, static_cast<llvm::DIScope*>(emitter->CurrentScope()));
}

#else

llvm::DebugLoc GetDebugLocation(LLVMEmitter* emitter, const soul::ast::LineColLen& lineColLen)
{
    if (!emitter->diCompileUnit || !lineColLen.IsValid() || !emitter->currentDIBuilder) return llvm::DebugLoc();
    return llvm::DILocation::get(emitter->context, lineColLen.line, lineColLen.col, static_cast<llvm::DIScope*>(emitter->CurrentScope()));
}

#endif

void LLVMEmitter::PushScope(void* scope)
{
    scopes.push_back(static_cast<llvm::DIScope*>(scope));
}

void LLVMEmitter::PopScope()
{
    scopes.pop_back();
}

void* LLVMEmitter::CurrentScope()
{
    llvm::DIScope* currentScope = diCompileUnit;
    if (!scopes.empty())
    {
        currentScope = scopes.back();
    }
    return currentScope;
}

void LLVMEmitter::SetCurrentDebugLocation(const soul::ast::LineColLen& lineColLen)
{
    if (!diCompileUnit || !currentDIBuilder) return;
    if (inPrologue || !lineColLen.IsValid())
    {
        currentDebugLocation = llvm::DebugLoc();
        builder.SetCurrentDebugLocation(currentDebugLocation);
    }
    else
    {
        currentDebugLocation = GetDebugLocation(this, lineColLen);
        builder.SetCurrentDebugLocation(currentDebugLocation);
    }
}

void LLVMEmitter::SetCurrentDebugLocation(const soul::ast::Span& span)
{
}

void LLVMEmitter::ResetCurrentDebugLocation()
{
    currentDebugLocation = llvm::DebugLoc();
    builder.SetCurrentDebugLocation(currentDebugLocation);
}

void* LLVMEmitter::GetArrayBeginAddress(void* arrayType, void* arrayPtr)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(builder.getInt64(0));
    elementIndeces.push_back(builder.getInt64(0));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* beginPtr = builder.CreateGEP(static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#else
    llvm::Value* beginPtr = builder.CreateGEP(static_cast<llvm::Type*>(arrayType), static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#endif 
    return beginPtr;
}

void* LLVMEmitter::GetArrayEndAddress(void* arrayType, void* arrayPtr, uint64_t size)
{
    ArgVector elementIndeces;
    elementIndeces.push_back(builder.getInt64(0));
    elementIndeces.push_back(builder.getInt64(size));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* endPtr = builder.CreateGEP(static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#else
    llvm::Value* endPtr = builder.CreateGEP(static_cast<llvm::Type*>(arrayType), static_cast<llvm::Value*>(arrayPtr), elementIndeces);
#endif 
    return endPtr;
}

void* LLVMEmitter::CreateBasicBlock(const std::string& name)
{
    return llvm::BasicBlock::Create(context, name, function);
}

void* LLVMEmitter::CreateIncludeBasicBlockInstruction(void* basicBlock)
{
    return nullptr;
}

void LLVMEmitter::PushParentBlock()
{
}

void LLVMEmitter::PopParentBlock()
{
}

void LLVMEmitter::SetHandlerBlock(void* tryBlock, void* catchBlock)
{
}

void LLVMEmitter::SetCleanupBlock(void* cleanupBlock)
{
}

int LLVMEmitter::GetBasicBlockId(void* basicBlock)
{
    return -1;
}

void LLVMEmitter::SetInPrologue(bool inPrologue_)
{
    inPrologue = inPrologue_;
}

void* LLVMEmitter::CreateBeginTry()
{
    return nullptr;
}

void* LLVMEmitter::CreateEndTry(void* nextDest)
{
    return nullptr;
}

void* LLVMEmitter::CreateBeginCatch()
{
    return nullptr;
}

void* LLVMEmitter::CreateEndCatch(void* nextDest)
{
    return nullptr;
}

void* LLVMEmitter::GetOrInsertGlobal(const std::string& name, void* type)
{
    return module->getOrInsertGlobal(name, static_cast<llvm::Type*>(type));
}

void* LLVMEmitter::GetOrInsertAnyComdat(const std::string& name, void* global)
{
    llvm::GlobalVariable* globalVar = llvm::cast<llvm::GlobalVariable>(static_cast<llvm::Constant*>(global));
    llvm::Comdat* comdat = module->getOrInsertComdat(name);
    comdat->setSelectionKind(llvm::Comdat::SelectionKind::Any);
    globalVar->setComdat(comdat);
    return comdat;
}

void* LLVMEmitter::GetOrInsertAnyFunctionComdat(const std::string& name, void* function)
{
    llvm::Comdat* comdat = module->getOrInsertComdat(name);
    comdat->setSelectionKind(llvm::Comdat::SelectionKind::Any);
    static_cast<llvm::Function*>(function)->setComdat(comdat);
    return comdat;
}

void LLVMEmitter::SetInitializer(void* global, void* initializer)
{
    llvm::GlobalVariable* globalVar = llvm::cast<llvm::GlobalVariable>(static_cast<llvm::Constant*>(global));
    globalVar->setInitializer(static_cast<llvm::Constant*>(initializer));
}

void LLVMEmitter::SetPrivateLinkage(void* global)
{
    llvm::GlobalVariable* globalVar = llvm::cast<llvm::GlobalVariable>(static_cast<llvm::Constant*>(global));
    globalVar->setLinkage(llvm::GlobalValue::PrivateLinkage);
}

void* LLVMEmitter::GetOrInsertFunction(const std::string& name, void* type, bool nothrow)
{
#if (LLVM_VERSION_MAJOR >= 9)
    auto callee = module->getOrInsertFunction(name, static_cast<llvm::FunctionType*>(type)).getCallee();
    auto fun = llvm::dyn_cast<llvm::Function>(callee);
    return fun;
#else
    return module->getOrInsertFunction(name, static_cast<llvm::FunctionType*>(type));
#endif
}

void* LLVMEmitter::CreateGlobalStringPtr(const std::string& name)
{
    llvm::Constant* strConstant = llvm::ConstantDataArray::getString(context, name);
    llvm::GlobalVariable* gv = new llvm::GlobalVariable(*module, strConstant->getType(), true, llvm::GlobalValue::PrivateLinkage, strConstant, "", nullptr,
        llvm::GlobalVariable::NotThreadLocal, 0);
    gv->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
    gv->setAlignment(llvm::MaybeAlign(1));
    llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Constant* indices[] = { zero, zero };
    return llvm::ConstantExpr::getInBoundsGetElementPtr(gv->getValueType(), gv, indices);
}

void* LLVMEmitter::CreateGlobalWStringPtr(const std::u16string& name)
{
    return nullptr;
}

void* LLVMEmitter::CreateGlobalUStringPtr(const std::u32string& name)
{
    return nullptr;
}

void* LLVMEmitter::CreateCall(void* functionType, void* callee, const std::vector<void*>& args)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    return builder.CreateCall(static_cast<llvm::FunctionType*>(functionType), static_cast<llvm::Value*>(callee), arguments);
#else
    return builder.CreateCall(static_cast<llvm::Value*>(callee), arguments);
#endif
}

void* LLVMEmitter::CreateCallInst(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    std::vector<llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<llvm::Value*> inputs;
        inputs.push_back(static_cast<llvm::Value*>(bundle));
        bundleDefs.push_back(llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
#if (LLVM_VERSION_MAJOR < 16)
    llvm::CallInst* callInst = llvm::CallInst::Create(llvm::cast<llvm::FunctionType>(
        llvm::cast<llvm::PointerType>(static_cast<llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#else
    llvm::CallInst* callInst = llvm::CallInst::Create(static_cast<llvm::FunctionType*>(functionType),
        static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
#else
    llvm::CallInst* callInst = llvm::CallInst::Create(static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
    if (diBuilder)
    {
        callInst->setDebugLoc(GetDebugLocation(this, lineColLen));
    }
    return callInst;
}

void* LLVMEmitter::CreateCallInstToBasicBlock(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, void* basicBlock, 
    const soul::ast::LineColLen& lineColLen)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    std::vector<llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<llvm::Value*> inputs;
        inputs.push_back(static_cast<llvm::Value*>(bundle));
        bundleDefs.push_back(llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
#if (LLVM_VERSION_MAJOR < 16)
    llvm::CallInst* callInst = llvm::CallInst::Create(llvm::cast<llvm::FunctionType>(
        llvm::cast<llvm::PointerType>(static_cast<llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(basicBlock));
#else
    llvm::CallInst* callInst = llvm::CallInst::Create(static_cast<llvm::FunctionType*>(functionType),
        static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(basicBlock));
#endif
#else
    llvm::CallInst* callInst = llvm::CallInst::Create(static_cast<llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(basicBlock));
#endif
    if (diBuilder)
    {
        callInst->setDebugLoc(GetDebugLocation(this, lineColLen));
    }
    return callInst;
}

void* LLVMEmitter::CreateInvoke(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
#if (LLVM_VERSION_MAJOR >= 11)
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateInvoke(llvm::cast<llvm::FunctionType>(
        llvm::cast<llvm::PointerType>(static_cast<llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<llvm::Value*>(callee), static_cast<llvm::BasicBlock*>(normalBlock), static_cast<llvm::BasicBlock*>(unwindBlock), arguments);
#else
    return builder.CreateInvoke(static_cast<llvm::FunctionType*>(functionType), static_cast<llvm::Value*>(callee),
        static_cast<llvm::BasicBlock*>(normalBlock), static_cast<llvm::BasicBlock*>(unwindBlock), arguments);

#endif
#else
    return builder.CreateInvoke(static_cast<llvm::Value*>(callee), static_cast<llvm::BasicBlock*>(normalBlock), static_cast<llvm::BasicBlock*>(unwindBlock), arguments);
#endif
}

void* LLVMEmitter::CreateInvokeInst(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, const std::vector<void*>& bundles,
    const soul::ast::LineColLen& lineColLen)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    std::vector<llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<llvm::Value*> inputs;
        inputs.push_back(static_cast<llvm::Value*>(bundle));
        bundleDefs.push_back(llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
#if (LLVM_VERSION_MAJOR < 16)
    llvm::InvokeInst* invokeInst = llvm::InvokeInst::Create(llvm::cast<llvm::FunctionType>(
        llvm::cast<llvm::PointerType>(static_cast<llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<llvm::Value*>(callee), static_cast<llvm::BasicBlock*>(normalBlock), static_cast<llvm::BasicBlock*> (unwindBlock),
        arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#else
    llvm::InvokeInst* invokeInst = llvm::InvokeInst::Create(static_cast<llvm::FunctionType*>(functionType),
        static_cast<llvm::Value*>(callee), static_cast<llvm::BasicBlock*>(normalBlock), static_cast<llvm::BasicBlock*> (unwindBlock),
        arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
#else
    llvm::InvokeInst* invokeInst = llvm::InvokeInst::Create(static_cast<llvm::Value*>(callee), static_cast<llvm::BasicBlock*>(normalBlock),
        static_cast<llvm::BasicBlock*>(unwindBlock),
        arguments, bundleDefs, "", static_cast<llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
    if (diBuilder)
    {
        invokeInst->setDebugLoc(GetDebugLocation(this, lineColLen)); 
    }
    return invokeInst;
}

void* LLVMEmitter::GetObjectFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(builder.getInt32(0));
    objectIndeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* objectPtr = builder.CreateGEP(static_cast<llvm::Value*>(classDelegatePtr), objectIndeces);
#else
    llvm::Value* objectPtr = builder.CreateGEP(static_cast<llvm::Type*>(classDelegateType), static_cast<llvm::Value*>(classDelegatePtr), objectIndeces);
#endif
    return objectPtr;
}

void* LLVMEmitter::GetDelegateFromClassDelegate(void* classDelegateType, void* classDelegatePtr)
{
    ArgVector delegateIndeces;
    delegateIndeces.push_back(builder.getInt32(0));
    delegateIndeces.push_back(builder.getInt32(1));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* delegatePtr = builder.CreateGEP(static_cast<llvm::Value*>(classDelegatePtr), delegateIndeces);
#else
    llvm::Value* delegatePtr = builder.CreateGEP(static_cast<llvm::Type*>(classDelegateType), static_cast<llvm::Value*>(classDelegatePtr), delegateIndeces);
#endif
    return delegatePtr;
}

void* LLVMEmitter::GetObjectFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(builder.getInt32(0));
    objectIndeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* objectPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(interfaceTypePtr), objectIndeces);
    llvm::Value* objectPtr = builder.CreateLoad(objectPtrPtr);
#else
    llvm::Value* objectPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(interfaceType), static_cast<llvm::Value*>(interfaceTypePtr), objectIndeces);
    llvm::Value* objectPtr = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), objectPtrPtr);
#endif
    return objectPtr;
}

void* LLVMEmitter::GetObjectPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(builder.getInt32(0));
    objectIndeces.push_back(builder.getInt32(0));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* objectPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(interfaceTypePtr), objectIndeces);
#else
    llvm::Value* objectPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(interfaceType), static_cast<llvm::Value*>(interfaceTypePtr), objectIndeces);
#endif
    return objectPtrPtr;
}

void* LLVMEmitter::GetImtPtrPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    ArgVector interfaceIndeces;
    interfaceIndeces.push_back(builder.getInt32(0));
    interfaceIndeces.push_back(builder.getInt32(1));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* interfacePtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(interfaceTypePtr), interfaceIndeces);
#else
    llvm::Value* interfacePtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(interfaceType), static_cast<llvm::Value*>(interfaceTypePtr), interfaceIndeces);
#endif
    return interfacePtrPtr;
}

void* LLVMEmitter::GetImtPtrFromInterface(void* interfaceType, void* interfaceTypePtr)
{
    ArgVector interfaceIndeces;
    interfaceIndeces.push_back(builder.getInt32(0));
    interfaceIndeces.push_back(builder.getInt32(1));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* interfacePtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(interfaceTypePtr), interfaceIndeces);
    llvm::Value* interfacePtr = builder.CreateLoad(interfacePtrPtr);
#else
    llvm::Value* interfacePtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(interfaceType), static_cast<llvm::Value*>(interfaceTypePtr), interfaceIndeces);
    llvm::Value* interfacePtr = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), interfacePtrPtr);
#endif
    llvm::Value* imtPtr = builder.CreateBitCast(interfacePtr, llvm::PointerType::get(builder.getPtrTy(), 0)); // getInt8PtrTy -> getPtrTy
    return imtPtr;
}

void* LLVMEmitter::GetInterfaceMethod(void* interfaceType, void* imtPtr, int32_t methodIndex, void* interfaceMethodType)
{
    ArgVector methodIndeces;
    methodIndeces.push_back(builder.getInt32(methodIndex));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* methodPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(imtPtr), methodIndeces);
    llvm::Value* methodPtr = builder.CreateLoad(methodPtrPtr);
#else
    llvm::Value* methodPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(interfaceType), static_cast<llvm::Value*>(imtPtr), methodIndeces);
    llvm::Value* methodPtr = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), methodPtrPtr);
#endif
    llvm::Value* callee = builder.CreateBitCast(methodPtr, llvm::PointerType::get(static_cast<llvm::Type*>(interfaceMethodType), 0));
    return callee;
}

void* LLVMEmitter::GetVmtPtr(void* classType, void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType)
{
    ArgVector vmtPtrIndeces;
    vmtPtrIndeces.push_back(builder.getInt32(0));
    vmtPtrIndeces.push_back(builder.getInt32(vmtPtrIndex));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* vmtPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(thisPtr), vmtPtrIndeces);
    llvm::Value* loadedVmtPtr = builder.CreateLoad(vmtPtrPtr);
#else
    llvm::Value* vmtPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(classType), static_cast<llvm::Value*>(thisPtr), vmtPtrIndeces);
    llvm::Value* loadedVmtPtr = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), vmtPtrPtr);
#endif
    void* vmtPtr = builder.CreateBitCast(loadedVmtPtr, static_cast<llvm::Type*>(vmtPtrType));
    return vmtPtr;
}

void* LLVMEmitter::GetMethodPtr(void* vmtType, void* vmtPtr, int32_t vmtIndex)
{
    ArgVector funPtrIndeces;
    funPtrIndeces.push_back(builder.getInt32(0));
    funPtrIndeces.push_back(builder.getInt32(vmtIndex));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* funPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(vmtPtr), funPtrIndeces);
    void* funAsVoidPtr = builder.CreateLoad(funPtrPtr);
#else
    llvm::Value* funPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(vmtType), static_cast<llvm::Value*>(vmtPtr), funPtrIndeces);
    void* funAsVoidPtr = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), funPtrPtr);
#endif
    return funAsVoidPtr;
}

void* LLVMEmitter::GetImtArray(void* vmtType, void* vmtObjectPtr, int32_t imtsVmtIndexOffset)
{
    ArgVector imtsArrayIndeces;
    imtsArrayIndeces.push_back(builder.getInt32(0));
    imtsArrayIndeces.push_back(builder.getInt32(imtsVmtIndexOffset));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* imtsArrayPtrPtr = builder.CreateGEP(static_cast<llvm::Value*>(vmtObjectPtr), imtsArrayIndeces);
#else
    llvm::Value* imtsArrayPtrPtr = builder.CreateGEP(static_cast<llvm::Type*>(vmtType), static_cast<llvm::Value*>(vmtObjectPtr), imtsArrayIndeces);
#endif
    llvm::Value* imtsArrayPtr = builder.CreateBitCast(imtsArrayPtrPtr, 
        llvm::PointerType::get(llvm::PointerType::get(builder.getPtrTy(), 0), 0));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* imtArray = builder.CreateLoad(imtsArrayPtr);
#else
    llvm::Value* imtArray = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), imtsArrayPtr);
#endif
    return imtArray;
}

void* LLVMEmitter::GetImt(void* imtArrayType, void* imtArray, int32_t interfaceIndex)
{
    ArgVector imtArrayIndeces;
    imtArrayIndeces.push_back(builder.getInt32(interfaceIndex));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* imtArrayPtr = builder.CreateGEP(static_cast<llvm::Value*>(imtArray), imtArrayIndeces);
#else
    llvm::Value* imtArrayPtr = builder.CreateGEP(static_cast<llvm::Type*>(imtArrayType), static_cast<llvm::Value*>(imtArray), imtArrayIndeces);
#endif
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* imt = builder.CreateLoad(imtArrayPtr);
#else
    llvm::Value* imt = builder.CreateLoad(static_cast<llvm::Type*>(GetIrTypeForVoidPtrType()), imtArrayPtr);
#endif
    return imt;
}

void* LLVMEmitter::GetMemberVariablePtr(void* classType, void* classPtr, int32_t memberVariableLayoutIndex)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(memberVariableLayoutIndex));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* memberVariablePtr = builder.CreateGEP(static_cast<llvm::Value*>(classPtr), indeces);
#else
    llvm::Value* memberVariablePtr = builder.CreateGEP(static_cast<llvm::Type*>(classType), static_cast<llvm::Value*>(classPtr), indeces);
#endif
    return memberVariablePtr;
}

void* LLVMEmitter::SizeOf(void* elementType, void* ptrType)
{
    void* nullPtr = CreateDefaultIrValueForPtrType(static_cast<llvm::Type*>(ptrType));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* gep = builder.CreateGEP(static_cast<llvm::Value*>(nullPtr), builder.getInt64(1));
#else
    llvm::Value* gep = builder.CreateGEP(static_cast<llvm::Type*>(elementType), static_cast<llvm::Value*>(nullPtr), builder.getInt64(1));
#endif
    llvm::Value* size = builder.CreatePtrToInt(gep, builder.getInt64Ty());
    return size;
}

void* LLVMEmitter::GetClassIdPtr(void* vmtArrayType, void* vmtPtr, int32_t classIdVmtIndexOffset)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(classIdVmtIndexOffset));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* classIdPtr = builder.CreateGEP(static_cast<llvm::Value*>(vmtPtr), indeces);
#else
    llvm::Value* classIdPtr = builder.CreateGEP(static_cast<llvm::Type*>(vmtArrayType), static_cast<llvm::Value*>(vmtPtr), indeces);
#endif
    return classIdPtr;
}

void* LLVMEmitter::GetClassName(void* vmtArrayType, void* vmtPtr, int32_t classNameVmtIndexOffset)
{
    ArgVector indeces;
    indeces.push_back(builder.getInt32(0));
    indeces.push_back(builder.getInt32(classNameVmtIndexOffset));
#if (LLVM_VERSION_MAJOR < 16)
    llvm::Value* classNamePtr = builder.CreateGEP(static_cast<llvm::Value*>(vmtPtr), indeces);
    llvm::Value* className = builder.CreateLoad(classNamePtr);
#else 
    llvm::Type* charPtrPtrType = static_cast<llvm::Type*>(GetIrTypeForPtrType(GetIrTypeForPtrType(GetIrTypeForChar())));
    llvm::Value* classNamePtr = builder.CreateGEP(static_cast<llvm::Type*>(vmtArrayType), static_cast<llvm::Value*>(vmtPtr), indeces);
    llvm::Type* charPtrType = static_cast<llvm::Type*>(GetIrTypeForPtrType(GetIrTypeForChar()));
    llvm::Value* className = builder.CreateLoad(charPtrType, classNamePtr);
#endif
    return className;
}

void* LLVMEmitter::ComputeAddress(void* type, void* ptr, void* index)
{
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreateGEP(static_cast<llvm::Value*>(ptr), static_cast<llvm::Value*>(index));
#else
    return builder.CreateGEP(static_cast<llvm::Type*>(type), static_cast<llvm::Value*>(ptr), static_cast<llvm::Value*>(index));
#endif
}

void* LLVMEmitter::CreatePtrDiff(void* elementType, void* left, void* right)
{
#if (LLVM_VERSION_MAJOR < 16)
    return builder.CreatePtrDiff(static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
#else
    return builder.CreatePtrDiff(static_cast<llvm::Type*>(elementType), static_cast<llvm::Value*>(left), static_cast<llvm::Value*>(right));
#endif
}

uint32_t LLVMEmitter::GetPrivateFlag()
{
    return static_cast<uint32_t>(llvm::DINode::DIFlags::FlagPrivate);
}

uint32_t LLVMEmitter::GetProtectedFlag()
{
    return static_cast<uint32_t>(llvm::DINode::DIFlags::FlagProtected);
}

uint32_t LLVMEmitter::GetPublicFlag()
{
    return static_cast<uint32_t>(llvm::DINode::DIFlags::FlagPublic);
}

uint32_t LLVMEmitter::GetNoFlags()
{
    return static_cast<uint32_t>(llvm::DINode::DIFlags::FlagZero);
}

void* LLVMEmitter::DIBuilder()
{
    return currentDIBuilder;
}

void LLVMEmitter::SetCurrentDIBuilder(void* diBuilder)
{
    currentDIBuilder = static_cast<llvm::DIBuilder*>(diBuilder);
}

void* LLVMEmitter::CreateModule(const std::string& moduleName)
{
    return new llvm::Module(moduleName, context);
}

void LLVMEmitter::DestroyModule(void* module)
{
    delete static_cast<llvm::Module*>(module);
}

void LLVMEmitter::SetModule(void* module_)
{
    module = static_cast<llvm::Module*>(module_);
}

void LLVMEmitter::SetTargetTriple(const std::string& targetTriple)
{
    module->setTargetTriple(targetTriple);
}

void LLVMEmitter::SetDataLayout(void* dataLayout_)
{
    dataLayout = static_cast<llvm::DataLayout*>(dataLayout_);
    module->setDataLayout(*dataLayout);
}

void LLVMEmitter::SetSourceFileName(const std::string& sourceFileName)
{
    module->setSourceFileName(sourceFileName);
}

void LLVMEmitter::SetDICompileUnit(void* diCompileUnit_)
{
    diCompileUnit = static_cast<llvm::DICompileUnit*>(diCompileUnit_);
}

void LLVMEmitter::SetDIFile(void* diFile_)
{
    diFile = static_cast<llvm::DIFile*>(diFile_);
}

void LLVMEmitter::SaveObjectPointer(void* objectPointer_)
{
    if (objectPointer == nullptr)
    {
        objectPointer = static_cast<llvm::Value*>(objectPointer_);
    }
}

void LLVMEmitter::SetObjectPointer(void* objectPointer_)
{
    objectPointer = static_cast<llvm::Value*>(objectPointer_);
}

void* LLVMEmitter::GetObjectPointer()
{
    return objectPointer;
}

void LLVMEmitter::SetFunction(void* function_, int32_t fileIndex, const util::uuid& moduleId, const util::uuid& functionId)
{
    function = static_cast<llvm::Function*>(function_);
}

void LLVMEmitter::SetFunctionName(const std::string& functionName)
{
}

void LLVMEmitter::SetFunctionComment(void* function, const std::string& functionComment)
{
}

void LLVMEmitter::BeginScope()
{
}

void LLVMEmitter::EndScope()
{
}

int16_t LLVMEmitter::GetCurrentScopeId() const
{
    return 0;
}

void LLVMEmitter::SetCurrentScopeId(int16_t scopeId)
{
}

int32_t LLVMEmitter::AddControlFlowGraphNode()
{
    return -1;
}

void LLVMEmitter::SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId)
{
}

void LLVMEmitter::AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId)
{
}

void LLVMEmitter::AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject)
{
}

void LLVMEmitter::BeginInstructionFlag(int16_t flag)
{
}

void LLVMEmitter::EndInstructionFlag(int16_t flag)
{
}

void* LLVMEmitter::CurrentBasicBlock() const
{
    return currentBasicBlock;
}

void LLVMEmitter::SetCurrentBasicBlock(void* currentBasicBlock_)
{
    currentBasicBlock = static_cast<llvm::BasicBlock*>(currentBasicBlock_);
    builder.SetInsertPoint(currentBasicBlock);
}

void LLVMEmitter::StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized)
{
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
#ifdef _WIN32
    module->addModuleFlag(llvm::Module::Warning, "CodeView", 1);
#endif
    diBuilder.reset(new llvm::DIBuilder(*module));
    currentDIBuilder = diBuilder.get();
    llvm::DIFile* sourceFile = diBuilder->createFile(util::Path::GetFileName(sourceFilePath), util::Path::GetDirectoryName(sourceFilePath));
    SetDIFile(sourceFile);
    llvm::DICompileUnit* diCompileUnit = diBuilder->createCompileUnit(cmajorLanguageTag, sourceFile, "Cmajor compiler version " + compilerVersion, optimized, "", 0);
    SetDICompileUnit(diCompileUnit);
    PushScope(sourceFile);
    llvm::DIModule* mod = diBuilder->createModule(static_cast<llvm::DIScope*>(CurrentScope()), module->getName(), "", "", "");
}

void LLVMEmitter::FinalizeDebugInfo()
{
    ReplaceForwardDeclarations();
    diBuilder->finalize();
}

void LLVMEmitter::EndDebugInfo()
{
    PopScope();
    diBuilder.reset();
}

void* LLVMEmitter::CreateDebugInfoForNamespace(void* scope, const std::string& name)
{
    llvm::DINamespace* ns = diBuilder->createNameSpace(static_cast<llvm::DIScope*>(CurrentScope()), name, false);
    return ns;
}

void LLVMEmitter::VerifyModule()
{
    std::string errorMessageStore;
    llvm::raw_string_ostream errorMessage(errorMessageStore);
    if (llvm::verifyModule(*module, &errorMessage))
    {
        throw std::runtime_error("Emitter: verification of module '" + module->getSourceFileName() + "' failed. " + errorMessage.str());
    }
}

#if (LLVM_VERSION_MAJOR <= 12)

void LLVMEmitter::Compile(const std::string& objectFilePath)
{
    llvm::legacy::PassManager passManager;
    std::error_code errorCode;
    llvm::raw_fd_ostream objectFile(objectFilePath, errorCode, llvm::sys::fs::F_None);
#if (LLVM_VERSION_MAJOR >= 10)
    if (static_cast<llvm::TargetMachine*>(impl->emittingContext->TargetMachine())->addPassesToEmitFile(passManager, objectFile, nullptr, llvm::CodeGenFileType::CGFT_ObjectFile))
    {
        throw std::runtime_error("Emitter: cannot emit object code file '" + objectFilePath + "': addPassesToEmitFile failed");
    }
#else
    if (static_cast<llvm::TargetMachine*>(impl->emittingContext->TargetMachine())->addPassesToEmitFile(passManager, objectFile, nullptr,
        llvm::TargetMachine::CodeGenFileType::CGFT_ObjectFile))
    {
        throw std::runtime_error("Emitter: cannot emit object code file '" + objectFilePath + "': addPassesToEmitFile failed");
    }
#endif
    passManager.run(*module);
    objectFile.flush();
    if (objectFile.has_error())
    {
        throw std::runtime_error("Emitter: could not emit object code file '" + objectFilePath + "': " + util::PlatformStringToUtf8(errorCode.message()));
    }
}

#else

void LLVMEmitter::Compile(const std::string& objectFilePath)
{
    llvm::TargetMachine* targetMachine = static_cast<llvm::TargetMachine*>(EmittingContext()->TargetMachine());

    llvm::LoopAnalysisManager loopAnalysisManager;
    llvm::FunctionAnalysisManager functionAnalysisManager;
    llvm::CGSCCAnalysisManager codeGenAnalysisManager;
    llvm::ModuleAnalysisManager moduleAnalysisManager;
    llvm::PassBuilder passBuilder(targetMachine);
    passBuilder.registerModuleAnalyses(moduleAnalysisManager);
    passBuilder.registerCGSCCAnalyses(codeGenAnalysisManager);
    passBuilder.registerFunctionAnalyses(functionAnalysisManager);
    passBuilder.registerLoopAnalyses(loopAnalysisManager);
    passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, codeGenAnalysisManager, moduleAnalysisManager);
    llvm::OptimizationLevel optLevel;
    switch (EmittingContext()->OptimizationLevel())
    {
        case 0:
        {
            optLevel = llvm::OptimizationLevel::O0;
            break;
        }
        case 1:
        {
            optLevel = llvm::OptimizationLevel::O1;
            break;
        }
        case 2:
        {
            optLevel = llvm::OptimizationLevel::O2;
            break;
        }
        case 3:
        {
            optLevel = llvm::OptimizationLevel::O3;
            break;
        }
    }
    llvm::ModulePassManager modulePassManager = passBuilder.buildPerModuleDefaultPipeline(optLevel);

    std::unique_ptr<llvm::raw_fd_ostream> irFile;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm))
    {
        std::error_code llErrorCode;
        irFile.reset(new llvm::raw_fd_ostream(util::Path::ChangeExtension(objectFilePath, ".ll"), llErrorCode));
        modulePassManager.addPass(llvm::PrintModulePass(*irFile, "", false, false));
    }

    modulePassManager.run(*module, moduleAnalysisManager);

    std::error_code errorCode;
    llvm::raw_fd_ostream objectFile(objectFilePath, errorCode);
    llvm::legacy::PassManager codeGenPassManager;
    bool failed = targetMachine->addPassesToEmitFile(codeGenPassManager, objectFile, nullptr, llvm::CGFT_ObjectFile);
    if (failed)
    {
        throw std::runtime_error("LLVM emitter: cannot emit object code file '" + objectFilePath + "': addPassesToEmitFile failed: " +
            util::PlatformStringToUtf8(errorCode.message()));
    }
    codeGenPassManager.run(*module);
}

#endif

void LLVMEmitter::ReplaceForwardDeclarations()
{
    std::unordered_map<llvm::DIType*, util::uuid> currentFwdDeclarationMap;
    std::swap(currentFwdDeclarationMap, fwdDeclarationMap);
    while (!currentFwdDeclarationMap.empty())
    {
        for (const auto& p : currentFwdDeclarationMap)
        {
            llvm::DIType* fwdDeclaration = p.first;
            const util::uuid& typeId = p.second;
            void* diType = GetDITypeByTypeId(typeId);
            if (!diType)
            {
                auto it = classPtrMap.find(typeId);
                if (it != classPtrMap.cend())
                {
                    void* classPtr = it->second;
                    std::string className = classNameMap[classPtr];
                    //std::cout << "> create di type for class" << className << std::endl;
                    diType = CreateClassDIType(classPtr);
                    //std::cout << "< create di type for class" << className << std::endl;
                }
                else
                {
                    throw std::runtime_error("Emitter::ReplaceForwardDeclarations(): class ptr not mapped");
                }
            }
            //std::string typeName = diTypeNameMap[static_cast<llvm::DIType*>(diType)];
            //std::cout << "> replacing " << fwdDeclaration << " by " << typeName << " aka " << diType << std::endl;
            llvm::DIType* llvmDIType = static_cast<llvm::DIType*>(diType);
            fwdDeclaration->replaceAllUsesWith(llvmDIType);
            //std::cout << "< replacing " << fwdDeclaration << " by " << typeName << " aka " << diType << std::endl;
        }
        currentFwdDeclarationMap.clear();
        std::swap(currentFwdDeclarationMap, fwdDeclarationMap);
    }
}

void* LLVMEmitter::GetIrObject(void* symbol) const
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

void LLVMEmitter::SetIrObject(void* symbol, void* irObject)
{
    irObjectMap[symbol] = static_cast<llvm::Value*>(irObject);
}

void* LLVMEmitter::GetFunctionIrType(void* symbol) const
{
    auto it = functionIrTypeMap.find(symbol);
    if (it != functionIrTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLVMEmitter::SetFunctionIrType(void* symbol, void* irType)
{
    functionIrTypeMap[symbol] = static_cast<llvm::FunctionType*>(irType);
}

bool LLVMEmitter::IsVmtObjectCreated(void* symbol) const
{
    return vmtObjectCreatedSet.find(symbol) != vmtObjectCreatedSet.cend();
}

void LLVMEmitter::SetVmtObjectCreated(void* symbol)
{
    vmtObjectCreatedSet.insert(symbol);
}

bool LLVMEmitter::IsStaticObjectCreated(void* symbol) const
{
    return staticObjectCreatedSet.find(symbol) != staticObjectCreatedSet.cend();
}

void LLVMEmitter::SetStaticObjectCreated(void* symbol)
{
    staticObjectCreatedSet.insert(symbol);
}

void* LLVMEmitter::GetStaticObjectType(void* symbol) const
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

void LLVMEmitter::SetStaticObjectType(void* symbol, void* type)
{
    staticTypeMap[symbol] = static_cast<llvm::StructType*>(type);
}

void* LLVMEmitter::GetVmtObjectType(void* symbol) const
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

void LLVMEmitter::SetVmtObjectType(void* symbol, void* vmtObjectType)
{
    vmtObjectTypeMap[symbol] = static_cast<llvm::ArrayType*>(vmtObjectType);
}

std::string LLVMEmitter::GetStaticObjectName(void* symbol) const
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

void LLVMEmitter::SetStaticObjectName(void* symbol, const std::string& staticObjectName)
{
    staticObjectNameMap[symbol] = staticObjectName;
}

std::string LLVMEmitter::GetVmtObjectName(void* symbol) const
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

void LLVMEmitter::SetVmtObjectName(void* symbol, const std::string& vmtObjectName)
{
    vmtObjectNameMap[symbol] = vmtObjectName;
}

std::string LLVMEmitter::GetImtArrayObjectName(void* symbol) const
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

void LLVMEmitter::SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName)
{
    imtArrayObjectNameMap[symbol] = imtArrayObjectName;
}

uint64_t LLVMEmitter::GetClassTypeSizeInBits(void* classIrType)
{
    uint64_t sizeInBits = dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::StructType*>(classIrType)))->getSizeInBits();
    return sizeInBits;
}

#if (LLVM_VERSION_MAJOR >= 10)

uint64_t LLVMEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    uint64_t alignInBits = 8 * dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::StructType*>(classIrType)))->getAlignment().value();
    return alignInBits;
}

#else

uint64_t LLVMEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    uint32_t alignInBits = 8 * dataLayout->getStructLayout(llvm::cast<llvm::StructType>(static_cast<llvm::StructType*>(classIrType)))->getAlignment();
    return alignInBits;
}

#endif

void LLVMEmitter::AddInlineFunctionAttribute(void* function)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    fun->addFnAttr(llvm::Attribute::InlineHint); 
}

void LLVMEmitter::SetFunctionLinkage(void* function, bool setInline)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    if (setInline)
    {
        fun->setLinkage(llvm::GlobalValue::LinkageTypes::WeakODRLinkage);
    }
}

void LLVMEmitter::SetFunctionLinkageToLinkOnceODRLinkage(void* function)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    fun->setLinkage(llvm::GlobalValue::LinkageTypes::LinkOnceODRLinkage);
}

void LLVMEmitter::SetFunctionCallConventionToStdCall(void* function)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    fun->setCallingConv(llvm::CallingConv::X86_StdCall);
}

void* LLVMEmitter::CreateSubroutineType(const std::vector<void*>& elementTypes)
{
    std::vector<llvm::Metadata*> elements;
    for (void* elementType : elementTypes)
    {
        elements.push_back(static_cast<llvm::Metadata*>(elementType));
    }
    llvm::DISubroutineType* subroutineType = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(elements));
    return subroutineType;
}

unsigned LLVMEmitter::GetPureVirtualVirtuality()
{
    return static_cast<unsigned>(llvm::dwarf::DW_VIRTUALITY_pure_virtual);
}

unsigned LLVMEmitter::GetVirtualVirtuality()
{
    return static_cast<unsigned>(llvm::dwarf::DW_VIRTUALITY_virtual);
}

unsigned LLVMEmitter::GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit)
{
    unsigned flags = llvm::DINode::DIFlags::FlagZero;
    if (isStatic)
    {
        flags |= llvm::DINode::FlagStaticMember;
    }
    flags |= accessFlags;
    if (isExplicit)
    {
        flags |= llvm::DINode::FlagExplicit;
    }
    return flags;
}

void* LLVMEmitter::CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
    void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags)
{
    void* subprogram = diBuilder->createMethod(static_cast<llvm::DIScope*>(CurrentScope()), name, mangledName, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(fullSpan)),
        lineColLen.line, static_cast<llvm::DISubroutineType*>(subroutineType), vtableIndex, 0, static_cast<llvm::DIType*>(vtableHolder),
        static_cast<llvm::DINode::DIFlags>(flags), llvm::DISubprogram::SPFlagDefinition);
    return subprogram;
}

void* LLVMEmitter::CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
    void* subroutineType, unsigned flags)
{
    void* subprogram = diBuilder->createFunction(static_cast<llvm::DIScope*>(CurrentScope()), name, mangledName, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(fullSpan)),
        lineColLen.line, static_cast<llvm::DISubroutineType*>(subroutineType), lineColLen.line, static_cast<llvm::DINode::DIFlags>(flags),
        llvm::DISubprogram::SPFlagDefinition);
    return subprogram;
}

void LLVMEmitter::SetDISubprogram(void* function, void* subprogram)
{
    static_cast<llvm::Function*>(function)->setSubprogram(static_cast<llvm::DISubprogram*>(subprogram));
}

void* LLVMEmitter::CreateAlloca(void* irType)
{
    return builder.CreateAlloca(static_cast<llvm::Type*>(irType));
}

void* LLVMEmitter::CreateDIParameterVariable(const std::string& name, int index, 
    const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, void* irType, void* allocaInst)
{
    llvm::DILocalVariable* paramVar = diBuilder->createParameterVariable(static_cast<llvm::DIScope*>(CurrentScope()), name, index, static_cast<llvm::DIFile*>(
        GetDebugInfoForFile(fullSpan)),
        lineColLen.line, static_cast<llvm::DIType*>(irType));
    diBuilder->insertDeclare(static_cast<llvm::Value*>(allocaInst), paramVar, diBuilder->createExpression(), GetDebugLocation(this, lineColLen),
        builder.GetInsertBlock());
    return paramVar;
}

void* LLVMEmitter::CreateDIAutoVariable(const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen, void* irType, void* allocaInst)
{
    llvm::DILocalVariable* localVar = diBuilder->createAutoVariable(static_cast<llvm::DIScope*>(CurrentScope()), name,
        static_cast<llvm::DIFile*>(GetDebugInfoForFile(fullSpan)),
        lineColLen.line, static_cast<llvm::DIType*>(irType));
    diBuilder->insertDeclare(static_cast<llvm::Value*>(allocaInst), localVar, diBuilder->createExpression(), GetDebugLocation(this, lineColLen),
        builder.GetInsertBlock());
    return localVar;
}

void* LLVMEmitter::GetFunctionArgument(void* function, int argumentIndex)
{
    auto it = static_cast<llvm::Function*>(function)->args().begin() + argumentIndex;
    return &*it;
}

void LLVMEmitter::SetDebugLoc(void* callInst)
{
    static_cast<llvm::CallInst*>(callInst)->setDebugLoc(currentDebugLocation);
}

void* LLVMEmitter::CreateRet(void* value)
{
    return builder.CreateRet(static_cast<llvm::Value*> (value));
}

void* LLVMEmitter::CreateRetVoid()
{
    return builder.CreateRetVoid();
}

void LLVMEmitter::SetPersonalityFunction(void* function, void* personalityFunction)
{
    // getInt8PtrTy -> getPtrTy
    static_cast<llvm::Function*>(function)->setPersonalityFn(llvm::ConstantExpr::getBitCast(static_cast<llvm::Function*>(personalityFunction), builder.getPtrTy()));
}

void LLVMEmitter::AddNoUnwindAttribute(void* function)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    fun->addFnAttr(llvm::Attribute::NoUnwind); 
}

void LLVMEmitter::AddUWTableAttribute(void* function)
{
    llvm::Function* fun = static_cast<llvm::Function*>(function);
    //fun->addFnAttr(llvm::Attribute::UWTable);   TODO UWTABLE ATTRIBUTE IS NOT WORKING!!!
}

void* LLVMEmitter::CreateLexicalBlock(const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen)
{
    llvm::DILexicalBlock* block = diBuilder->createLexicalBlock(static_cast<llvm::DIScope*>(CurrentScope()),
        static_cast<llvm::DIFile*>(GetDebugInfoForFile(fullSpan)),
        lineColLen.line, lineColLen.col);
    PushScope(block);
    return block;
}

void* LLVMEmitter::CreateSwitch(void* condition, void* defaultDest, unsigned numCases)
{
    void* switchInst = builder.CreateSwitch(static_cast<llvm::Value*>(condition), static_cast<llvm::BasicBlock*>(defaultDest), numCases);
    return switchInst;
}

void LLVMEmitter::AddCase(void* switchInst, void* caseValue, void* caseDest)
{
    static_cast<llvm::SwitchInst*>(switchInst)->addCase(llvm::cast<llvm::ConstantInt>(static_cast<llvm::Value*>(caseValue)), static_cast<llvm::BasicBlock*>(caseDest));
}

void* LLVMEmitter::CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    return builder.CreateCleanupPad(static_cast<llvm::Value*>(parentPad), arguments);
}

void* LLVMEmitter::CreateCleanupPad(const std::vector<void*>& args)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    return builder.CreateCleanupPad(llvm::ConstantTokenNone::get(context), arguments);
}

void* LLVMEmitter::CreateCleanupRet(void* cleanupPad, void* unwindTarget)
{
    return builder.CreateCleanupRet(llvm::cast<llvm::CleanupPadInst>(static_cast<llvm::Value*>(cleanupPad)), static_cast<llvm::BasicBlock*>(unwindTarget));
}

void* LLVMEmitter::CreateCatchRet(void* catchPad, void* returnTarget)
{
    return builder.CreateCatchRet(llvm::cast<llvm::CatchPadInst>(static_cast<llvm::Value*>(catchPad)), static_cast<llvm::BasicBlock*>(returnTarget));
}

void* LLVMEmitter::CreateCatchSwitch(void* unwindBlock)
{
    return builder.CreateCatchSwitch(llvm::ConstantTokenNone::get(context), static_cast<llvm::BasicBlock*>(unwindBlock), 1);
}

void* LLVMEmitter::CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock)
{
    return builder.CreateCatchSwitch(static_cast<llvm::Value*>(parentPad), static_cast<llvm::BasicBlock*>(unwindBlock), 1);
}

void LLVMEmitter::AddHandlerToCatchSwitch(void* catchSwitch, void* target)
{
    static_cast<llvm::CatchSwitchInst*>(catchSwitch)->addHandler(static_cast<llvm::BasicBlock*>(target));
}

void* LLVMEmitter::CreateCatchPad(void* parentPad, const std::vector<void*>& args)
{
    std::vector<llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<llvm::Value*>(arg));
    }
    return builder.CreateCatchPad(static_cast<llvm::Value*>(parentPad), arguments);
}

void LLVMEmitter::SetLineNumber(int32_t lineNumber)
{
    emittingDelegate->SetLineNumber(lineNumber);
}

void* LLVMEmitter::HandlerBlock()
{
    return emittingDelegate->HandlerBlock();
}

void* LLVMEmitter::CleanupBlock()
{
    return emittingDelegate->CleanupBlock();
}

bool LLVMEmitter::NewCleanupNeeded()
{
    return emittingDelegate->NewCleanupNeeded();
}

void LLVMEmitter::CreateCleanup()
{
    emittingDelegate->CreateCleanup();
}

void* LLVMEmitter::CreateClassDIType(void* classPtr)
{
    return emittingDelegate->CreateClassDIType(classPtr);
}

void* LLVMEmitter::GetGlobalStringPtr(int stringId)
{
    return emittingDelegate->GetGlobalStringPtr(stringId);
}

void* LLVMEmitter::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalWStringConstant(stringId, arrayType);
}

void* LLVMEmitter::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    return emittingDelegate->GetGlobalUStringConstant(stringId, arrayType);
}

void* LLVMEmitter::GetGlobalUuidConstant(int uuidId)
{
    return emittingDelegate->GetGlobalUuidConstant(uuidId);
}

void* LLVMEmitter::GenerateTrap(const std::vector<void*>& args)
{
    return nullptr;
}

void LLVMEmitter::SetCompileUnitId(const std::string& compileUnitId_)
{
    compileUnitId = compileUnitId_;
}

const std::string& LLVMEmitter::CompileUnitId() const
{
    return compileUnitId;
}

void* LLVMEmitter::GetClsIdValue(const std::string& typeId)
{
    return nullptr;
}

void* LLVMEmitter::CreateMDBool(bool value)
{
    return nullptr;
}

void* LLVMEmitter::CreateMDLong(int64_t value)
{
    return nullptr;
}

void* LLVMEmitter::CreateMDString(const std::string& value)
{
    return nullptr;
}

void* LLVMEmitter::CreateMDStructRef(int id)
{
    return nullptr;
}

void* LLVMEmitter::CreateMDStruct()
{
    return nullptr;
}

void* LLVMEmitter::CreateMDBasicBlockRef(void* bb)
{
    return nullptr;
}

int LLVMEmitter::GetMDStructId(void* mdStruct)
{
    return 0;
}

void LLVMEmitter::AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem)
{
}

void LLVMEmitter::SetFunctionMdId(void* function, int mdId)
{
}

void* LLVMEmitter::GetMDStructRefForSourceFile(const std::string& sourceFileName)
{
    return nullptr;
}

void LLVMEmitter::SetMetadataRef(void* inst, void* mdStructRef)
{
}

void LLVMEmitter::FinalizeFunction(void* function, bool hasCleanup)
{
}

int LLVMEmitter::Install(const std::string& str)
{
    return emittingDelegate->Install(str);
}

int LLVMEmitter::Install(const std::u16string& str)
{
    return emittingDelegate->Install(str);
}

int LLVMEmitter::Install(const std::u32string& str)
{
    return emittingDelegate->Install(str);
}

void* LLVMEmitter::CreateLandingPad(void* lpType)
{
    llvm::LandingPadInst* lp = builder.CreateLandingPad(static_cast<llvm::Type*>(lpType), 1);
    return lp;
}

void LLVMEmitter::SetLandindPadAsCleanup(void* landingPad)
{
    llvm::LandingPadInst* lp = static_cast<llvm::LandingPadInst*>(landingPad);
    lp->setCleanup(true);
}

void LLVMEmitter::MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock)
{
    if (lastAlloca)
    {
        static_cast<llvm::AllocaInst*>(allocaInst)->moveAfter(static_cast<llvm::AllocaInst*>(lastAlloca));
    }
    else
    {
        llvm::BasicBlock* block = static_cast<llvm::BasicBlock*>(basicBlock);
        if (block->empty())
        {
            static_cast<llvm::AllocaInst*>(allocaInst)->removeFromParent();
#if (LLVM_VERSION_MAJOR < 16)
            block->getInstList().push_back(static_cast<llvm::AllocaInst*>(allocaInst));
#else
            static_cast<llvm::AllocaInst*>(allocaInst)->insertInto(block, block->end());
#endif
        }
        else
        {
            static_cast<llvm::AllocaInst*>(allocaInst)->moveBefore(block->getTerminator());
        }
    }
}

void LLVMEmitter::AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId)
{
    llvm::LandingPadInst* lp = static_cast<llvm::LandingPadInst*>(landingPad);
    llvm::Constant* typeId = static_cast<llvm::Constant*>(exceptionTypeId);
    lp->addClause(llvm::cast<llvm::Constant>(builder.CreateBitCast(typeId, builder.getPtrTy()))); // getInt8PtrTy -> getPtrTy
}

void* LLVMEmitter::CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces)
{
    return builder.CreateExtractValue(static_cast<llvm::Value*>(aggregate), indeces);
}

void* LLVMEmitter::CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces)
{
    return builder.CreateInsertValue(static_cast<llvm::Value*>(aggregate), static_cast<llvm::Value*>(value), indeces);
}

void* LLVMEmitter::CreateUndefValue(void* type)
{
    return llvm::UndefValue::get(static_cast<llvm::Type*>(type));
}

void LLVMEmitter::CreateResume(void* exception)
{
    builder.CreateResume(static_cast<llvm::Value*>(exception));
}

void LLVMEmitter::DebugPrintDebugInfo(const std::string& filePath)
{
    std::vector<std::string> info;
    for (auto& p : diTypeNameMap)
    {
        std::stringstream s;
        s << std::setw(16) << std::setfill('0');
        s << std::hex << p.first;
        s << std::setw(0);
        s << " = " << p.second;
        info.push_back(s.str());
    }
    std::sort(info.begin(), info.end());
    std::ofstream file(filePath);
    for (const std::string& line : info)
    {
        file << line << std::endl;
    }
}

void LLVMEmitter::BeginSubstituteLineNumber(int32_t lineNumber)
{
    substituteLineNumber = true;
    currentLineNumber = lineNumber;
}

void LLVMEmitter::EndSubstituteLineNumber()
{
    substituteLineNumber = false;
}

void LLVMEmitter::SetBoundCompileUnit(void* boundCompileUnit_)
{
}

void* LLVMEmitter::GetBoundCompileUnit() const
{
    return nullptr;
}

void LLVMEmitter::SetCurrentSourcePos(int32_t lineNumber, int16_t scol, int16_t ecol)
{
}

std::string LLVMEmitter::GetSourceFilePath(int fileIndex, const util::uuid& moduleId)
{
    return emittingDelegate->GetSourceFilePath(fileIndex, moduleId);
}

cmajor::ir::Pad* LLVMEmitter::CurrentPad()
{
    return emittingDelegate->CurrentPad();
}

void LLVMEmitter::PrintModule()
{
    module->print(llvm::errs(), nullptr);
}

} // namespace cmllvm
