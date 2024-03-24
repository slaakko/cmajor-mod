// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.util;

import cmajor.masm.rt.io;

void RtmPrintInteger(int64_t x)
{
    std::string s = std::to_string(x);
    RtmPrintString(s.c_str());
}

void RtmPrintFloat(float x)
{
    std::string s = std::to_string(x);
    RtmPrintString(s.c_str());
}

void RtmPrintDouble(double x)
{
    std::string s = std::to_string(x);
    RtmPrintString(s.c_str());
}

void RtmPrintString(const char* s)
{
    int32_t errorId = 0;
    std::string str(s);
    str.append(1, '\n');
    void* stdOut = RtmOpenStdFile(1, errorId);
    RtmPrintToFile(stdOut, str.c_str());
}
