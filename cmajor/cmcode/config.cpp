// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.config;

import cmajor.service.config;
import soul.xml.dom;
import soul.xml.dom.parser;

import util;

namespace cmcode {

bool UseDebugServers()
{
    //return true;
    return false;
}

bool ServerLogging()
{
    return true;
}

bool DebugWait()
{
    //return false;
    return true;
}

bool ServerVerbose()
{
    return true;
}

class Config
{
public:
    static Config& Instance();
    Configuration& GetConfiguration() { return configuration; }
    void Load();
    void Save();
private:
    Configuration configuration;
};

Config& Config::Instance()
{
    static Config instance;
    return instance;
}

void Config::Load()
{
    std::string configXmlFilePath = util::Path::Combine(cmajor::service::CmajorConfigDir(), "cmcode.config.xml");
    if (std::filesystem::exists(configXmlFilePath))
    {
        std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(configXmlFilePath);
        Configuration cfg;
        cfg.FromXml(configDoc->DocumentElement());
        configuration = Configuration(cfg);
        if (!configuration.options.defined)
        {
            configuration.options.defined = true;
            configuration.options.showStartupDialog = true;
            configuration.options.numberOfRecentSolutions = 5;
            configuration.options.codeCompletion = true;
            configuration.options.parsingFrequency = 1000;
        }
        if (!configuration.options.codeCompletionDefined)
        {
            configuration.options.codeCompletion = true;
            configuration.options.parsingFrequency = 1000;
        }
        if (!configuration.options.themeDefined)
        {
            configuration.options.theme = "light";
        }
    }
}

void Config::Save()
{
    std::string configXmlFilePath = util::Path::Combine(cmajor::service::CmajorConfigDir(), "cmcode.config.xml");
    configuration.options.defined = true;
    configuration.options.codeCompletionDefined = true;
    configuration.options.themeDefined = true;
    std::unique_ptr<soul::xml::Element> configurationElement(configuration.ToXml("configuration"));
    soul::xml::Document configDoc;
    configDoc.AppendChild(configurationElement.release());
    std::ofstream configFile(configXmlFilePath);
    util::CodeFormatter formatter(configFile);
    configDoc.Write(formatter);
}

void LoadConfiguration()
{
    Config::Instance().Load();
}

void SaveConfiguration()
{
    Config::Instance().Save();
}

const Options& GetOptions()
{
    if (!Config::Instance().GetConfiguration().options.defined)
    {
        Config::Instance().GetConfiguration().options.defined = true;
        Config::Instance().GetConfiguration().options.showStartupDialog = true;
        Config::Instance().GetConfiguration().options.numberOfRecentSolutions = 5;
        Config::Instance().GetConfiguration().options.codeCompletion = true;
        Config::Instance().GetConfiguration().options.parsingFrequency = 1000;
    }
    if (!Config::Instance().GetConfiguration().options.codeCompletionDefined)
    {
        Config::Instance().GetConfiguration().options.codeCompletionDefined = true;
        Config::Instance().GetConfiguration().options.codeCompletion = true;
        Config::Instance().GetConfiguration().options.parsingFrequency = 1000;
    }
    if (!Config::Instance().GetConfiguration().options.themeDefined)
    {
        Config::Instance().GetConfiguration().options.themeDefined = true;
        Config::Instance().GetConfiguration().options.theme = "light";
    }
    return Config::Instance().GetConfiguration().options;
}

void SetOptions(const Options& options)
{
    Config::Instance().GetConfiguration().options = options;
}

const BuildSettings& GetBuildSettings()
{
    return Config::Instance().GetConfiguration().buildSettings;
}

void SetBuildSettings(const BuildSettings& buildSettings)
{
    Config::Instance().GetConfiguration().buildSettings = buildSettings;
}

WindowSettings& GetWindowSettings()
{
    return Config::Instance().GetConfiguration().windowSettings;
}

const std::vector<RecentSolution>& GetRecentSolutions()
{
    std::vector<std::string> removeFilePaths;
    const std::vector<RecentSolution>& recentSolutions = Config::Instance().GetConfiguration().recentSolutions;
    for (const RecentSolution& recentSolution : recentSolutions)
    {
        if (!std::filesystem::exists(recentSolution.filePath))
        {
            removeFilePaths.push_back(recentSolution.filePath);
        }
    }
    for (const std::string& removeFilePath : removeFilePaths)
    {
        RemoveRecentSolution(removeFilePath);
    }
    return Config::Instance().GetConfiguration().recentSolutions;
}

void AddRecentSolution(const std::string& solutionName, const std::string& solutionFilePath)
{
    std::string filePath = util::GetFullPath(solutionFilePath);
    RemoveRecentSolution(filePath);
    const Options& options = GetOptions();
    std::vector<RecentSolution>& recentSolutions = Config::Instance().GetConfiguration().recentSolutions;
    while (recentSolutions.size() >= options.numberOfRecentSolutions)
    {
        recentSolutions.pop_back();
    }
    RecentSolution recentSolution;
    recentSolution.name = solutionName;
    recentSolution.filePath = filePath;
    recentSolutions.insert(recentSolutions.begin(), recentSolution);
}

void RemoveRecentSolution(const std::string& solutionFilePath)
{
    std::vector<RecentSolution>& recentSolutions = Config::Instance().GetConfiguration().recentSolutions;
    std::string filePath = util::GetFullPath(solutionFilePath);
    recentSolutions.erase(
        std::remove_if(
            recentSolutions.begin(), recentSolutions.end(), [&](const RecentSolution& solution) {  return solution.filePath == filePath; }), recentSolutions.end()); 
}

} // namespace cmcode
