// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/cmcode/solution_settings.xml-schema' by xmlsergen version 5.0.0

export module cmcode.solution.settings;

import std.core;
import util.uuid;
import util.time;
import soul.xml.element;

export class SolutionBreakpoint
{
public:
    SolutionBreakpoint();
    virtual ~SolutionBreakpoint();
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

export class SolutionSettings
{
public:
    SolutionSettings();
    virtual ~SolutionSettings();
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
    bool callStackOpen;
    bool localsViewOpen;
    std::vector<std::string> openFiles;
    std::string currentOpenFile;
    int32_t currentCursorLine;
    std::vector<SolutionBreakpoint> breakpoints;
    std::vector<std::string> expandedProjects;
};

