// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.input;

import soul.xml.dom;
import std.core;

export namespace cmdoclib {

struct Input
{
    Input();
    std::string baseDir;
    std::string solutionFilePath;
    std::string docFilePath;
    std::string relativeSolutionFilePath;
    std::string targetDirPath;
    std::vector<std::string> libraryDirs;
    std::map<std::string, std::string> libraryPrefixMap;
    std::vector<std::unique_ptr<soul::xml::Document>> externalModuleDocs;
    std::unique_ptr<soul::xml::Document> docs;
    int errors;
};

Input ReadInput(const std::string& cmdocXmlFilePath);
Input* GetInputPtr();
void SetInputPtr(Input* input);
void SetEmptyLibraryPrefix(const std::string& moduleName);

} // namespace cmdoclib
