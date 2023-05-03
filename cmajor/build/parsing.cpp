// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.parsing;

import cmajor.lexers;
import cmajor.parsers;
import util;

namespace cmajor::build {

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(int fileIndex, soul::lexer::FileMap& fileMap) 
{
    std::string sourceFilePath = fileMap.GetFilePath(fileIndex);
    std::string content = util::ReadFile(sourceFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), sourceFilePath);
    int32_t file = fileMap.MapFile(sourceFilePath);
    lexer.SetFile(file);
    using LexerType = decltype(lexer);
    cmajor::parser::context::Context context;
    std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::compile::unit::parser::CompileUnitParser<LexerType>::Parse(lexer, &context);
    fileMap.AddFileContent(file, std::move(ucontent), lexer.GetLineStartIndeces());
    return compileUnit;
}

std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, const std::string& toolChain)
{
    std::string content = util::ReadFile(projectFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), projectFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Project> project = cmajor::projects::parser::ProjectParser<LexerType>::Parse(lexer, config, backend, toolChain);
    project->ResolveDeclarations();
    return project;
}

std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath)
{
    std::string content = util::ReadFile(solutionFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), solutionFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Solution> solution = cmajor::solutions::parser::SolutionParser<LexerType>::Parse(lexer);
    solution->ResolveDeclarations();
    return solution;
}

void ParseSingleThreaded(cmajor::ast::Project* project, const std::vector<int>& fileIndeces, soul::lexer::FileMap& fileMap)
{
    for (int fileIndex : fileIndeces)
    {
        std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = ParseSourceFile(fileIndex, fileMap);
        project->AddCompileUnit(compileUnit.release());
    }
}

struct ParsingThreadData
{
    ParsingThreadData(soul::lexer::FileMap& fileMap_, int size_) : fileMap(fileMap_), stop(false) { compileUnits.resize(size_); exceptions.resize(size_); }
    soul::lexer::FileMap& fileMap;
    std::list<int> fileIndexQueue;
    std::mutex mtx;
    bool stop;
    std::vector<std::unique_ptr<cmajor::ast::CompileUnitNode>> compileUnits;
    std::vector<std::exception_ptr> exceptions;
};

void ParseThreadFunc(ParsingThreadData* threadData)
{
    while (!threadData->stop)
    {
        int fileIndex = -1;
        {
            std::lock_guard lock(threadData->mtx);
            if (threadData->fileIndexQueue.empty())
            {
                return;
            }
            else
            {
                fileIndex = threadData->fileIndexQueue.front();
                threadData->fileIndexQueue.pop_front();
            }
        }
        try
        {
            std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = ParseSourceFile(fileIndex, threadData->fileMap);
            threadData->compileUnits[fileIndex] = std::move(compileUnit);
        }
        catch (...)
        {
            threadData->exceptions[fileIndex] = std::current_exception();
            threadData->stop = true;
        }
    }
}

void ParseMultiThreaded(cmajor::ast::Project* project, const std::vector<int>& fileIndeces, soul::lexer::FileMap& fileMap)
{
    ParsingThreadData threadData(fileMap, fileIndeces.size());
    for (int fileIndex : fileIndeces)
    {
        threadData.fileIndexQueue.push_back(fileIndex);
    }
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        std::thread thread(ParseThreadFunc, &threadData);
        threads.push_back(std::move(thread));
    }
    for (auto& thread : threads)
    {
        thread.join();
    }
    for (auto& exception : threadData.exceptions)
    {
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    for (auto& compileUnit : threadData.compileUnits)
    {
        project->AddCompileUnit(compileUnit.release());
    }
}

std::unique_ptr<cmajor::ast::Project> ParseProject(Flags flags, const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend,
    const std::string& toolChain, soul::lexer::FileMap& fileMap)
{
    std::unique_ptr<cmajor::ast::Project> project = ParseProjectFile(projectFilePath, config, backend, toolChain);
    std::vector<int> fileIndeces;
    for (const auto& sourceFilePath : project->SourceFilePaths())
    {
        fileIndeces.push_back(fileMap.MapFile(sourceFilePath));
    }
    if ((flags & Flags::singleThreaded) != Flags::none)
    {
        ParseSingleThreaded(project.get(), fileIndeces, fileMap);
    }
    else
    {
        ParseMultiThreaded(project.get(), fileIndeces, fileMap);
    }
    return project;
}

} // namespace cmajor::build
