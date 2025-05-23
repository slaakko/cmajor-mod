// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.compiling;

import std.core;
import cmajor.symbols;
import cmajor.ast;
import cmajor.build.flags;
import cmajor.binder.attribute.binder;
import cmajor.binder.bound.compile.unit;
import cmajor.ir;
import cmdoclib;

export namespace cmajor::build {

void CreateSymbols(cmajor::symbols::Context* context, cmajor::symbols::SymbolTable& symbolTable, cmajor::ast::Project* project, bool& stop);

std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> BindTypes(cmajor::symbols::Context* context, cmajor::ast::Project* project,
    cmajor::binder::AttributeBinder* attributeBinder, bool& stop);

void Compile(cmajor::ast::Project* project, cmajor::symbols::Context* context, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, std::vector<std::string>& asmFilePaths, std::map<int, cmdoclib::File>& docFileMap, bool& stop);

} // namespace cmajor::build
