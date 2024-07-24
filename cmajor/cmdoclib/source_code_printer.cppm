// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.source_code_printer;

import cmdoclib.file;
import cmajor.ast;
import cmajor.binder;
import std.core;

export namespace cmdoclib {

void GenerateSourceCode(cmajor::ast::Project* project, cmajor::binder::BoundCompileUnit* boundCompileUnit, std::map<int, File>& fileMap);

} // namespace cmdoclib
