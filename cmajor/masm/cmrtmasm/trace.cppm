// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.trace;

import std.core;

export namespace cmajor::masm::rt {

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

} // cmajor::masm::rt

export {

extern "C" void RtmPushFunction(void* traceInfo);
extern "C" void RtmPopFunction(void* traceInfo);

}
