// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_FIBER_INCLUDED
#define CMRT_CPP_FIBER_INCLUDED
#include <stdint.h>

extern "C" void* RtmConvertThreadToFiber(void* param);
extern "C" void* RtmCreateFiber(uint64_t stackSize, void* startAddress, void* param);
extern "C" void RtmSwitchToFiber(void* fiber);
extern "C" void* RtmGetFiberData();
extern "C" void RtmDeleteFiber(void* fiber);

#endif // CMRT_CPP_FIBER_INCLUDED