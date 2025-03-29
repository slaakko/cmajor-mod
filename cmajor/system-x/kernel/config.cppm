// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.config;

import std.core;

export namespace cmajor::systemx::kernel {

class Config
{
public:
    static Config& Instance();
    bool IncludedInMountRootDirs(const std::string& rootDir) const;
private:
    Config();
    void Write();
    void Read();
    std::string filePath;
    std::set<std::string> mountRootDirs;
};

} // namespace cmajor::systemx::kernel
