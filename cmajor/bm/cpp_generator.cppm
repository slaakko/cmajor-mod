// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binary.message.cpp.generator;

import std.core;
import soul.ast.source.pos;

export namespace cmajor::binary::message::cpp {

void GenerateBinaryMessageFiles(const std::string& schemaFileName, bool verbose);

} // namespace cmajor::binary::message
