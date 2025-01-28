// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.fiber;

import std.core;

export {

extern "C" void* RtmConvertThreadToFiber(void* param);
extern "C" void* RtmCreateFiber(uint64_t stackSize, void* startAddress, void* param);
extern "C" void RtmSwitchToFiber(void* fiber);
extern "C" void* RtmGetFiberData();
extern "C" void RtmDeleteFiber(void* fiber);

}