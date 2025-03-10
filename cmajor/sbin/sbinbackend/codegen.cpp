// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.backend.codegen;

import cmajor.symbols;
import cmajor.sbin.assembly;

namespace cmajor::sbin::backend {

SBinCodeGenerator::SBinCodeGenerator(cmajor::ir::Emitter* emitter_) : emitter(emitter_), masmCodeGenerator(emitter)
{
}

void SBinCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{ 
    boundCompileUnit.Accept(masmCodeGenerator);
    cmajor::sbin::assembly::Assembler assembler(boundCompileUnit.AsmFilePath(), boundCompileUnit.GetModule().LogStreamId(), 
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose), cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::print));
    assembler.Assemble();
}

} // namespace cmajor::sbin::backend
