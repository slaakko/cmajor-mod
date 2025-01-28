// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.metadata;

import cmajor.systemx.intermediate.data;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::systemx::intermediate {

export namespace metadata {}

class MetadataStruct;
class Context;

enum class MetadataItemKind
{
    metadataRef, metadataBool, metadataLong, metadataString
};

class MetadataItem
{
public:
    MetadataItem(MetadataItemKind kind_);
    virtual ~MetadataItem();
    MetadataItemKind Kind() const { return kind; }
private:
    MetadataItemKind kind;
};

class MetadataRef : public MetadataItem
{
public:
    MetadataRef(const soul::ast::SourcePos& sourcePos_, int32_t nodeId_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    int32_t NodeId() const { return nodeId; }
    MetadataStruct* GetMetadataStruct() const { return metadataStruct; }
    void SetMetadataStruct(MetadataStruct* metadataStruct_) { metadataStruct = metadataStruct_; }
private:
    soul::ast::SourcePos sourcePos;
    int32_t nodeId;
    MetadataStruct* metadataStruct;
};

class MetadataBool : public MetadataItem
{
public:
    MetadataBool(bool value_);
    bool Value() const { return value; }
private:
    bool value;
};

class MetadataLong : public MetadataItem
{
public:
    MetadataLong(int64_t value_);
    int64_t Value() const { return value; }
private:
    int64_t value;
};

class MetadataString : public MetadataItem
{
public:
    MetadataString(const std::string& value_);
    const std::string& Value() const { return value; }
private:
    std::string value;
};

class MetadataStruct
{
public:
    MetadataStruct(const soul::ast::SourcePos& sourcePos_, int32_t id_);
    MetadataStruct(const MetadataStruct&) = delete;
    MetadataStruct& operator=(const MetadataStruct&) = delete;
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    int32_t Id() const { return id; }
    void AddItem(const std::string& fieldName, MetadataItem* item);
    MetadataItem* GetItem(const std::string& fieldName) const;
private:
    soul::ast::SourcePos sourcePos;
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
    MetadataStruct* GetMetadataStruct(int32_t id) const;
    MetadataStruct* AddMetadataStruct(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context);
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value);
    MetadataRef* CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId);
    void ResolveMetadataReferences(Context* context);
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

} // cmajor::systemx::intermediate
