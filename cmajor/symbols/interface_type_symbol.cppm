// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.interfaces;

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import cmajor.symbols.classes;
import cmajor.symbols.variable.symbol;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import std.core;

export namespace cmajor::symbols {

class InterfaceTypeDefaultConstructor;
class InterfaceTypeCopyConstructor;
class InterfaceTypeMoveConstructor;
class InterfaceTypeCopyAssignment;
class InterfaceTypeMoveAssignment;
class InterfaceTypeEqual;

class InterfaceTypeSymbol : public TypeSymbol
{
public:
    InterfaceTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::string TypeString(Context* context) const override { return "interface"; }
    void AddMember(Symbol* member, Context* context) override;
    void Accept(SymbolCollector* collector) override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    const std::vector<MemberFunctionSymbol*>& MemberFunctions() const { return memberFunctions; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        MemberFunctionSymbol* interfaceMemberFunction, Context* contex);
    InterfaceTypeDefaultConstructor* DefaultConstructor() const { return defaultConstructor; }
    InterfaceTypeCopyConstructor* CopyConstructor() const { return copyConstructor; }
    InterfaceTypeMoveConstructor* MoveConstructor() const { return moveConstructor; }
    InterfaceTypeCopyAssignment* CopyAssignment() const { return copyAssignment; }
    InterfaceTypeMoveAssignment* MoveAssigment() const { return moveAssignment; }
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "InterfaceTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "IN"; }
    std::string GetSymbolCategoryDescription() const override { return "interface"; }
private:
    std::vector<MemberFunctionSymbol*> memberFunctions;
    InterfaceTypeDefaultConstructor* defaultConstructor;
    InterfaceTypeCopyConstructor* copyConstructor;
    InterfaceTypeMoveConstructor* moveConstructor;
    InterfaceTypeCopyAssignment* copyAssignment;
    InterfaceTypeMoveAssignment* moveAssignment;
};

class InterfaceTypeDefaultConstructor : public FunctionSymbol
{
public:
    InterfaceTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeDefaultConstructor(InterfaceTypeSymbol* interfaceType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeDefaultConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeCopyConstructor : public FunctionSymbol
{
public:
    InterfaceTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeCopyConstructor(InterfaceTypeSymbol* interfaceType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeCopyConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeMoveConstructor : public FunctionSymbol
{
public:
    InterfaceTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeMoveConstructor(InterfaceTypeSymbol* interfaceType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeMoveConstructor"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeCopyAssignment : public FunctionSymbol
{
public:
    InterfaceTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeCopyAssignment(InterfaceTypeSymbol* interfaceType_, TypeSymbol* voidType, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeCopyAssignment"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeMoveAssignment : public FunctionSymbol
{
public:
    InterfaceTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeMoveAssignment(InterfaceTypeSymbol* interfaceType_, TypeSymbol* voidType, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeMoveAssignment"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeEqual : public FunctionSymbol
{
public:
    InterfaceTypeEqual(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeEqual(InterfaceTypeSymbol* interfaceType_, TypeSymbol* boolType, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeEqual"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class InterfaceTypeLess : public FunctionSymbol
{
public:
    InterfaceTypeLess(const soul::ast::Span& span_, const std::u32string& name_);
    InterfaceTypeLess(InterfaceTypeSymbol* interfaceType_, TypeSymbol* boolType, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "InterfaceTypeLess"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

class ClassToInterfaceConversion : public FunctionSymbol
{
public:
    ClassToInterfaceConversion(const soul::ast::Span& span_, const std::u32string& name_);
    ClassToInterfaceConversion(ClassTypeSymbol* sourceClassType_, InterfaceTypeSymbol* targetInterfaceType_, int32_t interfaceIndex_, const soul::ast::Span& span_, 
        Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context, bool add) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
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
    GetObjectPtrFromInterface(const soul::ast::Span& span_, const std::u32string& name_);
    GetObjectPtrFromInterface(InterfaceTypeSymbol* interfaceTypeSymbol, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "GetObjectPtrFromInterface"; }
private:
    InterfaceTypeSymbol* interfaceType;
};

} // namespace cmajor::symbols
