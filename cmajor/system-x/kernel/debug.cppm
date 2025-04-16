// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.debug;

import std.core;

export namespace cmajor::systemx::kernel {

const int debugSystemErrorMode = 1 << 0;    // 1
const int debugStateMode = 1 << 1;          // 2
const int debugLockingMode = 1 << 2;        // 4
const int debugTerminalMode = 1 << 3;       // 8
const int debugMsgQueueMode = 1 << 4;       // 16
const int debugShellMode = 1 << 5;          // 32
const int debugLexerMode = 1 << 6;          // 64
const int debugAppMode = 1 << 7;            // 128
const int debugAllMode =                    // 255
    debugSystemErrorMode |
    debugStateMode |
    debugLockingMode |
    debugTerminalMode |
    debugMsgQueueMode |
    debugShellMode |
    debugLexerMode |
    debugAppMode;

void SetDebugMode(int debugMode);
int GetDebugMode();
bool InDebugMode(int debugMode);
void SetDebugLogPort(int port);
void DebugWrite(const std::string& debugMessage);
void StartDebug();
void StopDebug();
void InitDebug();
void DoneDebug();

} // namespace cmajor::systemx::kernel
