// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_FIBER_HPP
#define UTIL_FIBER_HPP

namespace util {

void* ConvertThreadToFiber(void* param);
void* CreateFiber(uint64_t stackSize, void* startAddress, void* param);
void SwitchToFiber(void* fiber);
void* GetFiberData();
void DeleteFiber(void* fiber);

} // namespace util

#endif // UTIL_FIBER_HPP
