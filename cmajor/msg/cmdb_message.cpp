// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/msg/cmdb_message.schema' by xmlsergen version 4.1.0

module cmajor.msg.cmdb.message;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

Location::Location()
     : func(), file(), line(), scol(), ecol(), level()
{
}

Location::~Location()
{
}

std::string Location::ClassName() const
{
    return "Location";
}

std::string Location::StaticClassName()
{
    return "Location";
}

void* Location::Create()
{
    return new Location();
}

void Location::Register()
{
    soul::xml::serialization::RegisterClass<Location>();
}

void Location::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Location"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string Location::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Location"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void Location::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void Location::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "Location");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* Location::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(func, "func"));
    element->AppendChild(soul::xml::serialization::ToXml(file, "file"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(scol, "scol"));
    element->AppendChild(soul::xml::serialization::ToXml(ecol, "ecol"));
    element->AppendChild(soul::xml::serialization::ToXml(level, "level"));
    return element;
}

void Location::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "func", func);
    soul::xml::serialization::FromXml(element, "file", file);
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "scol", scol);
    soul::xml::serialization::FromXml(element, "ecol", ecol);
    soul::xml::serialization::FromXml(element, "level", level);
}

Type::Type()
     : name(), id()
{
}

Type::~Type()
{
}

std::string Type::ClassName() const
{
    return "Type";
}

std::string Type::StaticClassName()
{
    return "Type";
}

void* Type::Create()
{
    return new Type();
}

void Type::Register()
{
    soul::xml::serialization::RegisterClass<Type>();
}

void Type::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Type"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string Type::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Type"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void Type::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void Type::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "Type");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* Type::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(name, "name"));
    element->AppendChild(soul::xml::serialization::ToXml(id, "id"));
    return element;
}

void Type::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "name", name);
    soul::xml::serialization::FromXml(element, "id", id);
}

Result::Result()
     : staticType(), dynamicType(), initialized(), value()
{
}

Result::~Result()
{
}

std::string Result::ClassName() const
{
    return "Result";
}

std::string Result::StaticClassName()
{
    return "Result";
}

void* Result::Create()
{
    return new Result();
}

void Result::Register()
{
    soul::xml::serialization::RegisterClass<Result>();
}

void Result::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Result"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string Result::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Result"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void Result::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void Result::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "Result");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* Result::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(staticType, "staticType"));
    element->AppendChild(soul::xml::serialization::ToXml(dynamicType, "dynamicType"));
    element->AppendChild(soul::xml::serialization::ToXml(initialized, "initialized"));
    element->AppendChild(soul::xml::serialization::ToXml(value, "value"));
    return element;
}

void Result::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "staticType", staticType);
    soul::xml::serialization::FromXml(element, "dynamicType", dynamicType);
    soul::xml::serialization::FromXml(element, "initialized", initialized);
    soul::xml::serialization::FromXml(element, "value", value);
}

ChildResult::ChildResult()
     : expr(), name(), type(), dynType(), value(), count()
{
}

ChildResult::~ChildResult()
{
}

std::string ChildResult::ClassName() const
{
    return "ChildResult";
}

std::string ChildResult::StaticClassName()
{
    return "ChildResult";
}

void* ChildResult::Create()
{
    return new ChildResult();
}

void ChildResult::Register()
{
    soul::xml::serialization::RegisterClass<ChildResult>();
}

void ChildResult::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ChildResult"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ChildResult::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ChildResult"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ChildResult::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ChildResult::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ChildResult");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ChildResult::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(expr, "expr"));
    element->AppendChild(soul::xml::serialization::ToXml(name, "name"));
    element->AppendChild(soul::xml::serialization::ToXml(type, "type"));
    element->AppendChild(soul::xml::serialization::ToXml(dynType, "dynType"));
    element->AppendChild(soul::xml::serialization::ToXml(value, "value"));
    element->AppendChild(soul::xml::serialization::ToXml(count, "count"));
    return element;
}

void ChildResult::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "expr", expr);
    soul::xml::serialization::FromXml(element, "name", name);
    soul::xml::serialization::FromXml(element, "type", type);
    soul::xml::serialization::FromXml(element, "dynType", dynType);
    soul::xml::serialization::FromXml(element, "value", value);
    soul::xml::serialization::FromXml(element, "count", count);
}

TargetState::TargetState()
     : stopReason(), exitCode(), breakpointId(), signalName(), signalMeaning()
{
}

TargetState::~TargetState()
{
}

std::string TargetState::ClassName() const
{
    return "TargetState";
}

std::string TargetState::StaticClassName()
{
    return "TargetState";
}

void* TargetState::Create()
{
    return new TargetState();
}

void TargetState::Register()
{
    soul::xml::serialization::RegisterClass<TargetState>();
}

void TargetState::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetState"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetState::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetState"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetState::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetState::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetState");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetState::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(stopReason, "stopReason"));
    element->AppendChild(soul::xml::serialization::ToXml(exitCode, "exitCode"));
    element->AppendChild(soul::xml::serialization::ToXml(breakpointId, "breakpointId"));
    element->AppendChild(soul::xml::serialization::ToXml(signalName, "signalName"));
    element->AppendChild(soul::xml::serialization::ToXml(signalMeaning, "signalMeaning"));
    return element;
}

void TargetState::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "stopReason", stopReason);
    soul::xml::serialization::FromXml(element, "exitCode", exitCode);
    soul::xml::serialization::FromXml(element, "breakpointId", breakpointId);
    soul::xml::serialization::FromXml(element, "signalName", signalName);
    soul::xml::serialization::FromXml(element, "signalMeaning", signalMeaning);
}

SourceLoc::SourceLoc()
     : path(), line()
{
}

SourceLoc::~SourceLoc()
{
}

std::string SourceLoc::ClassName() const
{
    return "SourceLoc";
}

std::string SourceLoc::StaticClassName()
{
    return "SourceLoc";
}

void* SourceLoc::Create()
{
    return new SourceLoc();
}

void SourceLoc::Register()
{
    soul::xml::serialization::RegisterClass<SourceLoc>();
}

void SourceLoc::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SourceLoc"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string SourceLoc::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SourceLoc"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void SourceLoc::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void SourceLoc::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "SourceLoc");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* SourceLoc::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(path, "path"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    return element;
}

void SourceLoc::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "path", path);
    soul::xml::serialization::FromXml(element, "line", line);
}

BreakpointInfo::BreakpointInfo()
     : success(), breakpointId(), numInsts(), location(), error()
{
}

BreakpointInfo::~BreakpointInfo()
{
}

std::string BreakpointInfo::ClassName() const
{
    return "BreakpointInfo";
}

std::string BreakpointInfo::StaticClassName()
{
    return "BreakpointInfo";
}

void* BreakpointInfo::Create()
{
    return new BreakpointInfo();
}

void BreakpointInfo::Register()
{
    soul::xml::serialization::RegisterClass<BreakpointInfo>();
}

void BreakpointInfo::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakpointInfo"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BreakpointInfo::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakpointInfo"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BreakpointInfo::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BreakpointInfo::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BreakpointInfo");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BreakpointInfo::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(breakpointId, "breakpointId"));
    element->AppendChild(soul::xml::serialization::ToXml(numInsts, "numInsts"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void BreakpointInfo::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "breakpointId", breakpointId);
    soul::xml::serialization::FromXml(element, "numInsts", numInsts);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "error", error);
}

StartDebugRequest::StartDebugRequest()
     : breakpointLocations()
{
}

StartDebugRequest::~StartDebugRequest()
{
}

std::string StartDebugRequest::ClassName() const
{
    return "StartDebugRequest";
}

std::string StartDebugRequest::StaticClassName()
{
    return "StartDebugRequest";
}

void* StartDebugRequest::Create()
{
    return new StartDebugRequest();
}

void StartDebugRequest::Register()
{
    soul::xml::serialization::RegisterClass<StartDebugRequest>();
}

void StartDebugRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StartDebugRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StartDebugRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StartDebugRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StartDebugRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StartDebugRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StartDebugRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StartDebugRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(breakpointLocations, "breakpointLocations"));
    return element;
}

void StartDebugRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "breakpointLocations", breakpointLocations);
}

StartDebugReply::StartDebugReply()
     : success(), location(), state(), breakpointInfos(), error()
{
}

StartDebugReply::~StartDebugReply()
{
}

std::string StartDebugReply::ClassName() const
{
    return "StartDebugReply";
}

std::string StartDebugReply::StaticClassName()
{
    return "StartDebugReply";
}

void* StartDebugReply::Create()
{
    return new StartDebugReply();
}

void StartDebugReply::Register()
{
    soul::xml::serialization::RegisterClass<StartDebugReply>();
}

void StartDebugReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StartDebugReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StartDebugReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StartDebugReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StartDebugReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StartDebugReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StartDebugReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StartDebugReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(breakpointInfos, "breakpointInfos"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void StartDebugReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "breakpointInfos", breakpointInfos);
    soul::xml::serialization::FromXml(element, "error", error);
}

StopDebugRequest::StopDebugRequest()
{
}

StopDebugRequest::~StopDebugRequest()
{
}

std::string StopDebugRequest::ClassName() const
{
    return "StopDebugRequest";
}

std::string StopDebugRequest::StaticClassName()
{
    return "StopDebugRequest";
}

void* StopDebugRequest::Create()
{
    return new StopDebugRequest();
}

void StopDebugRequest::Register()
{
    soul::xml::serialization::RegisterClass<StopDebugRequest>();
}

void StopDebugRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopDebugRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopDebugRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopDebugRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopDebugRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopDebugRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopDebugRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopDebugRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopDebugRequest::FromXml(soul::xml::Element* element)
{
}

StopDebugReply::StopDebugReply()
{
}

StopDebugReply::~StopDebugReply()
{
}

std::string StopDebugReply::ClassName() const
{
    return "StopDebugReply";
}

std::string StopDebugReply::StaticClassName()
{
    return "StopDebugReply";
}

void* StopDebugReply::Create()
{
    return new StopDebugReply();
}

void StopDebugReply::Register()
{
    soul::xml::serialization::RegisterClass<StopDebugReply>();
}

void StopDebugReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopDebugReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopDebugReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopDebugReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopDebugReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopDebugReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopDebugReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopDebugReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopDebugReply::FromXml(soul::xml::Element* element)
{
}

GenericDebugErrorReply::GenericDebugErrorReply()
     : errorMessage()
{
}

GenericDebugErrorReply::~GenericDebugErrorReply()
{
}

std::string GenericDebugErrorReply::ClassName() const
{
    return "GenericDebugErrorReply";
}

std::string GenericDebugErrorReply::StaticClassName()
{
    return "GenericDebugErrorReply";
}

void* GenericDebugErrorReply::Create()
{
    return new GenericDebugErrorReply();
}

void GenericDebugErrorReply::Register()
{
    soul::xml::serialization::RegisterClass<GenericDebugErrorReply>();
}

void GenericDebugErrorReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GenericDebugErrorReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GenericDebugErrorReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GenericDebugErrorReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GenericDebugErrorReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GenericDebugErrorReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GenericDebugErrorReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GenericDebugErrorReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(errorMessage, "errorMessage"));
    return element;
}

void GenericDebugErrorReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "errorMessage", errorMessage);
}

ContinueRequest::ContinueRequest()
{
}

ContinueRequest::~ContinueRequest()
{
}

std::string ContinueRequest::ClassName() const
{
    return "ContinueRequest";
}

std::string ContinueRequest::StaticClassName()
{
    return "ContinueRequest";
}

void* ContinueRequest::Create()
{
    return new ContinueRequest();
}

void ContinueRequest::Register()
{
    soul::xml::serialization::RegisterClass<ContinueRequest>();
}

void ContinueRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ContinueRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ContinueRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ContinueRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ContinueRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ContinueRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ContinueRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ContinueRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void ContinueRequest::FromXml(soul::xml::Element* element)
{
}

ContinueReply::ContinueReply()
     : success(), location(), state(), error()
{
}

ContinueReply::~ContinueReply()
{
}

std::string ContinueReply::ClassName() const
{
    return "ContinueReply";
}

std::string ContinueReply::StaticClassName()
{
    return "ContinueReply";
}

void* ContinueReply::Create()
{
    return new ContinueReply();
}

void ContinueReply::Register()
{
    soul::xml::serialization::RegisterClass<ContinueReply>();
}

void ContinueReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ContinueReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ContinueReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ContinueReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ContinueReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ContinueReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ContinueReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ContinueReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void ContinueReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "error", error);
}

NextRequest::NextRequest()
{
}

NextRequest::~NextRequest()
{
}

std::string NextRequest::ClassName() const
{
    return "NextRequest";
}

std::string NextRequest::StaticClassName()
{
    return "NextRequest";
}

void* NextRequest::Create()
{
    return new NextRequest();
}

void NextRequest::Register()
{
    soul::xml::serialization::RegisterClass<NextRequest>();
}

void NextRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("NextRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string NextRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("NextRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void NextRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void NextRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "NextRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* NextRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void NextRequest::FromXml(soul::xml::Element* element)
{
}

NextReply::NextReply()
     : success(), location(), state(), error()
{
}

NextReply::~NextReply()
{
}

std::string NextReply::ClassName() const
{
    return "NextReply";
}

std::string NextReply::StaticClassName()
{
    return "NextReply";
}

void* NextReply::Create()
{
    return new NextReply();
}

void NextReply::Register()
{
    soul::xml::serialization::RegisterClass<NextReply>();
}

void NextReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("NextReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string NextReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("NextReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void NextReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void NextReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "NextReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* NextReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void NextReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "error", error);
}

StepRequest::StepRequest()
{
}

StepRequest::~StepRequest()
{
}

std::string StepRequest::ClassName() const
{
    return "StepRequest";
}

std::string StepRequest::StaticClassName()
{
    return "StepRequest";
}

void* StepRequest::Create()
{
    return new StepRequest();
}

void StepRequest::Register()
{
    soul::xml::serialization::RegisterClass<StepRequest>();
}

void StepRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StepRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StepRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StepRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StepRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StepRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StepRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StepRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StepRequest::FromXml(soul::xml::Element* element)
{
}

StepReply::StepReply()
     : success(), location(), state(), error()
{
}

StepReply::~StepReply()
{
}

std::string StepReply::ClassName() const
{
    return "StepReply";
}

std::string StepReply::StaticClassName()
{
    return "StepReply";
}

void* StepReply::Create()
{
    return new StepReply();
}

void StepReply::Register()
{
    soul::xml::serialization::RegisterClass<StepReply>();
}

void StepReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StepReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StepReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StepReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StepReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StepReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StepReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StepReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void StepReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "error", error);
}

FinishRequest::FinishRequest()
{
}

FinishRequest::~FinishRequest()
{
}

std::string FinishRequest::ClassName() const
{
    return "FinishRequest";
}

std::string FinishRequest::StaticClassName()
{
    return "FinishRequest";
}

void* FinishRequest::Create()
{
    return new FinishRequest();
}

void FinishRequest::Register()
{
    soul::xml::serialization::RegisterClass<FinishRequest>();
}

void FinishRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FinishRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string FinishRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FinishRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void FinishRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void FinishRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "FinishRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* FinishRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void FinishRequest::FromXml(soul::xml::Element* element)
{
}

FinishReply::FinishReply()
     : success(), location(), state(), error()
{
}

FinishReply::~FinishReply()
{
}

std::string FinishReply::ClassName() const
{
    return "FinishReply";
}

std::string FinishReply::StaticClassName()
{
    return "FinishReply";
}

void* FinishReply::Create()
{
    return new FinishReply();
}

void FinishReply::Register()
{
    soul::xml::serialization::RegisterClass<FinishReply>();
}

void FinishReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FinishReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string FinishReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FinishReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void FinishReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void FinishReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "FinishReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* FinishReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void FinishReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "error", error);
}

UntilRequest::UntilRequest()
     : sourceLoc()
{
}

UntilRequest::~UntilRequest()
{
}

std::string UntilRequest::ClassName() const
{
    return "UntilRequest";
}

std::string UntilRequest::StaticClassName()
{
    return "UntilRequest";
}

void* UntilRequest::Create()
{
    return new UntilRequest();
}

void UntilRequest::Register()
{
    soul::xml::serialization::RegisterClass<UntilRequest>();
}

void UntilRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("UntilRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string UntilRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("UntilRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void UntilRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void UntilRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "UntilRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* UntilRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(sourceLoc, "sourceLoc"));
    return element;
}

void UntilRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "sourceLoc", sourceLoc);
}

UntilReply::UntilReply()
     : success(), location(), state(), error()
{
}

UntilReply::~UntilReply()
{
}

std::string UntilReply::ClassName() const
{
    return "UntilReply";
}

std::string UntilReply::StaticClassName()
{
    return "UntilReply";
}

void* UntilReply::Create()
{
    return new UntilReply();
}

void UntilReply::Register()
{
    soul::xml::serialization::RegisterClass<UntilReply>();
}

void UntilReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("UntilReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string UntilReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("UntilReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void UntilReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void UntilReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "UntilReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* UntilReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(state, "state"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void UntilReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "state", state);
    soul::xml::serialization::FromXml(element, "error", error);
}

BreakRequest::BreakRequest()
     : breakpointLocation()
{
}

BreakRequest::~BreakRequest()
{
}

std::string BreakRequest::ClassName() const
{
    return "BreakRequest";
}

std::string BreakRequest::StaticClassName()
{
    return "BreakRequest";
}

void* BreakRequest::Create()
{
    return new BreakRequest();
}

void BreakRequest::Register()
{
    soul::xml::serialization::RegisterClass<BreakRequest>();
}

void BreakRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BreakRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BreakRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BreakRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BreakRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BreakRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(breakpointLocation, "breakpointLocation"));
    return element;
}

void BreakRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "breakpointLocation", breakpointLocation);
}

BreakReply::BreakReply()
     : breakpointInfo()
{
}

BreakReply::~BreakReply()
{
}

std::string BreakReply::ClassName() const
{
    return "BreakReply";
}

std::string BreakReply::StaticClassName()
{
    return "BreakReply";
}

void* BreakReply::Create()
{
    return new BreakReply();
}

void BreakReply::Register()
{
    soul::xml::serialization::RegisterClass<BreakReply>();
}

void BreakReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BreakReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BreakReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BreakReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BreakReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BreakReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BreakReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(breakpointInfo, "breakpointInfo"));
    return element;
}

void BreakReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "breakpointInfo", breakpointInfo);
}

DeleteRequest::DeleteRequest()
     : breakpointId()
{
}

DeleteRequest::~DeleteRequest()
{
}

std::string DeleteRequest::ClassName() const
{
    return "DeleteRequest";
}

std::string DeleteRequest::StaticClassName()
{
    return "DeleteRequest";
}

void* DeleteRequest::Create()
{
    return new DeleteRequest();
}

void DeleteRequest::Register()
{
    soul::xml::serialization::RegisterClass<DeleteRequest>();
}

void DeleteRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DeleteRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string DeleteRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DeleteRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void DeleteRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void DeleteRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "DeleteRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* DeleteRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(breakpointId, "breakpointId"));
    return element;
}

void DeleteRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "breakpointId", breakpointId);
}

DeleteReply::DeleteReply()
     : success(), error()
{
}

DeleteReply::~DeleteReply()
{
}

std::string DeleteReply::ClassName() const
{
    return "DeleteReply";
}

std::string DeleteReply::StaticClassName()
{
    return "DeleteReply";
}

void* DeleteReply::Create()
{
    return new DeleteReply();
}

void DeleteReply::Register()
{
    soul::xml::serialization::RegisterClass<DeleteReply>();
}

void DeleteReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DeleteReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string DeleteReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DeleteReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void DeleteReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void DeleteReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "DeleteReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* DeleteReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void DeleteReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "error", error);
}

DepthRequest::DepthRequest()
{
}

DepthRequest::~DepthRequest()
{
}

std::string DepthRequest::ClassName() const
{
    return "DepthRequest";
}

std::string DepthRequest::StaticClassName()
{
    return "DepthRequest";
}

void* DepthRequest::Create()
{
    return new DepthRequest();
}

void DepthRequest::Register()
{
    soul::xml::serialization::RegisterClass<DepthRequest>();
}

void DepthRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DepthRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string DepthRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DepthRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void DepthRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void DepthRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "DepthRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* DepthRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void DepthRequest::FromXml(soul::xml::Element* element)
{
}

DepthReply::DepthReply()
     : success(), depth(), error()
{
}

DepthReply::~DepthReply()
{
}

std::string DepthReply::ClassName() const
{
    return "DepthReply";
}

std::string DepthReply::StaticClassName()
{
    return "DepthReply";
}

void* DepthReply::Create()
{
    return new DepthReply();
}

void DepthReply::Register()
{
    soul::xml::serialization::RegisterClass<DepthReply>();
}

void DepthReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DepthReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string DepthReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DepthReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void DepthReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void DepthReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "DepthReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* DepthReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(depth, "depth"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void DepthReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "depth", depth);
    soul::xml::serialization::FromXml(element, "error", error);
}

FramesRequest::FramesRequest()
     : lowFrame(), highFrame()
{
}

FramesRequest::~FramesRequest()
{
}

std::string FramesRequest::ClassName() const
{
    return "FramesRequest";
}

std::string FramesRequest::StaticClassName()
{
    return "FramesRequest";
}

void* FramesRequest::Create()
{
    return new FramesRequest();
}

void FramesRequest::Register()
{
    soul::xml::serialization::RegisterClass<FramesRequest>();
}

void FramesRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FramesRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string FramesRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FramesRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void FramesRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void FramesRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "FramesRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* FramesRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(lowFrame, "lowFrame"));
    element->AppendChild(soul::xml::serialization::ToXml(highFrame, "highFrame"));
    return element;
}

void FramesRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "lowFrame", lowFrame);
    soul::xml::serialization::FromXml(element, "highFrame", highFrame);
}

FramesReply::FramesReply()
     : success(), frames(), error()
{
}

FramesReply::~FramesReply()
{
}

std::string FramesReply::ClassName() const
{
    return "FramesReply";
}

std::string FramesReply::StaticClassName()
{
    return "FramesReply";
}

void* FramesReply::Create()
{
    return new FramesReply();
}

void FramesReply::Register()
{
    soul::xml::serialization::RegisterClass<FramesReply>();
}

void FramesReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FramesReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string FramesReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("FramesReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void FramesReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void FramesReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "FramesReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* FramesReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(frames, "frames"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void FramesReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "frames", frames);
    soul::xml::serialization::FromXml(element, "error", error);
}

EvaluateRequest::EvaluateRequest()
     : expression()
{
}

EvaluateRequest::~EvaluateRequest()
{
}

std::string EvaluateRequest::ClassName() const
{
    return "EvaluateRequest";
}

std::string EvaluateRequest::StaticClassName()
{
    return "EvaluateRequest";
}

void* EvaluateRequest::Create()
{
    return new EvaluateRequest();
}

void EvaluateRequest::Register()
{
    soul::xml::serialization::RegisterClass<EvaluateRequest>();
}

void EvaluateRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string EvaluateRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void EvaluateRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void EvaluateRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "EvaluateRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* EvaluateRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(expression, "expression"));
    return element;
}

void EvaluateRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "expression", expression);
}

EvaluateReply::EvaluateReply()
     : success(), result(), error()
{
}

EvaluateReply::~EvaluateReply()
{
}

std::string EvaluateReply::ClassName() const
{
    return "EvaluateReply";
}

std::string EvaluateReply::StaticClassName()
{
    return "EvaluateReply";
}

void* EvaluateReply::Create()
{
    return new EvaluateReply();
}

void EvaluateReply::Register()
{
    soul::xml::serialization::RegisterClass<EvaluateReply>();
}

void EvaluateReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string EvaluateReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void EvaluateReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void EvaluateReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "EvaluateReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* EvaluateReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(result, "result"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void EvaluateReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "result", result);
    soul::xml::serialization::FromXml(element, "error", error);
}

CountRequest::CountRequest()
     : expression()
{
}

CountRequest::~CountRequest()
{
}

std::string CountRequest::ClassName() const
{
    return "CountRequest";
}

std::string CountRequest::StaticClassName()
{
    return "CountRequest";
}

void* CountRequest::Create()
{
    return new CountRequest();
}

void CountRequest::Register()
{
    soul::xml::serialization::RegisterClass<CountRequest>();
}

void CountRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CountRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CountRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CountRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CountRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CountRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CountRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CountRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(expression, "expression"));
    return element;
}

void CountRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "expression", expression);
}

CountReply::CountReply()
     : success(), count(), error()
{
}

CountReply::~CountReply()
{
}

std::string CountReply::ClassName() const
{
    return "CountReply";
}

std::string CountReply::StaticClassName()
{
    return "CountReply";
}

void* CountReply::Create()
{
    return new CountReply();
}

void CountReply::Register()
{
    soul::xml::serialization::RegisterClass<CountReply>();
}

void CountReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CountReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CountReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CountReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CountReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CountReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CountReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CountReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(count, "count"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void CountReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "count", count);
    soul::xml::serialization::FromXml(element, "error", error);
}

EvaluateChildRequest::EvaluateChildRequest()
     : expression(), start(), count()
{
}

EvaluateChildRequest::~EvaluateChildRequest()
{
}

std::string EvaluateChildRequest::ClassName() const
{
    return "EvaluateChildRequest";
}

std::string EvaluateChildRequest::StaticClassName()
{
    return "EvaluateChildRequest";
}

void* EvaluateChildRequest::Create()
{
    return new EvaluateChildRequest();
}

void EvaluateChildRequest::Register()
{
    soul::xml::serialization::RegisterClass<EvaluateChildRequest>();
}

void EvaluateChildRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateChildRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string EvaluateChildRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateChildRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void EvaluateChildRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void EvaluateChildRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "EvaluateChildRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* EvaluateChildRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(expression, "expression"));
    element->AppendChild(soul::xml::serialization::ToXml(start, "start"));
    element->AppendChild(soul::xml::serialization::ToXml(count, "count"));
    return element;
}

void EvaluateChildRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "expression", expression);
    soul::xml::serialization::FromXml(element, "start", start);
    soul::xml::serialization::FromXml(element, "count", count);
}

EvaluateChildReply::EvaluateChildReply()
     : success(), results(), error()
{
}

EvaluateChildReply::~EvaluateChildReply()
{
}

std::string EvaluateChildReply::ClassName() const
{
    return "EvaluateChildReply";
}

std::string EvaluateChildReply::StaticClassName()
{
    return "EvaluateChildReply";
}

void* EvaluateChildReply::Create()
{
    return new EvaluateChildReply();
}

void EvaluateChildReply::Register()
{
    soul::xml::serialization::RegisterClass<EvaluateChildReply>();
}

void EvaluateChildReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateChildReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string EvaluateChildReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("EvaluateChildReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void EvaluateChildReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void EvaluateChildReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "EvaluateChildReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* EvaluateChildReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(results, "results"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void EvaluateChildReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "results", results);
    soul::xml::serialization::FromXml(element, "error", error);
}

TargetRunningRequest::TargetRunningRequest()
{
}

TargetRunningRequest::~TargetRunningRequest()
{
}

std::string TargetRunningRequest::ClassName() const
{
    return "TargetRunningRequest";
}

std::string TargetRunningRequest::StaticClassName()
{
    return "TargetRunningRequest";
}

void* TargetRunningRequest::Create()
{
    return new TargetRunningRequest();
}

void TargetRunningRequest::Register()
{
    soul::xml::serialization::RegisterClass<TargetRunningRequest>();
}

void TargetRunningRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetRunningRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetRunningRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetRunningRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetRunningRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetRunningRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetRunningRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetRunningRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void TargetRunningRequest::FromXml(soul::xml::Element* element)
{
}

TargetRunningReply::TargetRunningReply()
{
}

TargetRunningReply::~TargetRunningReply()
{
}

std::string TargetRunningReply::ClassName() const
{
    return "TargetRunningReply";
}

std::string TargetRunningReply::StaticClassName()
{
    return "TargetRunningReply";
}

void* TargetRunningReply::Create()
{
    return new TargetRunningReply();
}

void TargetRunningReply::Register()
{
    soul::xml::serialization::RegisterClass<TargetRunningReply>();
}

void TargetRunningReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetRunningReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetRunningReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetRunningReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetRunningReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetRunningReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetRunningReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetRunningReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void TargetRunningReply::FromXml(soul::xml::Element* element)
{
}

TargetInputRequest::TargetInputRequest()
{
}

TargetInputRequest::~TargetInputRequest()
{
}

std::string TargetInputRequest::ClassName() const
{
    return "TargetInputRequest";
}

std::string TargetInputRequest::StaticClassName()
{
    return "TargetInputRequest";
}

void* TargetInputRequest::Create()
{
    return new TargetInputRequest();
}

void TargetInputRequest::Register()
{
    soul::xml::serialization::RegisterClass<TargetInputRequest>();
}

void TargetInputRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetInputRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetInputRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetInputRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetInputRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetInputRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetInputRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetInputRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void TargetInputRequest::FromXml(soul::xml::Element* element)
{
}

TargetInputReply::TargetInputReply()
     : line(), eof()
{
}

TargetInputReply::~TargetInputReply()
{
}

std::string TargetInputReply::ClassName() const
{
    return "TargetInputReply";
}

std::string TargetInputReply::StaticClassName()
{
    return "TargetInputReply";
}

void* TargetInputReply::Create()
{
    return new TargetInputReply();
}

void TargetInputReply::Register()
{
    soul::xml::serialization::RegisterClass<TargetInputReply>();
}

void TargetInputReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetInputReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetInputReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetInputReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetInputReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetInputReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetInputReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetInputReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(eof, "eof"));
    return element;
}

void TargetInputReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "eof", eof);
}

TargetOutputRequest::TargetOutputRequest()
     : handle(), output()
{
}

TargetOutputRequest::~TargetOutputRequest()
{
}

std::string TargetOutputRequest::ClassName() const
{
    return "TargetOutputRequest";
}

std::string TargetOutputRequest::StaticClassName()
{
    return "TargetOutputRequest";
}

void* TargetOutputRequest::Create()
{
    return new TargetOutputRequest();
}

void TargetOutputRequest::Register()
{
    soul::xml::serialization::RegisterClass<TargetOutputRequest>();
}

void TargetOutputRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetOutputRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetOutputRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetOutputRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetOutputRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetOutputRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetOutputRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetOutputRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(handle, "handle"));
    element->AppendChild(soul::xml::serialization::ToXml(output, "output"));
    return element;
}

void TargetOutputRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "handle", handle);
    soul::xml::serialization::FromXml(element, "output", output);
}

TargetOutputReply::TargetOutputReply()
{
}

TargetOutputReply::~TargetOutputReply()
{
}

std::string TargetOutputReply::ClassName() const
{
    return "TargetOutputReply";
}

std::string TargetOutputReply::StaticClassName()
{
    return "TargetOutputReply";
}

void* TargetOutputReply::Create()
{
    return new TargetOutputReply();
}

void TargetOutputReply::Register()
{
    soul::xml::serialization::RegisterClass<TargetOutputReply>();
}

void TargetOutputReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetOutputReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string TargetOutputReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("TargetOutputReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void TargetOutputReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void TargetOutputReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "TargetOutputReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* TargetOutputReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void TargetOutputReply::FromXml(soul::xml::Element* element)
{
}

LogDebugMessageRequest::LogDebugMessageRequest()
     : logMessage()
{
}

LogDebugMessageRequest::~LogDebugMessageRequest()
{
}

std::string LogDebugMessageRequest::ClassName() const
{
    return "LogDebugMessageRequest";
}

std::string LogDebugMessageRequest::StaticClassName()
{
    return "LogDebugMessageRequest";
}

void* LogDebugMessageRequest::Create()
{
    return new LogDebugMessageRequest();
}

void LogDebugMessageRequest::Register()
{
    soul::xml::serialization::RegisterClass<LogDebugMessageRequest>();
}

void LogDebugMessageRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogDebugMessageRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string LogDebugMessageRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogDebugMessageRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void LogDebugMessageRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void LogDebugMessageRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "LogDebugMessageRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* LogDebugMessageRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(logMessage, "logMessage"));
    return element;
}

void LogDebugMessageRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "logMessage", logMessage);
}

LogDebugMessageReply::LogDebugMessageReply()
{
}

LogDebugMessageReply::~LogDebugMessageReply()
{
}

std::string LogDebugMessageReply::ClassName() const
{
    return "LogDebugMessageReply";
}

std::string LogDebugMessageReply::StaticClassName()
{
    return "LogDebugMessageReply";
}

void* LogDebugMessageReply::Create()
{
    return new LogDebugMessageReply();
}

void LogDebugMessageReply::Register()
{
    soul::xml::serialization::RegisterClass<LogDebugMessageReply>();
}

void LogDebugMessageReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogDebugMessageReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string LogDebugMessageReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogDebugMessageReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void LogDebugMessageReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void LogDebugMessageReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "LogDebugMessageReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* LogDebugMessageReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void LogDebugMessageReply::FromXml(soul::xml::Element* element)
{
}

KillRequest::KillRequest()
{
}

KillRequest::~KillRequest()
{
}

std::string KillRequest::ClassName() const
{
    return "KillRequest";
}

std::string KillRequest::StaticClassName()
{
    return "KillRequest";
}

void* KillRequest::Create()
{
    return new KillRequest();
}

void KillRequest::Register()
{
    soul::xml::serialization::RegisterClass<KillRequest>();
}

void KillRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KillRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KillRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KillRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KillRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KillRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KillRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KillRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KillRequest::FromXml(soul::xml::Element* element)
{
}

KillReply::KillReply()
{
}

KillReply::~KillReply()
{
}

std::string KillReply::ClassName() const
{
    return "KillReply";
}

std::string KillReply::StaticClassName()
{
    return "KillReply";
}

void* KillReply::Create()
{
    return new KillReply();
}

void KillReply::Register()
{
    soul::xml::serialization::RegisterClass<KillReply>();
}

void KillReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KillReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KillReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KillReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KillReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KillReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KillReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KillReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KillReply::FromXml(soul::xml::Element* element)
{
}


namespace {

struct Reg
{
    Reg()
    {
        Location::Register();
        Type::Register();
        Result::Register();
        ChildResult::Register();
        TargetState::Register();
        SourceLoc::Register();
        BreakpointInfo::Register();
        StartDebugRequest::Register();
        StartDebugReply::Register();
        StopDebugRequest::Register();
        StopDebugReply::Register();
        GenericDebugErrorReply::Register();
        ContinueRequest::Register();
        ContinueReply::Register();
        NextRequest::Register();
        NextReply::Register();
        StepRequest::Register();
        StepReply::Register();
        FinishRequest::Register();
        FinishReply::Register();
        UntilRequest::Register();
        UntilReply::Register();
        BreakRequest::Register();
        BreakReply::Register();
        DeleteRequest::Register();
        DeleteReply::Register();
        DepthRequest::Register();
        DepthReply::Register();
        FramesRequest::Register();
        FramesReply::Register();
        EvaluateRequest::Register();
        EvaluateReply::Register();
        CountRequest::Register();
        CountReply::Register();
        EvaluateChildRequest::Register();
        EvaluateChildReply::Register();
        TargetRunningRequest::Register();
        TargetRunningReply::Register();
        TargetInputRequest::Register();
        TargetInputReply::Register();
        TargetOutputRequest::Register();
        TargetOutputReply::Register();
        LogDebugMessageRequest::Register();
        LogDebugMessageReply::Register();
        KillRequest::Register();
        KillReply::Register();
    }
};

Reg reg;

} // namespace

