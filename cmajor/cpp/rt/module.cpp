// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/module.hpp>

namespace cmajor::rt {

void* rtModuleHandle = nullptr;

void* GetRTModuleHandle()
{
    return rtModuleHandle;
}

void SetRTModuleHandle(void* rtModuleHandle_)
{
    rtModuleHandle = rtModuleHandle_;
}

}  // namespace cmajor::rt
