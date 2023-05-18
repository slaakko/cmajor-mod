// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_ASSERT_INCLUDED
#define UTIL_ASSERT_INCLUDED
#include <stdexcept>

namespace util {

#ifdef NDEBUG

#define Assert(expression, message) ((void)0)

#else

#define Assert(expression, message) if (!(expression)) throw std::runtime_error(std::string("assertion failed: ") + message)

#endif

}

#endif // UTIL_ASSERT_INCLUDED
