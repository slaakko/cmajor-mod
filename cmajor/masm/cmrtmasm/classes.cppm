// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.classes;

import std.core;

export namespace cmajor::masm::rt {

void RunDestructors();

} // cmajor::masm::rt;

export {

extern "C" const char* RtmClassName(void* typeId);
extern "C" bool RtmIs(void* typeId, void* baseTypeId);
extern "C" void RtmBeginStaticInitCriticalSection();
extern "C" void RtmEndtaticInitCriticalSection();
extern "C" void RtmEnqueueDestruction(void* destructor, void* destructorThisArg);

}
