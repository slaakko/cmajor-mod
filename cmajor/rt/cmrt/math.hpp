// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_MATH_INCLUDED
#define CMRT_CPP_MATH_INCLUDED
#include <stdint.h>

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
extern "C" double RtmRemainder(double x, double y);

#endif // CMRT_CPP_MATH_INCLUDED
