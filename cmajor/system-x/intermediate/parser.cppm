// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.parser;

import cmajor.systemx.intermediate.context;
import std.core;

export namespace cmajor::systemx::intermediate {

void Parse(int logStreamId, const std::string& filePath, Context& context, bool verbose);

} // cmajor::systemx::intermediate
