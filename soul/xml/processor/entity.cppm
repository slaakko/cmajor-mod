// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.processor.entity;

import std.core;
import soul.ast.source.pos;

export namespace soul::xml::processor {

enum class EntityKind
{
    internalEntity, externalEntity
};

class Entity
{
public:
    Entity(EntityKind kind_, const std::string& name_);
    virtual ~Entity();
    EntityKind Kind() const { return kind; }
    const std::string& Name() const { return name; }
    virtual const std::u32string& Value() const = 0;
    bool IsInternal() const { return kind == EntityKind::internalEntity; }
    bool IsExternal() const { return kind == EntityKind::externalEntity; }
private:
    EntityKind kind;
    std::string name;
};

class InternalEntity : public Entity
{
public:
    InternalEntity(const std::string& name_, const std::u32string& value_);
    virtual const std::u32string& Value() const override { return value; }
private:
    std::u32string value;
};

class Entities
{
public:
    static Entities& Instance();
    Entities();
    void AddEntity(Entity* entity);
    Entity* GetEntity(const std::string& entityName) const;
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::map<std::string, Entity*> entityMap;
};

} // namespace soul::xml::processor
