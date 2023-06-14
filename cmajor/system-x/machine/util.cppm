// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.util;

import std.core;

export namespace cmajor::systemx::machine {

template<class T>
inline uint64_t SignExtend(T value)
{
    int64_t shift = (64 - 8 * sizeof(T));
    return static_cast<uint64_t>((static_cast<int64_t>(value) << shift) >> shift);
}

inline uint64_t BitCount(uint64_t x)
{
    uint64_t c = 0;
    uint64_t m = 1;
    for (int i = 0; i < 64; ++i)
    {
        if ((x & m) != 0)
        {
            ++c;
        }
        m = m << 1;
    }
    return c;
}

inline uint8_t SaturatingSubtraction(uint8_t x, uint8_t y)
{
    if (x >= y) return x - y; else return 0;
}

inline uint16_t SaturatingSubtraction(uint16_t x, uint16_t y)
{
    if (x >= y) return x - y; else return 0;
}

inline uint32_t SaturatingSubtraction(uint32_t x, uint32_t y)
{
    if (x >= y) return x - y; else return 0;
}

inline uint64_t SaturatingSubtraction(uint64_t x, uint64_t y)
{
    if (x >= y) return x - y; else return 0;
}

} // namespace cmajor::systemx::machine
