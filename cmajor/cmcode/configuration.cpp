// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cmcode/configuration.xml-schema' by xmlsergen version 4.1.0

module cmcode.configuration;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

RecentSolution::RecentSolution()
     : name(), filePath()
{
}

RecentSolution::~RecentSolution()
{
}

std::string RecentSolution::ClassName() const
{
    return "RecentSolution";
}

std::string RecentSolution::StaticClassName()
{
    return "RecentSolution";
}

void* RecentSolution::Create()
{
    return new RecentSolution();
}

void RecentSolution::Register()
{
    soul::xml::serialization::RegisterClass<RecentSolution>();
}

void RecentSolution::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("RecentSolution"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string RecentSolution::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("RecentSolution"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void RecentSolution::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void RecentSolution::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "RecentSolution");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* RecentSolution::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(name, "name"));
    element->AppendChild(soul::xml::serialization::ToXml(filePath, "filePath"));
    return element;
}

void RecentSolution::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "name", name);
    soul::xml::serialization::FromXml(element, "filePath", filePath);
}

BuildSettings::BuildSettings()
     : singleThreadedCompile(), generateIntermediateCodeFiles()//, linkWithDebugRuntime()
{
}

BuildSettings::~BuildSettings()
{
}

std::string BuildSettings::ClassName() const
{
    return "BuildSettings";
}

std::string BuildSettings::StaticClassName()
{
    return "BuildSettings";
}

void* BuildSettings::Create()
{
    return new BuildSettings();
}

void BuildSettings::Register()
{
    soul::xml::serialization::RegisterClass<BuildSettings>();
}

void BuildSettings::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildSettings"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BuildSettings::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildSettings"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BuildSettings::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BuildSettings::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BuildSettings");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BuildSettings::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(singleThreadedCompile, "singleThreadedCompile"));
    element->AppendChild(soul::xml::serialization::ToXml(generateIntermediateCodeFiles, "generateIntermediateCodeFiles"));
    //element->AppendChild(soul::xml::serialization::ToXml(linkWithDebugRuntime, "linkWithDebugRuntime"));
    return element;
}

void BuildSettings::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "singleThreadedCompile", singleThreadedCompile);
    soul::xml::serialization::FromXml(element, "generateIntermediateCodeFiles", generateIntermediateCodeFiles);
    //soul::xml::serialization::FromXml(element, "linkWithDebugRuntime", linkWithDebugRuntime);
}

Options::Options()
     : defined(), /*codeCompletionDefined(), */showStartupDialog(), numberOfRecentSolutions(), /*codeCompletion(), parsingFrequency(), */themeDefined(), theme()
{
}

Options::~Options()
{
}

std::string Options::ClassName() const
{
    return "Options";
}

std::string Options::StaticClassName()
{
    return "Options";
}

void* Options::Create()
{
    return new Options();
}

void Options::Register()
{
    soul::xml::serialization::RegisterClass<Options>();
}

void Options::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Options"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string Options::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Options"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void Options::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void Options::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "Options");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* Options::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(defined, "defined"));
    //element->AppendChild(soul::xml::serialization::ToXml(codeCompletionDefined, "codeCompletionDefined"));
    element->AppendChild(soul::xml::serialization::ToXml(showStartupDialog, "showStartupDialog"));
    element->AppendChild(soul::xml::serialization::ToXml(numberOfRecentSolutions, "numberOfRecentSolutions"));
//    element->AppendChild(soul::xml::serialization::ToXml(codeCompletion, "codeCompletion"));
//    element->AppendChild(soul::xml::serialization::ToXml(parsingFrequency, "parsingFrequency"));
    element->AppendChild(soul::xml::serialization::ToXml(themeDefined, "themeDefined"));
    element->AppendChild(soul::xml::serialization::ToXml(theme, "theme"));
    return element;
}

void Options::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "defined", defined);
    //soul::xml::serialization::FromXml(element, "codeCompletionDefined", codeCompletionDefined);
    soul::xml::serialization::FromXml(element, "showStartupDialog", showStartupDialog);
    soul::xml::serialization::FromXml(element, "numberOfRecentSolutions", numberOfRecentSolutions);
    //soul::xml::serialization::FromXml(element, "codeCompletion", codeCompletion);
    //soul::xml::serialization::FromXml(element, "parsingFrequency", parsingFrequency);
    soul::xml::serialization::FromXml(element, "themeDefined", themeDefined);
    soul::xml::serialization::FromXml(element, "theme", theme);
}

WindowLocation::WindowLocation()
     : x(), y()
{
}

WindowLocation::~WindowLocation()
{
}

std::string WindowLocation::ClassName() const
{
    return "WindowLocation";
}

std::string WindowLocation::StaticClassName()
{
    return "WindowLocation";
}

void* WindowLocation::Create()
{
    return new WindowLocation();
}

void WindowLocation::Register()
{
    soul::xml::serialization::RegisterClass<WindowLocation>();
}

void WindowLocation::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowLocation"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string WindowLocation::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowLocation"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void WindowLocation::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void WindowLocation::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "WindowLocation");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* WindowLocation::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(x, "x"));
    element->AppendChild(soul::xml::serialization::ToXml(y, "y"));
    return element;
}

void WindowLocation::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "x", x);
    soul::xml::serialization::FromXml(element, "y", y);
}

WindowSize::WindowSize()
     : width(), height()
{
}

WindowSize::~WindowSize()
{
}

std::string WindowSize::ClassName() const
{
    return "WindowSize";
}

std::string WindowSize::StaticClassName()
{
    return "WindowSize";
}

void* WindowSize::Create()
{
    return new WindowSize();
}

void WindowSize::Register()
{
    soul::xml::serialization::RegisterClass<WindowSize>();
}

void WindowSize::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowSize"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string WindowSize::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowSize"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void WindowSize::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void WindowSize::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "WindowSize");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* WindowSize::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(width, "width"));
    element->AppendChild(soul::xml::serialization::ToXml(height, "height"));
    return element;
}

void WindowSize::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "width", width);
    soul::xml::serialization::FromXml(element, "height", height);
}

WindowSettings::WindowSettings()
     : defined(), windowState(), location(), size(), maximizedHorizontalSplitterDistance(), maximizedVerticalSplitterDistance(), normalHorizontalSplitterDistance(), normalVerticalSplitterDistance()
{
}

WindowSettings::~WindowSettings()
{
}

std::string WindowSettings::ClassName() const
{
    return "WindowSettings";
}

std::string WindowSettings::StaticClassName()
{
    return "WindowSettings";
}

void* WindowSettings::Create()
{
    return new WindowSettings();
}

void WindowSettings::Register()
{
    soul::xml::serialization::RegisterClass<WindowSettings>();
}

void WindowSettings::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowSettings"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string WindowSettings::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("WindowSettings"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void WindowSettings::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void WindowSettings::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "WindowSettings");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* WindowSettings::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(defined, "defined"));
    element->AppendChild(soul::xml::serialization::ToXml(windowState, "windowState"));
    element->AppendChild(soul::xml::serialization::ToXml(location, "location"));
    element->AppendChild(soul::xml::serialization::ToXml(size, "size"));
    element->AppendChild(soul::xml::serialization::ToXml(maximizedHorizontalSplitterDistance, "maximizedHorizontalSplitterDistance"));
    element->AppendChild(soul::xml::serialization::ToXml(maximizedVerticalSplitterDistance, "maximizedVerticalSplitterDistance"));
    element->AppendChild(soul::xml::serialization::ToXml(normalHorizontalSplitterDistance, "normalHorizontalSplitterDistance"));
    element->AppendChild(soul::xml::serialization::ToXml(normalVerticalSplitterDistance, "normalVerticalSplitterDistance"));
    return element;
}

void WindowSettings::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "defined", defined);
    soul::xml::serialization::FromXml(element, "windowState", windowState);
    soul::xml::serialization::FromXml(element, "location", location);
    soul::xml::serialization::FromXml(element, "size", size);
    soul::xml::serialization::FromXml(element, "maximizedHorizontalSplitterDistance", maximizedHorizontalSplitterDistance);
    soul::xml::serialization::FromXml(element, "maximizedVerticalSplitterDistance", maximizedVerticalSplitterDistance);
    soul::xml::serialization::FromXml(element, "normalHorizontalSplitterDistance", normalHorizontalSplitterDistance);
    soul::xml::serialization::FromXml(element, "normalVerticalSplitterDistance", normalVerticalSplitterDistance);
}

Configuration::Configuration()
     : recentSolutions(), options(), buildSettings(), windowSettings()
{
}

Configuration::~Configuration()
{
}

std::string Configuration::ClassName() const
{
    return "Configuration";
}

std::string Configuration::StaticClassName()
{
    return "Configuration";
}

void* Configuration::Create()
{
    return new Configuration();
}

void Configuration::Register()
{
    soul::xml::serialization::RegisterClass<Configuration>();
}

void Configuration::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Configuration"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string Configuration::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("Configuration"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void Configuration::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void Configuration::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "Configuration");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* Configuration::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(recentSolutions, "recentSolutions"));
    element->AppendChild(soul::xml::serialization::ToXml(options, "options"));
    element->AppendChild(soul::xml::serialization::ToXml(buildSettings, "buildSettings"));
    element->AppendChild(soul::xml::serialization::ToXml(windowSettings, "windowSettings"));
    return element;
}

void Configuration::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "recentSolutions", recentSolutions);
    soul::xml::serialization::FromXml(element, "options", options);
    soul::xml::serialization::FromXml(element, "buildSettings", buildSettings);
    soul::xml::serialization::FromXml(element, "windowSettings", windowSettings);
}


namespace {

struct Reg
{
    Reg()
    {
        RecentSolution::Register();
        BuildSettings::Register();
        Options::Register();
        WindowLocation::Register();
        WindowSize::Register();
        WindowSettings::Register();
        Configuration::Register();
    }
};

Reg reg;

} // namespace

