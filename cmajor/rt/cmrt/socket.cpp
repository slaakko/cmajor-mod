// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/socket.hpp>
#include <cmrt/error.hpp>
#include <util/unicode.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>    
#include <Windows.h>
#undef min
#undef max

namespace cmajor::cpp::rt {

std::string GetSocketErrorMessage(int errorCode)
{
    char16_t buf[1024];
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, (LPWSTR)(&buf[0]), sizeof(buf) / 2, NULL);
    return util::ToUtf8(std::u16string(buf));
}

int GetLastSocketError()
{
    return WSAGetLastError();
}

int initError = 0;

void InitSocket()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(ver, &wsaData) != 0)
    {
        int errorCode = GetLastSocketError();
        std::string errorMessage = "socket initialization failed with error code " + std::to_string(errorCode) + ": " + GetSocketErrorMessage(errorCode);
        initError = AllocateError(errorMessage);
    }
}

void DoneSocket()
{
    WSACleanup();
}

struct SocketData
{
    SocketData(SOCKET socket_) : socket(socket_)
    {
    }
    SOCKET socket;
};

}

void* RtmCreateSocket(int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return nullptr;
    }
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return nullptr;
    }
    return new cmajor::cpp::rt::SocketData(s);
}

void RtmBindSocket(void* socketHandle, int32_t port, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);
    int result = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if (result != 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return;
    }
}

void RtmListenSocket(void* socketHandle, int32_t backlog, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    int result = listen(s, backlog);
    if (result != 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    }
}

void* RtmAcceptSocket(void* socketHandle, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return nullptr;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return nullptr;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return nullptr;
    }
    SOCKET a = accept(s, NULL, NULL);
    if (a == INVALID_SOCKET)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return nullptr;
    }
    void* acceptedSocketHandle = new cmajor::cpp::rt::SocketData(a);
    return acceptedSocketHandle;
}

void RtmCloseSocket(void* socketHandle, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    int result = closesocket(s);
    if (result != 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    }
}

void RtmShutdownSocket(void* socketHandle, ShutdownMode mode, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return;
    }
    int how = SD_RECEIVE;
    switch (mode)
    {
    case ShutdownMode::receive: how = SD_RECEIVE; break;
    case ShutdownMode::send: how = SD_SEND; break;
    case ShutdownMode::both: how = SD_BOTH; break;
    }
    int result = shutdown(s, how);
    if (result != 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    }
}

void* RtmConnectSocket(const char* node, const char* service, int32_t& errorId)
{
    errorId = 0;
    struct addrinfo h;
    struct addrinfo* rp;
    struct addrinfo* res;
    memset(&h, 0, sizeof(struct addrinfo));
    h.ai_flags = 0;
    h.ai_family = AF_INET;
    h.ai_socktype = SOCK_STREAM;
    h.ai_protocol = IPPROTO_TCP;
    h.ai_addrlen = 0;
    h.ai_addr = 0;
    h.ai_canonname = 0;
    h.ai_next = 0;
    int result = getaddrinfo(node, service, &h, &res);
    if (result != 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return nullptr;
    }
    else
    {
        for (rp = res; rp != 0; rp = rp->ai_next)
        {
            SOCKET s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (s == -1)
            {
                continue;
            }
            int result = connect(s, rp->ai_addr, (int)rp->ai_addrlen);
            if (result == 0)
            {
                freeaddrinfo(res);
                return new cmajor::cpp::rt::SocketData(s);
            }
            else
            {
                freeaddrinfo(res);
                int errorCode = cmajor::cpp::rt::GetLastSocketError();
                std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
                errorId = cmajor::cpp::rt::AllocateError(errorMessage);
                return nullptr;
            }
        }
    }
    std::string errorMessage = "could not connect";
    errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    return nullptr;
}

int32_t RtmSendSocket(void* socketHandle, uint8_t* buf, int32_t len, int32_t flags, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return -1;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return -1;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return -1;
    }
    int32_t result = send(s, (const char*)buf, len, flags);
    if (result < 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return -1;
    }
    return result;
}

int32_t RtmReceiveSocket(void* socketHandle, uint8_t* buf, int32_t len, int32_t flags, int32_t& errorId)
{
    errorId = 0;
    if (cmajor::cpp::rt::initError != 0)
    {
        errorId = cmajor::cpp::rt::initError;
        return -1;
    }
    if (!socketHandle)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return -1;
    }
    cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
    SOCKET s = socketData->socket;
    if (s == INVALID_SOCKET)
    {
        errorId = cmajor::cpp::rt::AllocateError("invalid socket handle");
        return -1;
    }
    int32_t result = recv(s, (char*)buf, len, flags);
    if (result < 0)
    {
        int errorCode = cmajor::cpp::rt::GetLastSocketError();
        std::string errorMessage = cmajor::cpp::rt::GetSocketErrorMessage(errorCode);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
        return -1;
    }
    return result;
}

void RtmDestroySocket(void* socketHandle)
{
    if (socketHandle)
    {
        cmajor::cpp::rt::SocketData* socketData = static_cast<cmajor::cpp::rt::SocketData*>(socketHandle);
        delete socketData;
    }
}
