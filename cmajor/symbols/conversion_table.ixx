module;
#include <boost/uuid/uuid.hpp>
export module cmajor.symbols.conversion.table;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

/*
#include <cmajor/symbols/TypeSymbol.hpp>
#include <cmajor/symbols/FunctionSymbol.hpp>
#include <boost/functional/hash.hpp>
*/

import cmajor.symbols.type.symbol;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::symbols {

class Module;
//class TypeSymbol;
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
/* TODO
struct ConversionTableEntryHash
{
    size_t operator()(const ConversionTableEntry& entry) const
    {
        return boost::hash<boost::uuids::uuid>()(entry.sourceType->TypeId()) ^ boost::hash<boost::uuids::uuid>()(entry.targetType->TypeId());
    }
};
*/
struct ConversionTableEntryLess
{
    bool operator()(const ConversionTableEntry& left, const ConversionTableEntry& right) const
    {
        if (left.sourceType < right.sourceType)
        {
            return true;
        }
        else if (left.sourceType > right.sourceType)
        {
            return false;
        }
        else
        {
            return left.targetType < right.targetType;
        }
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
    FunctionSymbol* GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) const;
    void AddGeneratedConversion(std::unique_ptr<FunctionSymbol>&& generatedConversion);
    void Add(const ConversionTable& that);
    void Check();
private:
    Owner owner;
    Module* module;
    // TODO
    //std::unordered_map<ConversionTableEntry, FunctionSymbol*, ConversionTableEntryHash> conversionMap;
    std::map<ConversionTableEntry, FunctionSymbol*, ConversionTableEntryLess> conversionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> generatedConversions;
};
} // namespace cmajor::symbols
