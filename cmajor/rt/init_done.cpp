// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.rt.init.done;

import util;
import cmajor.rt.error;
import cmajor.rt.io;
import cmajor.rt.directory;
import cmajor.rt.call.stack;
import cmajor.rt.classes;
import cmajor.rt.statics;
import cmajor.rt.string;
import cmajor.rt.mutex;
import cmajor.rt.condition.variable;
import cmajor.rt.thread;
import cmajor.rt.memory;
import cmajor.rt.socket;
import cmajor.rt.environment;
import cmajor.rt.unwind;
import cmajor.rt.debug;

GlobalInitFunctionType initCompileUnitsFunction = nullptr;

void RtInit(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray,
    GlobalInitFunctionType globalInitializationFunction)
{
    cmajor::rt::Init(numberOfPolymorphicClassIds, polymorphicClassIdArray, numberOfStaticClassIds, staticClassIdArray);
    initCompileUnitsFunction = globalInitializationFunction;
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

void RtInitCompileUnits()
{
    std::lock_guard<std::recursive_mutex> initLock(initMutex);
    if (initCompileUnitsFunction)
    {
        GlobalInitFunctionType init = initCompileUnitsFunction;
        initCompileUnitsFunction = nullptr;
        init();
    }
}

void RtBeginUnwindInfoInit()
{
    initMutex.lock();
    RtInitCompileUnits();
}

void RtEndUnwindInfoInit()
{
    initMutex.unlock();
}

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
    InitUnwind();
    //InitCmdbSession(); TODO
    // StartCmdbSession(); TODO
}

void Done()
{
    // DoneCmdbSession(); TODO
    DoneUnwind();
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
