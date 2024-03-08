// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.build.vs_build;

import cmajor.masm.build.vs_project_file;
import cmajor.masm.build.msbuild;

namespace cmajor::masm::build {

void VSBuild(cmajor::ast::Project* project, cmajor::symbols::Module* module, const std::vector<std::string> asmFilePaths, const std::vector<std::string> cppFilePaths, bool verbose)
{
    std::string vsProjectFilePath = MakeVSProjectFile(project, module, asmFilePaths, cppFilePaths, verbose);
    MSBuild(vsProjectFilePath, cmajor::symbols::GetConfig(), project->LogStreamId());
}
   
} // namespace cmajor::masm::build
