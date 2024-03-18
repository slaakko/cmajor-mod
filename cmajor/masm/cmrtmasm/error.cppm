// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.error;

import std.core;

export {

extern "C" void RtmPanic(const char* message);
extern "C" void RtmFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber);

}
