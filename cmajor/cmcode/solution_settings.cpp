// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/cmcode/solution_settings.xml-schema' by xmlsergen version 5.0.0

module cmcode.solution.settings;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

SolutionBreakpoint::SolutionBreakpoint()
     : file(), line(), condition(), disabled()
{
}

SolutionBreakpoint::~SolutionBreakpoint()
{
}

std::string SolutionBreakpoint::ClassName() const
{
    return "SolutionBreakpoint";
}

std::string SolutionBreakpoint::StaticClassName()
{
    return "SolutionBreakpoint";
}

void* SolutionBreakpoint::Create()
{
    return new SolutionBreakpoint();
}

void SolutionBreakpoint::Register()
{
    soul::xml::serialization::RegisterClass<SolutionBreakpoint>();
}

void SolutionBreakpoint::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SolutionBreakpoint"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string SolutionBreakpoint::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SolutionBreakpoint"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void SolutionBreakpoint::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void SolutionBreakpoint::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "SolutionBreakpoint");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* SolutionBreakpoint::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(file, "file"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(condition, "condition"));
    element->AppendChild(soul::xml::serialization::ToXml(disabled, "disabled"));
    return element;
}

void SolutionBreakpoint::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "file", file);
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "condition", condition);
    soul::xml::serialization::FromXml(element, "disabled", disabled);
}

SolutionSettings::SolutionSettings()
     : callStackOpen(), localsViewOpen(), openFiles(), currentOpenFile(), currentCursorLine(), breakpoints(), expandedProjects()
{
}

SolutionSettings::~SolutionSettings()
{
}

std::string SolutionSettings::ClassName() const
{
    return "SolutionSettings";
}

std::string SolutionSettings::StaticClassName()
{
    return "SolutionSettings";
}

void* SolutionSettings::Create()
{
    return new SolutionSettings();
}

void SolutionSettings::Register()
{
    soul::xml::serialization::RegisterClass<SolutionSettings>();
}

void SolutionSettings::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SolutionSettings"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string SolutionSettings::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("SolutionSettings"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void SolutionSettings::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void SolutionSettings::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "SolutionSettings");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* SolutionSettings::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(callStackOpen, "callStackOpen"));
    element->AppendChild(soul::xml::serialization::ToXml(localsViewOpen, "localsViewOpen"));
    element->AppendChild(soul::xml::serialization::ToXml(openFiles, "openFiles"));
    element->AppendChild(soul::xml::serialization::ToXml(currentOpenFile, "currentOpenFile"));
    element->AppendChild(soul::xml::serialization::ToXml(currentCursorLine, "currentCursorLine"));
    element->AppendChild(soul::xml::serialization::ToXml(breakpoints, "breakpoints"));
    element->AppendChild(soul::xml::serialization::ToXml(expandedProjects, "expandedProjects"));
    return element;
}

void SolutionSettings::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "callStackOpen", callStackOpen);
    soul::xml::serialization::FromXml(element, "localsViewOpen", localsViewOpen);
    soul::xml::serialization::FromXml(element, "openFiles", openFiles);
    soul::xml::serialization::FromXml(element, "currentOpenFile", currentOpenFile);
    soul::xml::serialization::FromXml(element, "currentCursorLine", currentCursorLine);
    soul::xml::serialization::FromXml(element, "breakpoints", breakpoints);
    soul::xml::serialization::FromXml(element, "expandedProjects", expandedProjects);
}


namespace {

struct Reg
{
    Reg()
    {
        SolutionBreakpoint::Register();
        SolutionSettings::Register();
    }
};

Reg reg;

} // namespace

