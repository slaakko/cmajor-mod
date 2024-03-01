// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef INIT_DONE_HPP
#define INIT_DONE_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API void RtInit(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray);
extern "C" RT_API void RtDone();
extern "C" RT_API bool DynamicInitVmtsAndCompare(void* vmt1, void* vmt2);
extern "C" RT_API void RtExit(int32_t exitCode);

namespace cmajor::rt {

void Init(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray);
void Done();

} // namespace cmajor::rt

#endif // INIT_DONE_HPP
