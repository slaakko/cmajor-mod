// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.binary.file;

import cmajor.systemx.object.symbol;
import cmajor.systemx.object.link;
import cmajor.systemx.object.debug;
import cmajor.systemx.object.resource;
import std.core;
import util;

export namespace cmajor::systemx::object {

enum class BinaryFileKind
{
    objectFile, archiveFile, executableFile
};

enum class SectionKind
{
    fhdr, code, data, symb, link, dbug, rsrc
};

const char binary_file_version_1 = '1';
const char current_binary_file_version = binary_file_version_1;

class Section;
class BinaryFile;
class Resource;

BinaryFile* ReadBinaryFile(const std::string& filePath);
BinaryFile* ReadBinaryFile(util::BinaryStreamReader& reader, const std::string& filePath);
Section* ReadSection(BinaryFile* file, util::BinaryStreamReader& reader);

class Section
{
public:
    Section(SectionKind kind_, BinaryFile* file_);
    virtual ~Section();
    BinaryFile* File() const { return file; }
    SectionKind Kind() const { return kind; }
    int64_t Address() const { return pos; }
    int64_t Length() const { return data.size(); }
    int64_t BaseAddress() const { return baseAddress; }
    void SetBaseAddress(int64_t baseAddress_) { baseAddress = baseAddress_; }
    bool IsHeaderSection() const { return kind == SectionKind::fhdr; }
    bool IsCodeSection() const { return kind == SectionKind::code; }
    bool IsDataSection() const { return kind == SectionKind::data; }
    bool IsSymbolSection() const { return kind == SectionKind::symb; }
    bool IsLinkSection() const { return kind == SectionKind::link; }
    bool IsDebugSection() const { return kind == SectionKind::dbug; }
    bool IsResourceSection() const { return kind == SectionKind::rsrc; }
    std::string Name() const;
    virtual void Write(util::BinaryStreamWriter& writer);
    virtual void Read(util::BinaryStreamReader& reader);
    virtual void AddSymbol(Symbol* symbol);
    virtual void Finalize();
    void EmitByte(uint8_t x);
    void EmitByte(int64_t address, uint8_t x);
    uint8_t ReadByte();
    uint8_t GetByte(int64_t address) const;
    void EmitWyde(uint16_t x);
    void EmitWyde(int64_t address, uint16_t x);
    uint16_t ReadWyde();
    uint16_t GetWyde(int64_t address) const;
    void EmitTetra(uint32_t x);
    void EmitTetra(int64_t address, uint32_t x);
    uint32_t ReadTetra();
    uint32_t GetTetra(int64_t address) const;
    void EmitOcta(uint64_t x);
    void EmitOcta(int64_t address, uint64_t x);
    uint64_t ReadOcta();
    uint64_t GetOcta(int64_t address) const;
    void EmitShortOffset(uint16_t offset);
    void EmitShortOffset(int64_t address, uint16_t offset);
    void EmitLongOffset(uint32_t offset);
    void EmitLongOffset(int64_t address, uint32_t offset);
    void EmitString(const std::string& string);
    std::string ReadString();
    void EmitValue(const cmajor::systemx::object::Value& value);
    Value ReadValue();
    void Align(int64_t alignment);
    void SetPos(int64_t pos_) { pos = pos_; }
    int64_t DataLength() const { return dataLength; }
    void SetDataLength(int64_t dataLength_) { dataLength = dataLength_; }
    Section* CopyTargetSection() const { return copyTargetSection; }
    void SetCopyTargetSection(Section* copyTargetSection_) { copyTargetSection = copyTargetSection_; }
    int64_t RemoveOffset() const { return removeOffset; }
    void SetRemoveOffset(int64_t removeOffset_) { removeOffset = removeOffset_; }
    int64_t DataStart() const { return dataStart; }
private:
    BinaryFile* file;
    SectionKind kind;
    int64_t baseAddress;
    std::vector<uint8_t> data;
    int64_t pos;
    int64_t dataLength;
    Section* copyTargetSection;
    int64_t removeOffset;
    int64_t dataStart;
};

class HeaderSection : public Section
{
public:
    HeaderSection(BinaryFile* file_);
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
};

class CodeSection : public Section
{
public:
    CodeSection(BinaryFile* file_);
};

class DataSection : public Section
{
public:
    DataSection(BinaryFile* file_);
    void AddSymbol(Symbol* symbol) override;
    void Finalize() override;
private:
    std::vector<Symbol*> symbols;
};

class SymbolSection : public Section
{
public:
    SymbolSection(BinaryFile* file_);
    void Read(util::BinaryStreamReader& reader) override;
    void Write(util::BinaryStreamWriter& writer) override;
private:
    void EmitSymbolTable();
    void EmitSymbol(Symbol* symbol);
    Symbol* ReadSymbol();
};

class LinkSection : public Section
{
public:
    LinkSection(BinaryFile* file_);
    LinkSection(const LinkSection&) = delete;
    LinkSection& operator=(const LinkSection&) = delete;
    void Finalize() override;
    void Read(util::BinaryStreamReader& reader) override;
    int AddLinkCommand(LinkCommand* linkCommand, bool setId);
    void MapLinkCommand(LinkCommand* linkCommand);
    void MapLinkCommands();
    LinkCommand* GetLinkCommand(int32_t linkCommandId) const;
    const std::vector<std::unique_ptr<LinkCommand>>& LinkCommands() const { return linkCommands; };
    void ReplaceLinkCommmands(std::vector<std::unique_ptr<LinkCommand>>& linkCommands_);
private:
    void EmitLinkCommands();
    std::vector<std::unique_ptr<LinkCommand>> linkCommands;
    std::map<int32_t, LinkCommand*> linkCommandMap;
};

class DebugSection : public Section
{
public:
    DebugSection(BinaryFile* file_);
    DebugSection(const DebugSection&) = delete;
    DebugSection operator=(const DebugSection&) = delete;
    void AddDebugRecord(DebugRecord* debugRecord);
    void Finalize() override;
    void Read(util::BinaryStreamReader& reader) override;
    const std::vector<std::unique_ptr<DebugRecord>>& DebugRecords() const { return debugRecords; };
    TypeInfoRecord* GetTypeInfoRecord(int32_t typeId) const;
private:
    void EmitDebugRecords();
    std::vector<std::unique_ptr<DebugRecord>> debugRecords;
    std::map<int32_t, TypeInfoRecord*> typeInfoMap;
};

class ResourceSection : public Section
{
public:
    ResourceSection(BinaryFile* file_);
    ResourceSection(const ResourceSection&) = delete;
    ResourceSection& operator=(const ResourceSection&) = delete;
    void AddResource(Resource* resource);
    void Finalize() override;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    const std::vector<std::unique_ptr<Resource>>& Resources() const { return resources; }
    void AddResourceInfo(const ResourceInfo& resourceInfo);
private:
    void EmitResources();
    std::vector<std::unique_ptr<Resource>> resources;
    std::vector<ResourceInfo> resourceInfos;
};

class BinaryFile
{
public:
    BinaryFile(const std::string& filePath_, BinaryFileKind kind_);
    BinaryFile(const BinaryFile&) = delete;
    BinaryFile& operator=(const BinaryFile&) = delete;
    virtual ~BinaryFile();
    virtual void CreateSections() = 0;
    virtual void Finalize();
    virtual void FinishRead();
    void WriteFile();
    virtual void Write(util::BinaryStreamWriter& writer);
    virtual void Read(util::BinaryStreamReader& reader);
    const std::string& FilePath() const { return filePath; }
    const std::string& FileName() const { return fileName; }
    void SetFileName(const std::string& fileName_);
    BinaryFileKind Kind() const { return kind; }
    BinaryFile* Parent() const { return parent; }
    void SetParent(BinaryFile* parent_) { parent = parent_; }
    int32_t SectionCount() const { return sections.size(); }
    void AddSection(Section* section);
    HeaderSection* GetHeaderSection() const { return headerSection; }
    CodeSection* GetCodeSection() const { return codeSection; }
    DataSection* GetDataSection() const { return dataSection; }
    ResourceSection* GetResourceSection() const { return resourceSection; }
    SymbolSection* GetSymbolSection() const { return symbolSection; }
    LinkSection* GetLinkSection() const { return linkSection; }
    DebugSection* GetDebugSection() const { return debugSection; }
    SymbolTable& GetSymbolTable() { return *symbolTable; }
    SymbolTable* ReleaseSymbolTable() { return symbolTable.release(); }
private:
    BinaryFileKind kind;
    BinaryFile* parent;
    std::string filePath;
    std::string fileName;
    std::vector<std::unique_ptr<Section>> sections;
    HeaderSection* headerSection;
    CodeSection* codeSection;
    DataSection* dataSection;
    ResourceSection* resourceSection;
    SymbolSection* symbolSection;
    LinkSection* linkSection;
    DebugSection* debugSection;
    std::unique_ptr<SymbolTable> symbolTable;
};

class ObjectFile : public BinaryFile
{
public:
    ObjectFile();
    ObjectFile(const std::string& filePath_);
    void CreateSections() override;
    void FinishRead() override;
    void MapLinkCommands();
};

class ArchiveFile : public BinaryFile
{
public:
    ArchiveFile(const std::string& filePath_);
    void CreateSections() override;
    void AddObjectFile(ObjectFile* objectFile);
    const std::vector<std::unique_ptr<ObjectFile>>& ObjectFiles() const { return objectFiles; }
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
private:
    std::vector<std::unique_ptr<ObjectFile>> objectFiles;
};

class ExecutableFile : public BinaryFile
{
public:
    ExecutableFile(const std::string& filePath_);
    void CreateSections() override;
    void FinishRead() override;
};

} // namespace cmajor::systemx::object
