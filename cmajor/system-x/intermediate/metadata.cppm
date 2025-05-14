// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.metadata;

import cmajor.systemx.intermediate.data;
import soul.ast.source.pos;
import util;
import std.core;

export namespace cmajor::systemx::intermediate {

export namespace metadata {}

const int64_t fileInfoNodeType = 0;
const int64_t funcInfoNodeType = 1;
const int64_t lineInfoNodeType = 2;
const int64_t cfgNodeType = 3;
const int64_t beginTryNodeType = 4;
const int64_t endTryNodeType = 5;
const int64_t catchNodeType = 6;
const int64_t beginCleanupNodeType = 7;
const int64_t endCleanupNodeType = 8;

class MetadataStruct;
class Context;
class BasicBlock;

enum class MetadataItemKind
{
    metadataRef, metadataBasicBlockRef, metadataBool, metadataLong, metadataString, metadataArray
};

class MetadataItem
{
public:
    MetadataItem(MetadataItemKind kind_);
    virtual ~MetadataItem();
    MetadataItemKind Kind() const { return kind; }
    bool IsMetadataRef() const { return kind == MetadataItemKind::metadataRef; }
    bool IsMetadataBasicBlockRef() const { return kind == MetadataItemKind::metadataBasicBlockRef; }
    bool IsMetadataBool() const { return kind == MetadataItemKind::metadataBool; }
    bool IsMetadataLong() const { return kind == MetadataItemKind::metadataLong; }
    bool IsMetadataString() const { return kind == MetadataItemKind::metadataString; }
    bool IsMetadataArray() const { return kind == MetadataItemKind::metadataArray; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
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
    void Write(util::CodeFormatter& formatter) override;
private:
    soul::ast::SourcePos sourcePos;
    int32_t nodeId;
    MetadataStruct* metadataStruct;
};

class MetadataBasicBlockRef : public MetadataItem
{
public:
    MetadataBasicBlockRef(BasicBlock* bb_);
    void Write(util::CodeFormatter& formatter) override;
private:
    BasicBlock* bb;
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

class MetadataArray : public MetadataItem
{
public:
    MetadataArray();
    void AddItem(MetadataItem* item);
    int ItemCount() const { return static_cast<int>(items.size()); }
    MetadataItem* GetItem(int index) const { return items[index]; }
    void Write(util::CodeFormatter& formatter) override;
private:
    std::vector<MetadataItem*> items;
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
    void Write(util::CodeFormatter& formatter);
    void WriteDefinition(util::CodeFormatter& formatter);
private:
    soul::ast::SourcePos sourcePos;
    int32_t id;
    std::map<std::string, MetadataItem*> itemMap;
    std::vector<std::pair<std::string, MetadataItem*>> items;
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
    MetadataStruct* CreateMetadataStruct();
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value, bool crop);
    MetadataArray* CreateMetadataArray();
    MetadataRef* CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId);
    MetadataBasicBlockRef* CreateMetadataBasicBlockRef(BasicBlock* bb);
    void ResolveMetadataReferences(Context* context);
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

} // cmajor::systemx::intermediate
