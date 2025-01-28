// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.service.config;

import std.core;

export namespace cmajor::service {

std::string CmajorRootDir();
std::string CmajorConfigDir();
std::string CmajorLogDir();
std::string CmajorProjectsDir();
std::string BinaryPortMapConfigFilePath();
std::string BinaryPortMapLogFilePath();

} // namespace cmajor::service

