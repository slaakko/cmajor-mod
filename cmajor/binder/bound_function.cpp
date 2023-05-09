// =================================
// Copyright (c) 2023 Seppo Laakko
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
    BoundNode(functionSymbol_->GetSourcePos(), functionSymbol_->SourceModuleId(), BoundNodeType::boundFunction), boundCompileUnit(boundCompileUnit_), 
    functionSymbol(functionSymbol_), hasGotos(false)
{
}

void BoundFunction::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from function", GetSourcePos(), ModuleId());
}

void BoundFunction::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to function", GetSourcePos(), ModuleId());
}

void BoundFunction::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundFunction::SetBody(std::unique_ptr<BoundCompoundStatement>&& body_)
{
    body = std::move(body_);
}

void BoundFunction::AddTemporaryDestructorCall(std::unique_ptr<BoundFunctionCall>&& destructorCall,
    BoundFunction* currentFunction, cmajor::symbols::ContainerScope* currentContainerScope, 
    const soul::ast::SourcePos& span, const util::uuid& moduleId)
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
                    std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                    GenerateDestructorImplementation(boundClass.get(), destructorSymbol, *boundCompileUnit, currentContainerScope, currentFunction, span, moduleId);
                    boundCompileUnit->AddBoundNode(std::move(boundClass));
                }
            }
        }
        else if (destructorSymbol->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            bool firstTry = GetBoundCompileUnit()->GetClassTemplateRepository().Instantiate(destructorSymbol, currentContainerScope, currentFunction, span, moduleId);
            if (!firstTry)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(destructorSymbol->Parent());
                std::lock_guard<std::recursive_mutex> lock(GetBoundCompileUnit()->GetModule().GetLock());
                cmajor::symbols::ClassTemplateSpecializationSymbol* copy = GetBoundCompileUnit()->GetSymbolTable().CopyClassTemplateSpecialization(specialization);
                GetBoundCompileUnit()->GetClassTemplateRepository().BindClassTemplateSpecialization(copy, currentContainerScope, span, moduleId);
                int index = destructorSymbol->GetIndex();
                cmajor::symbols::FunctionSymbol* functionSymbol = copy->GetFunctionByIndex(index);
                bool secondTry = GetBoundCompileUnit()->InstantiateClassTemplateMemberFunction(functionSymbol, currentContainerScope, currentFunction, span, moduleId);
                if (!secondTry)
                {
                    throw cmajor::symbols::Exception("internal error: could not instantiate destructor of a class template specialization '" + util::ToUtf8(specialization->FullName()) + "'",
                        specialization->GetSourcePos(), specialization->SourceModuleId());
                }
            }
        }
    }
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

void BoundFunction::SetLineCode(std::unique_ptr<BoundStatement>&& lineCode_)
{
    lineCode.reset(lineCode_.release());
}

} // namespace cmajor::binder
