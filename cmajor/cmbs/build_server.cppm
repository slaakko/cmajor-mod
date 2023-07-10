// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmbs.build.server;

import std.core;

export namespace cmbs {

void StartBuildServer(int port, std::condition_variable* exitVar, bool* exiting, bool logging, bool progress);
void StopBuildServer();
bool BuildServerStopRequested();

} // namespace cmbs
