// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef RANDOM_HPP
#define RANDOM_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API uint32_t RtGetRandomSeed();
extern "C" RT_API void RtInitRand(uint32_t seed);
extern "C" RT_API uint32_t RtRandom();
extern "C" RT_API uint64_t RtRandom64();

#endif // RANDOM_HPP
