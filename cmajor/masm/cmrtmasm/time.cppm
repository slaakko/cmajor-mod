// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.time;

import std.core;

export {

extern "C" int64_t RtmNow();
extern "C" void RtmSleep(int64_t nanoSeconds);
extern "C" void RtmGetCurrentDate(int16_t* year, int8_t* month, int8_t* day);
extern "C" void RtmGetCurrentDateTime(int16_t* year, int8_t* month, int8_t* day, int32_t* secs);

}
