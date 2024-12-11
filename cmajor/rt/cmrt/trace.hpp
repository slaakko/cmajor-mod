// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_TRACE_INCLUDED
#define CMRT_TRACE_INCLUDED
#include <stdint.h>
#include <vector>
#include <string>

namespace cmajor::rt {

struct FunctionLine
{
    FunctionLine(int64_t fn_, int32_t line_) : fn(fn_), line(line_) {}
    int64_t fn;
    int32_t line;
};

class StackTrace
{
public:
    StackTrace();
    bool IsEmpty() const { return fnLines.empty(); }
    void AddFunctionLine(const FunctionLine& fnLine);
    std::string ToString();
private:
    std::vector<FunctionLine> fnLines;
};

StackTrace GetStackTrace();

} // cmajor::rt

extern "C" void RtmPushFunction(void* traceInfo);
extern "C" void RtmPopFunction(void* traceInfo);

#endif // CMRT_TRACE_INCLUDED
