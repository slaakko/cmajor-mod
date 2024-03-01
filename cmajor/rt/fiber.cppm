// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.fiber;

import std.core;

export extern "C" RT_API void* RtConvertThreadToFiber(void* param);
export extern "C" RT_API void* RtCreateFiber(uint64_t stackSize, void* startAddress, void* param);
export extern "C" RT_API void RtSwitchToFiber(void* fiber);
export extern "C" RT_API void* RtGetFiberData();
export extern "C" RT_API void RtDeleteFiber(void* fiber);
