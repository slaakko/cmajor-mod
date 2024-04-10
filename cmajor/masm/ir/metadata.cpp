// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.ir.metadata;

import cmajor.masm.ir.context;

namespace cmajor::masm::ir {

MetadataRef::MetadataRef(const soul::ast::Span& span_, int32_t nodeId_) :
    MetadataItem(MetadataItemKind::metadataRef), span(span_), nodeId(nodeId_), metadataStruct(nullptr)
{
}

void MetadataRef::Write(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(nodeId));
}

MetadataItem::MetadataItem(MetadataItemKind kind_) : kind(kind_)
{
}

MetadataItem::~MetadataItem()
{
}

MetadataBool::MetadataBool(bool value_) : MetadataItem(MetadataItemKind::metadataBool), value(value_)
{
}

void MetadataBool::Write(util::CodeFormatter& formatter)
{
    if (value)
    {
        formatter.Write("true");
    }
    else
    {
        formatter.Write("false");
    }
}

MetadataLong::MetadataLong(int64_t value_) : MetadataItem(MetadataItemKind::metadataLong), value(value_)
{
}

void MetadataLong::Write(util::CodeFormatter& formatter)
{
    formatter.Write(std::to_string(value));
}

MetadataString::MetadataString(const std::string& value_) : MetadataItem(MetadataItemKind::metadataString), value(value_)
{
}

void MetadataString::Write(util::CodeFormatter& formatter)
{
    formatter.Write("\"");
    formatter.Write(util::StringStr(value));
    formatter.Write("\"");
}

MetadataStruct::MetadataStruct(const soul::ast::Span& span_, int32_t id_) : MetadataItem(MetadataItemKind::metadataStruct), span(span_), id(id_)
{
}

void MetadataStruct::AddItem(const std::string& fieldName, MetadataItem* item)
{
    itemMap[fieldName] = item;
}

MetadataItem* MetadataStruct::GetItem(const std::string& fieldName) const
{
    auto it = itemMap.find(fieldName);
    if (it != itemMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void MetadataStruct::Write(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(id));
}

void MetadataStruct::WriteDefinition(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(id) + " = {");
    bool first = true;
    for (const auto& item : itemMap)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        formatter.Write(item.first);
        formatter.Write(": ");
        item.second->Write(formatter);
    }
    formatter.WriteLine("}");
}

Metadata::Metadata() : context(nullptr), trueItem(), falseItem()
{
}

MetadataStruct* Metadata::CreateMetadataStruct()
{
    return AddMetadataStruct(soul::ast::Span(), metadataNodes.size());
}

MetadataStruct* Metadata::GetMetadataStruct(int32_t id) const
{
    auto it = metadataMap.find(id);
    if (it != metadataMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

MetadataStruct* Metadata::AddMetadataStruct(const soul::ast::Span& span, int32_t id)
{
    MetadataStruct* prev = GetMetadataStruct(id);
    if (prev)
    {
        throw std::runtime_error("error adding metadata node: node id " + std::to_string(id) + " not unique");
    }
    MetadataStruct* metadataStruct = new MetadataStruct(span, id);
    metadataNodes.push_back(std::unique_ptr<MetadataStruct>(metadataStruct));
    metadataMap[metadataStruct->Id()] = metadataStruct;
    return metadataStruct;
}

MetadataBool* Metadata::CreateMetadataBool(bool value)
{
    if (value)
    {
        if (!trueItem)
        {
            trueItem = new MetadataBool(true);
            metadataItems.push_back(std::unique_ptr<MetadataItem>(trueItem));
        }
        return trueItem;
    }
    else
    {
        if (!falseItem)
        {
            falseItem = new MetadataBool(false);
            metadataItems.push_back(std::unique_ptr<MetadataItem>(falseItem));
        }
        return falseItem;
    }
}

MetadataLong* Metadata::CreateMetadataLong(int64_t value)
{
    auto it = longItemMap.find(value);
    if (it != longItemMap.cend())
    {
        return it->second;
    }
    else
    {
        MetadataLong* metadataLong = new MetadataLong(value);
        metadataItems.push_back(std::unique_ptr<MetadataItem>(metadataLong));
        longItemMap[value] = metadataLong;
        return metadataLong;
    }
}

MetadataString* Metadata::CreateMetadataString(const std::string& value, bool crop)
{
    std::string val = value;
    if (crop)
    {
        val = value.substr(1, value.length() - 2);
    }
    auto it = stringItemMap.find(val);
    if (it != stringItemMap.cend())
    {
        return it->second;
    }
    else
    {
        MetadataString* metadataString = new MetadataString(val);
        metadataItems.push_back(std::unique_ptr<MetadataItem>(metadataString));
        stringItemMap[val] = metadataString;
        return metadataString;
    }
}

MetadataRef* Metadata::CreateMetadataRef(const soul::ast::Span& span, int32_t nodeId)
{
    auto it = referenceMap.find(nodeId);
    if (it != referenceMap.cend())
    {
        return it->second;
    }
    else
    {
        MetadataRef* metadataRef = new MetadataRef(span, nodeId);
        referenceMap[metadataRef->NodeId()] = metadataRef;
        metadataItems.push_back(std::unique_ptr<MetadataItem>(metadataRef));
        metadataReferences.push_back(metadataRef);
        return metadataRef;
    }
}

void Metadata::ResolveMetadataReferences()
{
    for (MetadataRef* metadataRef : metadataReferences)
    {
        int32_t nodeId = metadataRef->NodeId();
        MetadataStruct* metadataNode = GetMetadataStruct(nodeId);
        if (metadataNode)
        {
            metadataRef->SetMetadataStruct(metadataNode);
        }
        else
        {
            throw std::runtime_error("error resolving metadata reference: node id " + std::to_string(nodeId) + " not found");
        }
    }
}

void Metadata::Write(util::CodeFormatter& formatter)
{
    if (metadataNodes.empty()) return;
    formatter.WriteLine();
    formatter.WriteLine("metadata");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& node : metadataNodes)
    {
        node->WriteDefinition(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

} // cmajor::masm::ir
