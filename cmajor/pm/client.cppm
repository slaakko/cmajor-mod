// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.portmap.client;

import std.core;

export namespace cmajor::portmap {

void StartPortMapClient(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid);
void StopPortMapClient();

} // namespace cmajor::cmpm
