// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.classmap;

import std.core;
import util;

export namespace soul::lexer {

using namespace util;

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

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& classMapName)
{
    std::string currentExecutableName = Path::GetFileName(GetFullPath(GetPathToExecutable()));
    BinaryResourcePtr resource(currentExecutableName, classMapName);
    MemoryStream memoryStream(resource.Data(), resource.Size());
    BinaryStreamReader rawReader(memoryStream);
    int32_t size = rawReader.ReadInt();
    int32_t* data = new int32_t[size];
    DeflateStream compressedStream(CompressionMode::decompress, memoryStream);
    BinaryStreamReader reader(compressedStream);
    for (int64_t i = 0; i < size; ++i)
    {
        int32_t x = reader.ReadInt();
        data[i] = x;
    }
    ClassMap<Char>* classMap = new ClassMap<Char>(data, size);
    return classMap;
}

} // soul::lexer
