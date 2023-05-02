module;
#include <boost/multiprecision/integer.hpp>
export module util.prime;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <soulng/util/UtilApi.hpp>
//#include <boost/multiprecision/cpp_int.hpp>

export namespace util {

		// Returns smallest prime greater than x

		void NextPrime(boost::multiprecision::uint128_t& x);

} // namespace util
