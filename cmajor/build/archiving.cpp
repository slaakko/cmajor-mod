// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.archiving;

import cmajor.symbols;
import cmajor.llvm;
import cmajor.backend.systemx;
import cmajor.systemx.object;
import util;
import std.filesystem;

namespace cmajor::build {

void Archive(cmajor::ast::Project* project, const std::vector<std::string>& objectFilePaths)
{
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    switch (cmajor::symbols::GetBackEnd())
    {
        case cmajor::symbols::BackEnd::llvm:
        {
            util::LogMessage(project->LogStreamId(), "Generating library file...");
            if (std::filesystem::exists(project->LibraryFilePath()))
            {
                std::filesystem::remove(project->LibraryFilePath());
            }
            cmajor::llvm::Archive(project->LibraryFilePath(), objectFilePaths);
            util::LogMessage(project->LogStreamId(), "==> " + project->LibraryFilePath());
            break;
        }
        case cmajor::symbols::BackEnd::systemx:
        {
            cmajor::systemx::object::CreateArchive(0, project->LibraryFilePath(), objectFilePaths, verbose);
            break;
        }
    }
}

} // namespace cmajor::build
