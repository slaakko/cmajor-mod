// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.init_done;

import std.core;

export {

extern "C" void RtmInit();
extern "C" void RtmDone();

}
