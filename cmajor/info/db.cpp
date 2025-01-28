// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.info.db;

namespace cmajor::info::db {

SourceLoc::SourceLoc() : path(), line(0)
{
}

std::unique_ptr<soul::xml::Element> SourceLoc::ToXml() const
{
    soul::xml::Element* sourceLocElement = soul::xml::MakeElement("sourceLoc");
    sourceLocElement->SetAttribute("path", path);
    sourceLocElement->SetAttribute("line", std::to_string(line));
    return std::unique_ptr<soul::xml::Element>(sourceLocElement);
}

Location::Location() : func(), file(), line(0), scol(0), ecol(0), level(0)
{
}

std::unique_ptr<soul::xml::Element> Location::ToXml() const
{
    soul::xml::Element* locationElement = soul::xml::MakeElement("location");
    locationElement->SetAttribute("func", func);
    locationElement->SetAttribute("addr", addr);
    locationElement->SetAttribute("file", file);
    locationElement->SetAttribute("line", std::to_string(line));
    locationElement->SetAttribute("scol", std::to_string(scol));
    locationElement->SetAttribute("ecol", std::to_string(ecol));
    locationElement->SetAttribute("level", std::to_string(level));
    return std::unique_ptr<soul::xml::Element>(locationElement);
}

bool Location::IsValid() const
{
    return !file.empty() && line != 0;
}

Type::Type() : name(), id()
{
}

std::unique_ptr<soul::xml::Element> Type::ToXml() const
{
    soul::xml::Element* typeElement = soul::xml::MakeElement("type");
    typeElement->SetAttribute("name", name);
    typeElement->SetAttribute("id", id);
    return std::unique_ptr<soul::xml::Element>(typeElement);
}

Result::Result() : staticType(), dynamicType(), initialized(false), value()
{
}

std::unique_ptr<soul::xml::Element> Result::ToXml() const
{
    soul::xml::Element* resultElement = soul::xml::MakeElement("result");
    resultElement->AppendChild(staticType.ToXml().release());
    resultElement->AppendChild(dynamicType.ToXml().release());
    resultElement->SetAttribute("initialized", initialized ? "true" : "false");
    resultElement->SetAttribute("value", value);
    return std::unique_ptr<soul::xml::Element>(resultElement);
}

ChildResult::ChildResult() : expr(), name(), type(), dynType(), value(), count(0)
{
}

std::unique_ptr<soul::xml::Element> ChildResult::ToXml() const
{
    soul::xml::Element* childResultElement = soul::xml::MakeElement("childResult");
    childResultElement->SetAttribute("expr", expr);
    childResultElement->SetAttribute("name", name);
    childResultElement->SetAttribute("type", type);
    childResultElement->SetAttribute("dynType", dynType);
    childResultElement->SetAttribute("value", value);
    childResultElement->SetAttribute("count", std::to_string(count));
    return std::unique_ptr<soul::xml::Element>(childResultElement);
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

EvaluateReply::EvaluateReply() : result(), success(true), error()
{
}

std::unique_ptr<soul::xml::Element> EvaluateReply::ToXml() const
{
    std::unique_ptr<soul::xml::Element> resultElement = result.ToXml();
    resultElement->SetAttribute("success", success ? "true" : "false");
    resultElement->SetAttribute("error", error);
    return resultElement;
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

EvaluateChildReply::EvaluateChildReply() : results(), success(true), error()
{
}

std::unique_ptr<soul::xml::Element> EvaluateChildReply::ToXml() const
{
    soul::xml::Element* resultElement = soul::xml::MakeElement("result");
    for (const auto& childResult : results)
    {
        resultElement->AppendChild(childResult.ToXml().release());
    }
    resultElement->SetAttribute("success", success ? "true" : "false");
    resultElement->SetAttribute("error", error);
    return std::unique_ptr<soul::xml::Element>(resultElement);
}

} // namespace cmajor::info::db

