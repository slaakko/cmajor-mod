// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.entity.reference;

import std.core;
import soul.xml.character.data;

export namespace soul::xml {

class EntityReference : public CharacterData
{
public:
    EntityReference(const soul::ast::SourcePos& sourcePos_);
    EntityReference(const soul::ast::SourcePos& sourcePos_, const std::string& entityRef_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

EntityReference* MakeEntityReference(const std::string& entityRef);

} // namespace soul::xml
