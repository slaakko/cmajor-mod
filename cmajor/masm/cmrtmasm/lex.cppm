// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.lex;

import std.core;

export extern "C" int* RtmGetClassMap(const char* classMapName, int32_t& errorId);
