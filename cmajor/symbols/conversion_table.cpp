// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.conversion.table;

import soul.ast.span;
import cmajor.symbols.modules;
import cmajor.symbols.function.symbol;
import util;

namespace cmajor::symbols {

ConversionTableEntry::ConversionTableEntry(TypeSymbol* sourceType_, TypeSymbol* targetType_) : sourceType(sourceType_), targetType(targetType_)
{
#ifdef VALID_CONVERSION_TABLE_ENTRY_CHECK
    CheckValid();
#endif
}

ConversionTable::ConversionTable(Owner owner_, Context* context_, Module* module_) : owner(owner_), context(context_), module(module_)
{
}

void ConversionTableEntry::CheckValid() const
{
    if (!sourceType || !targetType)
    {
        throw std::runtime_error("invalid conversion table entry: source type is null or target type is null");
    }
    if (!sourceType->BaseType() || !targetType->BaseType())
    {
        throw std::runtime_error("invalid conversion table entry: source base type is null or target base type is null");
    }
}

void ConversionTable::AddConversion(FunctionSymbol* conversion)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module && module->IsImmutable())
    {
        throw ModuleImmutableException(context->RootModule(), module, soul::ast::Span(), soul::ast::Span());
    }
#endif
    ConversionTableEntry entry(conversion->ConversionSourceType(), conversion->ConversionTargetType());
#ifdef VALID_CONVERSION_TABLE_ENTRY_CHECK
    entry.CheckValid();
#endif
    conversionMap.erase(entry);
    conversionMap.insert(std::make_pair(entry, conversion));
}

void ConversionTable::Add(const ConversionTable& that)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module && module->IsImmutable())
    {
        throw ModuleImmutableException(context->RootModule(), module, soul::ast::Span(), soul::ast::Span());
    }
#endif
    for (const auto& p : that.conversionMap)
    {
        ConversionTableEntry entry(p.first);
#ifdef VALID_CONVERSION_TABLE_ENTRY_CHECK
        entry.CheckValid();
#endif
        conversionMap[entry] = p.second;
    }
}

void ConversionTable::Check()
{
    for (const auto& p : conversionMap)
    {
        const ConversionTableEntry& entry = p.first;
        entry.CheckValid();
    }
}

FunctionSymbol* ConversionTable::GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType, Context* context) const
{
    TypeSymbol* sourcePlainType = sourceType->PlainType(context);
    TypeSymbol* targetPlainType = targetType->PlainType(context);
    ConversionTableEntry entry(sourcePlainType, targetPlainType);
    auto it = conversionMap.find(entry);
    if (it != conversionMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

void ConversionTable::AddGeneratedConversion(std::unique_ptr<FunctionSymbol>&& generatedConversion)
{
    generatedConversions.push_back(std::move(generatedConversion));
}

} // namespace cmajor::symbols
