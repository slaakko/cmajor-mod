// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.debug;

namespace util {

void DebugBreak()
{
    ::DebugBreak();
}

} // namespace util
