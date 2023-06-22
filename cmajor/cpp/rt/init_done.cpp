// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/init_done.hpp>
#include <rt/classes.hpp>
#include <rt/io.hpp>
#include <rt/directory.hpp>
#include <rt/thread.hpp>
#include <rt/memory.hpp>
#include <rt/socket.hpp>
#include <rt/environment.hpp>
#include <rt/statics.hpp>
#include <rt/unwind.hpp>
#include <util/init_done.hpp>
#include <mutex>

void RtInit(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray)
{
    cmajor::rt::Init(numberOfPolymorphicClassIds, polymorphicClassIdArray, numberOfStaticClassIds, staticClassIdArray);
}

bool DynamicInitVmtsAndCompare(void* vmt1, void* vmt2)
{
    return cmajor::rt::DynamicInitVmtsAndCompare(vmt1, vmt2);
}

void RtDone()
{
    cmajor::rt::Done();
}

void RtExit(int32_t exitCode)
{
    exit(exitCode);
}

std::recursive_mutex initMutex;

namespace cmajor::rt {

void Init(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray)
{
    util::Init();
    InitIo();
    InitDirectory();
    InitMemory();
    InitThread();
    InitSocket();
    InitEnvironment();
    InitStatics();
    InitClasses(numberOfPolymorphicClassIds, polymorphicClassIdArray, numberOfStaticClassIds, staticClassIdArray);
    //InitCmdbSession(); TODO
    // StartCmdbSession(); TODO
}

void Done()
{
    // DoneCmdbSession(); TODO
    DoneStatics();
    DoneEnvironment();
    DoneSocket();
    DoneThread();
    DoneMemory();
    DoneDirectory();
    DoneIo();
    util::Done();
}

} // namespace cmajor::rt
