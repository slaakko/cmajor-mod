// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.ascii;

import std.core;

export extern "C" RT_API char RtAsciiToUpper(char c);
export extern "C" RT_API char RtAsciiToLower(char c);
