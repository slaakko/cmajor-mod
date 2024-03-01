// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.system;

import std.core;

export namespace util {

std::string GetPathToExecutable();
std::string GetPathToModule(void* moduleHandle);

} // util;
