// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.socket_client;

import util;
import std.core;

export namespace soul::json::rpc {

std::unique_ptr<util::JsonValue> SendReceive(int port, util::JsonValue* request);

} // namespace soul::json::rpc
