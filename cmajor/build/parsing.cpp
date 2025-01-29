// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.parsing;

import cmajor.lexers;
import cmajor.parsers;
import util;

namespace cmajor::build {

void PrintSourceFile(const std::string& outFilePath, cmajor::ast::Node* compileUnitNode)
{
    std::ofstream outFile(outFilePath);
    util::CodeFormatter formatter(outFile);
    cmajor::ast::SourceWriter writer(formatter);
    compileUnitNode->Accept(writer);
}

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(int fileIndex, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module)
{
    std::string sourceFilePath = fileMap.GetFilePath(fileIndex);
    std::string content = util::ReadFile(sourceFilePath, true);
    std::u32string ucontent;
    try
    {
        ucontent = util::ToUtf32(content);
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + sourceFilePath);
    }
    auto lexer = cmajor::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), sourceFilePath);
    lexer.SetFile(fileIndex);
    using LexerType = decltype(lexer);
    cmajor::parser::context::Context context;
    if (module)
    {
        context.SetModuleId(module->Id());
    }
    std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::compile::unit::parser::CompileUnitParser<LexerType>::Parse(lexer, &context);
    if (module)
    {
        compileUnit->SetModuleId(module->Id());
    }
    compileUnit->SetFileIndex(fileIndex);
    fileMap.AddFileContent(fileIndex, std::move(ucontent), lexer.GetLineStartIndeces());
    if ((flags & Flags::ast) != Flags::none)
    {
        std::string astFilePath = util::Path::ChangeExtension(sourceFilePath, ".ast");
        {
            cmajor::ast::AstWriter astWriter(astFilePath);
            astWriter.Write(compileUnit.get());
        }
        {
            cmajor::ast::AstReader astReader(astFilePath);
            std::unique_ptr<cmajor::ast::Node> compileUnitNode(astReader.ReadNode());
            if ((flags & Flags::print) != Flags::none)
            {
                std::string outFilePath = util::Path::ChangeExtension(sourceFilePath, ".out.cm");
                PrintSourceFile(outFilePath, compileUnitNode.get());
            }
        }
    }
    return compileUnit;
}

std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel)
{
    std::string content = util::ReadFile(projectFilePath, true);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), projectFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Project> project = cmajor::projects::parser::ProjectParser<LexerType>::Parse(lexer, config, backend, optLevel);
    project->ResolveDeclarations();
    return project;
}

std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath)
{
    std::string content = util::ReadFile(solutionFilePath, true);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), solutionFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Solution> solution = cmajor::solutions::parser::SolutionParser<LexerType>::Parse(lexer);
    solution->ResolveDeclarations();
    return solution;
}

void ParseSingleThreaded(cmajor::ast::Project* project, const std::vector<int>& fileIndeces, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module)
{
    for (int fileIndex : fileIndeces)
    {
        std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = ParseSourceFile(fileIndex, fileMap, flags, module);
        project->AddCompileUnit(compileUnit.release());
    }
}

struct ParsingThreadData
{
    ParsingThreadData(Flags flags_, soul::lexer::FileMap& fileMap_, int size_, cmajor::symbols::Module* module_) : 
        flags(flags_), fileMap(fileMap_), stop(false), module(module_)
    { 
        compileUnits.resize(size_); exceptions.resize(size_); 
    }
    soul::lexer::FileMap& fileMap;
    std::list<int> fileIndexQueue;
    std::mutex mtx;
    bool stop;
    Flags flags;
    std::vector<std::unique_ptr<cmajor::ast::CompileUnitNode>> compileUnits;
    std::vector<std::exception_ptr> exceptions;
    cmajor::symbols::Module* module;
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
            std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = ParseSourceFile(fileIndex, threadData->fileMap, threadData->flags, threadData->module);
            threadData->compileUnits[fileIndex] = std::move(compileUnit);
        }
        catch (...)
        {
            threadData->exceptions[fileIndex] = std::current_exception();
            threadData->stop = true;
        }
    }
}

void ParseMultiThreaded(cmajor::ast::Project* project, const std::vector<int>& fileIndeces, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module)
{
    ParsingThreadData threadData(flags, fileMap, fileIndeces.size(), module);
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

void ParseSourceFiles(cmajor::ast::Project* project, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module)
{
    std::vector<int> fileIndeces;
    for (const auto& sourceFilePath : project->SourceFilePaths())
    {
        fileIndeces.push_back(fileMap.MapFile(sourceFilePath));
    }
    if ((flags & Flags::singleThreadedParse) != Flags::none)
    {
        ParseSingleThreaded(project, fileIndeces, fileMap, flags, module);
    }
    else
    {
        ParseMultiThreaded(project, fileIndeces, fileMap, flags, module);
    }
}

std::unique_ptr<cmajor::ast::Project> ParseProject(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel,
    soul::lexer::FileMap& fileMap, Flags flags)
{
    if ((flags & Flags::verbose) != Flags::none)
    {
        std::cout << ">> " << projectFilePath << "\n";
    }
    std::unique_ptr<cmajor::ast::Project> project = ParseProjectFile(projectFilePath, config, backend, optLevel);
    return project;
}

std::unique_ptr<cmajor::ast::Solution> ParseSolution(const std::string& solutionFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel, Flags flags)
{
    if ((flags & Flags::verbose) != Flags::none)
    {
        std::cout << "> " << solutionFilePath << "\n";
    }
    std::unique_ptr<cmajor::ast::Solution> solution = ParseSolutionFile(solutionFilePath);
    for (const auto& projectFilePath : solution->ProjectFilePaths())
    {
        soul::lexer::FileMap fileMap;
        std::unique_ptr<cmajor::ast::Project> project = ParseProject(projectFilePath, config, backend, optLevel, fileMap, flags);
        solution->AddProject(std::move(project));
    }
    return solution;
}

} // namespace cmajor::build
