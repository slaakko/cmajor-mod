// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.optimize;

import std.core;
import cmajor.masm.intermediate;

export namespace cmajor::masm::optimizer {

enum class Optimizations : uint64_t
{
    none = 0,
    xor_ = 1 << 0,
    jump = 1 << 1,
    removeNops = 1 << 2,
    jumpTableSwitch = 1 << 3,
    arithmetic = 1 << 4,
    identity = 1 << 5,
    deadCodeElimination = 1 << 6,
    moveLocalsToEntryBlock = 1 << 7,
    inln = 1 << 8,
    o0 = none,
    o1 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock,
    o2 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock | inln,
    o3 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock | inln
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

inline void SetOptimizations(Optimizations optimizations)
{
    currentOptimizations = optimizations;
}

inline bool HasOptimization(Optimizations optimization)
{
    return (currentOptimizations & optimization) != Optimizations::none;
}

void Optimize(cmajor::masm::intermediate::Context* intermediateContext);

} // cmajor::masm::optimizer
