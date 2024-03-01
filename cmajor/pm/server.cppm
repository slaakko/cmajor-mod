// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.portmap.server;

import std.core;

export namespace cmajor::portmap {

const int leaseExpirationTimeSecs = 120;
const int leaseRenewalTimeSecs = 60;

void RunPortMapServer();

} // namespace cmajor::cmpm
