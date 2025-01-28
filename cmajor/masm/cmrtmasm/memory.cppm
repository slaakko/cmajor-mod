// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.memory;

import std.core;

export {

extern "C" void* RtmMemAlloc(int64_t size);
extern "C" void RtmMemFree(void* ptr);

} 
