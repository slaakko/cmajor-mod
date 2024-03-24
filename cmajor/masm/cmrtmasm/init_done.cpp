// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.init_done;

import cmajor.masm.rt.classes;
import cmajor.masm.rt.io;

void RtmInit()
{
    cmajor::masm::rt::InitIO();
}

void RtmDone()
{
    cmajor::masm::rt::RunDestructors();
    cmajor::masm::rt::DoneIO();
}
