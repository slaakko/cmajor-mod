// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Object/COFF.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/BitCode/BitcodeWriter.h>
#include <llvm/Target/TargetMachine.h>

module cmajor.llvm.emitter;

import cmajor.llvm.value.stack;
import util;

namespace cmajor::llvm {

using ArgVector = ::llvm::SmallVector<::llvm::Value*, 4>;

const unsigned cmajorLanguageTag = ::llvm::dwarf::DW_LANG_C_plus_plus_11; // closest approximation

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
    ::llvm::Function* function;
    ::llvm::BasicBlock* currentBasicBlock;
    ::llvm::Value* objectPointer;
    bool inPrologue;
    std::vector<::llvm::DIScope*> scopes;
    std::unordered_map<void*, ::llvm::Value*> irObjectMap;
    std::unordered_map<void*, ::llvm::Type*> irTypeMap;
    std::unordered_map<void*, ::llvm::FunctionType*> functionIrTypeMap;
    std::unordered_set<void*> vmtObjectCreatedSet;
    std::unordered_set<void*> staticObjectCreatedSet;
    std::unordered_map<void*, ::llvm::StructType*> staticTypeMap;
    std::unordered_map<void*, ::llvm::ArrayType*> vmtObjectTypeMap;
    std::unordered_map<void*, std::string> staticObjectNameMap;
    std::unordered_map<void*, std::string> vmtObjectNameMap;
    std::unordered_map<void*, std::string> imtArrayObjectNameMap;
    std::string compileUnitId;
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
    int32_t currentLineNumber;
    bool substituteLineNumber;
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
    function(nullptr),
    currentBasicBlock(nullptr),
    objectPointer(nullptr),
    inPrologue(false),
    currentLineNumber(-1),
    substituteLineNumber(false)
{
}

LLvmEmitter::LLvmEmitter(cmajor::ir::EmittingContext* emittingContext_) : impl(new LLvmEmitterImpl(emittingContext_)), emittingDelegate(nullptr)
{
    SetStack(&impl->stack);
}

LLvmEmitter::~LLvmEmitter()
{
    delete impl;
}

cmajor::ir::EmittingContext* LLvmEmitter::EmittingContext() const
{
    return impl->emittingContext;
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
        impl->currentDebugLocation = GetDebugLocation(impl, sourcePos);
        impl->builder.SetCurrentDebugLocation(impl->currentDebugLocation);
    }
}

void LLvmEmitter::ResetCurrentDebugLocation()
{
    impl->currentDebugLocation = ::llvm::DebugLoc();
    impl->builder.SetCurrentDebugLocation(impl->currentDebugLocation);
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

void* LLvmEmitter::CreateBeginTry()
{
    return nullptr;
}

void* LLvmEmitter::CreateEndTry(void* nextDest)
{
    return nullptr;
}

void* LLvmEmitter::CreateBeginCatch()
{
    return nullptr;
}

void* LLvmEmitter::CreateEndCatch(void* nextDest)
{
    return nullptr;
}

void* LLvmEmitter::GetOrInsertGlobal(const std::string& name, void* type)
{
    return impl->module->getOrInsertGlobal(name, static_cast<::llvm::Type*>(type));
}

void* LLvmEmitter::GetOrInsertAnyComdat(const std::string& name, void* global)
{
    ::llvm::GlobalVariable* globalVar = ::llvm::cast<::llvm::GlobalVariable>(static_cast<::llvm::Constant*>(global));
    ::llvm::Comdat* comdat = impl->module->getOrInsertComdat(name);
    comdat->setSelectionKind(::llvm::Comdat::SelectionKind::Any);
    globalVar->setComdat(comdat);
    return comdat;
}

void* LLvmEmitter::GetOrInsertAnyFunctionComdat(const std::string& name, void* function)
{
    ::llvm::Comdat* comdat = impl->module->getOrInsertComdat(name);
    comdat->setSelectionKind(::llvm::Comdat::SelectionKind::Any);
    static_cast<::llvm::Function*>(function)->setComdat(comdat);
    return comdat;
}

void LLvmEmitter::SetInitializer(void* global, void* initializer)
{
    ::llvm::GlobalVariable* globalVar = ::llvm::cast<::llvm::GlobalVariable>(static_cast<::llvm::Constant*>(global));
    globalVar->setInitializer(static_cast<::llvm::Constant*>(initializer));
}

void LLvmEmitter::SetPrivateLinkage(void* global)
{
    ::llvm::GlobalVariable* globalVar = ::llvm::cast<::llvm::GlobalVariable>(static_cast<::llvm::Constant*>(global));
    globalVar->setLinkage(::llvm::GlobalValue::PrivateLinkage);
}

void* LLvmEmitter::GetOrInsertFunction(const std::string& name, void* type, bool nothrow)
{
#if (LLVM_VERSION_MAJOR >= 9)
    return impl->module->getOrInsertFunction(name, static_cast<::llvm::FunctionType*>(type)).getCallee();
#else
    return impl->module->getOrInsertFunction(name, static_cast<::llvm::FunctionType*>(type));
#endif
}

void* LLvmEmitter::CreateGlobalStringPtr(const std::string& name)
{
    ::llvm::Constant* strConstant = ::llvm::ConstantDataArray::getString(impl->Context(), name);
    ::llvm::GlobalVariable* gv = new ::llvm::GlobalVariable(*impl->module, strConstant->getType(), true, ::llvm::GlobalValue::PrivateLinkage, strConstant, "", nullptr, 
        ::llvm::GlobalVariable::NotThreadLocal, 0);
    gv->setUnnamedAddr(::llvm::GlobalValue::UnnamedAddr::Global);
    gv->setAlignment(::llvm::MaybeAlign(1));
    ::llvm::Constant* zero = ::llvm::ConstantInt::get(::llvm::Type::getInt32Ty(impl->Context()), 0);
    ::llvm::Constant* indices[] = { zero, zero };
    return ::llvm::ConstantExpr::getInBoundsGetElementPtr(gv->getValueType(), gv, indices);
}

void* LLvmEmitter::CreateGlobalWStringPtr(const std::u16string& name)
{
    return nullptr;
}

void* LLvmEmitter::CreateGlobalUStringPtr(const std::u32string& name)
{
    return nullptr;
}

void* LLvmEmitter::CreateCall(void* callee, const std::vector<void*>& args)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    return impl->builder.CreateCall(::llvm::cast<::llvm::FunctionType>(static_cast<::llvm::Value*>(callee)->getType()->getPointerElementType()), 
        static_cast<::llvm::Value*>(callee), arguments);
#else
    return impl->builder.CreateCall(static_cast<::llvm::Value*>(callee), arguments);
#endif
}

void* LLvmEmitter::CreateCallInst(void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::SourcePos& sourcePos)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    std::vector<::llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<::llvm::Value*> inputs;
        inputs.push_back(static_cast<::llvm::Value*>(bundle));
        bundleDefs.push_back(::llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    ::llvm::CallInst* callInst = ::llvm::CallInst::Create(::llvm::cast<::llvm::FunctionType>(
        ::llvm::cast<::llvm::PointerType>(static_cast<::llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<::llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(CurrentBasicBlock()));
#else
    ::llvm::CallInst* callInst = ::llvm::CallInst::Create(static_cast<::llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
    if (impl->diBuilder)
    {
        callInst->setDebugLoc(GetDebugLocation(impl, sourcePos));
    }
    return callInst;
}

void* LLvmEmitter::CreateCallInstToBasicBlock(void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, void* basicBlock, const soul::ast::SourcePos& sourcePos)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    std::vector<::llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<::llvm::Value*> inputs;
        inputs.push_back(static_cast<::llvm::Value*>(bundle));
        bundleDefs.push_back(::llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    ::llvm::CallInst* callInst = ::llvm::CallInst::Create(::llvm::cast<::llvm::FunctionType>(
        ::llvm::cast<::llvm::PointerType>(static_cast<::llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<::llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(basicBlock));
#else
    ::llvm::CallInst* callInst = ::llvm::CallInst::Create(static_cast<::llvm::Value*>(callee), arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(basicBlock));
#endif
    if (impl->diBuilder)
    {
        callInst->setDebugLoc(GetDebugLocation(impl, sourcePos));
    }
    return callInst;
}

void* LLvmEmitter::CreateInvoke(void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    return impl->builder.CreateInvoke(::llvm::cast<::llvm::FunctionType>(
        ::llvm::cast<::llvm::PointerType>(static_cast<::llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<::llvm::Value*>(callee), static_cast<::llvm::BasicBlock*>(normalBlock), static_cast<::llvm::BasicBlock*>(unwindBlock), arguments);
#else
    return impl->builder.CreateInvoke(static_cast<::llvm::Value*>(callee), static_cast<::llvm::BasicBlock*>(normalBlock), static_cast<::llvm::BasicBlock*>(unwindBlock), arguments);
#endif
}

void* LLvmEmitter::CreateInvokeInst(void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, const std::vector<void*>& bundles,
    const soul::ast::SourcePos& sourcePos)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    std::vector<::llvm::OperandBundleDef> bundleDefs;
    for (void* bundle : bundles)
    {
        std::vector<::llvm::Value*> inputs;
        inputs.push_back(static_cast<::llvm::Value*>(bundle));
        bundleDefs.push_back(::llvm::OperandBundleDef("funclet", inputs));
    }
#if (LLVM_VERSION_MAJOR >= 11)
    ::llvm::InvokeInst* invokeInst = ::llvm::InvokeInst::Create(::llvm::cast<::llvm::FunctionType>(
        ::llvm::cast<::llvm::PointerType>(static_cast<::llvm::Value*>(callee)->getType())->getElementType()),
        static_cast<::llvm::Value*>(callee), static_cast<::llvm::BasicBlock*>(normalBlock), static_cast<::llvm::BasicBlock* > (unwindBlock),
        arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(CurrentBasicBlock()));
#else
    ::llvm::InvokeInst* invokeInst = ::llvm::InvokeInst::Create(static_cast<::llvm::Value*>(callee), static_cast<::llvm::BasicBlock*>(normalBlock), 
        static_cast<::llvm::BasicBlock*>(unwindBlock),
        arguments, bundleDefs, "", static_cast<::llvm::BasicBlock*>(CurrentBasicBlock()));
#endif
    if (impl->diBuilder)
    {
        invokeInst->setDebugLoc(GetDebugLocation(impl, sourcePos));
    }
    return invokeInst;
}

void* LLvmEmitter::GetObjectFromClassDelegate(void* classDelegatePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(impl->builder.getInt32(0));
    objectIndeces.push_back(impl->builder.getInt32(0));
    ::llvm::Value* objectPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(classDelegatePtr), objectIndeces);
    return objectPtr;
}

void* LLvmEmitter::GetDelegateFromClassDelegate(void* classDelegatePtr)
{
    ArgVector delegateIndeces;
    delegateIndeces.push_back(impl->builder.getInt32(0));
    delegateIndeces.push_back(impl->builder.getInt32(1));
    ::llvm::Value* delegatePtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(classDelegatePtr), delegateIndeces);
    return delegatePtr;
}

void* LLvmEmitter::GetObjectFromInterface(void* interfaceTypePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(impl->builder.getInt32(0));
    objectIndeces.push_back(impl->builder.getInt32(0));
    ::llvm::Value* objectPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(interfaceTypePtr), objectIndeces);
    ::llvm::Value* objectPtr = impl->builder.CreateLoad(objectPtrPtr);
    return objectPtr;
}

void* LLvmEmitter::GetObjectPtrFromInterface(void* interfaceTypePtr)
{
    ArgVector objectIndeces;
    objectIndeces.push_back(impl->builder.getInt32(0));
    objectIndeces.push_back(impl->builder.getInt32(0));
    ::llvm::Value* objectPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(interfaceTypePtr), objectIndeces);
    return objectPtrPtr;
}

void* LLvmEmitter::GetImtPtrPtrFromInterface(void* interfaceTypePtr)
{
    ArgVector interfaceIndeces;
    interfaceIndeces.push_back(impl->builder.getInt32(0));
    interfaceIndeces.push_back(impl->builder.getInt32(1));
    ::llvm::Value* interfacePtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(interfaceTypePtr), interfaceIndeces);
    return interfacePtrPtr;
}

void* LLvmEmitter::GetImtPtrFromInterface(void* interfaceTypePtr)
{
    ArgVector interfaceIndeces;
    interfaceIndeces.push_back(impl->builder.getInt32(0));
    interfaceIndeces.push_back(impl->builder.getInt32(1));
    ::llvm::Value* interfacePtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(interfaceTypePtr), interfaceIndeces);
    ::llvm::Value* interfacePtr = impl->builder.CreateLoad(interfacePtrPtr);
    ::llvm::Value* imtPtr = impl->builder.CreateBitCast(interfacePtr, ::llvm::PointerType::get(impl->builder.getInt8PtrTy(), 0));
    return imtPtr;
}

void* LLvmEmitter::GetInterfaceMethod(void* imtPtr, int32_t methodIndex, void* interfaceMethodType)
{
    ArgVector methodIndeces;
    methodIndeces.push_back(impl->builder.getInt32(methodIndex));
    ::llvm::Value* methodPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(imtPtr), methodIndeces);
    ::llvm::Value* methodPtr = impl->builder.CreateLoad(methodPtrPtr);
    ::llvm::Value* callee = impl->builder.CreateBitCast(methodPtr, ::llvm::PointerType::get(static_cast<::llvm::Type*>(interfaceMethodType), 0));
    return callee;
}

void* LLvmEmitter::GetVmtPtr(void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType)
{
    ArgVector vmtPtrIndeces;
    vmtPtrIndeces.push_back(impl->builder.getInt32(0));
    vmtPtrIndeces.push_back(impl->builder.getInt32(vmtPtrIndex));
    ::llvm::Value* vmtPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(thisPtr), vmtPtrIndeces);
    void* vmtPtr = impl->builder.CreateBitCast(impl->builder.CreateLoad(vmtPtrPtr), static_cast<::llvm::Type*>(vmtPtrType));
    return vmtPtr;
}

void* LLvmEmitter::GetMethodPtr(void* vmtPtr, int32_t vmtIndex)
{
    ArgVector funPtrIndeces;
    funPtrIndeces.push_back(impl->builder.getInt32(0));
    funPtrIndeces.push_back(impl->builder.getInt32(vmtIndex));
    ::llvm::Value* funPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(vmtPtr), funPtrIndeces);
    void* funAsVoidPtr = impl->builder.CreateLoad(funPtrPtr);
    return funAsVoidPtr;
}

void* LLvmEmitter::GetImtArray(void* vmtObjectPtr, int32_t imtsVmtIndexOffset)
{
    ArgVector imtsArrayIndeces;
    imtsArrayIndeces.push_back(impl->builder.getInt32(0));
    imtsArrayIndeces.push_back(impl->builder.getInt32(imtsVmtIndexOffset));
    ::llvm::Value* imtsArrayPtrPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(vmtObjectPtr), imtsArrayIndeces);
    ::llvm::Value* imtsArrayPtr = impl->builder.CreateBitCast(imtsArrayPtrPtr, ::llvm::PointerType::get(::llvm::PointerType::get(impl->builder.getInt8PtrTy(), 0), 0));
    ::llvm::Value* imtArray = impl->builder.CreateLoad(imtsArrayPtr);
    return imtArray;
}

void* LLvmEmitter::GetImt(void* imtArray, int32_t interfaceIndex)
{
    ArgVector imtArrayIndeces;
    imtArrayIndeces.push_back(impl->builder.getInt32(interfaceIndex));
    ::llvm::Value* imtArrayPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(imtArray), imtArrayIndeces);
    ::llvm::Value* imt = impl->builder.CreateLoad(imtArrayPtr);
    return imt;
}

void* LLvmEmitter::GetMemberVariablePtr(void* classPtr, int32_t memberVariableLayoutIndex)
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(memberVariableLayoutIndex));
    ::llvm::Value* memberVariablePtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(classPtr), indeces);
    return memberVariablePtr;
}

void* LLvmEmitter::SizeOf(void* ptrType)
{
    void* nullPtr = CreateDefaultIrValueForPtrType(static_cast<::llvm::Type*>(ptrType));
    ::llvm::Value* gep = impl->builder.CreateGEP(static_cast<::llvm::Value*>(nullPtr), impl->builder.getInt64(1));
    ::llvm::Value* size = impl->builder.CreatePtrToInt(gep, impl->builder.getInt64Ty());
    return size;
}

void* LLvmEmitter::GetClassIdPtr(void* vmtPtr, int32_t classIdVmtIndexOffset)
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(classIdVmtIndexOffset));
    ::llvm::Value* classIdPtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(vmtPtr), indeces);
    return classIdPtr;
}

void* LLvmEmitter::GetClassName(void* vmtPtr, int32_t classNameVmtIndexOffset)
{
    ArgVector indeces;
    indeces.push_back(impl->builder.getInt32(0));
    indeces.push_back(impl->builder.getInt32(classNameVmtIndexOffset));
    ::llvm::Value* classNamePtr = impl->builder.CreateGEP(static_cast<::llvm::Value*>(vmtPtr), indeces);
    ::llvm::Value* className = impl->builder.CreateLoad(classNamePtr);
    return className;
}

void* LLvmEmitter::ComputeAddress(void* ptr, void* index)
{
    return impl->builder.CreateGEP(static_cast<::llvm::Value*>(ptr), static_cast<::llvm::Value*>(index));
}

void* LLvmEmitter::CreatePtrDiff(void* left, void* right)
{
    return impl->builder.CreatePtrDiff(static_cast<::llvm::Value*>(left), static_cast<::llvm::Value*>(right));
}

uint32_t LLvmEmitter::GetPrivateFlag()
{
    return static_cast<uint32_t>(::llvm::DINode::DIFlags::FlagPrivate);
}

uint32_t LLvmEmitter::GetProtectedFlag()
{
    return static_cast<uint32_t>(::llvm::DINode::DIFlags::FlagProtected);
}

uint32_t LLvmEmitter::GetPublicFlag()
{
    return static_cast<uint32_t>(::llvm::DINode::DIFlags::FlagPublic);
}

uint32_t LLvmEmitter::GetNoFlags()
{
    return static_cast<uint32_t>(::llvm::DINode::DIFlags::FlagZero);
}

void* LLvmEmitter::DIBuilder()
{
    return impl->currentDIBuilder;
}

void LLvmEmitter::SetCurrentDIBuilder(void* diBuilder)
{
    impl->currentDIBuilder = static_cast<::llvm::DIBuilder*>(diBuilder);
}

void* LLvmEmitter::CreateModule(const std::string& moduleName)
{
    return new ::llvm::Module(moduleName, impl->Context());
}

void LLvmEmitter::DestroyModule(void* module)
{
    delete static_cast<::llvm::Module*>(module);
}

void LLvmEmitter::SetModule(void* module_)
{
    impl->module = static_cast<::llvm::Module*>(module_);
}

void LLvmEmitter::SetTargetTriple(const std::string& targetTriple)
{
    impl->module->setTargetTriple(targetTriple);
}

void LLvmEmitter::SetDataLayout(void* dataLayout_)
{
    impl->dataLayout = static_cast<::llvm::DataLayout*>(dataLayout_); 
    impl->module->setDataLayout(*impl->dataLayout);
}

void LLvmEmitter::SetSourceFileName(const std::string& sourceFileName)
{
    impl->module->setSourceFileName(sourceFileName);
}

void LLvmEmitter::SetDICompileUnit(void* diCompileUnit_)
{
    impl->diCompileUnit = static_cast<::llvm::DICompileUnit*>(diCompileUnit_);
}

void LLvmEmitter::SetDIFile(void* diFile_)
{
    impl->diFile = static_cast<::llvm::DIFile*>(diFile_);
}

void LLvmEmitter::SaveObjectPointer(void* objectPointer_)
{
    if (impl->objectPointer == nullptr)
    {
        impl->objectPointer = static_cast<::llvm::Value*>(objectPointer_);
    }
}

void LLvmEmitter::SetObjectPointer(void* objectPointer_)
{
    impl->objectPointer = static_cast<::llvm::Value*>(objectPointer_);
}

void* LLvmEmitter::GetObjectPointer()
{
    return impl->objectPointer;
}

void LLvmEmitter::SetFunction(void* function_, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId)
{
    impl->function = static_cast<::llvm::Function*>(function_);
}

void LLvmEmitter::SetFunctionName(const std::string& functionName)
{
}

void LLvmEmitter::BeginScope()
{
}

void LLvmEmitter::EndScope()
{
}

int16_t LLvmEmitter::GetCurrentScopeId() const
{
    return 0;
}

void LLvmEmitter::SetCurrentScopeId(int16_t scopeId)
{
}

int32_t LLvmEmitter::AddControlFlowGraphNode()
{
    return -1;
}

void LLvmEmitter::SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId)
{
}

void LLvmEmitter::AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId)
{
}

void LLvmEmitter::AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject)
{
}

void LLvmEmitter::BeginInstructionFlag(int16_t flag)
{
}

void LLvmEmitter::EndInstructionFlag(int16_t flag)
{
}

void* LLvmEmitter::CurrentBasicBlock() const
{
    return impl->currentBasicBlock;
}

void LLvmEmitter::SetCurrentBasicBlock(void* currentBasicBlock_)
{
    impl->currentBasicBlock = static_cast<::llvm::BasicBlock*>(currentBasicBlock_); 
    impl->builder.SetInsertPoint(impl->currentBasicBlock);
}

void LLvmEmitter::StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized)
{
    impl->module->addModuleFlag(::llvm::Module::Warning, "Debug Info Version", ::llvm::DEBUG_METADATA_VERSION);
#ifdef _WIN32
    impl->module->addModuleFlag(::llvm::Module::Warning, "CodeView", 1);
#endif
    impl->diBuilder.reset(new ::llvm::DIBuilder(*impl->module));
    impl->currentDIBuilder = impl->diBuilder.get();
    ::llvm::DIFile* sourceFile = impl->diBuilder->createFile(util::Path::GetFileName(sourceFilePath), util::Path::GetDirectoryName(sourceFilePath));
    SetDIFile(sourceFile);
    ::llvm::DICompileUnit* diCompileUnit = impl->diBuilder->createCompileUnit(cmajorLanguageTag, sourceFile, "Cmajor compiler version " + compilerVersion, optimized, "", 0);
    SetDICompileUnit(diCompileUnit);
    PushScope(sourceFile);
}

void LLvmEmitter::FinalizeDebugInfo()
{
    ReplaceForwardDeclarations();
    impl->diBuilder->finalize();
}

void LLvmEmitter::EndDebugInfo()
{
    PopScope();
    impl->diBuilder.reset();
}

void* LLvmEmitter::CreateDebugInfoForNamespace(void* scope, const std::string& name)
{
    ::llvm::DINamespace* ns = impl->diBuilder->createNameSpace(static_cast<::llvm::DIScope*>(CurrentScope()), name, false);
    return ns;
}

void LLvmEmitter::EmitIrText(const std::string& filePath)
{
    std::ofstream llFile(filePath);
    ::llvm::raw_os_ostream llOs(llFile);
    impl->module->print(llOs, nullptr);
    llOs.flush();
}

void LLvmEmitter::EmitIrFile(const std::string& filePath)
{
    std::ofstream bcFile(filePath, std::ios::binary);
    ::llvm::raw_os_ostream bcOs(bcFile);
    ::llvm::WriteBitcodeToFile(*impl->module, bcOs);
    bcOs.flush();
}

void LLvmEmitter::Optimize(const std::string& bcFilePath, const std::string& optBCFilePath, const std::string& optimizationFlags)
{
    // todo
}

void LLvmEmitter::Disassemble(const std::string& bcFilePath, const std::string& llFilePath)
{
    // todo
}

void LLvmEmitter::Compile(const std::string& bcFilePath, const std::string& objectFilePath, int optimizationLevel)
{
    // todo
}

void LLvmEmitter::VerifyModule()
{
    std::string errorMessageStore;
    ::llvm::raw_string_ostream errorMessage(errorMessageStore);
    if (::llvm::verifyModule(*impl->module, &errorMessage))
    {
        throw std::runtime_error("Emitter: verification of module '" + impl->module->getSourceFileName() + "' failed. " + errorMessage.str());
    }
}

void LLvmEmitter::EmitObjectCodeFile(const std::string& objectFilePath)
{
    ::llvm::legacy::PassManager passManager;
    std::error_code errorCode;
    ::llvm::raw_fd_ostream objectFile(objectFilePath, errorCode, ::llvm::sys::fs::F_None);
#if (LLVM_VERSION_MAJOR >= 10)
    if (static_cast<::llvm::TargetMachine*>(impl->emittingContext->TargetMachine())->addPassesToEmitFile(passManager, objectFile, nullptr, ::llvm::CodeGenFileType::CGFT_ObjectFile))
    {
        throw std::runtime_error("Emitter: cannot emit object code file '" + objectFilePath + "': addPassesToEmitFile failed");
    }
#else
    if (static_cast<::llvm::TargetMachine*>(impl->emittingContext->TargetMachine())->addPassesToEmitFile(passManager, objectFile, nullptr,
        ::llvm::TargetMachine::CodeGenFileType::CGFT_ObjectFile))
    {
        throw std::runtime_error("Emitter: cannot emit object code file '" + objectFilePath + "': addPassesToEmitFile failed");
    }
#endif
    passManager.run(*impl->module);
    objectFile.flush();
    if (objectFile.has_error())
    {
        throw std::runtime_error("Emitter: could not emit object code file '" + objectFilePath + "': " + util::PlatformStringToUtf8(errorCode.message()));
    }
}

void LLvmEmitter::ReplaceForwardDeclarations()
{
    // todo
}

void* LLvmEmitter::GetIrObject(void* symbol) const
{
    auto it = impl->irObjectMap.find(symbol);
    if (it != impl->irObjectMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("emitter: IR object not found");
    }
}

void LLvmEmitter::SetIrObject(void* symbol, void* irObject)
{
    impl->irObjectMap[symbol] = static_cast<::llvm::Value*>(irObject);
}

void* LLvmEmitter::GetFunctionIrType(void* symbol) const
{
    auto it = impl->functionIrTypeMap.find(symbol);
    if (it != impl->functionIrTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLvmEmitter::SetFunctionIrType(void* symbol, void* irType)
{
    impl->functionIrTypeMap[symbol] = static_cast<::llvm::FunctionType*>(irType);
}

bool LLvmEmitter::IsVmtObjectCreated(void* symbol) const
{
    return impl->vmtObjectCreatedSet.find(symbol) != impl->vmtObjectCreatedSet.cend();
}

void LLvmEmitter::SetVmtObjectCreated(void* symbol)
{
    impl->vmtObjectCreatedSet.insert(symbol);
}

bool LLvmEmitter::IsStaticObjectCreated(void* symbol) const
{
    return impl->staticObjectCreatedSet.find(symbol) != impl->staticObjectCreatedSet.cend();
}

void LLvmEmitter::SetStaticObjectCreated(void* symbol)
{
    impl->staticObjectCreatedSet.insert(symbol);
}

void* LLvmEmitter::GetStaticObjectType(void* symbol) const
{
    auto it = impl->staticTypeMap.find(symbol);
    if (it != impl->staticTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLvmEmitter::SetStaticObjectType(void* symbol, void* type)
{
    impl->staticTypeMap[symbol] = static_cast<::llvm::StructType*>(type);
}

void* LLvmEmitter::GetVmtObjectType(void* symbol) const
{
    auto it = impl->vmtObjectTypeMap.find(symbol);
    if (it != impl->vmtObjectTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LLvmEmitter::SetVmtObjectType(void* symbol, void* vmtObjectType)
{
    impl->vmtObjectTypeMap[symbol] = static_cast<::llvm::ArrayType*>(vmtObjectType);
}

std::string LLvmEmitter::GetStaticObjectName(void* symbol) const
{
    auto it = impl->staticObjectNameMap.find(symbol);
    if (it != impl->staticObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void LLvmEmitter::SetStaticObjectName(void* symbol, const std::string& staticObjectName)
{
    impl->staticObjectNameMap[symbol] = staticObjectName;
}

std::string LLvmEmitter::GetVmtObjectName(void* symbol) const
{
    auto it = impl->vmtObjectNameMap.find(symbol);
    if (it != impl->vmtObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void LLvmEmitter::SetVmtObjectName(void* symbol, const std::string& vmtObjectName)
{
    impl->vmtObjectNameMap[symbol] = vmtObjectName;
}

std::string LLvmEmitter::GetImtArrayObjectName(void* symbol) const
{
    auto it = impl->imtArrayObjectNameMap.find(symbol);
    if (it != impl->imtArrayObjectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void LLvmEmitter::SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName)
{
    impl->imtArrayObjectNameMap[symbol] = imtArrayObjectName;
}

uint64_t LLvmEmitter::GetClassTypeSizeInBits(void* classIrType)
{
    uint64_t sizeInBits = impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::StructType*>(classIrType)))->getSizeInBits();
    return sizeInBits;
}

#if (LLVM_VERSION_MAJOR >= 10)

uint64_t LLvmEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    uint64_t alignInBits = 8 * impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::StructType*>(classIrType)))->getAlignment().value();
    return alignInBits;
}

#else

uint64_t LLvmEmitter::GetClassTypeAlignmentInBits(void* classIrType)
{
    uint32_t alignInBits = 8 * impl->dataLayout->getStructLayout(::llvm::cast<::llvm::StructType>(static_cast<::llvm::StructType*>(classIrType)))->getAlignment();
    return alignInBits;
}

#endif

void LLvmEmitter::AddInlineFunctionAttribute(void* function)
{
    ::llvm::Function* fun = static_cast<::llvm::Function*>(function);
    fun->addFnAttr(::llvm::Attribute::InlineHint);
}

void LLvmEmitter::SetFunctionLinkage(void* function, bool setInline)
{
    ::llvm::Function* fun = static_cast<::llvm::Function*>(function);
    if (setInline)
    {
        fun->setLinkage(::llvm::GlobalValue::LinkageTypes::WeakODRLinkage);
    }
}

void LLvmEmitter::SetFunctionLinkageToLinkOnceODRLinkage(void* function)
{
    ::llvm::Function* fun = static_cast<::llvm::Function*>(function);
    fun->setLinkage(::llvm::GlobalValue::LinkageTypes::LinkOnceODRLinkage);
}

void LLvmEmitter::SetFunctionCallConventionToStdCall(void* function)
{
    ::llvm::Function* fun = static_cast<::llvm::Function*>(function);
    fun->setCallingConv(::llvm::CallingConv::X86_StdCall);
}

void* LLvmEmitter::CreateSubroutineType(const std::vector<void*>& elementTypes)
{
    std::vector<::llvm::Metadata*> elements;
    for (void* elementType : elementTypes)
    {
        elements.push_back(static_cast<::llvm::Metadata*>(elementType));
    }
    ::llvm::DISubroutineType* subroutineType = impl->diBuilder->createSubroutineType(impl->diBuilder->getOrCreateTypeArray(elements));
    return subroutineType;
}

unsigned LLvmEmitter::GetPureVirtualVirtuality()
{
    return static_cast<unsigned>(::llvm::dwarf::DW_VIRTUALITY_pure_virtual);
}

unsigned LLvmEmitter::GetVirtualVirtuality()
{
    return static_cast<unsigned>(::llvm::dwarf::DW_VIRTUALITY_virtual);
}

unsigned LLvmEmitter::GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit)
{
    unsigned flags = ::llvm::DINode::DIFlags::FlagZero;
    if (isStatic)
    {
        flags |= ::llvm::DINode::FlagStaticMember;
    }
    flags |= accessFlags;
    if (isExplicit)
    {
        flags |= ::llvm::DINode::FlagExplicit;
    }
    return flags;
}

void* LLvmEmitter::CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags)
{
    void* subprogram = impl->diBuilder->createMethod(static_cast<::llvm::DIScope*>(CurrentScope()), name, mangledName, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)),
        sourcePos.line, static_cast<::llvm::DISubroutineType*>(subroutineType), vtableIndex, 0, static_cast<::llvm::DIType*>(vtableHolder),
        static_cast<::llvm::DINode::DIFlags>(flags), ::llvm::DISubprogram::SPFlagDefinition);
    return subprogram;
}

void* LLvmEmitter::CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    void* subroutineType, unsigned flags)
{
    void* subprogram = impl->diBuilder->createFunction(static_cast<::llvm::DIScope*>(CurrentScope()), name, mangledName, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)),
        sourcePos.line, static_cast<::llvm::DISubroutineType*>(subroutineType), sourcePos.line, static_cast<::llvm::DINode::DIFlags>(flags),
        ::llvm::DISubprogram::SPFlagDefinition);
    return subprogram;
}

void LLvmEmitter::SetDISubprogram(void* function, void* subprogram)
{
    static_cast<::llvm::Function*>(function)->setSubprogram(static_cast<::llvm::DISubprogram*>(subprogram));
}

void* LLvmEmitter::CreateAlloca(void* irType)
{
    return impl->builder.CreateAlloca(static_cast<::llvm::Type*>(irType));
}

void* LLvmEmitter::CreateDIParameterVariable(const std::string& name, int index, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, void* irType, void* allocaInst)
{
    ::llvm::DILocalVariable* paramVar = impl->diBuilder->createParameterVariable(static_cast<::llvm::DIScope*>(CurrentScope()), name, index, static_cast<::llvm::DIFile*>(
        GetDebugInfoForFile(sourcePos, moduleId)),
        sourcePos.line, static_cast<::llvm::DIType*>(irType));
    ::llvm::SmallVector<int64_t, 13> expr; // todo
    impl->diBuilder->insertDeclare(static_cast<::llvm::Value*>(allocaInst), paramVar, impl->diBuilder->createExpression(expr), GetDebugLocation(impl, sourcePos), 
        impl->builder.GetInsertBlock());
    return paramVar;
}

void* LLvmEmitter::CreateDIAutoVariable(const std::string& name, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, void* irType, void* allocaInst)
{
    ::llvm::DILocalVariable* localVar = impl->diBuilder->createAutoVariable(static_cast<::llvm::DIScope*>(CurrentScope()), name, 
        static_cast<::llvm::DIFile*>(GetDebugInfoForFile(sourcePos, moduleId)),
        sourcePos.line, static_cast<::llvm::DIType*>(irType));
    ::llvm::SmallVector<int64_t, 13> expr; // todo
    impl->diBuilder->insertDeclare(static_cast<::llvm::Value*>(allocaInst), localVar, impl->diBuilder->createExpression(expr), GetDebugLocation(impl, sourcePos), 
        impl->builder.GetInsertBlock());
    return localVar;
}

void* LLvmEmitter::GetFunctionArgument(void* function, int argumentIndex)
{
    auto it = static_cast<::llvm::Function*>(function)->args().begin() + argumentIndex;
    return &*it;
}

void LLvmEmitter::SetDebugLoc(void* callInst)
{
    static_cast<::llvm::CallInst*>(callInst)->setDebugLoc(impl->currentDebugLocation);
}

void* LLvmEmitter::CreateRet(void* value)
{
    return impl->builder.CreateRet(static_cast<::llvm::Value* > (value));
}

void* LLvmEmitter::CreateRetVoid()
{
    return impl->builder.CreateRetVoid();
}

void LLvmEmitter::SetPersonalityFunction(void* function, void* personalityFunction)
{
    static_cast<::llvm::Function*>(function)->setPersonalityFn(::llvm::ConstantExpr::getBitCast(static_cast<::llvm::Function*>(personalityFunction), impl->builder.getInt8PtrTy()));
}

void LLvmEmitter::AddNoUnwindAttribute(void* function)
{
    static_cast<::llvm::Function*>(function)->addFnAttr(::llvm::Attribute::NoUnwind);
}

void LLvmEmitter::AddUWTableAttribute(void* function)
{
    static_cast<::llvm::Function*>(function)->addFnAttr(::llvm::Attribute::UWTable);
}

void* LLvmEmitter::CreateLexicalBlock(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    ::llvm::DILexicalBlock* block = impl->diBuilder->createLexicalBlock(static_cast<::llvm::DIScope*>(CurrentScope()), 
        static_cast<::llvm::DIFile*>(GetDebugInfoForFile(sourcePos, moduleId)),
        sourcePos.line, sourcePos.col);
    PushScope(block);
    return block;
}

void* LLvmEmitter::CreateSwitch(void* condition, void* defaultDest, unsigned numCases)
{
    void* switchInst = impl->builder.CreateSwitch(static_cast<::llvm::Value*>(condition), static_cast<::llvm::BasicBlock*>(defaultDest), numCases);
    return switchInst;
}

void LLvmEmitter::AddCase(void* switchInst, void* caseValue, void* caseDest)
{
    static_cast<::llvm::SwitchInst*>(switchInst)->addCase(::llvm::cast<::llvm::ConstantInt>(static_cast<::llvm::Value*>(caseValue)), static_cast<::llvm::BasicBlock*>(caseDest));
}

void* LLvmEmitter::CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    return impl->builder.CreateCleanupPad(static_cast<::llvm::Value*>(parentPad), arguments);
}

void* LLvmEmitter::CreateCleanupPad(const std::vector<void*>& args)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    return impl->builder.CreateCleanupPad(::llvm::ConstantTokenNone::get(impl->Context()), arguments);
}

void* LLvmEmitter::CreateCleanupRet(void* cleanupPad, void* unwindTarget)
{
    return impl->builder.CreateCleanupRet(::llvm::cast<::llvm::CleanupPadInst>(static_cast<::llvm::Value*>(cleanupPad)), static_cast<::llvm::BasicBlock*>(unwindTarget));
}

void* LLvmEmitter::CreateCatchRet(void* catchPad, void* returnTarget)
{
    return impl->builder.CreateCatchRet(::llvm::cast<::llvm::CatchPadInst>(static_cast<::llvm::Value*>(catchPad)), static_cast<::llvm::BasicBlock*>(returnTarget));
}

void* LLvmEmitter::CreateCatchSwitch(void* unwindBlock)
{
    return impl->builder.CreateCatchSwitch(::llvm::ConstantTokenNone::get(impl->Context()), static_cast<::llvm::BasicBlock*>(unwindBlock), 1);
}

void* LLvmEmitter::CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock)
{
    return impl->builder.CreateCatchSwitch(static_cast<::llvm::Value*>(parentPad), static_cast<::llvm::BasicBlock*>(unwindBlock), 1);
}

void LLvmEmitter::AddHandlerToCatchSwitch(void* catchSwitch, void* target)
{
    static_cast<::llvm::CatchSwitchInst*>(catchSwitch)->addHandler(static_cast<::llvm::BasicBlock*>(target));
}

void* LLvmEmitter::CreateCatchPad(void* parentPad, const std::vector<void*>& args)
{
    std::vector<::llvm::Value*> arguments;
    for (void* arg : args)
    {
        arguments.push_back(static_cast<::llvm::Value*>(arg));
    }
    return impl->builder.CreateCatchPad(static_cast<::llvm::Value*>(parentPad), arguments);
}

void LLvmEmitter::SetLineNumber(int32_t lineNumber)
{
    emittingDelegate->SetLineNumber(lineNumber);
}

void* LLvmEmitter::HandlerBlock()
{
    return emittingDelegate->HandlerBlock();
}

void* LLvmEmitter::CleanupBlock()
{
    return emittingDelegate->CleanupBlock();
}

bool LLvmEmitter::NewCleanupNeeded()
{
    return emittingDelegate->NewCleanupNeeded();
}

void LLvmEmitter::CreateCleanup()
{
    emittingDelegate->CreateCleanup();
}

void* LLvmEmitter::CreateClassDIType(void* classPtr)
{
    return emittingDelegate->CreateClassDIType(classPtr);
}

void* LLvmEmitter::GetGlobalStringPtr(int stringId)
{
    return emittingDelegate->GetGlobalStringPtr(stringId);
}

void* LLvmEmitter::GetGlobalWStringConstant(int stringId)
{
    return emittingDelegate->GetGlobalWStringConstant(stringId);
}

void* LLvmEmitter::GetGlobalUStringConstant(int stringId)
{
    return emittingDelegate->GetGlobalUStringConstant(stringId);
}

void* LLvmEmitter::GetGlobalUuidConstant(int uuidId)
{
    return emittingDelegate->GetGlobalUuidConstant(uuidId);
}

void* LLvmEmitter::GenerateTrap(const std::vector<void*>& args)
{
    return nullptr;
}

void LLvmEmitter::SetCompileUnitId(const std::string& compileUnitId_)
{
    impl->compileUnitId = compileUnitId_;
}

void* LLvmEmitter::GetClsIdValue(const std::string& typeId)
{
    return nullptr;
}

void* LLvmEmitter::CreateMDBool(bool value)
{
    return nullptr;
}

void* LLvmEmitter::CreateMDLong(int64_t value)
{
    return nullptr;
}

void* LLvmEmitter::CreateMDString(const std::string& value)
{
    return nullptr;
}

void* LLvmEmitter::CreateMDStructRef(int id)
{
    return nullptr;
}

void* LLvmEmitter::CreateMDStruct()
{
    return nullptr;
}

void* LLvmEmitter::CreateMDBasicBlockRef(void* bb)
{
    return nullptr;
}

int LLvmEmitter::GetMDStructId(void* mdStruct)
{
    return 0;
}

void LLvmEmitter::AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem)
{
}

void LLvmEmitter::SetFunctionMdId(void* function, int mdId)
{
}

void* LLvmEmitter::GetMDStructRefForSourceFile(const std::string& sourceFileName)
{
    return nullptr;
}

void LLvmEmitter::SetMetadataRef(void* inst, void* mdStructRef)
{
}

void LLvmEmitter::FinalizeFunction(void* function, bool hasCleanup)
{
}

int LLvmEmitter::Install(const std::string& str)
{
    return emittingDelegate->Install(str);
}

int LLvmEmitter::Install(const std::u16string& str)
{
    return emittingDelegate->Install(str);
}

int LLvmEmitter::Install(const std::u32string& str)
{
    return emittingDelegate->Install(str);
}

void* LLvmEmitter::CreateLandingPad(void* lpType)
{
    ::llvm::LandingPadInst* lp = impl->builder.CreateLandingPad(static_cast<::llvm::Type*>(lpType), 1);
    return lp;
}

void LLvmEmitter::SetLandindPadAsCleanup(void* landingPad)
{
    ::llvm::LandingPadInst* lp = static_cast<::llvm::LandingPadInst*>(landingPad);
    lp->setCleanup(true);
}

void LLvmEmitter::MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock)
{
    if (lastAlloca)
    {
        static_cast<::llvm::AllocaInst*>(allocaInst)->moveAfter(static_cast<::llvm::AllocaInst*>(lastAlloca));
    }
    else
    {
        ::llvm::BasicBlock* block = static_cast<::llvm::BasicBlock*>(basicBlock);
        if (block->empty())
        {
            static_cast<::llvm::AllocaInst*>(allocaInst)->removeFromParent();
            block->getInstList().push_back(static_cast<::llvm::AllocaInst*>(allocaInst));
        }
        else
        {
            static_cast<::llvm::AllocaInst*>(allocaInst)->moveBefore(block->getTerminator());
        }
    }
}

void LLvmEmitter::AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId)
{
    ::llvm::LandingPadInst* lp = static_cast<::llvm::LandingPadInst*>(landingPad);
    ::llvm::Constant* typeId = static_cast<::llvm::Constant*>(exceptionTypeId);
    lp->addClause(::llvm::cast<::llvm::Constant>(impl->builder.CreateBitCast(typeId, impl->builder.getInt8PtrTy())));
}

void* LLvmEmitter::CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces)
{
    return impl->builder.CreateExtractValue(static_cast<::llvm::Value*>(aggregate), indeces);
}

void* LLvmEmitter::CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces)
{
    return impl->builder.CreateInsertValue(static_cast<::llvm::Value*>(aggregate), static_cast<::llvm::Value*>(value), indeces);
}

void* LLvmEmitter::CreateUndefValue(void* type)
{
    return ::llvm::UndefValue::get(static_cast<::llvm::Type*>(type));
}

void LLvmEmitter::CreateResume(void* exception)
{
    impl->builder.CreateResume(static_cast<::llvm::Value*>(exception));
}

void LLvmEmitter::DebugPrintDebugInfo(const std::string& filePath)
{
    std::vector<std::string> info;
    for (auto& p : impl->diTypeNameMap)
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

void LLvmEmitter::BeginSubstituteLineNumber(int32_t lineNumber)
{
    impl->substituteLineNumber = true;
    impl->currentLineNumber = lineNumber;
}

void LLvmEmitter::EndSubstituteLineNumber()
{
    impl->substituteLineNumber = false;
}

void LLvmEmitter::SetBoundCompileUnit(void* boundCompileUnit_) 
{
}

void* LLvmEmitter::GetBoundCompileUnit() const 
{ 
    return nullptr; 
}

void LLvmEmitter::SetCurrentSourcePos(int32_t lineNumber, int16_t scol, int16_t ecol)
{
}

std::string LLvmEmitter::GetSourceFilePath(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    return emittingDelegate->GetSourceFilePath(sourcePos, moduleId);
}

cmajor::ir::Pad* LLvmEmitter::CurrentPad()
{
    return emittingDelegate->CurrentPad();
}

} // cmajor::llvm
