// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.request;

namespace cmajor::debugger {

Request::Request(RequestKind kind_) : kind(kind_)
{
}

Request::~Request()
{
}

bool Request::IsExecRequest() const
{
    switch (kind)
    {
        case RequestKind::run:
        case RequestKind::cont:
        case RequestKind::next:
        case RequestKind::step:
        {
            return true;
        }
    }
    return false;
}

RunRequest::RunRequest() : Request(RequestKind::run)
{
}

std::string RunRequest::ToString() const
{
    return "-exec-run";
}

ContinueRequest::ContinueRequest() : Request(RequestKind::cont)
{
}

std::string ContinueRequest::ToString() const
{
    return "-exec-continue";
}

NextRequest::NextRequest() : Request(RequestKind::next)
{
}

std::string NextRequest::ToString() const
{
    return "-exec-next";
}

StepRequest::StepRequest() : Request(RequestKind::step)
{
}

std::string StepRequest::ToString() const
{
    return "-exec-step";
}

ExitRequest::ExitRequest() : Request(RequestKind::exit)
{
}

std::string ExitRequest::ToString() const
{
    return "-gdb-exit";
}

BreakInsertRequest::BreakInsertRequest(const std::string& location_) : Request(RequestKind::breakInsert), location(location_)
{
}

std::string BreakInsertRequest::ToString() const
{
    return "-break-insert " + location;
}

DepthRequest::DepthRequest() : Request(RequestKind::depth)
{
}

std::string DepthRequest::ToString() const
{
    return "-stack-info-depth";
}

FramesRequest::FramesRequest(int lowFrame_, int highFrame_) : Request(RequestKind::frames), lowFrame(lowFrame_), highFrame(highFrame_)
{
}

std::string FramesRequest::ToString() const
{
    return "-stack-list-frames " + std::to_string(lowFrame) + " " + std::to_string(highFrame);
}

} // namespace cmajor::debugger
