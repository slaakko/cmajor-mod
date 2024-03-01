// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.thread;

import std.core;

export extern "C" RT_API int32_t RtGetHardwareConcurrency();
export extern "C" RT_API int32_t RtStartThreadFunction(void* function);
export extern "C" RT_API int32_t RtStartThreadFunctionWithParam(void* function, void* param);
export extern "C" RT_API int32_t RtStartThreadMethod(void* classDelegate);
export extern "C" RT_API int32_t RtStartThreadMethodWithParam(void* classDelegate, void* param);
export extern "C" RT_API bool RtJoinThread(int32_t threadId);
export extern "C" RT_API int32_t RtThisThreadId();

export namespace cmajor::rt {

void InitThread();
void DoneThread();

} // namespace cmajor::rt
