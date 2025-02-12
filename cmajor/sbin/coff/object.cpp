// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.coff;

namespace cmajor::sbin::coff {

Positions::Positions(util::MemoryStream& memoryStream_) : memoryStream(memoryStream_), symbolTablePosition(0)
{
}

void Positions::SetRawDataPosition(uint16_t sectionNumber, int64_t rawDataPosition)
{
    rawDataPositions[sectionNumber] = rawDataPosition;
}

int64_t Positions::GetRawDataPosition(uint16_t sectionNumber) const
{
    auto it = rawDataPositions.find(sectionNumber);
    if (it != rawDataPositions.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("raw data position for section " + std::to_string(sectionNumber) + " not found");
    }
}

void Positions::SetRelocationsPosition(uint16_t sectionNumber, int64_t relocationsPosition)
{
    relocationsPositions[sectionNumber] = relocationsPosition;
}

int64_t Positions::GetRelocationsPosition(uint16_t sectionNumber) const
{
    auto it = relocationsPositions.find(sectionNumber);
    if (it != relocationsPositions.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("relocations position for section " + std::to_string(sectionNumber) + " not found");
    }
}

void Positions::SetLineNumbersPosition(uint16_t sectionNumber, int64_t lineNumbersPosition)
{
    lineNumbersPositions[sectionNumber] = lineNumbersPosition;
}

int64_t Positions::GetLineNumbersPosition(uint16_t sectionNumber) const
{
    auto it = lineNumbersPositions.find(sectionNumber);
    if (it != lineNumbersPositions.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("line numbers position for section " + std::to_string(sectionNumber) + " not found");
    }
}

std::string SectionHeaderCharacteristicsStr(uint32_t characteristics)
{
    std::string s;
    if ((characteristics & IMAGE_SCN_TYPE_NO_PAD) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_TYPE_NO_PAD");
    }
    if ((characteristics & IMAGE_SCN_CNT_CODE) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_CNT_CODE");
    }
    if ((characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_CNT_INITIALIZED_DATA");
    }
    if ((characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_CNT_UNINITIALIZED_DATA");
    }
    if ((characteristics & IMAGE_SCN_LNK_INFO) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_LNK_INFO");
    }
    if ((characteristics & IMAGE_SCN_LNK_REMOVE) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_LNK_REMOVE");
    }
    if ((characteristics & IMAGE_SCN_LNK_COMDAT) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_LNK_COMDAT");
    }
    if ((characteristics & IMAGE_SCN_GPREL) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_GPREL");
    }
    if ((characteristics & IMAGE_SCN_ALIGNMENT_MASK) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        uint32_t alignment = (characteristics & IMAGE_SCN_ALIGNMENT_MASK) >> IMAGE_SCN_ALIGNMENT_SHIFT;
        switch (alignment)
        {
        case IMAGE_SCN_ALIGN_1BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_1BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_2BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_2BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_4BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_4BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_8BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_8BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_16BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_16BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_32BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_32BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_64BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_64BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_128BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_128BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_256BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_256BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_512BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_512BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_1024BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_1024BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_2048BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_2048BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_4096BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_4096BYTES");
            break;
        }
        case IMAGE_SCN_ALIGN_8192BYTES:
        {
            s.append("IMAGE_SCN_ALIGN_8192BYTES");
            break;
        }
        }
    }
    if ((characteristics & IMAGE_SCN_LNK_NRELOC_OVFL) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_LNK_NRELOC_OVFL");
    }
    if ((characteristics & IMAGE_SCN_MEM_DISCARDABLE) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_DISCARDABLE");
    }
    if ((characteristics & IMAGE_SCN_MEM_NOT_CACHED) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_NOT_CACHED");
    }
    if ((characteristics & IMAGE_SCN_MEM_NOT_PAGED) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_NOT_PAGED");
    }
    if ((characteristics & IMAGE_SCN_MEM_SHARED) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_SHARED");
    }
    if ((characteristics & IMAGE_SCN_MEM_EXECUTE) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_EXECUTE");
    }
    if ((characteristics & IMAGE_SCN_MEM_READ) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_READ");
    }
    if ((characteristics & IMAGE_SCN_MEM_WRITE) != 0u)
    {
        if (!s.empty())
        {
            s.append(" | ");
        }
        s.append("IMAGE_SCN_MEM_WRITE");
    }
    return s;
}

std::string RelocationTypeStr(uint16_t type)
{
    switch (type)
    {
        case IMAGE_REL_AMD64_ABSOLUTE:
        {
            return "IMAGE_REL_AMD64_ABSOLUTE";
        }
        case IMAGE_REL_AMD64_ADDR64:
        {
            return "IMAGE_REL_AMD64_ADDR64";
        }
        case IMAGE_REL_AMD64_ADDR32:
        {
            return "IMAGE_REL_AMD64_ADDR32";
        }
        case IMAGE_REL_AMD64_ADDR32NB:
        {
            return "IMAGE_REL_AMD64_ADDR32NB";
        }
        case IMAGE_REL_AMD64_REL32:
        {
            return "IMAGE_REL_AMD64_REL32";
        }
        case IMAGE_REL_AMD64_REL32_1:
        {
            return "IMAGE_REL_AMD64_REL32_1";
        }
        case IMAGE_REL_AMD64_REL32_2:
        {
            return "IMAGE_REL_AMD64_REL32_2";
        }
        case IMAGE_REL_AMD64_REL32_3:
        {
            return "IMAGE_REL_AMD64_REL32_3";
        }
        case IMAGE_REL_AMD64_REL32_4:
        {
            return "IMAGE_REL_AMD64_REL32_4";
        }
        case IMAGE_REL_AMD64_REL32_5:
        {
            return "IMAGE_REL_AMD64_REL32_5";
        }
        case IMAGE_REL_AMD64_SECTION:
        {
            return "IMAGE_REL_AMD64_SECTION";
        }
        case IMAGE_REL_AMD64_SECREL:
        {
            return "IMAGE_REL_AMD64_SECREL";
        }
        case IMAGE_REL_AMD64_SECREL7:
        {
            return "IMAGE_REL_AMD64_SECREL7";
        }
        case IMAGE_REL_AMD64_TOKEN:
        {
            return "IMAGE_REL_AMD64_TOKEN";
        }
        case IMAGE_REL_AMD64_SREL32:
        {
            return "IMAGE_REL_AMD64_SREL32";
        }
        case IMAGE_REL_AMD64_PAIR:
        {
            return "IMAGE_REL_AMD64_PAIR";
        }
        case IMAGE_REL_AMD64_SSPAN32:
        {
            return "IMAGE_REL_AMD64_SSPAN32";
        }
    }
    return std::string();
}

std::string GetSectionNumberStr(int16_t sectionNumber)
{
    switch (sectionNumber)
    {
        case IMAGE_SYM_UNDEFINED: return "0 IMAGE_SYM_UNDEFINED";
        case IMAGE_SYM_ABSOLUTE: return "-1 IMAGE_SYM_ABSOLUTE";
        case IMAGE_SYM_DEBUG: return "-2 IMAGE_SYM_DEBUG";
    }
    return std::to_string(sectionNumber);
}

std::string GetTypeStr(int16_t type)
{
    std::string str;
    int8_t lsb = (type >> 8) & 0xFF;
    int8_t msb = type & 0xFF;
    switch (lsb)
    {
        case IMAGE_SYM_TYPE_NULL:
        {
            str.append("00 IMAGE_SYM_TYPE_NULL");
            break;
        }
    }
    switch (msb)
    {
        case IMAGE_SYM_DTYPE_NULL:
        {
            if (!str.empty())
            {
                str.append(1, ' ');
            }
            str.append("00 IMAGE_SYM_DTYPE_NULL");
            break;
        }
        case IMAGE_SYM_DTYPE_FUNCTION:
        {
            if (!str.empty())
            {
                str.append(1, ' ');
            }
            str.append("20 IMAGE_SYM_DTYPE_FUNCTION");
            break;
        }
    }
    return str;
}

std::string GetStorageClassStr(uint8_t storageClass)
{
    switch (storageClass)
    {
        case IMAGE_SYM_CLASS_NULL:
        {
            return "IMAGE_SYM_CLASS_NULL";
        }
        case IMAGE_SYM_CLASS_EXTERNAL:
        {
            return "IMAGE_SYM_CLASS_EXTERNAL";
        }
        case IMAGE_SYM_CLASS_STATIC:
        {
            return "IMAGE_SYM_CLASS_STATIC";
        }
        case IMAGE_SYM_CLASS_LABEL:
        {
            return "IMAGE_SYM_CLASS_LABEL";
        }
        case IMAGE_SYM_CLASS_FUNCTION:
        {
            return "IMAGE_SYM_CLASS_FUNCTION";
        }
        case IMAGE_SYM_CLASS_FILE:
        {
            return "IMAGE_SYM_CLASS_FILE";
        }
    }
    return std::string();
}

std::string GetSelectionStr(uint8_t selection)
{
    switch (selection)
    {
        case IMAGE_COMDAT_SELECT_NODUPLICATES:
        {
            return "IMAGE_COMDAT_SELECT_NODUPLICATES";
        }
        case IMAGE_COMDAT_SELECT_ANY:
        {
            return "IMAGE_COMDAT_SELECT_ANY";
        }
        case IMAGE_COMDAT_SELECT_SAME_SIZE:
        {
            return "IMAGE_COMDAT_SELECT_SAME_SIZE";
        }
        case IMAGE_COMDAT_SELECT_EXACT_MATCH:
        {
            return "IMAGE_COMDAT_SELECT_EXACT_MATCH";
        }
        case IMAGE_COMDAT_SELECT_ASSOCIATIVE:
        {
            return "IMAGE_COMDAT_SELECT_ASSOCIATIVE";
        }
        case IMAGE_COMDAT_SELECT_LARGEST:
        {
            return "IMAGE_COMDAT_SELECT_LARGEST";
        }
    }
    return std::string();
}

CoffHeader::CoffHeader() : 
    machine(IMAGE_FILE_MACHINE_AMD64), numberOfSections(0), timestamp(std::time(nullptr)), symbolTableOffset(0), numberOfSymbols(0), sizeOfOptionalHeader(0), characteristics()
{
}

CoffHeader::CoffHeader(util::LittleEndianMemoryReader& reader) : 
    machine(), numberOfSections(), timestamp(), symbolTableOffset(), numberOfSymbols(), sizeOfOptionalHeader(), characteristics()
{
    machine = reader.ReadUShort();
    if (machine != IMAGE_FILE_MACHINE_AMD64)
    {
        throw std::runtime_error("IMAGE_FILE_MACHINE_AMD64 expected");
    }
    numberOfSections = reader.ReadUShort();
    timestamp = reader.ReadTimeStamp();
    symbolTableOffset = reader.ReadUInt();
    numberOfSymbols = reader.ReadUInt();
    sizeOfOptionalHeader = reader.ReadUShort();
    characteristics = reader.ReadUShort();
}

void CoffHeader::Dump(util::CodeFormatter& formatter)
{
    formatter << "COFF header" << "\n";
    formatter << "-----------" << "\n";
    formatter << "\n";
    int titleWidth = 22;
    std::string machineTitle = util::Format("machine", titleWidth, util::FormatJustify::right);
    std::string machineStr = "x64 (" + util::ToHexString(machine) + ")";
    formatter << machineTitle << " : " << machineStr << "\n";
    std::string numSectionsTitle = util::Format("number of sections", titleWidth, util::FormatJustify::right);
    std::string numSectionsStr = util::ToHexString(numberOfSections);
    formatter << numSectionsTitle << " : " << numSectionsStr << "\n";
    std::string timestampTitle = util::Format("timestamp", titleWidth, util::FormatJustify::right);
    std::string timestampStr = util::ToHexString(static_cast<uint32_t>(timestamp));
    formatter << timestampTitle << " : " << timestampStr << "\n";
    std::string symbolTableOffsetTitle = util::Format("symbol table offset", titleWidth, util::FormatJustify::right);
    std::string symbolTableOffsetStr = util::ToHexString(symbolTableOffset);
    formatter << symbolTableOffsetTitle << " : " << symbolTableOffsetStr << "\n";
    std::string optionalHeaderSizeTitle = util::Format("optional header size", titleWidth, util::FormatJustify::right);
    std::string optionalHeaderSizeStr = util::ToHexString(sizeOfOptionalHeader);
    formatter << optionalHeaderSizeTitle << " : " << optionalHeaderSizeStr << "\n";
    std::string characteristicsTitle = util::Format("characteristics", titleWidth, util::FormatJustify::right);
    std::string characteristicsStr = util::ToHexString(characteristics);
    formatter << characteristicsTitle << " : " << characteristicsStr << "\n";
    formatter << "\n";
}

void CoffHeader::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    writer.Write(static_cast<uint16_t>(machine));
    writer.Write(static_cast<uint16_t>(file->Sections().size()));
    uint32_t ts = static_cast<uint32_t>(timestamp);
    writer.Write(static_cast<uint32_t>(ts));
    positions.SetSymbolTablePosition(writer.Position());
    writer.Write(static_cast<uint32_t>(0));
    writer.Write(static_cast<uint32_t>(file->GetSymbolTable()->GetNumberOfEntries()));
    writer.Write(static_cast<uint16_t>(sizeOfOptionalHeader));
    writer.Write(static_cast<uint16_t>(characteristics));
}

SectionHeader::SectionHeader() : 
    number(1), name(), virtualSize(), virtualAddress(), sizeOfRawData(), rawDataOffset(), relocationsOffset(), lineNumbersOffset(), numberOfRelocations(),
    numberOfLineNumbers(), characteristics()
{
    for (int i = 0; i < 8; ++i)
    {
        name[i] = static_cast<char>(0);
    }
}

SectionHeader::SectionHeader(util::LittleEndianMemoryReader& reader) : 
    number(1), name(), virtualSize(), virtualAddress(), sizeOfRawData(), rawDataOffset(), relocationsOffset(), lineNumbersOffset(), numberOfRelocations(),
    numberOfLineNumbers(), characteristics()
{
    for (int i = 0; i < 8; ++i)
    {
        name[i] = reader.ReadChar();
    }
    virtualSize = reader.ReadInt();
    virtualAddress = reader.ReadInt();
    sizeOfRawData = reader.ReadUInt();
    rawDataOffset = reader.ReadUInt();
    relocationsOffset = reader.ReadUInt();
    lineNumbersOffset = reader.ReadUInt();
    numberOfRelocations = reader.ReadUShort();
    numberOfLineNumbers = reader.ReadUShort();
    characteristics = reader.ReadUInt();
}

void SectionHeader::SetName(const std::string& name_, CoffObjectFile* file)
{
    for (int i = 0; i < 8; ++i)
    {
        name[i] = static_cast<char>(0);
    }
    if (name_.length() <= 8)
    {
        for (int i = 0; i < name_.length(); ++i)
        {
            name[i] = name_[i];
        }
    }
    else
    {
        name[0] = '/';
        uint32_t n = file->GetStringTable()->AddString(name_);
        std::string ns = std::to_string(n);
        for (int i = 0; i < ns.length(); ++i)
        {
            name[i + 1] = ns[i];
        }
    }
}

void SectionHeader::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    formatter << "Section " << util::ToHexString(number) << "\n";
    formatter << "------------" << "\n";
    formatter << "\n";
    int titleWidth = 22;
    nameStr.clear();
    if (name[0] == '/')
    {
        std::string ns;
        for (int i = 1; i < 8; ++i)
        {
            if (name[i] == '\0') break;
            ns.append(1, name[i]);
        }
        uint32_t offset = std::stoi(ns);
        nameStr = file->GetStringTable()->GetString(offset);
    }
    else
    {
        for (char c : name)
        {
            if (c == '\0') break;
            nameStr.append(1, c);
        }
    }
    std::string nameTitle = util::Format("name", titleWidth, util::FormatJustify::right);
    formatter << nameTitle << " : " << nameStr << "\n";
    std::string rawDataSizeTitle = util::Format("size of raw data", titleWidth, util::FormatJustify::right);
    std::string rawDataSizeStr = util::ToHexString(sizeOfRawData);
    formatter << rawDataSizeTitle << " : " << rawDataSizeStr << "\n";
    std::string rawDataOffsetTitle = util::Format("raw data offset", titleWidth, util::FormatJustify::right);
    std::string rawDataOffsetStr = util::ToHexString(rawDataOffset);
    formatter << rawDataOffsetTitle << " : " << rawDataOffsetStr << "\n";
    std::string relocationsOffsetTitle = util::Format("relocations offset", titleWidth, util::FormatJustify::right);
    std::string relocationsOffsetStr = util::ToHexString(relocationsOffset);
    formatter << relocationsOffsetTitle << " : " << relocationsOffsetStr << "\n";
    std::string lineNumbersOffsetTitle = util::Format("line numberes offset", titleWidth, util::FormatJustify::right);
    std::string lineNumbersOffsetStr = util::ToHexString(lineNumbersOffset);
    formatter << lineNumbersOffsetTitle << " : " << lineNumbersOffsetStr << "\n";
    std::string numberOfRelocationsTitle = util::Format("number of relocations", titleWidth, util::FormatJustify::right);
    std::string numberOfRelocationsStr = util::ToHexString(numberOfRelocations);
    formatter << numberOfRelocationsTitle << " : " << numberOfRelocationsStr << "\n";
    std::string numberOfLineNumbersTitle = util::Format("number of line numbers", titleWidth, util::FormatJustify::right);
    std::string numberOfLineNumbersStr = util::ToHexString(numberOfLineNumbers);
    formatter << numberOfLineNumbersTitle << " : " << numberOfLineNumbersStr << "\n";
    std::string characteristicsTitle = util::Format("characteristics", titleWidth, util::FormatJustify::right);
    std::string characteristicsStr = SectionHeaderCharacteristicsStr(characteristics);
    formatter << characteristicsTitle << " : " << characteristicsStr << "\n";
    formatter << "\n";
}

void SectionHeader::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    for (int i = 0; i < 8; ++i)
    {
        writer.Write(static_cast<uint8_t>(name[i]));
    }
    writer.Write(static_cast<uint32_t>(0)); // virtualSize
    writer.Write(static_cast<uint32_t>(0)); // VirtualAddress
    Section* section = file->GetSection(number);
    if (section)
    {
        writer.Write(static_cast<uint32_t>(section->Data().size()));
    }
    else
    {
        throw std::runtime_error("invalid section number");
    }
    int64_t rawDataPosition = writer.Position();
    positions.SetRawDataPosition(number, rawDataPosition);
    writer.Write(static_cast<uint32_t>(0)); // rawDataOffset
    int64_t relocationsPosition = writer.Position();
    positions.SetRelocationsPosition(number, relocationsPosition);
    writer.Write(static_cast<uint32_t>(0)); // relocationsOffset
    int64_t lineNumbersPosition = writer.Position();
    positions.SetLineNumbersPosition(number, lineNumbersPosition);
    writer.Write(static_cast<uint32_t>(0)); // lineNumbersOffset
    writer.Write(static_cast<uint16_t>(section->GetRelocationTable()->Relocations().size())); // numberOfRelocations
    writer.Write(static_cast<uint16_t>(0)); // numberOfLineNumbers
    writer.Write(static_cast<uint32_t>(characteristics));
}

Relocation::Relocation() : virtualAddress(), symbolTableIndex(), type()
{
}

Relocation::Relocation(uint32_t virtualAddress_, uint32_t symbolTableIndex_, uint16_t type_) : 
    virtualAddress(virtualAddress_), symbolTableIndex(symbolTableIndex_), type(type_)
{
}

Relocation::Relocation(util::LittleEndianMemoryReader& reader) : virtualAddress(), symbolTableIndex(), type()
{
    virtualAddress = reader.ReadUInt();
    symbolTableIndex = reader.ReadUInt();
    type = reader.ReadUShort();
}

void Relocation::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    int titleWidth = 22;
    std::string addressTitle = util::Format("address", titleWidth, util::FormatJustify::right);
    std::string addressStr = util::ToHexString(virtualAddress);
    formatter << addressTitle << " : " << addressStr << "\n";
    std::string symbolTitle = util::Format("symbol", titleWidth, util::FormatJustify::right);
    std::string symbolStr = util::ToHexString(symbolTableIndex);
    SymbolTableEntry* entry = file->GetSymbolTable()->GetEntry(symbolTableIndex);
    if (entry)
    {
        std::string name = entry->GetName(file);
        if (!name.empty())
        {
            symbolStr.append(1, ' ').append(name);
        }
    }
    formatter << symbolTitle << " : " << symbolStr << "\n";
    std::string typeTitle = util::Format("type", titleWidth, util::FormatJustify::right);
    std::string typeStr = util::ToHexString(type);
    std::string typeName = RelocationTypeStr(type);
    if (!typeName.empty())
    {
        typeStr.append(1, ' ').append(typeName);

    }
    formatter << typeTitle << " : " << typeStr << "\n";
}

void Relocation::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    writer.Write(static_cast<uint32_t>(virtualAddress));
    writer.Write(static_cast<uint32_t>(symbolTableIndex));
    writer.Write(static_cast<uint16_t>(type));
}

RelocationTable::RelocationTable() : relocations()
{
}

RelocationTable::RelocationTable(util::LittleEndianMemoryReader& reader, uint16_t numberOfRelocations) : relocations()
{
    for (uint16_t i = 0; i < numberOfRelocations; ++i)
    {
        Relocation relocation(reader);
        relocations.push_back(relocation);
    }
}

void RelocationTable::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    formatter << "Relocations" << "\n";
    formatter << "-----------" << "\n";
    formatter << "\n";
    int titleWidth = 22;
    for (auto& relocation : relocations)
    {
        relocation.Dump(formatter, file);
        formatter << "\n";
    }
}

void RelocationTable::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    for (auto& relocation : relocations)
    {
        relocation.Write(writer, file, positions);
    }
}

Section::Section() : sectionHeader(new SectionHeader()), relocationTable(new RelocationTable()), data(), checkSum(0u)
{
}

Section::Section(util::LittleEndianMemoryReader& reader, ReadFlags readFlags) : sectionHeader(), relocationTable(), data(), checkSum(0u)
{
    sectionHeader.reset(new SectionHeader(reader));
    if ((readFlags & ReadFlags::readRelocations) != ReadFlags::none)
    {
        util::LittleEndianMemoryReader relocationTableReader(reader.Ptr() + sectionHeader->RelocationsOffset(), sectionHeader->NumberOfRelocations() * Relocation::Size());
        relocationTable.reset(new RelocationTable(relocationTableReader, sectionHeader->NumberOfRelocations()));
    }
    if ((readFlags & ReadFlags::readSectionData) != ReadFlags::none)
    {
        util::LittleEndianMemoryReader dataReader(reader.Ptr() + sectionHeader->RawDataOffset(), sectionHeader->SizeOfRawData());
        data = dataReader.ReadBlock(sectionHeader->SizeOfRawData());
        ComputeCheckSum();
    }
}

void Section::Dump(util::CodeFormatter& formatter, CoffObjectFile* file, DumpFlags flags)
{
    if (sectionHeader)
    {
        sectionHeader->Dump(formatter, file);
    }
    if ((flags & DumpFlags::dumpSectionData) != DumpFlags::none)
    {
        formatter << "Raw data:" << "\n";
        formatter << "---------" << "\n";
        std::string lineChars;
        for (uint64_t i = 0u; i < data.size(); ++i)
        {
            if (i > 0)
            {
                if ((i & 0x0Fu) == 0x00u)
                {
                    formatter << "  " << lineChars;
                    lineChars.clear();
                    formatter << "\n";
                }
                else
                {
                    formatter << " ";
                }
            }
            uint8_t x = data[i];
            if (x >= 32 && x < 128)
            {
                char c = static_cast<char>(x);
                lineChars.append(util::CharStr(c));
            }
            else
            {
                lineChars.append(1, '.');
            }
            formatter << util::ToHexString(x);
        }
        if (!lineChars.empty())
        {
            int n = 15 - (data.size() & 0x0Fu) + 1;
            for (int i = 0; i < n; ++i)
            {
                formatter << "   ";
            }
            formatter << "  " << lineChars;
        }
        formatter << "\n";
        if (!data.empty())
        {
            formatter << "\n";
            int titleWidth = 22;
            std::string checkSumTitle = util::Format("computed check sum", titleWidth, util::FormatJustify::right);
            std::string checkSumStr = util::ToHexString(checkSum);
            formatter << checkSumTitle << " : " << checkSumStr << "\n";
            formatter << "\n";
        }
    }
    if (relocationTable)
    {
        if ((flags & DumpFlags::dumpRelocations) != DumpFlags::none)
        {
            relocationTable->Dump(formatter, file);
        }
    }
    formatter << "\n";
}

void Section::ComputeCheckSum() 
{
    checkSum = util::Crc32(data);
}

void Section::WriteHeader(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    sectionHeader->Write(writer, file, positions);
}

void Section::WriteData(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    if (data.empty()) return;
    int64_t position = writer.Position();
    util::LittleEndianMemoryWriter rawDataOffsetWriter(positions.Data() + positions.GetRawDataPosition(sectionHeader->Number()), 4u);
    uint32_t rawDataOffset = static_cast<uint32_t>(position);
    rawDataOffsetWriter.Write(static_cast<uint32_t>(rawDataOffset));
    for (uint8_t b : data)
    {
        writer.Write(static_cast<uint8_t>(b));
    }
}

void Section::WriteRelocationTable(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    if (relocationTable->Relocations().empty()) return;
    int64_t position = writer.Position();
    util::LittleEndianMemoryWriter relocationsOffsetWriter(positions.Data() + positions.GetRelocationsPosition(sectionHeader->Number()), 4u);
    uint32_t relocationsOffset = static_cast<uint32_t>(position);
    relocationsOffsetWriter.Write(static_cast<uint32_t>(relocationsOffset));
    relocationTable->Write(writer, file, positions);
}

SymbolNameLocation::SymbolNameLocation() : zeros(0), offset()
{
}

SymbolNameLocation::SymbolNameLocation(uint32_t offset_) : zeros(0), offset(offset_)
{
}

SymbolTableEntry::SymbolTableEntry() : index(1), shortName(), value(), sectionNumber(), type(), storageClass(), numberOfAuxSymbols()
{
    for (int i = 0; i < 8; ++i)
    {
        shortName[i] = static_cast<char>(0);
    }
}

SymbolTableEntry::SymbolTableEntry(util::LittleEndianMemoryReader& reader) : index(1), shortName(), value(), sectionNumber(), type(), storageClass(), numberOfAuxSymbols()
{
    for (int i = 0; i < 8; ++i)
    {
        shortName[i] = reader.ReadChar();
    }
    value = reader.ReadUInt();
    sectionNumber = reader.ReadShort();
    type = reader.ReadShort();
    storageClass = reader.ReadByte();
    numberOfAuxSymbols = reader.ReadByte();
}

bool SymbolTableEntry::IsSectionDefinition() const
{
    if (storageClass == IMAGE_SYM_CLASS_STATIC && value == 0u)
    {
        return true;
    }
    return false;
}

std::string SymbolTableEntry::GetName(CoffObjectFile* file) const
{
    if (location.InStringTable())
    {
        return file->GetStringTable()->GetString(location.Offset());
    }
    else
    {
        std::string name;
        for (int i = 0; i < 8; ++i)
        {
            if (shortName[i] == '\0') break;
            name.append(1, shortName[i]);
        }
        return name;
    }
}

void SymbolTableEntry::SetName(const std::string& name, CoffObjectFile* file)
{
    for (int i = 0; i < 8; ++i)
    {
        shortName[i] = static_cast<char>(0);
    }
    if (name.length() <= 8)
    {
        for (int i = 0; i < name.length(); ++i)
        {
            shortName[i] = name[i];
        }
    }
    else
    {
        location = SymbolNameLocation(file->GetStringTable()->AddString(name));
    }
}

void SymbolTableEntry::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    int titleWidth = 22;
    formatter << "SYMBOL TABLE ENTRY" << "\n";
    formatter << "------------------" << "\n";
    std::string indexTitle = util::Format("index", titleWidth, util::FormatJustify::right);
    std::string indexStr = util::ToHexString(index);
    formatter << indexTitle << " : " << indexStr << "\n";
    std::string nameTitle = util::Format("name", titleWidth, util::FormatJustify::right);
    std::string nameStr = GetName(file);
    formatter << nameTitle << " : " << nameStr << "\n";
    std::string valueTitle = util::Format("value", titleWidth, util::FormatJustify::right);
    std::string valueStr = util::ToHexString(value);
    formatter << valueTitle << " : " << valueStr << "\n";
    std::string sectionNumberTitle = util::Format("section number", titleWidth, util::FormatJustify::right);
    std::string sectionNumberStr = GetSectionNumberStr(sectionNumber);
    formatter << sectionNumberTitle << " : " << sectionNumberStr << "\n";
    std::string typeTitle = util::Format("type", titleWidth, util::FormatJustify::right);
    std::string typeStr = util::ToHexString(uint16_t(type)) + " " + GetTypeStr(type);
    formatter << typeTitle << " : " << typeStr << "\n";
    std::string storageClassTitle = util::Format("storage class", titleWidth, util::FormatJustify::right);
    std::string storageClassStr = util::ToHexString(storageClass) + " " + GetStorageClassStr(storageClass);
    formatter << storageClassTitle << " : " << storageClassStr << "\n";
    std::string numberOfAuxSymbolsTitle = util::Format("number of aux symbols", titleWidth, util::FormatJustify::right);
    std::string numberOfAuxSymbolStr = util::ToHexString(numberOfAuxSymbols);
    formatter << numberOfAuxSymbolsTitle << " : " << numberOfAuxSymbolStr << "\n";
    if (auxSectionDefinitionEntry)
    {
        std::string auxSectionEntryTitle = util::Format("aux entry follows", titleWidth, util::FormatJustify::right);
        formatter << auxSectionEntryTitle << "\n";
        formatter << "\n";
        auxSectionDefinitionEntry->Dump(formatter, file);
    }
    formatter << "\n";
}

void SymbolTableEntry::SetAuxSectionDefinitionEntry(AuxSectionDefinitionSymbolTableEntry* auxSectionDefinitionEntry_)
{
    auxSectionDefinitionEntry.reset(auxSectionDefinitionEntry_);
}

void SymbolTableEntry::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    for (int i = 0; i < 8; ++i)
    {
        writer.Write(static_cast<uint8_t>(shortName[i]));
    }
    writer.Write(static_cast<uint32_t>(value));
    writer.Write(static_cast<int16_t>(sectionNumber));
    writer.Write(static_cast<int16_t>(type));
    writer.Write(static_cast<uint8_t>(storageClass));
    writer.Write(static_cast<uint8_t>(numberOfAuxSymbols));
    if (numberOfAuxSymbols == 1 && auxSectionDefinitionEntry)
    {
        auxSectionDefinitionEntry->Write(writer, file, positions);
    }
}

AuxSectionDefinitionSymbolTableEntry::AuxSectionDefinitionSymbolTableEntry() : 
    index(1), length(), numberOfRelocations(), numberOfLineNumbers(), checkSum(), number(), selection()
{
}

AuxSectionDefinitionSymbolTableEntry::AuxSectionDefinitionSymbolTableEntry(util::LittleEndianMemoryReader& reader) :
    index(1), length(), numberOfRelocations(), numberOfLineNumbers(), checkSum(), number(), selection()
{
    length = reader.ReadUInt();
    numberOfRelocations = reader.ReadUShort();
    numberOfLineNumbers = reader.ReadUShort();
    checkSum = reader.ReadUInt();
    number = reader.ReadUShort();
    selection = reader.ReadByte();
}

void AuxSectionDefinitionSymbolTableEntry::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    int titleWidth = 22;
    formatter << "AUX SECTION DEFINITION ENTRY" << "\n";
    formatter << "----------------------------" << "\n";
    std::string indexTitle = util::Format("index", titleWidth, util::FormatJustify::right);
    std::string indexStr = util::ToHexString(index);
    formatter << indexTitle << " : " << indexStr << "\n";
    std::string lengthTitle = util::Format("length", titleWidth, util::FormatJustify::right);
    std::string lengthStr = util::ToHexString(length);
    formatter << lengthTitle << " : " << lengthStr << "\n";
    std::string numberOfRelocationsTitle = util::Format("number of relocations", titleWidth, util::FormatJustify::right);
    std::string numberOfReloacationsStr = util::ToHexString(numberOfRelocations);
    formatter << numberOfRelocationsTitle << " : " << numberOfReloacationsStr << "\n";
    std::string numberOfLineNumbersTitle = util::Format("number of line numbers", titleWidth, util::FormatJustify::right);
    std::string numberOfLineNumbersStr = util::ToHexString(numberOfLineNumbers);
    formatter << numberOfLineNumbersTitle << " : " << numberOfLineNumbersStr << "\n";
    std::string checkSumTitle = util::Format("check sum", titleWidth, util::FormatJustify::right);
    std::string checkSumStr = util::ToHexString(checkSum);
    formatter << checkSumTitle << " : " << checkSumStr << "\n";
    std::string numberTitle = util::Format("section number", titleWidth, util::FormatJustify::right);
    std::string numberStr = util::ToHexString(number);
    formatter << numberTitle << " : " << numberStr << "\n";
    std::string selectionTitle = util::Format("selection", titleWidth, util::FormatJustify::right);
    std::string selectionStr = util::ToHexString(selection) + " " + GetSelectionStr(selection);
    formatter << selectionTitle << " : " << selectionStr << "\n";
}

void AuxSectionDefinitionSymbolTableEntry::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    writer.Write(static_cast<uint32_t>(length));
    writer.Write(static_cast<uint16_t>(numberOfRelocations));
    writer.Write(static_cast<uint16_t>(numberOfLineNumbers));
    writer.Write(static_cast<uint32_t>(checkSum));
    writer.Write(static_cast<uint16_t>(number));
    writer.Write(static_cast<uint8_t>(selection));
    for (int i = 0; i < 3; ++i)
    {
        writer.Write(static_cast<uint8_t>(0));
    }
}

SymbolTable::SymbolTable() : entries(), nextEntryNumber(1), numberOfEntries(0)
{
}

SymbolTable::SymbolTable(util::LittleEndianMemoryReader& reader, uint32_t numberOfSymbols) : entries(), nextEntryNumber(1), numberOfEntries(0)
{
    int64_t count = numberOfSymbols * SymbolTableEntry::Size();
    util::LittleEndianMemoryReader symbolTableReader(reader.Pos(), count);
    for (uint32_t i = 0u; i < numberOfSymbols; ++i)
    {
        std::unique_ptr<SymbolTableEntry> entry(new SymbolTableEntry(symbolTableReader));
        entry->SetIndex(i + 1);
        uint8_t numAuxSymbols = entry->NumberOfAuxSymbols();
        if (entry->IsSectionDefinition())
        {
            if (numAuxSymbols == 1)
            {
                util::LittleEndianMemoryReader auxSectionDefinitionEntryReader(symbolTableReader.Pos(), AuxSectionDefinitionSymbolTableEntry::Size());
                std::unique_ptr<AuxSectionDefinitionSymbolTableEntry> auxSectionDefinitionEntry(new AuxSectionDefinitionSymbolTableEntry(auxSectionDefinitionEntryReader));
                entry->SetAuxSectionDefinitionEntry(auxSectionDefinitionEntry.release());
                entry->GetAuxSectionDefinitionEntry()->SetIndex(i + 2);
            }
            else
            {
                int x = 0;
            }
        }
        entries.push_back(std::move(entry));
        if (numAuxSymbols > 0)
        {
            symbolTableReader.SetPos(symbolTableReader.Pos() + numAuxSymbols * SymbolTableEntry::Size());
            i += numAuxSymbols;
            for (uint8_t a = 0; a < numAuxSymbols; ++a)
            {
                entries.push_back(std::unique_ptr<SymbolTableEntry>());
            }
        }
    }
    reader.SetPos(symbolTableReader.Pos());
}

SymbolTableEntry* SymbolTable::GetEntry(uint32_t symbolIndex) const
{
    if (symbolIndex >= 1u && symbolIndex <= entries.size())
    {
        return entries[symbolIndex - 1].get();
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::Dump(util::CodeFormatter& formatter, CoffObjectFile* file)
{
    formatter << "Symbol table " << "\n";
    formatter << "------------" << "\n";
    formatter << "\n";
    int titleWidth = 22;
    for (int i = 0; i < entries.size(); ++i)
    {
        SymbolTableEntry* entry = entries[i].get();
        if (entry)
        {
            entry->Dump(formatter, file);
        }
    }
}

void SymbolTable::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    int64_t position = writer.Position();
    util::LittleEndianMemoryWriter symbolTableOffsetWriter(positions.Data() + positions.SymbolTablePosition(), 4u);
    uint32_t symbolTableOffset = static_cast<uint32_t>(position);
    symbolTableOffsetWriter.Write(static_cast<uint32_t>(symbolTableOffset));
    for (const auto& entry : entries)
    {
        entry->Write(writer, file, positions);
    }
}

StringTable::StringTable() : size(4), offsetStringMap(), stringOffsetMap()
{
}

StringTable::StringTable(util::LittleEndianMemoryReader& reader) : size(), offsetStringMap(), stringOffsetMap()
{
    size = reader.ReadInt();
    int64_t count = std::max(0, size - 4);
    util::LittleEndianMemoryReader stringTableReader(reader.Pos(), count);
    const uint8_t* pos = stringTableReader.Pos();
    const uint8_t* start = pos;
    const uint8_t* end = start + count;
    while (pos < end)
    {
        uint32_t offset = static_cast<uint32_t>(pos - start + 4);
        std::string s = stringTableReader.ReadString();
        offsetStringMap[offset] = s;
        pos = stringTableReader.Pos();
    }
    reader.SetPos(reader.Pos() + count);
}

std::string StringTable::GetString(uint32_t offset) const
{
    auto it = offsetStringMap.find(offset);
    if (it != offsetStringMap.end())
    {
        return it->second;
    }
    return std::string();
}

uint32_t StringTable::AddString(const std::string& s)
{
    auto it = stringOffsetMap.find(s);
    if (it != stringOffsetMap.end())
    {
        return it->second;
    }
    uint32_t offset = size;
    stringOffsetMap[s] = offset;
    strings.push_back(s);
    size += static_cast<uint32_t>(s.length()) + 1u;
    return offset;
}

void StringTable::Write(util::LitteEndianBinaryStreamWriter& writer, CoffObjectFile* file, Positions& positions)
{
    for (const auto& s : strings)
    {
        writer.Write(s, true);
    }
}

CoffObjectFile::CoffObjectFile() : coffHeader(new CoffHeader()), sections(), symbolTable(new SymbolTable()), stringTable(new StringTable())
{
}

CoffObjectFile::CoffObjectFile(const std::string& objectFilePath, ReadFlags readFlags) : coffHeader(), sections(), symbolTable(), stringTable()
{
    std::vector<uint8_t> content = util::ReadBinaryFile(objectFilePath);
    const uint8_t* ptr = content.data();
    int64_t count = content.size();
    util::LittleEndianMemoryReader reader(ptr, count);
    util::LittleEndianMemoryReader coffHeaderReader(ptr, CoffHeader::Size());
    coffHeader.reset(new CoffHeader(coffHeaderReader));
    const uint8_t* pos = coffHeaderReader.Pos() + coffHeader->SizeOfOptionalHeader();
    reader.SetPos(pos);
    for (uint16_t i = 0u; i < coffHeader->NumberOfSections(); ++i)
    {
        std::unique_ptr<Section> section(new Section(reader, readFlags));
        section->GetSectionHeader()->SetNumber(i + 1);
        sections.push_back(std::move(section));
    }
    reader.SetPos(ptr + coffHeader->SymbolTableOffset());
    symbolTable.reset(new SymbolTable(reader, coffHeader->NumberOfSymbols()));
    stringTable.reset(new StringTable(reader));
}

void CoffObjectFile::Dump(util::CodeFormatter& formatter, DumpFlags flags)
{
    if (coffHeader)
    {
        coffHeader->Dump(formatter);
    }
    for (const auto& section : sections)
    {
        section->Dump(formatter, this, flags);
    }
    if (symbolTable)
    {
        symbolTable->Dump(formatter, this);
    }
}

void CoffObjectFile::Write(const std::string& objectFilePath)
{
    util::MemoryStream memoryStream;
    util::LitteEndianBinaryStreamWriter writer(memoryStream);
    Positions positions(memoryStream);
    coffHeader->Write(writer, this, positions);
    for (const auto& section : sections)
    {
        section->WriteHeader(writer, this, positions);
    }
    for (const auto& section : sections)
    {
        section->WriteData(writer, this, positions);
        section->WriteRelocationTable(writer, this, positions);
    }
    symbolTable->Write(writer, this, positions);
    stringTable->Write(writer, this, positions);
    util::FileStream fileStream(objectFilePath, util::OpenMode::binary | util::OpenMode::write);
    memoryStream.CopyTo(fileStream);
}

void CoffObjectFile::AddSection(Section* section)
{
    sections.push_back(std::unique_ptr<Section>(section));
}

Section* CoffObjectFile::GetSection(uint16_t sectionNumber) const
{
    if (sectionNumber > 0)
    {
        if (sectionNumber - 1 < sections.size())
        {
            return sections[sectionNumber - 1].get();
        }
    }
    return nullptr;
}

} // namespace machine::coff
