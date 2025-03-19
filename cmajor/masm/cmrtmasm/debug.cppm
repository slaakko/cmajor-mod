// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.debug;

import std.core;

export namespace cmajor::masm::rt {

void StartCmdbSession();
bool IsCmdbSessionOpen();
void WriteBytesToCmdbSession(int fileHandle, const uint8_t* buffer, int64_t count);
int64_t ReadBytesFromCmdbSession(uint8_t* buffer, int64_t bufferSize);
bool CmdbSessionEof();
void InitCmdbSession();
void DoneCmdbSession();
extern "C" void RtmDebugBreak();

} // namespace cmajor::masm::rt
