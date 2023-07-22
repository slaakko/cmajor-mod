// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.classmap;

import std.core;
import util;

export namespace soul::lexer {

template<typename Char>
class ClassMap
{
public:
    ClassMap(const int32_t* data_, int32_t upperBound_) : data(data_), upperBound(upperBound_) { }
    int32_t GetClass(Char c) const
    {
        int32_t i = static_cast<int32_t>(c);
        if (i < upperBound)
        {
            return data[i];
        }
        else
        {
            return -1;
        }
    }
private:
    const int32_t* data;
    int32_t upperBound;
};

#ifndef OTAVA

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& classMapName)
{
    std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
    util::BinaryResourcePtr resource(currentExecutableName, classMapName);
    util::MemoryStream memoryStream(resource.Data(), resource.Size());
    util::BinaryStreamReader rawReader(memoryStream);
    int32_t size = rawReader.ReadInt();
    int32_t* data = new int32_t[size];
    util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
    util::BinaryStreamReader reader(compressedStream);
    for (int64_t i = 0; i < size; ++i)
    {
        int32_t x = reader.ReadInt();
        data[i] = x;
    }
    ClassMap<Char>* classMap = new ClassMap<Char>(data, size);
    return classMap;
}

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& moduleFileName, const std::string& classMapName, util::ResourceFlags resourceFlags)
{
    util::BinaryResourcePtr resource(moduleFileName, classMapName, resourceFlags);
    util::MemoryStream memoryStream(resource.Data(), resource.Size());
    util::BinaryStreamReader rawReader(memoryStream);
    int32_t size = rawReader.ReadInt();
    int32_t* data = new int32_t[size];
    util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
    util::BinaryStreamReader reader(compressedStream);
    for (int64_t i = 0; i < size; ++i)
    {
        int32_t x = reader.ReadInt();
        data[i] = x;
    }
    ClassMap<Char>* classMap = new ClassMap<Char>(data, size);
    return classMap;
}

#else

extern "C" int32_t* read_lexer_resource(const char* resource_name, int32_t& size);
extern "C" void free_lexer_resource(int32_t* lexer_resource);

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& classMapName)
{
    int32_t size = 0;
    int32_t* data = read_lexer_resource(classMapName.c_str(), size);
    ClassMap<Char>* cm = new ClassMap<Char>(data, size);
    return cm;
}

#endif

} // soul::lexer
