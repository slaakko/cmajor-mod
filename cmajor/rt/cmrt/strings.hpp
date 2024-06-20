// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_STRINGS_INCLUDED
#define CMRT_STRINGS_INCLUDED

extern "C" int RtmAllocateString(const char* s);
extern "C" void RtmFreeString(int stringHandle);
extern "C" const char* RtmGetString(int stringHandle);

#endif // CMRT_STRINGS_INCLUDED
