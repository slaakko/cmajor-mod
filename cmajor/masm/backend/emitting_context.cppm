// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.backend.emitting.context;

import cmajor.ir;
import std.core;

export namespace cmajor::masm::backend {

class MasmEmittingContext : public cmajor::ir::EmittingContext
{
public:
    MasmEmittingContext(int optimizationLevel_);
    int OptimizationLevel() const override { return optimizationLevel; }
    const std::string& TargetTripleStr() const override;
    void* Triple() const override;
    void* TargetMachine() override;
    void* DataLayout() const override;
private:
    int optimizationLevel;
    std::string tripleStr;
};

} // cmajor::masm::backend
