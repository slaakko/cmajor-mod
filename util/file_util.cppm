// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file.util;

import std.core;

export namespace util {

void CopyFile(const std::string& source, const std::string& dest, bool force, bool verbose);

} // namespace util
