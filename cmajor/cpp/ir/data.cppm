// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.ir.data;

import cmajor.cpp.ir.type;
import cmajor.cpp.ir.value;
import util;

export namespace cmajor::cpp::ir {

class GlobalVariable : public Value
{
public:
    GlobalVariable(Type* type_, const std::string& name_);
    std::string Name(Context& context) override;
    Type* GetType(Context& context) override;
    ConstantValue* Initializer() const { return initializer; }
    void SetInitializer(ConstantValue* initializer_) { initializer = initializer_; }
    void Write(Context& context, util::CodeFormatter& formatter);
    //void SetLinkOnce() { linkOnce = true; }
    void SetStringPtr() { stringPtr = true; }
private:
    Type* type;
    std::string name;
    ConstantValue* initializer;
    //bool linkOnce;
    bool stringPtr;
};

class DataRepository
{
public:
    DataRepository();
    DataRepository(const DataRepository&) = delete;
    DataRepository& operator=(const DataRepository&) = delete;
    GlobalVariable* GetOrInsertGlobal(const std::string& name, Type* type);
    GlobalVariable* CreateGlobalStringPtr(Context& context, const std::string& stringValue);
    GlobalVariable* CreateGlobalWStringPtr(Context& context, const std::u16string& stringValue);
    GlobalVariable* CreateGlobalUStringPtr(Context& context, const std::u32string& stringValue);
    void Write(Context& context, util::CodeFormatter& formatter);
    void SetCompileUnitId(const std::string& compileUnitId_);
    const std::string& CompileUnitId() const { return compileUnitId; }
private:
    std::vector<std::unique_ptr<GlobalVariable>> globalVariableDefinitions;
    std::unordered_map<std::string, GlobalVariable*> globalVariableMap;
    int nextStringId;
    std::string compileUnitId;
};

} // namespace cmajor::cpp::ir
