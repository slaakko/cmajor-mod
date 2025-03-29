// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.optimizer.emitter;

import cmajor.systemx.intermediate;
import std.core;

export namespace cmajor::systemx::optimizer {

void EmitParamRegArg(cmajor::systemx::intermediate::ArgInstruction& inst, cmajor::systemx::intermediate::CodeGenerator& codeGen);
void EmitParamRegParam(cmajor::systemx::intermediate::ParamInstruction& inst, cmajor::systemx::intermediate::CodeGenerator& codeGen);

} // cmajor::systemx::optimizer
