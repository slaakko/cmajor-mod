// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.memory;

import std.core;

export extern "C" RT_API void* RtMemAlloc(int64_t size);
export extern "C" RT_API void* RtMemAllocInfo(int64_t size, const char* info);
export extern "C" RT_API void RtDispose(void* ptr);
export extern "C" RT_API void RtMemFree(void* ptr);
export extern "C" RT_API void RtMemZero(void* ptr, int64_t size);

export namespace cmajor::rt{

void SetDebugHeap();
void SetDebugAllocation(int allocation);

void InitMemory();
void DoneMemory();

}  // namespace cmajor::rt
