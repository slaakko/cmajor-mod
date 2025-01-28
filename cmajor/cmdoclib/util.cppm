// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.util;

import cmdoclib.file;
import cmajor.symbols;
import std.core;

export namespace cmdoclib {

struct FileByName
{
    bool operator()(File* left, File* right) const
    {
        return left->name < right->name;
    }
};

struct ByCodeName
{
    bool operator()(cmajor::symbols::Symbol* left, cmajor::symbols::Symbol* right) const
    {
        if (left->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol) return true;
        if (right->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol) return false;
        if (left->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol && right->GetSymbolType() != cmajor::symbols::SymbolType::constructorSymbol) return true;
        if (right->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol && left->GetSymbolType() != cmajor::symbols::SymbolType::constructorSymbol) return false;
        if (left->CodeName() < right->CodeName()) return true;
        if (right->CodeName() < left->CodeName()) return false;
        if (left->GetSymbolType() == cmajor::symbols::SymbolType::conceptSymbol && right->GetSymbolType() == cmajor::symbols::SymbolType::conceptSymbol)
        {
            cmajor::symbols::ConceptSymbol* leftConcecpt = static_cast<cmajor::symbols::ConceptSymbol*>(left);
            cmajor::symbols::ConceptSymbol* rightConcecpt = static_cast<cmajor::symbols::ConceptSymbol*>(right);
            return leftConcecpt->Arity() < rightConcecpt->Arity();
        }
        if (left->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol && 
            right->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* leftSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(left);
            cmajor::symbols::ClassTemplateSpecializationSymbol* rightSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(right);
            return leftSpecialization->TemplateArgumentTypes().size() < rightSpecialization->TemplateArgumentTypes().size();
        }
        if (left->IsFunctionSymbol() && right->IsFunctionSymbol())
        {
            cmajor::symbols::FunctionSymbol* leftFun = static_cast<cmajor::symbols::FunctionSymbol*>(left);
            cmajor::symbols::FunctionSymbol* rightFun = static_cast<cmajor::symbols::FunctionSymbol*>(right);
            int leftArity = leftFun->Arity();
            int rightArity = rightFun->Arity();
            int n = std::min(leftArity, rightArity);
            for (int i = 0; i < n; ++i)
            {
                cmajor::symbols::ParameterSymbol* leftParam = leftFun->Parameters()[i];
                cmajor::symbols::ParameterSymbol* rightParam = rightFun->Parameters()[i];
                if (leftParam->GetType()->CodeName() < rightParam->GetType()->CodeName()) return true;
                if (rightParam->GetType()->CodeName() < leftParam->GetType()->CodeName()) return false;
            }
            if (leftArity < rightArity) return true;
            if (rightArity < leftArity) return false;
            if (leftFun->TemplateParameters().size() < rightFun->TemplateParameters().size()) return true;
            if (rightFun->TemplateParameters().size() < leftFun->TemplateParameters().size()) return false;
            if (!leftFun->Constraint() && rightFun->Constraint()) return true;
            if (!rightFun->Constraint() && leftFun->Constraint()) return false;
            leftFun->SetFlag(cmajor::symbols::FunctionSymbolFlags::includeConstraint);
            rightFun->SetFlag(cmajor::symbols::FunctionSymbolFlags::includeConstraint);
            return leftFun->Constraint()->ToString() < rightFun->Constraint()->ToString();
        }
        return left->Name() < right->Name();
    }
};

} // namespace cmdoclib
