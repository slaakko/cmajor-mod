// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.metadata;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.code;

namespace cmajor::systemx::intermediate {

MetadataRef::MetadataRef(const soul::ast::SourcePos& sourcePos_, int32_t nodeId_) : 
    MetadataItem(MetadataItemKind::metadataRef), sourcePos(sourcePos_), nodeId(nodeId_), metadataStruct(nullptr)
{
}

void MetadataRef::Write(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(nodeId));
}

MetadataBasicBlockRef::MetadataBasicBlockRef(BasicBlock* bb_) : MetadataItem(MetadataItemKind::metadataBasicBlockRef), bb(bb_)
{
}

void MetadataBasicBlockRef::Write(util::CodeFormatter& formatter)
{
    formatter.Write(std::to_string(bb->Id()));
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

MetadataArray::MetadataArray() : MetadataItem(MetadataItemKind::metadataArray)
{
}

void MetadataArray::AddItem(MetadataItem* item)
{
    items.push_back(item);
}

void MetadataArray::Write(util::CodeFormatter& formatter)
{
    formatter.Write("[");
    bool first = true;
    for (const auto& item : items)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        item->Write(formatter);
    }
    formatter.Write("]");
}

MetadataStruct::MetadataStruct(const soul::ast::SourcePos& sourcePos_, int32_t id_) : sourcePos(sourcePos_), id(id_)
{
}

void MetadataStruct::AddItem(const std::string& fieldName, MetadataItem* item)
{
    itemMap[fieldName] = item;
    items.push_back(std::make_pair(fieldName, item));
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
    for (const auto& item : items)
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

MetadataStruct* Metadata::CreateMetadataStruct()
{
    MetadataStruct* metadataStruct = new MetadataStruct(soul::ast::SourcePos(), metadataNodes.size());
    metadataNodes.push_back(std::unique_ptr<MetadataStruct>(metadataStruct));
    metadataMap[metadataStruct->Id()] = metadataStruct;
    return metadataStruct;
}

MetadataStruct* Metadata::AddMetadataStruct(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context)
{
    MetadataStruct* prev = GetMetadataStruct(id);
    if (prev)
    {
        Error("error adding metadata node: node id " + std::to_string(id) + " not unique", sourcePos, context, prev->GetSourcePos());
    }
    MetadataStruct* metadataStruct = new MetadataStruct(sourcePos, id);
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

MetadataArray* Metadata::CreateMetadataArray()
{
    MetadataArray* metadataArray = new MetadataArray();
    metadataItems.push_back(std::unique_ptr<MetadataItem>(metadataArray));
    return metadataArray;
}

MetadataRef* Metadata::CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId)
{
    auto it = referenceMap.find(nodeId);
    if (it != referenceMap.cend())
    {
        return it->second;
    }
    else
    {
        MetadataRef* metadataRef = new MetadataRef(sourcePos, nodeId);
        referenceMap[metadataRef->NodeId()] = metadataRef;
        metadataItems.push_back(std::unique_ptr<MetadataItem>(metadataRef));
        metadataReferences.push_back(metadataRef);
        return metadataRef;
    }
}

MetadataBasicBlockRef* Metadata::CreateMetadataBasicBlockRef(BasicBlock* bb)
{
    MetadataBasicBlockRef* bbRef = new MetadataBasicBlockRef(bb);
    metadataItems.push_back(std::unique_ptr<MetadataItem>(bbRef));
    return bbRef;
}

void Metadata::ResolveMetadataReferences(Context* context)
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
            Error("error resolving metadata reference: node id " + std::to_string(nodeId) + " not found", metadataRef->GetSourcePos(), context);
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

} // cmajor::systemx::intermediate
