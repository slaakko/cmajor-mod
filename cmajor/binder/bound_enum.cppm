// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.enumerations;

import std.core;
import cmajor.symbols;
import cmajor.ir;
import cmajor.binder.bound.node;

export namespace cmajor::binder {

class BoundEnumTypeDefinition : public BoundNode
{
public:
    BoundEnumTypeDefinition(cmajor::symbols::EnumTypeSymbol* enumTypeSymbol_);
    void Accept(BoundNodeVisitor& visitor) override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    cmajor::symbols::EnumTypeSymbol* GetEnumTypeSymbol() const { return enumTypeSymbol; }
private:
    cmajor::symbols::EnumTypeSymbol* enumTypeSymbol;
};

} // namespace cmajor::binder
