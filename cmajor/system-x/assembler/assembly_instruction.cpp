// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.assembly.instruction;

import cmajor.systemx.assembler.assembler;
import cmajor.systemx.object;
import cmajor.systemx.machine;

namespace cmajor::systemx::assembler {

AssemblyInstruction::AssemblyInstruction()
{
}

AssemblyInstruction::~AssemblyInstruction()
{
}

class ExternInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void ExternInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    for (auto& value : currentInst->Operands())
    {
        if (value.IsSymbolValue())
        {
            cmajor::systemx::object::Symbol* symbol = value.GetSymbol();
            symbol->SetLinkage(cmajor::systemx::object::Linkage::external);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs symbol operands", currentInst->GetSourcePos());
        }
    }
}

class LinkOnceInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void LinkOnceInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    for (auto& value : currentInst->Operands())
    {
        if (value.IsSymbolValue())
        {
            cmajor::systemx::object::Symbol* symbol = value.GetSymbol();
            symbol->SetLinkage(cmajor::systemx::object::Linkage::once);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs symbol operands", currentInst->GetSourcePos());
        }
    }
}

class FuncInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void FuncInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    cmajor::systemx::object::Symbol* functionSymbol = assembler.CurrentSymbol();
    functionSymbol->SetValue(
        cmajor::systemx::object::Value(assembler.CurrentSection()->Address(),
            cmajor::systemx::object::ValueFlags::definition | cmajor::systemx::object::ValueFlags::pure | cmajor::systemx::object::ValueFlags::address | cmajor::systemx::object::ValueFlags::function,
            functionSymbol));
    functionSymbol->SetStart(assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address());
    functionSymbol->SetAlignment(4);
    assembler.SetParentIndex(functionSymbol->Index());
    assembler.SetCurrentFunctionSymbol(functionSymbol);
    cmajor::systemx::object::StartFuncRecord* startFuncRecord = new cmajor::systemx::object::StartFuncRecord(functionSymbol->Index());
    assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(startFuncRecord);
}

class EndFuncInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void EndFuncInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    cmajor::systemx::object::Symbol* functionSymbol = assembler.CurrentSymbol();
    functionSymbol->SetLength(assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - functionSymbol->Start());
    assembler.SetParentIndex(-1);
    assembler.SetCurrentFunctionSymbol(nullptr);
    cmajor::systemx::object::EndFuncRecord* endFuncRecord = new cmajor::systemx::object::EndFuncRecord(functionSymbol->Index());
    assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(endFuncRecord);
}

class StructureInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void StructureInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    cmajor::systemx::object::Symbol* structureSymbol = assembler.CurrentSymbol();
    structureSymbol->SetValue(
        cmajor::systemx::object::Value(assembler.CurrentSection()->Address(),
            cmajor::systemx::object::ValueFlags::definition | cmajor::systemx::object::ValueFlags::pure | cmajor::systemx::object::ValueFlags::address | cmajor::systemx::object::ValueFlags::structure,
            structureSymbol));
    structureSymbol->SetStart(assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address());
    structureSymbol->SetAlignment(8);
    assembler.SetParentIndex(structureSymbol->Index());
    assembler.SetCurrentStructureSymbol(structureSymbol);
};

class EndStructureInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void EndStructureInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    cmajor::systemx::object::Symbol* structureSymbol = assembler.CurrentSymbol();
    structureSymbol->SetLength(assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - structureSymbol->Start());
    assembler.SetParentIndex(-1);
    assembler.SetCurrentStructureSymbol(nullptr);
}

class BeginSpecInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void BeginSpecInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    assembler.BeginSpec();
}

class EndSpecInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void EndSpecInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    assembler.EndSpec();
}

class ByteInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void ByteInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (assembler.CurrentSection()->IsDataSection())
    {
        int n = currentInst->Operands().size();
        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                assembler.EmitPureByteValue("byte", currentInst->Operands()[i]);
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs at least one operand", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " can appear only .DATA section", currentInst->GetSourcePos());
    }
}

class WydeInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void WydeInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (assembler.CurrentSection()->IsDataSection())
    {
        int n = currentInst->Operands().size();
        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                assembler.EmitPureWydeValue("wyde", currentInst->Operands()[i]);
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs at least one operand", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " can appear only .DATA section", currentInst->GetSourcePos());
    }
}

class TetraInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void TetraInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (assembler.CurrentSection()->IsDataSection())
    {
        int n = currentInst->Operands().size();
        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                assembler.EmitPureTetraValue("tetra", currentInst->Operands()[i]);
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs at least one operand", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " can appear only .DATA section", currentInst->GetSourcePos());
    }
}

class OctaInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void OctaInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (assembler.InSpec())
    {
        int n = currentInst->Operands().size();
        if (n > 0)
        {
            cmajor::systemx::object::Value discriminator = currentInst->Operands()[0];
            switch (discriminator.Val())
            {
                case FILEINFO:
                {
                    if (n == 3)
                    {
                        cmajor::systemx::object::Value sourceFileNameValue = currentInst->Operands()[1];
                        const std::string& sourceFileName = assembler.GetString(sourceFileNameValue.Val(), currentInst->GetSourcePos());
                        cmajor::systemx::object::Value sourceFileId = currentInst->Operands()[2];
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(new cmajor::systemx::object::FileInfoRecord(sourceFileName, sourceFileId.Val()));
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA FILEINFO requires three operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case FUNCINFO:
                {
                    if (n > 6)
                    {
                        cmajor::systemx::object::Value functionValue = currentInst->Operands()[1];
                        cmajor::systemx::object::Symbol* symbol = functionValue.GetSymbol();
                        if (symbol)
                        {
                            if (symbol->Index() != -1)
                            {
                                uint32_t functionSymbolIndex = static_cast<uint32_t>(symbol->Index());
                                cmajor::systemx::object::Value fullNameValue = currentInst->Operands()[2];
                                const std::string& fullName = assembler.GetString(fullNameValue.Val(), currentInst->GetSourcePos());
                                if (!fullName.empty())
                                {
                                    cmajor::systemx::object::Value sourceFileNameIdValue = currentInst->Operands()[3];
                                    cmajor::systemx::object::Value frameSizeValue = currentInst->Operands()[4];
                                    cmajor::systemx::object::Value mainValue = currentInst->Operands()[5];
                                    cmajor::systemx::object::Value cfgSizeValue = currentInst->Operands()[6];
                                    std::vector<std::pair<int32_t, int32_t>> cfg;
                                    uint64_t cfgSize = cfgSizeValue.Val();
                                    int operandIndex = 7;
                                    for (uint64_t i = 0; i < cfgSize; ++i)
                                    {
                                        cmajor::systemx::object::Value prevLine = currentInst->Operands()[operandIndex++];
                                        cmajor::systemx::object::Value nextLine = currentInst->Operands()[operandIndex++];
                                        cfg.push_back(std::make_pair(static_cast<int32_t>(prevLine.Val()), static_cast<int32_t>(nextLine.Val())));
                                    }
                                    cmajor::systemx::object::FuncInfoRecord* funcInfoRecord = new cmajor::systemx::object::FuncInfoRecord(
                                        functionSymbolIndex, fullName, sourceFileNameIdValue.Val(), frameSizeValue.Val(), mainValue.Val() == 1);
                                    for (const auto& cfgRecord : cfg)
                                    {
                                        funcInfoRecord->AddToCfg(cfgRecord.first, cfgRecord.second);
                                    }
                                    assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(funcInfoRecord);
                                }
                                else
                                {
                                    assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA FUNCINFO full function name not defined", currentInst->GetSourcePos());
                                }
                            }
                            else
                            {
                                assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA FUNCINFO symbol index not defined", currentInst->GetSourcePos());
                            }
                        }
                        else
                        {
                            assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA FUNCINFO operand 1 must be a symbol", currentInst->GetSourcePos());
                        }
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA FUNCINFO requires at least 7 operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case LINEINFO:
                {
                    if (n == 2)
                    {
                        cmajor::systemx::object::Value lineValue = currentInst->Operands()[1];
                        uint32_t offset = static_cast<uint32_t>(
                            assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - assembler.CurrentFunctionSymbol()->Start());
                        cmajor::systemx::object::LineInfoRecord* lineInfoRecord = new cmajor::systemx::object::LineInfoRecord(offset, static_cast<uint32_t>(lineValue.Val()));
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(lineInfoRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA LINEINFO requires two operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case BEGINTRY:
                {
                    if (n == 3)
                    {
                        cmajor::systemx::object::Value tryBlockIdValue = currentInst->Operands()[1];
                        uint32_t tryBlockId = static_cast<uint32_t>(tryBlockIdValue.Val());
                        cmajor::systemx::object::Value parentTryBlockIdValue = currentInst->Operands()[2];
                        uint32_t parentTryBlockId = static_cast<uint32_t>(parentTryBlockIdValue.Val());
                        uint32_t offset = static_cast<uint32_t>(
                            assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - assembler.CurrentFunctionSymbol()->Start());
                        cmajor::systemx::object::BeginTryRecord* beginTryRecord = new cmajor::systemx::object::BeginTryRecord(tryBlockId, parentTryBlockId, offset);
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(beginTryRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA BEGINTRY requires three operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case ENDTRY:
                {
                    if (n == 2)
                    {
                        cmajor::systemx::object::Value tryBlockIdValue = currentInst->Operands()[1];
                        uint32_t tryBlockId = static_cast<uint32_t>(tryBlockIdValue.Val());
                        uint32_t offset = static_cast<uint32_t>(
                            assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - assembler.CurrentFunctionSymbol()->Start());
                        cmajor::systemx::object::EndTryRecord* endTryRecord = new cmajor::systemx::object::EndTryRecord(tryBlockId, offset);
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(endTryRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA ENDTRY requires two operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case CATCH:
                {
                    if (n == 5)
                    {
                        cmajor::systemx::object::Value catchBlockIdValue = currentInst->Operands()[1];
                        uint32_t catchBlockId = static_cast<uint32_t>(catchBlockIdValue.Val());
                        cmajor::systemx::object::Value tryBlockIdValue = currentInst->Operands()[2];
                        uint32_t tryBlockId = static_cast<uint32_t>(tryBlockIdValue.Val());
                        cmajor::systemx::object::Value caughtTypeId1Value = currentInst->Operands()[3];
                        uint64_t caughtTypeId1 = caughtTypeId1Value.Val();
                        cmajor::systemx::object::Value caughtTypeId2Value = currentInst->Operands()[4];
                        uint64_t caughtTypeId2 = caughtTypeId2Value.Val();
                        cmajor::systemx::object::CatchRecord* catchRecord = new cmajor::systemx::object::CatchRecord(catchBlockId, tryBlockId, caughtTypeId1, caughtTypeId2);
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(catchRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA CATCH requires five operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case BEGINCLEANUP:
                {
                    if (n == 3)
                    {
                        cmajor::systemx::object::Value cleanupBlockIdValue = currentInst->Operands()[1];
                        uint32_t cleanupBlockId = static_cast<uint32_t>(cleanupBlockIdValue.Val());
                        cmajor::systemx::object::Value tryBlockIdValue = currentInst->Operands()[2];
                        uint32_t tryBlockId = static_cast<uint32_t>(tryBlockIdValue.Val());
                        uint32_t offset = static_cast<uint32_t>(
                            assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - assembler.CurrentFunctionSymbol()->Start());
                        cmajor::systemx::object::BeginCleanupRecord* beginCleanupRecord = new cmajor::systemx::object::BeginCleanupRecord(cleanupBlockId, tryBlockId, offset);
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(beginCleanupRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA BEGINCLEANUP requires three operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                case ENDCLEANUP:
                {
                    if (n == 2)
                    {
                        cmajor::systemx::object::Value cleanupBlockIdValue = currentInst->Operands()[1];
                        uint32_t cleanupBlockId = static_cast<uint32_t>(cleanupBlockIdValue.Val());
                        uint32_t offset = static_cast<uint32_t>(
                            assembler.CurrentSection()->BaseAddress() + assembler.CurrentSection()->Address() - assembler.CurrentFunctionSymbol()->Start());
                        cmajor::systemx::object::EndCleanupRecord* endCleanupRecord = new cmajor::systemx::object::EndCleanupRecord(cleanupBlockId, offset);
                        assembler.GetObjectFile()->GetDebugSection()->AddDebugRecord(endCleanupRecord);
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + ": OCTA ENDCLEANUP requires two operands", currentInst->GetSourcePos());
                    }
                    break;
                }
                default:
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + ": unknown discriminator value", currentInst->GetSourcePos());
                    break;
                }
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs at least one operand", currentInst->GetSourcePos());
        }
    }
    else
    {
        if (assembler.CurrentSection()->IsDataSection())
        {
            int n = currentInst->Operands().size();
            if (n > 0)
            {
                for (int i = 0; i < n; ++i)
                {
                    cmajor::systemx::object::Value value = currentInst->Operands()[i];
                    if (value.IsSymbolValue())
                    {
                        assembler.EmitSymbolOcta(value);
                    }
                    else if (value.IsPureValue())
                    {
                        assembler.EmitPureOctaValue("octa", value);
                    }
                    else if (value.IsTypeIdIndex())
                    {
                        assembler.EmitClsIdCommmand(value.Val(), currentInst->GetSourcePos());
                    }
                    else
                    {
                        assembler.Error(currentInst->GetOpCode()->Name() + " operand must be a symbol, pure or type id index", currentInst->GetSourcePos());
                    }
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs at least one operand", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " can appear only .DATA section", currentInst->GetSourcePos());
        }
    }
}

class SetInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void SetInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (currentInst->Operands().size() == 2)
    {
        if (currentInst->Operands()[0].IsRegValue())
        {
            if (currentInst->Operands()[1].IsRegValue())
            {
                assembler.EmitOpCode(cmajor::systemx::machine::ORI);
                assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
                assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
                assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
            }
            else if (currentInst->Operands()[1].IsPureValue())
            {
                assembler.EmitSetPureRegValue(currentInst->Operands()[0], currentInst->Operands()[1]);
            }
            else if (currentInst->Operands()[1].IsSymbolValue())
            {
                cmajor::systemx::object::Symbol* symbol = currentInst->Operands()[1].GetSymbol();
                assembler.EmitAbsoluteRegisterValue("X", currentInst->Operands()[0], symbol);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a register or pure value", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two operands", currentInst->GetSourcePos());
    }
}

class LoadInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void LoadInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        uint8_t opc = static_cast<uint8_t>(currentInst->GetOpCode()->Value());
        if (currentInst->Operands()[1].IsRegValue())
        {
            if (n == 2 || n == 3 && currentInst->Operands()[2].IsPureValue())
            {
                ++opc;
            }
            assembler.EmitOpCode(opc);
            if (currentInst->Operands()[0].IsRegValue())
            {
                assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
            if (n == 2)
            {
                assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
            }
            else if (n == 3)
            {
                if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
                }
                else if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
        }
        else if (currentInst->Operands()[1].IsSymbolValue())
        {
            assembler.EmitAbsoluteRegisterValue("X", currentInst->Operands()[0], currentInst->Operands()[1]);
            assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value() + 1));
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a register or a symbol", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class StoreInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void StoreInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        uint8_t opc = static_cast<uint8_t>(currentInst->GetOpCode()->Value());
        if (currentInst->Operands()[1].IsRegValue())
        {
            if (n == 2 || n == 3 && currentInst->Operands()[2].IsPureValue())
            {
                ++opc;
            }
            assembler.EmitOpCode(opc);
            if (currentInst->Operands()[0].IsRegValue())
            {
                assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
            if (n == 2)
            {
                assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
            }
            else if (n == 3)
            {
                if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
                }
                else if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
        }
        else if (currentInst->Operands()[1].IsSymbolValue())
        {
            assembler.EmitAbsoluteRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg), currentInst->Operands()[1]);
            assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value() + 1));
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            assembler.EmitRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg));
            assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a register or a symbol", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class StoreConstantInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void StoreConstantInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        uint8_t opc = static_cast<uint8_t>(currentInst->GetOpCode()->Value());
        if (n == 2 || n == 3 && currentInst->Operands()[2].IsPureValue())
        {
            ++opc;
        }
        if (currentInst->Operands()[1].IsRegValue())
        {
            assembler.EmitOpCode(opc);
            if (currentInst->Operands()[0].IsPureValue())
            {
                assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be pure", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
            if (n == 3)
            {
                if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
                }
                else if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else if (n == 2)
            {
                assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
        }
        else if (currentInst->Operands()[1].IsSymbolValue())
        {
            assembler.EmitAbsoluteRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg), currentInst->Operands()[1]);
            assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value() + 1));
            assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
            assembler.EmitRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg));
            assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a register or a symbol", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class RegisterInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void RegisterInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        if (currentInst->Operands()[0].IsRegValue() && currentInst->Operands()[1].IsRegValue())
        {
            int opc = currentInst->GetOpCode()->Value();
            if (opc == cmajor::systemx::machine::DIVU)
            {
                assembler.EmitOpCode(cmajor::systemx::machine::SETL);
                assembler.EmitRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg));
                assembler.EmitPureWydeValue("zero", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
                assembler.EmitOpCode(cmajor::systemx::machine::PUT);
                assembler.EmitPureByteValue("X", cmajor::systemx::object::Value(static_cast<uint64_t>(cmajor::systemx::machine::rD)));
                assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
                assembler.EmitRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg));
            }
            if (n == 2)
            {
                if (opc == LDA)
                {
                    opc = cmajor::systemx::machine::ADDU;
                }
                assembler.EmitOpCode(static_cast<uint8_t>(opc + 1));
            }
            else if (n == 3)
            {
                if (opc == LDA)
                {
                    opc = cmajor::systemx::machine::ADDU;
                }
                if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitOpCode(static_cast<uint8_t>(opc + 1));
                }
                else if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitOpCode(static_cast<uint8_t>(opc));
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X and Y operands must be registers", currentInst->GetSourcePos());
        }
        if (n == 2)
        {
            assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        }
        else if (n == 3)
        {
            if (currentInst->Operands()[2].IsRegValue())
            {
                assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
            }
            else if (currentInst->Operands()[2].IsPureValue())
            {
                assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class NegateInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void NegateInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        if (currentInst->Operands()[0].IsRegValue())
        {
            uint8_t opc = currentInst->GetOpCode()->Value();
            if (n == 3)
            {
                if (currentInst->Operands()[2].IsPureValue())
                {
                    ++opc;
                }
                assembler.EmitOpCode(opc);
                assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
                if (currentInst->Operands()[1].IsPureValue())
                {
                    assembler.EmitPureByteValue("Y", currentInst->Operands()[1]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be pure", currentInst->GetSourcePos());
                }
                if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
                }
                else if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else if (n == 2)
            {
                if (currentInst->Operands()[1].IsPureValue())
                {
                    ++opc;
                }
                assembler.EmitOpCode(opc);
                assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
                assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
                if (currentInst->Operands()[1].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[1]);
                }
                else if (currentInst->Operands()[1].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[1]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class FloatingPointInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void FloatingPointInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 3)
    {
        if (currentInst->Operands()[0].IsRegValue() && currentInst->Operands()[1].IsRegValue() && currentInst->Operands()[2].IsRegValue())
        {
            assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value()));
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
            assembler.EmitRegisterValue("Y", currentInst->Operands()[1]);
            assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs three register operands", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs three operands", currentInst->GetSourcePos());
    }
}

class FloatingRoundingInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void FloatingRoundingInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        int opc = currentInst->GetOpCode()->Value();
        if (n == 2)
        {
            assembler.EmitOpCode(static_cast<uint8_t>(opc));
        }
        else if (n == 3)
        {
            if (currentInst->Operands()[2].IsRegValue())
            {
                assembler.EmitOpCode(static_cast<uint8_t>(opc));
            }
            else if (currentInst->Operands()[2].IsPureValue())
            {
                assembler.EmitOpCode(static_cast<uint8_t>(opc + 1));
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
            }
        }
        if (currentInst->Operands()[0].IsRegValue())
        {
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
        if (n == 2)
        {
            assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(1)));
        }
        else if (n == 3)
        {
            if (currentInst->Operands()[1].IsPureValue())
            {
                assembler.EmitPureByteValue("Y", currentInst->Operands()[1]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be pure", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
        }
        if (n == 2)
        {
            assembler.EmitRegisterValue("Z", currentInst->Operands()[1]);
        }
        else if (n == 3)
        {
            if (currentInst->Operands()[2].IsRegValue())
            {
                assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
            }
            else if (currentInst->Operands()[2].IsPureValue())
            {
                assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class WydeImmediateInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void WydeImmediateInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2)
    {
        assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value()));
        if (currentInst->Operands()[0].IsRegValue())
        {
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
        assembler.EmitPureWydeValue("Y", currentInst->Operands()[1]);
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two operands", currentInst->GetSourcePos());
    }
}

class JumpInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void JumpInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    if (currentInst->Operands().size() == 1)
    {
        cmajor::systemx::object::Value value = currentInst->Operands()[0];
        if (value.IsSymbolValue())
        {
            cmajor::systemx::object::Symbol* symbol = value.GetSymbol();
            if (symbol->GetLinkage() == cmajor::systemx::object::Linkage::internal || symbol->GetLinkage() == cmajor::systemx::object::Linkage::external)
            {
                if (value.GetFlag(cmajor::systemx::object::ValueFlags::address))
                {
                    uint64_t to = value.Val();
                    uint64_t at = assembler.CurrentSection()->Address();
                    if (at >= to)
                    {
                        uint64_t offset = (at - to) >> 2;
                        if (offset <= static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()) << 8)
                        {
                            assembler.EmitOpCode(cmajor::systemx::machine::JMPB);
                            assembler.EmitLongOffset(static_cast<uint32_t>(offset));
                        }
                        else
                        {
                            assembler.Error(currentInst->GetOpCode()->Name() + " too far", currentInst->GetSourcePos());
                        }
                    }
                    else
                    {
                        uint64_t offset = (to - at) >> 2;
                        if (offset <= static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()) << 8)
                        {
                            assembler.EmitOpCode(cmajor::systemx::machine::JMP);
                            assembler.EmitLongOffset(static_cast<uint32_t>(offset));
                        }
                        else
                        {
                            assembler.Error(currentInst->GetOpCode()->Name() + " too far", currentInst->GetSourcePos());
                        }
                    }
                }
                else if (value.GetFlag(cmajor::systemx::object::ValueFlags::undefined))
                {
                    assembler.EmitForwardLongJump(symbol);
                }
                else
                {
                    assembler.Error("invalid " + currentInst->GetOpCode()->Name() + " X operand", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " to undefined symbol " + symbol->FullName(), currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a symbol", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs one operand", currentInst->GetSourcePos());
    }
}

class BranchInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void BranchInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2)
    {
        uint8_t opc = static_cast<uint8_t>(currentInst->GetOpCode()->Value());
        if (currentInst->Operands()[0].IsRegValue())
        {
            cmajor::systemx::object::Value operand1 = currentInst->Operands()[1];
            if (operand1.IsSymbolValue())
            {
                cmajor::systemx::object::Symbol* symbol = operand1.GetSymbol();
                if (operand1.GetFlag(cmajor::systemx::object::ValueFlags::address))
                {
                    uint64_t to = operand1.Val();
                    uint64_t at = assembler.CurrentSection()->Address();
                    if (at >= to)
                    {
                        uint64_t offset = (at - to) >> 2;
                        if (offset <= static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()))
                        {
                            assembler.EmitOpCode(opc + 1);
                            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
                            assembler.EmitShortOffset(static_cast<uint16_t>(offset));
                        }
                        else
                        {
                            assembler.Error(currentInst->GetOpCode()->Name() + " too far", currentInst->GetSourcePos());
                        }
                    }
                    else
                    {
                        uint64_t offset = (to - at) >> 2;
                        if (offset <= static_cast<uint64_t>(std::numeric_limits<uint16_t>::max()))
                        {
                            assembler.EmitOpCode(opc);
                            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
                            assembler.EmitShortOffset(static_cast<uint16_t>(offset));
                        }
                        else
                        {
                            assembler.Error(currentInst->GetOpCode()->Name() + " too far", currentInst->GetSourcePos());
                        }
                    }
                }
                else if (operand1.GetFlag(cmajor::systemx::object::ValueFlags::undefined))
                {
                    assembler.EmitForwardShortJump("X", opc, currentInst->Operands()[0], symbol);
                }
                else
                {
                    assembler.Error("invalid " + currentInst->GetOpCode()->Name() + " Y operand", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a symbol", currentInst->GetSourcePos());
            }
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two operands", currentInst->GetSourcePos());
    }
}

class GetInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void GetInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2)
    {
        assembler.EmitOpCode(cmajor::systemx::machine::GET);
        if (currentInst->Operands()[0].IsRegValue())
        {
            assembler.EmitRegisterValue("X", currentInst->Operands()[0]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be a register", currentInst->GetSourcePos());
        }
        assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        if (currentInst->Operands()[1].IsPureValue())
        {
            assembler.EmitPureByteValue("Z", currentInst->Operands()[1]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be pure", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two operands", currentInst->GetSourcePos());
    }
}

class PutInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void PutInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2)
    {
        assembler.EmitOpCode(cmajor::systemx::machine::PUT);
        if (currentInst->Operands()[0].IsPureValue())
        {
            assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be pure", currentInst->GetSourcePos());
        }
        assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        if (currentInst->Operands()[1].IsRegValue())
        {
            assembler.EmitRegisterValue("Z", currentInst->Operands()[1]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two operands", currentInst->GetSourcePos());
    }
}

class CallInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void CallInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 2 || n == 3)
    {
        uint8_t opc = currentInst->GetOpCode()->Value();
        cmajor::systemx::object::Value operand1 = currentInst->Operands()[1];
        if (operand1.IsRegValue())
        {
            if (n == 2 || n == 3 && currentInst->Operands()[2].IsPureValue())
            {
                ++opc;
            }
            assembler.EmitOpCode(opc);
            if (currentInst->Operands()[0].IsPureValue())
            {
                assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be pure", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("Y", operand1);
            if (n == 2)
            {
                assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
            }
            else if (n == 3)
            {
                if (currentInst->Operands()[2].IsRegValue())
                {
                    assembler.EmitRegisterValue("Z", currentInst->Operands()[2]);
                }
                else if (currentInst->Operands()[2].IsPureValue())
                {
                    assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
                }
                else
                {
                    assembler.Error(currentInst->GetOpCode()->Name() + " Z operand must be a register or pure", currentInst->GetSourcePos());
                }
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
            }
        }
        else if (operand1.IsSymbolValue())
        {
            cmajor::systemx::object::Symbol* symbol = operand1.GetSymbol();
            assembler.EmitAbsoluteRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg), operand1);
            assembler.EmitOpCode(static_cast<uint8_t>(currentInst->GetOpCode()->Value() + 1));
            if (currentInst->Operands()[0].IsPureValue())
            {
                assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
            }
            else
            {
                assembler.Error(currentInst->GetOpCode()->Name() + " X operand must be pure", currentInst->GetSourcePos());
            }
            assembler.EmitRegisterValue("ex", cmajor::systemx::object::Value(cmajor::systemx::machine::regEX, cmajor::systemx::object::ValueFlags::reg));
            assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " Y operand must be a register or symbol", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs two or three operands", currentInst->GetSourcePos());
    }
}

class ReturnInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void ReturnInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 0)
    {
        assembler.EmitOpCode(cmajor::systemx::machine::RET);
        assembler.EmitPureByteValue("X", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " takes no operands", currentInst->GetSourcePos());
    }
}

class TrapInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void TrapInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 3)
    {
        if (currentInst->Operands()[0].IsPureValue() && currentInst->Operands()[1].IsPureValue() && currentInst->Operands()[2].IsPureValue())
        {
            assembler.EmitOpCode(cmajor::systemx::machine::TRAP);
            assembler.EmitPureByteValue("X", currentInst->Operands()[0]);
            assembler.EmitPureByteValue("Y", currentInst->Operands()[1]);
            assembler.EmitPureByteValue("Z", currentInst->Operands()[2]);
        }
        else
        {
            assembler.Error(currentInst->GetOpCode()->Name() + " needs three pure operands", currentInst->GetSourcePos());
        }
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " needs three operands", currentInst->GetSourcePos());
    }
}

class SwymInstruction : public AssemblyInstruction
{
public:
    void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) override;
};

void SwymInstruction::Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler)
{
    Instruction* currentInst = assembler.CurrentInstruction();
    int n = currentInst->Operands().size();
    if (n == 0)
    {
        assembler.EmitOpCode(cmajor::systemx::machine::SWYM);
        assembler.EmitPureByteValue("X", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        assembler.EmitPureByteValue("Y", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
        assembler.EmitPureByteValue("Z", cmajor::systemx::object::Value(static_cast<uint64_t>(0)));
    }
    else
    {
        assembler.Error(currentInst->GetOpCode()->Name() + " takes no operands", currentInst->GetSourcePos());
    }
}

class AssemblyInstructionMap
{
public:
    static AssemblyInstructionMap& Instance();
    AssemblyInstruction* GetAssemblyInstruction(int opCode) const;
private:
    AssemblyInstructionMap();
    AssemblyInstruction* AddInstruction(AssemblyInstruction* instruction);
    void MapInstruction(int opc, AssemblyInstruction* instruction);
    static std::unique_ptr<AssemblyInstructionMap> instance;
    std::vector<std::unique_ptr<AssemblyInstruction>> instructions;
    std::map<int, AssemblyInstruction*> instructionMap;
};

AssemblyInstructionMap& AssemblyInstructionMap::Instance()
{
    static AssemblyInstructionMap instance;
    return instance;
}

AssemblyInstructionMap::AssemblyInstructionMap()
{
    AssemblyInstruction* externInstruction = AddInstruction(new ExternInstruction());
    MapInstruction(cmajor::systemx::assembler::EXTERN, externInstruction);
    AssemblyInstruction* linkOnceInstruction = AddInstruction(new LinkOnceInstruction());
    MapInstruction(cmajor::systemx::assembler::LINKONCE, linkOnceInstruction);
    AssemblyInstruction* funcInstruction = AddInstruction(new FuncInstruction());
    MapInstruction(cmajor::systemx::assembler::FUNC, funcInstruction);
    AssemblyInstruction* endFuncInstruction = AddInstruction(new EndFuncInstruction());
    MapInstruction(cmajor::systemx::assembler::ENDF, endFuncInstruction);
    AssemblyInstruction* structInstruction = AddInstruction(new StructureInstruction());
    MapInstruction(cmajor::systemx::assembler::STRUCT, structInstruction);
    AssemblyInstruction* endStructInstruction = AddInstruction(new EndStructureInstruction());
    MapInstruction(cmajor::systemx::assembler::ENDS, endStructInstruction);
    AssemblyInstruction* byteInstruction = AddInstruction(new ByteInstruction());
    MapInstruction(cmajor::systemx::assembler::BYTE, byteInstruction);
    AssemblyInstruction* wydeInstruction = AddInstruction(new WydeInstruction());
    MapInstruction(cmajor::systemx::assembler::WYDE, wydeInstruction);
    AssemblyInstruction* tetraInstruction = AddInstruction(new TetraInstruction());
    MapInstruction(cmajor::systemx::assembler::TETRA, tetraInstruction);
    AssemblyInstruction* octaInstruction = AddInstruction(new OctaInstruction());
    MapInstruction(cmajor::systemx::assembler::OCTA, octaInstruction);
    AssemblyInstruction* setInstruction = AddInstruction(new SetInstruction());
    MapInstruction(cmajor::systemx::assembler::SET, setInstruction);
    AssemblyInstruction* loadInstruction = AddInstruction(new LoadInstruction());
    MapInstruction(cmajor::systemx::machine::LDB, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDBU, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDW, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDWU, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDT, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDTU, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDO, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDOU, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDHT, loadInstruction);
    MapInstruction(cmajor::systemx::machine::LDSF, loadInstruction);
    AssemblyInstruction* storeInstruction = AddInstruction(new StoreInstruction());
    MapInstruction(cmajor::systemx::machine::STB, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STBU, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STW, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STWU, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STT, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STTU, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STO, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STOU, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STHT, storeInstruction);
    MapInstruction(cmajor::systemx::machine::STSF, storeInstruction);
    AssemblyInstruction* storeConstantInstruction = AddInstruction(new StoreConstantInstruction());
    MapInstruction(cmajor::systemx::machine::STCO, storeInstruction);
    AssemblyInstruction* registerInstruction = AddInstruction(new RegisterInstruction());
    MapInstruction(cmajor::systemx::assembler::LDA, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ADD, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ADDU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SUB, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SUBU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::MUL, registerInstruction);
    MapInstruction(cmajor::systemx::machine::MULU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::DIV, registerInstruction);
    MapInstruction(cmajor::systemx::machine::DIVU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::I2ADDU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::I4ADDU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::I8ADDU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::I16ADDU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SL, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SLU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SR, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SRU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CMP, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CMPU, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSZ, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSP, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSOD, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSNN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSNZ, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSNP, registerInstruction);
    MapInstruction(cmajor::systemx::machine::CSEV, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSZ, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSP, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSOD, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSNN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSNZ, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSNP, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ZSEV, registerInstruction);
    MapInstruction(cmajor::systemx::machine::AND, registerInstruction);
    MapInstruction(cmajor::systemx::machine::OR, registerInstruction);
    MapInstruction(cmajor::systemx::machine::XOR, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ANDN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ORN, registerInstruction);
    MapInstruction(cmajor::systemx::machine::NAND, registerInstruction);
    MapInstruction(cmajor::systemx::machine::NOR, registerInstruction);
    MapInstruction(cmajor::systemx::machine::NXOR, registerInstruction);
    MapInstruction(cmajor::systemx::machine::MUX, registerInstruction);
    MapInstruction(cmajor::systemx::machine::SADD, registerInstruction);
    MapInstruction(cmajor::systemx::machine::BDIF, registerInstruction);
    MapInstruction(cmajor::systemx::machine::WDIF, registerInstruction);
    MapInstruction(cmajor::systemx::machine::TDIF, registerInstruction);
    MapInstruction(cmajor::systemx::machine::ODIF, registerInstruction);
    MapInstruction(cmajor::systemx::machine::GO, registerInstruction);
    AssemblyInstruction* negateInstruction = AddInstruction(new NegateInstruction());
    MapInstruction(cmajor::systemx::machine::NEG, negateInstruction);
    MapInstruction(cmajor::systemx::machine::NEGU, negateInstruction);
    AssemblyInstruction* floatingPointInstruction = AddInstruction(new FloatingPointInstruction());
    MapInstruction(cmajor::systemx::machine::FADD, floatingPointInstruction);
    MapInstruction(cmajor::systemx::machine::FSUB, floatingPointInstruction);
    MapInstruction(cmajor::systemx::machine::FMUL, floatingPointInstruction);
    MapInstruction(cmajor::systemx::machine::FDIV, floatingPointInstruction);
    MapInstruction(cmajor::systemx::machine::FCMP, floatingPointInstruction);
    AssemblyInstruction* floatingRoundingInstruction = AddInstruction(new FloatingRoundingInstruction());
    MapInstruction(cmajor::systemx::machine::FINT, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::FIX, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::FIXU, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::FLOT, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::FLOTU, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::SFLOT, floatingRoundingInstruction);
    MapInstruction(cmajor::systemx::machine::SFLOTU, floatingRoundingInstruction);
    AssemblyInstruction* wydeImmediateInstruction = AddInstruction(new WydeImmediateInstruction());
    MapInstruction(cmajor::systemx::machine::SETH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::SETMH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::SETML, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::SETL, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::INCH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::INCMH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::INCML, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::INCL, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ORH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ORMH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ORML, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ORL, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ANDNH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ANDNMH, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ANDNML, wydeImmediateInstruction);
    MapInstruction(cmajor::systemx::machine::ANDNL, wydeImmediateInstruction);
    AssemblyInstruction* jumpInstruction = AddInstruction(new JumpInstruction());
    MapInstruction(cmajor::systemx::machine::JMP, jumpInstruction);
    AssemblyInstruction* branchInstruction = AddInstruction(new BranchInstruction());
    MapInstruction(cmajor::systemx::machine::BN, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BZ, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BP, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BOD, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BNN, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BNZ, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BNP, branchInstruction);
    MapInstruction(cmajor::systemx::machine::BEV, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBN, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBZ, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBP, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBOD, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBNN, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBNZ, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBNP, branchInstruction);
    MapInstruction(cmajor::systemx::machine::PBEV, branchInstruction);
    AssemblyInstruction* getInstruction = AddInstruction(new GetInstruction());
    MapInstruction(cmajor::systemx::machine::GET, getInstruction);
    AssemblyInstruction* putInstruction = AddInstruction(new PutInstruction());
    MapInstruction(cmajor::systemx::machine::PUT, putInstruction);
    AssemblyInstruction* callInstruction = AddInstruction(new CallInstruction());
    MapInstruction(cmajor::systemx::machine::CALL, callInstruction);
    AssemblyInstruction* returnInstruction = AddInstruction(new ReturnInstruction());
    MapInstruction(cmajor::systemx::machine::RET, returnInstruction);
    AssemblyInstruction* trapInstruction = AddInstruction(new TrapInstruction());
    MapInstruction(cmajor::systemx::machine::TRAP, trapInstruction);
    AssemblyInstruction* swymInstruction = AddInstruction(new SwymInstruction());
    MapInstruction(cmajor::systemx::machine::SWYM, swymInstruction);
    AssemblyInstruction* beginSpecInstruction = AddInstruction(new BeginSpecInstruction());
    MapInstruction(cmajor::systemx::assembler::BSPEC, beginSpecInstruction);
    AssemblyInstruction* endSpecInstruction = AddInstruction(new EndSpecInstruction());
    MapInstruction(cmajor::systemx::assembler::ESPEC, endSpecInstruction);
}

AssemblyInstruction* AssemblyInstructionMap::AddInstruction(AssemblyInstruction* instruction)
{
    instructions.push_back(std::unique_ptr<AssemblyInstruction>(instruction));
    return instruction;
}

void AssemblyInstructionMap::MapInstruction(int opc, AssemblyInstruction* instruction)
{
    instructionMap[opc] = instruction;
}

AssemblyInstruction* AssemblyInstructionMap::GetAssemblyInstruction(int opCode) const
{
    auto it = instructionMap.find(opCode);
    if (it != instructionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

AssemblyInstruction* GetAssemblyInstruction(int opc)
{
    return AssemblyInstructionMap::Instance().GetAssemblyInstruction(opc);
}

} // namespace cmajor::systemx::assembler
