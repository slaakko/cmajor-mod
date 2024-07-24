// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.env;

import std.core;

export {

extern "C" int RtmGetEnvironmentVariable(const char* envVarName);

}
