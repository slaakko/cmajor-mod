// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.debug;

import std.core;

export namespace cmajor::systemx::kernel {

const int debugSystemErrorMode = 1 << 0;    // 1
const int debugTerminalMode = 1 << 1;       // 2
const int debugMsgQueueMode = 1 << 2;       // 4
const int debugShellMode = 1 << 3;          // 8
const int debugLexerMode = 1 << 4;          // 16
const int debugAppMode = 1 << 5;            // 32

void SetDebugMode(int debugMode);
int GetDebugMode();
void SetDebugLogPort(int port);
void DebugWrite(const std::string& debugMessage);
void StartDebug();
void StopDebug();
void InitDebug();
void DoneDebug();

} // namespace cmajor::systemx::kernel
