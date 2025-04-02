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

class CompileQueue
{
public:
    CompileQueue(std::mutex* mtx_, const std::string& name_, bool& stop_, bool& ready_, int logStreamId_);
    void Put(int compileUnitIndex);
    int Get();
    void NotifyAll();
private:
    std::mutex* mtx;
    std::string name;
    std::list<int> queue;
    std::condition_variable cond;
    bool& stop;
    bool& ready;
    int logStreamId;
};

CompileQueue::CompileQueue(std::mutex* mtx_, const std::string& name_, bool& stop_, bool& ready_, int logStreamId_) :
    mtx(mtx_), name(name_), stop(stop_), ready(ready_), logStreamId(logStreamId_)
{
}

void CompileQueue::Put(int compileUnitIndex)
{
    std::lock_guard<std::mutex> lock(*mtx);
    queue.push_back(compileUnitIndex);
    cond.notify_one();
}

int CompileQueue::Get()
{
    while (!stop && !ready)
    {
        std::unique_lock<std::mutex> lock(*mtx);
        cond.wait(lock, [this] { return stop || ready || !queue.empty(); });
        if (stop || ready) return -1;
        int compileUnitIndex = queue.front();
        queue.pop_front();
        return compileUnitIndex;
    }
    return -1;
}

void CompileQueue::NotifyAll()
{
    cond.notify_all();
}

struct CompileData
{
    CompileData(std::mutex* mtx_, cmajor::symbols::Context* context_, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits_,
        std::vector<std::string>& objectFilePaths_, std::vector<std::string>& asmFilePaths_, bool& stop_, bool& ready_, int numThreads_, CompileQueue& input_, CompileQueue& output_) :
        mtx(mtx_), context(context_), boundCompileUnits(boundCompileUnits_), objectFilePaths(objectFilePaths_), asmFilePaths(asmFilePaths_), stop(stop_), ready(ready_), 
        numThreads(numThreads_), input(input_), output(output_)
    {
        exceptions.resize(numThreads);
        sourceFileFilePaths.resize(boundCompileUnits.size());
        for (int i = 0; i < boundCompileUnits.size(); ++i)
        {
            sourceFileFilePaths[i] = boundCompileUnits[i]->GetCompileUnitNode()->FilePath();
        }
    }
    std::mutex* mtx;
    cmajor::symbols::Context* context;
    std::vector<std::string> sourceFileFilePaths;
    std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits;
    std::vector<std::string>& objectFilePaths;
    std::vector<std::string>& asmFilePaths;
    bool& stop;
    bool& ready;
    int numThreads;
    CompileQueue& input;
    CompileQueue& output;
    std::vector<std::exception_ptr> exceptions;
};

void CompileThreadFunction(CompileData* data, int threadId)
{
    try
    {
        //SetRootModuleForCurrentThread(data->module);
        while (!data->stop && !data->ready)
        {
            int compileUnitIndex = data->input.Get();
            if (compileUnitIndex >= 0 && compileUnitIndex < data->boundCompileUnits.size())
            {
                cmajor::binder::BoundCompileUnit* boundCompileUnit = data->boundCompileUnits[compileUnitIndex].get();
                cmajor::symbols::Context compileUnitContext;
                compileUnitContext.SetRootModule(data->context->RootModule());
                boundCompileUnit->SetContext(&compileUnitContext);
                std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(
                    cmajor::symbols::GetOptimizationLevel());
                GenerateCode(*boundCompileUnit, emittingContext.get()); 
                {
                    std::lock_guard<std::mutex> lock(*data->mtx);
                    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
                    {
                        data->asmFilePaths.push_back(boundCompileUnit->AsmFilePath());
                    }
                    else
                    {
                        data->objectFilePaths.push_back(boundCompileUnit->ObjectFilePath());
                    }
                }
                data->output.Put(compileUnitIndex);
            }
        }

    }
    catch (...)
    {
        std::lock_guard<std::mutex> lock(*data->mtx);
        std::exception_ptr exception = std::current_exception();
        if (threadId >= 0 && threadId < data->exceptions.size())
        {
            data->exceptions[threadId] = exception;
        }
        data->stop = true;
        data->output.NotifyAll();
    }
}

std::mutex mtx;

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
    bool ready = false;
    CompileQueue input(&mtx, "input", stop, ready, context->RootModule()->LogStreamId());
    CompileQueue output(&mtx, "output", stop, ready, context->RootModule()->LogStreamId());
    CompileData compileData(&mtx, context, boundCompileUnits, objectFilePaths, asmFilePaths, stop, ready, numThreads, input, output);
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread{ CompileThreadFunction, &compileData, i });
    }
    for (int i = 0; i < n; ++i)
    {
        input.Put(i);
    }
    int numOutputsReceived = 0;
    while (numOutputsReceived < n && !stop)
    {
        int compileUnitIndex = output.Get();  
        if (compileUnitIndex != -1) 
        {
            ++numOutputsReceived;
        } 
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
        compileData.input.NotifyAll();
    }
    for (int i = 0; i < numThreads; ++i)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    } 
    for (int i = 0; i < numThreads; ++i)
    {
        if (compileData.exceptions[i])
        {
            std::rethrow_exception(compileData.exceptions[i]);
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
