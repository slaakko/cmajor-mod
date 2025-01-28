// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.init_done;

import cmajor.masm.rt.classes;
import cmajor.masm.rt.debug;
import cmajor.masm.rt.io;
import cmajor.masm.rt.thread;
import cmajor.masm.rt.socket;
import util;

void RtmInit()
{
    util::Init();
    cmajor::masm::rt::InitIO();
    cmajor::masm::rt::InitCmdbSession();
    cmajor::masm::rt::StartCmdbSession();
    cmajor::masm::rt::InitSocket();
}

void RtmDone()
{
    cmajor::masm::rt::DoneSocket();
    cmajor::masm::rt::DoneCmdbSession();
    cmajor::masm::rt::DoneThread();
    cmajor::masm::rt::RunDestructors();
    cmajor::masm::rt::DoneIO();
    util::Done();
}
