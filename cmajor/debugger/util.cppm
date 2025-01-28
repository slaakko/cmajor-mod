// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.util;

import cmajor.debug;
import cmajor.info;
import std.core;

export namespace cmajor::debugger {

class Results;
class Reply;
class Tuple;
class Debugger;

cmajor::info::db::Location ToLocation(const cmajor::debug::Frame& frame);

cmajor::debug::Frame GetCppFrame(Results* results, cmajor::debug::DebugInfo* debugInfo);

std::string MakeExternalFilePath(const std::string& path, cmajor::debug::DebugInfo* debugInfo);

std::string ToExternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix);

std::string ToInternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix);

Tuple* MakeFrameTuple(const cmajor::debug::Frame& frame);

bool StackFrameHasLine(Reply* reply);

std::vector<cmajor::info::db::Location> GetFrames(cmajor::debugger::Results* results, cmajor::debug::DebugInfo* debugInfo, cmajor::debug::DebuggerOutputWriter* outputWriter);

cmajor::debug::DIType* GetDynamicType(cmajor::debug::BoundDebugNode* node, cmajor::debug::DIType* type, Debugger* debugger);

} // namespace cmajor::debugger
