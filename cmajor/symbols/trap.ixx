module;
#include <boost/uuid/uuid.hpp>

export module cmajor.symbols.trap;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.function.symbol;
import soul.ast.source.pos;
import cmajor.ir.emitter;

import std.core;

export namespace cmajor::symbols {

    class SymbolTable;

    class TrapFunction : public FunctionSymbol
    {
    public:
        TrapFunction(SymbolTable& symbolTable);
        TrapFunction(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& sourceModuleId, const std::u32string& name_);
        void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    };

    void InitTrap(SymbolTable& symbolTable);

} // namespace cmajor::symbols

