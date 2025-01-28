// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.install;

import std.core;
import cmajor.symbols;

export namespace cmajor::build {

void InstallSystemLibraries(cmajor::symbols::Module* systemInstallModule);
void InstallSystemWindowsLibraries(cmajor::symbols::Module* systemInstallWindowsModule);

} // namespace cmajor::build
