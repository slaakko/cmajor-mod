// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_MEMORY_STREAM_HPP
#define UTIL_MEMORY_STREAM_HPP
#include <util/stream.hpp>

namespace util {

class MemoryStream : public Stream
{
public:
    MemoryStream();
    MemoryStream(uint8_t* data_, int64_t size_);
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
    void Seek(int64_t pos, Origin origin) override;
    int64_t Tell() override;
    uint8_t* Data() { return data; }
    int64_t Size() const { return size; }
    int64_t ReadPos() const { return readPos; }
    const std::vector<uint8_t>& Content() const { return content; }
    void SetFromContent();
private:
    uint8_t* data;
    int64_t size;
    int64_t readPos;
    std::vector<uint8_t> content;
};

} // namespace util

#endif // UTIL_MEMORY_STREAM_HPP
