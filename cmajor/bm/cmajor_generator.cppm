// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binary.message.cmajor.generator;

import std.core;
import soul.ast.source.pos;

export namespace cmajor::binary::message::cm {

void GenerateBinaryMessageFiles(const std::string& schemaFileName, bool verbose);

} // namespace cmajor::binary::message
