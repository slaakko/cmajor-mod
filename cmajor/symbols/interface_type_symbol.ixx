export module cmajor.symbols.interface_.type.symbol;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/FunctionSymbol.hpp>
//#include <cmajor/symbols/ClassTypeSymbol.hpp>
import cmajor.symbols.type.symbol;

import cmajor.symbols.function.symbol;
import cmajor.symbols.class_.type.symbol;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import std.core;

export namespace cmajor::symbols {

class InterfaceTypeCopyConstructor;

class InterfaceTypeSymbol : public TypeSymbol
{
public:
    InterfaceTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "interface"; }
    void AddMember(Symbol* member) override;
    void Accept(SymbolCollector* collector) override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    const std::vector<MemberFunctionSymbol*>& MemberFunctions() const { return memberFunctions; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, MemberFunctionSymbol* interfaceMemberFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    void SetCopyConstructor(InterfaceTypeCopyConstructor* copyConstructor_) { copyConstructor = copyConstructor_; }
    InterfaceTypeCopyConstructor* CopyConstructor() { return copyConstructor; }
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "InterfaceTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "IN"; }
    std::string GetSymbolCategoryDescription() const override { return "interface"; }
private:
    std::vector<MemberFunctionSymbol*> memberFunctions;
    InterfaceTypeCopyConstructor* copyConstructor;
};

class InterfaceTypeDefaultConstructor : public FunctionSymbol
{
public:
    InterfaceTypeDefaultConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    InterfaceTypeDefaultConstructor(InterfaceTypeSymbol* interfaceType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeDefaultConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeCopyConstructor : public FunctionSymbol
{
public:
    InterfaceTypeCopyConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    InterfaceTypeCopyConstructor(InterfaceTypeSymbol* interfaceType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeCopyConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeMoveConstructor : public FunctionSymbol
{
public:
    InterfaceTypeMoveConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    InterfaceTypeMoveConstructor(InterfaceTypeSymbol* interfaceType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeMoveConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeCopyAssignment : public FunctionSymbol
{
public:
    InterfaceTypeCopyAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    InterfaceTypeCopyAssignment(InterfaceTypeSymbol* interfaceType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeCopyAssignment"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeMoveAssignment : public FunctionSymbol
{
public:
    InterfaceTypeMoveAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    InterfaceTypeMoveAssignment(InterfaceTypeSymbol* interfaceType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeMoveAssignment"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class ClassToInterfaceConversion : public FunctionSymbol
{
public:
    ClassToInterfaceConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassToInterfaceConversion(ClassTypeSymbol* sourceClassType_, InterfaceTypeSymbol* targetInterfaceType_, int32_t interfaceIndex_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsClassToInterfaceTypeConversion() const override { return true; }
    const char* ClassName() const override { return "ClassToInterfaceConversion"; }
    void Check() override;
private:
    ClassTypeSymbol* sourceClassType;
    InterfaceTypeSymbol* targetInterfaceType;
    int32_t interfaceIndex;
};

class GetObjectPtrFromInterface : public FunctionSymbol
{
public:
    GetObjectPtrFromInterface(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    GetObjectPtrFromInterface(InterfaceTypeSymbol* interfaceTypeSymbol);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "GetObjectPtrFromInterface"; }
private:
    InterfaceTypeSymbol* interfaceType;
};
} // namespace cmajor::symbols
