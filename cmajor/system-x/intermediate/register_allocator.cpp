// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.reg.allocator;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.machine;
import cmajor.systemx.assembler;
import util;

namespace cmajor::systemx::intermediate {

ArgLocation::ArgLocation(int index_) : index(index_), constant(nullptr)
{
}

void ArgLocation::SetConstant(cmajor::systemx::assembler::HexadecimalConstant* constant_)
{
    constant = constant_;
}

void ArgLocation::SetValue(uint64_t start)
{
    constant->SetValue(start + static_cast<uint64_t>(index + 1) * 8);
}

CallFrame::CallFrame() : saveNumLocals(0)
{
}

void CallFrame::NextArgLocation(cmajor::systemx::assembler::HexadecimalConstant* constant)
{
    ArgLocation* argLocation = new ArgLocation(argLocations.size());
    argLocation->SetConstant(constant);
    argLocations.push_back(std::unique_ptr<ArgLocation>(argLocation));
}

void CallFrame::Resolve(int frameSize)
{
    uint64_t start = frameSize;
    start = start + static_cast<uint64_t>(8) * (static_cast<uint64_t>(3) + saveNumLocals);
    for (auto& argLocation : argLocations)
    {
        argLocation->SetValue(start);
    }
}

Frame::Frame()
{
    ResetCallFrame();
}

FrameLocation Frame::GetFrameLocation(int64_t size)
{
    if (frameLocations.empty())
    {
        FrameLocation frameLocation(0, 8, size);
        frameLocations.push_back(frameLocation);
        return frameLocation;
    }
    else
    {
        const FrameLocation& last = frameLocations.back();
        FrameLocation frameLocation(frameLocations.size(), last.offset + last.size, size);
        frameLocations.push_back(frameLocation);
        return frameLocation;
    }
}

int64_t Frame::Size() const
{
    if (frameLocations.empty())
    {
        return 8;
    }
    else
    {
        const FrameLocation& last = frameLocations.back();
        return last.offset + last.size;
    }
}

void Frame::ResetCallFrame()
{
    currentCallFrame.reset(new CallFrame());
}

void Frame::AddCallFrame()
{
    callFrames.push_back(std::move(currentCallFrame));
    ResetCallFrame();
}

void Frame::ResolveCallFrames()
{
    int frameSize = Size();
    for (auto& callFrame : callFrames)
    {
        callFrame->Resolve(frameSize);
    }
}

RegisterPool::RegisterPool() : localRegisterCount(cmajor::systemx::machine::NumLocalRegs())
{
    for (int i = 0; i < localRegisterCount; ++i)
    {
        localRegisterPool.insert(Register(RegisterKind::local, static_cast<uint8_t>(i)));
    }
    globalRegisterMap[cmajor::systemx::machine::regAX] = Register(RegisterKind::global, cmajor::systemx::machine::regAX);
    globalRegisterMap[cmajor::systemx::machine::regBX] = Register(RegisterKind::global, cmajor::systemx::machine::regBX);
    globalRegisterMap[cmajor::systemx::machine::regCX] = Register(RegisterKind::global, cmajor::systemx::machine::regCX);
    globalRegisterMap[cmajor::systemx::machine::regDX] = Register(RegisterKind::global, cmajor::systemx::machine::regDX);
    globalRegisterMap[cmajor::systemx::machine::regEX] = Register(RegisterKind::global, cmajor::systemx::machine::regEX);
    globalRegisterMap[cmajor::systemx::machine::regIX] = Register(RegisterKind::global, cmajor::systemx::machine::regIX);
    globalRegisterMap[cmajor::systemx::machine::regFP] = Register(RegisterKind::global, cmajor::systemx::machine::regFP);
    globalRegisterMap[cmajor::systemx::machine::regSP] = Register(RegisterKind::global, cmajor::systemx::machine::regSP);
}

Register RegisterPool::GetGlobalRegister(uint8_t number)
{
    auto it = globalRegisterMap.find(number);
    if (it != globalRegisterMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("invalid global register number " + std::to_string(number));
    }
}

void RegisterPool::AddLocalRegister(const Register& reg)
{
    localRegisterPool.insert(reg);
}

Register RegisterPool::GetLocalRegister()
{
    if (localRegisterPool.empty())
    {
        throw std::runtime_error("register pool is empty");
    }
    Register reg = *localRegisterPool.begin();
    localRegisterPool.erase(reg);
    return reg;
}

RegisterAllocator::~RegisterAllocator()
{
}

Register GetGlobalRegister(Context* context, uint8_t registerNumber)
{
    return context->GetRegisterPool().GetGlobalRegister(registerNumber);
}

Register GetLocalRegister(Context* context)
{
    return context->GetRegisterPool().GetLocalRegister();
}

} // cmajor::systemx::intermediate
