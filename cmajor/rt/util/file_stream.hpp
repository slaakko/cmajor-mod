// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef FILE_STREAM_HPP
#define FILE_STREAM_HPP
#include <util/stream.hpp>
#include <stdint.h>
#include <string>

namespace util {

enum class OpenMode : int
{
    none = 0u,
    read = 1u << 0u,
    write = 1u << 1u,
    append = 1u << 2u,
    binary = 1u << 3u
};

inline constexpr OpenMode operator|(OpenMode left, OpenMode right)
{
    return OpenMode(int(left) | int(right));
}

inline constexpr OpenMode operator&(OpenMode left, OpenMode right)
{
    return OpenMode(int(left) & int(right));
}

inline constexpr OpenMode operator~(OpenMode mode)
{
    return OpenMode(~int(mode));
}

class FileStream : public Stream
{
public:
    FileStream(int handle);
    FileStream(const std::string& filePath_, OpenMode openMode);
    ~FileStream() override;
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
    void Flush() override;
    void Seek(int64_t pos, Origin origin) override;
    int64_t Tell() override;
    int64_t Size() const;
private:
    std::string filePath;
    FILE* file;
    bool needToClose;
};

std::string ReadFile(const std::string& filePath);

} // util

#endif // FILE_STREAM_HPP
