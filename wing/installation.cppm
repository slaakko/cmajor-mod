// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.installation;
//#include <wing/Registry.hpp>
//#include <vector>

import wing.registry;
import std.core;

export namespace wing {

std::vector<std::string> GetInstalledSoftwareRegistryKeyPaths();

RegistryKey OpenOrCreateRegistryKeyForSoftwareKey(const std::string& softwareKey);
void DeleteRegistryKeyForSoftwareKey(const std::string& softwareKey);
} // wing