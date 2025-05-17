// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.type.table;

import cmajor.systemx.machine;
import cmajor.systemx.object.type;
import std.core;

export namespace cmajor::systemx::object {

class BinaryFile;
class Section;
class StringTable;
class SymbolTable;

struct TypeTableIndexEntry
{
    TypeTableIndexEntry();
    void Write(Section* section);
    int64_t Size() const { return 16; }
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable);
    int32_t typeId;
    int64_t address;
};

bool operator<(const TypeTableIndexEntry& left, const TypeTableIndexEntry& right);

class TypeTableIndex
{
public:
    TypeTableIndex();
    void AddEntry(const TypeTableIndexEntry& entry);
    void Sort();
    void Write(BinaryFile& binaryFile);
    void Read(SymbolTable* symbolTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);
    int64_t SearchTypeEntryAddress(int32_t typeId) const;
private:
    std::vector<TypeTableIndexEntry> entries;
};

class TypeTable
{
public:
    TypeTable();
    int32_t MakeInternalTypeId(int32_t externalTypeId, BinaryFile* binaryFile);
    void AddStrings(StringTable* stringTable);
    void Write(BinaryFile& binaryFile);
    Type* GetFundamentalType(int32_t fundamentalTypeId);
    Type* GetType(int32_t typeId, int64_t rv, cmajor::systemx::machine::Memory& memory, StringTable* stringTable, SymbolTable* symbolTable);
private:
    VoidType voidType;
    BoolType boolType;
    SByteType sbyteType;
    ByteType byteType;
    ShortType shortType;
    UShortType ushortType;
    IntType intType;
    UIntType uintType;
    LongType longType;
    ULongType ulongType;
    FloatType floatType;
    DoubleType doubleType;
    CharType charType;
    WCharType wcharType;
    UCharType ucharType;
    std::map<std::pair<int32_t, BinaryFile*>, int32_t> oxTypeIdMap;
    std::map<std::string, Type*> userTypeMap;
    std::map<int32_t, Type*> typeMap;
    std::map<int32_t, Type*> systemTypeMap;
    std::vector<std::unique_ptr<Type>> types;
    bool indexRead;
    TypeTableIndex index;
};

} // namespace cmajor::systemx::object
