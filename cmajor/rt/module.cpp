// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.rt.mod;

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
