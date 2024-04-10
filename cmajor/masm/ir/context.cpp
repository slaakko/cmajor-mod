// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.ir.context;

import std.core;

namespace cmajor::masm::ir {

Context::Context() : currentFunction(nullptr), currentBasicBlock(nullptr)
{
}

void Context::AddValue(Value* value)
{
    values.push_back(std::unique_ptr<Value>(value));
}

Value* Context::GetBoolValue(bool value)
{
    Value* boolValue = new BoolValue(value);
    AddValue(boolValue);
    return boolValue;
}

Value* Context::GetSByteValue(int8_t value)
{
    Value* sbyteValue = new SByteValue(value);
    AddValue(sbyteValue);
    return sbyteValue;
}

Value* Context::GetByteValue(uint8_t value)
{
    Value* byteValue = new ByteValue(value);
    AddValue(byteValue);
    return byteValue;
}

Value* Context::GetShortValue(int16_t value)
{
    Value* shortValue = new ShortValue(value);
    AddValue(shortValue);
    return shortValue;
}

Value* Context::GetUShortValue(uint16_t value)
{
    Value* ushortValue = new UShortValue(value);
    AddValue(ushortValue);
    return ushortValue;
}

Value* Context::GetIntValue(int32_t value)
{
    Value* intValue = new IntValue(value);
    AddValue(intValue);
    return intValue;
}

Value* Context::GetUIntValue(uint32_t value)
{
    Value* uintValue = new UIntValue(value);
    AddValue(uintValue);
    return uintValue;
}

Value* Context::GetLongValue(int64_t value)
{
    Value* longValue = new LongValue(value);
    AddValue(longValue);
    return longValue;
}

Value* Context::GetULongValue(uint64_t value)
{
    Value* ulongValue = new ULongValue(value);
    AddValue(ulongValue);
    return ulongValue;
}

Value* Context::GetFloatValue(float value)
{
    Value* floatValue = new FloatValue(value);
    AddValue(floatValue);
    return floatValue;
}

Value* Context::GetDoubleValue(double value)
{
    Value* doubleValue = new DoubleValue(value);
    AddValue(doubleValue);
    return doubleValue;
}

ArrayValue* Context::GetArrayValue(Type* arrayType, const std::vector<Value*>& elements, const std::string& prefix)
{
    ArrayValue* arrayValue = new ArrayValue(arrayType, elements, prefix);
    AddValue(arrayValue);
    return arrayValue;
}

StructureValue* Context::GetStructureValue(Type* structureType, const std::vector<Value*>& members)
{
    StructureValue* structureValue = new StructureValue(structureType, members);
    AddValue(structureValue);
    return structureValue;
}

StringValue* Context::GetStringValue(Type* stringType, const std::string& value)
{
    StringValue* stringValue = new StringValue(stringType, value);
    AddValue(stringValue);
    return stringValue;
}

ConversionValue* Context::GetConversionValue(Type* type, Value* from)
{
    ConversionValue* conversionValue = new ConversionValue(type, from);
    AddValue(conversionValue);
    return conversionValue;
}

ClsIdValue* Context::GetClsIdValue(const std::string& typeId)
{
    ClsIdValue* clsIdValue = new ClsIdValue(typeId);
    AddValue(clsIdValue);
    return clsIdValue;
}

Instruction* Context::CreateNot(Value* arg)
{
    Instruction* inst = new NotInstruction(arg);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateNeg(Value* arg)
{
    Instruction* inst = new NegInstruction(arg);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateAdd(Value* left, Value* right)
{
    Instruction* inst = new AddInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSub(Value* left, Value* right)
{
    Instruction* inst = new SubInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateMul(Value* left, Value* right)
{
    Instruction* inst = new MulInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateDiv(Value* left, Value* right)
{
    Instruction* inst = new DivInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateMod(Value* left, Value* right)
{
    Instruction* inst = new ModInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateAnd(Value* left, Value* right)
{
    Instruction* inst = new AndInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateOr(Value* left, Value* right)
{
    Instruction* inst = new OrInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateXor(Value* left, Value* right)
{
    Instruction* inst = new XorInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateShl(Value* left, Value* right)
{
    Instruction* inst = new ShlInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateShr(Value* left, Value* right)
{
    Instruction* inst = new ShrInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateEqual(Value* left, Value* right)
{
    Instruction* inst = new EqualInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLess(Value* left, Value* right)
{
    Instruction* inst = new LessInstruction(left, right);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSignExtend(Value* arg, Type* destType)
{
    Instruction* inst = new SignExtendInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateZeroExtend(Value* arg, Type* destType)
{
    Instruction* inst = new ZeroExtendInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateTruncate(Value* arg, Type* destType)
{
    Instruction* inst = new TruncateInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateBitCast(Value* arg, Type* destType)
{
    Instruction* inst = new BitCastInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateIntToFloat(Value* arg, Type* destType)
{
    Instruction* inst = new IntToFloatInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateFloatToInt(Value* arg, Type* destType)
{
    Instruction* inst = new FloatToIntInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateIntToPtr(Value* arg, Type* destType)
{
    Instruction* inst = new IntToPtrInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrToInt(Value* arg, Type* destType)
{
    Instruction* inst = new PtrToIntInstruction(arg, destType);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateParam(Type* type)
{
    Instruction* inst = new ParamInstruction(type);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLocal(Type* type)
{
    Instruction* inst = new LocalInstruction(type);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLoad(Value* ptr)
{
    Instruction* inst = new LoadInstruction(ptr);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateStore(Value* value, Value* ptr)
{
    Instruction* inst = new StoreInstruction(value, ptr);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateArg(Value* arg)
{
    Instruction* inst = new ArgInstruction(arg);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateElemAddr(Value* ptr, Value* index)
{
    Instruction* inst = new ElemAddrInstruction(ptr, index);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrOffset(Value* ptr, Value* offset)
{
    Instruction* inst = new PtrOffsetInstruction(ptr, offset);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrDiff(Value* leftPtr, Value* rightPtr)
{
    Instruction* inst = new PtrDiffInstruction(leftPtr, rightPtr);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateCall(Value* function)
{
    Instruction* inst = new CallInstruction(function);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateRet(Value* value)
{
    Instruction* inst = new RetInstruction(value);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateJump(BasicBlock* dest)
{
    Instruction* inst = new JumpInstruction(dest);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateBranch(Value* cond, BasicBlock* trueDest, BasicBlock* falseDest)
{
    Instruction* inst = new BranchInstruction(cond, trueDest, falseDest);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSwitch(Value* cond, BasicBlock* defaultDest)
{
    Instruction* inst = new SwitchInstruction(cond, defaultDest);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateNop()
{
    Instruction* inst = new NoOperationInstruction();
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

GlobalVariable* Context::GetOrInsertGlobal(const std::string& name, Type* type)
{
    return dataRepository.GetOrInsertGlobal(name, type);
}

GlobalVariable* Context::CreateGlobalStringPtr(const std::string& stringValue)
{
    return dataRepository.CreateGlobalStringPtr(*this, stringValue);
}

MetadataStruct* Context::CreateMetadataStruct()
{
    return metadata.CreateMetadataStruct();
}

MetadataStruct* Context::AddMetadataStruct(const soul::ast::Span& span, int32_t id)
{
    return metadata.AddMetadataStruct(span, id);
}

MetadataBool* Context::CreateMetadataBool(bool value)
{
    return metadata.CreateMetadataBool(value);
}

MetadataLong* Context::CreateMetadataLong(int64_t value)
{
    return metadata.CreateMetadataLong(value);
}

MetadataString* Context::CreateMetadataString(const std::string& value, bool crop)
{
    return metadata.CreateMetadataString(value, crop);
}

MetadataRef* Context::CreateMetadataRef(const soul::ast::Span& span, int32_t nodeId)
{
    return metadata.CreateMetadataRef(span, nodeId);
}

void Context::AddMetadataStructItem(MetadataStruct* metadataStruct, const std::string& fieldName, MetadataItem* item)
{
    metadataStruct->AddItem(fieldName, item);
}

void Context::SetCompileUnitId(const std::string& compileUnitId_)
{
    dataRepository.SetCompileUnitId(compileUnitId_);
}

} // namespace cmajor::masm::ir
