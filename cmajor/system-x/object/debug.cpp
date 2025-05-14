// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.debug;

import cmajor.systemx.object.binary.file;
import util;

namespace cmajor::systemx::object {

std::string DebugRecordKindStr(DebugRecordKind kind)
{
    switch (kind)
    {
        case DebugRecordKind::fileInfo: return "FILEINFO";
        case DebugRecordKind::funcInfo: return "FUNCINFO";
        case DebugRecordKind::localInfo: return "LOCALINFO";
        case DebugRecordKind::startFunc: return "STARTFUNC";
        case DebugRecordKind::endFunc: return "ENDFUNC";
        case DebugRecordKind::lineInfo: return "LINEINFO";
        case DebugRecordKind::beginTry: return "BEGINTRY";
        case DebugRecordKind::endTry: return "ENDTRY";
        case DebugRecordKind::catch_: return "CATCH";
        case DebugRecordKind::beginCleanup: return "BEGINCLEANUP";
        case DebugRecordKind::endCleanup: return "ENDCLEANUP";
        case DebugRecordKind::structTypeInfo: return "STRUCTTYPEINFO";
        case DebugRecordKind::fieldInfo: return "FIELDINFO";
        case DebugRecordKind::arrayTypeInfo: return "ARRAYTYPEINFO";
        case DebugRecordKind::functionTypeInfo: return "FUNCIONTYPEINFO";
    }
    return std::string();
}

DebugRecord::DebugRecord(DebugRecordKind kind_) : kind(kind_)
{
}

DebugRecord::~DebugRecord()
{
}

void DebugRecord::Emit(DebugSection* debugSection)
{
    debugSection->EmitByte(static_cast<uint8_t>(kind));
}

void DebugRecord::Read(DebugSection* debugSection)
{
}

TypeInfoRecord::TypeInfoRecord(DebugRecordKind kind_) : DebugRecord(kind_), fullName(), typeId(-1)
{
}

TypeInfoRecord::TypeInfoRecord(DebugRecordKind kind_, const std::string& fullName_, int32_t typeId_) : DebugRecord(kind_), fullName(fullName_), typeId(typeId_)
{
}

void TypeInfoRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitString(fullName);
    debugSection->EmitTetra(typeId);
}

void TypeInfoRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    fullName = debugSection->ReadString();
    typeId = debugSection->ReadTetra();
}

FileInfoRecord::FileInfoRecord() : DebugRecord(DebugRecordKind::fileInfo), sourceFileName(), sourceFileNameId()
{
}

FileInfoRecord::FileInfoRecord(const std::string& sourceFileName_, uint32_t sourceFileNameId_) :
    DebugRecord(DebugRecordKind::fileInfo), sourceFileName(sourceFileName_), sourceFileNameId(sourceFileNameId_)
{
}

void FileInfoRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitString(sourceFileName);
    debugSection->EmitTetra(sourceFileNameId);
}

void FileInfoRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    sourceFileName = debugSection->ReadString();
    sourceFileNameId = debugSection->ReadTetra();
}

std::string FileInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("sourceFileName=").append(sourceFileName).append(", sourceFileNameId=").append(std::to_string(sourceFileNameId)).append(")");
    return str;
}

LocalInfoRecord::LocalInfoRecord() : DebugRecord(DebugRecordKind::localInfo), name(), typeId(-1), offset(-1)
{
}

LocalInfoRecord::LocalInfoRecord(const std::string& name_, int32_t typeId_, int32_t offset_) : 
    DebugRecord(DebugRecordKind::localInfo), name(name_), typeId(typeId_), offset(offset_)
{
}

void LocalInfoRecord::Emit(DebugSection* debugSection)
{
    debugSection->EmitString(name);
    debugSection->EmitTetra(typeId);
    debugSection->EmitTetra(offset);
}

void LocalInfoRecord::Read(DebugSection* debugSection)
{
    name = debugSection->ReadString();
    typeId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string LocalInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(name=").append(name).append(", typeId=#").append(util::ToHexString(static_cast<uint32_t>(typeId))).append(", offset=#").
        append(util::ToHexString(static_cast<uint32_t>(offset))).append(")");
    return str;
}

FuncInfoRecord::FuncInfoRecord() : DebugRecord(DebugRecordKind::funcInfo), functionSymbolIndex(), fullName(), sourceFileNameId(), frameSize(), main(false)
{
}

FuncInfoRecord::FuncInfoRecord(uint32_t functionSymbolIndex_, const std::string& fullName_, uint32_t sourceFileNameId_, int64_t frameSize_, bool main_) :
    DebugRecord(DebugRecordKind::funcInfo), functionSymbolIndex(functionSymbolIndex_), fullName(fullName_), sourceFileNameId(sourceFileNameId_), frameSize(frameSize_), 
    main(main_)
{
}

void FuncInfoRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(functionSymbolIndex);
    debugSection->EmitString(fullName);
    debugSection->EmitTetra(sourceFileNameId);
    debugSection->EmitOcta(frameSize);
    uint32_t cfgSize = static_cast<uint32_t>(cfg.size());
    debugSection->EmitTetra(cfgSize);
    for (uint32_t i = 0; i < cfgSize; ++i)
    {
        int32_t prevLine = cfg[i].first;
        int32_t nextLine = cfg[i].second;
        debugSection->EmitTetra(prevLine);
        debugSection->EmitTetra(nextLine);
    }
    debugSection->EmitByte(main ? 1 : 0);
    uint32_t localsSize = static_cast<uint32_t>(localInfoRecords.size());
    debugSection->EmitTetra(localsSize);
    for (uint32_t i = 0; i < localsSize; ++i)
    {
        LocalInfoRecord& localInfoRecord = localInfoRecords[i];
        localInfoRecord.Emit(debugSection);
    }
}

void FuncInfoRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    functionSymbolIndex = debugSection->ReadTetra();
    if (functionSymbolIndex == 0x000000a8)
    {
        int x = 0;
    }
    fullName = debugSection->ReadString();
    sourceFileNameId = debugSection->ReadTetra();
    frameSize = debugSection->ReadOcta();
    uint32_t cfgSize = debugSection->ReadTetra();
    for (uint32_t i = 0; i < cfgSize; ++i)
    {
        uint32_t prev = debugSection->ReadTetra();
        uint32_t next = debugSection->ReadTetra();
        AddToCfg(prev, next);
    }
    main = debugSection->ReadByte() != 0;
    uint32_t localsSize = debugSection->ReadTetra();
    for (uint32_t i = 0; i < localsSize; ++i)
    {
        LocalInfoRecord localInfoRecord;
        localInfoRecord.Read(debugSection);
        localInfoRecords.push_back(std::move(localInfoRecord));
    }
}

std::string FuncInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("functionSymbolIndex=").append(std::to_string(functionSymbolIndex)).append(", fullName=").append(fullName).append(", sourceFileNameId=").append(
        std::to_string(sourceFileNameId)).append(", frameSize=#").append(util::ToHexString(std::uint64_t(frameSize))).append(", main=").
        append(main ? "true" : "false").append("locals_count=#").append(util::ToHexString(std::uint32_t(localInfoRecords.size()))).append(")");
    return str;
}

void FuncInfoRecord::AddToCfg(int32_t prev, int32_t next)
{
    cfg.push_back(std::make_pair(prev, next));
}

void FuncInfoRecord::AddLocalInfoRecord(LocalInfoRecord&& localInfoRecord)
{
    localInfoRecords.push_back(std::move(localInfoRecord));
}

StartFuncRecord::StartFuncRecord() : DebugRecord(DebugRecordKind::startFunc), functionSymbolIndex()
{
}

StartFuncRecord::StartFuncRecord(uint32_t functionSymbolIndex_) : DebugRecord(DebugRecordKind::startFunc), functionSymbolIndex(functionSymbolIndex_)
{
}

void StartFuncRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(functionSymbolIndex);
}

void StartFuncRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    functionSymbolIndex = debugSection->ReadTetra();
}

std::string StartFuncRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("functionSymbolIndex=").append(std::to_string(functionSymbolIndex)).append(")");
    return str;
}

EndFuncRecord::EndFuncRecord() : DebugRecord(DebugRecordKind::endFunc), functionSymbolIndex()
{
}

EndFuncRecord::EndFuncRecord(uint32_t functionSymbolIndex_) : DebugRecord(DebugRecordKind::endFunc), functionSymbolIndex(functionSymbolIndex_)
{
}

void EndFuncRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(functionSymbolIndex);
}

void EndFuncRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    functionSymbolIndex = debugSection->ReadTetra();
}

std::string EndFuncRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("functionSymbolIndex=").append(std::to_string(functionSymbolIndex)).append(")");
    return str;
}

LineInfoRecord::LineInfoRecord() : DebugRecord(DebugRecordKind::lineInfo), offset(), lineColLen(), index(-1)
{
}

LineInfoRecord::LineInfoRecord(uint32_t offset_, const soul::ast::LineColLen& lineColLen_, int32_t index_) :
    DebugRecord(DebugRecordKind::lineInfo), offset(offset_), lineColLen(lineColLen_), index(index_)
{
}

void LineInfoRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(offset);
    debugSection->EmitTetra(index);
    debugSection->EmitTetra(lineColLen.line);
    debugSection->EmitTetra(lineColLen.col);
    debugSection->EmitTetra(lineColLen.len);
}

void LineInfoRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    offset = debugSection->ReadTetra();
    index = debugSection->ReadTetra();
    int32_t lineNumber = debugSection->ReadTetra();
    int32_t col = debugSection->ReadTetra();
    int32_t len = debugSection->ReadTetra();
    lineColLen = soul::ast::LineColLen(lineNumber, col, len);
}

std::string LineInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(offset=#").append(util::ToHexString(offset)).append(", lineNumber=").
        append(std::to_string(lineColLen.line)).
        append(", col=").append(std::to_string(lineColLen.col)).append(", len=").append(std::to_string(lineColLen.len)).append(")");
    return str;
}

BeginTryRecord::BeginTryRecord() : DebugRecord(DebugRecordKind::beginTry), tryBlockId(), parentTryBlockId(), offset()
{
}

BeginTryRecord::BeginTryRecord(uint32_t tryBlockId_, uint32_t parentTryBlockId_, uint32_t offset_) :
    DebugRecord(DebugRecordKind::beginTry), tryBlockId(tryBlockId_), parentTryBlockId(parentTryBlockId_), offset(offset_)
{
}

void BeginTryRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(tryBlockId);
    debugSection->EmitTetra(parentTryBlockId);
    debugSection->EmitTetra(offset);
}

void BeginTryRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    tryBlockId = debugSection->ReadTetra();
    parentTryBlockId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string BeginTryRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("tryBlockId=#").append(util::ToHexString(tryBlockId)).append(", parentTryBlockId=#").append(util::ToHexString(parentTryBlockId)).append(
        ", offset=#").append(util::ToHexString(offset)).append(")");
    return str;
}

EndTryRecord::EndTryRecord() : DebugRecord(DebugRecordKind::endTry), tryBlockId(), offset()
{
}

EndTryRecord::EndTryRecord(uint32_t tryBlockId_, uint32_t offset_) :
    DebugRecord(DebugRecordKind::endTry), tryBlockId(tryBlockId_), offset(offset_)
{
}

void EndTryRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(tryBlockId);
    debugSection->EmitTetra(offset);
}

void EndTryRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    tryBlockId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string EndTryRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("tryBlockId=#").append(util::ToHexString(tryBlockId)).append(", offset=#").append(util::ToHexString(offset)).append(")");
    return str;
}

CatchRecord::CatchRecord() : DebugRecord(DebugRecordKind::catch_), catchBlockId(), tryBlockId(), caughtTypeId1(), caughtTypeId2()
{
}

CatchRecord::CatchRecord(uint32_t catchBlockId_, uint32_t tryBlockId_, uint64_t caughtTypeId1_, uint64_t caughtTypeId2_) :
    DebugRecord(DebugRecordKind::catch_), catchBlockId(catchBlockId_), tryBlockId(tryBlockId_), caughtTypeId1(caughtTypeId1_), caughtTypeId2(caughtTypeId2_)
{
}

void CatchRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(catchBlockId);
    debugSection->EmitTetra(tryBlockId);
    debugSection->EmitOcta(caughtTypeId1);
    debugSection->EmitOcta(caughtTypeId2);
}

void CatchRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    catchBlockId = debugSection->ReadTetra();
    tryBlockId = debugSection->ReadTetra();
    caughtTypeId1 = debugSection->ReadOcta();
    caughtTypeId2 = debugSection->ReadOcta();
}

std::string CatchRecord::ToString() const
{
    util::uuid typeId;
    IntsToUuid(caughtTypeId1, caughtTypeId2, typeId);
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("catchBlockId=#").append(util::ToHexString(catchBlockId)).append(", tryBlockId=#").append(util::ToHexString(tryBlockId)).append(
        ", typeId=").append(util::ToString(typeId)).append(")");
    return str;
}

BeginCleanupRecord::BeginCleanupRecord() : DebugRecord(DebugRecordKind::beginCleanup), cleanupBlockId(), tryBlockId(), offset()
{
}

BeginCleanupRecord::BeginCleanupRecord(uint32_t cleanupBlockId_, uint32_t tryBlockId_, uint32_t offset_) :
    DebugRecord(DebugRecordKind::beginCleanup), cleanupBlockId(cleanupBlockId_), tryBlockId(tryBlockId_), offset(offset_)
{

}
void BeginCleanupRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(cleanupBlockId);
    debugSection->EmitTetra(tryBlockId);
    debugSection->EmitTetra(offset);
}

void BeginCleanupRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    cleanupBlockId = debugSection->ReadTetra();
    tryBlockId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string BeginCleanupRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("cleanupBlockId=#").append(util::ToHexString(cleanupBlockId)).append(", tryBlockId=#").append(util::ToHexString(tryBlockId)).append(
        ", offset=#").append(util::ToHexString(offset)).append(")");
    return str;
}

EndCleanupRecord::EndCleanupRecord() : DebugRecord(DebugRecordKind::endCleanup), cleanupBlockId(), offset()
{
}

EndCleanupRecord::EndCleanupRecord(uint32_t cleanupBlockId_, uint32_t offset_) :
    DebugRecord(DebugRecordKind::endCleanup), cleanupBlockId(cleanupBlockId_), offset(offset_)
{
}

void EndCleanupRecord::Emit(DebugSection* debugSection)
{
    DebugRecord::Emit(debugSection);
    debugSection->EmitTetra(cleanupBlockId);
    debugSection->EmitTetra(offset);
}

void EndCleanupRecord::Read(DebugSection* debugSection)
{
    DebugRecord::Read(debugSection);
    cleanupBlockId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string EndCleanupRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("cleanupBlockId=#").append(util::ToHexString(cleanupBlockId)).append(
        ", offset=#").append(util::ToHexString(offset)).append(")");
    return str;
}

FieldInfoRecord::FieldInfoRecord() : DebugRecord(DebugRecordKind::fieldInfo), name(), typeId(-1), offset(-1)
{
}

FieldInfoRecord::FieldInfoRecord(const std::string& name_, int32_t typeId_, int32_t offset_) : 
    DebugRecord(DebugRecordKind::fieldInfo), name(name_), typeId(typeId_), offset(offset_)
{
}

void FieldInfoRecord::Emit(DebugSection* debugSection)
{
    debugSection->EmitString(name);
    debugSection->EmitTetra(typeId);
    debugSection->EmitTetra(offset);
}

void FieldInfoRecord::Read(DebugSection* debugSection)
{
    name = debugSection->ReadString();
    typeId = debugSection->ReadTetra();
    offset = debugSection->ReadTetra();
}

std::string FieldInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("name=").append(name).append(", typeId=#").append(util::ToHexString(static_cast<uint32_t>(typeId))).
        append(", offset=#").append(util::ToHexString(static_cast<uint32_t>(offset)));
    return str;
}

StructTypeInfoRecord::StructTypeInfoRecord() : TypeInfoRecord(DebugRecordKind::structTypeInfo), size(-1)
{
}

StructTypeInfoRecord::StructTypeInfoRecord(const std::string& fullName_, int32_t typeId_, int64_t size_) :
    TypeInfoRecord(DebugRecordKind::structTypeInfo, fullName_, typeId_), size(size_)
{
}

void StructTypeInfoRecord::Emit(DebugSection* debugSection)
{
    TypeInfoRecord::Emit(debugSection);
    debugSection->EmitOcta(size);
    int32_t n = static_cast<int32_t>(fieldInfoRecords.size());
    debugSection->EmitTetra(n);
    for (int32_t i = 0; i < n; ++i)
    {
        FieldInfoRecord& fieldInfoRecord = fieldInfoRecords[i]; 
        fieldInfoRecord.Emit(debugSection);
    }
}

void StructTypeInfoRecord::Read(DebugSection* debugSection)
{
    TypeInfoRecord::Read(debugSection);
    size = debugSection->ReadOcta();
    int32_t n = debugSection->ReadTetra();
    for (int32_t i = 0; i < n; ++i)
    {
        FieldInfoRecord fieldInfoRecord;
        fieldInfoRecord.Read(debugSection);
        fieldInfoRecords.push_back(std::move(fieldInfoRecord));
    }
}

std::string StructTypeInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("fullName=").append(FullName()).append(", typeId=#").append(util::ToHexString(static_cast<uint32_t>(TypeId()))).
        append(", size=#").append(util::ToHexString(static_cast<uint64_t>(size))).
        append(", field_info_count=#").append(util::ToHexString(static_cast<uint32_t>(fieldInfoRecords.size()))).append(")");
    return str;
}

void StructTypeInfoRecord::AddFieldInfoRecord(FieldInfoRecord&& fieldInfoRecord)
{
    fieldInfoRecords.push_back(std::move(fieldInfoRecord));
}

ArrayTypeInfoRecord::ArrayTypeInfoRecord() : TypeInfoRecord(DebugRecordKind::arrayTypeInfo), elementTypeId(-1), size(-1)
{
}

ArrayTypeInfoRecord::ArrayTypeInfoRecord(const std::string& fullName_, int32_t typeId_, int32_t elementTypeId_, int64_t size_) :
    TypeInfoRecord(DebugRecordKind::arrayTypeInfo, fullName_, typeId_), elementTypeId(elementTypeId_), size(size_)
{
}

void ArrayTypeInfoRecord::Emit(DebugSection* debugSection)
{
    TypeInfoRecord::Emit(debugSection);
    debugSection->EmitTetra(elementTypeId);
    debugSection->EmitOcta(size);
}

void ArrayTypeInfoRecord::Read(DebugSection* debugSection)
{
    TypeInfoRecord::Read(debugSection);
    elementTypeId = debugSection->ReadTetra();
    size = debugSection->ReadOcta();
}

std::string ArrayTypeInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("fullName=").append(FullName()).append(", typeId=#").append(util::ToHexString(static_cast<uint32_t>(TypeId()))).
        append(", elementTypeId=#").append(util::ToHexString(static_cast<uint32_t>(elementTypeId))).
        append(", size=#").append(util::ToHexString(static_cast<uint64_t>(size)));
    return str;
}

FunctionTypeInfoRecord::FunctionTypeInfoRecord() : TypeInfoRecord(DebugRecordKind::functionTypeInfo)
{
}

FunctionTypeInfoRecord::FunctionTypeInfoRecord(const std::string& fullName_, int32_t typeId_) :
    TypeInfoRecord(DebugRecordKind::functionTypeInfo, fullName_, typeId_)
{
}

void FunctionTypeInfoRecord::Emit(DebugSection* debugSection)
{
    TypeInfoRecord::Emit(debugSection);
}

void FunctionTypeInfoRecord::Read(DebugSection* debugSection)
{
    TypeInfoRecord::Read(debugSection);
}

std::string FunctionTypeInfoRecord::ToString() const
{
    std::string str = DebugRecordKindStr(Kind());
    str.append("(").append("fullName=").append(FullName()).append(", typeId=#").append(util::ToHexString(static_cast<uint32_t>(TypeId()))).
        append(")");
    return str;
}

DebugRecord* MakeDebugRecord(DebugRecordKind kind)
{
    switch (kind)
    {
        case DebugRecordKind::fileInfo: return new FileInfoRecord();
        case DebugRecordKind::funcInfo: return new FuncInfoRecord();
        case DebugRecordKind::localInfo: return new LocalInfoRecord();
        case DebugRecordKind::startFunc:return new StartFuncRecord();
        case DebugRecordKind::endFunc: return new EndFuncRecord();
        case DebugRecordKind::lineInfo: return new LineInfoRecord();
        case DebugRecordKind::beginTry: return new BeginTryRecord();
        case DebugRecordKind::endTry: return new EndTryRecord();
        case DebugRecordKind::catch_: return new CatchRecord();
        case DebugRecordKind::beginCleanup: return new BeginCleanupRecord();
        case DebugRecordKind::endCleanup: return new EndCleanupRecord();
        case DebugRecordKind::structTypeInfo: return new StructTypeInfoRecord();
        case DebugRecordKind::fieldInfo: return new FieldInfoRecord();
        case DebugRecordKind::arrayTypeInfo: return new ArrayTypeInfoRecord();
        case DebugRecordKind::functionTypeInfo: return new FunctionTypeInfoRecord();
    }
    return nullptr;
}

DebugRecord* ReadDebugRecord(DebugSection* debugSection)
{
    DebugRecordKind kind = static_cast<DebugRecordKind>(debugSection->ReadByte());
    DebugRecord* debugRecord = MakeDebugRecord(kind);
    debugRecord->Read(debugSection);
    return debugRecord;
}

} // namespace cmajor::systemx::object
