// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.build.msbuild;

import std.core;

export namespace cmajor::masm::build {

void MSBuild(const std::string& vsProjectFilePath, const std::string& config, int logStreamId);

} // namespace otava::build
