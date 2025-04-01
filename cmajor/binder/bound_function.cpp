// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound.function;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.bound.statement;
import cmajor.binder.bound.node.visitor;
import cmajor.binder.bound.expression;
import cmajor.binder.bound_class;
import util;

namespace cmajor::binder {

BoundFunction::BoundFunction(BoundCompileUnit* boundCompileUnit_, cmajor::symbols::FunctionSymbol* functionSymbol_) :
    BoundNode(functionSymbol_->GetSpan(), BoundNodeType::boundFunction), boundCompileUnit(boundCompileUnit_), 
    functionSymbol(functionSymbol_), hasGotos(false)
{
}

void BoundFunction::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from function", GetFullSpan());
}

void BoundFunction::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to function", GetFullSpan());
}

void BoundFunction::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundFunction::SetBody(std::unique_ptr<BoundCompoundStatement>&& body_)
{
    body = std::move(body_);
    body->SetParent(this);
}

void BoundFunction::AddTemporaryDestructorCall(std::unique_ptr<BoundFunctionCall>&& destructorCall,
    BoundFunction* currentFunction, cmajor::symbols::ContainerScope* currentContainerScope, 
    cmajor::ast::Node* node)
{
    cmajor::symbols::FunctionSymbol* functionSymbol = destructorCall->GetFunctionSymbol();
    if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
    {
        cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(functionSymbol);
        if (destructorSymbol->IsGeneratedFunction() && !GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
        {
            if (destructorSymbol->Parent()->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(destructorSymbol->Parent());
                if (!boundCompileUnit->IsGeneratedDestructorInstantiated(destructorSymbol))
                {
                    boundCompileUnit->SetGeneratedDestructorInstantiated(destructorSymbol);
                    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || 
                        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                    {
                        cmajor::symbols::DestructorSymbol* copy = static_cast<cmajor::symbols::DestructorSymbol*>(destructorSymbol->Copy());
                        boundCompileUnit->GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copy));
                        cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit->GetCompileUnitNode();
                        if (compileUnitNode)
                        {
                            copy->SetCompileUnitId(compileUnitNode->Id());
                            copy->ComputeMangledName(boundCompileUnit->GetContext());
                        }
                        destructorSymbol->SetInstantiatedName(copy->MangledName());
                        destructorSymbol = copy;
                        destructorCall->SetFunctionSymbol(destructorSymbol);
                    }
                    std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                    GenerateDestructorImplementation(boundClass.get(), destructorSymbol, *boundCompileUnit, currentContainerScope, currentFunction, node);
                    boundCompileUnit->AddBoundNode(std::move(boundClass));
                }
            }
        }
        else if (destructorSymbol->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::DestructorSymbol* instantiatedDestructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(
                GetBoundCompileUnit()->GetClassTemplateRepository().Instantiate(destructorSymbol, currentContainerScope, currentFunction, node));
            if (!instantiatedDestructorSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(destructorSymbol->Parent());
                std::lock_guard<std::recursive_mutex> lock(GetBoundCompileUnit()->GetModule().GetLock());
                cmajor::symbols::ClassTemplateSpecializationSymbol* copy = GetBoundCompileUnit()->GetSymbolTable().CopyClassTemplateSpecialization(specialization);
                GetBoundCompileUnit()->GetClassTemplateRepository().BindClassTemplateSpecialization(copy, currentContainerScope, node);
                int index = destructorSymbol->GetIndex();
                cmajor::symbols::FunctionSymbol* functionSymbol = copy->GetFunctionByIndex(index);
                functionSymbol = GetBoundCompileUnit()->InstantiateClassTemplateMemberFunction(functionSymbol, currentContainerScope, currentFunction, node);
                if (!functionSymbol)
                {
                    throw cmajor::symbols::Exception("internal error: could not instantiate destructor of a class template specialization '" + 
                        util::ToUtf8(specialization->FullName()) + "'", specialization->GetFullSpan());
                }
                destructorCall->SetFunctionSymbol(functionSymbol);
            }
            else
            {
                destructorSymbol = instantiatedDestructorSymbol;
                destructorCall->SetFunctionSymbol(destructorSymbol);
            }
        }
    }
    destructorCall->SetParent(this);
    temporaryDestructorCalls.push_back(std::move(destructorCall));
}

void BoundFunction::MoveTemporaryDestructorCallsTo(BoundExpression& expression)
{
    for (std::unique_ptr<BoundFunctionCall>& destructorCall : temporaryDestructorCalls)
    {
        expression.AddTemporaryDestructorCall(std::move(destructorCall));
    }
    temporaryDestructorCalls.clear();
}

void BoundFunction::AddLabeledStatement(BoundStatement* labeledStatement)
{
    labeledStatements.push_back(labeledStatement);
}

void BoundFunction::SetEnterCode(std::vector<std::unique_ptr<BoundStatement>>&& enterCode_)
{
    enterCode = std::move(enterCode_);
}

void BoundFunction::SetExitCode(std::vector<std::unique_ptr<BoundStatement>>&& exitCode_)
{
    exitCode = std::move(exitCode_);
}

void BoundFunction::SetCheckerCode(std::vector<std::unique_ptr<BoundStatement>>&& checkerCode_)
{
    checkerCode = std::move(checkerCode_);
}

void BoundFunction::SetLineCode(std::unique_ptr<BoundStatement>&& lineCode_)
{
    lineCode.reset(lineCode_.release());
}

} // namespace cmajor::binder
