// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.socket_stream;

import util.stream;
import util.socket;
import std.core;

export namespace util {

class SocketStream : public Stream
{
public:
    SocketStream(TcpSocket&& socket_);
    const TcpSocket& Socket() const { return socket; }
    TcpSocket& Socket() { return socket; }
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
private:
    TcpSocket socket;
};

} // util
