// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.coff;

import std.core;
import util;

export namespace cmajor::sbin::coff {

class CoffObjectFile;

// machine:
const uint16_t IMAGE_FILE_MACHINE_AMD64 = 0x8664u;

// section header characteristics:
const uint32_t IMAGE_SCN_TYPE_NO_PAD = 0x00000008u;
const uint32_t IMAGE_SCN_CNT_CODE = 0x00000020u;
const uint32_t IMAGE_SCN_CNT_INITIALIZED_DATA = 0x00000040u;
const uint32_t IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x00000080u;
const uint32_t IMAGE_SCN_LNK_INFO = 0x00000200u;
const uint32_t IMAGE_SCN_LNK_REMOVE = 0x00000800u;
const uint32_t IMAGE_SCN_LNK_COMDAT = 0x00001000u;
const uint32_t IMAGE_SCN_GPREL = 0x00008000u;
const uint32_t IMAGE_SCN_ALIGNMENT_MASK = 0x00F00000u;
const uint32_t IMAGE_SCN_ALIGNMENT_SHIFT = 20u;
const uint32_t IMAGE_SCN_ALIGN_1BYTES = 0x1u;
const uint32_t IMAGE_SCN_ALIGN_2BYTES = 0x2u;
const uint32_t IMAGE_SCN_ALIGN_4BYTES = 0x3u;
const uint32_t IMAGE_SCN_ALIGN_8BYTES = 0x4u;
const uint32_t IMAGE_SCN_ALIGN_16BYTES = 0x5u;
const uint32_t IMAGE_SCN_ALIGN_32BYTES = 0x6u;
const uint32_t IMAGE_SCN_ALIGN_64BYTES = 0x7u;
const uint32_t IMAGE_SCN_ALIGN_128BYTES = 0x8u;
const uint32_t IMAGE_SCN_ALIGN_256BYTES = 0x9u;
const uint32_t IMAGE_SCN_ALIGN_512BYTES = 0xAu;
const uint32_t IMAGE_SCN_ALIGN_1024BYTES = 0xBu;
const uint32_t IMAGE_SCN_ALIGN_2048BYTES = 0xCu;
const uint32_t IMAGE_SCN_ALIGN_4096BYTES = 0xDu;
const uint32_t IMAGE_SCN_ALIGN_8192BYTES = 0xEu;
const uint32_t IMAGE_SCN_LNK_NRELOC_OVFL = 0x01000000u;
const uint32_t IMAGE_SCN_MEM_DISCARDABLE = 0x02000000u;
const uint32_t IMAGE_SCN_MEM_NOT_CACHED = 0x04000000u;
const uint32_t IMAGE_SCN_MEM_NOT_PAGED = 0x08000000u;
const uint32_t IMAGE_SCN_MEM_SHARED = 0x10000000u;
const uint32_t IMAGE_SCN_MEM_EXECUTE = 0x20000000u;
const uint32_t IMAGE_SCN_MEM_READ = 0x40000000u;
const uint32_t IMAGE_SCN_MEM_WRITE = 0x80000000u;

// relocation types:
const uint16_t IMAGE_REL_AMD64_ABSOLUTE = 0x0000u;
const uint16_t IMAGE_REL_AMD64_ADDR64 = 0x0001u;
const uint16_t IMAGE_REL_AMD64_ADDR32 = 0x0002u;
const uint16_t IMAGE_REL_AMD64_ADDR32NB = 0x0003u;
const uint16_t IMAGE_REL_AMD64_REL32 = 0x0004u;
const uint16_t IMAGE_REL_AMD64_REL32_1 = 0x0005u;
const uint16_t IMAGE_REL_AMD64_REL32_2 = 0x0006u;
const uint16_t IMAGE_REL_AMD64_REL32_3 = 0x0007u;
const uint16_t IMAGE_REL_AMD64_REL32_4 = 0x0008u;
const uint16_t IMAGE_REL_AMD64_REL32_5 = 0x0009u;
const uint16_t IMAGE_REL_AMD64_SECTION = 0x000Au;
const uint16_t IMAGE_REL_AMD64_SECREL = 0x000Bu;
const uint16_t IMAGE_REL_AMD64_SECREL7 = 0x000Cu;
const uint16_t IMAGE_REL_AMD64_TOKEN = 0x000Du;
const uint16_t IMAGE_REL_AMD64_SREL32 = 0x000Eu;
const uint16_t IMAGE_REL_AMD64_PAIR = 0x000Fu;
const uint16_t IMAGE_REL_AMD64_SSPAN32 = 0x0010u;

// symbol special section number values:
const int16_t IMAGE_SYM_UNDEFINED = 0;
const int16_t IMAGE_SYM_ABSOLUTE = -1;
const int16_t IMAGE_SYM_DEBUG = -2;

// symbol storage class:
const uint8_t IMAGE_SYM_CLASS_NULL = 0u;
const uint8_t IMAGE_SYM_CLASS_EXTERNAL = 2u;
const uint8_t IMAGE_SYM_CLASS_STATIC = 3u;
const uint8_t IMAGE_SYM_CLASS_LABEL = 6u;
const uint8_t IMAGE_SYM_CLASS_FUNCTION = 101u;
const uint8_t IMAGE_SYM_CLASS_FILE = 103u;

// symbol type:
const uint8_t IMAGE_SYM_TYPE_NULL = 0u;
const uint8_t IMAGE_SYM_DTYPE_NULL = 0u;
const uint8_t IMAGE_SYM_DTYPE_FUNCTION = 0x20u;

// COMDAT selection:
const uint8_t IMAGE_COMDAT_SELECT_NODUPLICATES = 1;
const uint8_t IMAGE_COMDAT_SELECT_ANY = 2;
const uint8_t IMAGE_COMDAT_SELECT_SAME_SIZE = 3;
const uint8_t IMAGE_COMDAT_SELECT_EXACT_MATCH = 4;
const uint8_t IMAGE_COMDAT_SELECT_ASSOCIATIVE = 5;
const uint8_t IMAGE_COMDAT_SELECT_LARGEST = 6;

class Positions
{
public:
    Positions(util::MemoryStream& memoryStream_);
    uint8_t* Data() const { return memoryStream.Data(); }
    int64_t SymbolTablePosition() const { return symbolTablePosition; }
    void SetSymbolTablePosition(int64_t position) { symbolTablePosition = position; }
    void SetRawDataPosition(uint16_t sectionNumber, int64_t rawDataPosition);
    int64_t GetRawDataPosition(uint16_t sectionNumber) const;
    void SetRelocationsPosition(uint16_t sectionNumber, int64_t relocationsPosition);
    int64_t GetRelocationsPosition(uint16_t sectionNumber) const;
    void SetLineNumbersPosition(uint16_t sectionNumber, int64_t lineNumbersPosition);
    int64_t GetLineNumbersPosition(uint16_t sectionNumber) const;
private:
    util::MemoryStream& memoryStream;
    int64_t symbolTablePosition;
    std::map<uint16_t, int64_t> rawDataPositions;
    std::map<uint16_t, int64_t> relocationsPositions;
    std::map<uint16_t, int64_t> lineNumbersPositions;
};

class CoffHeader
{
public:
    CoffHeader();
    CoffHeader(util::LittleEndianMemoryReader& reader);
    static int64_t Size() { return 20; }
    void Dump(util::CodeFormatter& formatter);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    uint16_t Machine() const { return machine; }
    uint16_t NumberOfSections() const { return numberOfSections; }
    std::time_t Timestamp() const { return timestamp; }
    uint32_t SymbolTableOffset() const { return symbolTableOffset; }
    uint32_t NumberOfSymbols() const { return numberOfSymbols; }
    uint16_t SizeOfOptionalHeader() const { return sizeOfOptionalHeader; }
    uint16_t Characteristics() const { return characteristics; }
private:
    uint16_t machine;
    uint16_t numberOfSections;
    std::time_t timestamp;
    uint32_t symbolTableOffset;
    uint32_t numberOfSymbols;
    uint16_t sizeOfOptionalHeader;
    uint16_t characteristics;
};

class SectionHeader
{
public:
    SectionHeader();
    SectionHeader(util::LittleEndianMemoryReader& reader);
    static int64_t Size() { return 40; }
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    void SetCharacteristicsFlag(uint32_t flag)
    {
        characteristics = characteristics | flag;
    }
    void SetAlignment(uint32_t alignment)
    {
        characteristics = characteristics | (alignment << IMAGE_SCN_ALIGNMENT_SHIFT);
    }
    uint16_t Number() const { return number; }
    void SetNumber(uint16_t number_) { number = number_; }
    void SetName(const std::string& name_, CoffObjectFile* file);
    int32_t VirtualSize() const { return virtualSize; }
    int32_t VirtualAddress() const { return virtualAddress; }
    uint32_t SizeOfRawData() const { return sizeOfRawData; }
    uint32_t RawDataOffset() const { return rawDataOffset; }
    uint32_t RelocationsOffset() const { return relocationsOffset; }
    uint32_t LineNumbersOffset() const { return lineNumbersOffset; }
    uint16_t NumberOfRelocations() const { return numberOfRelocations; }
    uint16_t NumberOfLineNumbers() const { return numberOfLineNumbers; }
    uint32_t Characteristics() const { return characteristics; }
private:
    uint16_t number;
    char name[8];
    std::string nameStr;
    int32_t virtualSize;
    int32_t virtualAddress;
    uint32_t sizeOfRawData;
    uint32_t rawDataOffset;
    uint32_t relocationsOffset;
    uint32_t lineNumbersOffset;
    uint16_t numberOfRelocations;
    uint16_t numberOfLineNumbers;
    uint32_t characteristics;
};

class Relocation
{
public:
    Relocation();
    Relocation(util::LittleEndianMemoryReader& reader);
    Relocation(uint32_t virtualAddress_, uint32_t symbolTableIndex_, uint16_t type_);
    static int64_t Size() { return 10; }
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
private:
    uint32_t virtualAddress;
    uint32_t symbolTableIndex;
    uint16_t type;
};

class RelocationTable
{
public:
    RelocationTable();
    RelocationTable(util::LittleEndianMemoryReader& reader, uint16_t numberOfRelocations);
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    const std::vector<Relocation>& Relocations() const { return relocations; }
private:
    std::vector<Relocation> relocations;
};

enum class ReadFlags : int
{
    none = 0,
    readRelocations = 1 << 0,
    readSectionData = 1 << 1
};

constexpr ReadFlags operator|(ReadFlags left, ReadFlags right)
{
    return ReadFlags(int(left) | int(right));
}

constexpr ReadFlags operator&(ReadFlags left, ReadFlags right)
{
    return ReadFlags(int(left) & int(right));
}

constexpr ReadFlags operator~(ReadFlags flags)
{
    return ReadFlags(~int(flags));
}

enum class DumpFlags : int
{
    none = 0,
    dumpSectionData = 1 << 0,
    dumpRelocations = 1 << 1
};

constexpr DumpFlags operator|(DumpFlags left, DumpFlags right)
{
    return DumpFlags(int(left) | int(right));
}

constexpr DumpFlags operator&(DumpFlags left, DumpFlags right)
{
    return DumpFlags(int(left) & int(right));
}

constexpr DumpFlags operator~(DumpFlags flags)
{
    return DumpFlags(~int(flags));
}

class Section
{
public:
    Section();
    Section(util::LittleEndianMemoryReader& reader, ReadFlags readFlags);
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file, DumpFlags flags);
    void WriteHeader(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    void WriteData(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    void WriteRelocationTable(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    const std::vector<uint8_t>& Data() const { return data; }
    void SetData(std::vector<uint8_t>&& data_) { data = std::move(data_); }
    uint32_t CheckSum() const { return checkSum; }
    void ComputeCheckSum();
    SectionHeader* GetSectionHeader() const { return sectionHeader.get(); }
    RelocationTable* GetRelocationTable() const { return relocationTable.get(); }
private:
    std::unique_ptr<SectionHeader> sectionHeader;
    std::unique_ptr<RelocationTable> relocationTable;
    std::vector<uint8_t> data;
    uint32_t checkSum;
};

class AuxSectionDefinitionSymbolTableEntry
{
public:
    AuxSectionDefinitionSymbolTableEntry();
    AuxSectionDefinitionSymbolTableEntry(util::LittleEndianMemoryReader& reader);
    static int64_t Size() { return 18; }
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    uint32_t Index() const { return index; }
    void SetIndex(uint32_t index_) { index = index_; }
    uint32_t Length() const { return length; }
    void SetLength(uint32_t length_) { length = length_; }
    uint16_t NumberOfRelocations() const { return numberOfRelocations; }
    void SetNumberOfRelocations(uint16_t numberOfRelocations_) { numberOfRelocations = numberOfRelocations_; }
    uint16_t NumberOfLineNumbers() const { return numberOfLineNumbers; }
    void SetNumberOfLineNumbers(uint16_t numberOfLineNumbers_) { numberOfLineNumbers = numberOfLineNumbers_; }
    uint32_t CheckSum() const { return checkSum; }
    void SetCheckSum(uint32_t checkSum_) { checkSum = checkSum_; }
    uint16_t Number() const { return number; }
    void SetNumber(uint16_t number_) { number = number_; }
    uint8_t Selection() const { return selection; }
private:
    uint32_t index;
    uint32_t length;
    uint16_t numberOfRelocations;
    uint16_t numberOfLineNumbers;
    uint32_t checkSum;
    uint16_t number;
    uint8_t selection;
};

class SymbolNameLocation
{
public:
    SymbolNameLocation();
    SymbolNameLocation(uint32_t offset_);
    bool InStringTable() const
    {
        return zeros == 0u;
    }
    uint32_t Offset() const { return offset; }
private:
    uint32_t zeros;
    uint32_t offset; // offset into the string table
};

class SymbolTableEntry
{
public:
    SymbolTableEntry();
    SymbolTableEntry(util::LittleEndianMemoryReader& reader);
    static int64_t Size() { return 18; }
    uint32_t Index() const { return index; }
    void SetIndex(uint32_t index_) { index = index_; }
    std::string GetName(CoffObjectFile* file) const;
    void SetName(const std::string& name, CoffObjectFile* file);
    uint32_t Value() const { return value; }
    void SetValue(uint32_t value_) { value = value_; }
    int16_t SectionNumber() const { return sectionNumber; }
    void SetSectioNumber(int16_t sectionNumber_) { sectionNumber = sectionNumber_; }
    int16_t Type() const { return type; }
    void SetType(int16_t type_) { type = type_; }
    uint8_t StorageClass() const { return storageClass; }
    void SetStorageClass(uint8_t storageClass_) { storageClass = storageClass_; }
    uint8_t NumberOfAuxSymbols() const { return numberOfAuxSymbols; }
    void SetNumberOfAuxSymbols(uint8_t numberOfAuxSymbols_) { numberOfAuxSymbols = numberOfAuxSymbols_; }
    void SetAuxSectionDefinitionEntry(AuxSectionDefinitionSymbolTableEntry* auxSectionDefinitionEntry_);
    AuxSectionDefinitionSymbolTableEntry* GetAuxSectionDefinitionEntry() const { return auxSectionDefinitionEntry.get(); }
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    bool IsSectionDefinition() const;
private:
    uint32_t index;
    union
    {
        char shortName[8];
        SymbolNameLocation location;
    };
    uint32_t value;
    int16_t sectionNumber;
    int16_t type;
    uint8_t storageClass;
    uint8_t numberOfAuxSymbols;
    std::unique_ptr<AuxSectionDefinitionSymbolTableEntry> auxSectionDefinitionEntry;
};

class SymbolTable
{
public:
    SymbolTable();
    SymbolTable(util::LittleEndianMemoryReader& reader, uint32_t numberOfSymbols);
    void Dump(util::CodeFormatter& formatter, CoffObjectFile* file);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    SymbolTableEntry* GetEntry(uint32_t symbolIndex) const;
    uint32_t GetNextEntryNumber() { return nextEntryNumber++; }
    uint32_t GetNumberOfEntries() const { return numberOfEntries; }
private:
    std::vector<std::unique_ptr<SymbolTableEntry>> entries;
    uint32_t nextEntryNumber;
    uint32_t numberOfEntries;
};

class StringTable
{
public:
    StringTable();
    StringTable(util::LittleEndianMemoryReader& reader);
    void Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions);
    std::string GetString(uint32_t offset) const;
    uint32_t AddString(const std::string& s);
private:
    int32_t size;
    std::map<uint32_t, std::string> offsetStringMap;
    std::map<std::string, uint32_t> stringOffsetMap;
    std::vector<std::string> strings;
};

class CoffObjectFile
{
public:
    CoffObjectFile();
    CoffObjectFile(const std::string& objectFilePath, ReadFlags readFlags);
    void Dump(util::CodeFormatter& formatter, DumpFlags flags);
    void Write(const std::string& objectFilePath);
    CoffHeader* Header() const { return coffHeader.get(); }
    const std::vector<std::unique_ptr<Section>>& Sections() const { return sections; }
    uint16_t GetNextSectionNumber() const { return static_cast<uint16_t>(sections.size() + 1u); }
    void AddSection(Section* section);
    Section* GetSection(uint16_t sectionNumber) const;
    SymbolTable* GetSymbolTable() const { return symbolTable.get(); }
    StringTable* GetStringTable() const { return stringTable.get(); }
private:
    std::unique_ptr<CoffHeader> coffHeader;
    std::vector<std::unique_ptr<Section>> sections;
    std::unique_ptr<SymbolTable> symbolTable;
    std::unique_ptr<StringTable> stringTable;
};

} // namespace machine::coff
