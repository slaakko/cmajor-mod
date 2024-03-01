// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/assert.hpp>
#include <rt/io.hpp>
#include <rt/error.hpp>
#include <sstream>

void RtFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber)
{
    std::stringstream s;
    s << "assertion '" << assertion << "' failed in function '" << function << "' at " << sourceFilePath << ":" << lineNumber << "\n";
    std::string str = s.str();
    int32_t errorStringHandle = -1;
    void* stdError = RtOpenStdFile(2, errorStringHandle);
    RtWrite(stdError, reinterpret_cast<const uint8_t*>(str.c_str()), str.length(), errorStringHandle);
    exit(exitCodeAssertionFailed);
}
