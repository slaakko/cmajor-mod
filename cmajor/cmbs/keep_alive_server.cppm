// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmbs.keep.alive.server;

import std.core;

export namespace cmbs {

const int keepAliveSecs = 60;
const int timeoutSecs = 120;

void StartKeepAliveServer(int keepAliveServerPort, std::condition_variable* exitVar, bool* exiting, bool logging);
void StopKeepAliveServer();
bool Timeout();

} // namespace cmbs
