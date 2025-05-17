// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.context;

import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.code.lexer;
import util;

namespace cmajor::systemx::intermediate {

Context::Context() : compileUnit(), currentBasicBlock(nullptr), currentLineColLen(), lineColLenInfoMap()
{
    compileUnit.SetContext(this);
    types.SetContext(this);
    data.SetContext(this);
    code.SetContext(this);
    metadata.SetContext(this);
}

void Context::SetFilePath(const std::string& filePath_)
{
    compileUnit.SetFilePath(filePath_);
}

const std::string& Context::FilePath() const
{
    return compileUnit.FilePath();
}

void Context::Write(const std::string& intermediateFilePath)
{
    compileUnit.SetFilePath(intermediateFilePath);
    compileUnit.Write();
}

std::string Context::ErrorLines(const soul::ast::SourcePos& sourcePos)
{
    if (sourcePos.IsValid())
    {
        return util::ToUtf8(fileMap.GetFileLine(sourcePos.file, sourcePos.line));
    }
    else
    {
        return std::string();
    }
}

void Context::SetCompileUnitInfo(const std::string& compileUnitId_, MetadataRef* metadataRef)
{
    compileUnit.SetInfo(compileUnitId_, metadataRef);
}

void Context::AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* metadataRef)
{
    types.AddStructureType(sourcePos, typeId, fieldTypeRefs, metadataRef);
}

void Context::AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef, MetadataRef* metadataRef)
{
    types.AddArrayType(sourcePos, typeId, size, elementTypeRef, metadataRef);
}

void Context::AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs, 
    MetadataRef* metadataRef)
{
    types.AddFunctionType(sourcePos, typeId, returnTypeRef, paramTypeRefs, metadataRef);
}

void Context::AddGlobalVariable(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& variableName, ConstantValue* initializer, bool once)
{
    data.AddGlobalVariable(sourcePos, type, variableName, initializer, once);
}

GlobalVariable* Context::GetOrInsertGlobal(const std::string& name, Type* type)
{
    return data.GetOrInsertGlobal(name, type);
}

void Context::ResolveTypes()
{
    types.Resolve(this);
}

void Context::ResolveType(TypeRef& typeRef)
{
    types.ResolveType(typeRef, this);
}

ConstantValue* Context::GetTrueValue()
{
    return data.GetTrueValue(types);
}

ConstantValue* Context::GetFalseValue()
{
    return data.GetFalseValue(types);
}

ConstantValue* Context::GetBooleanLiteral(const soul::ast::SourcePos& sourcePos, Type* type, bool value)
{
    if (type->IsBooleanType())
    {
        if (value)
        {
            return GetTrueValue();
        }
        else
        {
            return GetFalseValue();
        }
    }
    else
    {
        Error("error making literal value: Boolean type expected", sourcePos, this);
    }
    return nullptr;
}

ConstantValue* Context::GetSByteValue(int8_t value)
{
    return data.GetSByteValue(value, types);
}

ConstantValue* Context::GetByteValue(uint8_t value)
{
    return data.GetByteValue(value, types);
}

ConstantValue* Context::GetShortValue(int16_t value)
{
    return data.GetShortValue(value, types);
}

ConstantValue* Context::GetUShortValue(uint16_t value)
{
    return data.GetUShortValue(value, types);
}

ConstantValue* Context::GetIntValue(int32_t value)
{
    return data.GetIntValue(value, types);
}

ConstantValue* Context::GetUIntValue(uint32_t value)
{
    return data.GetUIntValue(value, types);
}

ConstantValue* Context::GetLongValue(int64_t value)
{
    return data.GetLongValue(value, types);
}

ConstantValue* Context::GetULongValue(uint64_t value)
{
    return data.GetULongValue(value, types);
}

ConstantValue* Context::GetFloatValue(float value)
{
    return data.GetFloatValue(value, types);
}

ConstantValue* Context::GetDoubleValue(double value)
{
    return data.GetDoubleValue(value, types);
}

ConstantValue* Context::GetCharValue(char value)
{
    return data.GetCharValue(value, types);
}

ConstantValue* Context::GetWCharValue(char16_t value)
{
    return data.GetWCharValue(value, types);
}

ConstantValue* Context::GetUCharValue(char32_t value)
{
    return data.GetUCharValue(value, types);
}

ConstantValue* Context::GetNullValue(const soul::ast::SourcePos& sourcePos, Type* type)
{
    if (type->IsPointerType())
    {
        return data.GetNullValue(type);
    }
    else
    {
        Error("error making null literal value: pointer type expected", sourcePos, this);
    }
    return nullptr;
}

ConstantValue* Context::MakeArrayValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& elements, const std::string& prefix)
{
    return data.MakeArrayValue(sourcePos, elements, prefix);
}

ConstantValue* Context::MakeStructureValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& fieldValues)
{
    return data.MakeStructureValue(sourcePos, fieldValues);
}

ConstantValue* Context::MakeStringValue(const soul::ast::SourcePos& sourcePos, const std::string& value, bool crop)
{
    return data.MakeStringValue(sourcePos, value, crop);
}

ConstantValue* Context::MakeStringArrayValue(const soul::ast::SourcePos& sourcePos, char prefix, const std::vector<ConstantValue*>& strings)
{
    return data.MakeStringArrayValue(sourcePos, prefix, strings);
}

ConstantValue* Context::MakeConversionValue(const soul::ast::SourcePos& sourcePos, Type* type, ConstantValue* from)
{
    return data.MakeConversionValue(sourcePos, type, from);
}

ConstantValue* Context::MakeClsIdValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& clsIdStr, bool crop)
{
    return data.MakeClsIdValue(sourcePos, type, clsIdStr, crop);
}

ConstantValue* Context::MakeSymbolValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& symbol)
{
    return data.MakeSymbolValue(sourcePos, type, symbol);
}

ConstantValue* Context::MakeNumericLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& strValue)
{
    return data.MakeNumericLiteral(sourcePos, type, strValue, types, this);
}

ConstantValue* Context::MakeAddressLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& id, bool resolve)
{
    return data.MakeAddressLiteral(sourcePos, type, id, this, resolve);
}

Function* Context::CurrentFunction() const
{
    return code.CurrentFunction();
}

void Context::SetCurrentFunction(Function* function)
{
    code.SetCurrentFunction(function);
}

Function* Context::AddFunctionDefinition(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId, bool once, bool main, bool inline_, 
    MetadataRef* metadataRef)
{
    Type* t = type;
    if (t->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        t = pointerType->BaseType();
    }
    if (t->IsFunctionType())
    {
        return code.AddFunctionDefinition(sourcePos, type, functionId, once, main, inline_, metadataRef, this);
    }
    else
    {
        Error("error adding function '" + functionId + "' definition: invalid type '" + type->Name() + "': function type or function pointer type expected", sourcePos, this);
    }
    return nullptr;
}

Function* Context::AddFunctionDeclaration(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId)
{
    Type* t = type;
    if (t->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        t = pointerType->BaseType();
    }
    if (t->IsFunctionType())
    {
        return code.AddFunctionDeclaration(sourcePos, type, functionId);
    }
    else
    {
        Error("error adding function '" + functionId + "' declaration: invalid type '" + type->Name() + "': function type or function pointer type expected", sourcePos, this);
    }
    return nullptr;
}

Function* Context::GetOrInsertFunction(const std::string& name, Type* type)
{
    return code.GetOrInsertFunction(name, type);
}

Instruction* Context::CreateNot(Value* arg)
{
    Instruction* inst = new NotInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(arg->GetType()), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateNeg(Value* arg)
{
    Instruction* inst = new NegInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(arg->GetType()), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateAdd(Value* left, Value* right)
{
    Instruction* inst = new AddInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSub(Value* left, Value* right)
{
    Instruction* inst = new SubInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateMul(Value* left, Value* right)
{
    Instruction* inst = new MulInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateDiv(Value* left, Value* right)
{
    Instruction* inst = new DivInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateMod(Value* left, Value* right)
{
    Instruction* inst = new ModInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateAnd(Value* left, Value* right)
{
    Instruction* inst = new AndInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateOr(Value* left, Value* right)
{
    Instruction* inst = new OrInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateXor(Value* left, Value* right)
{
    Instruction* inst = new XorInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateShl(Value* left, Value* right)
{
    Instruction* inst = new ShlInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateShr(Value* left, Value* right)
{
    Instruction* inst = new ShrInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateEqual(Value* left, Value* right)
{
    Instruction* inst = new EqualInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(types.GetBoolType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLess(Value* left, Value* right)
{
    Instruction* inst = new LessInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(types.GetBoolType()), left, right);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSignExtend(Value* arg, Type* destType)
{
    Instruction* inst = new SignExtendInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateZeroExtend(Value* arg, Type* destType)
{
    Instruction* inst = new ZeroExtendInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateTruncate(Value* arg, Type* destType)
{
    Instruction* inst = new TruncateInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateBitCast(Value* arg, Type* destType)
{
    Instruction* inst = new BitcastInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateIntToFloat(Value* arg, Type* destType)
{
    Instruction* inst = new IntToFloatInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateFloatToInt(Value* arg, Type* destType)
{
    Instruction* inst = new FloatToIntInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateIntToPtr(Value* arg, Type* destType)
{
    Instruction* inst = new IntToPtrInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrToInt(Value* arg, Type* destType)
{
    Instruction* inst = new PtrToIntInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(destType), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateParam(Type* type)
{
    Instruction* inst = new ParamInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(type));
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLocal(Type* type)
{
    Type* ptrType = type->AddPointer(this);
    Instruction* inst = new LocalInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(ptrType), type);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateLoad(Value* ptr)
{
    Type* type = ptr->GetType()->RemovePointer(soul::ast::SourcePos(), this);
    Instruction* inst = new LoadInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(type), ptr);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateStore(Value* value, Value* ptr)
{
    Instruction* inst = new StoreInstruction(soul::ast::SourcePos(), value, ptr);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateArg(Value* arg)
{
    Instruction* inst = new ArgInstruction(soul::ast::SourcePos(), arg);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateElemAddr(Value* ptr, Value* index)
{
    Type* type = ptr->GetType()->RemovePointer(ptr->GetSourcePos(), this);
    if (type->IsStructureType())
    {
        StructureType* structureType = static_cast<StructureType*>(type);
        int64_t idx = 0;
        if (index->IsLongValue())
        {
            LongValue* idxValue = static_cast<LongValue*>(index);
            idx = idxValue->GetValue();
        }
        else
        {
            Error("long value expected", soul::ast::SourcePos(), this);
        }
        Type* fieldType = structureType->FieldType(idx);
        type = fieldType->AddPointer(this);
    }
    else if (type->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(type);
        Type* elementType = arrayType->ElementType();
        type = elementType->AddPointer(this);
    }
    Instruction* inst = new ElemAddrInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(type), ptr, index);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrOffset(Value* ptr, Value* offset)
{
    Instruction* inst = new PtrOffsetInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(ptr->GetType()), ptr, offset);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreatePtrDiff(Value* leftPtr, Value* rightPtr)
{
    Instruction* inst = new PtrDiffInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(types.GetLongType()), leftPtr, rightPtr);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateCall(Value* callee)
{
    Instruction* inst = nullptr;
    Type* type = callee->GetType();
    if (type->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        type = pointerType->BaseType();
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        if (functionType->ReturnType()->IsVoidType())
        {
            inst = new ProcedureCallInstruction(soul::ast::SourcePos(), callee);
        }
        else
        {
            inst = new FunctionCallInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(functionType->ReturnType()), callee);
        }
    }
    else
    {
        Error("create call: function type or function pointer type expected", callee->GetSourcePos(), this);
    }
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateRet(Value* value)
{
    Instruction* inst = new RetInstruction(soul::ast::SourcePos(), value);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateJump(BasicBlock* dest)
{
    JmpInstruction* inst = new JmpInstruction(soul::ast::SourcePos(), dest->Id());
    inst->SetTargetBasicBlock(dest);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateBranch(Value* cond, BasicBlock* trueDest, BasicBlock* falseDest)
{
    BranchInstruction* inst = new BranchInstruction(soul::ast::SourcePos(), cond, trueDest->Id(), falseDest->Id());
    inst->SetTrueTargetBasicBlock(trueDest);
    inst->SetFalseTargetBasicBlock(falseDest);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateSwitch(Value* cond, BasicBlock* defaultDest)
{
    SwitchInstruction* inst = new SwitchInstruction(soul::ast::SourcePos(), cond, defaultDest->Id());
    inst->SetDefaultTargetBlock(defaultDest);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateTrap(const std::vector<Value*>& args)
{
    Value* b0 = nullptr;
    Value* b1 = nullptr;
    Value* b2 = nullptr;
    int n = args.size();
    for (int i = 0; i < n; ++i)
    {
        Value* arg = args[i];
        if (i == 0)
        {
            b0 = arg;
        }
        else if (i == 1)
        {
            b1 = arg;
        }
        else if (i == 2)
        {
            b2 = arg;
        }
        else
        {
            CreateArg(arg);
        }
    }
    Instruction* inst = new TrapInstruction(soul::ast::SourcePos(), code.CurrentFunction()->MakeRegValue(types.GetLongType()), b0, b1, b2);
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

Instruction* Context::CreateNop()
{
    NoOperationInstruction* inst = new NoOperationInstruction(soul::ast::SourcePos());
    AddLineInfo(inst);
    currentBasicBlock->AddInstruction(inst);
    return inst;
}

void Context::SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen)
{
    if (lineColLen.IsValid())
    {
        currentLineColLen = lineColLen;
        code.CurrentFunction()->MapLineColLen(lineColLen);
    }
}

int32_t Context::GetLineColLenIndex(const soul::ast::LineColLen& lineColLen) const
{
    return code.CurrentFunction()->GetLineColLenIndex(lineColLen);
}

void Context::AddLineInfo(Instruction* inst)
{
    if (currentLineColLen.IsValid())
    {
        MetadataRef* lineNumberInfo = nullptr;
        auto it = lineColLenInfoMap.find(currentLineColLen);
        if (it != lineColLenInfoMap.cend())
        {
            lineNumberInfo = it->second;
        }
        else
        {
            MetadataStruct* lineNumberStruct = metadata.CreateMetadataStruct();
            lineNumberStruct->AddItem("nodeType", metadata.CreateMetadataLong(lineInfoNodeType));
            lineNumberStruct->AddItem("index", metadata.CreateMetadataLong(GetLineColLenIndex(currentLineColLen)));
            lineNumberStruct->AddItem("line", metadata.CreateMetadataLong(currentLineColLen.line));
            lineNumberStruct->AddItem("col", metadata.CreateMetadataLong(currentLineColLen.col));
            lineNumberStruct->AddItem("len", metadata.CreateMetadataLong(currentLineColLen.len));
            lineNumberInfo = metadata.CreateMetadataRef(soul::ast::SourcePos(), lineNumberStruct->Id());
            lineColLenInfoMap[currentLineColLen] = lineNumberInfo;
        }
        inst->SetMetadataRef(lineNumberInfo);
    }
}

MetadataStruct* Context::AddMetadataStruct(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context)
{
    return metadata.AddMetadataStruct(sourcePos, id, context);
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

MetadataArray* Context::CreateMetadataArray()
{
    return metadata.CreateMetadataArray();
}

MetadataRef* Context::CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId)
{
    return metadata.CreateMetadataRef(sourcePos, nodeId);
}

void Context::ResolveMetadataReferences()
{
    metadata.ResolveMetadataReferences(this);
}

MetadataRef* Context::GetMetadataRefForSourceFile(const std::string& sourceFileName)
{
    auto it = sourceFileMap.find(sourceFileName);
    if (it != sourceFileMap.cend())
    {
        return it->second;
    }
    else
    {
        MetadataStruct* sourceFileStruct = metadata.CreateMetadataStruct();
        sourceFileStruct->AddItem("nodeType", metadata.CreateMetadataLong(fileInfoNodeType));
        sourceFileStruct->AddItem("sourceFileName", metadata.CreateMetadataString(sourceFileName, false));
        MetadataRef* sourceFileNameRef = metadata.CreateMetadataRef(soul::ast::SourcePos(), sourceFileStruct->Id());
        sourceFileMap[sourceFileName] = sourceFileNameRef;
        return sourceFileNameRef;
    }
}

void Context::ResetRegisterPool()
{
    registerPool = RegisterPool();
}

} // cmajor::systemx::intermediate
