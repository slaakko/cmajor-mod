// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.random;

import std.core;

export extern "C" RT_API uint32_t RtGetRandomSeed();
export extern "C" RT_API void RtInitRand(uint32_t seed);
export extern "C" RT_API uint32_t RtRandom();
export extern "C" RT_API uint64_t RtRandom64();
