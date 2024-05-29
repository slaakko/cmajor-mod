// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_THREAD_INCLUDED
#define CMRT_CPP_THREAD_INCLUDED
#include <stdint.h>

namespace cmajor::rt {

void DoneThread();

} // namespace cmajor::rt

extern "C" int32_t RtmGetHardwareConcurrency();
extern "C" int32_t RtmStartThreadFunction(void* function);
extern "C" int32_t RtmStartThreadFunctionWithParam(void* function, void* param);
extern "C" int32_t RtmStartThreadMethod(void* classDelegate);
extern "C" int32_t RtmStartThreadMethodWithParam(void* classDelegate, void* param);
extern "C" bool RtmJoinThread(int32_t threadId);
extern "C" int32_t RtmThisThreadId();

#endif // CMRT_CPP_THREAD_INCLUDED
