// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.archive.windows;

import std.core;

export namespace cmajor::llvm {

void ArchiveWindows(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths);

} // cmajor::llvm
