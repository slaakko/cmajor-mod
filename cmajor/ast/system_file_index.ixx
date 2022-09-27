export module cmajor.ast.system.file.index;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

/*
#include <soulng/util/Error.hpp>
#include <map>
#include <memory>
#include <stdint.h>
*/
import std.core;

export namespace cmajor::ast {

const uint32_t firstSystemFileIndex = 1000000;

class SystemFileIndex
{
public:
    static void Init();
    // TODO
    static SystemFileIndex& Instance() { /*Assert(instance, "system file index not initialized");*/  return *instance; }
    uint32_t RegisterSystemSourceFile(const std::string& systemSourceFilePath);
    std::string GetSystemSourceFilePath(uint32_t systemFileIndex) const;
    void Write(const std::string& systemFileIndexFilePath);
    void Read(const std::string& systemFileIndexFilePath);
private:
    static std::unique_ptr<SystemFileIndex> instance;
    uint32_t nextSystemFileIndex;
    std::string cmajorRootDir;
    std::map<uint32_t, std::string> systemFileMap;
    SystemFileIndex();
};

void SystemFileIndexInit();
} // namespace cmajor::ast