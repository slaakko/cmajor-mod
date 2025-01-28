// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.socket_server;

import soul.json.rpc.server;
import util;

namespace soul::json::rpc {

void RunServer(int port, bool wait)
{
    try
    {
        util::TcpSocket socket;
        socket.Bind(port);
        socket.Listen(10);
        bool exiting = false;
        while (!exiting)
        {
            util::TcpSocket clientSocket = socket.Accept();
            if (exiting) break;
            util::SocketStream socketStream(std::move(clientSocket));
            util::BufferedStream bufferedStream(socketStream);
            while (!exiting)
            {
                exiting = soul::json::rpc::ProcessRequest(bufferedStream, wait);
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
    }
}

} // namespace soul::json::rpc
