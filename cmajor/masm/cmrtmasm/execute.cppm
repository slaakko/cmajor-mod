// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.execute;

import std.core;

export {

extern "C" int32_t RtmExecute(const char* command);
extern "C" int32_t RtmBeginExec(const char* command);
extern "C" void RtmAddRedirection(int32_t execHandle, int handle, const char* file);
extern "C" int32_t RtmEndExec(int32_t execHandle);

}