// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.random;

import util;

void RtmInitRand(uint32_t seed)
{
    util::set_rand_seed(seed);
}

uint32_t RtmRandom()
{
    return util::Random();
}

uint64_t RtmRandom64()
{
    return util::Random64();
}
