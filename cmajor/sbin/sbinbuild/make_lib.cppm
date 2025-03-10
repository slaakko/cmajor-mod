// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.build.make_lib;

import cmajor.ast;
import cmajor.symbols;
import std.core;

export namespace cmajor::sbin::build {

std::string MakeLib(cmajor::ast::Project* project, cmajor::symbols::Module* module, const std::vector<std::string>& objectFilePaths, bool program, bool verbose);

} // namespace cmajor::sbin::build
