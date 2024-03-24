// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.ascii;

import std.core;

export {

extern "C" char RtmAsciiToUpper(char c);
extern "C" char RtmAsciiToLower(char c);

}
