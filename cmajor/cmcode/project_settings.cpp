// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/cmcode/project_settings.xml-schema' by xmlsergen version 5.0.0

module cmcode.project.settings;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

ProjectBreakpoint::ProjectBreakpoint()
     : file(), line(), condition(), disabled()
{
}

ProjectBreakpoint::~ProjectBreakpoint()
{
}

std::string ProjectBreakpoint::ClassName() const
{
    return "ProjectBreakpoint";
}

std::string ProjectBreakpoint::StaticClassName()
{
    return "ProjectBreakpoint";
}

void* ProjectBreakpoint::Create()
{
    return new ProjectBreakpoint();
}

void ProjectBreakpoint::Register()
{
    soul::xml::serialization::RegisterClass<ProjectBreakpoint>();
}

void ProjectBreakpoint::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ProjectBreakpoint"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ProjectBreakpoint::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ProjectBreakpoint"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ProjectBreakpoint::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ProjectBreakpoint::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ProjectBreakpoint");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ProjectBreakpoint::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(file, "file"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(condition, "condition"));
    element->AppendChild(soul::xml::serialization::ToXml(disabled, "disabled"));
    return element;
}

void ProjectBreakpoint::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "file", file);
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "condition", condition);
    soul::xml::serialization::FromXml(element, "disabled", disabled);
}

ProjectSettings::ProjectSettings()
     : programArguments(), breakpoints()
{
}

ProjectSettings::~ProjectSettings()
{
}

std::string ProjectSettings::ClassName() const
{
    return "ProjectSettings";
}

std::string ProjectSettings::StaticClassName()
{
    return "ProjectSettings";
}

void* ProjectSettings::Create()
{
    return new ProjectSettings();
}

void ProjectSettings::Register()
{
    soul::xml::serialization::RegisterClass<ProjectSettings>();
}

void ProjectSettings::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ProjectSettings"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string ProjectSettings::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("ProjectSettings"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void ProjectSettings::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void ProjectSettings::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "ProjectSettings");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* ProjectSettings::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(programArguments, "programArguments"));
    element->AppendChild(soul::xml::serialization::ToXml(breakpoints, "breakpoints"));
    return element;
}

void ProjectSettings::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "programArguments", programArguments);
    soul::xml::serialization::FromXml(element, "breakpoints", breakpoints);
}


namespace {

struct Reg
{
    Reg()
    {
        ProjectBreakpoint::Register();
        ProjectSettings::Register();
    }
};

Reg reg;

} // namespace

