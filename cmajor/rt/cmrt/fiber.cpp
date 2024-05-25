// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/fiber.hpp>
#include <util/fiber.hpp>

void* RtmConvertThreadToFiber(void* param)
{
    return util::ConvertThreadToFiber(param);
}

void* RtmCreateFiber(uint64_t stackSize, void* startAddress, void* param)
{
    return util::CreateFiber(stackSize, startAddress, param);
}

void RtmSwitchToFiber(void* fiber)
{
    util::SwitchToFiber(fiber);
}

void* RtmGetFiberData()
{
    return util::GetFiberData();
}

void RtmDeleteFiber(void* fiber)
{
    util::DeleteFiber(fiber);
}
