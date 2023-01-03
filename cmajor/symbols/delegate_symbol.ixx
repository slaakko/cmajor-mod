export module cmajor.symbols.delegate_.symbol;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/ClassTypeSymbol.hpp>
//#include <cmajor/symbols/FunctionSymbol.hpp>

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import cmajor.ast.specifier;
import cmajor.ir.gen.object;
import soul.ast.source.pos;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class ParameterSymbol;

class DelegateTypeSymbol : public TypeSymbol
{
public:
    DelegateTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void AddMember(Symbol* member) override;
    std::string TypeString() const override { return "delegate"; }
    std::string Syntax() const override;
    std::u32string Id() const override;
    bool IsDelegateType() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    //llvm::DIType* CreateDIType(cmajor::ir::Emitter& emitter) override; todo
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    int Arity() const { return parameters.size(); }
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    const TypeSymbol* ReturnType() const { return returnType; }
    TypeSymbol* ReturnType() { return returnType; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    bool ReturnsClassInterfaceOrClassDelegateByValue() const;
    void SetReturnParam(ParameterSymbol* returnParam_);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "DelegateTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "DG"; }
    std::string GetSymbolCategoryDescription() const override { return "delegate"; }
    std::string GetSymbolHelp() const override;
private:
    TypeSymbol* returnType;
    std::vector<ParameterSymbol*> parameters;
    std::unique_ptr<ParameterSymbol> returnParam;
};

class DelegateTypeDefaultConstructor : public FunctionSymbol
{
public:
    DelegateTypeDefaultConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeDefaultConstructor(DelegateTypeSymbol* delegateType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeDefaultConstructor"; }
    void Check() override;
private:
    DelegateTypeSymbol* delegateType;
};

class DelegateTypeCopyConstructor : public FunctionSymbol
{
public:
    DelegateTypeCopyConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeCopyConstructor(DelegateTypeSymbol* delegateType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeCopyConstructor"; }
};

class DelegateTypeMoveConstructor : public FunctionSymbol
{
public:
    DelegateTypeMoveConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeMoveConstructor(DelegateTypeSymbol* delegateType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeMoveConstructor"; }
};

class DelegateTypeCopyAssignment : public FunctionSymbol
{
public:
    DelegateTypeCopyAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeCopyAssignment(DelegateTypeSymbol* delegateType, TypeSymbol* voidType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeCopyAssignment"; }
};

class DelegateTypeMoveAssignment : public FunctionSymbol
{
public:
    DelegateTypeMoveAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeMoveAssignment(DelegateTypeSymbol* delegateType, TypeSymbol* voidType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeMoveAssignment"; }
};

class DelegateTypeReturn : public FunctionSymbol
{
public:
    DelegateTypeReturn(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeReturn(DelegateTypeSymbol* delegateType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeReturn"; }
};

class DelegateTypeEquality : public FunctionSymbol
{
public:
    DelegateTypeEquality(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    DelegateTypeEquality(DelegateTypeSymbol* delegateType, TypeSymbol* boolType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeEquality"; }
};

class FunctionToDelegateConversion : public FunctionSymbol
{
public:
    FunctionToDelegateConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    FunctionToDelegateConversion(TypeSymbol* sourceType_, TypeSymbol* targetType_, FunctionSymbol* function_);
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "FunctionToDelegateConversion"; }
    void Check() override;
private:
    TypeSymbol* sourceType;
    TypeSymbol* targetType;
    FunctionSymbol* function;
};

class DelegateToVoidPtrConversion : public FunctionSymbol
{
public:
    DelegateToVoidPtrConversion(TypeSymbol* delegateType_, TypeSymbol* voidPtrType_);
    ConversionType GetConversionType() const override { return ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    const char* ClassName() const override { return "DelegateToVoidPtrConversion"; }
private:
    TypeSymbol* delegateType;
    TypeSymbol* voidPtrType;
};

class VoidPtrToDelegateConversion : public FunctionSymbol
{
public:
    VoidPtrToDelegateConversion(TypeSymbol* voidPtrType_, TypeSymbol* delegateType_, TypeSymbol* ulongType_);
    ConversionType GetConversionType() const override { return ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    const char* ClassName() const override { return "VoidPtrToDelegateConversion"; }
private:
    TypeSymbol* voidPtrType;
    TypeSymbol* delegateType;
    TypeSymbol* ulongType;
};

class ClassDelegateTypeSymbol : public TypeSymbol
{
public:
    ClassDelegateTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void AddMember(Symbol* member) override;
    std::string TypeString() const override { return "class_delegate"; }
    std::string Syntax() const override;
    std::u32string Id() const override;
    bool IsClassDelegateType() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    //llvm::DIType* CreateDIType(cmajor::ir::Emitter& emitter) override; todo
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    int Arity() const { return parameters.size(); }
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    const TypeSymbol* ReturnType() const { return returnType; }
    TypeSymbol* ReturnType() { return returnType; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    bool ReturnsClassInterfaceOrClassDelegateByValue() const;
    void SetReturnParam(ParameterSymbol* returnParam_);
    DelegateTypeSymbol* DelegateType() { return delegateType; }
    ClassTypeSymbol* ObjectDelegatePairType() { /* TODO Assert(objectDelegatePairType, "object delegate pair type not set");*/  return objectDelegatePairType; }
    FunctionSymbol* CopyConstructor() { /* TODO Assert(copyConstructor, "class delegate copy constructor not set"); */ return copyConstructor; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "ClassDelegateTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "CD"; }
    std::string GetSymbolCategoryDescription() const override { return "class delegate"; }
    std::string GetSymbolHelp() const override;
private:
    TypeSymbol* returnType;
    std::vector<ParameterSymbol*> parameters;
    std::unique_ptr<ParameterSymbol> returnParam;
    DelegateTypeSymbol* delegateType;
    ClassTypeSymbol* objectDelegatePairType;
    FunctionSymbol* copyConstructor;
};

class ClassDelegateTypeDefaultConstructor : public FunctionSymbol
{
public:
    ClassDelegateTypeDefaultConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeDefaultConstructor(ClassDelegateTypeSymbol* classDelegateType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeDefaultConstructor"; }
    void Check() override;
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeCopyConstructor : public FunctionSymbol
{
public:
    ClassDelegateTypeCopyConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeCopyConstructor(ClassDelegateTypeSymbol* classDelegateType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    bool IsClassDelegateCopyConstructor() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeCopyConstructor"; }
};

class ClassDelegateTypeMoveConstructor : public FunctionSymbol
{
public:
    ClassDelegateTypeMoveConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeMoveConstructor(ClassDelegateTypeSymbol* classDelegateType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeMoveConstructor"; }
};

class ClassDelegateTypeCopyAssignment : public FunctionSymbol
{
public:
    ClassDelegateTypeCopyAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeCopyAssignment(ClassDelegateTypeSymbol* classDelegateType, TypeSymbol* voidType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeCopyAssignment"; }
};

class ClassDelegateTypeMoveAssignment : public FunctionSymbol
{
public:
    ClassDelegateTypeMoveAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeMoveAssignment(ClassDelegateTypeSymbol* classDelegateType, TypeSymbol* voidType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeMoveAssignment"; }
};

class ClassDelegateTypeEquality : public FunctionSymbol
{
public:
    ClassDelegateTypeEquality(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassDelegateTypeEquality(ClassDelegateTypeSymbol* classDelegateType, TypeSymbol* boolType);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeEquality"; }
};

class MemberFunctionToClassDelegateConversion : public FunctionSymbol
{
public:
    MemberFunctionToClassDelegateConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    MemberFunctionToClassDelegateConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, TypeSymbol* sourceType_, ClassDelegateTypeSymbol* targetType_, FunctionSymbol* function_);
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    bool IsMemberFunctionToClassDelegateConversion() const override { return true; }
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "MemberFunctionToClassDelegateConversion"; }
    void Check() override;
private:
    TypeSymbol* sourceType;
    ClassDelegateTypeSymbol* targetType;
    FunctionSymbol* function;
    //LocalVariableSymbol* objectDelegatePairVariable;
};
} // namespace cmajor::symbols
