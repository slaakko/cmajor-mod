// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cmcode/project_settings.xml-schema' by xmlsergen version 4.1.0

export module cmcode.project.settings;

import std.core;
import util.uuid;
import util.time;
import soul.xml.element;

export class ProjectBreakpoint
{
public:
    ProjectBreakpoint();
    virtual ~ProjectBreakpoint();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
    std::string file;
    int32_t line;
    std::string condition;
    bool disabled;
};

export class ProjectSettings
{
public:
    ProjectSettings();
    virtual ~ProjectSettings();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
    std::string programArguments;
    std::vector<ProjectBreakpoint> breakpoints;
};

