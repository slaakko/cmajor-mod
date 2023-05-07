// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.source.file.module_map;

import cmajor.ast.project;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class Module;

void MapSourceFileToModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath, const util::uuid& moduleId);
Module* GetModuleBySourceFile(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath);

} // namespace cmajor::symbols;
