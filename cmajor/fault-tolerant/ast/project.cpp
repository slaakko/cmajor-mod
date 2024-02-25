// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.project;

import std.filesystem;
import util;

namespace cmajor::fault::tolerant::ast {

ProjectDeclaration::ProjectDeclaration(ProjectDeclarationKind kind_) : kind(kind_)
{
}

ProjectDeclaration::~ProjectDeclaration()
{
}

ReferenceDeclaration::ReferenceDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationKind::referenceDeclaration), filePath(filePath_)
{
}

SourceFileDeclaration::SourceFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationKind::sourceFileDeclaration), filePath(filePath_)
{
}

ResourceFileDeclaration::ResourceFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationKind::resourceFileDeclaration), filePath(filePath_)
{
}

TextFileDeclaration::TextFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationKind::textFileDeclaration), filePath(filePath_)
{
}

ActionFileDeclaration::ActionFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationKind::actionFileDeclaration), filePath(filePath_)
{
}

TargetDeclaration::TargetDeclaration(Target target_) : ProjectDeclaration(ProjectDeclarationKind::targetDeclaration), target(target_)
{
}

Project::Project(const std::string& name_, const std::string& filePath_, const std::string& config_, Backend backend_) : 
    name(name_), filePath(filePath_), config(config_), backend(backend_), target(Target::program), valid(true), index(-1)
{
    libDir = util::GetFullPath(util::Path::Combine(util::Path::GetDirectoryName(filePath), "lib"));
    if (backend == Backend::cpp)
    {
        libDir = util::Path::Combine(libDir, "cpp");
    }
    libDir = util::Path::Combine(libDir, config);
    std::filesystem::create_directories(libDir);
    symbolFilePath = util::Path::Combine(libDir, name + ".sym");
}

void Project::AddDeclaration(ProjectDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<ProjectDeclaration>(declaration));
}

void Project::ResolveDeclarations()
{
    std::string basePath = util::Path::GetDirectoryName(filePath);
    for (const auto& declaration : declarations)
    {
        switch (declaration->Kind())
        {
            case ProjectDeclarationKind::referenceDeclaration:  
            {
                ReferenceDeclaration* referenceDeclaration = static_cast<ReferenceDeclaration*>(declaration.get());
                relativeReferenceFilePaths.push_back(referenceDeclaration->FilePath());
                referenceFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, referenceDeclaration->FilePath())));
                break;
            }
            case ProjectDeclarationKind::sourceFileDeclaration:
            {
                SourceFileDeclaration* sourceFileDeclaration = static_cast<SourceFileDeclaration*>(declaration.get());
                relativeSourceFilePaths.push_back(sourceFileDeclaration->FilePath());
                sourceFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, sourceFileDeclaration->FilePath())));
                break;
            }
            case ProjectDeclarationKind::resourceFileDeclaration:
            {
                ResourceFileDeclaration* resourceFileDeclaration = static_cast<ResourceFileDeclaration*>(declaration.get());
                relativeResourceFilePaths.push_back(resourceFileDeclaration->FilePath());
                resourceFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, resourceFileDeclaration->FilePath())));
                break;
            }
            case ProjectDeclarationKind::textFileDeclaration:
            {
                TextFileDeclaration* textFileDeclaration = static_cast<TextFileDeclaration*>(declaration.get());
                relativeTextFilePaths.push_back(textFileDeclaration->FilePath());
                textFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, textFileDeclaration->FilePath())));
                break;
            }
            case ProjectDeclarationKind::actionFileDeclaration:
            {
                ActionFileDeclaration* actionFileDeclaration = static_cast<ActionFileDeclaration*>(declaration.get());
                relativeActionFilePaths.push_back(actionFileDeclaration->FilePath());
                actionFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, actionFileDeclaration->FilePath())));
                break;
            }
            case ProjectDeclarationKind::targetDeclaration:
            {
                TargetDeclaration* targetDeclaration = static_cast<TargetDeclaration*>(declaration.get());
                target = targetDeclaration->GetTarget();
                break;
            }
        }
    }
}

void Project::AddDependsOnProject(Project* dependsOnProject)
{
    if (std::find(dependsOn.begin(), dependsOn.end(), dependsOnProject) == dependsOn.end())
    {
        dependsOn.push_back(dependsOnProject);
    }
}

void Project::AddCompileUnit(CompileUnitNode* compileUnit)
{
    compileUnit->SetProject(this);
    compileUnit->SetCompileUnitIndex(compileUnits.size());
    compileUnits.push_back(std::unique_ptr<CompileUnitNode>(compileUnit));
    compileUnitMap[compileUnit->FilePath()] = compileUnit;
}

CompileUnitNode* Project::GetCompileUnit(int index) const
{
    if (index >= 0 && index < compileUnits.size())
    {
        return compileUnits[index].get();
    }
    else
    {
        throw std::runtime_error("Project '" + name + "': invalid compile unit index");
    }
}

CompileUnitNode* Project::GetCompileUnit(const std::string& sourceFilePath) const
{
    auto it = compileUnitMap.find(sourceFilePath);
    if (it != compileUnitMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Project::ResetCompileUnit(int index, CompileUnitNode* compileUnit)
{
    if (index >= 0 && index < compileUnits.size())
    {
        compileUnit->SetCompileUnitIndex(index);
        compileUnits[index].reset(compileUnit);
        compileUnitMap[compileUnit->FilePath()] = compileUnit;
    }
    else
    {
        throw std::runtime_error("Project '" + name + "': invalid compile unit index");
    }
}

} // namespace cmajor::fault::tolerant::ast
