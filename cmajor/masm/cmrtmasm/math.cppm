// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.math;

import std.core;

export {

extern "C" double RtmPow(double x, int32_t exponent);
extern "C" double Pow(double x, double y);
extern "C" double RtmSqrt(double x);
extern "C" double RtmPi();
extern "C" double RtmCos(double angleRad);
extern "C" double RtmSin(double angleRad);
extern "C" double RtmArcCos(double x);
extern "C" double RtmArcSin(double x);
extern "C" double RtmAtan2(double y, double x);
extern "C" double RtmEpsilon();

}