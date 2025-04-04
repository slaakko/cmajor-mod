// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.compiling;

import cmajor.binder;
import cmajor.backend.selector;
import cmajor.ir;
import soul.ast.span;
import util;

namespace cmajor::build {

void CreateSymbols(cmajor::symbols::Context* context, cmajor::symbols::SymbolTable& symbolTable, cmajor::ast::Project* project, bool& stop)
{
    std::lock_guard<std::recursive_mutex> lock(symbolTable.GetModule()->Lock());
    cmajor::symbols::SymbolCreatorVisitor symbolCreator(symbolTable, context);
    for (const std::unique_ptr<cmajor::ast::CompileUnitNode>& compileUnit : project->CompileUnits())
    {
        if (stop)
        {
            return;
        }
        symbolTable.SetCurrentCompileUnit(compileUnit.get());
        compileUnit->Accept(symbolCreator);
    }
}

std::unique_ptr<cmajor::binder::BoundCompileUnit> BindTypes(cmajor::symbols::Context* context, cmajor::ast::CompileUnitNode* compileUnitNode, 
    cmajor::binder::AttributeBinder* attributeBinder)
{
    std::unique_ptr<cmajor::binder::BoundCompileUnit> boundCompileUnit(new cmajor::binder::BoundCompileUnit(context, compileUnitNode, attributeBinder));
    boundCompileUnit->PushBindingTypes();
    cmajor::binder::TypeBinder typeBinder(*boundCompileUnit);
    compileUnitNode->Accept(typeBinder);
    boundCompileUnit->PopBindingTypes();
    return boundCompileUnit;
}

std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> BindTypes(cmajor::symbols::Context* context, cmajor::ast::Project* project,
    cmajor::binder::AttributeBinder* attributeBinder, bool& stop)
{
    std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> boundCompileUnits;
    for (const std::unique_ptr<cmajor::ast::CompileUnitNode>& compileUnit : project->CompileUnits())
    {
        if (stop)
        {
            return std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>();
        }
        std::unique_ptr<cmajor::binder::BoundCompileUnit> boundCompileUnit = BindTypes(context, compileUnit.get(), attributeBinder);
        boundCompileUnits.push_back(std::move(boundCompileUnit));
    }
    return boundCompileUnits;
}

void BindStatements(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    cmajor::binder::StatementBinder statementBinder(boundCompileUnit);
    boundCompileUnit.GetCompileUnitNode()->Accept(statementBinder);
}

void AnalyzeControlFlow(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    cmajor::binder::AnalyzeControlFlow(boundCompileUnit);
}

void GenerateCode(cmajor::binder::BoundCompileUnit& boundCompileUnit, cmajor::ir::EmittingContext* emittingContext)
{
    cmajor::backend::BackEnd* backend = cmajor::backend::GetCurrentBackEnd();
    std::unique_ptr<cmajor::ir::Emitter> emitter = backend->CreateEmitter(emittingContext);
    std::unique_ptr<cmajor::codegen::CodeGenerator> codeGenerator = backend->CreateCodeGenerator(emitter.get());
    boundCompileUnit.Accept(*codeGenerator);
}

void CompileSingleThreaded(cmajor::ast::Project* project, cmajor::symbols::Context* context, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, std::vector<std::string>& asmFilePaths, std::map<int, cmdoclib::File>& docFileMap, bool& stop)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(project->LogStreamId(), "Compiling...");
    }
    context->RootModule()->StartBuild();
    int maxFileIndex = 0;
    for (std::unique_ptr<cmajor::binder::BoundCompileUnit>& boundCompileUnit : boundCompileUnits)
    {
        if (stop)
        {
            return;
        }
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(project->LogStreamId(), "> " + boundCompileUnit->GetCompileUnitNode()->FilePath());
        }
        BindStatements(*boundCompileUnit);
        if (boundCompileUnit->HasGotos())
        {
            cmajor::build::AnalyzeControlFlow(*boundCompileUnit);
        }
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            cmdoclib::GenerateSourceCode(project, boundCompileUnit.get(), docFileMap);
        }
        else
        {
            std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
            GenerateCode(*boundCompileUnit, emittingContext.get());
            if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
            {
                asmFilePaths.push_back(boundCompileUnit->AsmFilePath());
            }
            else 
            {
                objectFilePaths.push_back(boundCompileUnit->ObjectFilePath());
            }
        }
    }
    context->RootModule()->StopBuild();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(context->RootModule()->LogStreamId(), util::ToUtf8(context->RootModule()->Name()) + " compilation time: " + 
            util::FormatTimeMs(context->RootModule()->GetBuildTimeMs()));
    }
}

struct CompileData
{
    CompileData(cmajor::symbols::Context* context_, bool& stop_,util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*>& inputQueue_, 
        util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*>& outputQueue_) :
        context(context_), stop(stop_), inputQueue(inputQueue_), outputQueue(outputQueue_), exceptions()
    {
    }
    std::mutex mtx;
    cmajor::symbols::Context* context;
    bool& stop;
    util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*>& inputQueue;
    util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*>& outputQueue;
    std::vector<std::exception_ptr> exceptions;
};

void CompileThreadFunction(CompileData* compileData)
{
    try
    {
        cmajor::binder::BoundCompileUnit* compileUnit = compileData->inputQueue.Get();
        while (compileUnit && !compileData->stop)
        {
            std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
            GenerateCode(*compileUnit, emittingContext.get());
            compileData->outputQueue.Put(compileUnit);
            compileUnit = compileData->inputQueue.Get();
        }
    }
    catch (...)
    {
        std::lock_guard lock(compileData->mtx);
        compileData->exceptions.push_back(std::current_exception());
        compileData->inputQueue.Exit();
        compileData->outputQueue.Exit();
        compileData->stop = true;
    }
}

void CompileMultiThreaded(cmajor::ast::Project* project, cmajor::symbols::Context* context, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, std::vector<std::string>& asmFilePaths, bool& stop)
{
    int numThreads = std::min(static_cast<int>(std::thread::hardware_concurrency()), static_cast<int>(boundCompileUnits.size()));
    if (numThreads <= 0)
    {
        numThreads = 1;
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(project->LogStreamId(), "Compiling using " + std::to_string(numThreads) + " threads...");
    }
    context->RootModule()->StartBuild();
    int n = boundCompileUnits.size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundCompileUnit* compileUnit = boundCompileUnits[i].get();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(context->RootModule()->LogStreamId(), "> " + compileUnit->GetCompileUnitNode()->FilePath());
        }
        try
        {
            BindStatements(*compileUnit);
            if (compileUnit->HasGotos())
            {
                cmajor::build::AnalyzeControlFlow(*compileUnit);
            }
            compileUnit->SetImmutable();
        }
        catch (...)
        {
            stop = true;
            throw;
        }
    }
    util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*> inputQueue;
    util::SynchronizedQueue<cmajor::binder::BoundCompileUnit*> outputQueue;
    CompileData compileData(context, stop, inputQueue, outputQueue);
    for (const auto& compileUnit : boundCompileUnits)
    {
        inputQueue.Put(compileUnit.get());
    }
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread{ CompileThreadFunction, &compileData });
    }
    int numOutputsReceived = 0;
    while (numOutputsReceived < boundCompileUnits.size() && !stop)
    {
        cmajor::binder::BoundCompileUnit* compileUnit = outputQueue.Get();
        if (compileUnit)
        {
            ++numOutputsReceived;
        }
    }
    inputQueue.Exit();
    outputQueue.Exit();
    for (int i = 0; i < numThreads; ++i)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }
    for (int i = 0; i < compileData.exceptions.size(); ++i)
    {
        if (compileData.exceptions[i])
        {
            std::rethrow_exception(compileData.exceptions[i]);
        }
    }
    for (int i = 0; i < boundCompileUnits.size(); ++i)
    {
        cmajor::binder::BoundCompileUnit* compileUnit = boundCompileUnits[i].get();
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
        {
            asmFilePaths.push_back(compileUnit->AsmFilePath());
        }
        else
        {
            objectFilePaths.push_back(compileUnit->ObjectFilePath());
        }
    }
    context->RootModule()->StopBuild();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(context->RootModule()->LogStreamId(), util::ToUtf8(context->RootModule()->Name()) + " compilation time: " +
            util::FormatTimeMs(context->RootModule()->GetBuildTimeMs()));
    }
}

void Compile(cmajor::ast::Project* project, cmajor::symbols::Context* context, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, std::vector<std::string>& asmFilePaths, std::map<int, cmdoclib::File>& docFileMap, bool& stop)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile))
    {
        CompileSingleThreaded(project, context, boundCompileUnits, objectFilePaths, asmFilePaths, docFileMap, stop);
    }
    else
    {
        CompileMultiThreaded(project, context, boundCompileUnits, objectFilePaths, asmFilePaths, stop);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose) && 
        (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm ||
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin))
    {
        int functionsCompiled = 0;
        int functionsInlined = 0;
        for (const auto& boudCompileUnit : boundCompileUnits)
        {
            functionsCompiled += boudCompileUnit->TotalFunctions(); 
            functionsInlined += boudCompileUnit->FunctionsInlined();
        }
        util::LogMessage(context->RootModule()->LogStreamId(), std::to_string(functionsCompiled) + " functions compiled");
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
        {
            util::LogMessage(context->RootModule()->LogStreamId(), std::to_string(functionsInlined) + " functions inlined");
        }
    }
}

} // namespace cmajor::build
