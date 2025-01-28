// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/lex.hpp>
#include <cmrt/error.hpp>
#include <util/path.hpp>
#include <util/system.hpp>
#include <util/binary_resource_ptr.hpp>
#include <util/memory_stream.hpp>
#include <util/binary_stream_reader.hpp>
#include <util/deflate_stream.hpp>
#include <map>

namespace cmajor::rt {

class ClassMaps
{
public:
    static ClassMaps& Instance();
    int* GetClassMap(const std::string& classMapName);
private:
    ClassMaps();
    std::map<std::string, int*> classMapMap;
};

ClassMaps::ClassMaps()
{
}

ClassMaps& ClassMaps::Instance()
{
    static ClassMaps instance;
    return instance;
}

int* ClassMaps::GetClassMap(const std::string& classMapName)
{
    auto it = classMapMap.find(classMapName);
    if (it != classMapMap.end())
    {
        return it->second;
    }
    else
    {
        std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
        util::BinaryResourcePtr resource(currentExecutableName, classMapName);
        util::MemoryStream memoryStream(resource.Data(), resource.Size());
        util::BinaryStreamReader rawReader(memoryStream);
        int32_t size = rawReader.ReadInt();
        int* data = new int[size];
        util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
        util::BinaryStreamReader reader(compressedStream);
        for (int32_t i = 0; i < size; ++i)
        {
            int x = reader.ReadInt();
            data[i] = x;
        }
        classMapMap[classMapName] = data;
        return data;
    }
}

} // cmajor::rt

int* RtmGetClassMap(const char* classMapName, int32_t& errorId)
{
    try
    {
        return cmajor::rt::ClassMaps::Instance().GetClassMap(classMapName);
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return nullptr;
    }
}

