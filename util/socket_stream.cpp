// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.socket_stream;

namespace util {

SocketStream::SocketStream(TcpSocket&& socket_) : Stream(), socket(std::move(socket_))
{
}

int SocketStream::ReadByte()
{
    uint8_t x;
    int64_t bytesRead = Read(&x, 1);
    if (bytesRead > 0)
    {
        return x;
    }
    else
    {
        return -1;
    }
}

int64_t SocketStream::Read(uint8_t* buf, int64_t count) 
{
    int bytesRead = socket.Receive(buf, static_cast<int>(count));
    return bytesRead;
}

void SocketStream::Write(uint8_t x)
{
    Write(&x, 1);
}

void SocketStream::Write(uint8_t* buf, int64_t count)
{
    socket.Send(buf, static_cast<int>(count));
}

} // util
