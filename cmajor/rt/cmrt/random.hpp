// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_RANDOM_INCLUDED
#define CMRT_CPP_RANDOM_INCLUDED
#include <stdint.h>

extern "C" void RtmInitRand(uint32_t seed);
extern "C" uint32_t RtmRandom();
extern "C" uint64_t RtmRandom64();

#endif // CMRT_CPP_RANDOM_INCLUDED
