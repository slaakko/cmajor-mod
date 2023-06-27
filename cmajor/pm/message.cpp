// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/pm/message.schema' by xmlsergen version 4.1.0

module cmajor.portmap.message;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

HelloPmsRequest::HelloPmsRequest()
{
}

HelloPmsRequest::~HelloPmsRequest()
{
}

std::string HelloPmsRequest::ClassName() const
{
    return "HelloPmsRequest";
}

std::string HelloPmsRequest::StaticClassName()
{
    return "HelloPmsRequest";
}

void* HelloPmsRequest::Create()
{
    return new HelloPmsRequest();
}

void HelloPmsRequest::Register()
{
    soul::xml::serialization::RegisterClass<HelloPmsRequest>();
}

void HelloPmsRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("HelloPmsRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string HelloPmsRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("HelloPmsRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void HelloPmsRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void HelloPmsRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "HelloPmsRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* HelloPmsRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void HelloPmsRequest::FromXml(soul::xml::Element* element)
{
}

HelloPmsReply::HelloPmsReply()
     : version()
{
}

HelloPmsReply::~HelloPmsReply()
{
}

std::string HelloPmsReply::ClassName() const
{
    return "HelloPmsReply";
}

std::string HelloPmsReply::StaticClassName()
{
    return "HelloPmsReply";
}

void* HelloPmsReply::Create()
{
    return new HelloPmsReply();
}

void HelloPmsReply::Register()
{
    soul::xml::serialization::RegisterClass<HelloPmsReply>();
}

void HelloPmsReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("HelloPmsReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string HelloPmsReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("HelloPmsReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void HelloPmsReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void HelloPmsReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "HelloPmsReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* HelloPmsReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(version, "version"));
    return element;
}

void HelloPmsReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "version", version);
}

GetFreePortNumberRequest::GetFreePortNumberRequest()
     : processName(), pid()
{
}

GetFreePortNumberRequest::~GetFreePortNumberRequest()
{
}

std::string GetFreePortNumberRequest::ClassName() const
{
    return "GetFreePortNumberRequest";
}

std::string GetFreePortNumberRequest::StaticClassName()
{
    return "GetFreePortNumberRequest";
}

void* GetFreePortNumberRequest::Create()
{
    return new GetFreePortNumberRequest();
}

void GetFreePortNumberRequest::Register()
{
    soul::xml::serialization::RegisterClass<GetFreePortNumberRequest>();
}

void GetFreePortNumberRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetFreePortNumberRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GetFreePortNumberRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetFreePortNumberRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GetFreePortNumberRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GetFreePortNumberRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GetFreePortNumberRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GetFreePortNumberRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(processName, "processName"));
    element->AppendChild(soul::xml::serialization::ToXml(pid, "pid"));
    return element;
}

void GetFreePortNumberRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "processName", processName);
    soul::xml::serialization::FromXml(element, "pid", pid);
}

GetFreePortNumberReply::GetFreePortNumberReply()
     : portNumber()
{
}

GetFreePortNumberReply::~GetFreePortNumberReply()
{
}

std::string GetFreePortNumberReply::ClassName() const
{
    return "GetFreePortNumberReply";
}

std::string GetFreePortNumberReply::StaticClassName()
{
    return "GetFreePortNumberReply";
}

void* GetFreePortNumberReply::Create()
{
    return new GetFreePortNumberReply();
}

void GetFreePortNumberReply::Register()
{
    soul::xml::serialization::RegisterClass<GetFreePortNumberReply>();
}

void GetFreePortNumberReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetFreePortNumberReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GetFreePortNumberReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetFreePortNumberReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GetFreePortNumberReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GetFreePortNumberReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GetFreePortNumberReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GetFreePortNumberReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(portNumber, "portNumber"));
    return element;
}

void GetFreePortNumberReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "portNumber", portNumber);
}

ExtendPortLeaseRequest::ExtendPortLeaseRequest()
     : processName(), pid(), portNumbers()
{
}

ExtendPortLeaseRequest::~ExtendPortLeaseRequest()
{
}

std::string ExtendPortLeaseRequest::ClassName() const
{
    return "ExtendPortLeaseRequest";
}

std::string ExtendPortLeaseRequest::StaticClassName()
{
    return "ExtendPortLeaseRequest";
}

void* ExtendPortLeaseRequest::Create()
{
    return new ExtendPortLeaseRequest();
}

void ExtendPortLeaseRequest::Register()
{
    soul::xml::serialization::RegisterClass<ExtendPortLeaseRequest>();
}

void ExtendPortLeaseRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ExtendPortLeaseRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ExtendPortLeaseRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ExtendPortLeaseRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ExtendPortLeaseRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ExtendPortLeaseRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ExtendPortLeaseRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ExtendPortLeaseRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(processName, "processName"));
    element->AppendChild(soul::xml::serialization::ToXml(pid, "pid"));
    element->AppendChild(soul::xml::serialization::ToXml(portNumbers, "portNumbers"));
    return element;
}

void ExtendPortLeaseRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "processName", processName);
    soul::xml::serialization::FromXml(element, "pid", pid);
    soul::xml::serialization::FromXml(element, "portNumbers", portNumbers);
}

ExtendPortLeaseReply::ExtendPortLeaseReply()
{
}

ExtendPortLeaseReply::~ExtendPortLeaseReply()
{
}

std::string ExtendPortLeaseReply::ClassName() const
{
    return "ExtendPortLeaseReply";
}

std::string ExtendPortLeaseReply::StaticClassName()
{
    return "ExtendPortLeaseReply";
}

void* ExtendPortLeaseReply::Create()
{
    return new ExtendPortLeaseReply();
}

void ExtendPortLeaseReply::Register()
{
    soul::xml::serialization::RegisterClass<ExtendPortLeaseReply>();
}

void ExtendPortLeaseReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ExtendPortLeaseReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ExtendPortLeaseReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ExtendPortLeaseReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ExtendPortLeaseReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ExtendPortLeaseReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ExtendPortLeaseReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ExtendPortLeaseReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void ExtendPortLeaseReply::FromXml(soul::xml::Element* element)
{
}

StopPortMapServerRequest::StopPortMapServerRequest()
{
}

StopPortMapServerRequest::~StopPortMapServerRequest()
{
}

std::string StopPortMapServerRequest::ClassName() const
{
    return "StopPortMapServerRequest";
}

std::string StopPortMapServerRequest::StaticClassName()
{
    return "StopPortMapServerRequest";
}

void* StopPortMapServerRequest::Create()
{
    return new StopPortMapServerRequest();
}

void StopPortMapServerRequest::Register()
{
    soul::xml::serialization::RegisterClass<StopPortMapServerRequest>();
}

void StopPortMapServerRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopPortMapServerRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopPortMapServerRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopPortMapServerRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopPortMapServerRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopPortMapServerRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopPortMapServerRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopPortMapServerRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopPortMapServerRequest::FromXml(soul::xml::Element* element)
{
}

StopPortMapServerReply::StopPortMapServerReply()
{
}

StopPortMapServerReply::~StopPortMapServerReply()
{
}

std::string StopPortMapServerReply::ClassName() const
{
    return "StopPortMapServerReply";
}

std::string StopPortMapServerReply::StaticClassName()
{
    return "StopPortMapServerReply";
}

void* StopPortMapServerReply::Create()
{
    return new StopPortMapServerReply();
}

void StopPortMapServerReply::Register()
{
    soul::xml::serialization::RegisterClass<StopPortMapServerReply>();
}

void StopPortMapServerReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopPortMapServerReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopPortMapServerReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopPortMapServerReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopPortMapServerReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopPortMapServerReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopPortMapServerReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopPortMapServerReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopPortMapServerReply::FromXml(soul::xml::Element* element)
{
}

PortLease::PortLease()
     : portNumber(), processName(), pid(), leaseStartTime(), leaseRenewalTime(), leaseState()
{
}

PortLease::~PortLease()
{
}

std::string PortLease::ClassName() const
{
    return "PortLease";
}

std::string PortLease::StaticClassName()
{
    return "PortLease";
}

void* PortLease::Create()
{
    return new PortLease();
}

void PortLease::Register()
{
    soul::xml::serialization::RegisterClass<PortLease>();
}

void PortLease::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("PortLease"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string PortLease::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("PortLease"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void PortLease::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void PortLease::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "PortLease");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* PortLease::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(portNumber, "portNumber"));
    element->AppendChild(soul::xml::serialization::ToXml(processName, "processName"));
    element->AppendChild(soul::xml::serialization::ToXml(pid, "pid"));
    element->AppendChild(soul::xml::serialization::ToXml(leaseStartTime, "leaseStartTime"));
    element->AppendChild(soul::xml::serialization::ToXml(leaseRenewalTime, "leaseRenewalTime"));
    element->AppendChild(soul::xml::serialization::ToXml(leaseState, "leaseState"));
    return element;
}

void PortLease::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "portNumber", portNumber);
    soul::xml::serialization::FromXml(element, "processName", processName);
    soul::xml::serialization::FromXml(element, "pid", pid);
    soul::xml::serialization::FromXml(element, "leaseStartTime", leaseStartTime);
    soul::xml::serialization::FromXml(element, "leaseRenewalTime", leaseRenewalTime);
    soul::xml::serialization::FromXml(element, "leaseState", leaseState);
}

ViewPortLeaseRequest::ViewPortLeaseRequest()
{
}

ViewPortLeaseRequest::~ViewPortLeaseRequest()
{
}

std::string ViewPortLeaseRequest::ClassName() const
{
    return "ViewPortLeaseRequest";
}

std::string ViewPortLeaseRequest::StaticClassName()
{
    return "ViewPortLeaseRequest";
}

void* ViewPortLeaseRequest::Create()
{
    return new ViewPortLeaseRequest();
}

void ViewPortLeaseRequest::Register()
{
    soul::xml::serialization::RegisterClass<ViewPortLeaseRequest>();
}

void ViewPortLeaseRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ViewPortLeaseRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ViewPortLeaseRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ViewPortLeaseRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ViewPortLeaseRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ViewPortLeaseRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ViewPortLeaseRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ViewPortLeaseRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void ViewPortLeaseRequest::FromXml(soul::xml::Element* element)
{
}

ViewPortLeaseReply::ViewPortLeaseReply()
     : portLeases()
{
}

ViewPortLeaseReply::~ViewPortLeaseReply()
{
}

std::string ViewPortLeaseReply::ClassName() const
{
    return "ViewPortLeaseReply";
}

std::string ViewPortLeaseReply::StaticClassName()
{
    return "ViewPortLeaseReply";
}

void* ViewPortLeaseReply::Create()
{
    return new ViewPortLeaseReply();
}

void ViewPortLeaseReply::Register()
{
    soul::xml::serialization::RegisterClass<ViewPortLeaseReply>();
}

void ViewPortLeaseReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ViewPortLeaseReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ViewPortLeaseReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ViewPortLeaseReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ViewPortLeaseReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ViewPortLeaseReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ViewPortLeaseReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ViewPortLeaseReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(portLeases, "portLeases"));
    return element;
}

void ViewPortLeaseReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "portLeases", portLeases);
}


namespace {

struct Reg
{
    Reg()
    {
        HelloPmsRequest::Register();
        HelloPmsReply::Register();
        GetFreePortNumberRequest::Register();
        GetFreePortNumberReply::Register();
        ExtendPortLeaseRequest::Register();
        ExtendPortLeaseReply::Register();
        StopPortMapServerRequest::Register();
        StopPortMapServerReply::Register();
        PortLease::Register();
        ViewPortLeaseRequest::Register();
        ViewPortLeaseReply::Register();
    }
};

Reg reg;

} // namespace

