// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_UTIL_INCLUDED
#define CMRT_UTIL_INCLUDED
#include <stdint.h>

extern "C" void RtmPrintInteger(int64_t x);
extern "C" void RtmPrintFloat(float x);
extern "C" void RtmPrintDouble(double x);
extern "C" void RtmPrintString(const char* s);

#endif // CMRT_UTIL_INCLUDED
