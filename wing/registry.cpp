// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.registry;

import wing.base;
import util;

namespace wing {

RegistryKey::RegistryKey(HKEY key_) : key(key_)
{
}

RegistryKey::~RegistryKey()
{
    if (key)
    {
        RegCloseKey(key);
    }
}

RegistryKey::RegistryKey(RegistryKey&& that) noexcept : key(that.key)
{
    that.key = nullptr;
}

RegistryKey& RegistryKey::operator=(RegistryKey&& that) noexcept
{
    std::swap(key, that.key);
    return *this;
}

bool RegistryKey::HasValue(const std::string& name) const
{
    DWORD flags = RRF_RT_REG_SZ;
    std::u16string valueName = util::ToUtf16(name);
    LSTATUS status = RegGetValueW(key, nullptr, (LPCWSTR)valueName.c_str(), flags, nullptr, nullptr, nullptr);
    return status == ERROR_SUCCESS;
}

std::string RegistryKey::GetValue(const std::string& name) const
{
    DWORD flags = RRF_RT_REG_SZ;
    std::u16string valueName = util::ToUtf16(name);
    const int bufferSize = 4096;
    std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);
    DWORD size = bufferSize;
    LSTATUS status = RegGetValueW(key, nullptr, (LPCWSTR)valueName.c_str(), flags, nullptr, buffer.get(), &size);
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
    std::u16string value(reinterpret_cast<const char16_t*>(buffer.get()));
    return util::ToUtf8(value);
}

void RegistryKey::SetValue(const std::string& name, const std::string& value, RegistryValueKind valueKind)
{
    DWORD type = 0;
    switch (valueKind)
    {
    case RegistryValueKind::regSz: type = REG_SZ; break;
    case RegistryValueKind::regExpandSz: type = REG_EXPAND_SZ; break;
    }
    std::u16string valueName = util::ToUtf16(name);
    std::u16string data = util::ToUtf16(value);
    LSTATUS status = RegSetKeyValueW(key, nullptr, (LPCWSTR)valueName.c_str(), type, (LPCWSTR)data.c_str(), sizeof(char16_t) * (data.length() + 1));
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
}

void RegistryKey::DeleteValue(const std::string& name)
{
    std::u16string valueName = util::ToUtf16(name);
    LSTATUS status = RegDeleteKeyValueW(key, nullptr, (LPCWSTR)valueName.c_str());
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
}

void RegistryKey::SetIntegerValue(const std::string& name, int value)
{
    std::u16string valueName = util::ToUtf16(name);
    DWORD data = static_cast<DWORD>(value);
    LSTATUS status = RegSetKeyValueW(key, nullptr, (LPCWSTR)valueName.c_str(), REG_DWORD, &data, sizeof(data));
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
}

int RegistryKey::GetIntegerValue(const std::string& name) const
{
    std::u16string valueName = util::ToUtf16(name);
    DWORD data = 0;
    DWORD size = sizeof(data);
    LSTATUS status = RegGetValueW(key, nullptr, (LPCWSTR)valueName.c_str(), RRF_RT_DWORD, nullptr, &data, &size);
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
    return static_cast<int>(data);
}

std::string RegistryKey::GetSubkeyName(int index) const
{
    const int bufferSize = 4096;
    std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);
    DWORD size = bufferSize;
    LSTATUS status = RegEnumKeyExW(key, index, (LPWSTR)buffer.get(), &size, nullptr, nullptr, nullptr, nullptr);
    if (status != ERROR_SUCCESS)
    {
        if (status == ERROR_NO_MORE_ITEMS)
        {
            return std::string();
        }
        throw WindowsException(status);
    }
    std::u16string value(reinterpret_cast<const char16_t*>(buffer.get()));
    return util::ToUtf8(value);
}

RegistryKey RegistryKey::CurrentUser(REGSAM access)
{
    HKEY key = nullptr;
    LSTATUS status = RegOpenCurrentUser(access, &key);
    if (status == ERROR_SUCCESS)
    {
        return RegistryKey(key);
    }
    else
    {
        throw WindowsException(status);
    }
}

RegistryKey RegistryKey::Open(HKEY predefinedKey, const std::string& subKey, REGSAM access)
{
    HKEY key = nullptr;
    std::u16string subKeyStr = util::ToUtf16(subKey);
    LSTATUS status = RegOpenKeyExW(predefinedKey, (LPCWSTR)subKeyStr.c_str(), 0, access, &key);
    if (status == ERROR_SUCCESS)
    {
        return RegistryKey(key);
    }
    else
    {
        throw WindowsException(status);
    }
}

RegistryKey RegistryKey::Create(HKEY predefinedKey, const std::string& subKey)
{
    HKEY key = nullptr;
    std::u16string subKeyStr = util::ToUtf16(subKey);
    LSTATUS status = RegCreateKeyExW(predefinedKey, (LPCWSTR)subKeyStr.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &key, nullptr);
    if (status == ERROR_SUCCESS)
    {
        return RegistryKey(key);
    }
    else
    {
        throw WindowsException(status);
    }
}

void RegistryKey::Detele(HKEY predefinedKey, const std::string& subKey)
{
    std::u16string subKeyStr = util::ToUtf16(subKey);
    LSTATUS status = RegDeleteKeyW(predefinedKey, (LPCWSTR)subKeyStr.c_str());
    if (status != ERROR_SUCCESS)
    {
        throw WindowsException(status);
    }
}

bool RegistryKey::Exists(HKEY predefinedKey, const std::string& subKey, REGSAM access)
{
    HKEY key = nullptr;
    std::u16string subKeyStr = util::ToUtf16(subKey);
    LSTATUS status = RegOpenKeyExW(predefinedKey, (LPCWSTR)subKeyStr.c_str(), 0, access, &key);
    if (status == ERROR_SUCCESS)
    {
        RegCloseKey(key);
        return true;
    }
    else
    {
        return false;
    }
}

} // wing
