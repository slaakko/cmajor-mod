// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.debug;

import cmajor.service;
import cmajor.debug.message;
import cmajor.ast;
import std.core;

export namespace cmcode {

void StartDebugService(int pid, const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments, 
    const std::vector<cmajor::service::Breakpoint*>& breakpoints);

} // namespace cmcode
