// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_TIME_INCLUDED
#define CMRT_TIME_INCLUDED
#include <stdint.h>

extern "C" int64_t RtmNow();
extern "C" void RtmSleep(int64_t nanoSeconds);
extern "C" bool RtmGetCurrentDate(int16_t * year, int8_t * month, int8_t * day);
extern "C" bool RtmGetCurrentDateTime(int16_t * year, int8_t * month, int8_t * day, int32_t * secs);

#endif // CMRT_TIME_INCLUDED
