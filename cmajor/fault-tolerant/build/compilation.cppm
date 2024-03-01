// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.build.compilation;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import cmajor.fault.tolerant.parser;
import cmajor.fault.tolerant.binder;
import std.core;

export namespace cmajor::fault::tolerant::build {

class Compilation
{
public:
    Compilation();
    Compilation(cmajor::fault::tolerant::ast::Project* project_);
    cmajor::fault::tolerant::ast::Project* GetProject() const { return project.get(); }
    const std::string& Name() const;
    const std::string& FilePath() const;
    void SetIndex(int index_) { index = index_; }
    int Index() const { return index; }
    void CreateSymbolTable(bool initCore);
    cmajor::fault::tolerant::symbols::SymbolTable* GetSymbolTable() const { return symbolTable.get(); }
    void ImportSymbolTables();
    void BuildProject();
    void Read();
    std::vector<cmajor::fault::tolerant::symbols::TokenInfo> GetTokenInfo(const std::string& sourceFilePath);
private:
    int index;
    std::unique_ptr<cmajor::fault::tolerant::ast::Project> project;
    std::unique_ptr<cmajor::fault::tolerant::symbols::SymbolTable> symbolTable;
    std::unique_ptr<cmajor::fault::tolerant::binder::BoundProject> boundProject;
    void ImportSymbolTables(std::set<Compilation*>& imported, Compilation* toCompilation);
};

class CompilationTable
{
public:
    static CompilationTable& Instance();
    void Clear();
    void AddCompilation(Compilation* compilation);
    Compilation* GetCompilationByProjectFile(const std::string& projectFilePath) const;
    void MapCompilationBySourceFile(const std::string& sourceFilePath, Compilation* compilation);
    Compilation* GetCompilationBySourceFile(const std::string& sourceFilePath) const;
private:
    std::vector<std::unique_ptr<Compilation>> compilations;
    std::map<std::string, Compilation*> projectFileCompilationMap;
    std::map<std::string, Compilation*> sourceFileCompilationMap;
};

} // namespace cmajor::fault::tolerant::build

