// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.math;

import std.core;

export extern "C" RT_API double RtPow(double x, int32_t exponent);
export extern "C" RT_API double Pow(double x, double y);
export extern "C" RT_API double RtSqrt(double x);
export extern "C" RT_API double RtPi();
export extern "C" RT_API double RtCos(double angleRad);
export extern "C" RT_API double RtSin(double angleRad);
export extern "C" RT_API double RtArcCos(double x);
export extern "C" RT_API double RtArcSin(double x);
export extern "C" RT_API double RtAtan2(double y, double x);
export extern "C" RT_API double RtEpsilon();
