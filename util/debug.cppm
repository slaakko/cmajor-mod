// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.debug;

import std.core;

export namespace util {

void DebugBreak();

class UnexpectedExecutorThread : public std::runtime_error
{
public:
    UnexpectedExecutorThread();
};

} // namespace util
