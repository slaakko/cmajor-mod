// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.di.variable;

import util;
import std.core;
import soul.xml.dom;

export namespace cmajor::debug {

class Project;
class DIType;

class DIVariable
{
public:
    enum class Kind : int8_t
    {
        localVariable, memberVariable
    };
    DIVariable(Kind kind_);
    virtual ~DIVariable();
    void SetInitLineNumber(int32_t initLineNumber_);
    int32_t GetInitLineNumber() const { return initLineNumber; }
    Kind GetKind() const { return kind; }
    static std::string KindStr(Kind kind);
    void SetProject(Project* project_) { project = project_; }
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    const std::string& IrName() const { return irName; }
    void SetIrName(const std::string& irName_);
    const util::uuid& TypeId() const { return typeId; }
    void SetTypeId(const util::uuid& typeId_);
    DIType* GetType() const;
    std::unique_ptr<util::JsonValue> ToJson() const;
    std::unique_ptr<soul::xml::Element> ToXml() const;
private:
    Kind kind;
    int32_t initLineNumber;
    std::string name;
    std::string irName;
    util::uuid typeId;
    Project* project;
};

} // namespace cmajor::debug
