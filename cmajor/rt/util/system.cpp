// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util/system.hpp>
#include <stdexcept>
#ifdef _WIN32
#include <Windows.h>
#endif 

namespace util {

std::string GetPathToExecutable()
{
    char buf[4096];
    DWORD result = GetModuleFileNameA(NULL, buf, sizeof(buf));
    if (result == 0)
    {
        throw std::runtime_error("could not get path to current executable: GetModuleFileName failed");
    }
    return std::string(buf);
}

std::string GetPathToModule(void* moduleHandle)
{
    char buf[4096];
    DWORD result = GetModuleFileNameA((HMODULE)moduleHandle, buf, sizeof(buf));
    if (result == 0)
    {
        throw std::runtime_error("could not get path to current module: GetModuleFileName failed");
    }
    return std::string(buf);
}

} // util