// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.type.table;

import cmajor.systemx.object.binary.file;
import cmajor.systemx.object.debug;
import util;

namespace cmajor::systemx::object {

TypeTableIndexEntry::TypeTableIndexEntry() : typeId(-1), address(0)
{
}

void TypeTableIndexEntry::Write(Section* section)
{
    section->Align(8);
    section->EmitTetra(typeId);
    section->Align(8);
    section->EmitOcta(address);
    section->Align(8);
}

int64_t TypeTableIndexEntry::Read(int64_t addr, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable)
{
    addr = util::Align(addr, 8);
    typeId = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    addr = util::Align(addr, 8);
    address = memory.ReadOcta(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 8;
    addr = util::Align(addr, 8);
    return addr;
}

bool operator<(const TypeTableIndexEntry& left, const TypeTableIndexEntry& right)
{
    return left.typeId < right.typeId;
}

TypeTableIndex::TypeTableIndex()
{
}

void TypeTableIndex::AddEntry(const TypeTableIndexEntry& entry)
{
    entries.push_back(entry);
}

void TypeTableIndex::Sort()
{
    std::sort(entries.begin(), entries.end());
}

void TypeTableIndex::Write(BinaryFile& binaryFile)
{
    binaryFile.GetDataSection()->Align(8);
    int64_t start = binaryFile.GetDataSection()->BaseAddress() + binaryFile.GetDataSection()->Address();
    for (auto& entry : entries)
    {
        entry.Write(binaryFile.GetDataSection());
    }
    binaryFile.GetDataSection()->Align(8);
    int64_t end = binaryFile.GetDataSection()->BaseAddress() + binaryFile.GetDataSection()->Address();
    Symbol* symbol = new Symbol(SymbolKind::global, std::string(), "@type_table_index", Value(entries.size(), ValueFlags::pure));
    symbol->SetStart(start);
    symbol->SetSegment(Segment::data);
    symbol->SetLinkage(Linkage::external);
    symbol->SetLength(end - start);
    binaryFile.GetSymbolTable().AddSymbol(symbol);

}

void TypeTableIndex::Read(SymbolTable* symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory)
{
    Symbol* symbol = symbolTable->GetSymbol("@type_table_index");
    if (symbol)
    {
        uint64_t address = symbol->Start();
        uint64_t count = symbol->GetValue().Val();
        for (uint64_t i = 0; i < count; ++i)
        {
            TypeTableIndexEntry entry;
            entry.Read(address, rv, memory, *symbolTable);
            entries.push_back(entry);
            address += entry.Size();
        }
    }
    else
    {
        throw std::runtime_error("'@type_table_index' symbol not found");
    }
}

int64_t TypeTableIndex::SearchTypeEntryAddress(int32_t typeId) const
{
    if (entries.empty()) return 0;
    TypeTableIndexEntry indexEntry;
    indexEntry.typeId = typeId;
    indexEntry.address = 0;
    auto it = std::lower_bound(entries.begin(), entries.end(), indexEntry);
    if (it == entries.end() && it != entries.begin())
    {
        --it;
    }
    if (it != entries.begin() && it->typeId > typeId)
    {
        --it;
    }
    if (it->typeId == typeId)
    {
        return it->address;
    }
    return 0;
}

TypeTable::TypeTable() : indexRead(false)
{
}

int32_t TypeTable::MakeInternalTypeId(int32_t externalTypeId, BinaryFile* binaryFile)
{
    if (IsFundamentalTypeId(externalTypeId))
    {
        return externalTypeId;
    }
    auto it = oxTypeIdMap.find(std::make_pair( externalTypeId, binaryFile));
    if (it != oxTypeIdMap.end())
    {
        return it->second;
    }
    if (IsPointerTypeId(externalTypeId))
    {
        int32_t externalBaseTypeId = GetBaseTypeId(externalTypeId);
        int32_t internalBaseTypeId = MakeInternalTypeId(externalBaseTypeId, binaryFile);
        oxTypeIdMap[std::make_pair(externalBaseTypeId, binaryFile)] = internalBaseTypeId;
        int32_t internalTypeId = MakeTypeId(internalBaseTypeId, GetPointerCount(externalTypeId));
        oxTypeIdMap[std::make_pair(externalTypeId, binaryFile)] = internalTypeId;
        return internalTypeId;
    }
    TypeInfoRecord* typeInfoRecord = binaryFile->GetDebugSection()->GetTypeInfoRecord(externalTypeId);
    auto sit = userTypeMap.find(typeInfoRecord->FullName());
    if (sit != userTypeMap.end())
    {
        Type* type = sit->second;
        return type->Id();
    }
    int32_t internalTypeId = userTypeId + static_cast<int32_t>(types.size());
    int8_t systemType = GetSystemType(externalTypeId);
    if (systemType != notSystemType)
    {
        SetSystemType(internalTypeId, systemType);
    }
    oxTypeIdMap[std::make_pair(externalTypeId, binaryFile)] = internalTypeId;
    if (typeInfoRecord->IsStructureTypeInfoRecord())
    {
        StructTypeInfoRecord* structureTypeInfoRecord = static_cast<StructTypeInfoRecord*>(typeInfoRecord);
        StructureType* structureType = new StructureType(internalTypeId, structureTypeInfoRecord->FullName(), structureTypeInfoRecord->Size());
        types.push_back(std::unique_ptr<Type>(structureType));
        userTypeMap[structureType->Name()] = structureType;
        for (const FieldInfoRecord& fieldInfoRecord : structureTypeInfoRecord->FieldInfoRecords())
        {
            int32_t internalFieldTypeId = MakeInternalTypeId(fieldInfoRecord.TypeId(), binaryFile);
            Field field(fieldInfoRecord.Name(), internalFieldTypeId, fieldInfoRecord.Offset());
            structureType->AddField(std::move(field));
        }
    }
    else if (typeInfoRecord->IsArrayTypeInfoRecord())
    {
        ArrayTypeInfoRecord* arrayTypeInfoRecord = static_cast<ArrayTypeInfoRecord*>(typeInfoRecord);
        ArrayType* arrayType = new ArrayType(internalTypeId, arrayTypeInfoRecord->FullName(), arrayTypeInfoRecord->ElementTypeId(), arrayTypeInfoRecord->Size());
        types.push_back(std::unique_ptr<Type>(arrayType));
        userTypeMap[arrayType->Name()] = arrayType;
    }
    else if (typeInfoRecord->IsFunctionTypeInfoRecord())
    {
        FunctionTypeInfoRecord* functionTypeInfoRecord = static_cast<FunctionTypeInfoRecord*>(typeInfoRecord);
        FunctionType* functionType = new FunctionType(internalTypeId, functionTypeInfoRecord->FullName());
        types.push_back(std::unique_ptr<Type>(functionType));
        userTypeMap[functionType->Name()] = functionType;
    }
    else
    {
        throw std::runtime_error("structure, array or function type expected");
    }
    return internalTypeId;
}

void TypeTable::AddStrings(StringTable* stringTable)
{
    for (const auto& type : types)
    {
        type->AddStrings(stringTable);
    }
}

void TypeTable::Write(BinaryFile& binaryFile)
{
    binaryFile.GetDataSection()->Align(8);
    int32_t count = static_cast<int32_t>(types.size());
    for (int32_t i = 0; i < count; ++i)
    {
        Type* type = types[i].get();
        int64_t address = binaryFile.GetDataSection()->BaseAddress() + binaryFile.GetDataSection()->Address();
        type->Write(binaryFile.GetDataSection());
        TypeTableIndexEntry indexEntry;
        indexEntry.typeId = type->Id();
        indexEntry.address = address;
        index.AddEntry(indexEntry);
    }
    index.Sort();
    index.Write(binaryFile);
    binaryFile.GetDataSection()->Align(8);
}

Type* TypeTable::GetFundamentalType(int32_t fundamentalTypeId) 
{
    switch (fundamentalTypeId)
    {
        case voidTypeId: return &voidType;
        case boolTypeId: return &boolType;
        case sbyteTypeId: return &sbyteType;
        case byteTypeId: return &byteType;
        case shortTypeId: return &shortType;
        case ushortTypeId: return &ushortType;
        case intTypeId: return &intType;
        case uintTypeId: return &uintType;
        case longTypeId: return &longType;
        case ulongTypeId: return &ulongType;
        case floatTypeId: return &floatType;
        case doubleTypeId: return &doubleType;
        case charTypeId: return &charType;
        case wcharTypeId: return &wcharType;
        case ucharTypeId: return &ucharType;
    }
    return nullptr;
}

Type* TypeTable::GetType(int32_t typeId, int64_t rv, cmajor::systemx::machine::Memory& memory, StringTable* stringTable, SymbolTable* symbolTable)
{
    int8_t systemTypeId = GetSystemType(typeId);
    if (systemTypeId != notSystemType)
    {
        auto sit = systemTypeMap.find(typeId);
        if (sit != systemTypeMap.end())
        {
            return sit->second;
        }
    }
    auto it = typeMap.find(typeId);
    if (it != typeMap.end())
    {
        return it->second;
    }
    if (IsFundamentalTypeId(typeId))
    {
        return GetFundamentalType(typeId);
    }
    if (IsPointerTypeId(typeId))
    {
        int32_t baseTypeId = GetBaseTypeId(typeId);
        Type* baseType = GetType(baseTypeId, rv, memory, stringTable, symbolTable);
        int8_t pointerCount = GetPointerCount(typeId);
        std::string name = baseType->Name();
        name.append(pointerCount, '*');
        PointerType* pointerType = new PointerType(typeId, name, pointerCount, baseType);
        typeMap[typeId] = pointerType;
        types.push_back(std::unique_ptr<Type>(pointerType));
        return pointerType;
    }
    if (!indexRead)
    {
        indexRead = true;
        index.Read(symbolTable, rv, memory);
    }
    int64_t address = index.SearchTypeEntryAddress(typeId);
    if (address != 0)
    {
        Type* type = ReadType(address, *symbolTable, *stringTable, rv, memory);
        if (systemTypeId != notSystemType)
        {
            types.push_back(std::unique_ptr<Type>(type));
            Type* systemType = MakeSystemType(systemTypeId, typeId, type);
            systemTypeMap[typeId] = systemType;
            types.push_back(std::unique_ptr<Type>(systemType));
            return systemType;
        }
        else
        {
            typeMap[typeId] = type;
        }
        types.push_back(std::unique_ptr<Type>(type));
        return type;
    }
    else
    {
        throw std::runtime_error("type " + std::to_string(typeId) + " not found from type table");
    }
}

} // namespace cmajor::systemx::object
