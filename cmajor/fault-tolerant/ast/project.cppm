// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.project;

import cmajor.fault.tolerant.ast.compile.unit;
import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class Backend
{
    llvm, systemx, cpp
};

enum class ProjectDeclarationKind
{
    referenceDeclaration, sourceFileDeclaration, resourceFileDeclaration, textFileDeclaration, actionFileDeclaration, targetDeclaration
};

class ProjectDeclaration
{
public:
    ProjectDeclaration(ProjectDeclarationKind kind_);
    virtual ~ProjectDeclaration();
    ProjectDeclarationKind Kind() const { return kind; }
private:
    ProjectDeclarationKind kind;
};

class ReferenceDeclaration : public ProjectDeclaration
{
public:
    ReferenceDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class SourceFileDeclaration : public ProjectDeclaration
{
public:
    SourceFileDeclaration(const std::string & filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class ResourceFileDeclaration : public ProjectDeclaration
{
public:
    ResourceFileDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class TextFileDeclaration : public ProjectDeclaration
{
public:
    TextFileDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class ActionFileDeclaration : public ProjectDeclaration
{
public:
    ActionFileDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

enum class Target
{
    program, winguiapp, winapp, library, winlib, unitTest
};

class TargetDeclaration : public ProjectDeclaration
{
public:
    TargetDeclaration(Target target_);
    Target GetTarget() const { return target; }
private:
    Target target;
};

class Project
{
public:
    Project(const std::string& name_, const std::string& filePath_, const std::string& config_, Backend backend_);
    void AddDeclaration(ProjectDeclaration* declaration);
    void ResolveDeclarations();
    const std::string& Name() const { return name; }
    const std::string& FilePath() const { return filePath; }
    const std::string& Config() const { return config;}
    const std::string& LibDir() const { return libDir; }
    const std::string& SymbolFilePath() const { return symbolFilePath; }
    Backend GetBackend() const { return backend; }
    Target GetTarget() const { return target; }
    const std::vector<std::string>& RelativeReferenceFilePaths() const { return relativeReferenceFilePaths; }
    const std::vector<std::string>& ReferenceFilePaths() const { return referenceFilePaths; }
    const std::vector<std::string>& RelativeSourceFilePaths() const { return relativeSourceFilePaths; }
    const std::vector<std::string>& SourceFilePaths() const { return sourceFilePaths; }
    const std::vector<std::string>& RelativeResourceFilePaths() const { return relativeResourceFilePaths; }
    const std::vector<std::string>& ResourceFilePaths() const { return resourceFilePaths; }
    const std::vector<std::string>& RelativeTextFilePaths() const { return relativeTextFilePaths; }
    const std::vector<std::string>& TextFilePaths() const { return textFilePaths; }
    const std::vector<std::string>& RelativeActionFilePaths() const { return relativeActionFilePaths; }
    const std::vector<std::string>& ActionFilePaths() const { return actionFilePaths; }
    void SetValid(bool valid_) { valid = valid_; }
    bool IsValid() const { return valid; }
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    void AddDependsOnProject(Project* dependsOnProject);
    const std::vector<Project*>& DependsOn() const { return dependsOn; }
    void AddCompileUnit(CompileUnitNode* compileUnit);
    CompileUnitNode* GetCompileUnit(int index) const;
    CompileUnitNode* GetCompileUnit(const std::string& sourceFilePath) const;
    void ResetCompileUnit(int index, CompileUnitNode* compileUnit);
    const std::vector<std::unique_ptr<CompileUnitNode>>& CompileUnits() const { return compileUnits; }
private:
    std::string name;
    std::string filePath;
    std::string libDir;
    std::string symbolFilePath;
    std::string config;
    Backend backend;
    Target target;
    std::vector<std::unique_ptr<ProjectDeclaration>> declarations;
    std::vector<std::string> relativeReferenceFilePaths;
    std::vector<std::string> referenceFilePaths;
    std::vector<std::string> relativeSourceFilePaths;
    std::vector<std::string> sourceFilePaths;
    std::vector<std::string> relativeResourceFilePaths;
    std::vector<std::string> resourceFilePaths;
    std::vector<std::string> relativeTextFilePaths;
    std::vector<std::string> textFilePaths;
    std::vector<std::string> relativeActionFilePaths;
    std::vector<std::string> actionFilePaths;
    std::vector<Project*> dependsOn;
    bool valid;
    int index;
    std::vector<std::unique_ptr<CompileUnitNode>> compileUnits;
    std::map<std::string, CompileUnitNode*> compileUnitMap;
};

} // namespace cmajor::fault::tolerant::ast
