// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.memory;

void* RtmMemAlloc(int64_t size)
{
    return malloc(size);
}

void RtmMemFree(void* ptr)
{
    free(ptr);
}
