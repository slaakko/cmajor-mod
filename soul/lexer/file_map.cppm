// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.file.map;

import std.core;

export namespace soul::lexer {

class FileMap
{
public:
    FileMap();
    int32_t MapFile(const std::string& filePath);
    void MapFile(const std::string& filePath, int32_t fileId);
    const std::string& GetFilePath(int32_t fileId) const;
    void AddFileContent(int32_t fileId, std::u32string&& fileContent, std::vector<int>&& lineStartIndeces);
    bool HasFileContent(int32_t fileId) const;
    void ReadFile(int32_t fileId);
    const std::pair<std::u32string, std::vector<int>>& GetFileContent(int32_t file) const;
    std::u32string GetFileLine(int32_t fileId, int line);
    int32_t NextFileId() const { return nextFileId; }
private:
    int32_t nextFileId;
    std::map<int32_t, std::string> filePathMap;
    std::map<int32_t, std::pair<std::u32string, std::vector<int>>> fileContentsMap;
};

FileMap* GetGlobalFileMap();
void SetGlobalFileMap(soul::lexer::FileMap* fileMap_);

} // namespace soul::lexer
