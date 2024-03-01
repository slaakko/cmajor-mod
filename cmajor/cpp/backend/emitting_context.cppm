// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.backend.emitting.context;

import cmajor.ir;
import cmajor.cpp.ir;

export namespace cmajor::cpp::backend {

class CppEmittingContext : public cmajor::ir::EmittingContext
{
public:
    CppEmittingContext(int optimizationLevel_);
    int OptimizationLevel() const override { return optimizationLevel; }
    const std::string& TargetTripleStr() const override { return targetTriple; }
    void* Triple() const override { return nullptr; }
    void* TargetMachine() override { return nullptr; }
    void* DataLayout() const override { return nullptr; }
private:
    int optimizationLevel;
    std::string targetTriple;
};

} // namespace cmajor::cpp::backend
