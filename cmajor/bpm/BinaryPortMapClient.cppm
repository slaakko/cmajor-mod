// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module bpm.client;

import std.core;

export namespace bpm {

void StartPortMapClient(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid);
void StopPortMapClient();

} // namespace bpm