// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.code_generator;

import cmajor.masm.assembly;
import cmajor.masm.intermediate.visitor;
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
    void Emit(cmajor::masm::assembly::Instruction* assemblyInstruction);
    void Visit(Function& function) override;
    void Visit(BasicBlock& basicBlock) override;
    void Visit(RetInstruction& inst) override;
    void Visit(SignExtendInstruction& inst) override;
    void Visit(ParamInstruction& inst) override;
    void Visit(LoadInstruction& inst) override;
    void Visit(StoreInstruction& inst) override;
    void Visit(ArgInstruction& inst) override;
    void Visit(ProcedureCallInstruction& inst) override;
    void Visit(FunctionCallInstruction& inst) override;
    void Error(const std::string& message);
    void AddFrameLocation(cmajor::masm::assembly::NumericLiteral* frameLoc);
    void WriteOutputFile();
private:
    Context* context;
    cmajor::masm::assembly::File file;
    Function* currentFunction;
    Instruction* currentInst;
    cmajor::masm::assembly::Function* assemblyFunction;
    RegisterAllocator* registerAllocator;
    std::vector<cmajor::masm::assembly::NumericLiteral*> frameLocations;
    std::vector<ArgInstruction*> args;
    bool leader;
};

} // cmajor::masm::intermediate
