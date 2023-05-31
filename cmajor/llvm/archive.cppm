// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.archive;

import std.core;

export namespace cmajor::llvm {

void Archive(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths);

} // cmajor::llvm
