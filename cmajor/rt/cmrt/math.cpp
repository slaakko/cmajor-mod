// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/math.hpp>
#include <limits>
#include <cmath>
#include <numbers>

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
    return std::numbers::pi;
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

double RtmRemainder(double x, double y)
{
    return std::remainder(x, y);
}
