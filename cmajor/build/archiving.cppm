// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.archiving;

import std.core;
import cmajor.ast;

export namespace cmajor::build {

void Archive(cmajor::ast::Project* project, const std::vector<std::string>& objectFilePaths);

} // namespace cmajor::build
