// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.error;

namespace cmajor::symbols {

Error::Error(const std::string& message_) : std::runtime_error(message_), message(message_)
{
}

} // namespace cmajor::symbols
