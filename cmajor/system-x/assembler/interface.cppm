// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.interface;

import std.core;

export namespace cmajor::systemx::assembler {

void Assemble(int logStreamId, const std::string& assemblyFilePath, const std::string& objectFilePath, bool verbose);

} // namespace cmajor::systemx::assembler
