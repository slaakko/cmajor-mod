// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.ir.metadata;

import cmajor.systemx.ir.basic.block;
import util;

namespace cmajor::systemx::ir {

MDItem::MDItem(MDItemKind kind_) : kind(kind_)
{
}

MDItem::~MDItem()
{
}

MDBool::MDBool(bool value_) : MDItem(MDItemKind::bool_), value(value_)
{
}

void MDBool::Write(util::CodeFormatter& formatter)
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

MDLong::MDLong(int64_t value_) : MDItem(MDItemKind::long_), value(value_)
{
}

void MDLong::Write(util::CodeFormatter& formatter)
{
    formatter.Write(std::to_string(value));
}

MDString::MDString(const std::string& value_) : MDItem(MDItemKind::string), value(value_)
{
}

void MDString::Write(util::CodeFormatter& formatter)
{
    formatter.Write("\"");
    formatter.Write(util::StringStr(value));
    formatter.Write("\"");
}

MDStructRef::MDStructRef(int id_) : MDItem(MDItemKind::structRef), id(id_)
{
}

void MDStructRef::Write(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(id));
}

MDStruct::MDStruct(int id_) : MDItem(MDItemKind::struct_), id(id_)
{
}

void MDStruct::Write(util::CodeFormatter& formatter)
{
    formatter.Write("!" + std::to_string(id));
}

void MDStruct::WriteDefinition(util::CodeFormatter& formatter)
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
        formatter.Write(": "),
            item.second->Write(formatter);
    }
    formatter.WriteLine("}");
}

void MDStruct::AddItem(const std::string& fieldName, MDItem* item)
{
    items.push_back(std::make_pair(fieldName, item));
}

MDArray::MDArray() : MDItem(MDItemKind::array)
{
}

void MDArray::AddItem(MDItem* item)
{
    items.push_back(item);
}

void MDArray::Write(util::CodeFormatter& formatter)
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

MDBasicBlockRef::MDBasicBlockRef(void* bb_) : MDItem(MDItemKind::basicBlockRef), bb(bb_)
{
}

void MDBasicBlockRef::Write(util::CodeFormatter& formatter)
{
    BasicBlock* basicBlock = static_cast<BasicBlock*>(bb);
    formatter.Write(std::to_string(basicBlock->Id()));
}

Metadata::Metadata() : mdTrue(true), mdFalse(false)
{
}

MDBool* Metadata::CreateMDBool(bool value)
{
    if (value)
    {
        return &mdTrue;
    }
    else
    {
        return &mdFalse;
    }
}

MDLong* Metadata::CreateMDLong(int64_t value)
{
    MDLong* item = new MDLong(value);
    AddItem(item);
    return item;
}

MDString* Metadata::CreateMDString(const std::string& value)
{
    MDString* item = new MDString(value);
    AddItem(item);
    return item;
}

MDArray* Metadata::CreateMDArray()
{
    MDArray* item = new MDArray();
    AddItem(item);
    return item;
}

void Metadata::AddItem(MDItem* item)
{
    items.push_back(std::unique_ptr<MDItem>(item));
}

MDStructRef* Metadata::CreateMDStructRef(int id)
{
    MDStructRef* item = new MDStructRef(id);
    AddItem(item);
    return item;
}

MDStruct* Metadata::CreateMDStruct()
{
    MDStruct* item = new MDStruct(structs.size());
    structs.push_back(std::unique_ptr<MDStruct>(item));
    return item;
}

MDBasicBlockRef* Metadata::CreateMDBasicBlockRef(void* bb)
{
    MDBasicBlockRef* item = new MDBasicBlockRef(bb);
    AddItem(item);
    return item;
}

void Metadata::Write(util::CodeFormatter& formatter)
{
    if (structs.empty()) return;
    formatter.WriteLine();
    formatter.WriteLine("metadata");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& s : structs)
    {
        s->WriteDefinition(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

} // namespace cmajor::systemx::ir
