// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.system.file.index;

import std.core;

export namespace cmajor::ast {

const uint32_t firstSystemFileIndex = 1000000;

class SystemFileIndex
{
public:
    static SystemFileIndex& Instance();
    uint32_t RegisterSystemSourceFile(const std::string& systemSourceFilePath);
    std::string GetSystemSourceFilePath(uint32_t systemFileIndex) const;
    void Write(const std::string& systemFileIndexFilePath);
    void Read(const std::string& systemFileIndexFilePath);
private:
    uint32_t nextSystemFileIndex;
    std::string cmajorRootDir;
    std::map<uint32_t, std::string> systemFileMap;
    SystemFileIndex();
};

} // namespace cmajor::ast
