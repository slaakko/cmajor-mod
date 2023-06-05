// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.rt.directory;

import util;
import std.filesystem;

namespace cmajor::rt {

struct Iteration
{
    std::filesystem::directory_iterator directoryIterator;
    std::string directoryName;
    std::string path;
};

class DirectoryIterationTable
{
public:
    static void Init();
    static void Done();
    static DirectoryIterationTable& Instance() { return *instance; }
    int32_t BeginIterate(const char* directoryPath);
    const char* IterateFiles(int32_t handle);
    const char* IterateDirectories(int32_t handle);
    void EndIterate(int32_t handle);
private:
    static std::unique_ptr<DirectoryIterationTable> instance;
    DirectoryIterationTable();
    int32_t nextIterationHandle;
    std::unordered_map<int32_t, Iteration> iterationMap;
    std::mutex mtx;
};

std::unique_ptr<DirectoryIterationTable> DirectoryIterationTable::instance;

void DirectoryIterationTable::Init()
{
    instance.reset(new DirectoryIterationTable());
}

void DirectoryIterationTable::Done()
{
    instance.reset();
}

DirectoryIterationTable::DirectoryIterationTable() : nextIterationHandle(0)
{
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

void DirectoryIterationTable::EndIterate(int32_t handle)
{
    std::lock_guard<std::mutex> lock(mtx);
    iterationMap.erase(handle);
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

void InitDirectory()
{
    DirectoryIterationTable::Init();
}

void DoneDirectory()
{
    DirectoryIterationTable::Done();
}

} // namespace cmajor::rt

bool RtDirectoryExists(const char* directoryPath)
{
    return std::filesystem::exists(directoryPath);
}

void RtCreateDirectories(const char* directoryPath)
{
    std::filesystem::create_directories(directoryPath);
}

int32_t RtBeginIterateDirectory(const char* directoryPath)
{
    return cmajor::rt::DirectoryIterationTable::Instance().BeginIterate(directoryPath);
}

const char* RtGetNextFilePath(int32_t directoryIterationHandle)
{
    return cmajor::rt::DirectoryIterationTable::Instance().IterateFiles(directoryIterationHandle);
}

const char* RtGetNextDirectoryPath(int32_t directoryIterationHandle)
{
    return cmajor::rt::DirectoryIterationTable::Instance().IterateDirectories(directoryIterationHandle);
}

void RtEndIterateDirectory(int32_t directoryIterationHandle)
{
    cmajor::rt::DirectoryIterationTable::Instance().EndIterate(directoryIterationHandle);
}
