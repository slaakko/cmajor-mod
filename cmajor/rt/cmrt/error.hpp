// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_ERROR_INCLUDED
#define CMRT_ERROR_INCLUDED
#include <string>

namespace cmajor::rt {

class StackTrace;

int AllocateError(const std::string& errorMessage);

} // cmajor::rt

extern "C" void RtmPanic(const char* message);
extern "C" void RtmFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber);
extern "C" int RtmAllocateError(const char* errorMessage);
extern "C" const char* RtmGetErrorMessage(int errorId);
extern "C" const char* RtmGetErrorMessageWithoutStackTrace(int errorId);
extern "C" void RtmDisposeError(int errorId);
extern "C" int RtmGetErrno();

#endif // CMRT_ERROR_INCLUDED
