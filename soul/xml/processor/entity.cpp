// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.processor.entity;

namespace soul::xml::processor {

Entity::Entity(EntityKind kind_, const std::string& name_) : kind(kind_), name(name_)
{
}

Entity::~Entity()
{
}

InternalEntity::InternalEntity(const std::string& name_, const std::u32string& value_) : Entity(EntityKind::internalEntity, name_), value(value_)
{
}

Entities::Entities()
{
    AddEntity(new InternalEntity("quot", U"\""));
    AddEntity(new InternalEntity("amp", U"&"));
    AddEntity(new InternalEntity("apos", U"'"));
    AddEntity(new InternalEntity("lt", U"<"));
    AddEntity(new InternalEntity("gt", U">"));
}

Entities& Entities::Instance()
{
    static Entities instance;
    return instance;
}

void Entities::AddEntity(Entity* entity)
{
    entities.push_back(std::unique_ptr<Entity>(entity));
    entityMap[entity->Name()] = entity;
}

Entity* Entities::GetEntity(const std::string& entityName) const
{
    auto it = entityMap.find(entityName);
    if (it != entityMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

} // namespace soul::xml::processor
