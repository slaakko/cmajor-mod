// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.resources;

import cmajor.resource.processor;
import cmajor.symbols;
import cmajor.systemx.object;
import util;

namespace cmajor::build {

void AddResourcesSystemX(cmajor::ast::Project* project, std::vector<std::string>& objectFilePaths)
{
    for (const auto& resourceFilePath : project->ResourceFilePaths())
    {
        std::string objectFilePath = cmajor::systemx::object::GenerateResourceUnit(resourceFilePath, project->LibraryDir());
        objectFilePaths.push_back(objectFilePath);
    }
}

void AddResources(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        AddResourcesSystemX(project, objectFilePaths);
    }
    else
    {
        cmajor::resources::ProcessResourcesInProject(project, rootModule);
    }
}

void MakeResourceFile(const std::string& resourceFilePath, const std::string& classIndexFilePath, const std::string& traceDataFilePath)
{
    std::ofstream resourceFile(resourceFilePath);
    util::CodeFormatter formatter(resourceFile);
    formatter.WriteLine("CLASS_INDEX RCDATA \"" + classIndexFilePath + "\"");
    formatter.WriteLine("TRACE_DATA RCDATA \"" + traceDataFilePath + "\"");
}

std::string CompileResourceFile(const std::string& resourceFilePath, cmajor::ast::Project* project)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        std::string command = "llvm-rc";
        std::string resFilePath = util::QuotedPath(util::Path::ChangeExtension(resourceFilePath, ".res"));
        command.append(" ").append(util::QuotedPath(resourceFilePath)).append(" /fo " + resFilePath);
        util::ExecuteResult result = util::Execute(command);
        if (result.exitCode == 0)
        {
            return resFilePath;
        }
        else
        {
            throw std::runtime_error("error " + std::to_string(result.exitCode) + " creating runtime info resource file from '" + resourceFilePath + "': " + result.output);
        }
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        std::string command = "windres";
        std::string objectFilePath = util::QuotedPath(util::Path::ChangeExtension(resourceFilePath, ".o"));
        command.append(" ").append(util::QuotedPath(resourceFilePath)).append(" -o " + objectFilePath);
        util::ExecuteResult result = util::Execute(command);
        if (result.exitCode == 0)
        {
            return objectFilePath;
        }
        else
        {
            throw std::runtime_error("error " + std::to_string(result.exitCode) + " creating runtime info resource file from '" + resourceFilePath + "': " + result.output);
        }
    }
    else
    {
        throw std::runtime_error("only for C++ and LLVM backends");
    }
}

void GenerateRuntimeResourceFile(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, 
    const std::string& classIndexFilePath, const std::string& traceDataFilePath)
{
    std::string resourceScriptFilePath = util::Path::Combine(util::Path::GetDirectoryName(project->ModuleFilePath()), "runtime_info.rc");
    MakeResourceFile(resourceScriptFilePath, classIndexFilePath, traceDataFilePath);
    std::string compiledResourceFilePath = CompileResourceFile(resourceScriptFilePath, project);
    rootModule->AddResourceFilePath(compiledResourceFilePath);
}

void CompileResourceScriptFiles(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    for (const auto& rcFilePath : rootModule->AllResourceScriptFilePaths())
    {
        std::string compiledResourceFilePath = CompileResourceFile(rcFilePath, project);
        rootModule->AddResourceFilePath(compiledResourceFilePath);
    }
}

} // namespace cmajor::build
