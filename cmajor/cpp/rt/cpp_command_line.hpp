// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CPP_RT_COMMAND_LINE
#define CPP_RT_COMMAND_LINE

#include <rt_api.hpp>

extern "C" RT_API int RtArgc();
extern "C" RT_API const char** RtArgv();

#endif // CPP_RT_COMMAND_LINE
