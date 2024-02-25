// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.compile.unit;

import cmajor.systemx.ir.function;
import cmajor.systemx.ir.metadata;
import cmajor.systemx.ir.context;
import cmajor.systemx.ir.type;
import std.core;

export namespace cmajor::systemx::ir {

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
    MDStructRef* cu;
    std::string id;
    std::string sourceFilePath;
    std::string filePath;
    std::vector<std::unique_ptr<Function>> functions;
    std::unordered_map<std::string, Function*> functionMap;
};

} // namespace cmajor::systemx::ir
