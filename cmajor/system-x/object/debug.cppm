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
    fileInfo = 0, funcInfo = 1, startFunc = 2, endFunc = 3, lineInfo = 4, beginTry = 5, endTry = 6, catch_ = 7, beginCleanup = 8, endCleanup = 9
};

std::string DebugRecordKindStr(DebugRecordKind kind);

class DebugRecord
{
public:
    DebugRecord(DebugRecordKind kind_);
    virtual void Emit(DebugSection* debugSection);
    virtual void Read(DebugSection* debugSection);
    virtual std::string ToString() const = 0;
    DebugRecordKind Kind() const { return kind; }
private:
    DebugRecordKind kind;
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
private:
    uint32_t functionSymbolIndex;
    std::string fullName;
    uint32_t sourceFileNameId;
    int64_t frameSize;
    bool main;
    std::vector<std::pair<int32_t, int32_t>> cfg;
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

DebugRecord* MakeDebugRecord(DebugRecordKind kind);
DebugRecord* ReadDebugRecord(DebugSection* debugSection);

} // namespace cmsx::object
