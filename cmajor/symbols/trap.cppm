// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.trap;

import cmajor.symbols.function.symbol;
import soul.ast.span;
import cmajor.ir.emitter;

import std.core;

export namespace cmajor::symbols {

class SymbolTable;

class TrapFunction : public FunctionSymbol
{
public:
    TrapFunction(SymbolTable& symbolTable, Context* context);
    TrapFunction(const soul::ast::Span& span_, const std::u32string& name_);
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
};

void InitTrap(SymbolTable& symbolTable, Context* context);

} // namespace cmajor::symbols

