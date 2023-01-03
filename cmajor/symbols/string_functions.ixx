export module cmajor.symbols.string.functions;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import std.core;

export namespace cmajor::symbols {

class StringFunctionContainerSymbol : public TypeSymbol
{
public:
    StringFunctionContainerSymbol();
    StringFunctionContainerSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsStringFunctionContainer() const override { return true; }
};

class StringLengthFunction : public FunctionSymbol
{
public:
    StringLengthFunction(TypeSymbol* parentType);
    StringLengthFunction(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    std::unique_ptr<Value> ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, Value* receiver) const override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsCompileTimePrimitiveFunction() const override { return true; }
};
} // namespace cmajor::symbols
