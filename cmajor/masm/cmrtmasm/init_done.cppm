// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.init_done;

import std.core;

export {

extern "C" void RtmInit();
extern "C" void RtmDone();

}
