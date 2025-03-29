// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.optimizer.optimize;

import cmajor.systemx.intermediate;
import std.core;

export namespace cmajor::systemx::optimizer {

enum class Optimizations : uint64_t
{
    none = 0,
    paramRegs = 1 << 0,
    o0 = paramRegs,
    o1 = paramRegs,
    o2 = paramRegs,
    o3 = paramRegs
};

constexpr Optimizations operator|(Optimizations left, Optimizations right)
{
    return Optimizations(uint64_t(left) | uint64_t(right));
}

constexpr Optimizations operator&(Optimizations left, Optimizations right)
{
    return Optimizations(uint64_t(left) & uint64_t(right));
}

constexpr Optimizations operator~(Optimizations o)
{
    return Optimizations(~uint64_t(o));
}

extern Optimizations currentOptimizations;

inline Optimizations CurrentOptimizations()
{
    return currentOptimizations;
}

inline void SetOptimizations(int optLevel)
{
    switch (optLevel)
    {
        case 0:
        {
            currentOptimizations = Optimizations::o0;
            break;
        }
        case 1:
        {
            currentOptimizations = Optimizations::o1;
            break;
        }
        case 2:
        {
            currentOptimizations = Optimizations::o2;
            break;
        }
        case 3:
        {
            currentOptimizations = Optimizations::o3;
            break;
        }
    }
}

inline bool HasOptimization(Optimizations optimization)
{
    return (currentOptimizations & optimization) != Optimizations::none;
}

void Optimize(cmajor::systemx::intermediate::Context* intermediateContext);

} // namespace cmajor::systemx::optimizer
