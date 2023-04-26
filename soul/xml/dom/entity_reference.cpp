// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.entity.reference;

import soul.xml.visitor;

namespace soul::xml {

EntityReference::EntityReference(const soul::ast::SourcePos& sourcePos_) : 
    CharacterData(NodeKind::entityReferenceNode, sourcePos_, "entity_reference")
{
}

EntityReference::EntityReference(const soul::ast::SourcePos& sourcePos_, const std::string& entityRef_) : 
    CharacterData(NodeKind::entityReferenceNode, sourcePos_, "entity_reference", entityRef_)
{
}

void EntityReference::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EntityReference::Write(util::CodeFormatter& formatter)
{
    formatter.Write("&");
    formatter.Write(Data());
    formatter.Write(";");
}

EntityReference* MakeEntityReference(const std::string& entityRef)
{
    return new EntityReference(soul::ast::SourcePos(), entityRef);
}

} // namespace soul::xml
