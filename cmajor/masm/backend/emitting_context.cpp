// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.backend.emitting.context;

namespace cmajor::masm::backend {

MasmEmittingContext::MasmEmittingContext(int optimizationLevel_) : optimizationLevel(optimizationLevel_), tripleStr()
{
}

const std::string& MasmEmittingContext::TargetTripleStr() const
{
    return tripleStr;
}

void* MasmEmittingContext::Triple() const
{
    return nullptr;
}

void* MasmEmittingContext::TargetMachine()
{
    return nullptr;
}

void* MasmEmittingContext::DataLayout() const
{
    return nullptr;
}

} // namespace cmajor::masm::backend
