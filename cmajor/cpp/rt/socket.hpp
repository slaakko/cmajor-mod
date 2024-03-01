// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <rt_api.hpp>
#include <stdint.h>

enum class ShutdownMode : int32_t
{
    receive = 0, send = 1, both = 2
};

enum class ConnectOptions : int32_t
{
    none = 0, useTls = 1 << 0
};

inline ConnectOptions operator&(ConnectOptions left, ConnectOptions right)
{
    return ConnectOptions(int32_t(left) & int32_t(right));
}

inline ConnectOptions operator|(ConnectOptions left, ConnectOptions right)
{
    return ConnectOptions(int32_t(left) | int32_t(right));
}

extern "C" RT_API void* RtCreateSocket(int32_t & errorStringHandle);
extern "C" RT_API bool RtBindSocket(void* socketHandle, int32_t port, int32_t & errorStringHandle);
extern "C" RT_API bool RtListenSocket(void* socketHandle, int32_t backLog, int32_t & errorStringHandle);
extern "C" RT_API void* RtAcceptSocket(void* socketHandle, int32_t & errorStringHandle);
extern "C" RT_API bool RtCloseSocket(void* socketHandle, int32_t & errorStringHandle);
extern "C" RT_API bool RtShutdownSocket(void* socketHandle, ShutdownMode mode, int32_t & errorStringHandle);
extern "C" RT_API void* RtConnectSocket(const char* node, const char* service, ConnectOptions options, int32_t & errorStringHandle);
extern "C" RT_API int32_t RtSendSocket(void* socketHandle, uint8_t * buf, int32_t len, int32_t flags, int32_t & errorStringHandle);
extern "C" RT_API int32_t RtReceiveSocket(void* socketHandle, uint8_t * buf, int32_t len, int32_t flags, int32_t & errorStringHandle);
extern "C" RT_API void RtDestroySocket(void* socketHandle);

namespace cmajor::rt {

void InitSocket();
void DoneSocket();

}  // namespace cmajor::rt

#endif // SOCKET_HPP
