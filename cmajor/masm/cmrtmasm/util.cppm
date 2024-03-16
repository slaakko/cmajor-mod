// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.util;

import std.core;

export {

extern "C" void RtmPrintInteger(int64_t x);
extern "C" void RtmPrintFloat(float x);
extern "C" void RtmPrintDouble(double x);

}
