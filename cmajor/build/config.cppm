// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.config;

import std.core;

export namespace cmajor::build {

class BuildConfig
{
public:
    static BuildConfig& Instance();
    const std::string& ClangXXPath() const { return clangxxPath; }
    const std::string& GXXPath() const { return gxxPath; }
    const std::string& GDBPath() const { return gdbPath; }
    const std::string& ARPath() const { return arPath; }
private:
    BuildConfig();
    std::string filePath;
    std::string clangxxPath;
    std::string gxxPath;
    std::string gdbPath;
    std::string arPath;
};

const std::string& GetClangXXPathFromBuildConfig();
const std::string& GetGXXPathFromBuildConfig();
const std::string& GetGDBPathFromBuildConfig();
const std::string& GetARPathFromBuildConfig();

} // namespace cmajor::build
