// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.process;

import std.core;

export {

extern "C" void* RtmCreateProcess(const char* command, int32_t redirections, int32_t& errorId);
extern "C" void RtmDeleteProcess(void* process);
extern "C" bool RtmProcessRunning(void* process, int32_t& errorId);
extern "C" bool RtmProcessWaitForExit(void* process, int32_t& errorId);
extern "C" int RtmProcessExitCode(void* process, int32_t& errorId);
extern "C" bool RtmProcessTerminate(void* process, int32_t& errorId);
extern "C" bool RtmProcessEof(void* process, int handle, int32_t& errorId);
extern "C" int32_t RtmProcessReadLine(void* process, int handle, int32_t& errorId);
extern "C" int32_t RtmProcessReadToEnd(void* process, int handle, int32_t& errorId);
extern "C" bool RtmProcessWriteLine(void* process, const char* line, int32_t& errorId);

}


