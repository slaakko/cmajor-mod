// =================================
// Copyright (c) 2022 Seppo Laakko
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
    int64_t size = rawReader.ReadLong();
    int32_t upperBound = rawReader.ReadInt();
    int8_t* bytes = static_cast<int8_t*>(malloc(size));
    DeflateStream compressedStream(CompressionMode::decompress, memoryStream);
    BufferedStream bufferedStream(compressedStream);
    BinaryStreamReader reader(bufferedStream);
    for (int64_t i = 0; i < size; ++i)
    {
        uint8_t x = reader.ReadByte();
        bytes[i] = x;
    }
    int32_t* data = static_cast<int32_t*>(static_cast<void*>(bytes));
    ClassMap<Char>* classMap = new ClassMap<Char>(data, upperBound);
    return classMap;
}

} // soul::lexer
