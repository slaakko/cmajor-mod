// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.project;

import cmajor.ast.solution;
import cmajor.ast.compile.unit;
import std.core;
import std.filesystem;
import util;

namespace cmajor::ast {

std::string GetPlatform()
{
#ifdef _WIN32
        return "windows";
#else
        return "linux";
#endif
}

ModuleVersionTagVerifier* moduleVersionTagVerifier = nullptr;

void SetModuleVersionTagVerifier(ModuleVersionTagVerifier* verifier)
{
    moduleVersionTagVerifier = verifier;
}

std::string TargetStr(Target target)
{
    switch (target)
    {
    case Target::program: return "program";
    case Target::library: return "library";
    case Target::winapp: return "winapp";
    case Target::winlib: return "winlib";
    case Target::winguiapp: return "winguiapp";
    case Target::unitTest: return "unitTest";
    }
    return "library";
}

std::string BackEndStr(BackEnd backend)
{
    switch (backend)
    {
        case BackEnd::llvm: return "llvm";
        case BackEnd::systemx: return "systemx";
        case BackEnd::cpp: return "cpp";
        case BackEnd::masm: return "masm";
        case BackEnd::cm: return "cm";
    }
    return std::string();
}

Target ParseTarget(const std::string& targetStr)
{
    if (targetStr == "program")
    {
        return Target::program;
    }
    else if (targetStr == "library")
    {
        return Target::library;
    }
    else if (targetStr == "winapp")
    {
        return Target::winapp;
    }
    else if (targetStr == "winlib")
    {
        return Target::winlib;
    }
    else if (targetStr == "winguiapp")
    {
        return Target::winguiapp;
    }
    else if (targetStr == "unitTest")
    {
        return Target::unitTest;
    }
    return Target::program;
}

std::string CmajorRootDir()
{
    char* e = getenv("CMAJOR_ROOT");
    if (e == nullptr || !*e)
    {
        throw std::runtime_error("please set 'CMAJOR_ROOT' environment variable to contain /path/to/cmajor directory.");
    }
    return std::string(e);
}

std::string CmajorSystemLibDir(const std::string& config, BackEnd backend, int optLevel)
{
    if (backend == BackEnd::llvm)
    {
        std::filesystem::path sld(CmajorRootDir());
        sld /= "system";
        sld /= "lib";
        sld /= "llvm";
        sld /= config;
        if (config == "release")
        {
            sld /= std::to_string(optLevel);
        }
        return util::GetFullPath(sld.generic_string());
    }
    else if (backend == BackEnd::systemx)
    {
        std::filesystem::path sld(CmajorRootDir());
        sld /= "system-x";
        sld /= "system";
        sld /= "lib";
        sld /= config;
        if (config == "release")
        {
            sld /= std::to_string(optLevel);
        }
        return util::GetFullPath(sld.generic_string());
    }
    else if (backend == BackEnd::cpp)
    {
        std::filesystem::path sld(CmajorRootDir());
        sld /= "system";
        sld /= "lib";
        sld /= "cpp";
        sld /= config;
        if (config == "release")
        {
            sld /= std::to_string(optLevel);
        }
        return util::GetFullPath(sld.generic_string());
    }
    else if (backend == BackEnd::masm)
    {
        std::filesystem::path sld(CmajorRootDir());
        sld /= "system";
        sld /= "lib";
        sld /= "masm";
        sld /= config;
        if (config == "release")
        {
            sld /= std::to_string(optLevel);
        }
        return util::GetFullPath(sld.generic_string());
    }
    else if (backend == BackEnd::cm)
    {
        std::filesystem::path sld(CmajorRootDir());
        sld /= "system";
        sld /= "lib";
        sld /= "cm";
        sld /= config;
        if (config == "release")
        {
            sld /= std::to_string(optLevel);
        }
        return util::GetFullPath(sld.generic_string());
    }

    else
    {
        return std::string();
    }
}

std::string CmajorResourceDir()
{
    std::filesystem::path rd(CmajorRootDir());
    rd /= "res";
    return util::GetFullPath(rd.generic_string());
}

std::string CmajorLogFileDir()
{
    std::string cmajorRooDir = CmajorRootDir();
    std::string logFileDir = util::Path::Combine(cmajorRooDir, "log");
    std::filesystem::create_directories(logFileDir);
    return logFileDir;
}

std::string outDir;

void SetOutDir(const std::string& outDir_)
{
    outDir = outDir_;
}

const std::string& OutDir()
{
    return outDir;
}

std::string CmajorSystemModuleFilePath(const std::string& config, BackEnd backend, int optLevel)
{
    std::filesystem::path smfp(CmajorSystemLibDir(config, backend, optLevel));
    smfp /= "System.cmm";
    return util::GetFullPath(smfp.generic_string());
}

std::string CmajorSystemWindowsModuleFilePath(const std::string& config, BackEnd backend, int optLevel)
{
    std::filesystem::path smfp(CmajorSystemLibDir(config, backend, optLevel));
    smfp /= "System.Windows.cmm";
    return util::GetFullPath(smfp.generic_string());
}

std::string MakeCmajorRootRelativeFilePath(const std::string& filePath)
{
    std::string cmajorRootDir = util::GetFullPath(CmajorRootDir());
    std::string fullPath = util::GetFullPath(filePath);
    if (fullPath.starts_with(cmajorRootDir))
    {
        return "$CMAJOR_ROOT$" + fullPath.substr(cmajorRootDir.length());
    }
    else
    {
        return fullPath;
    }
}

std::string ExpandCmajorRootRelativeFilePath(const std::string& filePath)
{
    if (filePath.starts_with("$CMAJOR_ROOT$"))
    {
        std::string cmajorRootDir = util::GetFullPath(CmajorRootDir());
        return cmajorRootDir + filePath.substr(std::string("$CMAJOR_ROOT$").length());
    }
    else
    {
        return util::GetFullPath(filePath);
    }
}

std::vector<Project*> GetReferencedProjects(Project* project, Solution* solution)
{
    std::vector<Project*> referencedProjects;
    for (const std::string& referencedProjectFilePath : project->ReferencedProjectFilePaths())
    {
        std::string rpfp = util::GetFullPath(referencedProjectFilePath);
        int n = solution->Projects().size();
        bool found = false;
        int i = 0;
        while (i < n && !found)
        {
            Project* solutionProject = solution->Projects()[i].get();
            std::string fp = util::GetFullPath(solutionProject->FilePath());
            if (fp == rpfp)
            {
                referencedProjects.push_back(solutionProject);
                found = true;
            }
            ++i;
        }
    }
    return referencedProjects;
}

std::set<Project*> GetAllReferencedProjects(Project* project, Solution* solution)
{
    std::set<Project*> allReferencedProjects;
    AddReferencedProjects(allReferencedProjects, project, solution);
    return allReferencedProjects;
}

void AddReferencedProjects(std::set<Project*>& allReferencedProjects, Project* project, Solution* solution)
{
    std::vector<Project*> referencedProjects = GetReferencedProjects(project, solution);
    for (Project* referencedProject : referencedProjects)
    {
        if (allReferencedProjects.find(referencedProject) == allReferencedProjects.cend())
        {
            allReferencedProjects.insert(referencedProject);
            AddReferencedProjects(allReferencedProjects, referencedProject, solution);
        }
    }
}

ProjectDeclaration::ProjectDeclaration(ProjectDeclarationType declarationType_) : declarationType(declarationType_)
{
}

ProjectDeclaration::~ProjectDeclaration()
{
}

ReferenceDeclaration::ReferenceDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationType::referenceDeclaration), filePath(filePath_)
{
}

void ReferenceDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("reference <" + filePath + ">;");
}

SourceFileDeclaration::SourceFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationType::sourceFileDeclaration), filePath(filePath_)
{
}

void SourceFileDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("source <" + filePath + ">;");
}

ResourceFileDeclaration::ResourceFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationType::resourceFileDeclaration), filePath(filePath_)
{
}

void ResourceFileDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("resource <" + filePath + ">;");
}

TextFileDeclaration::TextFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationType::textFileDeclaration), filePath(filePath_)
{
}

void TextFileDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("text <" + filePath + ">;");
}

ActionFileDeclaration::ActionFileDeclaration(const std::string& filePath_) : ProjectDeclaration(ProjectDeclarationType::actionFileDeclaration), filePath(filePath_)
{
}

void ActionFileDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("action <" + filePath + ">;");
}

ResourceScriptFileDeclaration::ResourceScriptFileDeclaration(const std::string& filePath_) : 
    ProjectDeclaration(ProjectDeclarationType::resourceScriptFileDeclaration), filePath(filePath_)
{
}

void ResourceScriptFileDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("rc <" + filePath + ">;");
}

TargetDeclaration::TargetDeclaration(Target target_) : ProjectDeclaration(ProjectDeclarationType::targetDeclaration), target(target_)
{
}

void TargetDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("target=" + TargetStr(target) + ";");
}

Project::Project(const std::u32string& name_, const std::string& filePath_, const std::string& config_, BackEnd backend_, int optLevel_) :
    backend(backend_), name(name_), filePath(filePath_), config(config_), optLevel(optLevel_), target(Target::program), sourceBasePath(filePath), outdirBasePath(filePath),
    isSystemProject(false), logStreamId(0), built(false)
{
    std::string platform = GetPlatform();
    if (!outDir.empty())
    {
        sourceBasePath.remove_filename();
        outdirBasePath = outDir;
        outdirBasePath /= util::ToUtf8(name);
    }
    else
    {
        sourceBasePath.remove_filename();
        outdirBasePath = sourceBasePath;
    }
    systemLibDir = CmajorSystemLibDir(config, backend, optLevel);
    std::filesystem::path mfp(filePath);
    std::filesystem::path fn = mfp.filename();
    mfp.remove_filename();
    if (!outDir.empty())
    {
        mfp = outdirBasePath;
    }
    mfp /= "lib";
    if (backend == BackEnd::llvm)
    {
        mfp /= "llvm";
    }
    else if (backend == BackEnd::cpp)
    {
        mfp /= "cpp";
    }
    else if (backend == BackEnd::masm)
    {
        mfp /= "masm";
    }
    else if (backend == BackEnd::cm)
    {
        mfp /= "cm";
    }
    mfp /= config;
    if (config == "release")
    {
        mfp /= std::to_string(optLevel);
    }
    mfp /= fn;
    mfp.replace_extension(".cmm");
    moduleFilePath = util::GetFullPath(mfp.generic_string());
    std::filesystem::path lfp(mfp);
#ifdef _WIN32
    if (backend == BackEnd::systemx)
    {
        lfp.replace_extension(".a");
    }
    else if (backend == BackEnd::llvm)
    {
        lfp.replace_extension(".lib");
    }
    else if (backend == BackEnd::cpp)
    {
        lfp.replace_extension(".a");
    }
    else if (backend == BackEnd::masm)
    {
        lfp.replace_extension(".lib");
    }
    else if (backend == BackEnd::cm)
    {
        lfp.replace_extension(".lib");
    }
    else
    {
        lfp.replace_extension(".a");
    }
#endif
    libraryFilePath = util::GetFullPath(lfp.generic_string());
    std::filesystem::path efp(filePath);
    efp.remove_filename();
    if (!outDir.empty())
    {
        efp = outdirBasePath;
    }
    efp /= "bin";
    if (backend == BackEnd::llvm)
    {
        efp /= "llvm";
    }
    else if (backend == BackEnd::cpp)
    {
        efp /= "cpp";
    }
    else if (backend == BackEnd::masm)
    {
        efp /= "masm";
    }
    else if (backend == BackEnd::cm)
    {
        efp /= "cm";
    }

    efp /= config;
    if (config == "release")
    {
        efp /= std::to_string(optLevel);
    }
    efp /= fn;
#ifdef _WIN32
    if (backend == BackEnd::systemx)
    {
        efp.replace_extension(".x");
    }
    else if (backend == BackEnd::llvm)
    {
        efp.replace_extension(".exe");
    }
    else if (backend == BackEnd::cpp)
    {
        efp.replace_extension(".exe");
    }
    else if (backend == BackEnd::masm)
    {
        efp.replace_extension(".exe");
    }
    else if (backend == BackEnd::cm)
    {
        efp.replace_extension(".exe");
    }

#else
    if (backend == BackEnd::cpp)
    {
        efp.replace_extension();
    }
    else
    {
        efp.replace_extension();
    }
#endif
    executableFilePath = util::GetFullPath(efp.generic_string());
}

void Project::AddDeclaration(ProjectDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<ProjectDeclaration>(declaration));
}

void Project::ResolveDeclarations()
{
    for (const std::unique_ptr<ProjectDeclaration>& declaration : declarations)
    {
        switch (declaration->GetDeclarationType())
        {
        case ProjectDeclarationType::referenceDeclaration:
        {
            ReferenceDeclaration* reference = static_cast<ReferenceDeclaration*>(declaration.get());
            std::filesystem::path rp(reference->FilePath());
            relativeReferencedProjectFilePaths.push_back(rp.generic_string());
            if (rp.is_absolute())
            {
                referencedProjectFilePaths.push_back(util::GetFullPath(rp.generic_string()));
            }
            else
            {
                std::filesystem::path ar = sourceBasePath / rp;
                referencedProjectFilePaths.push_back(util::GetFullPath(ar.generic_string()));
            }
            std::filesystem::path fn = rp.filename();
            rp.remove_filename();
            if (rp.is_relative())
            {
                rp = systemLibDir / rp;
            }
            rp /= fn;
            if (rp.extension() == ".cmp" || rp.extension() == ".cmproj")
            {
                rp.replace_extension(".cmm");
            }
            if (rp.extension() != ".cmm")
            {
                throw std::runtime_error("invalid reference path extension '" + rp.generic_string() + "' (not .cmp, .cmproj or .cmm)");
            }
            if (!std::filesystem::exists(rp))
            {
                rp = reference->FilePath();
                rp.remove_filename();
                if (rp.is_relative())
                {
                    rp = outdirBasePath / rp;
                }
                rp /= "lib";
                if (backend == BackEnd::llvm)
                {
                    rp /= "llvm";
                }
                else if (backend == BackEnd::cpp)
                {
                    rp /= "cpp";
                }
                else if (backend == BackEnd::masm)
                {
                    rp /= "masm";
                }
                else if (backend == BackEnd::cm)
                {
                    rp /= "cm";
                }
                rp /= config;
                if (config == "release")
                {
                    rp /= std::to_string(optLevel);
                }
                rp /= fn;
                if (rp.extension() == ".cmp" || rp.extension() == ".cmproj")
                {
                    rp.replace_extension(".cmm");
                }
                if (rp.extension() != ".cmm")
                {
                    throw std::runtime_error("invalid reference path extension '" + rp.generic_string() + "' (not .cmp, .cmproj or .cmm)");
                }
            }
            std::string referencePath = util::GetFullPath(rp.generic_string());
            if (std::find(references.cbegin(), references.cend(), referencePath) == references.cend())
            {
                references.push_back(referencePath);
            }
            break;
        }
        case ProjectDeclarationType::sourceFileDeclaration:
        {
            SourceFileDeclaration* sourceFileDeclaration = static_cast<SourceFileDeclaration*>(declaration.get());
            std::filesystem::path sfp(sourceFileDeclaration->FilePath());
            relativeSourceFilePaths.push_back(sfp.generic_string());
            if (sfp.is_relative())
            {
                sfp = sourceBasePath / sfp;
            }
            if (sfp.extension() != ".cm")
            {
                throw std::runtime_error("invalid source file extension '" + sfp.generic_string() + "' (not .cm)");
            }
            if (!std::filesystem::exists(sfp))
            {
                throw std::runtime_error("source file path '" + util::GetFullPath(sfp.generic_string()) + "' not found");
            }
            std::string sourceFilePath = util::GetFullPath(sfp.generic_string());
            if (std::find(sourceFilePaths.cbegin(), sourceFilePaths.cend(), sourceFilePath) == sourceFilePaths.cend() && sourceFilePath != excludeSourceFilePath)
            {
                sourceFilePaths.push_back(sourceFilePath);
            }
            break;
        }
        case ProjectDeclarationType::resourceFileDeclaration:
        {
            ResourceFileDeclaration* resourceFileDeclaration = static_cast<ResourceFileDeclaration*>(declaration.get());
            std::filesystem::path rfp(resourceFileDeclaration->FilePath());
            relativeResourceFilePaths.push_back(rfp.generic_string());
            if (rfp.is_relative())
            {
                rfp = sourceBasePath / rfp;
            }
            if (rfp.extension() != ".xml")
            {
                throw std::runtime_error("invalid resource file extension '" + rfp.generic_string() + "' (not .xml)");
            }
            if (!std::filesystem::exists(rfp))
            {
                throw std::runtime_error("resource file path '" + util::GetFullPath(rfp.generic_string()) + "' not found");
            }
            std::string resourceFilePath = util::GetFullPath(rfp.generic_string());
            if (std::find(resourceFilePaths.cbegin(), resourceFilePaths.cend(), resourceFilePath) == resourceFilePaths.cend())
            {
                resourceFilePaths.push_back(resourceFilePath);
            }
            break;
        }
        case ProjectDeclarationType::resourceScriptFileDeclaration:
        {
            ResourceScriptFileDeclaration* resourceScriptFileDeclaration = static_cast<ResourceScriptFileDeclaration*>(declaration.get());
            std::filesystem::path rsfp(resourceScriptFileDeclaration->FilePath());
            relativeResourceScriptFilePaths.push_back(rsfp.generic_string());
            if (rsfp.is_relative())
            {
                rsfp = sourceBasePath / rsfp;
            }
            if (rsfp.extension() != ".rc")
            {
                throw std::runtime_error("invalid resource script file extension '" + rsfp.generic_string() + "' (not .rc)");
            }
            if (!std::filesystem::exists(rsfp))
            {
                throw std::runtime_error("resource script file path '" + util::GetFullPath(rsfp.generic_string()) + "' not found");
            }
            std::string resourceScriptFilePath = util::GetFullPath(rsfp.generic_string());
            if (std::find(resourceScriptFilePaths.cbegin(), resourceScriptFilePaths.cend(), resourceScriptFilePath) == resourceScriptFilePaths.cend())
            {
                resourceScriptFilePaths.push_back(resourceScriptFilePath);
            }
            break;
        }
        case ProjectDeclarationType::targetDeclaration:
        {
            TargetDeclaration* targetDeclaration = static_cast<TargetDeclaration*>(declaration.get());
            target = targetDeclaration->GetTarget();
            break;
        }
        case ProjectDeclarationType::textFileDeclaration:
        {
            TextFileDeclaration* textFileDeclaration = static_cast<TextFileDeclaration*>(declaration.get());
            std::filesystem::path tfp(textFileDeclaration->FilePath());
            relativeTextFilePaths.push_back(tfp.generic_string());
            if (tfp.is_relative())
            {
                tfp = sourceBasePath / tfp;
            }
            std::string textFilePath = util::GetFullPath(tfp.generic_string());
            textFilePaths.push_back(textFilePath);
            break;
        }
        case ProjectDeclarationType::actionFileDeclaration:
        {
            ActionFileDeclaration* actionDeclaration = static_cast<ActionFileDeclaration*>(declaration.get());
            std::filesystem::path afp(actionDeclaration->FilePath());
            relativeActionFilePaths.push_back(afp.generic_string());
            if (afp.is_relative())
            {
                afp = sourceBasePath / afp;
            }
            std::string actionFilePath = util::GetFullPath(afp.generic_string());
            actionFilePaths.push_back(actionFilePath);
            break;
        }
        default:
        {
            throw std::runtime_error("unknown project declaration");
        }
        }
    }
}

void Project::Write(const std::string& projectFilePath)
{
    std::ofstream projectFile(projectFilePath);
    util::CodeFormatter formatter(projectFile);
    formatter.WriteLine("project " + util::ToUtf8(Name()) + ";");
    for (const std::unique_ptr<ProjectDeclaration>& declaration : declarations)
    {
        declaration->Write(formatter);
    }
}

std::string Project::OutDir() const
{
    if (executableFilePath.empty())
    {
        return std::string();
    }
    return util::GetFullPath(util::Path::GetDirectoryName(executableFilePath));
}

std::string Project::ProjectDir() const
{
    return util::GetFullPath(util::Path::GetDirectoryName(filePath));
}

std::string Project::LibraryDir() const
{
    return util::GetFullPath(util::Path::GetDirectoryName(libraryFilePath));
}

bool Project::DependsOn(Project* that) const
{
    return std::find(references.cbegin(), references.cend(), that->moduleFilePath) != references.cend();
}

void Project::AddDependsOnProjects(Project* dependsOnProject)
{
    dependsOn.push_back(dependsOnProject);
}

void Project::SetModuleFilePath(const std::string& moduleFilePath_)
{
    moduleFilePath = moduleFilePath_;
}

void Project::SetLibraryFilePath(const std::string& libraryFilePath_)
{
    libraryFilePath = libraryFilePath_;
}

void Project::SetReferencedProjects(const std::vector<Project*>& referencedProjects)
{
    referencedProjectFilePaths.clear();
    relativeReferencedProjectFilePaths.clear();
    std::string absoluteProjectDirPath = util::GetFullPath(sourceBasePath.generic_string());
    for (Project* referencedProject : referencedProjects)
    {
        referencedProjectFilePaths.push_back(referencedProject->FilePath());
        std::string absoluteReferenceDirPath = util::GetFullPath(util::Path::GetDirectoryName(referencedProject->FilePath()));
        std::string referenceProjectDir = util::MakeRelativeDirPath(absoluteReferenceDirPath, absoluteProjectDirPath);
        relativeReferencedProjectFilePaths.push_back(util::Path::Combine(referenceProjectDir, util::Path::GetFileName(referencedProject->FilePath())));
    }
}

bool Project::IsUpToDate(const std::string& systemModuleFilePath) const
{
    if (!std::filesystem::exists(moduleFilePath))
    {
        return false;
    }
    for (const std::string& sourceFilePath : sourceFilePaths)
    {
        if (std::filesystem::last_write_time(sourceFilePath) > std::filesystem::last_write_time(moduleFilePath))
        {
            return false;
        }
    }
    for (const std::string& resourceFilePath : resourceFilePaths)
    {
        if (std::filesystem::last_write_time(resourceFilePath) > std::filesystem::last_write_time(moduleFilePath))
        {
            return false;
        }
    }
    for (const std::string& referenceFilePath : references)
    {
        if (std::filesystem::last_write_time(referenceFilePath) > std::filesystem::last_write_time(moduleFilePath))
        {
            return false;
        }
    }
    if (!systemModuleFilePath.empty() && !IsSystemProject() && std::filesystem::exists(systemModuleFilePath))
    {
        if (std::filesystem::last_write_time(systemModuleFilePath) > std::filesystem::last_write_time(moduleFilePath))
        {
            return false;
        }
    }
    return true;
}

bool Project::Built()
{
    return built;
}

void Project::SetBuilt()
{
    built = true;
}

bool Project::Ready()
{
    for (Project* dependOn : dependsOn)
    {
        if (!dependOn->Built())
        {
            return false;
        }
    }
    return true;
}

void Project::SetExcludeSourceFilePath(const std::string& excludeSourceFilePath_)
{
    excludeSourceFilePath = excludeSourceFilePath_;
}

std::string Project::Id() const
{
    std::string id = "project_";
    id.append(util::ToUtf8(name)).append(1, '_').append(util::GetSha1MessageDigest(filePath));
    return id;
}

void Project::AddDependsOnId(const std::string& dependsOnId)
{
    dependsOnIds.push_back(dependsOnId);
}

bool Project::HasSourceFile(const std::string& sourceFilePath) const
{
    for (const auto& filePath : sourceFilePaths)
    {
        if (filePath == sourceFilePath)
        {
            return true;
        }
    }
    return false;
}

bool Project::HasResourceFile(const std::string& resourceFilePath) const
{
    for (const auto& filePath : resourceFilePaths)
    {
        if (filePath == resourceFilePath)
        {
            return true;
        }
    }
    return false;
}

bool Project::HasTextFile(const std::string& textFilePath) const
{
    for (const auto& filePath : textFilePaths)
    {
        if (filePath == textFilePath)
        {
            return true;
        }
    }
    return false;
}

void Project::AddSourceFileName(const std::string& sourceFileName, const std::string& sourceFilePath)
{
    relativeSourceFilePaths.push_back(sourceFileName);
    std::sort(relativeSourceFilePaths.begin(), relativeSourceFilePaths.end());
    sourceFilePaths.push_back(sourceFilePath);
    std::sort(sourceFilePaths.begin(), sourceFilePaths.end());
}

void Project::AddResourceFileName(const std::string& resourceFileName, const std::string& resourceFilePath)
{
    relativeResourceFilePaths.push_back(resourceFileName);
    std::sort(relativeResourceFilePaths.begin(), relativeResourceFilePaths.end());
    resourceFilePaths.push_back(resourceFilePath);
    std::sort(resourceFilePaths.begin(), resourceFilePaths.end());
}

void Project::AddTextFileName(const std::string& textFileName, const std::string& textFilePath)
{
    relativeTextFilePaths.push_back(textFileName);
    std::sort(relativeTextFilePaths.begin(), relativeTextFilePaths.end());
    textFilePaths.push_back(textFilePath);
    std::sort(textFilePaths.begin(), textFilePaths.end());
}

void Project::RemoveFile(const std::string& filePath, const std::string& fileName)
{
    relativeSourceFilePaths.erase(std::remove(relativeSourceFilePaths.begin(), relativeSourceFilePaths.end(), fileName), relativeSourceFilePaths.end());
    sourceFilePaths.erase(std::remove(sourceFilePaths.begin(), sourceFilePaths.end(), filePath), sourceFilePaths.end());
    relativeResourceFilePaths.erase(std::remove(relativeResourceFilePaths.begin(), relativeResourceFilePaths.end(), fileName), relativeResourceFilePaths.end());
    resourceFilePaths.erase(std::remove(resourceFilePaths.begin(), resourceFilePaths.end(), filePath), resourceFilePaths.end());
    relativeTextFilePaths.erase(std::remove(relativeTextFilePaths.begin(), relativeTextFilePaths.end(), fileName), relativeTextFilePaths.end());
    textFilePaths.erase(std::remove(textFilePaths.begin(), textFilePaths.end(), filePath), textFilePaths.end());
}

void Project::Save()
{
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.WriteLine("project " + util::ToUtf8(name) + ";");
    formatter.WriteLine("target=" + TargetStr(target) + ";");
    for (const std::string& relativeReferenceFilePath : relativeReferencedProjectFilePaths)
    {
        formatter.WriteLine("reference <" + relativeReferenceFilePath + ">;");
    }
    for (const std::string& relativeSourceFilePath : relativeSourceFilePaths)
    {
        formatter.WriteLine("source <" + relativeSourceFilePath + ">;");
    }
    for (const std::string& relativeResourceFilePath : relativeResourceFilePaths)
    {
        formatter.WriteLine("resource <" + relativeResourceFilePath + ">;");
    }
    for (const std::string & relativeResourceScriptFilePath : relativeResourceScriptFilePaths)
    {
        formatter.WriteLine("rc <" + relativeResourceScriptFilePath + ">;");
    }
    for (const std::string& relativeTextFilePath : relativeTextFilePaths)
    {
        formatter.WriteLine("text <" + relativeTextFilePath + ">;");
    }
    for (const std::string& relativeActionFilePath : relativeActionFilePaths)
    {
        formatter.WriteLine("action <" + relativeActionFilePath + ">;");
    }
}

void Project::AddCompileUnit(CompileUnitNode* compileUnit)
{
    compileUnits.push_back(std::unique_ptr<CompileUnitNode>(compileUnit));
}

soul::ast::Span Project::RootSpan() const
{
    if (!compileUnits.empty())
    {
        return compileUnits.front()->GlobalNsSpan();
    }
    else
    {
        return soul::ast::Span();
    }
}

int32_t Project::RootFileIndex() const
{
    if (!compileUnits.empty())
    {
        return compileUnits.front()->FileIndex();
    }
    else
    {
        return -1;
    }
}

CompileUnitNode* Project::RootCompileUnit() const
{
    if (!compileUnits.empty())
    {
        return compileUnits.front().get();
    }
    else
    {
        return nullptr;
    }
}

} // namespace cmajor::ast
