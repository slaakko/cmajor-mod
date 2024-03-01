// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound_class;

import cmajor.binder.bound.function;
import cmajor.binder.bound.node.visitor;

namespace cmajor::binder {

BoundClass::BoundClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol_) :
    BoundNode(classTypeSymbol_->GetSpan(), BoundNodeType::boundClass), classTypeSymbol(classTypeSymbol_), inlineFunctionContainer(false)
{
}

void BoundClass::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundClass::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from class", GetFullSpan());
}

void BoundClass::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to class", GetFullSpan());
}

void BoundClass::AddMember(std::unique_ptr<BoundNode>&& member)
{
    member->SetParent(this);
    members.push_back(std::move(member));
}

bool BoundClass::ContainsSourceFunctions() const
{
    for (const auto& member : members)
    {
        if (member->GetBoundNodeType() == BoundNodeType::boundFunction)
        {
            BoundFunction* boundFunction = static_cast<BoundFunction*>(member.get());
            if (boundFunction->GetFunctionSymbol()->HasSource())
            {
                return true;
            }
        }
    }
    return false;
}

} // namespace cmajor::binder
