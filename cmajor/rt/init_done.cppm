// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.init.done;

import std.core;

using GlobalInitFunctionType = void(*)();

export extern "C" RT_API void RtInit(
    int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray,
    GlobalInitFunctionType globalInitializationFunction);
export extern "C" RT_API void RtDone();
export extern "C" RT_API bool DynamicInitVmtsAndCompare(void* vmt1, void* vmt2);
export extern "C" RT_API void RtExit(int32_t exitCode);
export extern "C" RT_API void RtBeginUnwindInfoInit();
export extern "C" RT_API void RtEndUnwindInfoInit();

export namespace cmajor::rt {

void Init(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray);
void Done();

} // namespace cmajor::rt