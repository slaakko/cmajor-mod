// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.data;

import cmajor.masm.assembly.declaration;
import util;
import std.core;

export namespace cmajor::masm::assembly {

class Instruction;

class Data
{
public:
    Data();
    void AddInstruction(Instruction* instruction);
    void Write(util::CodeFormatter& formatter);
private:
    std::vector<std::unique_ptr<Instruction>> instructions;
};

class PublicDataDeclaration : public Declaration
{
public:
    PublicDataDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

class ExternalDataDeclaration : public Declaration
{
public:
    ExternalDataDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

} // cmajor::masm::assembly
