// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.binary.resource.ptr;

import util.unicode;
import util.error;

namespace util {

BinaryResourcePtr::BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_) : BinaryResourcePtr(moduleName, resourceName_, util::ResourceFlags::none)
{
}

BinaryResourcePtr::BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_, util::ResourceFlags flags) : 
    resourceName(resourceName_), size(0)
{
    std::u16string moduleNameStr = ToUtf16(moduleName);
    std::u16string resourceNameStr = ToUtf16(resourceName);
    HMODULE moduleHandle = nullptr;
    if ((flags & ResourceFlags::loadLibraryAsDataFile) != ResourceFlags::none)
    {
        moduleHandle = LoadLibraryExW((LPCWSTR)moduleNameStr.c_str(), 0, LOAD_LIBRARY_AS_DATAFILE);
    }
    else
    {
        moduleHandle = GetModuleHandleW((LPCWSTR)moduleNameStr.c_str());
    }
    if (!moduleHandle)
    {
        WindowsException windowsException(GetLastError());
        throw std::runtime_error("error: error getting resource '" + resourceName + "' from module '" + moduleName + "': could not get module handle: " +
            windowsException.ErrorMessage());
    }
    HRSRC res = FindResourceW(moduleHandle, (LPCWSTR)resourceNameStr.c_str(), RT_RCDATA);
    if (!res)
    {
        std::string resourceFileName = resourceName + ".rc";
        WindowsException windowsException(GetLastError());
        throw std::runtime_error("error: error getting resource '" + resourceName + "' from module '" + moduleName + "': could not find resource: " +
            windowsException.ErrorMessage() + ": note: please add resource file '" + resourceFileName + "' to the C++ project that makes executable '" + moduleName + "'");
    }
    HGLOBAL handle = LoadResource(moduleHandle, res);
    if (!handle)
    {
        std::string resourceFileName = resourceName + ".rc";
        WindowsException windowsException(GetLastError());
        throw std::runtime_error("error: error loading resource '" + resourceName + "' from module '" + moduleName + ": " +
            windowsException.ErrorMessage() + ": note: please add resource file '" + resourceFileName + "' to the C++ project that makes executable '" + moduleName + "'");
    }
    data = static_cast<uint8_t*>(LockResource(handle));
    size = SizeofResource(moduleHandle, res);
}

} // util
