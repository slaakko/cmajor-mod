// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxutil.util;

import std.core;

export namespace cmajor::systemx {

std::vector<std::string> ParseArgs(const std::string& commandLine);
std::string SearchBin(const std::string& fileName);

} // namespace cmajor::systemx
