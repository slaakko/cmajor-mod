// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

module cmajor.masm.rt.math;

double RtmPow(double x, int32_t exponent)
{
    return std::pow(x, exponent);
}

double Pow(double x, double y)
{
    return std::pow(x, y);
}

double RtmSqrt(double x)
{
    return std::sqrt(x);
}

double RtmPi()
{
    return M_PI;
}

double RtmCos(double angleRad)
{
    return std::cos(angleRad);
}

double RtmSin(double angleRad)
{
    return std::sin(angleRad);
}

double RtmArcCos(double x)
{
    return std::acos(x);
}

double RtmArcSin(double x)
{
    return std::asin(x);
}

double RtmAtan2(double y, double x)
{
    return std::atan2(y, x);
}

double RtmEpsilon()
{
    return std::numeric_limits<double>::epsilon();
}
