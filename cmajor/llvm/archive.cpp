// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.llvm.archive;

import cmajor.llvm.archive.windows;

namespace cmajor::llvm {

void Archive(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths)
{
#ifdef _WIN32
    ArchiveCoff(libraryFilePath, objectFilePaths);
#else
    // todo
#endif
}

} // cmajor::llvm
