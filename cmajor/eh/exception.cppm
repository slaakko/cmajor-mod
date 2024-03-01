// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.eh.exception;

import std.core;

export namespace cmajor::eh {

class Exception
{
};

} // namespace cmajor::eh

export extern "C" void* EhGetExceptionTypeId();
