// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.code_generator;

import cmajor.masm.assembly;
import cmajor.masm.intermediate.visitor;
import soul.ast.span;
import std.core;

export namespace cmajor::masm::intermediate {

class Instruction;
class RegisterAllocator;

class CodeGenerator : public Visitor
{
public:
    CodeGenerator(Context* context_, const std::string& assemblyFilePath_);
    Context* Ctx() const { return context; }
    RegisterAllocator* RegAllocator() const { return registerAllocator; }
    int ExitLabelId() const;
    Function* CurrentFunction() const { return currentFunction; }
    const soul::ast::Span& Span() const;
    void Emit(cmajor::masm::assembly::Instruction* assemblyInstruction);
    void EmitDataValue(cmajor::masm::assembly::Value* dataValue, cmajor::masm::assembly::OpCode dataOpCode);
    void Visit(GlobalVariable& globalVariable) override;
    void Visit(Function& function) override;
    void Visit(BasicBlock& basicBlock) override;
    void Visit(RetInstruction& inst) override;
    void Visit(SignExtendInstruction& inst) override;
    void Visit(ZeroExtendInstruction& inst) override;
    void Visit(ParamInstruction& inst) override;
    void Visit(LoadInstruction& inst) override;
    void Visit(StoreInstruction& inst) override;
    void Visit(ArgInstruction& inst) override;
    void Visit(ProcedureCallInstruction& inst) override;
    void Visit(FunctionCallInstruction& inst) override;
    void Visit(ElemAddrInstruction& inst) override;
    void Visit(PtrOffsetInstruction& inst) override;
    void Visit(PtrDiffInstruction& inst) override;
    void Visit(TruncateInstruction& inst) override;
    void Visit(AddInstruction& inst) override;
    void Visit(SubInstruction& inst) override;
    void Visit(MulInstruction& inst) override;
    void Visit(DivInstruction& inst) override;
    void Visit(ModInstruction& inst) override;
    void Visit(AndInstruction& inst) override;
    void Visit(OrInstruction& inst) override;
    void Visit(XorInstruction& inst) override;
    void Visit(ShlInstruction& inst) override;
    void Visit(ShrInstruction& inst) override;
    void Visit(BitcastInstruction& inst) override;
    void Visit(IntToFloatInstruction& inst) override;
    void Visit(FloatToIntInstruction& inst) override;
    void Visit(IntToPtrInstruction& inst) override;
    void Visit(PtrToIntInstruction& inst) override;
    void Visit(EqualInstruction& inst) override;
    void Visit(LessInstruction& inst) override;
    void Visit(JmpInstruction& inst) override;
    void Visit(BranchInstruction& inst) override;
    void Visit(NotInstruction& inst) override;
    void Visit(NegInstruction& inst) override;
    void Visit(NoOperationInstruction& inst) override;
    void Visit(SwitchInstruction& inst) override;
    void Visit(BoolValue& value) override;
    void Visit(SByteValue& value) override;
    void Visit(ByteValue& value) override;
    void Visit(ShortValue& value) override;
    void Visit(UShortValue& value) override;
    void Visit(IntValue& value) override;
    void Visit(UIntValue& value) override;
    void Visit(LongValue& value) override;
    void Visit(ULongValue& value) override;
    void Visit(FloatValue& value) override;
    void Visit(DoubleValue& value) override;
    void Visit(NullValue& value) override;
    void Visit(AddressValue& value) override;
    void Visit(ArrayValue& value) override;
    void Visit(StructureValue& value) override;
    void Visit(StringValue& value) override;
    void Visit(StringArrayValue& value) override;
    void Visit(ConversionValue& value) override;
    void Visit(ClsIdValue& value) override;
    void Visit(SymbolValue& value) override;
    void Error(const std::string& message);
    void WriteOutputFile();
private:
    Context* context;
    cmajor::masm::assembly::File file;
    Function* currentFunction;
    Instruction* currentInst;
    cmajor::masm::assembly::Function* assemblyFunction;
    RegisterAllocator* registerAllocator;
    std::vector<ArgInstruction*> args;
    bool leader;
    cmajor::masm::assembly::Data* data;
    std::string label;
    cmajor::masm::assembly::Instruction* dataInstruction;
    cmajor::masm::assembly::OpCode prevDataOpCode;
    int64_t currentOffset;
};

} // cmajor::masm::intermediate
