// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.asm_file_parser;

import soul.lexer;
import cmajor.sbin.assembly.asm_file;
import std.core;

export namespace cmajor::sbin::assembly {

std::unique_ptr<AsmFileNode> ParseAsmFile(const std::string& asmFilePath, bool verbose, soul::lexer::FileMap& fileMap);

} // namespace cmajor::sbin::assembly

