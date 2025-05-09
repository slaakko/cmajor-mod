// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.random;

import std.core;

export {

extern "C" void RtmInitRand(uint32_t seed);
extern "C" uint32_t RtmRandom();
extern "C" uint64_t RtmRandom64();

}
