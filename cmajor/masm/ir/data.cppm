// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.data;

import cmajor.masm.ir.type;
import cmajor.masm.ir.value;
import cmajor.masm.ir.value;
import util;

export namespace cmajor::masm::ir {

class GlobalVariable : public Value
{
public:
    GlobalVariable(Type* type_, const std::string& name_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void SetInitializer(Value* initializer_) { initializer = initializer_; }
    Value* Initializer() const { return initializer; }
    void Write(Context& context, util::CodeFormatter& formatter);
private:
    Type* type;
    std::string name;
    Value* initializer;
};

class DataRepository
{
public:
    DataRepository();
    DataRepository(const DataRepository&) = delete;
    DataRepository& operator=(const DataRepository&) = delete;
    GlobalVariable* GetOrInsertGlobal(const std::string& name, Type* type);
    GlobalVariable* CreateGlobalStringPtr(Context& context, const std::string& stringValue);
    void Write(Context& context, util::CodeFormatter& formatter);
    void SetCompileUnitId(const std::string& compileUnitId_);
    const std::string& CompileUnitId() const { return compileUnitId; }
private:
    std::vector<std::unique_ptr<GlobalVariable>> globalVariableDefinitions;
    std::unordered_map<std::string, GlobalVariable*> globalVariableMap;
    int nextStringId;
    std::string compileUnitId;
};

} // namespace cmajor::masm::ir

