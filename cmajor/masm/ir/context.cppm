// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.context;

import cmajor.masm.ir.type;
import cmajor.masm.ir.data;
import cmajor.masm.ir.metadata;
import cmajor.masm.ir.value;
import cmajor.masm.ir.function;
import cmajor.masm.ir.basic_block;
import cmajor.masm.ir.instruction;
import soul.ast.span;
import std.core;

export namespace cmajor::masm::ir {

class Context
{
public:
    Context();
    Type* GetVoidType() { return typeRepository.GetVoidType(); }
    Type* GetBoolType() { return typeRepository.GetBoolType(); }
    Type* GetSByteType() { return typeRepository.GetSByteType(); }
    Type* GetByteType() { return typeRepository.GetByteType(); }
    Type* GetShortType() { return typeRepository.GetShortType(); }
    Type* GetUShortType() { return typeRepository.GetUShortType(); }
    Type* GetIntType() { return typeRepository.GetIntType(); }
    Type* GetUIntType() { return typeRepository.GetUIntType(); }
    Type* GetLongType() { return typeRepository.GetLongType(); }
    Type* GetULongType() { return typeRepository.GetULongType(); }
    Type* GetFloatType() { return typeRepository.GetFloatType(); }
    Type* GetDoubleType() { return typeRepository.GetDoubleType(); }
    Type* GetPtrType(Type* baseType) { return typeRepository.GetPtrType(baseType); }
    Type* GetStructureType(const std::vector<Type*>& memberTypes) { return typeRepository.GetStructureType(memberTypes); }
    Type* CreateStructureType() { return typeRepository.CreateStructureType(); }
    Type* GetArrayType(Type* elementType, uint64_t size) { return typeRepository.GetArrayType(elementType, size); }
    Type* GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes) { return typeRepository.GetFunctionType(returnType, paramTypes); }
    Value* GetDefaultBoolValue() { return GetBoolType()->DefaultValue(); }
    Value* GetDefaultSByteValue() { return GetSByteType()->DefaultValue(); }
    Value* GetDefaultByteValue() { return GetByteType()->DefaultValue(); }
    Value* GetDefaultShortValue() { return GetShortType()->DefaultValue(); }
    Value* GetDefaultUShortValue() { return GetUShortType()->DefaultValue(); }
    Value* GetDefaultIntValue() { return GetIntType()->DefaultValue(); }
    Value* GetDefaultUIntValue() { return GetUIntType()->DefaultValue(); }
    Value* GetDefaultLongValue() { return GetLongType()->DefaultValue(); }
    Value* GetDefaultULongValue() { return GetULongType()->DefaultValue(); }
    Value* GetDefaultFloatValue() { return GetFloatType()->DefaultValue(); }
    Value* GetDefaultDoubleValue() { return GetDoubleType()->DefaultValue(); }
    Value* GetNullValue(PtrType* ptrType) { return ptrType->DefaultValue(); }
    Value* GetBoolValue(bool value);
    Value* GetSByteValue(int8_t value);
    Value* GetByteValue(uint8_t value);
    Value* GetShortValue(int16_t value);
    Value* GetUShortValue(uint16_t value);
    Value* GetIntValue(int32_t value);
    Value* GetUIntValue(uint32_t value);
    Value* GetLongValue(int64_t value);
    Value* GetULongValue(uint64_t value);
    Value* GetFloatValue(float value);
    Value* GetDoubleValue(double value);
    ArrayValue* GetArrayValue(Type* arrayType, const std::vector<Value*>& elements, const std::string& prefix);
    StructureValue* GetStructureValue(Type* structureType, const std::vector<Value*>& members);
    StringValue* GetStringValue(Type* stringType, const std::string& value);
    ConversionValue* GetConversionValue(Type* type, Value* from);
    ClsIdValue* GetClsIdValue(const std::string& typeId);
    void SetCurrentBasicBlock(BasicBlock* bb) { currentBasicBlock = bb; }
    BasicBlock* GetCurrentBasicBlock() const { return currentBasicBlock; }
    Instruction* CreateNot(Value* arg);
    Instruction* CreateNeg(Value* arg);
    Instruction* CreateAdd(Value* left, Value* right);
    Instruction* CreateSub(Value* left, Value* right);
    Instruction* CreateMul(Value* left, Value* right);
    Instruction* CreateDiv(Value* left, Value* right);
    Instruction* CreateMod(Value* left, Value* right);
    Instruction* CreateAnd(Value* left, Value* right);
    Instruction* CreateOr(Value* left, Value* right);
    Instruction* CreateXor(Value* left, Value* right);
    Instruction* CreateShl(Value* left, Value* right);
    Instruction* CreateShr(Value* left, Value* right);
    Instruction* CreateEqual(Value* left, Value* right);
    Instruction* CreateLess(Value* left, Value* right);
    Instruction* CreateSignExtend(Value* arg, Type* destType);
    Instruction* CreateZeroExtend(Value* arg, Type* destType);
    Instruction* CreateTruncate(Value* arg, Type* destType);
    Instruction* CreateBitCast(Value* arg, Type* destType);
    Instruction* CreateIntToFloat(Value* arg, Type* destType);
    Instruction* CreateFloatToInt(Value* arg, Type* destType);
    Instruction* CreateIntToPtr(Value* arg, Type* destType);
    Instruction* CreatePtrToInt(Value* arg, Type* destType);
    Instruction* CreateParam(Type* type);
    Instruction* CreateLocal(Type* type);
    Instruction* CreateLoad(Value* ptr);
    Instruction* CreateStore(Value* value, Value* ptr);
    Instruction* CreateArg(Value* arg);
    Instruction* CreateElemAddr(Value* ptr, Value* index);
    Instruction* CreatePtrOffset(Value* ptr, Value* offset);
    Instruction* CreatePtrDiff(Value* leftPtr, Value* rightPtr);
    Instruction* CreateCall(Value* function);
    Instruction* CreateRet(Value* value);
    Instruction* CreateJump(BasicBlock* dest);
    Instruction* CreateBranch(Value* cond, BasicBlock* trueDest, BasicBlock* falseDest);
    Instruction* CreateSwitch(Value* cond, BasicBlock* defaultDest);
    Instruction* CreateNop();
    GlobalVariable* GetOrInsertGlobal(const std::string& name, Type* type);
    GlobalVariable* CreateGlobalStringPtr(const std::string& stringValue);
    MetadataStruct* AddMetadataStruct(const soul::ast::Span& span, int32_t id);
    MetadataStruct* CreateMetadataStruct();
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value, bool crop);
    MetadataRef* CreateMetadataRef(const soul::ast::Span& span, int32_t nodeId);
    void AddMetadataStructItem(MetadataStruct* metadataStruct, const std::string& fieldName, MetadataItem* item);
    void SetCompileUnitId(const std::string& compileUnitId_);
    void AddValue(Value* value);
    TypeRepository& GetTypeRepository() { return typeRepository; }
    DataRepository& GetDataRepository() { return dataRepository; }
    Metadata& GetMetadata() { return metadata; }
private:
    TypeRepository typeRepository;
    DataRepository dataRepository;
    Metadata metadata;
    std::vector<std::unique_ptr<Value>> values;
    Function* currentFunction;
    BasicBlock* currentBasicBlock;
};

} // namespace cmajor::masm::ir
