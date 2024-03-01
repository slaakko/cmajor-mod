// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module bpm.server;

import std.core;

export const int leaseExpirationTimeSecs = 120;
export const int leaseRenewalTimeSecs = 60;

export namespace bpm {

std::string BinaryPortMapConfigFilePath();
void RunPortMapServer();

} // namespace bpm
