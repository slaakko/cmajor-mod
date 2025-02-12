// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.asm_util;

import std.core;

export namespace cmajor::sbin::assembly {

namespace asm_util {}

uint64_t ParseHexNumber(const std::string& s);
double ParseReal(const std::string& s);
uint64_t ParseInteger(const std::string& s);
std::string ParseString(const std::string& s);

} // namespace cmajor::sbin::assembly
