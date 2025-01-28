// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_EXECUTE_INCLUDED
#define CMRT_EXECUTE_INCLUDED
#include <stdint.h>

extern "C" int32_t RtmExecute(const char* command);
extern "C" int32_t RtmBeginExec(const char* command);
extern "C" void RtmAddRedirection(int32_t execHandle, int handle, const char* file);
extern "C" int32_t RtmEndExec(int32_t execHandle);

#endif // CMRT_EXECUTE_INCLUDED
