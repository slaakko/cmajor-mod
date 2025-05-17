// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.simple.assembly.code.generator;

import cmajor.systemx.intermediate.opt.codegen;
import cmajor.systemx.intermediate.linear.scan.reg.allocator;
import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.compile.unit;
import cmajor.systemx.intermediate.code;
import cmajor.systemx.assembler;
import cmajor.systemx.machine;
import cmajor.symbols;
import util;

namespace cmajor::systemx::intermediate {

SimpleAssemblyCodeGenerator::SimpleAssemblyCodeGenerator(Context* context_, cmajor::systemx::assembler::AssemblyFile* assemblyFile_) :
    Visitor(context_), assemblyFile(assemblyFile_), emitSection(cmajor::systemx::assembler::AssemblySectionKind::code),
    assemblyFunction(nullptr), assemblyStructure(nullptr), assemblyInst(nullptr), currentInst(nullptr), currentFunction(nullptr),
    registerAllocator(nullptr), leader(false), debugInfo(nullptr), currentLineColLen(), currentOffset(0)
{
}

void SimpleAssemblyCodeGenerator::AddSourceFileInfo(CompileUnit& compileUnit)
{
    if (compileUnit.GetMetadataRef())
    {
        sourceFileNameMap[compileUnit.FilePath()] = compileUnit.GetMetadataRef()->NodeId();
    }
}

void SimpleAssemblyCodeGenerator::GenerateDebugInfo()
{
    Context* ctx = Ctx();
    for (Function* debugInfoFunc : debugInfoFunctions)
    {
        MetadataRef* metadataRef = debugInfoFunc->GetMetadataRef();
        if (metadataRef)
        {
            MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
            if (metadataStruct)
            {
                MetadataItem* sourceFileItem = metadataStruct->GetItem("sourceFile");
                if (sourceFileItem && sourceFileItem->Kind() == MetadataItemKind::metadataRef)
                {
                    MetadataRef* mdRef = static_cast<MetadataRef*>(sourceFileItem);
                    MetadataStruct* mdStruct = mdRef->GetMetadataStruct();
                    if (mdStruct)
                    {
                        MetadataItem* sourceFileNameItem = mdStruct->GetItem("sourceFileName");
                        if (sourceFileNameItem && sourceFileNameItem->Kind() == MetadataItemKind::metadataString)
                        {
                            MetadataString* mdString = static_cast<MetadataString*>(sourceFileNameItem);
                            sourceFileNameMap[mdString->Value()] = mdStruct->Id();
                        }
                    }
                }
            }
        }
    }
    for (const std::pair<std::string, int64_t >& r : sourceFileNameMap)
    {
        EmitSourceFileNameDebugInfo(r.first, r.second, *this);
    }
    for (Function* debugInfoFunc : debugInfoFunctions)
    {
        int64_t frameSize = 0;
        auto it = frameSizeMap.find(debugInfoFunc);
        if (it != frameSizeMap.cend())
        {
            frameSize = it->second;
        }
        EmitFunctionDebugInfo(debugInfoFunc, frameSize, *this);
    }
    for (const auto& declaredType : ctx->GetTypes().DeclaredTypes())
    {
        EmitTypeDebugInfo(declaredType, *this);
    }
}

void SimpleAssemblyCodeGenerator::GenerateCode()
{
    Ctx()->GetData().VisitGlobalVariables(*this);
    Ctx()->GetCode().VisitFunctions(*this);
}

void SimpleAssemblyCodeGenerator::WriteOutputFile()
{
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyFile->CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyFile->Write();
}

void SimpleAssemblyCodeGenerator::Visit(GlobalVariable& globalVariable)
{
    if (globalVariable.Initializer())
    {
        if (globalVariable.Once())
        {
            assemblyFile->GetLinkSection()->GetOrCreateLinkOnceObject()->AddLinkOnceSymbol(cmajor::systemx::assembler::MakeGlobalSymbol(globalVariable.Name()));
        }
        else
        {
            assemblyFile->GetLinkSection()->GetOrCreateExternObject()->AddExternSymbol(cmajor::systemx::assembler::MakeGlobalSymbol(globalVariable.Name()));
        }
        emitSection = cmajor::systemx::assembler::AssemblySectionKind::data;
        cmajor::systemx::assembler::AssemblySection* dataSection = assemblyFile->GetDataSection();
        assemblyStructure = dataSection->CreateStructure(globalVariable.Name());
        cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
        symbolName = globalVariable.Name() + "_data";
        octaInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(symbolName));
        leader = false;
        Emit(octaInst);
        leader = true;
        assemblyInst = nullptr;
        currentOffset = 0;
        globalVariable.Initializer()->Accept(*this);
        ByteType* byteType = Ctx()->GetTypes().GetByteType();
        ByteValue zero(0u, byteType);
        while ((currentOffset & 7) != 0)
        {
            zero.Accept(*this);
        }
    }
}

void SimpleAssemblyCodeGenerator::Emit(cmajor::systemx::assembler::Instruction* assemblyInstruction)
{
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInstruction->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    if (emitSection == cmajor::systemx::assembler::AssemblySectionKind::code)
    {
        if (leader)
        {
            leader = false;
            assemblyInstruction->SetLabel(cmajor::systemx::assembler::MakeLocalSymbol(currentInst->Parent()->Id()));
        }
        assemblyFunction->AddInstruction(assemblyInstruction);
    }
    else if (emitSection == cmajor::systemx::assembler::AssemblySectionKind::data)
    {
        if (leader)
        {
            leader = false;
            assemblyInstruction->SetLabel(cmajor::systemx::assembler::MakeGlobalSymbol(symbolName));
        }
        assemblyStructure->AddInstruction(assemblyInstruction);
    }
    assemblyInst = assemblyInstruction;
}

void SimpleAssemblyCodeGenerator::EmitOcta(uint64_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
        Emit(octaInst);
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInst->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitTetra(uint32_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* tetraInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::TETRA);
        Emit(tetraInst);
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInst->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitWyde(uint16_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* wydeInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::WYDE);
        Emit(wydeInst);
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInst->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitByte(uint8_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* byteInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BYTE);
        Emit(byteInst);
    }
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitSymbol(const std::string& name)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
        Emit(octaInst);
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInst->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(name));
}

int SimpleAssemblyCodeGenerator::ExitLabelId() const
{
    return currentInst->Parent()->Parent()->LastBasicBlock()->Id() + 1;
}

void SimpleAssemblyCodeGenerator::EmitClsId(const std::string& typeId)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
        Emit(octaInst);
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInst->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    assemblyInst->AddOperand(new cmajor::systemx::assembler::ClsIdConstant(soul::ast::SourcePos(), typeId));
}

void SimpleAssemblyCodeGenerator::EmitDebugInfoInst(cmajor::systemx::assembler::Instruction* assemblyInstruction)
{
    if (!debugInfo)
    {
        debugInfo = assemblyFile->GetDebugSection()->CreateDebugInfo();
    }
#ifdef THREAD_ID_CHECK
    if (std::this_thread::get_id() != assemblyInstruction->CreatorThreadId() || std::this_thread::get_id() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    debugInfo->AddInstruction(assemblyInstruction);
}

void SimpleAssemblyCodeGenerator::SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen, int32_t index)
{
    if (currentFunction)
    {
        if (currentFunction->IndexSeen(index))
        {
            return;
        }
        currentFunction->AddIndex(index);
    }
    if (!currentLineColLen.IsValid())
    {
        currentLineColLen = lineColLen;
        EmitLineNumberInfo(currentLineColLen, index, *this);
    }
    else if (currentLineColLen != lineColLen)
    {
        currentLineColLen = lineColLen;
        EmitLineNumberInfo(currentLineColLen, index, *this);
    }
}

void SimpleAssemblyCodeGenerator::BeginTry(uint32_t tryBlockId, uint32_t parentTryBlockId)
{
    EmitBeginTry(tryBlockId, parentTryBlockId, *this);
}

void SimpleAssemblyCodeGenerator::EndTry(uint32_t tryBlockId)
{
    EmitEndTry(tryBlockId, *this);
}

void SimpleAssemblyCodeGenerator::Catch(uint32_t catchBlockId, uint32_t tryBlockId, const std::string& caughtTypeIdStr)
{
    util::uuid caughtTypeId = util::ParseUuid(caughtTypeIdStr);
    uint64_t caughtTypeId1;
    uint64_t caughtTypeId2;
    util::UuidToInts(caughtTypeId, caughtTypeId1, caughtTypeId2);
    EmitCatch(catchBlockId, tryBlockId, caughtTypeId1, caughtTypeId2, *this);
}

void SimpleAssemblyCodeGenerator::BeginCleanup(uint32_t cleanupBlockId, uint32_t tryBlockId)
{
    EmitBeginCleanup(cleanupBlockId, tryBlockId, *this);
}

void SimpleAssemblyCodeGenerator::EndCleanup(uint32_t cleanupBlockId)
{
    EmitEndCleanup(cleanupBlockId, *this);
}

void SimpleAssemblyCodeGenerator::Error(const std::string& message)
{
    cmajor::systemx::intermediate::Error(message, currentInst->GetSourcePos(), GetContext());
}

const soul::ast::SourcePos& SimpleAssemblyCodeGenerator::GetSourcePos() const
{
    return currentInst->GetSourcePos();
}

void SimpleAssemblyCodeGenerator::Visit(Function& function)
{
    if (!function.IsDefined()) return;
    currentFunction = &function;
    currentLineColLen = soul::ast::LineColLen();
    if (function.GetFlag(FunctionFlags::once))
    {
        assemblyFile->GetLinkSection()->GetOrCreateLinkOnceObject()->AddLinkOnceSymbol(cmajor::systemx::assembler::MakeGlobalSymbol(function.Name()));
    }
    else
    {
        assemblyFile->GetLinkSection()->GetOrCreateExternObject()->AddExternSymbol(cmajor::systemx::assembler::MakeGlobalSymbol(function.Name()));
    }
    emitSection = cmajor::systemx::assembler::AssemblySectionKind::code;
    Ctx()->ResetRegisterPool();
    std::unique_ptr<RegisterAllocator> linearScanRregisterAllocator = LinearScanRegisterAllocation(function, Ctx());
    registerAllocator = linearScanRregisterAllocator.get();
    assemblyFunction = assemblyFile->CreateFunction(function.Name());
    MetadataRef* metadataRef = function.GetMetadataRef();
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = Ctx()->GetMetadata().GetMetadataStruct(metadataRef->NodeId());
        if (metadataStruct)
        {
            MetadataItem* metadataItem = metadataStruct->GetItem("fullName");
            if (metadataItem && metadataItem->IsMetadataString())
            {
                MetadataString* fullNameItem = static_cast<MetadataString*>(metadataItem);
                assemblyFunction->SetComment(fullNameItem->Value());
            }
        }
    }
    function.VisitBasicBlocks(*this);
    assemblyFunction->SetActiveFunctionPart(cmajor::systemx::assembler::FunctionPart::prologue);
    EmitPrologue(*this);
    assemblyFunction->SetActiveFunctionPart(cmajor::systemx::assembler::FunctionPart::epilogue);
    EmitEpilogue(*this);
    if (function.GetMetadataRef())
    {
        debugInfoFunctions.push_back(&function);
        frameSizeMap[&function] = registerAllocator->GetFrame().Size();
    }
}

void SimpleAssemblyCodeGenerator::Visit(BasicBlock& basicBlock)
{
    Instruction* inst = basicBlock.FirstInstruction();
    leader = true;
    while (inst)
    {
        currentInst = inst;
        ProcessInstructionMetadata(inst, *this);
        RegisterAllocationAction action = registerAllocator->Run(inst);
        if (action == RegisterAllocationAction::spill)
        {
            for (const SpillData& spillData : registerAllocator->GetSpillData())
            {
                EmitStore(spillData.spillToFrameLocation, spillData.registerToSpill, *this);
            }
        }
        inst->Accept(*this);
        inst = inst->Next();
    }
}

void SimpleAssemblyCodeGenerator::Visit(StoreInstruction& inst)
{
    EmitStore(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ArgInstruction& inst)
{
    EmitArg(inst, *this, new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::STOU), true);
}

void SimpleAssemblyCodeGenerator::Visit(JmpInstruction& inst)
{
    EmitJmp(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(BranchInstruction& inst)
{
    EmitBranch(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ProcedureCallInstruction& inst)
{
    EmitProcedureCall(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(RetInstruction& inst)
{
    EmitRet(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(SwitchInstruction& inst)
{
    EmitSwitch(inst, *this);
/*
    if (cmajor::symbols::GetOptimizationLevel() > 0)
    {
        EmitOptSwitch(inst, *this);
    }
    else
    {
        EmitSwitch(inst, *this);
    }
*/
}

void SimpleAssemblyCodeGenerator::Visit(NotInstruction& inst)
{
    EmitNot(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(NegInstruction& inst)
{
    EmitNeg(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(SignExtendInstruction& inst)
{
    EmitSignExtension(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ZeroExtendInstruction& inst)
{
    EmitZeroExtension(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(TruncateInstruction& inst)
{
    EmitTruncate(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(BitcastInstruction& inst)
{
    EmitBitcast(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(IntToFloatInstruction& inst)
{
    EmitIntToFloat(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(FloatToIntInstruction& inst)
{
    EmitFloatToInt(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(IntToPtrInstruction& inst)
{
    EmitIntToPtr(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(PtrToIntInstruction& inst)
{
    EmitPtrToInt(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(AddInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(SubInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(MulInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(DivInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ModInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(AndInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(OrInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(XorInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ShlInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ShrInstruction& inst)
{
    EmitBinOpInst(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(EqualInstruction& inst)
{
    EmitEqual(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(LessInstruction& inst)
{
    EmitLess(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ParamInstruction& inst)
{
    EmitParam(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(LoadInstruction& inst)
{
    EmitLoad(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ElemAddrInstruction& inst)
{
    EmitElemAddr(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(PtrOffsetInstruction& inst)
{
    EmitPtrOffset(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(PtrDiffInstruction& inst)
{
    EmitPtrDiff(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(FunctionCallInstruction& inst)
{
    EmitFunctionCall(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(TrapInstruction& inst)
{
    EmitTrap(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(NoOperationInstruction& inst)
{
    EmitNop(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(BoolValue& value)
{
    EmitBool(value, *this);
    currentOffset += 1;
}

void SimpleAssemblyCodeGenerator::Visit(SByteValue& value)
{
    EmitSByte(value, *this);
    currentOffset += 1;
}

void SimpleAssemblyCodeGenerator::Visit(ByteValue& value)
{
    cmajor::systemx::intermediate::EmitByte(value, *this);
    currentOffset += 1;
}

void SimpleAssemblyCodeGenerator::Visit(ShortValue& value)
{
    EmitShort(value, *this);
    currentOffset += 2;
}

void SimpleAssemblyCodeGenerator::Visit(UShortValue& value)
{
    EmitUShort(value, *this);
    currentOffset += 2;
}

void SimpleAssemblyCodeGenerator::Visit(IntValue& value)
{
    EmitInt(value, *this);
    currentOffset += 4;
}

void SimpleAssemblyCodeGenerator::Visit(UIntValue& value)
{
    EmitUInt(value, *this);
    currentOffset += 4;
}

void SimpleAssemblyCodeGenerator::Visit(LongValue& value)
{
    EmitLong(value, *this);
    currentOffset += 8;
}

void SimpleAssemblyCodeGenerator::Visit(ULongValue& value)
{
    EmitULong(value, *this);
    currentOffset += 8;
}

void SimpleAssemblyCodeGenerator::Visit(FloatValue& value)
{
    EmitFloat(value, *this);
    currentOffset += 4;
}

void SimpleAssemblyCodeGenerator::Visit(DoubleValue& value)
{
    EmitDouble(value, *this);
    currentOffset += 8;
}

void SimpleAssemblyCodeGenerator::Visit(CharValue& value)
{
    EmitChar(value, *this);
    currentOffset += 1;
}

void SimpleAssemblyCodeGenerator::Visit(WCharValue& value)
{
    EmitWChar(value, *this);
    currentOffset += 2;
}

void SimpleAssemblyCodeGenerator::Visit(UCharValue& value)
{
    EmitUChar(value, *this);
    currentOffset += 4;
}

void SimpleAssemblyCodeGenerator::Visit(NullValue& value)
{
    EmitNull(*this);
    currentOffset += 8;
}

void SimpleAssemblyCodeGenerator::Visit(AddressValue& value)
{
    EmitAddress(value, *this);
    currentOffset += 8;
}

void SimpleAssemblyCodeGenerator::Visit(ArrayValue& value)
{
    if (value.GetType()->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(value.GetType());
        Type* elementType = arrayType->ElementType();
        ConstantValue* zero = Ctx()->GetByteValue(0u);
        for (ConstantValue* elementValue : value.Elements())
        {
            assemblyInst = nullptr;
            elementValue->Accept(*this);
            if (currentOffset > 0)
            {
                int64_t alignment = elementType->Alignment();
                int64_t elementOffset = alignment * ((currentOffset - 1) / alignment + 1);
                while (currentOffset < elementOffset)
                {
                    assemblyInst = nullptr;
                    zero->Accept(*this);
                }
            }
        }
    }
    else
    {
        Error("error generating array value: array type expected");
    }
}

void SimpleAssemblyCodeGenerator::Visit(StructureValue& value)
{
    StructureType* structureType = nullptr;
    Type* type = value.GetType();
    if (type->IsStructureType())
    {
        structureType = static_cast<StructureType*>(type);
    }
    else
    {
        Error("structure type expected");
    }
    int64_t start = currentOffset;
    ConstantValue* zero = Ctx()->GetByteValue(0u);
    int64_t n = value.FieldValues().size();
    for (int64_t i = 0; i < n; ++i)
    {
        int64_t fieldOffset = structureType->GetFieldOffset(i);
        while (currentOffset - start < fieldOffset)
        {
            assemblyInst = nullptr;
            zero->Accept(*this);
        }
        ConstantValue* fieldValue = value.FieldValues()[i];
        assemblyInst = nullptr;
        fieldValue->Accept(*this);
    }
    while (currentOffset - start < structureType->Size())
    {
        assemblyInst = nullptr;
        zero->Accept(*this);
    }
}

void SimpleAssemblyCodeGenerator::Visit(StringValue& value)
{
    EmitString(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(StringArrayValue& value)
{
    for (ConstantValue* elementValue : value.Elements())
    {
        assemblyInst = nullptr;
        elementValue->Accept(*this);
    }
}

void SimpleAssemblyCodeGenerator::Visit(ConversionValue& value)
{
    if (!assemblyInst)
    {
        assemblyInst = value.GetType()->MakeAssemblyInst(GetContext());
        Emit(assemblyInst);
    }
    value.From()->Accept(*this);
}

void SimpleAssemblyCodeGenerator::Visit(ClsIdValue& value)
{
    cmajor::systemx::intermediate::EmitClsId(value.TypeId(), *this);
}

void SimpleAssemblyCodeGenerator::Visit(SymbolValue& value)
{
    cmajor::systemx::intermediate::EmitSymbol(value, *this);
}

} // cmajor::systemx::intermediate
