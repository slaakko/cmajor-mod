// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.function;

import cmajor.masm.assembly.declaration;
import std.core;
import util;

export namespace cmajor::masm::assembly {

class FunctionDeclaration : public Declaration
{
public:
    FunctionDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter);
};

class Instruction;

enum class FunctionPart
{
    prologue, body, epilogue
};

class Function
{
public:
    Function(const std::string& name_);
    const std::string& Name() const { return name; }
    void SetActiveFunctionPart(FunctionPart activeFunctionPart_);
    void AddInstruction(Instruction* inst);
    void Write(util::CodeFormatter& formatter);
private:
    std::string name;
    FunctionPart activeFunctionPart;
    std::vector<std::unique_ptr<Instruction>> prologue;
    std::vector<std::unique_ptr<Instruction>> body;
    std::vector<std::unique_ptr<Instruction>> epilogue;
};

} // namespace cmajor::masm::assembly
