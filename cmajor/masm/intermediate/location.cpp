// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.location;

import util;

namespace cmajor::masm::intermediate {

Frame::Frame() : calleeParamAreaSize(32), numUsedXMMRegs(0), rbxPushed(false)
{
}

FrameLocation Frame::GetParamLocation(int64_t size, cmajor::masm::assembly::Context* assemblyContext)
{
    if (paramLocations.empty())
    {
        FrameLocation paramLocation(masm::assembly::RegisterGroupKind::rsp, 0, 8, 8);
        paramLocations.push_back(paramLocation);
        return paramLocation;
    }
    else if (paramLocations.size() < 4)
    {
        const FrameLocation& last = paramLocations.back();
        FrameLocation paramLocation(masm::assembly::RegisterGroupKind::rsp, paramLocations.size(), last.offset + last.size, 8);
        paramLocations.push_back(paramLocation);
        return paramLocation;
    }
    else
    {
        const FrameLocation& last = paramLocations.back();
        cmajor::masm::assembly::Macro* macro = assemblyContext->MakeMacro("param" + std::to_string(paramLocations.size()) + "$");
        FrameLocation paramLocation(masm::assembly::RegisterGroupKind::rbp, paramLocations.size(), last.offset + last.size, size);
        paramLocation.macro = macro;
        paramLocations.push_back(paramLocation);
        return paramLocation;
    }
}

FrameLocation Frame::GetFrameLocation(int64_t size)
{
    if (frameLocations.empty())
    {
        FrameLocation frameLocation(masm::assembly::RegisterGroupKind::rbp, 0, 8, size);
        frameLocations.push_back(frameLocation);
        return frameLocation;
    }
    else
    {
        const FrameLocation& last = frameLocations.back();
        FrameLocation frameLocation(masm::assembly::RegisterGroupKind::rbp, frameLocations.size(), last.offset + last.size, size);
        frameLocations.push_back(frameLocation);
        return frameLocation;
    }
}

int64_t Frame::Size() const
{
    if (frameLocations.empty())
    {
        return 32 + numUsedXMMRegs * 16;
    }
    else
    {
        const FrameLocation& last = frameLocations.back();
        return util::Align(32 + last.offset + last.size + numUsedXMMRegs * 16, 16);
    }
}

void Frame::SetMacroValues(cmajor::masm::assembly::Context* assemblyContext)
{
    int retVal = 1;
    int pushRbp = 1;
    int numUsedLocalRegs = assemblyContext->GetRegisterPool()->UsedLocalRegs().size();
    int numUsedNonvolatileRegs = assemblyContext->GetRegisterPool()->UsedNonvolatileRegs().size();

    int64_t paramOffset = 8 * (retVal + pushRbp + numUsedLocalRegs + numUsedNonvolatileRegs);
    if (rbxPushed)
    {
        paramOffset += 8;
    }
    int64_t frameSize = Size();

    for (auto& paramLocation : paramLocations)
    {
        cmajor::masm::assembly::Macro* macro = paramLocation.macro;
        if (macro)
        {
            int64_t value = (paramLocation.index - 4) * 8 + paramOffset + frameSize;
            value -= XMMSaveRegSize();
            value -= calleeParamAreaSize - 4 * 8;
            macro->SetValue(assemblyContext->MakeIntegerLiteral(value, 8));
        }
    }
}

CallFrame::CallFrame()
{
}

void CallFrame::AllocateArgLocation(int64_t size)
{
    if (argLocations.empty())
    {
        ArgLocation argLocation(0, 0, 8);
        argLocations.push_back(argLocation);
    }
    else
    {
        const ArgLocation& last = argLocations.back();
        ArgLocation argLocation(argLocations.size(), last.offset + last.size, 8);
        argLocations.push_back(argLocation);
    }
}

ArgLocation CallFrame::GetArgLocation(int index)
{
    return argLocations[index];
}

} // cmajor::masm::intermediate
