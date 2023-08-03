// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.debug;

import cmajor.service;
import cmajor.info;
import cmajor.debuggers;
import cmajor.ast;
import std.core;

export namespace cmcode {

void StartDebugService(const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments, 
    const std::vector<cmajor::debugger::Breakpoint*>& breakpoints);

} // namespace cmcode
