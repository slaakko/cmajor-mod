// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.compiling;

import cmajor.binder;
import cmajor.backend.selector;
import cmajor.ir;
import util;

namespace cmajor::build {

void CreateSymbols(cmajor::symbols::SymbolTable& symbolTable, cmajor::ast::Project* project, bool& stop)
{
    cmajor::symbols::SymbolCreatorVisitor symbolCreator(symbolTable);
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

std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> BindTypes(cmajor::symbols::Module* module, cmajor::ast::Project* project,
    cmajor::binder::AttributeBinder* attributeBinder, bool& stop)
{
    std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> boundCompileUnits;
    for (const std::unique_ptr<cmajor::ast::CompileUnitNode>& compileUnit : project->CompileUnits())
    {
        if (stop)
        {
            return std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>();
        }
        std::unique_ptr<cmajor::binder::BoundCompileUnit> boundCompileUnit(new cmajor::binder::BoundCompileUnit(*module, compileUnit.get(), attributeBinder));
        boundCompileUnit->PushBindingTypes();
        cmajor::binder::TypeBinder typeBinder(*boundCompileUnit);
        compileUnit->Accept(typeBinder);
        boundCompileUnit->PopBindingTypes();
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

void CompileSingleThreaded(cmajor::ast::Project* project, cmajor::symbols::Module* module, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, bool& stop)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(project->LogStreamId(), "Compiling...");
    }
    module->StartBuild();
    int maxFileIndex = 0;
    for (std::unique_ptr<cmajor::binder::BoundCompileUnit>& boundCompileUnit : boundCompileUnits)
    {
        if (stop)
        {
            return;
        }
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(project->LogStreamId(), "> " + std::filesystem::path(boundCompileUnit->GetCompileUnitNode()->FilePath()).filename().generic_string());
        }
        BindStatements(*boundCompileUnit);
        if (boundCompileUnit->HasGotos())
        {
            cmajor::build::AnalyzeControlFlow(*boundCompileUnit);
        }
        std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
        GenerateCode(*boundCompileUnit, emittingContext.get());
        objectFilePaths.push_back(boundCompileUnit->ObjectFilePath());
    }
    module->StopBuild();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(module->LogStreamId(), util::ToUtf8(module->Name()) + " compilation time: " + util::FormatTimeMs(module->GetBuildTimeMs()));
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
    CompileData(std::mutex* mtx_, cmajor::symbols::Module* module_, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits_,
        std::vector<std::string>& objectFilePaths_, bool& stop_, bool& ready_, int numThreads_, CompileQueue& input_, CompileQueue& output_) :
        mtx(mtx_), module(module_), boundCompileUnits(boundCompileUnits_), objectFilePaths(objectFilePaths_), stop(stop_), ready(ready_), numThreads(numThreads_),
        input(input_), output(output_)
    {
        exceptions.resize(numThreads);
        sourceFileFilePaths.resize(boundCompileUnits.size());
        for (int i = 0; i < boundCompileUnits.size(); ++i)
        {
            sourceFileFilePaths[i] = boundCompileUnits[i]->GetCompileUnitNode()->FilePath();
        }
    }
    std::mutex* mtx;
    cmajor::symbols::Module* module;
    std::vector<std::string> sourceFileFilePaths;
    std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits;
    std::vector<std::string>& objectFilePaths;
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
        SetRootModuleForCurrentThread(data->module);
        while (!data->stop && !data->ready)
        {
            int compileUnitIndex = data->input.Get();
            if (compileUnitIndex >= 0 && compileUnitIndex < data->boundCompileUnits.size())
            {
                cmajor::binder::BoundCompileUnit* boundCompileUnit = data->boundCompileUnits[compileUnitIndex].get();
                std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
                GenerateCode(*boundCompileUnit, emittingContext.get()); 
                {
                    std::lock_guard<std::mutex> lock(*data->mtx);
                    data->objectFilePaths.push_back(boundCompileUnit->ObjectFilePath());
                    data->boundCompileUnits[compileUnitIndex].reset();
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

void CompileMultiThreaded(cmajor::ast::Project* project, cmajor::symbols::Module* module, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, bool& stop)
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
    module->StartBuild();
    bool ready = false;
    CompileQueue input(&mtx, "input", stop, ready, module->LogStreamId());
    CompileQueue output(&mtx, "output", stop, ready, module->LogStreamId());
    CompileData compileData(&mtx, module, boundCompileUnits, objectFilePaths, stop, ready, numThreads, input, output);
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(std::thread{ CompileThreadFunction, &compileData, i });
    }
    int n = boundCompileUnits.size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundCompileUnit* compileUnit = boundCompileUnits[i].get();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(module->LogStreamId(), "> " + compileUnit->GetCompileUnitNode()->FilePath());
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
            input.NotifyAll();
            for (int i = 0; i < numThreads; ++i)
            {
                if (threads[i].joinable())
                {
                    threads[i].join();
                }
            }
            throw;
        }
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
    module->StopBuild();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(module->LogStreamId(), util::ToUtf8(module->Name()) + " compilation time: " + util::FormatTimeMs(module->GetBuildTimeMs()));
    }
}

void Compile(cmajor::ast::Project* project, cmajor::symbols::Module* module, std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>>& boundCompileUnits,
    std::vector<std::string>& objectFilePaths, bool& stop)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile))
    {
        CompileSingleThreaded(project, module, boundCompileUnits, objectFilePaths, stop);
    }
    else
    {
        CompileMultiThreaded(project, module, boundCompileUnits, objectFilePaths, stop);
    }
}

} // namespace cmajor::build
