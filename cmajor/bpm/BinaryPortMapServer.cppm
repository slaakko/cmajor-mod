// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module bpm.server;

import std.core;

export const int leaseExpirationTimeSecs = 120;
export const int leaseRenewalTimeSecs = 60;

export namespace bpm {

void RunPortMapServer();

} // namespace bpm
