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
    int AddFilePath(const std::string& filePath);
    const std::string& GetFilePath(int file) const;
    void AddFileContent(std::u32string&& fileContent, std::vector<int>&& lineStartIndeces);
    bool HasFileContent(int file) const;
    const std::pair<std::u32string, std::vector<int>>& GetFileContent(int file) const;
    std::u32string GetFileLine(int file, int line) const;
private:
    std::vector<std::string> filePaths;
    std::vector<std::pair<std::u32string, std::vector<int>>> fileContents;
};

FileMap* GetGlobalFileMap();
void SetGlobalFileMap(soul::lexer::FileMap* fileMap_);

} // namespace soul::lexer
