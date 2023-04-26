module cmajor.symbols.error;
// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

namespace cmajor::symbols {

	Error::Error(const std::string& message_) : std::runtime_error(message_), message(message_)
	{
	}
} // namespace cmajor::symbols
