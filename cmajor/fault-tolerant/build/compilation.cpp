// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.build.compilation;

namespace cmajor::fault::tolerant::build {

Compilation::Compilation() : index(-1)
{
}

Compilation::Compilation(cmajor::fault::tolerant::ast::Project* project_) : project(project_), index(-1)
{
}

const std::string& Compilation::Name() const
{
    if (project)
    {
        return project->Name();
    }
    static std::string empty;
    return empty;
}

const std::string& Compilation::FilePath() const
{
    if (project)
    {
        return project->FilePath();
    }
    static std::string empty;
    return empty;
}

void Compilation::CreateSymbolTable(bool initCore)
{
    symbolTable.reset(new cmajor::fault::tolerant::symbols::SymbolTable());
    symbolTable->SetName(Name());
    if (initCore && symbolTable->Name() == "System.Core")
    {
        cmajor::fault::tolerant::symbols::InitCoreSymbolTable(symbolTable.get());
    }
}

void Compilation::ImportSymbolTables()
{
    std::set<Compilation*> imported;
    ImportSymbolTables(imported, this);
}

void Compilation::ImportSymbolTables(std::set<Compilation*>& imported, Compilation* toCompilation)
{
    if (!toCompilation->GetSymbolTable())
    {
        throw std::runtime_error("Compilation::ImportSymbolTables() : symbol table not created for compilation '" + toCompilation->FilePath() + "'");
    }
    for (const auto& projectReferenceFilePath : project->ReferenceFilePaths())
    {
        Compilation* referencedCompilation = CompilationTable::Instance().GetCompilationByProjectFile(projectReferenceFilePath);
        if (!referencedCompilation)
        {
            throw std::runtime_error("Compilation::ImportSymbolTables() : compilation '" + projectReferenceFilePath + "' not found");
        }
        cmajor::fault::tolerant::symbols::SymbolTable* referencedSymbolTable = referencedCompilation->GetSymbolTable();
        if (!referencedSymbolTable)
        {
            throw std::runtime_error("Compilation::ImportSymbolTables() : symbol table not created for compilation '" + projectReferenceFilePath + "'");
        }
        if (imported.find(referencedCompilation) == imported.end())
        {
            imported.insert(referencedCompilation);
            referencedCompilation->ImportSymbolTables(imported, toCompilation);
            toCompilation->GetSymbolTable()->Import(*referencedSymbolTable);
        }
    }
}

void Compilation::BuildProject()
{
    boundProject.reset(new cmajor::fault::tolerant::binder::BoundProject());
    for (const auto& sourceFilePath : project->SourceFilePaths())
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit = cmajor::fault::tolerant::parser::ParseCmajorCompileUnit(sourceFilePath);
        CompilationTable::Instance().MapCompilationBySourceFile(compileUnit->FilePath(), this);
        cmajor::fault::tolerant::symbols::Source* source = new cmajor::fault::tolerant::symbols::Source(compileUnit->FilePath());
        symbolTable->AddSource(source);
        cmajor::fault::tolerant::symbols::CreateSymbols(compileUnit.get(), symbolTable.get(), source);
        source->SetLineStarts(compileUnit->LineStarts());
        project->AddCompileUnit(compileUnit.release());
    }
    for (const auto& compileUnit : project->CompileUnits()) 
    {
        cmajor::fault::tolerant::binder::BoundCompileUnit* boundCompileUnit = new cmajor::fault::tolerant::binder::BoundCompileUnit();
        boundProject->AddBoundCompileUnit(boundCompileUnit);
        cmajor::fault::tolerant::binder::BindTypes(compileUnit.get(), boundCompileUnit, symbolTable.get());
        cmajor::fault::tolerant::binder::BindStatements(compileUnit.get(), boundCompileUnit, symbolTable.get());
    }
    cmajor::fault::tolerant::symbols::WriteSymbols(symbolTable.get(), project->SymbolFilePath());
}

void Compilation::Read()
{
    cmajor::fault::tolerant::symbols::SymbolReader reader(project->SymbolFilePath());
    symbolTable->Read(reader);
    reader.ResolveSymbols();
    symbolTable->MakeCompoundTypeMap();
    symbolTable->MakeArrayTypeSet();
    for (const auto& source : symbolTable->Sources())
    {
        CompilationTable::Instance().MapCompilationBySourceFile(source->FilePath(), this);
    }
}

std::vector<cmajor::fault::tolerant::symbols::TokenInfo> Compilation::GetTokenInfo(const std::string& sourceFilePath) 
{
    cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit = project->GetCompileUnit(sourceFilePath);
    if (compileUnit)
    {
        return cmajor::fault::tolerant::symbols::GetTokenInfo(compileUnit, symbolTable.get());
    }
    else
    {
        cmajor::fault::tolerant::symbols::Source* source = symbolTable->GetSource(sourceFilePath);
        if (source)
        {
            int sourceIndex = source->Index();
            {
                cmajor::fault::tolerant::symbols::ErrorContainer errors;
                std::vector<std::unique_ptr<cmajor::fault::tolerant::symbols::Symbol>> removedSymbols;
                for (const auto& symbol : source->Symbols())
                {
                    symbol->RemoveFromParent(&errors, removedSymbols);
                }
            }
            symbolTable->Compact();
            std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> parsedCompileUnit = cmajor::fault::tolerant::parser::ParseCmajorCompileUnit(sourceFilePath);
            compileUnit = parsedCompileUnit.get();
            CompilationTable::Instance().MapCompilationBySourceFile(parsedCompileUnit->FilePath(), this);
            source = new cmajor::fault::tolerant::symbols::Source(parsedCompileUnit->FilePath());
            symbolTable->ResetSource(sourceIndex, source);
            cmajor::fault::tolerant::symbols::CreateSymbols(parsedCompileUnit.get(), symbolTable.get(), source);
            source->SetLineStarts(parsedCompileUnit->LineStarts());
            project->AddCompileUnit(parsedCompileUnit.release());
            std::unique_ptr<cmajor::fault::tolerant::binder::BoundCompileUnit> boundCompileUnit(new cmajor::fault::tolerant::binder::BoundCompileUnit());
            cmajor::fault::tolerant::binder::BindTypes(compileUnit, boundCompileUnit.get(), symbolTable.get());
            cmajor::fault::tolerant::binder::BindStatements(compileUnit, boundCompileUnit.get(), symbolTable.get());
            return cmajor::fault::tolerant::symbols::GetTokenInfo(compileUnit, symbolTable.get());
        }
    }
    return std::vector<cmajor::fault::tolerant::symbols::TokenInfo>();
}


CompilationTable& CompilationTable::Instance()
{
    static CompilationTable instance;
    return instance;
}

void CompilationTable::Clear()
{
    compilations.clear();
    projectFileCompilationMap.clear();
    sourceFileCompilationMap.clear();
}

void CompilationTable::AddCompilation(Compilation* compilation)
{
    compilation->SetIndex(compilations.size());
    compilations.push_back(std::unique_ptr<Compilation>(compilation));
    projectFileCompilationMap[compilation->FilePath()] = compilation;
}

Compilation* CompilationTable::GetCompilationByProjectFile(const std::string& projectFilePath) const
{
    auto it = projectFileCompilationMap.find(projectFilePath);
    if (it != projectFileCompilationMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CompilationTable::MapCompilationBySourceFile(const std::string& sourceFilePath, Compilation* compilation)
{
    sourceFileCompilationMap[sourceFilePath] = compilation;
}

Compilation* CompilationTable::GetCompilationBySourceFile(const std::string& sourceFilePath) const
{
    auto it = sourceFileCompilationMap.find(sourceFilePath);
    if (it != sourceFileCompilationMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

} // namespace cmajor::fault::tolerant::binder
