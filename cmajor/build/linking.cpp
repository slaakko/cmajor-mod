// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.linking;

import cmajor.llvm;
import cmajor.systemx.object;
import util;

namespace cmajor::build {

void LinkSystemX(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        {
            bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
            std::string clsIdFileName = util::Path::ChangeExtension(project->LibraryFilePath(), ".clsid");
            cmajor::symbols::MakeClassIdFile(rootModule->GetSymbolTable().PolymorphicClasses(), clsIdFileName);
            std::filesystem::path bdp = project->ExecutableFilePath();
            bdp.remove_filename();
            std::filesystem::create_directories(bdp);
            std::vector<std::string> binaryFiles;
            std::string systemMainObjectFilePath = util::GetFullPath(util::Path::Combine(util::CmajorRoot(), "system-x/system/Main/Main.o"));
            binaryFiles.push_back(systemMainObjectFilePath);
            for (const auto& libraryFile : rootModule->LibraryFilePaths())
            {
                binaryFiles.push_back(libraryFile);
            }
            cmajor::systemx::object::Link(rootModule->LogStreamId(), project->ExecutableFilePath(), binaryFiles, clsIdFileName, verbose);
            break;
        }
    }
}

void Link(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (cmajor::symbols::GetBackEnd())
    {
        case cmajor::symbols::BackEnd::llvm:
        {
            cmajor::llvm::Link(project, rootModule);
            break;
        }
        case cmajor::symbols::BackEnd::systemx:
        {
            LinkSystemX(project, rootModule);
            break;
        }
    }
}

} // namespace cmajor::build
