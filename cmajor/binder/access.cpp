// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.access;

import util;

namespace cmajor::binder {

bool HasAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol)
{
    if (!fromFunction) return false;
    cmajor::symbols::FunctionSymbol* toContainingFunction = toSymbol->ContainingFunctionNoThrow();
    if (toContainingFunction)
    {
        if (fromFunction == toContainingFunction || cmajor::symbols::FunctionSymbolsEqual()(fromFunction, toContainingFunction))
        {
            return true;
        }
    }
    cmajor::symbols::ClassTypeSymbol* toContainingClass = toSymbol->ContainingClassNoThrow();
    if (toContainingClass)
    {
        if (!HasAccess(fromFunction, toContainingClass)) return false;
    }
    switch (toSymbol->DeclaredAccess())
    {
    case cmajor::symbols::SymbolAccess::public_: return true;
    case cmajor::symbols::SymbolAccess::protected_:
    {
        cmajor::symbols::ClassTypeSymbol* fromContainingClass = fromFunction->ContainingClassNoThrow();
        if (fromContainingClass)
        {
            if (toContainingClass && toContainingClass->IsSameParentOrAncestorOf(fromContainingClass))
            {
                return true;
            }
            if (fromContainingClass->HasBaseClass(toContainingClass))
            {
                return true;
            }
        }
        break;
    }
    case cmajor::symbols::SymbolAccess::internal_:
    {
        if (fromFunction->GetModule() == toSymbol->GetModule())
        {
            return true;;
        }
        break;
    }
    case cmajor::symbols::SymbolAccess::private_:
    {
        if (toContainingClass)
        {
            cmajor::symbols::ClassTypeSymbol* fromContainingClass = fromFunction->ContainingClassNoThrow();
            if (fromContainingClass)
            {
                if (toContainingClass->IsSameParentOrAncestorOf(fromContainingClass))
                {
                    return true;
                }
            }
        }
        break;
    }
    }
    return false;
}

void CheckAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol)
{
    if (!HasAccess(fromFunction, toSymbol))
    {
        throw cmajor::symbols::Exception(toSymbol->TypeString() + " '" + util::ToUtf8(toSymbol->FullName()) + "' is inaccessible due to its protection level",
            fromFunction->GetSourcePos(), fromFunction->SourceModuleId(), toSymbol->GetSourcePos(), toSymbol->SourceModuleId());
    }
}

} // namespace cmajor::binder
