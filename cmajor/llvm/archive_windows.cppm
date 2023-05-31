// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.archive.windows;

import std.core;

export namespace cmajor::llvm {

void ArchiveCoff(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths);

} // cmajor::llvm
