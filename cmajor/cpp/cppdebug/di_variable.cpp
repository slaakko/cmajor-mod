// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.di.variable;

import cmajor.debug.debug.info;
import util;

namespace cmajor::debug {

DIVariable::DIVariable(Kind kind_) : kind(kind_), initLineNumber(-1), name(), typeId(), project(nullptr)
{
}

DIVariable::~DIVariable()
{
}

void DIVariable::SetInitLineNumber(int32_t initLineNumber_)
{
    initLineNumber = initLineNumber_;
}

std::string DIVariable::KindStr(Kind kind)
{
    switch (kind)
    {
    case Kind::localVariable: return "local";
    case Kind::memberVariable: return "member";
    }
    return std::string();
}

void DIVariable::SetName(const std::string& name_)
{
    name = name_;
}

void DIVariable::SetIrName(const std::string& irName_)
{
    irName = irName_;
}

void DIVariable::SetTypeId(const util::uuid& typeId_)
{
    typeId = typeId_;
}

DIType* DIVariable::GetType() const
{
    if (project)
    {
        return project->GetType(typeId);
    }
    else
    {
        throw std::runtime_error("internal error: project of variable not set");
    }
}

void DIVariable::Write(util::BinaryStreamWriter& writer)
{
    writer.Write(static_cast<int8_t>(kind));
    writer.Write(initLineNumber);
    writer.Write(name);
    writer.Write(irName);
    writer.Write(typeId);
}

void DIVariable::Read(util::BinaryStreamReader& reader)
{
    kind = static_cast<Kind>(reader.ReadSByte());
    initLineNumber = reader.ReadInt();
    name = reader.ReadUtf8String();
    irName = reader.ReadUtf8String();
    reader.ReadUuid(typeId);
}

std::unique_ptr<util::JsonValue> DIVariable::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    jsonObject->AddField(U"name", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(name))));
    jsonObject->AddField(U"irName", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(irName))));
    jsonObject->AddField(U"typeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(typeId)))));
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

std::unique_ptr<soul::xml::Element> DIVariable::ToXml() const
{
    soul::xml::Element* variableElement = soul::xml::MakeElement("variable");
    variableElement->SetAttribute("name", name);
    variableElement->SetAttribute("irName", irName);
    variableElement->SetAttribute("typeId", util::ToString(typeId));
    return std::unique_ptr<soul::xml::Element>(variableElement);
}

} // namespace cmajor::debug
