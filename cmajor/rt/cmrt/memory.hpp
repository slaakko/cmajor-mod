// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_MEMORY_INCLUDED
#define CMRT_CPP_MEMORY_INCLUDED
#include <stdint.h>

extern "C" void* RtmMemAlloc(int64_t size);
extern "C" void RtmMemFree(void* ptr);

#endif // CMRT_CPP_MEMORY_INCLUDED
