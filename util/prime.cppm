// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#pragma warning(disable : 4996)
#include <boost/multiprecision/integer.hpp>
#pragma warning(default : 4996)

export module util.prime;

export namespace util {

// Returns smallest prime greater than x

void NextPrime(boost::multiprecision::uint128_t& x);

} // namespace util
