// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.ir.compile.unit;

import cmajor.cpp.ir.context;
import std.core;

export namespace cmajor::cpp::ir {

class CompileUnit
{
public:
    CompileUnit(const std::string& filePath_);
    CompileUnit(const CompileUnit&) = delete;
    CompileUnit& operator=(const CompileUnit&) = delete;
    void SetId(const std::string& id_);
    void SetSourceFilePath(const std::string& sourceFilePath_);
    void Write();
    Context* GetContext() { return &context; }
    Function* GetOrInsertFunction(const std::string& name, FunctionType* type);
private:
    Context context;
    std::string id;
    std::string sourceFilePath;
    std::string filePath;
    std::vector<std::unique_ptr<Function>> functions;
    std::unordered_map<std::string, Function*> functionMap;
};

} // namespace cmajor::cpp::ir
