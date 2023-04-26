// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.file.map;

import util;

namespace soul::lexer {

int FileMap::AddFilePath(const std::string& filePath)
{
    int file = filePaths.size();
    filePaths.push_back(filePath);
    return file;
}

const std::string& FileMap::GetFilePath(int file) const
{
    if (file >= 0 && file < filePaths.size())
    {
        return filePaths[file];
    }
    else
    {
        throw std::runtime_error("file path for file number " + std::to_string(file) + " not found from file map");
    }
}

void FileMap::AddFileContent(std::u32string&& fileContent, std::vector<int>&& lineStartIndeces)
{
    fileContents.push_back(std::make_pair(std::move(fileContent), std::move(lineStartIndeces)));
}

bool FileMap::HasFileContent(int file) const
{
    return file >= 0 && file < fileContents.size();
}

const std::pair<std::u32string, std::vector<int>>& FileMap::GetFileContent(int file) const
{
    if (file >= 0 && file < fileContents.size())
    {
        return fileContents[file];
    }
    else
    {
        throw std::runtime_error("file contents for file number " + std::to_string(file) + " not found from file map");
    }
}

std::u32string FileMap::GetFileLine(int file, int line) const
{
    const std::pair<std::u32string, std::vector<int>>& contents = GetFileContent(file);
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
