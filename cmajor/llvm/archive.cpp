// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.llvm.archive;

import cmajor.llvm.archive.windows;

namespace cmajor::llvm {

void Archive(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths)
{
#ifdef _WIN32
    ArchiveWindows(libraryFilePath, objectFilePaths);
#else
    // todo
#endif
}

} // cmajor::llvm
