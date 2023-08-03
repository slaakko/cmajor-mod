// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.info.db;

namespace cmajor::info::db {

SourceLoc::SourceLoc() : path(), line(0)
{
}

Location::Location() : func(), file(), line(0), scol(0), ecol(0), level(0)
{
}

bool Location::IsValid() const
{
    return !file.empty() && line != 0;
}

Type::Type() : name(), id()
{
}

Result::Result() : staticType(), dynamicType(), initialized(false), value()
{
}

ChildResult::ChildResult() : expr(), name(), type(), dynType(), value(), count(0)
{
}

DepthReply::DepthReply() : depth(0)
{
}

FramesRequest::FramesRequest() : lowFrame(0), highFrame(0)
{
}

FramesReply::FramesReply() : frames()
{
}

EvaluateRequest::EvaluateRequest() : expression()
{
}

EvaluateReply::EvaluateReply() : result()
{
}

CountRequest::CountRequest() : expression()
{
}

CountReply::CountReply() : count(0)
{
}

EvaluateChildRequest::EvaluateChildRequest() : expression(), start(0), count(0)
{
}

EvaluateChildReply::EvaluateChildReply() : results()
{
}

} // namespace cmajor::info::db
