// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.archiving;

import std.filesystem;
import cmajor.llvm;
import util;

namespace cmajor::build {

void Archive(cmajor::ast::Project* project, const std::vector<std::string>& objectFilePaths)
{
    util::LogMessage(project->LogStreamId(), "Generating library file...");
    if (std::filesystem::exists(project->LibraryFilePath()))
    {
        std::filesystem::remove(project->LibraryFilePath());
    }
    cmajor::llvm::Archive(project->LibraryFilePath(), objectFilePaths);
    util::LogMessage(project->LogStreamId(), "==> " + project->LibraryFilePath());
}

} // namespace cmajor::build
