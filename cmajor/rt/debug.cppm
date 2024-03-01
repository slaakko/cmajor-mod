// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.rt.debug;

import std.core;

export namespace cmajor::rt {

void StartCmdbSession();
bool IsCmdbSessionOpen();
void WriteBytesToCmdbSession(int fileHandle, const uint8_t* buffer, int64_t count);
int64_t ReadBytesFromCmdbSession(uint8_t* buffer, int64_t bufferSize);
void InitCmdbSession();
void DoneCmdbSession();

} // namespace cmajor::rt
