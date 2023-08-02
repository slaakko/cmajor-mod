// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.util;

import cmajor.debug;
import std.core;

export namespace cmajor::debugger {

class Results;
class Reply;
class Tuple;

cmajor::debug::Frame GetCppFrame(Results* results, cmajor::debug::DebugInfo* debugInfo);

std::string ToExternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix);

std::string ToInternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix);

Tuple* MakeFrameTuple(const cmajor::debug::Frame& frame);

bool StackFrameHasLine(Reply* reply);

} // namespace cmajor::debugger
