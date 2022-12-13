// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.trace;

import std.core;

export namespace cmajor::symbols {

	int32_t GetTraceFunctionId(const std::string& functionFullName);
	void ReadTraceTable();
	void WriteTraceTable();

} // namespace cmajor::symbols
