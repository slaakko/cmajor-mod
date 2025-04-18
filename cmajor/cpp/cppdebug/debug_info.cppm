// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debug.info;

import cmajor.debug.di.variable;
import cmajor.debug.di.type;
import soul.lexer;
import soul.ast.span;
import util;
import std.core;

export namespace cmajor::debug {

export namespace debug::info {}

class CompileUnitFunction;
class Scope;
class FunctionScope;
class SourceFileReference;
class DebuggerOutputWriter;

struct SourceSpan
{
    SourceSpan() : line(0), scol(0), ecol(0) {}
    SourceSpan(int32_t line_, int16_t scol_, int16_t ecol_) : line(line_), scol(scol_), ecol(ecol_) {}
    bool IsEmpty() const { return line == 0 && scol == 0 && ecol == 0; }
    std::string ToString() const;
    int32_t line;
    int16_t scol;
    int16_t ecol;
};

inline bool operator==(const SourceSpan& left, const SourceSpan& right)
{
    return left.line == right.line && left.scol == right.scol && left.ecol == right.ecol;
}

SourceSpan MakeSourceSpan(soul::lexer::FileMap* fileMap, const soul::ast::Span& span, int32_t fileIdex);

struct Frame
{
    Frame();
    std::string ToString(bool printLevel) const;
    std::unique_ptr<util::JsonValue> ToJson(bool includeLevel) const;
    bool IsEmpty() const { return func.empty() && file.empty() && line == 0; }
    int level;
    std::string addr;
    std::string func;
    std::string file;
    int line;
    int scol;
    int ecol;
};

struct InstructionLocation
{
    InstructionLocation();
    SourceSpan span;
    int32_t projectIndex;
    int32_t compileUnitIndex;
    int32_t cppLineNumber;
};

enum class InstructionFlags : int16_t
{
    none = 0,
    entryCode = 1 << 0,
    exitCode = 1 << 1,
    beginBrace = 1 << 2,
    endBrace = 1 << 3,
    startFunction = 1 << 4,
    throwInst = 1 << 5,
    catchInst = 1 << 6
};

inline constexpr InstructionFlags operator|(InstructionFlags left, InstructionFlags right)
{
    return InstructionFlags(int16_t(left) | int16_t(right));
}

inline constexpr InstructionFlags operator&(InstructionFlags left, InstructionFlags right)
{
    return InstructionFlags(int16_t(left) & int16_t(right));
}

inline constexpr InstructionFlags operator~(InstructionFlags operand)
{
    return InstructionFlags(~int16_t(operand));
}

std::string InstructionFlagsStr(InstructionFlags flags);

class Instruction
{
public:
    Instruction(CompileUnitFunction* compileUnitFunction_, int32_t cppLineNumber_, const SourceSpan& span_, int32_t cppLineIndex_, int16_t scopeId_, InstructionFlags flags_);
    CompileUnitFunction* GetCompileUnitFunction() const { return compileUnitFunction; }
    SourceFileReference* GetSourceFileReference() const;
    Scope* GetScope() const;
    FunctionScope* GetFunctionScope() const;
    int32_t CppLineNumber() const { return cppLineNumber; }
    const SourceSpan& GetSourceSpan() const { return span; }
    int32_t CppLineIndex() const { return cppLineIndex; }
    int16_t ScopeId() const { return scopeId; }
    InstructionFlags GetFlags() const { return flags; }
    std::string GetExplicitCppLocationArgs() const;
    std::string GetFileLineCppLocationArgs() const;
    Frame GetCmajorFrame() const;
    void PrintSource(util::CodeFormatter& formatter);
    Instruction* Next() const { return next; }
    void SetNext(Instruction* next_) { next = next_; }
    bool IsStopInstruction() const;
    bool AtEndBrace() const;
private:
    CompileUnitFunction* compileUnitFunction;
    int32_t cppLineNumber;
    SourceSpan span;
    int32_t cppLineIndex;
    int16_t scopeId;
    InstructionFlags flags;
    Instruction* next;
};

void AddToNextSet(std::set<Instruction*>& nextSet, Instruction* inst);

class ControlFlowGraphNode
{
public:
    ControlFlowGraphNode(int32_t nodeId_, const SourceSpan& span_, int32_t cppLineIndex_, int32_t cppLineNumber_);
    int32_t NodeId() const { return nodeId; }
    const SourceSpan& GetSourceSpan() const { return span; }
    int32_t CppLineIndex() const { return cppLineIndex; }
    int32_t CppLineNumber() const { return cppLineNumber; }
    const std::set<int32_t>& Next() const { return nextSet; }
    void AddNext(int32_t next);
    void SetInst(Instruction* inst_) { inst = inst_; }
    Instruction* Inst() const { return inst; }
private:
    int32_t nodeId;
    SourceSpan span;
    int32_t cppLineIndex;
    int32_t cppLineNumber;
    std::set<int32_t> nextSet;
    Instruction* inst;
};

class ControlFlowGraph
{
public:
    ControlFlowGraph();
    ControlFlowGraph(const ControlFlowGraph&) = delete;
    ControlFlowGraph(ControlFlowGraph&&) = delete;
    ControlFlowGraph& operator=(const ControlFlowGraph&) = delete;
    ControlFlowGraph& operator=(ControlFlowGraph&&) = delete;
    void AddNode(ControlFlowGraphNode* node);
    ControlFlowGraphNode* GetNodeById(int32_t nodeId) const;
    ControlFlowGraphNode* GetNodeByCppLineNumber(int32_t cppLineNumber) const;
private:
    std::unordered_map<int32_t, ControlFlowGraphNode*> nodeIdMap;
    std::unordered_map<int32_t, ControlFlowGraphNode*> cppLineNodeMap;
    std::vector<std::unique_ptr<ControlFlowGraphNode>> nodes;
};

class FunctionScope : public Scope
{
public:
    FunctionScope(CompileUnitFunction* compileUnitFunction_, int16_t id_, int16_t parentScopeId_);
    FunctionScope(const FunctionScope&) = delete;
    FunctionScope(FunctionScope&&) = delete;
    FunctionScope& operator=(const FunctionScope&) = delete;
    FunctionScope& operator=(FunctionScope&&) = delete;
    std::string Name() const override;
    int16_t Id() const { return id; }
    int16_t ParentScopeId() const { return parentScopeId; }
    CompileUnitFunction* GetCompileUnitFunction() const { return compileUnitFunction; }
    Scope* GetParentScope() const;
    void AddLocalVariable(DIVariable* localVariable);
    DIVariable* GetVariable(const std::string& name) const override;
    const std::vector<std::unique_ptr<DIVariable>>& LocalVariables() const { return localVariables; }
    int LocalVariableCount() const override { return localVariables.size(); }
    bool IsFunctionScope() const { return true; }
private:
    CompileUnitFunction* compileUnitFunction;
    int16_t id;
    int16_t parentScopeId;
    std::vector<std::unique_ptr<DIVariable>> localVariables;
    std::unordered_map<std::string, DIVariable*> localVariableMap;
};

class CompileUnit;
class SourceFile;
class Function;
class SourceFileReference;

struct SourceFileKey
{
    SourceFileKey(int32_t fileIndex_, const util::uuid& sourceModuleId_) : fileIndex(fileIndex_), sourceModuleId(sourceModuleId_) {}
    int32_t fileIndex;
    util::uuid sourceModuleId;
};

bool operator==(const SourceFileKey& left, const SourceFileKey& right);
bool operator<(const SourceFileKey& left, const SourceFileKey& right);

class CompileUnitFunction
{
public:
    CompileUnitFunction(CompileUnit* compileUnit_, int32_t fileIndex_, const util::uuid& sourceModuleId_, const util::uuid& functionId_);
    CompileUnitFunction(const CompileUnitFunction&) = delete;
    CompileUnitFunction(CompileUnitFunction&&) = delete;
    CompileUnitFunction& operator=(const CompileUnitFunction&) = delete;
    CompileUnitFunction& operator=(CompileUnitFunction&&) = delete;
    CompileUnit* GetCompileUnit() const { return compileUnit; }
    SourceFileReference* GetSourceFileReference() const;
    SourceFile& GetSourceFile() const;
    Function* GetFunction() const;
    int32_t FileIndex() const { return fileIndex; }
    const util::uuid& FunctionId() const { return functionId; }
    void AddInstruction(Instruction* instruction);
    Instruction* GetInstruction(int index) const;
    void AddScope(FunctionScope* scope);
    const std::vector<std::unique_ptr<Instruction>>& Instructions() const { return instructions; }
    const std::vector<std::unique_ptr<FunctionScope>>& Scopes() const { return scopes; }
    ControlFlowGraph& GetControlFlowGraph() { return controlFlowGraph; }
    const std::vector<DIVariable*>& LocalVariables() const { return localVariables; }
    std::vector<DIVariable*>& LocalVariables() { return localVariables; }
    void AddLocalVariable(DIVariable* localVariable);
private:
    CompileUnit* compileUnit;
    int32_t fileIndex;
    util::uuid sourceModuleId;
    util::uuid functionId;
    std::vector<std::unique_ptr<Instruction>> instructions;
    std::vector<std::unique_ptr<FunctionScope>> scopes;
    ControlFlowGraph controlFlowGraph;
    std::vector<DIVariable*> localVariables;
};

class Project;

class CompileUnit
{
public:
    CompileUnit(Project* project_, const std::string& baseName_);
    CompileUnit(const CompileUnit&) = delete;
    CompileUnit(CompileUnit&&) = delete;
    CompileUnit& operator=(const CompileUnit&) = delete;
    CompileUnit& operator=(CompileUnit&&) = delete;
    Project* GetProject() const { return project; }
    const std::string& BaseName() const { return baseName; }
    void AddCompileUnitFunction(CompileUnitFunction* compileUnitFunction);
    const std::vector<std::unique_ptr<CompileUnitFunction>>& CompileUnitFunctions() const { return compileUnitFunctions; }
    void AddInstruction(Instruction* instruction);
    Instruction* GetInstruction(int32_t cppLineNumber) const;
    Instruction* GetNearestInstruction(int32_t cppLineNumber) const;
private:
    Project* project;
    std::string baseName;
    std::vector<std::unique_ptr<CompileUnitFunction>> compileUnitFunctions;
    std::map<int32_t, Instruction*> instructionMap;
};

class SourceFileReference
{
public:
    SourceFileReference(int32_t fileIndex_, const std::string& filePath_);
    int32_t FileIndex() const { return fileIndex; }
    const std::string& FilePath() const { return filePath; }
private:
    int32_t fileIndex;
    std::string filePath;
};

class Function
{
public:
    Function(const util::uuid& id_, const std::string& fullName_, const std::string& mangledName_);
    const util::uuid& Id() const { return id; }
    const std::string& FullName() const { return fullName; }
    const std::string& MangledName() const { return mangledName; }
private:
    util::uuid id;
    std::string fullName;
    std::string mangledName;
};

class DebugInfo;

class Project
{
public:
    Project(DebugInfo* debugInfo_, const std::string& name_, const std::string& directoryPath_, const std::string& cmajorRootPrefix_, const util::uuid& moduleId_);
    Project(const Project&) = delete;
    Project(Project&&) = delete;
    Project& operator=(const Project&) = delete;
    Project& operator=(Project&&) = delete;
    const std::string& Name() const { return name; }
    const std::string& DirectoryPath() const { return directoryPath; }
    std::string InternalDirectoryPath() const;
    const std::string& CmajorRootPrefix() const { return cmajorRootPrefix; }
    void AddCompileUnit(CompileUnit* compileUnit);
    CompileUnit* GetCompileUnit(const std::string& baseName) const;
    CompileUnit* GetCompileUnit(int32_t compileUnitIndex) const;
    void AddSourceFileReference(SourceFileReference* sourceFileReference);
    SourceFileReference* GetSourceFileReference(int32_t fileIndex) const;
    SourceFile& GetSourceFile(int32_t fileIndex) const;
    void AddFunction(Function* function);
    Function* GetFunction(const util::uuid& functionId) const;
    void SetMainFunction(CompileUnitFunction* mainFunction_) { mainFunction = mainFunction_; }
    CompileUnitFunction* GetMainFunction() const;
    void AddType(DIType* type);
    DIType* GetType(const util::uuid& typeId) const;
    DIPointerType* GetPointerType(DIType* pointedToType);
    DIReferenceType* GetReferenceType(DIType* referredToType);
    DIConstType* GetConstType(DIType* baseType);
    DIType* GetLongType() const { return longType; }
    DIType* GetBoolType() const { return boolType; }
    DebugInfo* GetDebugInfo() const { return debugInfo; }
    const util::uuid& ModuleId() const { return moduleId; }
private:
    DebugInfo* debugInfo;
    std::string name;
    std::string directoryPath;
    std::string cmajorRootPrefix;
    util::uuid moduleId;
    std::vector<std::unique_ptr<CompileUnit>> compileUnits;
    std::unordered_map<std::string, CompileUnit*> compileUnitMap;
    std::vector<std::unique_ptr<SourceFileReference>> sourceFileReferences;
    std::unordered_map<int32_t, SourceFileReference*> sourceFileReferenceMap;
    std::vector<std::unique_ptr<Function>> functions;
    std::unordered_map<util::uuid, Function*, util::UuidHash> functionMap;
    std::vector<std::unique_ptr<DIType>> types;
    std::unordered_map<util::uuid, DIType*, util::UuidHash> typeMap;
    std::unordered_map<util::uuid, DIPointerType*, util::UuidHash> pointerTypeMap;
    std::unordered_map<util::uuid, DIReferenceType*, util::UuidHash> referenceTypeMap;
    std::unordered_map<util::uuid, DIConstType*, util::UuidHash> constTypeMap;
    CompileUnitFunction* mainFunction;
    DIType* longType;
    DIType* boolType;
};

class SourceFile
{
public:
    SourceFile(DebugInfo* debugInfo_, const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
    const std::vector<std::string>& Lines() const { return lines; }
    void Print(util::CodeFormatter& formatter, int lineNumber, Instruction* currentInstruction, bool window);
private:
    DebugInfo* debugInfo;
    std::string filePath;
    std::vector<std::string> lines;
};

class SourceFileCache
{
public:
    SourceFileCache(DebugInfo* debugInfo_);
    SourceFileCache(const SourceFileCache&) = delete;
    SourceFileCache(SourceFileCache&&) = delete;
    SourceFileCache& operator=(const SourceFileCache&) = delete;
    SourceFileCache& operator=(SourceFileCache&&) = delete;
    SourceFile& GetSourceFile(const std::string& filePath);
private:
    DebugInfo* debugInfo;
    std::vector<std::unique_ptr<SourceFile>> sourceFiles;
    std::unordered_map<std::string, SourceFile*> sourceFileMap;
};

class SourceLineMap
{
public:
    SourceLineMap(DebugInfo* debugInfo_);
    std::vector<Instruction*> GetInstructions(int32_t sourceLineNumber) const;
    void AddInstructionLocation(const InstructionLocation& location);
private:
    DebugInfo* debugInfo;
    std::map<int32_t, std::vector<InstructionLocation>> sourceLineLocationsMap;
};

struct SourceLocation
{
    SourceLocation();
    SourceLocation(const std::string& path_, int line_);
    std::string ToString() const;
    bool IsEmpty() const { return path.empty() && line == 0; }
    std::string path;
    int line;
};

class SourceFileMap
{
public:
    SourceFileMap(DebugInfo* debugInfo_);
    SourceFileMap(const SourceFileMap&) = delete;
    SourceFileMap(SourceFileMap&&) = delete;
    SourceFileMap& operator=(const SourceFileMap&) = delete;
    SourceFileMap& operator=(SourceFileMap&&) = delete;
    std::vector<Instruction*> GetInstructions(const SourceLocation& location) const;
    std::string GetSourceFilePath(const SourceLocation& location) const;
    void AddInstructionLocation(const SourceFileKey& sourceFileKey, const InstructionLocation& location);
    void AddSourceFile(const SourceFileKey& sourceFileKey, const std::string& sourceFilePath);
    void ProcessSourceFileKeyLocationsMap();
private:
    DebugInfo* debugInfo;
    std::map<std::string, SourceLineMap*> sourceFileMap;
    std::vector<std::unique_ptr<SourceLineMap>> sourceLineMaps;
    std::map<SourceFileKey, std::string> sourceFileKeyMap;
    std::map<SourceFileKey, std::vector<InstructionLocation>> sourceFileKeyLocationsMap;
    void AddInstructionLocations(const SourceFileKey& sourceFileKey, const std::string& sourceFilePath);
};

std::string GetCurrentCmajorRootPrefix();

class DebugInfo
{
public:
    DebugInfo(const std::string& filePath_);
    DebugInfo(const DebugInfo&) = delete;
    DebugInfo(DebugInfo&&) = delete;
    DebugInfo& operator=(const DebugInfo&) = delete;
    DebugInfo& operator=(DebugInfo&&) = delete;
    void SetMainProject(Project* mainProject_);
    Project* GetMainProject() const;
    void AddProject(Project* project);
    Project* GetProjectByInternalPath(const std::string& internalPath) const;
    Project* GetProjectByPath(const std::string& directoryPath) const;
    Project* GetProjectByName(const std::string& projectName) const;
    Project* GetProjectById(const util::uuid& projectId) const;
    SourceFileCache& GetSourceFileCache() { return sourceFileCache; }
    SourceFileMap& GetSourceFileMap() { return sourceFileMap; }
    int GetSourceFileWindowSize() const { return sourceFileWindowSize; }
    void SetSourceFileWindowSize(int sourceFileWindowSize_) { sourceFileWindowSize = sourceFileWindowSize_; }
    Instruction* GetEntryInstruction(CompileUnitFunction* compileUnitFunction) const;
    Instruction* GetMainFunctionEntryInstruction() const;
    Instruction* GetInstruction(const Frame& frame, DebuggerOutputWriter& outputWriter) const;
    Instruction* GetNearestInstruction(const Frame& frame) const;
    Instruction* GetInstruction(const InstructionLocation& location) const;
    DIType* GetPolymorphicType(const std::string& vmtVarName) const;
    void AddPolymorphicType(DIClassType* polymorphicType);
    const std::string& FilePath() const { return filePath; }
    DIType* GetType(const std::string& typeId) const;
    void AddType(DIType* type);
    void AddThrowInstruction(Instruction* throwInstruction);
    const std::vector<Instruction*>& GetThrowInstructions() const { return throwInstructions; }
    void AddCatchInstruction(Instruction* catchInstruction);
    const std::vector<Instruction*>& GetCatchInstructions() const { return catchInstructions; }
    void ProcessSourceFileKeyLocationsMap();
private:
    std::string filePath;
    std::vector<std::unique_ptr<Project>> projects;
    std::unordered_map<std::string, Project*> projectPathMap;
    std::unordered_map<std::string, Project*> projectNameMap;
    std::unordered_map<util::uuid, Project*, util::UuidHash> projectIdMap;
    std::unordered_map<std::string, DIType*> polymorphicTypeMap;
    std::unordered_map<std::string, DIType*> typeMap;
    Project* mainProject;
    SourceFileCache sourceFileCache;
    SourceFileMap sourceFileMap;
    int sourceFileWindowSize;
    std::vector<Instruction*> throwInstructions;
    std::vector<Instruction*> catchInstructions;
};

std::unique_ptr<DebugInfo> ReadDebugInfo(const std::string& cmdbFilePath);

} // namespace cmajor::debug
