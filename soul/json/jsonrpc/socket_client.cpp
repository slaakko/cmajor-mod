// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.socket_client;

import soul.json.rpc.client;

namespace soul::json::rpc {

std::unique_ptr<util::JsonValue> SendReceive(int port, util::JsonValue* request)
{
    util::TcpSocket socket("127.0.0.1", std::to_string(port));
    util::SocketStream socketStream(std::move(socket));
    util::BufferedStream bufferedStream(socketStream);
    return Transact(request, bufferedStream);
}

} // namespace soul::json::rpc
