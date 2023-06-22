// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <rt_api.hpp>
#include <stdint.h>

namespace cmajor::rt {

void StartCmdbSession();
bool IsCmdbSessionOpen();
void WriteBytesToCmdbSession(int fileHandle, const uint8_t* buffer, int64_t count);
int64_t ReadBytesFromCmdbSession(uint8_t* buffer, int64_t bufferSize);
void InitCmdbSession();
void DoneCmdbSession();

} // namespace cmajor::rt

#endif // DEBUG_HPP
