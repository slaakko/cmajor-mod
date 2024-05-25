// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util/rand.hpp>
#include <random>
#include <chrono>

namespace util {

class Rng
{
public:
    Rng(uint64_t seed_);
    uint8_t Get() { return dist(mt); }
private:
    std::random_device rd;
    uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<> dist;
};

uint64_t GetSeed(uint64_t seed, std::random_device& rd)
{
    if (seed == -1)
    {
        return static_cast<uint64_t>(rd()) ^ static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }
    else
    {
        return seed;
    }
}

Rng::Rng(uint64_t seed_) : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, 255)
{
}

class Rng32
{
public:
    Rng32(uint64_t seed_);
    uint32_t Get() { return dist(mt); }
    uint64_t Seed() const { return seed; }
private:
    std::random_device rd;
    uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<> dist;
};

Rng32::Rng32(uint64_t seed_) : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, std::numeric_limits<int32_t>::max())
{
}

class Rng64
{
public:
    Rng64(uint64_t seed_);
    uint64_t Get() { return dist(mt); }
    uint64_t Seed() const { return seed; }
private:
    std::random_device rd;
    uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<> dist;
};

Rng64::Rng64(uint64_t seed_) : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, std::numeric_limits<int>::max())
{
}

thread_local uint64_t init_seed = -1;
thread_local Rng* rng = nullptr;
thread_local Rng32* rng32 = nullptr;
thread_local Rng64* rng64 = nullptr;

void set_rand_seed(uint64_t seed)
{
    init_seed = seed;
}

void reset_rng()
{
    if (rng)
    {
        delete rng;
        rng = nullptr;
    }
}

uint8_t get_random_byte()
{
    if (!rng)
    {
        rng = new Rng(init_seed);
    }
    return rng->Get();
}

uint32_t Random()
{
    if (!rng32)
    {
        rng32 = new Rng32(init_seed);
    }
    return rng32->Get();
}

uint64_t Random64()
{
    if (!rng64)
    {
        rng64 = new Rng64(init_seed);
    }
    return rng64->Get();
}

} // namespace util
