// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/multiprecision/integer.hpp>
export module util.prime;


export namespace util {

// Returns smallest prime greater than x

void NextPrime(boost::multiprecision::uint128_t& x);

} // namespace util
