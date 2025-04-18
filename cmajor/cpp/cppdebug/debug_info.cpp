// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.debug.info;

import cmajor.debug.io;
import cmajor.debug.di.variable;
import cmajor.debug.di.type;
import cmajor.debug.debugger.output.writer;
import soul.lexer;
import soul.ast.source.pos;
import util;

namespace cmajor::debug {

std::string SourceSpan::ToString() const
{
    std::string s = std::to_string(line);
    if (line != 0)
    {
        if (scol != 0 && ecol != 0)
        {
            s.append(1, ':').append(std::to_string(scol)).append(1, ':').append(std::to_string(ecol));
        }
    }
    return s;
}

SourceSpan MakeSourceSpan(soul::lexer::FileMap* fileMap, const soul::ast::Span& span, int32_t fileIndex)
{
    const std::vector<int>* lineStartIndeces = fileMap->LineStartIndeces(fileIndex);
    if (!lineStartIndeces) return SourceSpan();
    soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
    SourceSpan sourceSpan(lineColLen.line, lineColLen.col, lineColLen.col + lineColLen.len);
    return sourceSpan;
}

struct VariableNameLess
{
    bool operator()(DIVariable* left, DIVariable* right) const
    {
        return left->Name() < right->Name();
    }
};

Frame::Frame() : level(0), func(), file(), line(0), scol(0), ecol(0)
{
}

std::string Frame::ToString(bool printLevel) const
{
    std::string s;
    if (printLevel)
    {
        s.append(std::to_string(level)).append(": ");
    }
    if (IsEmpty())
    {
        s.append("<frame empty>");
    }
    else
    {
        s.append(func);
        s.append(1, ' ');
        s.append(file).append(":").append(std::to_string(line));
        if (scol != 0 && ecol != 0)
        {
            s.append(1, ':').append(std::to_string(scol)).append(1, ':').append(std::to_string(ecol));
        }
    }
    return s;
}

std::unique_ptr<util::JsonValue> Frame::ToJson(bool includeLevel) const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    if (includeLevel)
    {
        jsonObject->AddField(U"level", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(level)))));
    }
    if (!IsEmpty())
    {
        jsonObject->AddField(U"func", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(func))));
        jsonObject->AddField(U"file", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(file))));
        jsonObject->AddField(U"line", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(line)))));
        if (scol != 0 && ecol != 0)
        {
            jsonObject->AddField(U"scol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(scol)))));
            jsonObject->AddField(U"ecol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(ecol)))));
        }
    }
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

InstructionLocation::InstructionLocation() : span(), projectIndex(-1), compileUnitIndex(-1), cppLineNumber(-1)
{
}

std::string InstructionFlagsStr(InstructionFlags flags)
{
    std::string s;
    if ((flags & InstructionFlags::entryCode) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("entry");
    }
    if ((flags & InstructionFlags::exitCode) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("exit");
    }
    if ((flags & InstructionFlags::beginBrace) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("beginBrace");
    }
    if ((flags & InstructionFlags::endBrace) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("endBrace");
    }
    if ((flags & InstructionFlags::startFunction) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("startFunction");
    }
    if ((flags & InstructionFlags::throwInst) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("throw");
    }
    if ((flags & InstructionFlags::catchInst) != InstructionFlags::none)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("catch");
    }
    return s;
}

Instruction::Instruction(CompileUnitFunction* compileUnitFunction_, int32_t cppLineNumber_, const SourceSpan& span_, int32_t cppLineIndex_,
    int16_t scopeId_, InstructionFlags flags_) :
    compileUnitFunction(compileUnitFunction_), cppLineNumber(cppLineNumber_), span(span_), cppLineIndex(cppLineIndex_), scopeId(scopeId_),
    flags(flags_), next(nullptr)
{
}

SourceFileReference* Instruction::GetSourceFileReference() const
{
    return compileUnitFunction->GetSourceFileReference();
}

Frame Instruction::GetCmajorFrame() const
{
    Frame frame;
    Function* function = compileUnitFunction->GetFunction();
    frame.func = function->FullName();
    const SourceFile& sourceFile = compileUnitFunction->GetSourceFile();
    frame.file = sourceFile.FilePath();
    frame.line = span.line;
    frame.scol = span.scol;
    frame.ecol = span.ecol;
    return frame;
}

void Instruction::PrintSource(util::CodeFormatter& formatter)
{
    SourceFile& sourceFile = compileUnitFunction->GetSourceFile();
    sourceFile.Print(formatter, span.line, this, true);
}

std::string Instruction::GetExplicitCppLocationArgs() const
{
    std::string cppLocationArgs = "--source ";
    std::string projectDirectory = compileUnitFunction->GetCompileUnit()->GetProject()->InternalDirectoryPath();
    std::string cppSourceFileName = compileUnitFunction->GetCompileUnit()->BaseName() + ".cpp";
    cppLocationArgs.append(util::QuotedPath(util::Path::Combine(projectDirectory, cppSourceFileName)));
    cppLocationArgs.append(" --line ").append(std::to_string(cppLineNumber));
    return cppLocationArgs;
}

std::string Instruction::GetFileLineCppLocationArgs() const
{
    std::string cppLocationArgs;
    std::string projectDirectory = compileUnitFunction->GetCompileUnit()->GetProject()->InternalDirectoryPath();
    std::string cppSourceFileName = compileUnitFunction->GetCompileUnit()->BaseName() + ".cpp";
    cppLocationArgs.append(util::QuotedPath(util::Path::Combine(projectDirectory, cppSourceFileName + ":" + std::to_string(cppLineNumber))));
    return cppLocationArgs;
}

Scope* Instruction::GetScope() const
{
    if (scopeId != -1)
    {
        return compileUnitFunction->Scopes()[scopeId].get();
    }
    else
    {
        return nullptr;
    }
}

FunctionScope* Instruction::GetFunctionScope() const
{
    Scope* scope = GetScope();
    if (scope && scope->IsFunctionScope())
    {
        return static_cast<FunctionScope*>(scope);
    }
    return nullptr;
}

bool Instruction::IsStopInstruction() const
{
    if ((flags & (InstructionFlags::beginBrace | InstructionFlags::endBrace)) != InstructionFlags::none)
    {
        return true;
    }
    if ((flags & (InstructionFlags::entryCode | InstructionFlags::exitCode)) != InstructionFlags::none)
    {
        return false;
    }
    if ((flags & (InstructionFlags::startFunction)) != InstructionFlags::none)
    {
        return false;
    }
    return true;
}

bool Instruction::AtEndBrace() const
{
    return (flags & InstructionFlags::endBrace) != InstructionFlags::none;
}

void AddToNextSet(std::set<Instruction*>& nextSet, Instruction* inst)
{
    CompileUnitFunction* function = inst->GetCompileUnitFunction();
    if (function)
    {
        ControlFlowGraphNode* node = function->GetControlFlowGraph().GetNodeByCppLineNumber(inst->CppLineNumber());
        if (node)
        {
            for (int32_t nextId : node->Next())
            {
                ControlFlowGraphNode* next = function->GetControlFlowGraph().GetNodeById(nextId);
                if (next)
                {
                    Instruction* inst = next->Inst();
                    if (!inst)
                    {
                        CompileUnit* compileUnit = function->GetCompileUnit();
                        if (compileUnit)
                        {
                            inst = compileUnit->GetInstruction(next->CppLineNumber());
                            if (inst)
                            {
                                next->SetInst(inst);
                            }
                        }
                    }
                    if (inst)
                    {
                        nextSet.insert(inst);
                    }
                }
            }
        }
    }
}

Scope::~Scope()
{
}

ControlFlowGraphNode::ControlFlowGraphNode(int32_t nodeId_, const SourceSpan& span_, int32_t cppLineIndex_, int32_t cppLineNumber_) :
    nodeId(nodeId_), span(span_), cppLineIndex(cppLineIndex_), cppLineNumber(cppLineNumber_), inst(nullptr)
{
}

void ControlFlowGraphNode::AddNext(int32_t next)
{
    nextSet.insert(next);
}

ControlFlowGraph::ControlFlowGraph()
{
}

void ControlFlowGraph::AddNode(ControlFlowGraphNode* node)
{
    nodeIdMap[node->NodeId()] = node;
    cppLineNodeMap[node->CppLineNumber()] = node;
    nodes.push_back(std::unique_ptr<ControlFlowGraphNode>(node));
}

ControlFlowGraphNode* ControlFlowGraph::GetNodeById(int32_t nodeId) const
{
    auto it = nodeIdMap.find(nodeId);
    if (it != nodeIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

ControlFlowGraphNode* ControlFlowGraph::GetNodeByCppLineNumber(int32_t cppLineNumber) const
{
    auto it = cppLineNodeMap.find(cppLineNumber);
    if (it != cppLineNodeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

FunctionScope::FunctionScope(CompileUnitFunction* compileUnitFunction_, int16_t id_, int16_t parentScopeId_) :
    compileUnitFunction(compileUnitFunction_), id(id_), parentScopeId(parentScopeId_)
{
}

std::string FunctionScope::Name() const
{
    std::string name = compileUnitFunction->GetFunction()->FullName() + "." + std::to_string(id);
    return name;
}

Scope* FunctionScope::GetParentScope() const
{
    if (parentScopeId != -1)
    {
        return compileUnitFunction->Scopes()[parentScopeId].get();
    }
    else
    {
        return nullptr;
    }
}

void FunctionScope::AddLocalVariable(DIVariable* localVariable)
{
    localVariables.push_back(std::unique_ptr<DIVariable>(localVariable));
    localVariableMap[localVariable->Name()] = localVariable;
}

DIVariable* FunctionScope::GetVariable(const std::string& name) const
{
    auto it = localVariableMap.find(name);
    if (it != localVariableMap.cend())
    {
        return it->second;
    }
    else
    {
        Scope* parentScope = GetParentScope();
        if (parentScope)
        {
            return parentScope->GetVariable(name);
        }
        else
        {
            return nullptr;
        }
    }
}

bool operator==(const SourceFileKey& left, const SourceFileKey& right)
{
    return left.fileIndex == right.fileIndex && left.sourceModuleId == right.sourceModuleId;
}

bool operator<(const SourceFileKey& left, const SourceFileKey& right)
{
    if (left.sourceModuleId < right.sourceModuleId) return true;
    if (left.sourceModuleId > right.sourceModuleId) return false;
    return left.fileIndex < right.fileIndex;
}

CompileUnitFunction::CompileUnitFunction(CompileUnit* compileUnit_, int32_t fileIndex_, const util::uuid& sourceModuleId_, const util::uuid& functionId_) :
    compileUnit(compileUnit_), fileIndex(fileIndex_), sourceModuleId(sourceModuleId_), functionId(functionId_)
{
}

SourceFileReference* CompileUnitFunction::GetSourceFileReference() const
{
    DebugInfo* debugInfo = compileUnit->GetProject()->GetDebugInfo();
    Project* sourceProject = debugInfo->GetProjectById(sourceModuleId);
    return sourceProject->GetSourceFileReference(fileIndex);
}

SourceFile& CompileUnitFunction::GetSourceFile() const
{
    DebugInfo* debugInfo = compileUnit->GetProject()->GetDebugInfo();
    Project* sourceProject = debugInfo->GetProjectById(sourceModuleId);
    return sourceProject->GetSourceFile(fileIndex);
}

Function* CompileUnitFunction::GetFunction() const
{
    return compileUnit->GetProject()->GetFunction(functionId);
}

void CompileUnitFunction::AddInstruction(Instruction* instruction)
{
    instructions.push_back(std::unique_ptr<Instruction>(instruction));
}

Instruction* CompileUnitFunction::GetInstruction(int index) const
{
    if (index >= 0 && index < instructions.size())
    {
        return instructions[index].get();
    }
    else
    {
        Function* fun = GetFunction();
        throw std::runtime_error("error: function '" + fun->FullName() + "' in compile unit '" + GetCompileUnit()->BaseName() +
            "' does not have instruction number " + std::to_string(index));
    }
}

void CompileUnitFunction::AddScope(FunctionScope* scope)
{
    scopes.push_back(std::unique_ptr<FunctionScope>(scope));
}

void CompileUnitFunction::AddLocalVariable(DIVariable* localVariable)
{
    localVariables.push_back(localVariable);
}

CompileUnit::CompileUnit(Project* project_, const std::string& baseName_) : project(project_), baseName(baseName_)
{
}

void CompileUnit::AddCompileUnitFunction(CompileUnitFunction* compileUnitFunction)
{
    compileUnitFunctions.push_back(std::unique_ptr<CompileUnitFunction>(compileUnitFunction));
}

void CompileUnit::AddInstruction(Instruction* instruction)
{
    instructionMap[instruction->CppLineNumber()] = instruction;
}

Instruction* CompileUnit::GetInstruction(int32_t cppLineNumber) const
{
    auto it = instructionMap.find(cppLineNumber);
    if (it != instructionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Instruction* CompileUnit::GetNearestInstruction(int32_t cppLineNumber) const
{
    auto it = instructionMap.lower_bound(cppLineNumber);
    if (it != instructionMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

SourceFileReference::SourceFileReference(int32_t fileIndex_, const std::string& filePath_) : fileIndex(fileIndex_), filePath(filePath_)
{
}

Function::Function(const util::uuid& id_, const std::string& fullName_, const std::string& mangledName_) : id(id_), fullName(fullName_), mangledName(mangledName_)
{
}

Project::Project(DebugInfo* debugInfo_, const std::string& name_, const std::string& directoryPath_, const std::string& cmajorRootPrefix_, const util::uuid& moduleId_) :
    debugInfo(debugInfo_), name(name_), directoryPath(directoryPath_), cmajorRootPrefix(cmajorRootPrefix_), 
    moduleId(moduleId_), mainFunction(nullptr), longType(nullptr), boolType(nullptr)
{
}

void Project::AddCompileUnit(CompileUnit* compileUnit)
{
    compileUnits.push_back(std::unique_ptr<CompileUnit>(compileUnit));
    compileUnitMap[compileUnit->BaseName()] = compileUnit;
}

CompileUnit* Project::GetCompileUnit(const std::string& baseName) const
{
    auto it = compileUnitMap.find(baseName);
    if (it != compileUnitMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("compile unit with base name '" + baseName + "' not found from project '" + name + "'");
    }
}

std::string Project::InternalDirectoryPath() const
{
    std::string currentCmajorRootPrefix = GetCurrentCmajorRootPrefix();
    if (cmajorRootPrefix != currentCmajorRootPrefix)
    {
        if (directoryPath.starts_with(currentCmajorRootPrefix))
        {
            std::string suffix = directoryPath.substr(currentCmajorRootPrefix.length() + 1);
            return util::Path::Combine(cmajorRootPrefix, suffix);
        }
    }
    return directoryPath;
}

CompileUnit* Project::GetCompileUnit(int32_t compileUnitIndex) const
{
    return compileUnits[compileUnitIndex].get();
}

void Project::AddSourceFileReference(SourceFileReference* sourceFileReference)
{
    sourceFileReferences.push_back(std::unique_ptr<SourceFileReference>(sourceFileReference));
    sourceFileReferenceMap[sourceFileReference->FileIndex()] = sourceFileReference;
}

SourceFileReference* Project::GetSourceFileReference(int32_t fileIndex) const
{
    auto it = sourceFileReferenceMap.find(fileIndex);
    if (it != sourceFileReferenceMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

SourceFile& Project::GetSourceFile(int32_t fileIndex) const
{
    auto it = sourceFileReferenceMap.find(fileIndex);
    if (it != sourceFileReferenceMap.cend())
    {
        SourceFileReference* sourceFileReference = it->second;
        return debugInfo->GetSourceFileCache().GetSourceFile(sourceFileReference->FilePath());
    }
    else
    {
        throw std::runtime_error("source file reference with index '" + std::to_string(fileIndex) + "' not found from project '" + name + "'");
    }
}

void Project::AddFunction(Function* function)
{
    functions.push_back(std::unique_ptr<Function>(function));
    functionMap[function->Id()] = function;
}

Function* Project::GetFunction(const util::uuid& functionId) const
{
    auto it = functionMap.find(functionId);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("function with id '" + util::ToString(functionId) + "' not found from project '" + name + "'");
    }
}

CompileUnitFunction* Project::GetMainFunction() const
{
    if (mainFunction)
    {
        return mainFunction;
    }
    else
    {
        throw std::runtime_error("internal error: main function not set in the debug information file '" + debugInfo->FilePath() + "'");
    }
}

void Project::AddType(DIType* type)
{
    if (type->GetKind() == DIType::Kind::primitiveType)
    {
        DIPrimitiveType* primitiveType = static_cast<DIPrimitiveType*>(type);
        if (primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::longType)
        {
            longType = primitiveType;
        }
        else if (primitiveType->GetPrimitiveTypeKind() == DIPrimitiveType::Kind::boolType)
        {
            boolType = primitiveType;
        }
    }
    typeMap[type->Id()] = type;
    types.push_back(std::unique_ptr<DIType>(type));
    switch (type->GetKind())
    {
    case DIType::Kind::pointerType:
    {
        DIPointerType* pointerType = static_cast<DIPointerType*>(type);
        pointerTypeMap[pointerType->PointedTypeId()] = pointerType;
        break;
    }
    case DIType::Kind::referenceType:
    {
        DIReferenceType* referenceType = static_cast<DIReferenceType*>(type);
        referenceTypeMap[referenceType->BaseTypeId()] = referenceType;
        break;
    }
    case DIType::Kind::constType:
    {
        DIConstType* constType = static_cast<DIConstType*>(type);
        constTypeMap[constType->BaseTypeId()] = constType;
        break;
    }
    }
    debugInfo->AddType(type);
}

DIType* Project::GetType(const util::uuid& typeId) const
{
    auto it = typeMap.find(typeId);
    if (it != typeMap.cend())
    {
        return it->second;
    }
    else
    {
        return debugInfo->GetType(util::ToString(typeId));
    }
}

DIPointerType* Project::GetPointerType(DIType* pointedToType)
{
    auto it = pointerTypeMap.find(pointedToType->Id());
    if (it != pointerTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        DIPointerType* pointerType = new DIPointerType();
        pointerType->SetId(util::random_uuid());
        pointerType->SetPointedTypeId(pointedToType->Id());
        pointerType->SetName(pointedToType->Name() + "*");
        pointerType->SetIrName(pointedToType->IrName() + "*");
        pointerType->SetProject(pointedToType->GetProject());
        AddType(pointerType);
        return pointerType;
    }
}

DIReferenceType* Project::GetReferenceType(DIType* referredToType)
{
    auto it = referenceTypeMap.find(referredToType->Id());
    if (it != referenceTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        DIReferenceType* referenceType = new DIReferenceType();
        referenceType->SetId(util::random_uuid());
        referenceType->SetBaseTypeId(referredToType->Id());
        referenceType->SetName(referredToType->Name() + "&");
        referenceType->SetIrName(referredToType->IrName() + "*");
        referenceType->SetProject(referredToType->GetProject());
        AddType(referenceType);
        return referenceType;
    }
}

DIConstType* Project::GetConstType(DIType* baseType)
{
    auto it = constTypeMap.find(baseType->Id());
    if (it != constTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        DIConstType* constType = new DIConstType();
        constType->SetId(util::random_uuid());
        constType->SetBaseTypeId(baseType->Id());
        constType->SetName("const " + baseType->Name());
        constType->SetIrName(baseType->IrName());
        constType->SetProject(baseType->GetProject());
        AddType(constType);
        return constType;
    }
}

SourceFile::SourceFile(DebugInfo* debugInfo_, const std::string& filePath_) : debugInfo(debugInfo_), filePath(filePath_)
{
    std::string content = util::ReadFile(filePath, true);
    int state = 0;
    std::string line;
    for (char c : content)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '\r')
            {
                state = 1;
            }
            else if (c == '\n')
            {
                lines.push_back(line);
                line.clear();
                state = 2;
            }
            else
            {
                line.append(1, c);
            }
            break;
        }
        case 1:
        {
            if (c == '\n')
            {
                lines.push_back(line);
                line.clear();
                state = 2;
            }
            else if (c == '\r')
            {
                lines.push_back(line);
                line.clear();
            }
            else
            {
                lines.push_back(line);
                line.clear();
                line.append(1, c);
                state = 0;
            }
            break;
        }
        case 2:
        {
            if (c == '\r')
            {
                state = 1;
            }
            else if (c == '\n')
            {
                lines.push_back(line);
                line.clear();
            }
            else
            {
                line.append(1, c);
                state = 0;
            }
            break;
        }
        }
    }
    lines.push_back(line);
}

void SourceFile::Print(util::CodeFormatter& formatter, int lineNumber, Instruction* currentInstruction, bool window)
{
    if (lineNumber == 0)
    {
        lineNumber = 1;
    }
    SourceFileReference* sourceFileReference = nullptr;
    if (currentInstruction)
    {
        sourceFileReference = currentInstruction->GetCompileUnitFunction()->GetSourceFileReference();
    }
    bool sourceFileMatch = false;
    if (sourceFileReference)
    {
        sourceFileMatch = sourceFileReference->FilePath() == filePath;
    }
    int currentInstructionLineNumber = -1;
    if (currentInstruction)
    {
        currentInstructionLineNumber = currentInstruction->GetSourceSpan().line;
    }
    int windowSize = debugInfo->GetSourceFileWindowSize();
    int startLineNumber = lineNumber - windowSize;
    int endLineNumber = lineNumber + windowSize;
    if (!window)
    {
        startLineNumber = lineNumber;
        endLineNumber = lineNumber + 2 * windowSize - 1;
    }
    int numDigits = util::Log10(std::min(int(lines.size()), endLineNumber));
    for (int i = startLineNumber; i <= endLineNumber; ++i)
    {
        if (i > 0 && i <= lines.size())
        {
            formatter.Write(util::Format(std::to_string(i), numDigits, util::FormatWidth::exact, util::FormatJustify::right, ' '));
            const std::string& line = lines[i - 1];
            if (sourceFileMatch && i == currentInstructionLineNumber)
            {
                formatter.Write("> ");
                SourceSpan span = currentInstruction->GetSourceSpan();
                if (span.scol != 0 && span.ecol != 0)
                {
                    formatter.Write(line.substr(0, span.scol - 1));
                    formatter.Write(util::fgGreen);
                    formatter.Write(line.substr(span.scol - 1, span.ecol - span.scol));
                    formatter.Write(util::reset);
                    formatter.WriteLine(line.substr(span.ecol - 1));
                }
                else
                {
                    formatter.WriteLine(line);
                }
            }
            else
            {
                formatter.Write("  ");
                formatter.WriteLine(line);
            }
        }
    }
    if (endLineNumber >= lines.size())
    {
        formatter.WriteLine(std::to_string(lines.size() + 1) + ":-end of file-");
    }
}

SourceFile& SourceFileCache::GetSourceFile(const std::string& filePath)
{
    auto it = sourceFileMap.find(filePath);
    if (it != sourceFileMap.cend())
    {
        return *it->second;
    }
    SourceFile* sourceFile = new SourceFile(debugInfo, filePath);
    sourceFileMap[sourceFile->FilePath()] = sourceFile;
    sourceFiles.push_back(std::unique_ptr<SourceFile>(sourceFile));
    return *sourceFile;
}

SourceFileCache::SourceFileCache(DebugInfo* debugInfo_) : debugInfo(debugInfo_)
{
}

SourceLineMap::SourceLineMap(DebugInfo* debugInfo_) : debugInfo(debugInfo_)
{
}

void SourceLineMap::AddInstructionLocation(const InstructionLocation& location)
{
    std::vector<InstructionLocation>& locations = sourceLineLocationsMap[location.span.line];
    locations.push_back(location);
}

std::vector<Instruction*> SourceLineMap::GetInstructions(int32_t sourceLineNumber) const
{
    std::map<int32_t, std::vector<InstructionLocation>>::const_iterator it;
    std::vector<Instruction*> instructions;
    if (sourceLineNumber == 0)
    {
        it = sourceLineLocationsMap.begin();
    }
    else
    {
        it = sourceLineLocationsMap.find(sourceLineNumber);
    }
    if (it != sourceLineLocationsMap.cend())
    {
        const std::vector<InstructionLocation>& locations = it->second;
        for (const InstructionLocation& location : locations)
        {
            Instruction* instruction = debugInfo->GetInstruction(location);
            instructions.push_back(instruction);
        }
    }
    return instructions;
}

SourceLocation::SourceLocation() : path(), line(0)
{
}

SourceLocation::SourceLocation(const std::string& path_, int line_) : path(path_), line(line_)
{
}

std::string SourceLocation::ToString() const
{
    std::string s = path;
    if (line != 0)
    {
        s.append(":").append(std::to_string(line));
    }
    return s;
}

SourceFileMap::SourceFileMap(DebugInfo* debugInfo_) : debugInfo(debugInfo_)
{
}

std::string SourceFileMap::GetSourceFilePath(const SourceLocation& location) const
{
    std::string projectName = util::Path::GetDirectoryName(location.path);
    Project* project = debugInfo->GetProjectByName(projectName);
    std::string locationPath;
    if (project)
    {
        locationPath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(project->DirectoryPath(), ".."), ".."), ".."), ".."),
            util::Path::GetFileName(location.path)));
    }
    else
    {
        locationPath = location.path;
    }
    auto it = sourceFileMap.find(locationPath);
    if (it != sourceFileMap.cend())
    {
        return it->first;
    }
    else
    {
        std::vector<std::map<std::string, SourceLineMap*>::const_iterator> matchingSourceFilePathIterators;
        std::vector<std::string> locationPathComponents = util::Split(locationPath, '/');
        int nlpc = locationPathComponents.size();
        for (std::map<std::string, SourceLineMap*>::const_iterator it = sourceFileMap.cbegin(); it != sourceFileMap.cend(); ++it)
        {
            const std::string& sourceFilePath = it->first;
            bool sourceFilePathMatches = true;
            std::vector<std::string> sourceFileComponents = util::Split(sourceFilePath, '/');
            int nsfc = sourceFileComponents.size();
            int n = std::min(nlpc, nsfc);
            for (int i = 0; i < n; ++i)
            {
                if (locationPathComponents[nlpc - i - 1] != sourceFileComponents[nsfc - i - 1])
                {
                    sourceFilePathMatches = false;
                    break;
                }
            }
            if (sourceFilePathMatches)
            {
                matchingSourceFilePathIterators.push_back(it);
            }
        }
        if (matchingSourceFilePathIterators.empty())
        {
            throw std::runtime_error("no source file path matched location '" + location.ToString() + "'");
        }
        else if (matchingSourceFilePathIterators.size() == 1)
        {
            auto it = matchingSourceFilePathIterators.front();
            return it->first;
        }
        else
        {
            std::string matchingSourceFiles("    ");
            bool first = true;
            for (const auto it : matchingSourceFilePathIterators)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    matchingSourceFiles.append("\n    ");
                }
                std::string projectDir = util::Path::GetDirectoryName(it->first);
                Project* project = debugInfo->GetProjectByPath(projectDir);
                matchingSourceFiles.append(it->first);
                if (project != nullptr)
                {
                    matchingSourceFiles.append(", project name='").append(project->Name() + "'");
                }
            }
            throw std::runtime_error("More that one source file matched source location '" + location.ToString() + "':\n" + matchingSourceFiles + "\n" +
                "Give source location with project name '/' prefix to disambiguate.");
        }
    }
}

std::vector<Instruction*> SourceFileMap::GetInstructions(const SourceLocation& location) const
{
    std::string sourceFilePath = GetSourceFilePath(location);
    std::vector<Instruction*> instructions;
    auto it = sourceFileMap.find(sourceFilePath);
    if (it != sourceFileMap.cend())
    {
        SourceLineMap* lineMap = it->second;
        instructions = lineMap->GetInstructions(location.line);
    }
    if (instructions.empty())
    {
        throw std::runtime_error("no instructions matched source location '" + location.ToString() + "'");
    }
    return instructions;
}

void SourceFileMap::AddSourceFile(const SourceFileKey& sourceFileKey, const std::string& sourceFilePath)
{
    sourceFileKeyMap.insert(std::make_pair(sourceFileKey, sourceFilePath));
}

void SourceFileMap::AddInstructionLocation(const SourceFileKey& sourceFileKey, const InstructionLocation& location)
{
    sourceFileKeyLocationsMap[sourceFileKey].push_back(location);
}

void SourceFileMap::ProcessSourceFileKeyLocationsMap()
{
    for (const std::pair<SourceFileKey, std::string>& p : sourceFileKeyMap)
    {
        AddInstructionLocations(p.first, p.second);
    }
    sourceFileKeyMap.clear();
    sourceFileKeyLocationsMap.clear();
}

void SourceFileMap::AddInstructionLocations(const SourceFileKey& sourceFileKey, const std::string& sourceFilePath)
{
    if (sourceFilePath.empty()) return;
    auto it = sourceFileKeyLocationsMap.find(sourceFileKey);
    if (it != sourceFileKeyLocationsMap.cend())
    {
        const std::vector<InstructionLocation>& locations = it->second;
        SourceLineMap* lineMap;
        auto it = sourceFileMap.find(sourceFilePath);
        if (it == sourceFileMap.cend())
        {
            lineMap = new SourceLineMap(debugInfo);
            sourceLineMaps.push_back(std::unique_ptr<SourceLineMap>(lineMap));
            sourceFileMap[sourceFilePath] = lineMap;
        }
        else
        {
            lineMap = it->second;
        }
        for (const InstructionLocation& location : locations)
        {
            lineMap->AddInstructionLocation(location);
        }
    }
}

std::string GetCurrentCmajorRootPrefix()
{
    std::string currentCmajorRootPrefix = util::GetFullPath(util::CmajorRoot());
    return currentCmajorRootPrefix;
}

DebugInfo::DebugInfo(const std::string& filePath_) : filePath(filePath_), mainProject(nullptr), sourceFileCache(this), sourceFileMap(this), sourceFileWindowSize(5)
{
}

void DebugInfo::SetMainProject(Project* mainProject_)
{
    mainProject = mainProject_;
}

Project* DebugInfo::GetMainProject() const
{
    if (mainProject)
    {
        return mainProject;
    }
    else
    {
        throw std::runtime_error("internal error: main project not set in the debug information file '" + filePath + "'");
    }
}

void DebugInfo::AddProject(Project* project)
{
    projects.push_back(std::unique_ptr<Project>(project));
    projectPathMap[project->DirectoryPath()] = project;
    projectNameMap[project->Name()] = project;
    projectIdMap[project->ModuleId()] = project;
}

Project* DebugInfo::GetProjectByInternalPath(const std::string& internalPath) const
{
    for (const auto& project : projects)
    {
        if (internalPath.starts_with(project->CmajorRootPrefix()))
        {
            return project.get();
        }
    }
    return nullptr;
}

Project* DebugInfo::GetProjectByPath(const std::string& directoryPath) const
{
    auto it = projectPathMap.find(directoryPath);
    if (it != projectPathMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Project* DebugInfo::GetProjectByName(const std::string& projectName) const
{
    auto it = projectNameMap.find(projectName);
    if (it != projectNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Project* DebugInfo::GetProjectById(const util::uuid& projectId) const
{
    auto it = projectIdMap.find(projectId);
    if (it != projectIdMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("project id '" + util::ToString(projectId) + "' not found");
    }
}

Instruction* DebugInfo::GetEntryInstruction(CompileUnitFunction* compileUnitFunction) const
{
    return compileUnitFunction->GetInstruction(0);
}

Instruction* DebugInfo::GetMainFunctionEntryInstruction() const
{
    Project* mainProject = GetMainProject();
    CompileUnitFunction* mainFunction = mainProject->GetMainFunction();
    int index = 0;
    Instruction* inst = mainFunction->GetInstruction(index);
    while ((inst->GetFlags() & InstructionFlags::startFunction) != InstructionFlags::none)
    {
        ++index;
        inst = mainFunction->GetInstruction(index);
    }
    return inst;
}

Instruction* DebugInfo::GetNearestInstruction(const Frame& frame) const
{
    try
    {
        if (frame.func == "main" || frame.func == "wmain" || frame.func == "WinMain" || frame.func == "wWinMain")
        {
            return nullptr;
        }
        std::string projectDir = util::Path::GetDirectoryName(frame.file);
        Project* project = GetProjectByPath(projectDir);
        if (project)
        {
            std::string compileUnitBaseName = util::Path::GetFileNameWithoutExtension(frame.file);
            CompileUnit* compileUnit = project->GetCompileUnit(compileUnitBaseName);
            if (compileUnit)
            {
                Instruction* instruction = compileUnit->GetNearestInstruction(frame.line);
                return instruction;
            }
        }
    }
    catch (...)
    {
    }
    return nullptr;
}

Instruction* DebugInfo::GetInstruction(const Frame& frame, DebuggerOutputWriter& outputWriter) const
{
    if (frame.func == "main" || frame.func == "wmain" || frame.func == "WinMain" || frame.func == "wWinMain")
    {
        return nullptr;
    }
    try
    {
        std::string projectDir = util::Path::GetDirectoryName(frame.file);
        Project* project = GetProjectByPath(projectDir);
        if (project)
        {
            std::string compileUnitBaseName = util::Path::GetFileNameWithoutExtension(frame.file);
            CompileUnit* compileUnit = project->GetCompileUnit(compileUnitBaseName);
            if (compileUnit)
            {
                Instruction* instruction = compileUnit->GetInstruction(frame.line);
                return instruction;
            }
        }
    }
    catch (const std::exception& ex)
    {
        outputWriter.WriteWarning("error getting instruction from frame: " + frame.ToString(false) + ": " + ex.what());
    }
    return nullptr;
}

Instruction* DebugInfo::GetInstruction(const InstructionLocation& location) const
{
    Project* project = projects[location.projectIndex].get();
    CompileUnit* compileUnit = project->GetCompileUnit(location.compileUnitIndex);
    Instruction* instruction = compileUnit->GetInstruction(location.cppLineNumber);
    return instruction;
}

DIType* DebugInfo::GetPolymorphicType(const std::string& vmtVarName) const
{
    auto it = polymorphicTypeMap.find(vmtVarName);
    if (it != polymorphicTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void DebugInfo::AddPolymorphicType(DIClassType* polymorphicType)
{
    polymorphicTypeMap[polymorphicType->VmtVariableName()] = polymorphicType;
}

DIType* DebugInfo::GetType(const std::string& typeId) const
{
    auto it = typeMap.find(typeId);
    if (it != typeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void DebugInfo::AddType(DIType* type)
{
    typeMap[util::ToString(type->Id())] = type;
}

void DebugInfo::AddThrowInstruction(Instruction* throwInstruction)
{
    throwInstructions.push_back(throwInstruction);
}

void DebugInfo::AddCatchInstruction(Instruction* catchInstruction)
{
    catchInstructions.push_back(catchInstruction);
}

void DebugInfo::ProcessSourceFileKeyLocationsMap()
{
    sourceFileMap.ProcessSourceFileKeyLocationsMap();
}

std::unique_ptr<DebugInfo> ReadDebugInfo(const std::string& cmdbFilePath)
{
    std::unique_ptr<DebugInfo> debugInfo(new DebugInfo(cmdbFilePath));
    util::FileStream fileStream(cmdbFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamReader reader(bufferedStream);
    ReadCmdbFileTag(reader, cmdbFilePath);
    std::string mainProjectName;
    ReadMainProjectName(reader, mainProjectName);
    int32_t numProjects;
    ReadNumberOfProjects(reader, numProjects);
    for (int32_t i = 0; i < numProjects; ++i)
    {
        int32_t projectIndex = i;
        std::string projectName;
        std::string projectDirectoryPath;
        std::string cmajorRootPrefix;
        int32_t numCompileUnits;
        util::uuid moduleId;
        util::uuid mainFunctionId;
        ReadProjectTableHeader(reader, projectName, projectDirectoryPath, cmajorRootPrefix, moduleId, numCompileUnits, mainFunctionId);
        std::unique_ptr<Project> project(new Project(debugInfo.get(), projectName, projectDirectoryPath, cmajorRootPrefix, moduleId));
        if (projectName == mainProjectName)
        {
            debugInfo->SetMainProject(project.get());
        }
        for (int32_t i = 0; i < numCompileUnits; ++i)
        {
            int32_t compileUnitIndex = i;
            std::string compileUnitBaseName;
            ReadProjectTableRecord(reader, compileUnitBaseName);
            std::unique_ptr<CompileUnit> compileUnit(new CompileUnit(project.get(), compileUnitBaseName));
            int32_t numFunctions;
            ReadNumberOfCompileUnitFunctionRecords(reader, numFunctions);
            for (int32_t i = 0; i < numFunctions; ++i)
            {
                int32_t fileIndex;
                util::uuid sourceModuleId;
                util::uuid functionId;
                ReadCompileUnitFunctionRecord(reader, fileIndex, sourceModuleId, functionId);
                std::unique_ptr<CompileUnitFunction> compileUnitFunction(new CompileUnitFunction(compileUnit.get(), fileIndex, sourceModuleId, functionId));
                if (!mainFunctionId.is_nil() && functionId == mainFunctionId)
                {
                    project->SetMainFunction(compileUnitFunction.get());
                }
                int32_t numInstructionRecords;
                ReadNumberOfInstructionRecords(reader, numInstructionRecords);
                Instruction* prev = nullptr;
                bool firstThrowInst = true;
                bool firstCatchInst = true;
                for (int32_t i = 0; i < numInstructionRecords; ++i)
                {
                    int32_t cppLineNumber;
                    SourceSpan span;
                    int32_t cppLineIndex;
                    int16_t scopeId;
                    int16_t flags;
                    ReadInstructionRecord(reader, cppLineNumber, span, cppLineIndex, scopeId, flags);
                    std::unique_ptr<Instruction> instruction(new Instruction(compileUnitFunction.get(), cppLineNumber, span, cppLineIndex, scopeId,
                        static_cast<InstructionFlags>(flags)));
                    if (prev)
                    {
                        prev->SetNext(instruction.get());
                    }
                    prev = instruction.get();
                    compileUnit->AddInstruction(instruction.get());
                    if ((instruction->GetFlags() & InstructionFlags::throwInst) != InstructionFlags::none)
                    {
                        if (firstThrowInst || instruction->CppLineIndex() == 0)
                        {
                            debugInfo->AddThrowInstruction(instruction.get());
                            firstThrowInst = false;
                        }
                    }
                    else
                    {
                        firstThrowInst = true;
                    }
                    if ((instruction->GetFlags() & InstructionFlags::catchInst) != InstructionFlags::none)
                    {
                        if (firstCatchInst)
                        {
                            debugInfo->AddCatchInstruction(instruction.get());
                            firstCatchInst = false;
                        }
                    }
                    else
                    {
                        firstCatchInst = true;
                    }
                    compileUnitFunction->AddInstruction(instruction.release());
                    if (cppLineIndex == 0)
                    {
                        InstructionLocation location;
                        location.span = span;
                        location.projectIndex = projectIndex;
                        location.compileUnitIndex = compileUnitIndex;
                        location.cppLineNumber = cppLineNumber;
                        SourceFileKey sourceFileKey(fileIndex, sourceModuleId);
                        debugInfo->GetSourceFileMap().AddInstructionLocation(sourceFileKey, location);
                    }
                }
                int32_t numScopes;
                ReadNumberOfScopes(reader, numScopes);
                for (int32_t i = 0; i < numScopes; ++i)
                {
                    int16_t scopeId;
                    int16_t parentScopeId;
                    int32_t numLocalVariables;
                    ReadScopeRecord(reader, scopeId, parentScopeId, numLocalVariables);
                    std::unique_ptr<FunctionScope> scope(new FunctionScope(compileUnitFunction.get(), scopeId, parentScopeId));
                    for (int32_t i = 0; i < numLocalVariables; ++i)
                    {
                        DIVariable* localVariable = new DIVariable(DIVariable::Kind::localVariable);
                        localVariable->Read(reader);
                        scope->AddLocalVariable(localVariable);
                        localVariable->SetProject(project.get());
                        if (!localVariable->Name().starts_with("@"))
                        {
                            compileUnitFunction->AddLocalVariable(localVariable);
                        }
                    }
                    std::sort(compileUnitFunction->LocalVariables().begin(), compileUnitFunction->LocalVariables().end(), VariableNameLess());
                    compileUnitFunction->AddScope(scope.release());
                }
                int32_t controlFlowGraphNodeCount;
                ReadControlFlowGraphNodeCount(reader, controlFlowGraphNodeCount);
                for (int32_t i = 0; i < controlFlowGraphNodeCount; ++i)
                {
                    int32_t nodeId;
                    SourceSpan span;
                    int32_t cppLineIndex;
                    int32_t cppLineNumber;
                    ReadControlFlowGraphNode(reader, nodeId, span, cppLineIndex, cppLineNumber);
                    std::unique_ptr<ControlFlowGraphNode> node(new ControlFlowGraphNode(nodeId, span, cppLineIndex, cppLineNumber));
                    int32_t edgeCount;
                    ReadControlFlowGraphNodeEdgeCount(reader, edgeCount);
                    for (int32_t i = 0; i < edgeCount; ++i)
                    {
                        int32_t endNodeId;
                        ReadControlFlowGraphNodeEdge(reader, endNodeId);
                        node->AddNext(endNodeId);
                    }
                    compileUnitFunction->GetControlFlowGraph().AddNode(node.release());
                }
                compileUnit->AddCompileUnitFunction(compileUnitFunction.release());
            }
            project->AddCompileUnit(compileUnit.release());
        }
        int32_t numFileIndexRecords;
        ReadNumberOfFileIndexRecords(reader, numFileIndexRecords);
        for (int32_t i = 0; i < numFileIndexRecords; ++i)
        {
            int32_t fileIndex;
            std::string sourceFilePath;
            ReadFileIndexRecord(reader, fileIndex, sourceFilePath);
            std::unique_ptr<SourceFileReference> sourceFile(new SourceFileReference(fileIndex, sourceFilePath));
            project->AddSourceFileReference(sourceFile.release());
            SourceFileKey sourceFileKey(fileIndex, project->ModuleId());
            debugInfo->GetSourceFileMap().AddSourceFile(sourceFileKey, sourceFilePath);
        }
        int32_t numFunctionIndexFunctionRecords;
        ReadNumberOfFunctionIndexFunctionRecords(reader, numFunctionIndexFunctionRecords);
        for (int32_t i = 0; i < numFunctionIndexFunctionRecords; ++i)
        {
            util::uuid functionId;
            std::string fullFunctionName;
            std::string mangledFunctionName;
            ReadFunctionIndexFunctionRecord(reader, functionId, fullFunctionName, mangledFunctionName);
            std::unique_ptr<Function> function(new Function(functionId, fullFunctionName, mangledFunctionName));
            project->AddFunction(function.release());
        }
        int32_t numTypeIndexRecords = reader.ReadInt();
        for (int32_t i = 0; i < numTypeIndexRecords; ++i)
        {
            std::unique_ptr<DIType> type = ReadType(reader, project.get());
            if (type->GetKind() == DIType::Kind::classType || type->GetKind() == DIType::Kind::specializationType)
            {
                DIClassType* classType = static_cast<DIClassType*>(type.get());
                if (classType->IsPolymorphic())
                {
                    debugInfo->AddPolymorphicType(classType);
                }
            }
            project->AddType(type.release());
        }
        debugInfo->AddProject(project.release());
    }
    debugInfo->ProcessSourceFileKeyLocationsMap();
    return debugInfo;
}

} // namespace cmajor::debug
