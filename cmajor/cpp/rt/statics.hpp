// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef STATICS_HPP
#define STATICS_HPP
#include <rt_api.hpp>
#include <vector>
#include <util/uuid.hpp>

extern "C" RT_API void RtBeginStaticInitCriticalSection(void* staticClassId);
extern "C" RT_API void RtEndStaticInitCriticalSection(void* staticClassId);
extern "C" RT_API void RtEnqueueDestruction(void* destructor, void* arg);

namespace cmajor::rt {

void AllocateMutexes(const std::vector<util::uuid>& staticClassIds);

void InitStatics();
void DoneStatics();

}  // namespace cmajor::rt

#endif // STATICS_HPP
