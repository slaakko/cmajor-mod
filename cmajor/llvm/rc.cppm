// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.rc;

import std.core;

export namespace cmajor::llvm {

void ResourceCompile(const std::string& rcFilePath, const std::string& resourceFilePath);

} // namespace cmajor::llvm
