// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.data;

import cmajor.masm.assembly.instruction;

namespace cmajor::masm::assembly {

Data::Data()
{
}

void Data::AddInstruction(std::unique_ptr<Instruction>&& instruction)
{
    if (instruction->HasOperands())
    {
        instructions.push_back(std::move(instruction));
    }
}

void Data::Write(util::CodeFormatter& formatter)
{
    for (const auto& instruction : instructions)
    {
        instruction->Write(formatter);
    }
}

PublicDataDeclaration::PublicDataDeclaration(const std::string& name_) : Declaration(name_)
{
}

void PublicDataDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.Write("PUBLIC ");
    formatter.WriteLine(Name());
}

ExternalDataDeclaration::ExternalDataDeclaration(const std::string& name_) : Declaration(name_)
{
}

void ExternalDataDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.Write("EXTRN ");
    formatter.Write(Name());
    formatter.WriteLine(":BYTE");
}

} // cmajor::masm::assembly
