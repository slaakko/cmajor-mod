module;
#include <Windows.h>

// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.registry;

import std.core;

export namespace wing {

enum class RegistryValueKind
{
    regSz, regExpandSz
};

class RegistryKey
{
public:
    RegistryKey(HKEY key_);
    ~RegistryKey();
    RegistryKey(const RegistryKey&) = delete;
    RegistryKey(RegistryKey&& that) noexcept;
    RegistryKey& operator=(const RegistryKey&) = delete;
    RegistryKey& operator=(RegistryKey&& that) noexcept;
    bool HasValue(const std::string& name) const;
    std::string GetValue(const std::string& name) const;
    void SetValue(const std::string& name, const std::string& value, RegistryValueKind valueKind);
    void DeleteValue(const std::string& name);
    void SetIntegerValue(const std::string& name, int value);
    int GetIntegerValue(const std::string& name) const;
    std::string GetSubkeyName(int index) const;
    static RegistryKey CurrentUser(REGSAM access);
    static RegistryKey Open(HKEY predefinedKey, const std::string& subKey, REGSAM access);
    static RegistryKey Create(HKEY predefinedKey, const std::string& subKey);
    static void Detele(HKEY predefinedKey, const std::string& subKey);
    static bool Exists(HKEY predefinedKey, const std::string& subKey, REGSAM access);
private:
    HKEY key;
};

} // wing
