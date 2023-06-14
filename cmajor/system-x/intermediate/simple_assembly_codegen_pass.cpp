// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.simple.assembly.codegen.pass;

import cmajor.systemx.intermediate.simple.assembly.code.generator;
import cmajor.systemx.intermediate.context;
import cmajor.systemx.assembler;
import util;

namespace cmajor::systemx::intermediate {

SimpleAssemblyCodeGenPass::SimpleAssemblyCodeGenPass() : ModulePass("simple-assembly-code-gen")
{
}

void SimpleAssemblyCodeGenPass::Run(Context* context)
{
    std::string assemblyFilePath = util::Path::ChangeExtension(context->FilePath(), ".s");
    cmajor::systemx::assembler::AssemblyFile assemblyFile(assemblyFilePath);
    SimpleAssemblyCodeGenerator codeGenerator(context, &assemblyFile);
    context->GetData().VisitGlobalVariables(codeGenerator);
    context->GetCode().VisitFunctions(codeGenerator);
    codeGenerator.GenerateDebugInfo();
    codeGenerator.WriteOutputFile();
}

} // cmajor::systemx::intermediate
