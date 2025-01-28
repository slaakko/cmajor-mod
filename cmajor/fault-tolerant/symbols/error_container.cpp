// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.error.container;

namespace cmajor::fault::tolerant::symbols {

ErrorContainer::ErrorContainer()
{
}

void ErrorContainer::AddError(const std::runtime_error& error)
{
    errors.push_back(error);
}

} // namespace cmajor::fault::tolerant::symbols
