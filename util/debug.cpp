// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.debug;

namespace util {

UnexpectedExecutorThread::UnexpectedExecutorThread() : std::runtime_error("unexpected executor thread")
{
}

void DebugBreak()
{
    ::DebugBreak();
}

} // namespace util
