// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.build.system;

import util;
import std.core;

export namespace cmajor::fault::tolerant::build {

std::string SystemSolutionPath();

void BuildSystem();

void ReadSystem();

} // namespace cmajor::fault::tolerant::build
