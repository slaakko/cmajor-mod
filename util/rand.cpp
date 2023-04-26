// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.rand;

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

#ifdef _WIN32

    __declspec(thread) uint64_t init_seed = -1;
    __declspec(thread) Rng* rng = nullptr;

#else

    __thread uint64_t init_seed = -1;
    __thread Rng* rng = nullptr;

#endif

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

} // namespace util
