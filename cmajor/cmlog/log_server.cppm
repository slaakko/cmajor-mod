// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.log.log.server;

import std.core;

export namespace cmajor::log {

void SetLogServerPort(int port);
void StartLogServer();
void StopLogServer();
void InitLogServer();
void DoneLogServer();

} // namespace cmajor::log
