// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.location;

import util;

namespace cmajor::masm::intermediate {

Frame::Frame() : localSize(32)
{
}

FrameLocation Frame::GetParamLocation(int64_t size)
{
    if (paramLocations.empty())
    {
        FrameLocation paramLocation(masm::assembly::RegisterGroupKind::rsp, 0, 8, 0);
        paramLocations.push_back(paramLocation);
        return paramLocation;
    }
    else
    {
        const FrameLocation& last = paramLocations.back();
        FrameLocation paramLocation(masm::assembly::RegisterGroupKind::rsp, frameLocations.size(), last.offset + last.size, size);
        paramLocations.push_back(paramLocation);
        return paramLocation;
    }
}

FrameLocation Frame::GetFrameLocation(int64_t size)
{
    if (frameLocations.empty())
    {
        FrameLocation frameLocation(masm::assembly::RegisterGroupKind::rbp, 0, 0, size);
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
        return 32;
    }
    else
    {
        const FrameLocation& last = frameLocations.back();
        return util::Align(32 + last.offset + last.size, 16);
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
