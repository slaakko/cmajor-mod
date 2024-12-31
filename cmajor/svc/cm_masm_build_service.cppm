// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cm_masm_build.service;

import cmajor.info.bs;
import std.core;

export namespace cmajor::service {

cmajor::info::bs::BuildResult BuildWithCmMasmCompiler(const cmajor::info::bs::BuildCommand& buildCommmand);
void StopCmMasmBuild();

} // cmajor::service
