// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CLASSES_INCLUDED
#define CMRT_CLASSES_INCLUDED

namespace cmajor::rt {

void RunDestructors();

} // cmajor::rt;

extern "C" const char* RtmClassName(void* typeId);
extern "C" bool RtmIs(void* typeId, void* baseTypeId);
extern "C" void RtmBeginStaticInitCriticalSection();
extern "C" void RtmEndtaticInitCriticalSection();
extern "C" void RtmEnqueueDestruction(void* destructor, void* destructorThisArg);

#endif // CMRT_CLASSES_INCLUDED
