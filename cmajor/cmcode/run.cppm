// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.run;

import cmajor.ast;
import std.core;

export namespace cmcode {

void RunProgram(const std::string& backend, const std::string& config, int optLevel, cmajor::ast::Project* project, const std::string& programArguments);

} // namespace cmcode
