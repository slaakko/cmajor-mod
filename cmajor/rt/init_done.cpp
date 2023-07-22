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
import cmajor.rt.mod;
import soul.xml.xpath;

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
    try
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
        InitCmdbSession();
        soul::xml::xpath::SetModuleHandle(GetRTModuleHandle());
        StartCmdbSession();
    }
    catch (const std::exception& ex)
    {
        RtPrint("RtInit failed");
        RtPrint(ex.what());
        RtPrint("\n");
    }
}

void Done()
{
    try
    {
        DoneCmdbSession();
        DoneStatics();
        DoneEnvironment();
        DoneSocket();
        DoneThread();
        DoneMemory();
        DoneDirectory();
        DoneIo();
        util::Done();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

} // namespace cmajor::rt
