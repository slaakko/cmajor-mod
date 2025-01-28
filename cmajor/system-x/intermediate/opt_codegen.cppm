// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.opt.codegen;

import cmajor.systemx.intermediate.codegen;
import std.core;

export namespace cmajor::systemx::intermediate {

void EmitOptSwitch(SwitchInstruction& inst, CodeGenerator& codeGen);

} // cmajor::systemx::intermediate
