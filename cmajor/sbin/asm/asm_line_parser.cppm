// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.asm_line_parser;

import soul.lexer;
import cmajor.sbin.assembly.asm_file;
import std.core;

export namespace cmajor::sbin::assembly {

std::unique_ptr<Node> ParseAsmLine(const std::string& asmLine);

} // namespace cmajor::sbin::assembly

