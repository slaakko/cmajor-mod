// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/env.hpp>
#include <cmrt/strings.hpp>
#include <cstdlib>

int RtmGetEnvironmentVariable(const char* envVarName)
{
    char* value = nullptr;
    size_t numElements = 0;
    errno_t result = _dupenv_s(&value, &numElements, envVarName);
    if (result == 0 && value != nullptr)
    {
        int stringId = RtmAllocateString(value);
        free(value);
        return stringId;
    }
    else
    {
        return -1;
    }
}