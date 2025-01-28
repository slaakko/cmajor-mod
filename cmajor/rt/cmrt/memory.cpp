// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/memory.hpp>
#include <stdlib.h>

void* RtmMemAlloc(int64_t size)
{
    return malloc(size);
}

void RtmMemFree(void* ptr)
{
    free(ptr);
}
