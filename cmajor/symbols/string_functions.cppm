// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.string.functions;

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import cmajor.symbols.variable.symbol;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import std.core;

export namespace cmajor::symbols {

class StringFunctionContainerSymbol : public TypeSymbol
{
public:
    StringFunctionContainerSymbol(const soul::ast::Span& span_, Context* context);
    StringFunctionContainerSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsStringFunctionContainer() const override { return true; }
};

class StringLengthFunction : public FunctionSymbol
{
public:
    StringLengthFunction(TypeSymbol* parentType, Context* context);
    StringLengthFunction(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    std::unique_ptr<Value> ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::Span& span, Value* receiver) const override;
    bool IsBasicTypeOperation() const override { return true; }
    bool IsCompileTimePrimitiveFunction() const override { return true; }
};
} // namespace cmajor::symbols
