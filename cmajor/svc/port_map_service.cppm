// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.port.map.service;

import cmajor.binary.portmap.message;
import std.core;

export namespace cmajor::service {

int GetPortMapServicePortNumberFromConfig();
bool StartPortMapServer();
bool StopPortMapServer();
int GetPortMapServicePortNumber(bool& portMapServerStarted);
int GetFreePortNumber(const std::string& processName);
bool IsPortMapServerRunning();
std::vector<bpm::PortLease> GetPortLeases();

} // namespace cmajor::service
