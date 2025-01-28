// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.compile_unit;

import std.core;

export namespace cmajor::masm::intermediate {

class Context;

class CompileUnit
{
public:
    CompileUnit();
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    const std::string& FilePath() const { return filePath; }
    void SetFilePath(const std::string& filePath_);
    void Write();
private:
    Context* context;
    std::string filePath;
};

} // cmajor::masm::intermediate
