// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.modules;

import cmajor.debug;
import cmajor.symbols.resource.table;
import cmajor.symbols.warning;
import cmajor.symbols.function.index;
import cmajor.symbols.type.index;
import cmajor.symbols.sources;
import cmajor.symbols.classes;
import cmajor.ast.project;
import soul.lexer.file.map;
import soul.lexer.base;
import soul.ast.span;
import util;
import std.core;

export namespace cmajor::symbols {

class SymbolWriter;
class SymbolReader;
class SymbolTable;
class Sources;

bool IsSystemModule(const std::u32string& moduleName);
cmajor::debug::ContainerClassTemplateKind GetContainerClassTemplateKind(const std::u32string& fullClassName);

extern const char* cmajorModuleTag;

struct ModuleTag
{
    ModuleTag();
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    uint8_t bytes[4];
};

const uint8_t moduleFormat_1 = uint8_t('1');
const uint8_t moduleFormat_2 = uint8_t('2');
const uint8_t moduleFormat_3 = uint8_t('3');
const uint8_t moduleFormat_4 = uint8_t('4');
const uint8_t moduleFormat_5 = uint8_t('5');
const uint8_t moduleFormat_6 = uint8_t('6');
const uint8_t moduleFormat_7 = uint8_t('7');
const uint8_t moduleFormat_8 = uint8_t('8');
const uint8_t moduleFormat_9 = uint8_t('9');
const uint8_t moduleFormat_10 = uint8_t('A');
const uint8_t moduleFormat_11 = uint8_t('B');
const uint8_t moduleFormat_12 = uint8_t('C');
const uint8_t moduleFormat_13 = uint8_t('D');
const uint8_t moduleFormat_14 = uint8_t('E');
const uint8_t moduleFormat_15 = uint8_t('F');
const uint8_t moduleFormat_16 = uint8_t('G');
const uint8_t moduleFormat_17 = uint8_t('H');
const uint8_t moduleFormat_18 = uint8_t('I');
const uint8_t moduleFormat_19 = uint8_t('J');
const uint8_t moduleFormat_20 = uint8_t('K');
const uint8_t currentModuleFormat = moduleFormat_20;

enum class ModuleFlags : uint8_t
{
    none = 0, system = 1 << 0, core = 1 << 1, root = 1 << 2, immutable = 1 << 3, compiling = 1 << 4, fileIndexFilePathMapBuilt = 1 << 5, readFromModuleFile = 1 << 6, programModule = 1 << 7
};

inline ModuleFlags operator|(ModuleFlags left, ModuleFlags right)
{
    return ModuleFlags(uint8_t(left) | uint8_t(right));
}

inline ModuleFlags operator&(ModuleFlags left, ModuleFlags right)
{
    return ModuleFlags(uint8_t(left) & uint8_t(right));
}

inline ModuleFlags operator~(ModuleFlags flags)
{
    return ModuleFlags(~uint8_t(flags));
}

std::string ModuleFlagStr(ModuleFlags flags);

class Module;
struct ParseResult;

class ModuleDependency
{
public:
    ModuleDependency(Module* module_);
    Module* GetModule() const { return module; }
    void AddReferencedModule(Module* referencedModule);
    const std::vector<Module*>& ReferencedModules() const { return referencedModules; }
    void Dump(util::CodeFormatter& formatter);
    void Check();
private:
    Module* module;
    std::vector<Module*> referencedModules;
};

class FileTable
{
public:
    int32_t RegisterFilePath(const std::string& filePath);
    std::string GetFilePath(int32_t fileIndex) const;
    int32_t NumFilePaths() const { return filePaths.size(); }
    bool IsEmpty() const { return filePaths.empty(); }
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    void Dump(util::CodeFormatter& formatter);
private:
    std::vector<std::string> filePaths;
};

class SourceFileCache
{
public:
    SourceFileCache();
    SourceFileCache(const SourceFileCache&) = delete;
    SourceFileCache(SourceFileCache&&) = delete;
    SourceFileCache& operator=(const SourceFileCache&) = delete;
    SourceFileCache& operator=(SourceFileCache&&) = delete;
    const std::u32string& GetFileContent(const std::string& filePath);
private:
    std::unordered_map<std::string, std::unique_ptr<std::u32string>> fileContentMap;
};

class SourceFileInfo
{
public:
    SourceFileInfo();
    SourceFileInfo(int64_t sourceFileId_, const std::string& sourceFilePath_);
    int64_t SourceFileId() const { return sourceFileId; }
    const std::string& SourceFilePath() const { return sourceFilePath; }
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
private:
    int64_t sourceFileId;
    std::string sourceFilePath;
};

class FunctionTraceInfo
{
public:
    FunctionTraceInfo();
    FunctionTraceInfo(int64_t functionId_, const std::string& functionFullName_, int64_t sourceFileId_);
    int64_t FunctionId() const { return functionId; }
    const std::string& FunctionFullName() const { return functionFullName; }
    int64_t SourceFileId() const { return sourceFileId; }
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
private:
    int64_t functionId;
    std::string functionFullName;
    int64_t sourceFileId;
};

class Module
{
public:
    Module();
    Module(Context* context_, const std::string& filePath);
    Module(Context* context_, const std::string& filePath, bool readRoot);
    Module(Context* context_, const std::u32string& name_, const std::string& filePath_, cmajor::ast::Target target);
    ~Module();
    uint8_t Format() const { return format; }
    ModuleFlags Flags() const { return flags; }
    const std::u32string& Name() const { return name; }
    const util::uuid& Id() const { return id; }
    const std::string& OriginalFilePath() const { return originalFilePath; }
    const std::string& FilePathReadFrom() const { return filePathReadFrom; }
    const std::string& LibraryFilePath() const { return libraryFilePath; }
    const std::vector<std::string>& ResourceFilePaths() const { return resourceFilePaths; }
    void AddResourceFilePath(const std::string& resourceFilePath_);
    const std::vector<Module*>& AllReferencedModules() const { return allRefModules; }
    void PrepareForCompilation(const std::vector<std::string>& references, cmajor::ast::Target target, const soul::ast::Span& rootSpan, int rootFileIndex, 
        cmajor::ast::CompileUnitNode* rootCompileUnit, Context* context);
    SymbolTable& GetSymbolTable() { return *symbolTable; }
    bool HasSymbolTable() const { return symbolTable != nullptr; }
    void CreateSymbolTable();
    int64_t SymbolTablePos() const { return symbolTablePos; }
    FileTable& GetFileTable() { return fileTable; }
    void RegisterFileTable(FileTable* fileTable, Module* module);
    void MakeFilePathFileIndexMap();
#ifdef _WIN32
    ResourceTable& GetResourceTable() { return resourceTable; }
    ResourceTable& GetGlobalResourceTable() { return globalResourceTable; }
#endif
    std::string GetFilePath(int32_t fileIndex) const;
    void Write(SymbolWriter& writer);
    void SetDirectoryPath(const std::string& directoryPath_);
    void SetObjectFileDirectoryPath(const std::string& objectFileDirectoryPath_);
    const std::string& DirectoryPath() const { return directoryPath; }
    const std::string& ObjectFileDirectoryPath() const { return objectFileDirectoryPath; }
    const std::vector<std::string>& LibraryFilePaths() const { return libraryFilePaths; }
    bool IsSystemModule() const { return GetFlag(ModuleFlags::system); }
    void SetSystemModule() { SetFlag(ModuleFlags::system); }
    bool IsRootModule() const { return GetFlag(ModuleFlags::root); }
    void SetRootModule() { SetFlag(ModuleFlags::root); }
    bool IsImmutable() const { return GetFlag(ModuleFlags::immutable); }
    void SetImmutable() { SetFlag(ModuleFlags::immutable); }
    bool IsCore() const { return GetFlag(ModuleFlags::core); }
    void SetCore() { SetFlag(ModuleFlags::core); }
    bool GetFlag(ModuleFlags flag) const { return (flags & flag) != ModuleFlags::none; }
    void SetFlag(ModuleFlags flag) { flags = flags | flag; }
    void ResetFlag(ModuleFlags flag) { flags = flags & ~flag; }
    void AddExportedFunction(const std::string& exportedFunction);
    void AddExportedData(const std::string& data);
    const std::vector<std::string>& ExportedFunctions() { return exportedFunctions; }
    const std::vector<std::string>& ExportedData() { return exportedData; }
    const std::vector<std::string>& AllExportedFunctions() const { return allExportedFunctions; }
    const std::vector<std::string>& AllExportedData() const { return allExportedData; }
    void Dump(Context* context);
    ModuleDependency& GetModuleDependency() { return moduleDependency; }
    void SetCurrentProjectName(const std::u32string& currentProjectName_);
    std::u32string GetCurrentProjectName();
    void SetCurrentToolName(const std::u32string& currentToolName_);
    std::u32string GetCurrentToolName();
    CompileWarningCollection& WarningCollection();
    void ClearDefines();
    void DefineSymbol(const std::u32string& symbol);
    bool IsSymbolDefined(const std::u32string& symbol);
    void SetLogStreamId(int logStreamId_) { logStreamId = logStreamId_; }
    int LogStreamId() const { return logStreamId; }
    Module* GetSystemCoreModule();
    void Check();
    std::vector<Module*>& AllRefModules() { return allRefModules; }
    std::vector<Module*>& ReferencedModules() { return referencedModules; }
    void AddReferencedModule(Module* referencedModule);
    const std::vector<std::string>& ReferenceFilePaths() const { return referenceFilePaths; }
    void ReadHeader(cmajor::ast::Target target, SymbolReader& reader, Context* context, std::unordered_set<std::string>& importSet, std::vector<Module*>& modules,
        std::unordered_map<std::string, ModuleDependency*>& moduleDependencyMap, std::unordered_map<std::string, Module*>& readMap, bool& first);
    int DebugLogIndent() const { return debugLogIndent; }
    void IncDebugLogIndent() { ++debugLogIndent; }
    void DecDebugLogIndent() { --debugLogIndent; }
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    std::recursive_mutex& GetLock() { return lock; }
    void StartBuild();
    void StopBuild();
    int64_t GetBuildStartTimeMs() const { return buildStartMs; }
    int GetBuildTimeMs();
    bool Preparing() const { return preparing; }
    void SetPreparing(bool preparing_) { preparing = preparing_; }
    void AddCompileUnitId(const std::string& compileUnitId);
    const std::set<std::string>& AllCompileUnitIds() const { return allCompileUnitIds; }
    FunctionIndex& GetFunctionIndex() { return functionIndex; }
    TypeIndex& GetTypeIndex() { return typeIndex; }
    void WriteProjectDebugInfoFile(const std::string& projectDebugInfoFilePath);
    void WriteCmdbFile(const std::string& cmdbFilePath);
    void WriteDebugInfo(util::BinaryStreamWriter& cmdbWriter, int32_t& numProjects, Module* rootModule);
    std::unordered_map<int16_t, std::string>* GetModuleNameTable() { return &moduleNameTable; }
    std::unordered_map<std::string, int16_t>* GetModuleIdMap() { return &moduleIdMap; }
    int32_t GetFileIndexForFilePath(const std::string& filePath) const;
    void UpdateSourceFileModuleMap();
    std::recursive_mutex& Lock() { return lock; }
    // void SetSources(Sources* sources_) { sources.reset(sources_); }
    // Sources* GetSources() const { return sources.get(); }
    ParseResult ParseSources();
    ParseResult ParseSource(const std::string& sourceFilePath, const std::u32string& sourceCode);
    std::string GetCCList(const std::string& sourceFilePath, const std::u32string& ccText, const std::u32string& cursorLine, const std::vector<int>& ruleContext);
    std::string GetParamHelpList(const std::string& sourceFilePath, int symbolIndex);
    soul::lexer::FileMap& FileMap() { return fileMap; }
    void MakeFileMapFromFileTable();
    std::string GetErrorLines(const soul::ast::Span& span, int fileIndex, soul::ast::LineColLen& lineColLen);
    soul::ast::LineColLen GetLineColLen(const soul::ast::Span& span, int fileIndex);
    int64_t ModuleId() const;
    int64_t MakeFunctionId(const std::string& fullFunctionName, const std::string& sourceFilePath);
    void WriteFunctionTraceData(util::BinaryStreamWriter& writer);
    void ReadFunctionTraceData(util::BinaryStreamReader& reader);
    void ImportTraceData(Module* module);
    void WriteTraceData(const std::string& traceDataFilePath);
    ClassTypeFlagMap& GetClassTypeFlagMap() { return classTypeFlagMap; }
    void ImportClassTypeFlagMap(ClassTypeFlagMap& that);
    void ReadClassTypeFlagMap(SymbolReader& reader);
    void WriteClassTypeFlagMap(SymbolWriter& writer);
    void AddResourceScriptFilePath(const std::string& resourceScriptFilePath);
    void ImportResourceScriptFilePaths(Module* module);
    const std::vector<std::string>& ResourceScriptFilePaths() const { return resourceScriptFilePaths; }
    const std::vector<std::string>& AllResourceScriptFilePaths() const { return allResourceScriptFilePaths; }
    Context* GetContext() { return context; }
private:
    Context* context;
    uint8_t format;
    ModuleFlags flags;
    std::u32string name;
    util::uuid id;
    cmajor::ast::BackEnd backend;
    cmajor::ast::Config config;
    std::string originalFilePath;
    std::string filePathReadFrom;
    std::string libraryFilePath;
    std::vector<std::string> resourceFilePaths;
    std::vector<std::string> referenceFilePaths;
    FileTable fileTable;
    std::set<std::string> compileUnitIds;
    std::set<std::string> allCompileUnitIds;
#ifdef _WIN32
    ResourceTable resourceTable;
    ResourceTable globalResourceTable;
    std::vector<std::string> resourceScriptFilePaths;
    std::vector<std::string> allResourceScriptFilePaths;
#endif
    std::vector<FileTable*> fileTables;
    std::unordered_map<std::string, int32_t> filePathFileIndexMap;
    std::unordered_map<int16_t, std::string> moduleNameTable;
    std::unordered_map<std::string, int16_t> moduleIdMap;
    std::vector<std::string> exportedFunctions;
    std::vector<std::string> exportedData;
    std::vector<std::string> allExportedFunctions;
    std::vector<std::string> allExportedData;
    ModuleDependency moduleDependency;
    std::vector<Module*> referencedModules;
    std::vector<Module*> allRefModules;
    uint32_t symbolTablePos;
    std::unique_ptr<SymbolTable> symbolTable;
    std::string directoryPath;
    std::string objectFileDirectoryPath;
    std::vector<std::string> libraryFilePaths;
    std::u32string currentProjectName;
    std::u32string currentToolName;
    CompileWarningCollection warnings;
    std::set<std::u32string> defines;
    int logStreamId;
    bool headerRead;
    int debugLogIndent;
    Module* systemCoreModule;
    int index;
    bool preparing;
    std::recursive_mutex lock;
    int64_t buildStartMs;
    int64_t buildStopMs;
    FunctionIndex functionIndex;
    TypeIndex typeIndex;
    SourceFileCache sourceFileCache;
    // std::unique_ptr<Sources> sources;
    soul::lexer::FileMap fileMap;
    soul::lexer::LexerBase<char32_t>* lexer;
    std::vector<std::unique_ptr<SourceFileInfo>> sourceFileInfoVec;
    std::vector<SourceFileInfo*> allSourceFileInfoVec;
    std::map<std::string, SourceFileInfo*> sourceFileInfoMap;
    std::vector<std::unique_ptr<FunctionTraceInfo>> traceInfoVec;
    std::map<std::string, FunctionTraceInfo*> traceInfoMap;
    std::vector<FunctionTraceInfo*> allTraceInfoVec;
    std::set<std::u32string> traceDataImported;
    ClassTypeFlagMap classTypeFlagMap;
    void CheckUpToDate();
};

std::string GetSourceFilePath(int32_t fileIndex, const util::uuid& moduleId);
int GetLineNumber(const soul::ast::FullSpan& fullSpan);
soul::ast::LineColLen GetLineColLen(const soul::ast::FullSpan& fullSpan);
//bool HasRootModuleForCurrentThread();
//Module* GetRootModuleForCurrentThread();
//void SetRootModuleForCurrentThread(Module* rootModule_);

} // namespace cmajor::symbols
