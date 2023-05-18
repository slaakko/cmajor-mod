// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.codegen.windows;

import std.core;
import cmajor.ir;
import cmajor.llvm.codegen;

export namespace cmajor::llvm {

class WindowsCodeGenerator : public LLvmCodeGenerator
{
public:
    WindowsCodeGenerator(cmajor::ir::Emitter* emitter_);
};

} // namespace cmajor::llvm
