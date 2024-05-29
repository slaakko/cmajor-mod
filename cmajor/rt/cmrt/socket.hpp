// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_SOCKET_INCLUDED
#define CMRT_CPP_SOCKET_INCLUDED
#include <stdint.h>

namespace cmajor::rt {

void InitSocket();
void DoneSocket();

}

enum class ShutdownMode : int32_t
{
    receive = 0, send = 1, both = 2
};

extern "C" void* RtmCreateSocket(int32_t & errorId);
extern "C" void RtmBindSocket(void* socketHandle, int32_t port, int32_t & errorId);
extern "C" void RtmListenSocket(void* socketHandle, int32_t backlog, int32_t & errorId);
extern "C" void* RtmAcceptSocket(void* socketHandle, int32_t & errorId);
extern "C" void RtmCloseSocket(void* socketHandle, int32_t & errorId);
extern "C" void RtmShutdownSocket(void* socketHandle, ShutdownMode mode, int32_t & errorId);
extern "C" void* RtmConnectSocket(const char* node, const char* service, int32_t & errorId);
extern "C" int32_t RtmSendSocket(void* socketHandle, uint8_t * buf, int32_t len, int32_t flags, int32_t & errorId);
extern "C" int32_t RtmReceiveSocket(void* socketHandle, uint8_t * buf, int32_t len, int32_t flags, int32_t & errorId);
extern "C" void RtmDestroySocket(void* socketHandle);

#endif // CMRT_CPP_SOCKET_INCLUDED
