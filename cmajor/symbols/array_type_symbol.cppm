export module cmajor.symbols.array.type.symbol;

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import soul.ast.source.pos;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;

export namespace cmajor::symbols {

class ArrayTypeSymbol : public TypeSymbol
{
public:
    ArrayTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_, TypeSymbol* elementType_, int64_t size_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsArrayType() const override { return true; }
    TypeSymbol* ElementType() const { return elementType; }
    int64_t Size() const { return size; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "ArrayTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "AR"; }
    std::string GetSymbolCategoryDescription() const override { return "array type"; }
private:
    TypeSymbol* elementType;
    int64_t size;
};

class ArrayLengthFunction : public FunctionSymbol
{
public:
    ArrayLengthFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayLengthFunction(ArrayTypeSymbol* arrayType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    std::unique_ptr<Value> ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, Value* receiver) const override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsCompileTimePrimitiveFunction() const override { return true; }
    const char* ClassName() const override { return "ArrayLengthFunction"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};

class ArrayBeginFunction : public FunctionSymbol
{
public:
    ArrayBeginFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayBeginFunction(ArrayTypeSymbol* arrayType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayBeginFunction"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};

class ArrayEndFunction : public FunctionSymbol
{
public:
    ArrayEndFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayEndFunction(ArrayTypeSymbol* arrayType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayEndFunction"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};

class ArrayCBeginFunction : public FunctionSymbol
{
public:
    ArrayCBeginFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayCBeginFunction(ArrayTypeSymbol* arrayType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayCBeginFunction"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};

class ArrayCEndFunction : public FunctionSymbol
{
public:
    ArrayCEndFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ArrayCEndFunction(ArrayTypeSymbol* arrayType_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayCEndFunction"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};

class ArrayTypeDefaultConstructor : public FunctionSymbol
{
public:
    ArrayTypeDefaultConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeDefaultConstructor_);
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void SetTemporariesForElementTypeDefaultCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flag, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeDefaultConstructor"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
    FunctionSymbol* elementTypeDefaultConstructor;
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temporariesForElementTypeDefaultCtor;
};

class ArrayTypeCopyConstructor : public FunctionSymbol
{
public:
    ArrayTypeCopyConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeCopyConstructor_);
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void SetTemporariesForElementTypeCopyCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeCopyConstructor"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
    FunctionSymbol* elementTypeCopyConstructor;
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temporariesForElementTypeCopyConstructor;
};

class ArrayTypeMoveConstructor : public FunctionSymbol
{
public:
    ArrayTypeMoveConstructor(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeMoveConstructor_);
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void SetTemporariesForElementTypeMoveCtor(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeMoveConstructor"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
    FunctionSymbol* elementTypeMoveConstructor;
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temporariesForElementTypeMoveConstructor;
};

class ArrayTypeCopyAssignment : public FunctionSymbol
{
public:
    ArrayTypeCopyAssignment(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeCopyAssignment_);
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void SetTemporariesForElementTypeCopyAssignment(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeCopyAssignment"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
    FunctionSymbol* elementTypeCopyAssignment;
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temporariesForElementTypeCopyAssignment;
};

class ArrayTypeMoveAssignment : public FunctionSymbol
{
public:
    ArrayTypeMoveAssignment(ArrayTypeSymbol* arrayType_, FunctionSymbol* elementTypeMoveAssignment_);
    std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction) override;
    void SetTemporariesForElementTypeMoveAssignment(std::vector<std::unique_ptr<cmajor::ir::GenObject>>&& temporaries);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeMoveAssignment"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
    FunctionSymbol* elementTypeMoveAssignment;
    std::vector<std::unique_ptr<cmajor::ir::GenObject>> temporariesForElementTypeMoveAssignment;
};

class ArrayTypeElementAccess : public FunctionSymbol
{
public:
    ArrayTypeElementAccess(ArrayTypeSymbol* arrayType_);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsArrayElementAccess() const override { return true; }
    const char* ClassName() const override { return "ArrayTypeElementAccess"; }
    void Check() override;
private:
    ArrayTypeSymbol* arrayType;
};
} // namespace cmajor::symbols
