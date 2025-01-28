// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_DEBUG_INCLUDED
#define CMRT_DEBUG_INCLUDED
#include <stdint.h>

namespace cmajor::rt {

void StartCmdbSession();
bool IsCmdbSessionOpen();
void WriteBytesToCmdbSession(int fileHandle, const uint8_t* buffer, int64_t count);
int64_t ReadBytesFromCmdbSession(uint8_t* buffer, int64_t bufferSize);
bool CmdbSessionEof();
void InitCmdbSession();
void DoneCmdbSession();

} // namespace cmajor::rt

#endif // CMRT_DEBUG_INCLUDED
