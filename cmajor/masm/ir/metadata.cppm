// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.metadata;

import cmajor.masm.ir.data;
import util;
import soul.ast.span;
import std.core;

 export namespace cmajor::masm::ir {

export namespace metadata {}

class MetadataStruct;
class Context;

enum class MetadataItemKind
{
    metadataRef, metadataBool, metadataLong, metadataString, metadataStruct
};

class MetadataItem
{
public:
    MetadataItem(MetadataItemKind kind_);
    virtual ~MetadataItem();
    MetadataItemKind Kind() const { return kind; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    MetadataItemKind kind;
};

class MetadataRef : public MetadataItem
{
public:
    MetadataRef(const soul::ast::Span& span_, int32_t nodeId_);
    const soul::ast::Span& GetSpan() const { return span; }
    int32_t NodeId() const { return nodeId; }
    MetadataStruct* GetMetadataStruct() const { return metadataStruct; }
    void SetMetadataStruct(MetadataStruct* metadataStruct_) { metadataStruct = metadataStruct_; }
    void Write(util::CodeFormatter& formatter) override;
private:
    soul::ast::Span span;
    int32_t nodeId;
    MetadataStruct* metadataStruct;
};

class MetadataBool : public MetadataItem
{
public:
    MetadataBool(bool value_);
    bool Value() const { return value; }
    void Write(util::CodeFormatter& formatter) override;
private:
    bool value;
};

class MetadataLong : public MetadataItem
{
public:
    MetadataLong(int64_t value_);
    int64_t Value() const { return value; }
    void Write(util::CodeFormatter& formatter) override;
private:
    int64_t value;
};

class MetadataString : public MetadataItem
{
public:
    MetadataString(const std::string& value_);
    const std::string& Value() const { return value; }
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string value;
};

class MetadataStruct : public MetadataItem
{
public:
    MetadataStruct(const soul::ast::Span& span_, int32_t id_);
    MetadataStruct(const MetadataStruct&) = delete;
    MetadataStruct& operator=(const MetadataStruct&) = delete;
    const soul::ast::Span& GetSpan() const { return span; }
    int32_t Id() const { return id; }
    void AddItem(const std::string& fieldName, MetadataItem* item);
    MetadataItem* GetItem(const std::string& fieldName) const;
    void Write(util::CodeFormatter& formatter) override;
    void WriteDefinition(util::CodeFormatter& formatter);
private:
    soul::ast::Span span;
    int32_t id;
    std::map<std::string, MetadataItem*> itemMap;
};

class Metadata
{
public:
    Metadata();
    Metadata(const Metadata&) = delete;
    Metadata& operator=(const Metadata&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    MetadataStruct* CreateMetadataStruct();
    MetadataStruct* GetMetadataStruct(int32_t id) const;
    MetadataStruct* AddMetadataStruct(const soul::ast::Span& span, int32_t id);
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value, bool crop);
    MetadataRef* CreateMetadataRef(const soul::ast::Span& span, int32_t nodeId);
    void ResolveMetadataReferences();
    void Write(util::CodeFormatter& formatter);
private:
    Context* context;
    std::vector<std::unique_ptr<MetadataStruct>> metadataNodes;
    std::vector<std::unique_ptr<MetadataItem>> metadataItems;
    std::map<int32_t, MetadataStruct*> metadataMap;
    MetadataBool* trueItem;
    MetadataBool* falseItem;
    std::map<int64_t, MetadataLong*> longItemMap;
    std::map<std::string, MetadataString*> stringItemMap;
    std::map<int32_t, MetadataRef*> referenceMap;
    std::vector<MetadataRef*> metadataReferences;
};

} // cmajor::masm::ir
