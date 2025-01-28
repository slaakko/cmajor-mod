// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.conversion.table;

import cmajor.symbols.type.symbol;
import soul.ast.span;
import std.core;

export namespace cmajor::symbols {

class Module;
class FunctionSymbol;

struct ConversionTableEntry
{
    ConversionTableEntry(TypeSymbol* sourceType_, TypeSymbol* targetType_);
    void CheckValid() const;
    TypeSymbol* sourceType;
    TypeSymbol* targetType;
};

inline bool operator==(const ConversionTableEntry& left, const ConversionTableEntry& right)
{
    return TypesEqual(left.sourceType, right.sourceType) && TypesEqual(left.targetType, right.targetType);
}

struct ConversionTableEntryHash
{
    size_t operator()(const ConversionTableEntry& entry) const
    {
        return util::HashValue(entry.sourceType->TypeId()) ^ util::HashValue(entry.targetType->TypeId());
    }
};

class ConversionTable
{
public:
    enum class Owner
    {
        compileUnit, symbolTable
    };
    ConversionTable(Owner owner_, Module* module_);
    ConversionTable(const ConversionTable&) = delete;
    ConversionTable& operator=(const ConversionTable&) = delete;
    void AddConversion(FunctionSymbol* conversion);
    FunctionSymbol* GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType) const;
    void AddGeneratedConversion(std::unique_ptr<FunctionSymbol>&& generatedConversion);
    void Add(const ConversionTable& that);
    void Check();
private:
    Owner owner;
    Module* module;
    std::unordered_map<ConversionTableEntry, FunctionSymbol*, ConversionTableEntryHash> conversionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> generatedConversions;
};
} // namespace cmajor::symbols
