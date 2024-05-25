// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_RAND_HPP
#define UTIL_RAND_HPP
#include <stdint.h>

namespace util {

void set_rand_seed(uint64_t seed);
void reset_rng();
uint8_t get_random_byte();
uint32_t Random();
uint64_t Random64();

} // namespace util

#endif // UTIL_RAND_HPP
