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

} // namespace cmajor::debugger
