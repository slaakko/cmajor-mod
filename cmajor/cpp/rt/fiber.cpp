// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/fiber.hpp>
#include <util/fiber.hpp>

void* RtConvertThreadToFiber(void* param)
{
    return util::ConvertThreadToFiber(param);
}

void* RtCreateFiber(uint64_t stackSize, void* startAddress, void* param)
{
    return util::CreateFiber(stackSize, startAddress, param);
}

void RtSwitchToFiber(void* fiber)
{
    util::SwitchToFiber(fiber);
}

void* RtGetFiberData()
{
    return util::GetFiberData();
}

void RtDeleteFiber(void* fiber)
{
    util::DeleteFiber(fiber);
}
