// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef BUFFERED_STREAM_HPP
#define BUFFERED_STREAM_HPP
#include <util/stream.hpp>

namespace util {

class BufferedStream : public Stream
{
public:
    BufferedStream(Stream& baseStream_);
    BufferedStream(Stream& baseStream_, int64_t bufferSize_);
    ~BufferedStream() override;
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
    void Flush() override;
    void Seek(int64_t pos, Origin origin) override;
    int64_t Tell() override;
    Stream& BaseStream() { return baseStream; }
private:
    void FillBuf();
    Stream& baseStream;
    int64_t bufferSize;
    std::unique_ptr<uint8_t[]> buffer;
    int64_t pos;
    int64_t bytesAvailable;
    int64_t end;
};

} // namespace util

#endif // BUFFERED_STREAM_HPP
