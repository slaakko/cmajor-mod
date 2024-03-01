// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef FIBER_HPP
#define FIBER_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API void* RtConvertThreadToFiber(void* param);
extern "C" RT_API void* RtCreateFiber(uint64_t stackSize, void* startAddress, void* param);
extern "C" RT_API void RtSwitchToFiber(void* fiber);
extern "C" RT_API void* RtGetFiberData();
extern "C" RT_API void RtDeleteFiber(void* fiber);

#endif // FIBER_HPP
