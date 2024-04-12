// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.init_done;

import cmajor.masm.rt.classes;
import cmajor.masm.rt.debug;
import cmajor.masm.rt.io;
import cmajor.masm.rt.thread;

void RtmInit()
{
    cmajor::masm::rt::InitIO();
    cmajor::masm::rt::InitCmdbSession();
    cmajor::masm::rt::StartCmdbSession();
}

void RtmDone()
{
    cmajor::masm::rt::DoneCmdbSession();
    cmajor::masm::rt::DoneThread();
    cmajor::masm::rt::RunDestructors();
    cmajor::masm::rt::DoneIO();
}
