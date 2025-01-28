// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.thread;

import std.core;

export namespace cmajor::masm::rt {

void DoneThread();

} // namespace cmajor::masm::rt

export {

extern "C" int32_t RtmGetHardwareConcurrency();
extern "C" int32_t RtmStartThreadFunction(void* function);
extern "C" int32_t RtmStartThreadFunctionWithParam(void* function, void* param);
extern "C" int32_t RtmStartThreadMethod(void* classDelegate);
extern "C" int32_t RtmStartThreadMethodWithParam(void* classDelegate, void* param);
extern "C" bool RtmJoinThread(int32_t threadId);
extern "C" int32_t RtmThisThreadId();
 
}