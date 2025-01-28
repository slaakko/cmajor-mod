// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.symbol_table_xml;

import std.core;
import cmajor.symbols;
import cmdoclib.input;
import cmdoclib.file;

export namespace cmdoclib {

void GenerateSymbolTableXml(cmajor::symbols::Module* rootModule, std::map<int, File>& fileMap);
void ReadExternalModuleDocuments(Input* input);

} // namespace cmdoclib
