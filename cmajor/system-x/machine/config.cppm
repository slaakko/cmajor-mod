// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.config;

import std.core;

export namespace cmajor::systemx::machine {

int NumProcessors();
int NumLocalRegs();
int ClockTickMilliseconds();
int MaxProcs();
int MaxOpenFiles();
int MaxFilesystems();
int NumCachedBlocks();
int NumBlockHashQueues();
int NumCachedINodes();
int NumINodeHashQueues();
int KernelStackSize();
int RootFSNumBlocks();
int RootFSMaxFiles();
int32_t UID();
int32_t GID();
int32_t UMask();
std::string ConfigFilePath();

} // namespace cmajor::systemx::machine
