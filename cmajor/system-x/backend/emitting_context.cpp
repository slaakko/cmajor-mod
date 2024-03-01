// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.backend.emitting.context;

namespace cmajor::systemx::backend { 

SystemXEmittingContext::SystemXEmittingContext(int optimizationLevel_) : optimizationLevel(optimizationLevel_), tripleStr()
{
}

const std::string& SystemXEmittingContext::TargetTripleStr() const
{
    return tripleStr;
}

void* SystemXEmittingContext::Triple() const
{
    return nullptr;
}

void* SystemXEmittingContext::TargetMachine()
{
    return nullptr;
}

void* SystemXEmittingContext::DataLayout() const
{
    return nullptr;
}
 
} // namespace cmajor::systemx::backend
