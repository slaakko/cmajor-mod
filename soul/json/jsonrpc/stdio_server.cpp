// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.stdio_server;

import soul.json.rpc.server;
import util;

namespace soul::json::rpc {

void RunStdIOServer(bool wait)
{
    try
    {
        util::StdIOStream stream;
        bool exiting = false;
        while (!exiting)
        {
            exiting = soul::json::rpc::ProcessRequest(stream, wait);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
    }
}

} // namespace soul::json::rpc
