// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.file.map;

import util;

namespace soul::lexer {

FileMap::FileMap() : nextFileId(0)
{
}

int32_t FileMap::MapFile(const std::string& filePath)
{
    int32_t fileId = nextFileId++;
    MapFile(filePath, fileId);
    return fileId;
}

void FileMap::MapFile(const std::string& filePath, int32_t fileId)
{
    filePathMap[fileId] = filePath;
}

const std::string& FileMap::GetFilePath(int32_t fileId) const
{
    auto it = filePathMap.find(fileId);
    if (it != filePathMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("file path for file id " + std::to_string(fileId) + " not found from file map");
    }
}

void FileMap::AddFileContent(int32_t fileId, std::u32string&& fileContent, std::vector<int>&& lineStartIndeces)
{
    fileContentsMap[fileId] = std::make_pair(std::move(fileContent), std::move(lineStartIndeces));
}

bool FileMap::HasFileContent(int32_t fileId) const
{
    return fileContentsMap.find(fileId) != fileContentsMap.end();
}

const std::pair<std::u32string, std::vector<int>>& FileMap::GetFileContent(int32_t fileId) const
{
    auto it = fileContentsMap.find(fileId);
    if (it != fileContentsMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("file contents for file id " + std::to_string(fileId) + " not found from file map");
    }
}

std::vector<int> ComputeLineStartIndeces(const std::u32string& content)
{
    std::vector<int> indeces;
    indeces.push_back(0);
    const char32_t* start = content.c_str();
    const char32_t* end = content.c_str() + content.size();
    const char32_t* pos = start;
    bool startOfLine = true;
    while (pos != end)
    {
        if (startOfLine)
        {
            indeces.push_back(static_cast<int>(pos - start));
        }
        startOfLine = *pos == '\n';
        ++pos;
    }
    indeces.push_back(static_cast<int>(end - start));
    return indeces;
}

void FileMap::ReadFile(int32_t fileId) 
{
    std::string filePath = GetFilePath(fileId);
    std::string fileContent = util::ReadFile(filePath);
    std::u32string ucontent = util::ToUtf32(fileContent);
    std::vector<int> lineStartIndeces = ComputeLineStartIndeces(ucontent);
    AddFileContent(fileId, std::move(ucontent), std::move(lineStartIndeces));
}

std::u32string FileMap::GetFileLine(int32_t fileId, int line) 
{
    if (fileId == -1) return std::u32string();
    if (!HasFileContent(fileId))
    {
        ReadFile(fileId);
    }
    const std::pair<std::u32string, std::vector<int>>& contents = GetFileContent(fileId);
    std::u32string::size_type lineStart = contents.second[line];
    std::u32string::size_type lineLength = std::u32string::npos;
    if (line < contents.second.size() - 1)
    {
        lineLength = contents.second[line + 1] - lineStart;
    }
    std::u32string lineStr = contents.first.substr(lineStart, lineLength);
    int n = lineStr.length();
    int i = n - 1;
    while (i >= 0)
    {
        if (lineStr[i] != '\r' && lineStr[i] != '\n')
        {
            break;
        }
        --i;
    }
    std::u32string trimmedLine = lineStr.substr(0, i + 1);
    return trimmedLine;
}

std::mutex tokenMapMutex;

void FileMap::SetTokens(int32_t fileId, TokenVec&& tokens)
{
    std::lock_guard<std::mutex> lock(tokenMapMutex);
    tokenMap[fileId] = std::move(tokens);
}

const TokenVec& FileMap::GetTokens(int32_t fileId) const
{
    std::lock_guard<std::mutex> lock(tokenMapMutex);
    static TokenVec empty;
    auto it = tokenMap.find(fileId);
    if (it != tokenMap.cend())
    {
        return it->second;
    }
    return empty;
}

FileMap* globalFileMap = nullptr;

FileMap* GetGlobalFileMap()
{
    return globalFileMap;
}

void SetGlobalFileMap(FileMap* fileMap)
{
    globalFileMap = fileMap;
}

} // namespace soul::lexer
