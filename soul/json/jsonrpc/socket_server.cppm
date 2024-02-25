// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.socket_server;

import std.core;

export namespace soul::json::rpc {

void RunServer(int port, bool wait);

} // namespace soul::json::rpc
