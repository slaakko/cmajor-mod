// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/math.hpp>
#include <cmath>
#include <boost/math/constants/constants.hpp>

double RtPow(double x, int32_t exponent)
{
    return std::pow(x, exponent);
}

double Pow(double x, double y)
{
    return std::pow(x, y);
}

double RtSqrt(double x)
{
    return std::sqrt(x);
}

double RtPi()
{
    const double pi = boost::math::constants::pi<double>();
    return pi;
}

double RtCos(double angleRad)
{
    return std::cos(angleRad);
}

double RtSin(double angleRad)
{
    return std::sin(angleRad);
}

double RtArcCos(double x)
{
    return std::acos(x);
}

double RtArcSin(double x)
{
    return std::asin(x);
}

double RtAtan2(double y, double x)
{
    return std::atan2(y, x);
}

double RtEpsilon()
{
    return std::numeric_limits<double>::epsilon();
}
