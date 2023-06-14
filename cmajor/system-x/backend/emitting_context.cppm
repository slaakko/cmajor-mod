// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.backend.emitting.context;

import cmajor.ir;
import std.core;

export namespace cmajor::systemx::backend {

class SystemXEmittingContext : public cmajor::ir::EmittingContext
{
public:
    SystemXEmittingContext(int optimizationLevel_);
    int OptimizationLevel() const override { return optimizationLevel; }
    const std::string& TargetTripleStr() const override;
    void* Triple() const override;
    void* TargetMachine() override;
    void* DataLayout() const override;
private:
    int optimizationLevel;
    std::string tripleStr;
};

} // cmajor::llvm
