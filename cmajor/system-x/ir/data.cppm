// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.data;

import cmajor.systemx.ir.type;
import cmajor.systemx.ir.value;
import util;

export namespace cmajor::systemx::ir {

class GlobalVariable : public Value
{
public:
    GlobalVariable(Type* type_, const std::string& name_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    void SetInitializer(ConstantValue* initializer_) { initializer = initializer_; }
    ConstantValue* Initializer() const { return initializer; }
    void Write(Context& context, util::CodeFormatter& formatter);
    void SetLinkOnce() { linkOnce = true; }
private:
    Type* type;
    std::string name;
    ConstantValue* initializer;
    bool linkOnce;
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
private:
    std::vector<std::unique_ptr<GlobalVariable>> globalVariableDefinitions;
    std::unordered_map<std::string, GlobalVariable*> globalVariableMap;
    int nextStringId;
    std::string compileUnitId;
};

} // namespace cmajor::systemx::ir

