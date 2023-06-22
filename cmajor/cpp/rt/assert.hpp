// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef ASSERT_HPP
#define ASSERT_HPP
#include <rt_api.hpp>

extern "C" RT_API void RtFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber);

#endif // ASSERT_HPP
