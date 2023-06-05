// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.time;

import std.core;

export extern "C" RT_API int64_t RtNow();
export extern "C" RT_API void RtSleep(int64_t nanoSeconds);
export extern "C" RT_API void RtGetCurrentDate(int16_t* year, int8_t* month, int8_t* day);
export extern "C" RT_API void RtGetCurrentDateTime(int16_t* year, int8_t* month, int8_t* day, int32_t* secs);
export extern "C" RT_API int64_t RtGetCurrentTime();
