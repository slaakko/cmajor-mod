// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.crc32;

import std.core;

export namespace util {

uint32_t Crc32(const std::vector<uint8_t>& data);

} // namespace util
