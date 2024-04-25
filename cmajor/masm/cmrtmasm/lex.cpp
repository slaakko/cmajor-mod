// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.lex;

import cmajor.masm.rt.error;
import util;

namespace cmajor::masm::rt {

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

} // cmajor::masm::rt

int* RtmGetClassMap(const char* classMapName, int32_t& errorId)
{
    try
    {
        return cmajor::masm::rt::ClassMaps::Instance().GetClassMap(classMapName);
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::masm::rt::AllocateError(ex.what());
        return nullptr;
    }
}

