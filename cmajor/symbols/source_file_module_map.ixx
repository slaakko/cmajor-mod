module;
#include <boost/uuid/uuid.hpp>
export module cmajor.symbols.source.file.module_.map;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

/*
#include <sngcm/ast/Project.hpp>
#include <cmajor/symbols/SymbolsApi.hpp>
#include <boost/uuid/uuid.hpp>
*/
//import cmajor.symbols.module_;
import cmajor.ast.project;
import std.core;

export namespace cmajor::symbols {

class Module;

void MapSourceFileToModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath, const boost::uuids::uuid& moduleId);
Module* GetModuleBySourceFile(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath);
void InitSourceFileModuleMap();
void DoneSourceFileModuleMap();

} // namespace cmajor::symbols;