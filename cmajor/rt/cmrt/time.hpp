// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_TIME_INCLUDED
#define CMRT_CPP_TIME_INCLUDED
#include <stdint.h>

extern "C" int64_t RtmNow();
extern "C" void RtmSleep(int64_t nanoSeconds);
extern "C" void RtmGetCurrentDate(int16_t * year, int8_t * month, int8_t * day);
extern "C" void RtmGetCurrentDateTime(int16_t * year, int8_t * month, int8_t * day, int32_t * secs);

#endif // CMRT_CPP_TIME_INCLUDED