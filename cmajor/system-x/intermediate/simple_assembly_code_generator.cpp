// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.simple.assembly.code.generator;

import cmajor.systemx.intermediate.opt.codegen;
import cmajor.systemx.intermediate.linear.scan.reg.allocator;
import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.assembler;
import cmajor.symbols;
import util;

namespace cmajor::systemx::intermediate {

SimpleAssemblyCodeGenerator::SimpleAssemblyCodeGenerator(Context* context_, cmajor::systemx::assembler::AssemblyFile* assemblyFile_) :
    Visitor(context_), assemblyFile(assemblyFile_), emitSection(cmajor::systemx::assembler::AssemblySectionKind::code),
    assemblyFunction(nullptr), assemblyStructure(nullptr), assemblyInst(nullptr), currentInst(nullptr), currentFunction(nullptr),
    registerAllocator(nullptr), leader(false), debugInfo(nullptr), lineNumber(0)
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
}

void SimpleAssemblyCodeGenerator::WriteOutputFile()
{
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
        globalVariable.Initializer()->Accept(*this);
    }
}

void SimpleAssemblyCodeGenerator::Emit(cmajor::systemx::assembler::Instruction* assemblyInstruction)
{
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
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitTetra(uint32_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* tetraInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::TETRA);
        Emit(tetraInst);
    }
    assemblyInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(value));
}

void SimpleAssemblyCodeGenerator::EmitWyde(uint16_t value)
{
    if (!assemblyInst)
    {
        cmajor::systemx::assembler::Instruction* wydeInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::WYDE);
        Emit(wydeInst);
    }
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
    assemblyInst->AddOperand(new cmajor::systemx::assembler::ClsIdConstant(soul::ast::SourcePos(), typeId));
}

void SimpleAssemblyCodeGenerator::EmitDebugInfoInst(cmajor::systemx::assembler::Instruction* assemblyInstruction)
{
    if (!debugInfo)
    {
        debugInfo = assemblyFile->GetDebugSection()->CreateDebugInfo();
    }
    debugInfo->AddInstruction(assemblyInstruction);
}

void SimpleAssemblyCodeGenerator::SetCurrentLineNumber(uint32_t lineNumber_)
{
    if (lineNumber == 0)
    {
        lineNumber = lineNumber_;
    }
    else if (lineNumber != lineNumber_)
    {
        EmitLineNumberInfo(lineNumber, *this);
        lineNumber = lineNumber_;
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
    lineNumber = 0;
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
    function.VisitBasicBlocks(*this);
    if (lineNumber != 0)
    {
        EmitLineNumberInfo(lineNumber, *this);
    }
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
    EmitArg(inst, *this);
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

void SimpleAssemblyCodeGenerator::Visit(PhiInstruction& inst)
{
    Error("simple assembly code generator does not support phi instructions");
}

void SimpleAssemblyCodeGenerator::Visit(NoOperationInstruction& inst)
{
    EmitNop(inst, *this);
}

void SimpleAssemblyCodeGenerator::Visit(BoolValue& value)
{
    EmitBool(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(SByteValue& value)
{
    EmitSByte(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ByteValue& value)
{
    cmajor::systemx::intermediate::EmitByte(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ShortValue& value)
{
    EmitShort(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(UShortValue& value)
{
    EmitUShort(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(IntValue& value)
{
    EmitInt(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(UIntValue& value)
{
    EmitUInt(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(LongValue& value)
{
    EmitLong(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ULongValue& value)
{
    EmitULong(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(FloatValue& value)
{
    EmitFloat(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(DoubleValue& value)
{
    EmitDouble(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(NullValue& value)
{
    EmitNull(*this);
}

void SimpleAssemblyCodeGenerator::Visit(AddressValue& value)
{
    EmitAddress(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(ArrayValue& value)
{
    if (value.GetType()->IsArrayType())
    {
        if (!assemblyInst)
        {
            ArrayType* arrayType = static_cast<ArrayType*>(value.GetType());
            Type* elementType = arrayType->ElementType();
            assemblyInst = elementType->MakeAssemblyInst(GetContext());
            Emit(assemblyInst);
        }
        for (ConstantValue* elementValue : value.Elements())
        {
            elementValue->Accept(*this);
        }
    }
    else
    {
        Error("error generating array value: array type expected");
    }
}

void SimpleAssemblyCodeGenerator::Visit(StructureValue& value)
{
    for (ConstantValue* fieldValue : value.FieldValues())
    {
        assemblyInst = nullptr;
        fieldValue->Accept(*this);
    }
}

void SimpleAssemblyCodeGenerator::Visit(StringValue& value)
{
    EmitString(value, *this);
}

void SimpleAssemblyCodeGenerator::Visit(StringArrayValue& value)
{
    for (ConstantValue* elementValue : value.Strings())
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