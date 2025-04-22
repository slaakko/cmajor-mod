// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.function.table;

import cmajor.systemx.machine.memory;
import std.core;

export namespace cmajor::systemx::object {

class BinaryFile;
class ExecutableFile;
class Section;
class SymbolTable;
class LinkTable;
class FunctionTableEntry;

struct FunctionTableIndexEntry
{
    FunctionTableIndexEntry();
    FunctionTableIndexEntry(uint64_t functionStart_, uint64_t functionLength_, uint64_t entryId_, uint64_t entryAddress_);
    uint64_t Size() const { return 4 * 8; }
    void Read(uint64_t rv, cmajor::systemx::machine::Memory& memory, uint64_t address);
    void Write(Section* section);
    uint64_t functionStart;
    uint64_t functionLength;
    uint64_t entryId;
    uint64_t entryAddress;
};

bool operator<(const FunctionTableIndexEntry& left, const FunctionTableIndexEntry& right);

class FunctionTableIndex
{
public:
    FunctionTableIndex();
    void AddEntry(const FunctionTableIndexEntry& entry);
    void Sort();
    void Read(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    void Write(BinaryFile& binaryFile);
    FunctionTableIndexEntry* SearchEntry(uint64_t pc);
    uint64_t GetEntryAddress(uint64_t entryId) const;
private:
    std::vector<FunctionTableIndexEntry> indexEntries;
    std::map<uint64_t, uint64_t> entryIdMap;
};

class StringTable
{
public:
    StringTable();
    void Write(BinaryFile& binaryFile);
    int32_t AddString(const std::string& str);
    std::string GetString(SymbolTable& symbolTable, int32_t stringId, uint64_t rv, cmajor::systemx::machine::Memory& memory);
private:
    void ReadIndex(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    bool indexRead;
    std::vector<uint64_t> stringAddresses;
    std::vector<std::string> strings;
    std::map<int32_t, std::string> stringMap;
    std::map<std::string, int32_t> stringIdMap;
};

struct LineNumberTableEntry
{
    LineNumberTableEntry();
    LineNumberTableEntry(uint32_t offset_, uint32_t lineNumber_);
    uint64_t Size() const { return 2 * 4; }
    void Write(Section* section);
    void Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    uint32_t offset;
    uint32_t lineNumber;
};

class LineNumberTable
{
public:
    LineNumberTable();
    void AddEntry(const LineNumberTableEntry& entry);
    void Write(Section* section);
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    const std::vector<LineNumberTableEntry>& Entries() const { return entries; }
    int32_t SearchLineNumber(uint32_t offset) const;
    uint32_t GetOffset(uint32_t lineNumber) const;
private:
    std::vector<LineNumberTableEntry> entries;
};

class SourceFileTable
{
public:
    SourceFileTable();
    void AddSourceFile(const std::string& sourceFile);
    void Make(StringTable& stringTable);
    void Write(Section* section);
    int Count(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    std::string GetSourceFile(int index, StringTable& stringTable, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    int32_t GetSourceFileIndex(int32_t sourceFileId) const;
    void Read(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
private:
    bool read;
    std::set<std::string> sourceFiles;
    std::vector<int32_t> sourceFileIds;
    std::map<int32_t, int32_t> sourceFileIdMap;
};

struct LineFunctionEntry
{
    LineFunctionEntry();
    LineFunctionEntry(uint32_t lineNumber_, uint32_t functionId_);
    void Write(Section* section);
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    uint32_t lineNumber;
    uint32_t functionId;
};

bool operator<(const LineFunctionEntry& left, const LineFunctionEntry& right);

class LineFunctionIndex
{
public:
    LineFunctionIndex();
    void SetSourceFileIndex(int32_t sourceFileIndex_);
    void AddEntry(const LineFunctionEntry& entry);
    void Sort();
    void Write(Section* section);
    uint32_t GetFunctionId(uint32_t lineNumber, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
private:
    int32_t sourceFileIndex;
    std::vector<LineFunctionEntry> entries;
    bool read;
    void Read(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
};

class SourceFileLineFunctionIndex
{
public:
    SourceFileLineFunctionIndex();
    void AddFunction(FunctionTableEntry& functionTableEntry, StringTable& stringTable, SourceFileTable& sourceFileTable);
    void Write(Section* section);
    uint32_t GetFunctionId(int32_t sourceFileIndex, uint32_t lineNumber, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
private:
    std::map<int32_t, LineFunctionIndex> indexMap;
};

enum class ExceptionTableRecordKind : int32_t
{
    tryRecord = 0, handlerRecord = 1, cleanupRecord = 2
};

class ExceptionTable;

class ExceptionTableRecord
{
public:
    ExceptionTableRecord(ExceptionTableRecordKind kind_);
    ExceptionTableRecordKind Kind() const { return kind; }
    virtual ~ExceptionTableRecord();
    virtual int64_t Size() const { return 4; }
    virtual void Write(Section* section);
    virtual void Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    virtual void Setup(ExceptionTable* exceptionTable);
    ExceptionTable* Table() const { return table; }
private:
    ExceptionTableRecordKind kind;
    ExceptionTable* table;
};

ExceptionTableRecord* MakeExceptionTableRecord(ExceptionTableRecordKind kind);
ExceptionTableRecord* ReadExceptionTableRecord(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);

class HandlerRecord;
class CleanupRecord;

class TryRecord : public ExceptionTableRecord
{
public:
    TryRecord();
    TryRecord(uint32_t id_, uint32_t offset_, uint32_t parentTryRecordId_);
    int64_t Size() const override { return ExceptionTableRecord::Size() + 4 * 4; }
    uint32_t Id() const { return id; }
    void Write(Section* section) override;
    void Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory) override;
    uint32_t Offset() const { return offset; }
    uint32_t Length() const { return length; }
    void SetLength(uint32_t length_) { length = length_; }
    uint32_t ParentTryRecordId() const { return parentTryRecordId; }
    void Setup(ExceptionTable* exceptionTable) override;
    TryRecord* ParentTryRecord() const { return parentTryRecord; }
    void AddHandlerRecord(HandlerRecord* handlerRecord);
    const std::vector<HandlerRecord*>& HandlerRecords() const { return handlerRecords; }
    void AddCleanupRecord(CleanupRecord* cleanupRecord);
    const std::vector<CleanupRecord*>& CleanupRecords() const { return cleanupRecords; }
    void AddChildRecord(TryRecord* childRecord);
    const std::vector<TryRecord*>& ChildRecords() const { return childRecords; }
    TryRecord* SearchRecord(uint32_t offset);
private:
    uint32_t id;
    uint32_t offset;
    uint32_t length;
    uint32_t parentTryRecordId;
    TryRecord* parentTryRecord;
    std::vector<HandlerRecord*> handlerRecords;
    std::vector<CleanupRecord*> cleanupRecords;
    std::vector<TryRecord*> childRecords;
};

class HandlerRecord : public ExceptionTableRecord
{
public:
    HandlerRecord();
    HandlerRecord(uint64_t caughtClassId_, uint64_t handlerAddress_, uint32_t tryRecordId_);
    int64_t Size() const override { return ExceptionTableRecord::Size() + 5 * 4; }
    void Write(Section* section) override;
    void Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory) override;
    void Setup(ExceptionTable* exceptionTable) override;
    TryRecord* GetTryRecord() const { return tryRecord; }
    uint64_t CaughtClassId() const { return caughtClassId; }
    uint64_t HandlerAddress() const { return handlerAddress; }
    uint32_t TryRecordId() const { return tryRecordId; }
private:
    uint64_t caughtClassId;
    uint64_t handlerAddress;
    uint32_t tryRecordId;
    TryRecord* tryRecord;
};

class CleanupRecord : public ExceptionTableRecord
{
public:
    CleanupRecord();
    CleanupRecord(uint32_t offset_, uint64_t cleanupAddress_, uint32_t tryRecordId_);
    int64_t Size() const override { return ExceptionTableRecord::Size() + 5 * 4; }
    void Write(Section* section) override;
    void Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory) override;
    void Setup(ExceptionTable* exceptionTable) override;
    TryRecord* GetTryRecord() const { return tryRecord; }
    uint32_t Offset() const { return offset; }
    uint32_t Length() const { return length; }
    void SetLength(uint32_t length_) { length = length_; }
    uint64_t CleanupAddress() const { return cleanupAddress; }
    uint32_t TryRecordId() const { return tryRecordId; }
private:
    uint32_t offset;
    uint32_t length;
    uint64_t cleanupAddress;
    uint32_t tryRecordId;
    TryRecord* tryRecord;
};

class FunctionTableEntry;

class ExceptionTable
{
public:
    ExceptionTable();
    ExceptionTable(const ExceptionTable&) = delete;
    ExceptionTable& operator=(const ExceptionTable&) = delete;
    void Write(Section* section);
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    void AddRecord(ExceptionTableRecord* record);
    TryRecord* GetTryRecord(uint32_t id) const;
    ExceptionTableRecord* SearchRecord(uint32_t offset) const;
    FunctionTableEntry* GetFunctionTableEntry() const { return functionTableEntry; }
    void SetFunctionTableEntry(FunctionTableEntry* functionTableEntry_) { functionTableEntry = functionTableEntry_; }
private:
    std::vector<std::unique_ptr<ExceptionTableRecord>> records;
    std::map<uint32_t, TryRecord*> tryRecordMap;
    FunctionTableEntry* functionTableEntry;
};

class FunctionTableEntry
{
public:
    FunctionTableEntry();
    int64_t Write(StringTable& stringTable, Section* section);
    void Read(StringTable& stringTable, SymbolTable& symbolTable, int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    int64_t FunctionStart() const { return functionStart; }
    void SetFunctionStart(int64_t functionStart_) { functionStart = functionStart_; }
    int64_t FunctionLength() const { return functionLength; }
    void SetFunctionLength(int64_t functionLength_) { functionLength = functionLength_; }
    int64_t FrameSize() const { return frameSize; }
    void SetFrameSize(int64_t frameSize_) { frameSize = frameSize_; }
    bool IsMain() const { return main; }
    void SetMain() { main = true; }
    int32_t Id() const { return id; }
    void SetId(int32_t id_) { id = id_; }
    const std::string& FullName() const { return fullName; }
    void SetFullName(const std::string& fullName_);
    const std::string& MangledName() const { return mangledName; }
    void SetMangledName(const std::string& mangledName_);
    const std::string& SourceFileName() const { return sourceFileName; }
    void SetSourceFileName(const std::string& sourceFileName_);
    LineNumberTable& GetLineNumberTable() { return lineNumberTable; }
    int32_t SourceFileNameId() const { return sourceFileNameId; }
    int32_t SearchLineNumber(uint64_t pc) const;
    int64_t SearchPC(uint32_t lineNumber) const;
    uint64_t GetEntryPoint() const;
    ExceptionTable& GetExceptionTable() { return exceptionTable; }
    ExceptionTableRecord* SearchExceptionTableRecord(uint64_t pc) const;
    void AddToCfg(int32_t prevLine, int32_t nextLine);
    std::vector<int32_t> Next(int32_t line) const;
private:
    int64_t functionStart;
    int64_t functionLength;
    int64_t frameSize;
    bool main;
    int32_t id;
    std::int32_t fullNameId;
    std::int32_t mangledNameId;
    std::int32_t sourceFileNameId;
    std::string fullName;
    std::string mangledName;
    std::string sourceFileName;
    LineNumberTable lineNumberTable;
    ExceptionTable exceptionTable;
    std::map<int32_t, std::vector<int32_t>> cfg;
};

class FunctionTable
{
public:
    FunctionTable();
    FunctionTable(const FunctionTable&) = delete;
    FunctionTable& operator=(const FunctionTable&) = delete;
    void AddEntry(FunctionTableEntry* entry);
    void AddEntry(FunctionTableEntry* entry, bool setId);
    FunctionTableEntry* GetEntry(uint64_t pc, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    FunctionTableEntry* GetEntry(int32_t sourceFileIndex, uint32_t lineNumber, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    void Write(BinaryFile& binaryFile);
    int SourceFileCount(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    std::string GetSourceFileName(int32_t fileIndex, SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    int32_t GetSourceFileIndex(int32_t sourceFileId) const;
    void ReadSourceFileTable(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
private:
    void ReadIndex(SymbolTable& symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    bool indexRead;
    FunctionTableIndex index;
    StringTable stringTable;
    SourceFileTable sourceFileTable;
    SourceFileLineFunctionIndex sourceFileLineFunctionIndex;
    std::map<int64_t, FunctionTableEntry*> entryMap;
    std::vector<std::unique_ptr<FunctionTableEntry>> entries;
};

void MakeFunctionTable(const std::vector<std::unique_ptr<BinaryFile>>& binaryFiles, ExecutableFile& executable, LinkTable& linkTable);

} // namespace cmajor::systemx::object
