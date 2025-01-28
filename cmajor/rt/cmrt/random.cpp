// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/random.hpp>
#include <util/rand.hpp>

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
