// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.statics;

import std.core;
import util.uuid;

export extern "C" RT_API void RtBeginStaticInitCriticalSection(void* staticClassId);
export extern "C" RT_API void RtEndStaticInitCriticalSection(void* staticClassId);
export extern "C" RT_API void RtEnqueueDestruction(void* destructor, void* arg);

export namespace cmajor::rt {

void AllocateMutexes(const std::vector<util::uuid>& staticClassIds);

void InitStatics();
void DoneStatics();

}  // namespace cmajor::rt
