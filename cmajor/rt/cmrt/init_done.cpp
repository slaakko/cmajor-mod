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
    cmajor::cpp::rt::InitIO();
    cmajor::cpp::rt::InitCmdbSession();
    cmajor::cpp::rt::StartCmdbSession();
    cmajor::cpp::rt::InitSocket();
}

void RtmDone()
{
    cmajor::cpp::rt::DoneSocket();
    cmajor::cpp::rt::DoneCmdbSession();
    cmajor::cpp::rt::DoneThread();
    cmajor::cpp::rt::RunDestructors();
    cmajor::cpp::rt::DoneIO();
    util::Done();
}
