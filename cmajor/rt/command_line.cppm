// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.command.line;

import std.core;

export extern "C" RT_API int RtArgc();
export extern "C" RT_API const char** RtArgv();
