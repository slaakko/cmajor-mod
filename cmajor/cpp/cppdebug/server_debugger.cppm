// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.server.debugger;

import std.core;

export namespace cmajor::debug {

void StartDebuggerServer(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version,
    int port, bool log, bool idle);
void StopDebuggerServer();
void RunDebuggerServer(const std::string& executable, const std::vector<std::string>& args, bool verbose, bool breakOnThrow, const std::string& version,
    int port, bool log, bool idle);

} // namespace cmajor::debug
