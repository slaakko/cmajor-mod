// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.strings;

import std.core;

export {

extern "C" int RtmAllocateString(const char* s);
extern "C" void RtmFreeString(int stringHandle);
extern "C" const char* RtmGetString(int stringHandle);

}
