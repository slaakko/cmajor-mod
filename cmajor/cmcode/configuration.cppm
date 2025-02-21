// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/cmcode/configuration.xml-schema' by xmlsergen version 5.0.0

export module cmcode.configuration;

import std.core;
import util.uuid;
import util.time;
import soul.xml.element;

export class RecentSolution
{
public:
    RecentSolution();
    virtual ~RecentSolution();
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
    std::string name;
    std::string filePath;
};

export class BuildSettings
{
public:
    BuildSettings();
    virtual ~BuildSettings();
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
    bool singleThreadedCompile;
    bool generateIntermediateCodeFiles;
    std::string disabledWarnings;
};

export class Options
{
public:
    Options();
    virtual ~Options();
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
    bool defined;
    bool codeCompletionDefined;
    bool showStartupDialog;
    int32_t numberOfRecentSolutions;
    bool codeCompletion;
    int32_t parsingFrequency;
    bool themeDefined;
    std::string theme;
};

export class WindowLocation
{
public:
    WindowLocation();
    virtual ~WindowLocation();
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
    int32_t x;
    int32_t y;
};

export class WindowSize
{
public:
    WindowSize();
    virtual ~WindowSize();
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
    int32_t width;
    int32_t height;
};

export class WindowSettings
{
public:
    WindowSettings();
    virtual ~WindowSettings();
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
    bool defined;
    int32_t windowState;
    WindowLocation location;
    WindowSize size;
    int32_t maximizedHorizontalSplitterDistance;
    int32_t maximizedVerticalSplitterDistance;
    int32_t normalHorizontalSplitterDistance;
    int32_t normalVerticalSplitterDistance;
};

export class Configuration
{
public:
    Configuration();
    virtual ~Configuration();
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
    std::vector<RecentSolution> recentSolutions;
    Options options;
    BuildSettings buildSettings;
    WindowSettings windowSettings;
};

