// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.resources;

import cmajor.ast;
import std.core;

export namespace cmajor::build {

void AddResources(cmajor::ast::Project* project, std::vector<std::string>& objectFilePaths);

} // namespace cmajor::build
