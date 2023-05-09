// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound.enumerations;

import cmajor.binder.bound.node.visitor;

namespace cmajor::binder {

BoundEnumTypeDefinition::BoundEnumTypeDefinition(cmajor::symbols::EnumTypeSymbol* enumTypeSymbol_) : 
    BoundNode(enumTypeSymbol_->GetSourcePos(), enumTypeSymbol_->SourceModuleId(), BoundNodeType::boundEnumTypeDefinition), enumTypeSymbol(enumTypeSymbol_)
{
}

void BoundEnumTypeDefinition::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundEnumTypeDefinition::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from enum type", GetSourcePos(), ModuleId());
}

void BoundEnumTypeDefinition::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to enum type", GetSourcePos(), ModuleId());
}

} // namespace cmajor::binder
