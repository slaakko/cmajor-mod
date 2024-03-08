// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.compile.unit;

import cmajor.masm.ir.function;
import cmajor.masm.ir.context;
import cmajor.masm.ir.type;
import std.core;

export namespace cmajor::masm::ir {

class CompileUnit
{
public:
    CompileUnit(const std::string& filePath_);
    CompileUnit(const CompileUnit&) = delete;
    CompileUnit& operator=(const CompileUnit&) = delete;
    Function* GetOrInsertFunction(const std::string& name, FunctionType* type);
    void SetId(const std::string& id_);
    const std::string& SourceFilePath() const { return sourceFilePath; }
    void SetSourceFilePath(const std::string& sourceFilePath_);
    void Write();
    Context* GetContext() { return &context; }
private:
    Context context;
    std::string id;
    std::string sourceFilePath;
    std::string filePath;
    std::vector<std::unique_ptr<Function>> functions;
    std::unordered_map<std::string, Function*> functionMap;
};

} // namespace cmajor::masm::ir
