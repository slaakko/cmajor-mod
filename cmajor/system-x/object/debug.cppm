// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.debug;

import soul.ast.span;
import std.core;

export namespace cmajor::systemx::object {

class DebugSection;

enum class DebugRecordKind : uint8_t
{
    fileInfo = 0, funcInfo = 1, localInfo = 2, startFunc = 3, endFunc = 4, lineInfo = 5, beginTry = 6, endTry = 7, catch_ = 8, beginCleanup = 9, endCleanup = 10, 
    structTypeInfo = 11, fieldInfo = 12, arrayTypeInfo = 13, functionTypeInfo = 14
};

std::string DebugRecordKindStr(DebugRecordKind kind);

class DebugRecord
{
public:
    DebugRecord(DebugRecordKind kind_);
    virtual ~DebugRecord();
    virtual void Emit(DebugSection* debugSection);
    virtual void Read(DebugSection* debugSection);
    virtual std::string ToString() const = 0;
    DebugRecordKind Kind() const { return kind; }
    bool IsTypeInfoRecord() const { return IsStructureTypeInfoRecord() || IsArrayTypeInfoRecord() || IsFunctionTypeInfoRecord(); }
    bool IsStructureTypeInfoRecord() const { return kind == DebugRecordKind::structTypeInfo; }
    bool IsArrayTypeInfoRecord() const { return kind == DebugRecordKind::arrayTypeInfo; }
    bool IsFunctionTypeInfoRecord() const { return kind == DebugRecordKind::functionTypeInfo; }
private:
    DebugRecordKind kind;
};

class TypeInfoRecord : public DebugRecord
{
public:
    TypeInfoRecord(DebugRecordKind kind_);
    TypeInfoRecord(DebugRecordKind kind_, const std::string& fullName_, int32_t typeId_);
    const std::string& FullName() const { return fullName; }
    int32_t TypeId() const { return typeId; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
private:
    std::string fullName;
    int32_t typeId;
};

class FileInfoRecord : public DebugRecord
{
public:
    FileInfoRecord();
    FileInfoRecord(const std::string& sourceFileName_, uint32_t sourceFileNameId_);
    const std::string& SourceFileName() const { return sourceFileName; }
    uint32_t Id() const { return sourceFileNameId; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    std::string sourceFileName;
    uint32_t sourceFileNameId;
};

class LocalInfoRecord : public DebugRecord
{
public:
    LocalInfoRecord();
    LocalInfoRecord(const std::string& name_, int32_t typeId_, int32_t offset_);
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
    const std::string& Name() const { return name; }
    int32_t TypeId() const { return typeId; }
    int32_t Offset() const { return offset; }
private:
    std::string name;
    int32_t typeId;
    int32_t offset;
};

class FuncInfoRecord : public DebugRecord
{
public:
    FuncInfoRecord();
    FuncInfoRecord(uint32_t functionSymbolIndex_, const std::string& fullName_, uint32_t sourceFileNameId, int64_t frameSize_, bool main_);
    uint32_t FunctionSymbolIndex() const { return functionSymbolIndex; }
    const std::string& FullName() const { return fullName; }
    uint32_t SourceFileNameId() const { return sourceFileNameId; }
    int64_t FrameSize() const { return frameSize; }
    bool IsMain() const { return main; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
    void AddToCfg(int32_t prev, int32_t next);
    const std::vector<std::pair<int32_t, int32_t>>& Cfg() const { return cfg; }
    void AddLocalInfoRecord(LocalInfoRecord&& localInfoRecord);
    const std::vector<LocalInfoRecord>& LocalInfoRecords() const { return localInfoRecords; }
private:
    uint32_t functionSymbolIndex;
    std::string fullName;
    uint32_t sourceFileNameId;
    int64_t frameSize;
    bool main;
    std::vector<std::pair<int32_t, int32_t>> cfg;
    std::vector<LocalInfoRecord> localInfoRecords;
};

class StartFuncRecord : public DebugRecord
{
public:
    StartFuncRecord();
    StartFuncRecord(uint32_t functionSymbolIndex_);
    uint32_t FunctionSymbolIndex() const { return functionSymbolIndex; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t functionSymbolIndex;
};

class EndFuncRecord : public DebugRecord
{
public:
    EndFuncRecord();
    EndFuncRecord(uint32_t functionSymbolIndex_);
    uint32_t FunctionSymbolIndex() const { return functionSymbolIndex; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t functionSymbolIndex;
};

class LineInfoRecord : public DebugRecord
{
public:
    LineInfoRecord();
    LineInfoRecord(uint32_t offset_, const soul::ast::LineColLen& lineColLen_, int32_t index_);
    uint32_t Offset() const { return offset; }
    int32_t Index() const { return index; }
    const soul::ast::LineColLen& LineColLen() const { return lineColLen; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t offset;
    soul::ast::LineColLen lineColLen;
    int32_t index;
};

class BeginTryRecord : public DebugRecord
{
public:
    BeginTryRecord();
    BeginTryRecord(uint32_t tryBlockId_, uint32_t parentTryBlockId_, uint32_t offset_);
    uint32_t TryBlockId() const { return tryBlockId; }
    uint32_t ParentTryBlockId() const { return parentTryBlockId; }
    uint32_t Offset() const { return offset; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t tryBlockId;
    uint32_t parentTryBlockId;
    uint32_t offset;
};

class EndTryRecord : public DebugRecord
{
public:
    EndTryRecord();
    EndTryRecord(uint32_t tryBlockId_, uint32_t offset_);
    uint32_t TryBlockId() const { return tryBlockId; }
    uint32_t Offset() const { return offset; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t tryBlockId;
    uint32_t offset;
};

class CatchRecord : public DebugRecord
{
public:
    CatchRecord();
    CatchRecord(uint32_t catchBlockId_, uint32_t tryBlockId_, uint64_t caughtTypeId1_, uint64_t caughtTypeId2_);
    uint32_t CatchBlockId() const { return catchBlockId; }
    uint32_t TryBlockId() const { return tryBlockId; }
    uint64_t CaughtTypeId1() const { return caughtTypeId1; }
    uint64_t CaughtTypeId2() const { return caughtTypeId2; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t catchBlockId;
    uint32_t tryBlockId;
    uint64_t caughtTypeId1;
    uint64_t caughtTypeId2;
};

class BeginCleanupRecord : public DebugRecord
{
public:
    BeginCleanupRecord();
    BeginCleanupRecord(uint32_t cleanupBlockId_, uint32_t tryBlockId_, uint32_t offset_);
    uint32_t CleanupBlockId() const { return cleanupBlockId; }
    uint32_t TryBlockId() const { return tryBlockId; }
    uint32_t Offset() const { return offset; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t cleanupBlockId;
    uint32_t tryBlockId;
    uint32_t offset;
};

class EndCleanupRecord : public DebugRecord
{
public:
    EndCleanupRecord();
    EndCleanupRecord(uint32_t cleanupBlockId_, uint32_t offset_);
    uint32_t CleanupBlockId() const { return cleanupBlockId; }
    uint32_t Offset() const { return offset; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    uint32_t cleanupBlockId;
    uint32_t offset;
};

class FieldInfoRecord : public DebugRecord
{
public:
    FieldInfoRecord();
    FieldInfoRecord(const std::string& name_, int32_t typeId_, int32_t offset_);
    const std::string& Name() const { return name; }
    int32_t TypeId() const { return typeId; }
    int32_t Offset() const { return offset; }
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
private:
    std::string name;
    int32_t typeId;
    int32_t offset;
};

class StructTypeInfoRecord : public TypeInfoRecord
{
public:
    StructTypeInfoRecord();
    StructTypeInfoRecord(const std::string& fullName_, int32_t typeId_, int64_t size_);
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
    int64_t Size() const { return size; }
    void AddFieldInfoRecord(FieldInfoRecord&& fieldInfoRecord);
    const std::vector<FieldInfoRecord>& FieldInfoRecords() const { return fieldInfoRecords; }
private:
    int64_t size;
    std::vector<FieldInfoRecord> fieldInfoRecords;
};

class ArrayTypeInfoRecord : public TypeInfoRecord
{
public:
    ArrayTypeInfoRecord();
    ArrayTypeInfoRecord(const std::string& fullName_, int32_t typeId_, int32_t elementTypeId_, int64_t size_);
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
    int32_t ElementTypeId() const { return elementTypeId; }
    int64_t Size() const { return size; }
private:
    int32_t elementTypeId;
    int64_t size;
};

class FunctionTypeInfoRecord : public TypeInfoRecord
{
public:
    FunctionTypeInfoRecord();
    FunctionTypeInfoRecord(const std::string& fullName_, int32_t typeId_);
    void Emit(DebugSection* debugSection) override;
    void Read(DebugSection* debugSection) override;
    std::string ToString() const override;
};

DebugRecord* MakeDebugRecord(DebugRecordKind kind);
DebugRecord* ReadDebugRecord(DebugSection* debugSection);

} // namespace cmsx::object
