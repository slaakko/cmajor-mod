// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.assembly.instruction;

import cmajor.systemx.assembler.instruction;
import std.core;

export namespace cmajor::systemx::assembler {

export namespace assembler
{
    class Assembler;
}

class AssemblyInstruction
{
public:
    AssemblyInstruction();
    virtual ~AssemblyInstruction();
    virtual void Assemble(cmajor::systemx::assembler::assembler::Assembler& assembler) = 0;
};

AssemblyInstruction* GetAssemblyInstruction(int opc);

} // namespace cmsx::assembler
