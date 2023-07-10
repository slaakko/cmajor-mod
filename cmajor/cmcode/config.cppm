// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.config;

import cmcode.configuration;
import cmcode.project.settings;
import cmcode.solution.settings;
import std.core;

export namespace cmcode {

const int configurationSavePeriod = 3000;
const int configurationSaveTimerId = 11;

bool UseDebugServers();
bool ServerLogging();
bool DebugWait();
bool ServerVerbose();
void LoadConfiguration();
void SaveConfiguration();
const Options& GetOptions();
void SetOptions(const Options& options);
const BuildSettings& GetBuildSettings();
void SetBuildSettings(const BuildSettings& buildSettings);
WindowSettings& GetWindowSettings();
const std::vector<RecentSolution>& GetRecentSolutions();
void AddRecentSolution(const std::string& solutionName, const std::string& solutionFilePath);
void RemoveRecentSolution(const std::string& solutionFilePath);

} // namespace cmcode
