// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <string>

namespace util {

std::string GetPathToExecutable();
std::string GetPathToModule(void* moduleHandle);

} // util;

#endif // SYSTEM_HPP
