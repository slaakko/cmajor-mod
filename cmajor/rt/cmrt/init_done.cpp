// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/init_done.hpp>
#include <cmrt/classes.hpp>
#include <cmrt/debug.hpp>
#include <cmrt/io.hpp>
#include <cmrt/thread.hpp>
#include <cmrt/socket.hpp>
#include <util/init_done.hpp>

void RtmInit()
{
    util::Init();
    cmajor::rt::InitIO();
    cmajor::rt::InitCmdbSession();
    cmajor::rt::StartCmdbSession();
    cmajor::rt::InitSocket();
}

void RtmDone()
{
    cmajor::rt::DoneSocket();
    cmajor::rt::DoneCmdbSession();
    cmajor::rt::DoneThread();
    cmajor::rt::RunDestructors();
    cmajor::rt::DoneIO();
    util::Done();
}
