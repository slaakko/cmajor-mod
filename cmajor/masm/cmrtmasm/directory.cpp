// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.directory;

import util;
import std.filesystem;

namespace cmajor::masm::rt {

struct Iteration
{
    std::filesystem::directory_iterator directoryIterator;
    std::string directoryName;
    std::string path;
};

class DirectoryIterationTable
{
public:
    static DirectoryIterationTable& Instance();
    int32_t BeginIterate(const char* directoryPath);
    const char* IterateFiles(int32_t handle);
    const char* IterateDirectories(int32_t handle);
    void EndIterate(int32_t handle);
private:
    DirectoryIterationTable();
    int32_t nextIterationHandle;
    std::map<int32_t, Iteration> iterationMap;
    std::mutex mtx;
};

DirectoryIterationTable::DirectoryIterationTable()
{
}

DirectoryIterationTable& DirectoryIterationTable::Instance()
{
    static DirectoryIterationTable instance;
    return instance;
}

int32_t DirectoryIterationTable::BeginIterate(const char* directoryPath)
{
    std::lock_guard<std::mutex> lock(mtx);
    int32_t handle = nextIterationHandle++;
    Iteration iteration;
    iteration.directoryName = util::GetFullPath(util::Path::MakeCanonical(directoryPath));
    iteration.directoryIterator = std::filesystem::directory_iterator(util::MakeNativePath(iteration.directoryName));
    iterationMap[handle] = iteration;
    return handle;
}

const char* DirectoryIterationTable::IterateFiles(int32_t handle)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = iterationMap.find(handle);
    if (it != iterationMap.cend())
    {
        Iteration& iteration = it->second;
        while (iteration.directoryIterator != std::filesystem::directory_iterator() && !std::filesystem::is_regular_file(*iteration.directoryIterator))
        {
            ++iteration.directoryIterator;
        }
        if (iteration.directoryIterator != std::filesystem::directory_iterator())
        {
            iteration.path = util::PlatformStringToUtf8(util::GetFullPath(util::Path::Combine(
                util::Utf8StringToPlatformString(iteration.directoryName),
                std::filesystem::path(*iteration.directoryIterator).generic_string())));
            ++iteration.directoryIterator;
            return iteration.path.c_str();
        }
        return nullptr;
    }
    else
    {
        return nullptr;
    }
}

const char* DirectoryIterationTable::IterateDirectories(int32_t handle)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = iterationMap.find(handle);
    if (it != iterationMap.cend())
    {
        Iteration& iteration = it->second;
        while (iteration.directoryIterator != std::filesystem::directory_iterator() &&
            (!std::filesystem::is_directory(*iteration.directoryIterator) || iteration.directoryIterator->path().filename() == "." ||
                iteration.directoryIterator->path().filename() == ".."))
        {
            ++iteration.directoryIterator;
        }
        if (iteration.directoryIterator != std::filesystem::directory_iterator())
        {
            iteration.path = util::PlatformStringToUtf8(util::GetFullPath(util::Path::Combine(
                util::Utf8StringToPlatformString(iteration.directoryName),
                std::filesystem::path(*iteration.directoryIterator).generic_string())));
            ++iteration.directoryIterator;
            return iteration.path.c_str();
        }
        return nullptr;
    }
    else
    {
        return nullptr;
    }
}

void DirectoryIterationTable::EndIterate(int32_t handle)
{
    std::lock_guard<std::mutex> lock(mtx);
    iterationMap.erase(handle);
}

} // cmajor::masm::rt

bool RtmDirectoryExists(const char* directoryPath)
{
    return std::filesystem::exists(directoryPath);
}

void RtmCreateDirectories(const char* directoryPath)
{
    std::filesystem::create_directories(directoryPath);
}

int32_t RtmBeginIterateDirectory(const char* directoryPath)
{
    return cmajor::masm::rt::DirectoryIterationTable::Instance().BeginIterate(directoryPath);
}

const char* RtmGetNextFilePath(int32_t directoryIterationHandle)
{
    return cmajor::masm::rt::DirectoryIterationTable::Instance().IterateFiles(directoryIterationHandle);
}

const char* RtmGetNextDirectoryPath(int32_t directoryIterationHandle)
{
    return cmajor::masm::rt::DirectoryIterationTable::Instance().IterateDirectories(directoryIterationHandle);
}

void RtmEndIterateDirectory(int32_t directoryIterationHandle)
{
    cmajor::masm::rt::DirectoryIterationTable::Instance().EndIterate(directoryIterationHandle);
}
