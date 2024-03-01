// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.text.format;

import cmajor.systemx.machine.machine;
import std.core;

export namespace cmajor::systemx::object {

class SymbolTable;

using TrapAdderFunc = void(*)(SymbolTable& symbolTable);

void SetTrapAdderFunc(TrapAdderFunc trapAdderFunc);

void WriteBinaryFileAsText(const std::string& binaryFileName, cmajor::systemx::machine::Machine& machine, std::string& textFileName);

} // namespace cmajor::systemx::object
