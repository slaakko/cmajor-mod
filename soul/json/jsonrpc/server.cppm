// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.server;

import util;
import std.core;

export namespace soul::json::rpc {

bool ProcessRequest(util::Stream& stream, bool& wait);

} // namespace soul::json::rpc
