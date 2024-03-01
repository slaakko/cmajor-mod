// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.kill.channel;

import std.core;

export namespace cmajor::debug {

void StartKillChannel(int port);
void StopKillChannel();
void InitKillChannel();
void DoneKillChannel();

} // namespace cmajor::debug
