// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.delegate.symbol;

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import cmajor.ast.specifier;
import cmajor.ir.gen.object;
import soul.ast.span;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class ParameterSymbol;

class DelegateTypeSymbol : public TypeSymbol
{
public:
    DelegateTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void AddMember(Symbol* member, Context* context) override;
    std::string TypeString(Context* context) const override { return "delegate"; }
    std::string Syntax(Context* context) override;
    std::u32string Id() const override;
    bool IsDelegateType() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* IrFunctionType(cmajor::ir::Emitter& emitter, Context* context);
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    //llvm::DIType* CreateDIType(cmajor::ir::Emitter& emitter) override; todo
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    int Arity() const { return parameters.size(); }
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    const TypeSymbol* ReturnType() const { return returnType; }
    TypeSymbol* ReturnType() { return returnType; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    bool ReturnsClassInterfaceOrClassDelegateByValue() const;
    void SetReturnParam(ParameterSymbol* returnParam_);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context);
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
    DelegateTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeDefaultConstructor(DelegateTypeSymbol* delegateType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeDefaultConstructor"; }
    void Check() override;
private:
    DelegateTypeSymbol* delegateType;
};

class DelegateTypeCopyConstructor : public FunctionSymbol
{
public:
    DelegateTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeCopyConstructor(DelegateTypeSymbol* delegateType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeCopyConstructor"; }
};

class DelegateTypeMoveConstructor : public FunctionSymbol
{
public:
    DelegateTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeMoveConstructor(DelegateTypeSymbol* delegateType_, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeMoveConstructor"; }
private:
    DelegateTypeSymbol* delegateType;
};

class DelegateTypeCopyAssignment : public FunctionSymbol
{
public:
    DelegateTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeCopyAssignment(DelegateTypeSymbol* delegateType, TypeSymbol* voidType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeCopyAssignment"; }
};

class DelegateTypeMoveAssignment : public FunctionSymbol
{
public:
    DelegateTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeMoveAssignment(DelegateTypeSymbol* delegateType_, TypeSymbol* voidType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeMoveAssignment"; }
private:
    DelegateTypeSymbol* delegateType;
};

class DelegateTypeReturn : public FunctionSymbol
{
public:
    DelegateTypeReturn(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeReturn(DelegateTypeSymbol* delegateType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeReturn"; }
};

class DelegateTypeEquality : public FunctionSymbol
{
public:
    DelegateTypeEquality(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeEquality(DelegateTypeSymbol* delegateType, TypeSymbol* boolType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeEquality"; }
};

class DelegateTypeLess : public FunctionSymbol
{
public:
    DelegateTypeLess(const soul::ast::Span& span_, const std::u32string& name_);
    DelegateTypeLess(DelegateTypeSymbol* delegateType, TypeSymbol* boolType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "DelegateTypeLess"; }
};

class FunctionToDelegateConversion : public FunctionSymbol
{
public:
    FunctionToDelegateConversion(const soul::ast::Span& span_, const std::u32string& name_);
    FunctionToDelegateConversion(TypeSymbol* sourceType_, TypeSymbol* targetType_, FunctionSymbol* function_, Context* context);
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
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
    DelegateToVoidPtrConversion(TypeSymbol* delegateType_, TypeSymbol* voidPtrType_, Context* context);
    ConversionType GetConversionType() const override { return ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    const char* ClassName() const override { return "DelegateToVoidPtrConversion"; }
private:
    TypeSymbol* delegateType;
    TypeSymbol* voidPtrType;
};

class VoidPtrToDelegateConversion : public FunctionSymbol
{
public:
    VoidPtrToDelegateConversion(TypeSymbol* voidPtrType_, TypeSymbol* delegateType_, TypeSymbol* ulongType_, Context* context);
    ConversionType GetConversionType() const override { return ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    const char* ClassName() const override { return "VoidPtrToDelegateConversion"; }
private:
    TypeSymbol* voidPtrType;
    TypeSymbol* delegateType;
    TypeSymbol* ulongType;
};

class ClassDelegateTypeSymbol : public TypeSymbol
{
public:
    ClassDelegateTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void AddMember(Symbol* member, Context* context) override;
    std::string TypeString(Context* context) const override { return "class_delegate"; }
    std::string Syntax(Context* context) override;
    std::u32string Id() const override;
    bool IsClassDelegateType() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
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
    ClassTypeSymbol* ObjectDelegatePairType() { Assert(objectDelegatePairType, "object delegate pair type not set"); return objectDelegatePairType; }
    FunctionSymbol* CopyConstructor() { Assert(copyConstructor, "class delegate copy constructor not set"); return copyConstructor; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context);
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
    ClassDelegateTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeDefaultConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
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
    ClassDelegateTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeCopyConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    bool IsClassDelegateCopyConstructor() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeCopyConstructor"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeMoveConstructor : public FunctionSymbol
{
public:
    ClassDelegateTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeMoveConstructor(ClassDelegateTypeSymbol* classDelegateType_, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeMoveConstructor"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeCopyAssignment : public FunctionSymbol
{
public:
    ClassDelegateTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeCopyAssignment(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* voidType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeCopyAssignment"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeMoveAssignment : public FunctionSymbol
{
public:
    ClassDelegateTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeMoveAssignment(ClassDelegateTypeSymbol* classDelegateType, TypeSymbol* voidType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeMoveAssignment"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeEquality : public FunctionSymbol
{
public:
    ClassDelegateTypeEquality(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeEquality(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* boolType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeEquality"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class ClassDelegateTypeLess : public FunctionSymbol
{
public:
    ClassDelegateTypeLess(const soul::ast::Span& span_, const std::u32string& name_);
    ClassDelegateTypeLess(ClassDelegateTypeSymbol* classDelegateType_, TypeSymbol* boolType, Context* context);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    const char* ClassName() const override { return "ClassDelegateTypeLess"; }
private:
    ClassDelegateTypeSymbol* classDelegateType;
};

class MemberFunctionToClassDelegateConversion : public FunctionSymbol
{
public:
    MemberFunctionToClassDelegateConversion(const soul::ast::Span& span_, const std::u32string& name_);
    MemberFunctionToClassDelegateConversion(const soul::ast::Span& span_, TypeSymbol* sourceType_, ClassDelegateTypeSymbol* targetType_, FunctionSymbol* function_, 
        Context* context);
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    bool IsMemberFunctionToClassDelegateConversion() const override { return true; }
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "MemberFunctionToClassDelegateConversion"; }
    void Check() override;
private:
    TypeSymbol* sourceType;
    ClassDelegateTypeSymbol* targetType;
    FunctionSymbol* function;
};

} // namespace cmajor::symbols
